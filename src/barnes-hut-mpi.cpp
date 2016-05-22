#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <ctime>
#include <mpi.h>

#include "Configuration.cpp"
#include "Body.cpp"
#include "Node.cpp"
#include "Quadtree.cpp"
#include "ParallelQuadtree.cpp"

#define AU 1.4960e11
#define day 24*60*60
#define G 6.674e-11

// These variables {VAR} can be written in the makefile with -D{VAR}
//#define DEBUG
//#define WRITE_OUTPUT
//#define WRITE_TIME
//#define WRITE_QUADTREE

using namespace std;

int main(int argc, char* argv[]) {

  // INIT MPI
  MPI_Init(&argc, &argv);
  int myRank;
  int nbrProcs;
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &nbrProcs);

  #ifdef DEBUG
    cout << "Process " << myRank << " on " << nbrProcs << " says HELLO." << endl;
  #endif

  // Define different variables
  string fileName;
  double dt;
  double finalTime;
  int samplingFreq = 0;
  string initialFile;
  string outputFileName;
  double maxSize;
  double minSize;
  double theta;
  int iteration = 0;

  // Define variables that will be set using ConfigFile
  int nbrBodies;
  vector<double> positions;
  vector<double> velocities;
  vector<double> mass;

  vector<double> localData;
  vector<double> globalData;

  #ifdef WRITE_TIME
    // Time variables
    double startTotal = MPI_Wtime();
    double startSimulation = 0.0;
    double startSend = 0.0;
    double startTimeIteration = 0.0;
    double startBuilding = 0.0;
    double iterationTime = 0.0;
    double buildingTime = 0.0;
  #endif

  // Streams
  ofstream outputTimeFile;
  ofstream outputFile;

  // Get the configurations
  if(myRank == 0) {

    if(argc > 1) {
      fileName.append(argv[1]);
    } else {
      fileName = "config_bh_parallel.init";
    }

    Configuration conf(fileName);

    dt = conf.get<double>("dt");
    finalTime = conf.get<double>("finalTime");
    #if WRITE_OUTPUT || WRITE_TIME || WRITE_QUADTREE
      samplingFreq = conf.get<int>("samplingFreq");
    #endif
    initialFile = conf.get<string>("initialFile");
    outputFileName = conf.get<string>("outputFile");
    maxSize = conf.get<double>("maxSize");
    minSize = conf.get<double>("minSize");
    theta = conf.get<double>("theta");

    #ifdef WRITE_TIME
      size_t pos = outputFileName.rfind(".");
      string outputTimeFileName = outputFileName.substr(0,pos) + "_time" + outputFileName.substr(pos);
      outputTimeFile.open(outputTimeFileName.c_str());
      outputTimeFile << "# Info, Time [s]" << endl;
    #endif

    conf.prepareInitialValues(initialFile);
    // Get the masses, positions and velocities for each object.
    nbrBodies = conf.getNbrBodies();
    mass = conf.getInitialMass();
    positions = conf.getInitialPositions();
    velocities = conf.getInitialVelocities();

    if(nbrBodies < nbrProcs) {
      cerr << "Number of bodies too small. The number of bodies needs to be equal or bigger than the number of process." << endl;
      exit(1);
    }

    #ifdef DEBUG
      cout << "Number of bodies: " << nbrBodies << endl;
    #endif

    #ifdef WRITE_OUTPUT
      outputFile.open(outputFileName.c_str());
      outputFile.precision(12);
      outputFile << "# Time, Mass, X position, Y position" << endl;
      for (int i=0; i<nbrBodies; i++) {
        outputFile << 0 << ", " << fixedMass[i] << ", " << fixedPositions[2*i]/AU << ", " << fixedPositions[2*i+1]/AU << endl;
      }
      outputFile.close();
    #endif

    #ifdef WRITE_TIME
      outputTimeFile << "Loading data, " << MPI_Wtime() - startTotal << endl;
      startSend = MPI_Wtime();
    #endif
  }

  // Broadcast Some values to all the process
  MPI_Bcast(&nbrBodies, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&dt, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&finalTime, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&maxSize, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&minSize, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&theta, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // Resize the vectors mass, positions and velocities for all the ranks except 0
  if (myRank != 0) {
    mass.resize(nbrBodies);
    positions.resize(2*nbrBodies);
    velocities.resize(2*nbrBodies);
  }

  // Now we send the data to all the process
  MPI_Bcast(&mass[0], nbrBodies, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&positions[0], 2*nbrBodies, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&velocities[0], 2*nbrBodies, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  #ifdef DEBUG
    cout << "Process " << myRank << " on " << nbrProcs << " says RECEIVEDBCAST." << endl;
  #endif

  if(myRank == 0) {
    #ifdef WRITE_TIME
      outputTimeFile << "Sending data, " << MPI_Wtime() - startSend << endl;
      startBuilding = MPI_Wtime();
    #endif
  }

  // Now, we build the first Quadtree
  ParallelQuadtree pqtree(0.0, 0.0, maxSize*AU, minSize*AU, day*dt, theta, nbrProcs);
  for(int i=0; i<nbrBodies; i++) {
    Body body(mass[i], positions[2*i], positions[2*i+1], velocities[2*i], velocities[2*i+1], i);
    if (pqtree.checkIfBodyIsLost(body)) {
      #ifdef DEBUG
        if(myRank == 0) {
          cout << "Body " << i << " is lost in the space! (" << positions[2*i]/AU << ", " << positions[2*i+1]/AU << ")" << endl;
        }
      #endif
    } else {
      pqtree.insertBody(body, pqtree.root);
    }
  }

  // Update the number of bodies
  nbrBodies = pqtree.root.nbrBodies;
  globalData.resize(nbrBodies);

  if(myRank == 0) {
    #ifdef WRITE_TIME
      outputTimeFile << "Bulding Tree, " << MPI_Wtime() - startBuilding << endl;
      startSimulation = MPI_Wtime();
    #endif
  }

  int *nbrBodiesPerNode = new int[nbrProcs];
  int *startIndex = new int[nbrProcs];
  int *recvCounts = new int[nbrProcs];

  for(double t=0; t+dt<finalTime; t+=dt) {

    if(myRank == 0) {
      #ifdef DEBUG
        cout << "Start loops for time " << t+dt << endl;
      #endif

      #ifdef WRITE_TIME
        if((iteration+1)%samplingFreq == 0) {
          startTimeIteration = MPI_Wtime();
        }
      #endif
    }

    // First, we need to do the Load Balancing and check how many bodies are given to each process
    vector< vector< Node *> > assignedNodes(nbrProcs);
    assignedNodes = pqtree.localNodes(nbrBodiesPerNode);

    #ifdef DEBUG
      if(myRank == 0) {
        for(int i=0; i<nbrProcs; i++) {
          cout << "Rank " << i << ", nbrBodiesPerNode = " << nbrBodiesPerNode[i] << ", assignedNodes.size = " << assignedNodes[i].size() << endl;
        }
      }
    #endif

    // Fill the startIndex and recvCounts
    // 7 is because the collectBodies function is collection 7 info per body
    // Mass, positions (2), velocities (2), index and process

    startIndex[0] = 0;
    recvCounts[0] = nbrBodiesPerNode[0]*7;
    for(int i=1; i<nbrProcs; i++) {
      startIndex[i] = startIndex[i-1] + 7*nbrBodiesPerNode[i-1];
      recvCounts[i] = nbrBodiesPerNode[i]*7;
    }

    // Here, in the serial version, we would use the function calculateAllAccelerationsFromNode
    // But we only want to calcule the forces on the local bodies
    for(unsigned int i=0; i<assignedNodes[myRank].size(); i++) {
      pqtree.calculateAllAccelerationsFromNode(*assignedNodes[myRank][i], myRank);
    }

    localData.clear();
    // Now we can move the bodies and collect the local data
    for (unsigned int i=0; i<assignedNodes[myRank].size(); i++) {
      pqtree.updateBodies(*assignedNodes[myRank][i]);
      pqtree.collect(localData, *assignedNodes[myRank][i]);
    }

    globalData.clear();
    globalData.resize(nbrBodies);

    if(myRank == 0) {
      #ifdef WRITE_TIME
        if((iteration+1)%samplingFreq == 0) {
          startSend = MPI_Wtime();
        }
      #endif
    }

    #ifdef DEBUG
      cout << "Process " << myRank << " on " << nbrProcs << " says SENDING." << endl;
    #endif

    MPI_Bcast(&nbrBodies, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Send the data and gather them into the globalData vector
    MPI_Allgatherv(&localData[0], recvCounts[myRank], MPI_DOUBLE, &globalData[0], recvCounts, startIndex, MPI_DOUBLE, MPI_COMM_WORLD);

    #ifdef DEBUG
      cout << "Process " << myRank << " on " << nbrProcs << " says RECEIVED." << endl;
    #endif


    if(myRank == 0) {
      #ifdef WRITE_TIME
        if((iteration+1)%samplingFreq == 0) {
          startBuilding = MPI_Wtime();
          double sendTime = (MPI_Wtime() - startSend);
          outputTimeFile << "Iteration " << t+dt << " Send time, " << sendTime << endl;
        }
      #endif
    }

    // Rebuild the quadtree
    pqtree.cleanQuadtree();
    for(int i=0; i<nbrBodies; i++) {
      Body body(globalData[7*i], globalData[7*i+1], globalData[7*i+2], globalData[7*i+3], globalData[7*i+4], globalData[7*i+5]);
      if (pqtree.checkIfBodyIsLost(body)) {
        #ifdef DEBUG
          if(myRank == 0) {
            cout << "Body " << i << " is lost in the space! (" << positions[2*i]/AU << ", " << positions[2*i+1]/AU << ")" << endl;
          }
        #endif
      } else {
        pqtree.insertBody(body, pqtree.root);
      }
    }

    nbrBodies = pqtree.root.nbrBodies;

    iteration++;

    if(myRank == 0) {
      #ifdef WRITE_TIME
        if(iteration%samplingFreq == 0) {
          iterationTime = (MPI_Wtime() - startTimeIteration);
          outputTimeFile << "Iteration " << t+dt << ", " << iterationTime << endl;
          buildingTime = (MPI_Wtime() - startBuilding);
          outputTimeFile << "Iteration " << t+dt << " Building Tree, " << buildingTime << endl;
        }
      #endif
    }

  }

  #ifdef DEBUG
    cout << "Process " << myRank << " on " << nbrProcs << " says GOODBYE." << endl;
  #endif

  pqtree.cleanQuadtree();
  delete[] nbrBodiesPerNode;
  delete[] recvCounts;
  delete[] startIndex;

  if(myRank == 0) {
    #ifdef WRITE_TIME
      double simulationTime = (MPI_Wtime() - startSimulation);
      double totalTime = (MPI_Wtime() - startTotal);
      outputTimeFile << "Simulation time, " << simulationTime  << std::endl;
      outputTimeFile << "Total time, " << totalTime  << std::endl;
      outputTimeFile.close();
    #endif

    #ifdef WRITE_OUTPUT
      outputFile.close();
    #endif

    #ifdef WRITE_QUADTREE
      outputQTFile.close();
    #endif
  }

  #ifdef DEBUG
    cout << "Process " << myRank << " on " << nbrProcs << " says GOODBYE." << endl;
  #endif

  MPI_Finalize();
  exit(0);
}
