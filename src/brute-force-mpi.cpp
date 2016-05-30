#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <ctime>
#include <mpi.h>

#include "Configuration.cpp"

#define AU 1.4960e11
#define day 24*60*60
#define G 6.674e-11

// These variables {VAR} can be written in the makefile with -D{VAR}
//#define DEBUG
//#define WRITE_OUTPUT
//#define WRITE_TIME

using namespace std;

int main(int argc, char* argv[])
{

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
  string initialFile;
  string outputFileName;
  double maxSize;
  double dvx = 0.0;
  double dvy = 0.0;
  int iteration = 0;

  // Define variables that will be set using ConfigFile
  int nbrBodies;
  vector<double> fixedPositions;
  vector<double> fixedVelocities;
  vector<double> fixedMass;

  #if WRITE_TIME || WRITE_OUTPUT
    int samplingFreq = 0;
  #endif

  #ifdef WRITE_TIME
    // Time variables
    double startTotal = MPI_Wtime();
    double startSimulation = 0.0;
    double startSend = 0.0;
    double startTimeIteration = 0.0;
  #endif

  // Streams
  ofstream outputTimeFile;
  ofstream outputFile;

  // Get the configurations
  if(myRank == 0) {

    if(argc > 1) {
      fileName.append(argv[1]);
    } else {
      fileName = "../config/bf_parallel.init";
    }

    Configuration conf(fileName);

    dt = conf.get<double>("dt");
    finalTime = conf.get<double>("finalTime");
    #if WRITE_OUTPUT || WRITE_TIME
      samplingFreq = conf.get<int>("samplingFreq");
    #endif
    initialFile = conf.get<string>("initialFile");
    outputFileName = conf.get<string>("outputFile");
    maxSize = conf.get<double>("maxSize");

    #ifdef WRITE_TIME
      size_t pos = outputFileName.rfind(".");
      string outputTimeFileName = outputFileName.substr(0,pos) + "_time" + outputFileName.substr(pos);
      outputTimeFile.open(outputTimeFileName.c_str());
      outputTimeFile << "# Info, Time [s]" << endl;
    #endif

    conf.prepareInitialValues(initialFile);
    // Get the masses, positions and velocities for each object.
    nbrBodies = conf.getNbrBodies();
    fixedMass = conf.getInitialMass();
    fixedPositions = conf.getInitialPositions();
    fixedVelocities = conf.getInitialVelocities();

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

  }

  // Broadcast Some values to all the process
  MPI_Bcast(&nbrBodies, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&dt, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&finalTime, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&maxSize, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // Prepare the size of each local set of bodies for each process
  int *localNbrBodies = new int[nbrProcs];
  int *startIndex = new int[nbrProcs];
  int *recvCounts = new int[nbrProcs];
  int rest = nbrBodies%nbrProcs;
  int meanLocalNbrBodies = (nbrBodies-rest)/nbrProcs;

  int nbrBodiesGiven = 0;

  for (int i=0; i<nbrProcs; i++) {

    startIndex[i] = nbrBodiesGiven;

    if(rest>0 && i>0) // Don't want to give a supplementary one to the MASTER
    {
      localNbrBodies[i] = (meanLocalNbrBodies + 1);
      rest--;
      nbrBodiesGiven += meanLocalNbrBodies + 1;
    } else {
      localNbrBodies[i] = meanLocalNbrBodies;
      nbrBodiesGiven += meanLocalNbrBodies;
    }
  }

  for (int i=0; i<nbrProcs; i++) {
    recvCounts[i] = localNbrBodies[i]*2;
  }

  // Rank 0 knows the initial values for the mass and the positions, so for the others, we need to resize
  if (myRank != 0) {
    fixedPositions.resize(2*nbrBodies);
    fixedVelocities.resize(2*nbrBodies);
    fixedMass.resize(nbrBodies);
  }

  if(myRank == 0) {
    #ifdef DEBUG
      cout << "Loading the data is finished" << endl;
    #endif

    #ifdef WRITE_TIME
      outputTimeFile << "Loading data, " << MPI_Wtime() - startTotal << endl;
      startSend = MPI_Wtime();
    #endif
  }

  // Now we send the data to all the process
  MPI_Bcast(&fixedMass[0], nbrBodies, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&fixedPositions[0], 2*nbrBodies, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&fixedVelocities[0], 2*nbrBodies, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // Define the local vectors
  vector<double> localMass(&fixedMass[startIndex[myRank]], &fixedMass[startIndex[myRank]+localNbrBodies[myRank]]);
  vector<double> localPositions(&fixedPositions[2*startIndex[myRank]], &fixedPositions[2*(startIndex[myRank]+localNbrBodies[myRank])]);
  vector<double> localVelocities(&fixedVelocities[2*startIndex[myRank]], &fixedVelocities[2*(startIndex[myRank]+localNbrBodies[myRank])]);

  #ifdef DEBUG
    cout << "rank " << myRank << ": local nbr bodies = " << localNbrBodies[myRank] << "; localMass.size() = " << localMass.size() << endl;
  #endif

  if(myRank == 0) {
    #ifdef WRITE_TIME
      outputTimeFile << "Sending data, " << MPI_Wtime() - startSend << endl;
      startSimulation = MPI_Wtime();
    #endif
  }

  for(double t=0; t<finalTime; t+=dt) {

    if(myRank == 0) {
      #ifdef DEBUG
        cout << "Start loops for time " << t+dt << endl;
      #endif

      #ifdef WRITE_TIME
        startTimeIteration = MPI_Wtime();
      #endif
    }

    for(int i=0; i<localNbrBodies[myRank]; i++) {

      int idx = i + startIndex[myRank]; // Index needs to be shifted in order to compare to j
      dvx = 0.0;
      dvy = 0.0;
      for(int j=0; j<(int)fixedMass.size(); j++) { // Just cast in int the size in order to avoid Warning
        // Don't take into account the same planet
        if(j!=idx) {
          double distance = sqrt(pow(fixedPositions[2*j]-localPositions[2*i],2) + pow(fixedPositions[2*j+1]-localPositions[2*i+1],2));
          dvx += day*dt * (fixedPositions[2*j] - localPositions[2*i]) * G*fixedMass[j]/(distance*distance*distance);
          dvy += day*dt * (fixedPositions[2*j+1] - localPositions[2*i+1]) * G*fixedMass[j]/(distance*distance*distance);
        }
      }

      localVelocities[2*i] += dvx;
      localVelocities[2*i+1] += dvy;
      localPositions[2*i] += day*dt * localVelocities[2*i];
      localPositions[2*i+1] += day*dt * localVelocities[2*i+1];

    }

    if(myRank == 0) {
      #ifdef WRITE_TIME
        if((iteration+1)%samplingFreq == 0) {
          startSend = MPI_Wtime();
        }
      #endif
    }

    MPI_Allgatherv(&localPositions[0], recvCounts[myRank], MPI_DOUBLE, &fixedPositions[0], recvCounts, startIndex, MPI_DOUBLE, MPI_COMM_WORLD);

    iteration++;

    if(myRank==0) {
      #ifdef WRITE_TIME
        if(iteration%samplingFreq == 0) {
          double iterationTime = (MPI_Wtime() - startTimeIteration);
          double sendTime = (MPI_Wtime() - startSend);
          outputTimeFile << "Iteration " << t+dt << ", " << iterationTime << endl;
          outputTimeFile << "Iteration " << t+dt << " Send time, " << sendTime << endl;
        }
      #endif

      #ifdef WRITE_OUTPUT
        if(iteration%samplingFreq == 0) {
          outputFile.open(outputFileName.c_str(),fstream::app);
          outputFile.precision(12);
          for (unsigned int k = 0; k < fixedMass.size(); k++) {
            outputFile << t+dt << ", " << fixedMass[k] << ", " << fixedPositions[2*k]/AU << ", " <<  fixedPositions[2*k+1]/AU << endl;
          }
          outputFile.close();
        }
      #endif
    }
  }

  #ifdef WRITE_TIME
    double simulationTime = (MPI_Wtime() - startSimulation);
    double totalTime = (MPI_Wtime() - startTotal);
    outputTimeFile << "Simulation time, " << simulationTime  << std::endl;
    outputTimeFile << "Total time, " << totalTime  << std::endl;
    outputTimeFile.close();
  #endif

  MPI_Finalize();
  return 0;
}
