#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <ctime>
#include <mpi.h>

#include "Configuration.cpp"

#define UA 1.4960e11
#define year 365*24*60*60
#define G 6.674e-11

#define DEBUG
#define WRITE_OUTPUT
#define WRITE_TIME

using namespace std;

int main(int argc, char* argv[])
{

  // INIT MPI
  MPI_Init(&argc, &argv);
  int myRank;
  int nbrProcs;
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &nbrProcs);

  string fileName;

  double dt;
  double finalTime;
  double samplingFreq;
  string initialFile;
  string outputFileName;
  double dvx = 0.0;
  double dvy = 0.0;
  int iteration = 0;
  double maxSize;

  int nbrBodies;
  vector<double> mass;
  vector<double> positions;
  vector<double> velocities;
  vector<double> fixedPositions;

  double start;
  double end;
  double start_total;

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
    samplingFreq = conf.get<double>("samplingFreq");
    initialFile = conf.get<string>("initialFile");
    outputFileName = conf.get<string>("outputFile");
    maxSize = conf.get<double>("size");

    // Get the masses, positions and velocities for each object.
    nbrBodies = conf.getNbrBodies();
    mass = conf.getInitialMass();
    positions = conf.getInitialPositions();
    velocities = conf.getInitialVelocities();

    #ifdef DEBUG
      cout << "Number of bodies: " << nbrBodies << endl;
    #endif

    #ifdef WRITE_TIME
      start_total = MPI_Wtime();
      size_t pos = outputFileName.rfind(".");
      string outputTimeFileName = outputFileName.substr(0,pos) + "_time" + outputFileName.substr(pos);
      cout << outputTimeFileName << endl;
      outputTimeFile.open(outputTimeFileName.c_str());
      outputTimeFile << "# Info, Time [s]" << endl;
    #endif

    #ifdef WRITE_OUTPUT
      outputFile.open(outputFileName.c_str());
      outputFile.precision(12);
      outputFile << "# Time, Mass, X position, Y position, X velocity, Y velocity" << endl;
      for (int i=0; i<nbrBodies; i++) {
        outputFile << 0 << ", " << mass[i] << ", " << positions[2*i]/UA << ", " << positions[2*i+1]/UA << ", " << velocities[2*i] << ", " << velocities[2*i+1] << endl;
      }
    #endif
  }

  // Broadcast Some values to all the process
  MPI_Bcast(&nbrBodies, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&dt, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&finalTime, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&maxSize, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // Now we need to send the initial data
  int*

  for(double t=0; t<finalTime; t+=dt) {
    #ifdef DEBUG
      cout << "Process " << myRank << ": Start loops for time " << t+dt << endl;
    #endif

    // Fix the positions to update the speed and then the positions
    fixedPositions = positions;

    #ifdef WRITE_TIME
      std::clock_t startTimeIteration = std::clock();
    #endif

    for(unsigned int i=0; i<mass.size(); i++) {

      // Check if the planet is outside of the square and remove it if it's the case
      if(fabs(positions[2*i])/UA > maxSize || fabs(positions[2*i+1])/UA > maxSize) {
        #ifdef DEBUG
          cout << "Planet " << i << " is outside of the square! (" << positions[2*i]/UA << ", " << positions[2*i+1]/UA << ")" << endl;
        #endif

        mass.erase(mass.begin() + i);
        positions.erase(positions.begin() + 2*i, positions.begin() + 2*i+2);
        fixedPositions.erase(fixedPositions.begin() + 2*i, fixedPositions.begin() + 2*i+2);
        velocities.erase(velocities.begin() + 2*i, velocities.begin() + 2*i+2);
        nbrBodies--;
        i--;
      } else {
        dvx = 0.0;
        dvy = 0.0;
        for(unsigned int j=0; j<mass.size(); j++) {
          // Don't take into account the same planet
          if(i!=j) {
            double distance = sqrt(pow(fixedPositions[2*j]-positions[2*i],2) + pow(fixedPositions[2*j+1]-positions[2*i+1],2));

            // First we check if the distance between two bodies is not smaller than an arbitrary distance.
            // It this distance is too small, we collapse the two bodies
            // WARNING: If two bodies collapse together, we don't update its velocity with the forces
            if(distance < 100000) { // Arbitrary distance
              #ifdef DEBUG
                cout << "Planet " << i << " and Planet " << j << " are collapsing! Distance between them: " << distance << endl;
                cout << "   Position planet " << i << ": (" << positions[2*i]/UA << ", " << positions[2*i+1]/UA << ");" << endl;
                cout << "   Position planet " << j << ": (" << positions[2*j]/UA << ", " << positions[2*j+1]/UA << ") " << endl;
              #endif
               // Update the values. We decide that the mass will be the sume of the two planets
              velocities[2*i] = (mass[i]*velocities[2*i] + mass[j]*velocities[2*j])/(mass[i]+mass[j]);
              velocities[2*i+1] = (mass[i]*velocities[2*i+1] + mass[j]*velocities[2*j+1])/(mass[i]+mass[j]);
              mass[i] += mass[j];

              mass.erase(mass.begin() + j);
              positions.erase(positions.begin() + 2*j, positions.begin() + 2*j+2);
              fixedPositions.erase(fixedPositions.begin() + 2*j, fixedPositions.begin() + 2*j+2);
              velocities.erase(velocities.begin() + 2*j, velocities.begin() + 2*j+2);
              nbrBodies--;

              // Set dvx and dvy = 0;
              dvx = 0.0;
              dvy = 0.0;

              // Get out of the for loop
              break;
            } else {
              dvx += year*dt * (fixedPositions[2*j] - positions[2*i]) * G*mass[j]/(distance*distance*distance);
              dvy += year*dt * (fixedPositions[2*j+1] - positions[2*i+1]) * G*mass[j]/(distance*distance*distance);
            }
          }
        }

        // Now we update the position of the body i
        velocities[2*i] += dvx;
        velocities[2*i+1] += dvy;
        positions[2*i] += year*dt * velocities[2*i];
        positions[2*i+1] += year*dt * velocities[2*i+1];

      }
    }

    iteration++;

    #ifdef WRITE_TIME
      if(floor(iteration*samplingFreq) == iteration*samplingFreq) {
        double iterationTime = (std::clock() - startTimeIteration) / (double) CLOCKS_PER_SEC;
        outputTimeFile << "Iteration " << t+dt << ", " << iterationTime << endl;
      }
    #endif

    #ifdef WRITE_OUTPUT
      if(floor(iteration*samplingFreq) == iteration*samplingFreq) {
        for (int k = 0; k < nbrBodies;k++) {
          outputFile << t+dt << "," << mass[k] << ", " << positions[2*k]/UA << "," <<  positions[2*k+1]/UA << ", " << velocities[2*k] << "," <<  velocities[2*k+1] << std::endl;
        }
      }
    #endif
  }

  #ifdef WRITE_TIME
    double simulationTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    outputTimeFile << "Total time, " << simulationTime  << std::endl;
  #endif


}
