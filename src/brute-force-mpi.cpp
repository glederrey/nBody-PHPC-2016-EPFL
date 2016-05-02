#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <ctime>
#include <mpi.h>

#include "Configuration.cpp"

#define UA 1.4960e11
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
  MPI_Status status;

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
  double dvx = 0.0;
  double dvy = 0.0;
  int iteration = 0;

  // Define variables that will be set using ConfigFile
  int nbrBodies;
  vector<double> fixedPositions;
  vector<double> fixedVelocities;
  vector<double> fixedMass;

  // Time variables
  double startTotal = 0.0;
  double startSimulation = 0.0;
  double startSend = 0.0;
  double startTimeIteration = 0.0;

  // Streams
  ofstream outputTimeFile;
  ofstream outputFile;

  // Get the configurations
  if(myRank == 0) {

    if(argc > 1) {
      fileName.append(argv[1]);
    } else {
      fileName = "config.init";
    }

    Configuration conf(fileName);

    dt = conf.get<double>("dt");
    finalTime = conf.get<double>("finalTime");
    samplingFreq = conf.get<int>("samplingFreq");
    cout << samplingFreq << endl;
    initialFile = conf.get<string>("initialFile");
    outputFileName = conf.get<string>("outputFile");
    maxSize = conf.get<double>("size");

    #ifdef WRITE_TIME
      startTotal = MPI_Wtime();
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
      outputFile << "# Time, Mass, X position, Y position, X velocity, Y velocity" << endl;
      for (int i=0; i<nbrBodies; i++) {
        outputFile << 0 << ", " << fixedMass[i] << ", " << fixedPositions[2*i]/UA << ", " << fixedPositions[2*i+1]/UA << ", " << fixedVelocities[2*i] << ", " << fixedVelocities[2*i+1] << endl;
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

      // Check if the planet is outside of the square and remove it if it's the case
      if(fabs(localPositions[2*i])/UA > maxSize || fabs(localPositions[2*i+1])/UA > maxSize) {
        #ifdef DEBUG
          cout << "Process " << myRank << endl;
          cout << "Planet " << i << " in process " << myRank << " is outside of the square! (" << localPositions[2*i]/UA << ", " << localPositions[2*i+1]/UA << ")" << endl;
        #endif

        fixedMass.erase(fixedMass.begin() + idx);
        fixedPositions.erase(fixedPositions.begin() + 2*idx, fixedPositions.begin() + 2*idx+2);
        fixedVelocities.erase(fixedVelocities.begin() + 2*idx, fixedVelocities.begin() + 2*idx+2);
        localMass.erase(localMass.begin() + i);
        localPositions.erase(localPositions.begin() + 2*i, localPositions.begin() + 2*i+2);
        localVelocities.erase(localVelocities.begin() + 2*i, localVelocities.begin() + 2*i+2);
        localNbrBodies[myRank]--;
        i--;
      } else {
        dvx = 0.0;
        dvy = 0.0;
        for(int j=0; j<(int)fixedMass.size(); j++) { // Just cast in int the size in order to avoid Warning
          // Don't take into account the same planet
          if(j!=idx) {
            double distance = sqrt(pow(fixedPositions[2*j]-localPositions[2*i],2) + pow(fixedPositions[2*j+1]-localPositions[2*i+1],2));
            // First we check if the distance between two bodies is not smaller than an arbitrary distance.
            // It this distance is too small, we collapse the two bodies
            // WARNING: If two bodies collapse together, we don't update its velocity with the forces
            if(distance < 100000) { // Arbitrary distance
              #ifdef DEBUG
                cout << "Process " << myRank << endl;
                cout << "Planet " << i << " (in process " << myRank << ") and Planet " << j << " are collapsing! Distance between them: " << distance << endl;
                cout << "   Position planet " << i << " (mass: " << localMass[i] << "): (" << localPositions[2*i]/UA << ", " << localPositions[2*i+1]/UA << ");" << endl;
                cout << "   Position planet " << j << ": (mass: " << fixedMass[j] << "): (" << fixedPositions[2*j]/UA << ", " << fixedPositions[2*j+1]/UA << ") " << endl;
              #endif

               // Update the values. We decide that the mass will be the sume of the two planets
              localVelocities[2*i] = (localMass[i]*localVelocities[2*i] + fixedMass[j]*fixedVelocities[2*j])/(localMass[i]+fixedMass[j]);
              localVelocities[2*i+1] = (localMass[i]*localVelocities[2*i+1] + fixedMass[j]*fixedVelocities[2*j+1])/(localMass[i]+fixedMass[j]);
              localMass[i] += fixedMass[j];

              if(j >= startIndex[myRank] && j < startIndex[myRank] + localNbrBodies[myRank]) {
                localMass.erase(localMass.begin() + j);
                localPositions.erase(localPositions.begin() + 2*j, localPositions.begin() + 2*j+2);
                localVelocities.erase(localVelocities.begin() + 2*j, localVelocities.begin() + 2*j+2);
                localNbrBodies[myRank]--;
              }

              fixedMass.erase(fixedMass.begin() + j);
              fixedPositions.erase(fixedPositions.begin() + 2*j, fixedPositions.begin() + 2*j+2);
              fixedVelocities.erase(fixedVelocities.begin() + 2*j, fixedVelocities.begin() + 2*j+2);

              // Set dvx and dvy = 0;
              dvx = 0.0;
              dvy = 0.0;

              // Get out of the for loop
              break;
            } else {
              dvx += day*dt * (fixedPositions[2*j] - localPositions[2*i]) * G*fixedMass[j]/(distance*distance*distance);
              dvy += day*dt * (fixedPositions[2*j+1] - localPositions[2*i+1]) * G*fixedMass[j]/(distance*distance*distance);
            }
          }
        }

        localVelocities[2*i] += dvx;
        localVelocities[2*i+1] += dvy;
        localPositions[2*i] += day*dt * localVelocities[2*i];
        localPositions[2*i+1] += day*dt * localVelocities[2*i+1];

      }
    }

    if(myRank == 0) {
      #ifdef WRITE_TIME
        if((iteration+1)%samplingFreq == 0) {
          startSend = MPI_Wtime();
        }
      #endif
    }

    // Now, we first send the number of remaining bodies
    int size = localMass.size();
    MPI_Send(&size, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
    // Rank 0 will get them and update the vector localNbrBodies
    if(myRank == 0) {
      // Update localNbrBodies and startIndex
      nbrBodies = 0;
      for(int k=0; k<nbrProcs; k++) {
        MPI_Recv(&localNbrBodies[k], 1, MPI_INT, k, 3, MPI_COMM_WORLD, &status);
        nbrBodies += localNbrBodies[k];
        if(k>0) {
          startIndex[k] = localNbrBodies[k-1] + startIndex[k-1];
        }
      }
    }

    // Resend the number of bodies
    MPI_Bcast(&nbrBodies, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Send the local mass, local positions and local velocities
    MPI_Send(&localMass[0], localMass.size(), MPI_DOUBLE, 0, 4, MPI_COMM_WORLD);
    MPI_Send(&localPositions[0], localPositions.size(), MPI_DOUBLE, 0, 5, MPI_COMM_WORLD);
    MPI_Send(&localVelocities[0], localVelocities.size(), MPI_DOUBLE, 0, 6, MPI_COMM_WORLD);

    // Resize the different vectors for all the process
    fixedMass.resize(nbrBodies);
    fixedPositions.resize(2*nbrBodies);
    fixedVelocities.resize(2*nbrBodies);

    if(myRank == 0) {

      // Repopulate the vectors
      for(int k=0; k<nbrProcs; k++) {
        MPI_Recv(&fixedMass[startIndex[k]], localNbrBodies[k], MPI_DOUBLE, k, 4, MPI_COMM_WORLD, &status);
        MPI_Recv(&fixedPositions[2*startIndex[k]], 2*localNbrBodies[k], MPI_DOUBLE, k, 5, MPI_COMM_WORLD, &status);
        MPI_Recv(&fixedVelocities[2*startIndex[k]], 2*localNbrBodies[k], MPI_DOUBLE, k, 6, MPI_COMM_WORLD, &status);
      };

    }
    // Resend them
    MPI_Bcast(&fixedMass[0], nbrBodies, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&fixedPositions[0], 2*nbrBodies, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&fixedVelocities[0], 2*nbrBodies, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Now, we need to equalize the number of localBodies
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

    // Reallocate the local Variables
    localMass.resize(localNbrBodies[myRank]);
    localPositions.resize(2*localNbrBodies[myRank]);;
    localVelocities.resize(2*localNbrBodies[myRank]);;

    localMass.assign(fixedMass.begin()+startIndex[myRank], fixedMass.begin()+startIndex[myRank]+localNbrBodies[myRank]);
    localPositions.assign(fixedPositions.begin()+2*startIndex[myRank], fixedPositions.begin()+2*(startIndex[myRank]+localNbrBodies[myRank]));
    localVelocities.assign(fixedVelocities.begin()+2*startIndex[myRank], fixedVelocities.begin()+2*(startIndex[myRank]+localNbrBodies[myRank]));

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
            outputFile << t+dt << ", " << fixedMass[k] << ", " << fixedPositions[2*k]/UA << ", " <<  fixedPositions[2*k+1]/UA << ", " << fixedVelocities[2*k] << ", " <<  fixedVelocities[2*k+1] << endl;
          }
          outputFile.close();
        }
      #endif
    }

    MPI_Bcast(&nbrBodies, 1, MPI_INT, 0, MPI_COMM_WORLD);
  }

  #ifdef WRITE_TIME
    double simulationTime = (MPI_Wtime() - startSimulation);
    double totalTime = (MPI_Wtime() - startTotal);
    outputTimeFile << "Simulation time, " << simulationTime  << std::endl;
    outputTimeFile << "Total time, " << totalTime  << std::endl;
    outputTimeFile.close();
  #endif

  MPI_Finalize();
  exit(0);

}
