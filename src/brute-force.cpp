#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <ctime>

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

  string fileName;

  if(argc > 1) {
    fileName.append(argv[1]);
  } else {
    fileName = "config.init";
  }

  Configuration conf(fileName);

  double dt = conf.get<double>("dt");
  double finalTime = conf.get<double>("finalTime");
  int samplingFreq = conf.get<int>("samplingFreq");
  string initialFile = conf.get<string>("initialFile");
  string outputFileName = conf.get<string>("outputFile");
  double maxSize = conf.get<double>("size");
  double dvx = 0.0;
  double dvy = 0.0;
  int iteration = 0;
  clock_t startTotal;
  clock_t startSimulation;

  #ifdef WRITE_TIME
    startTotal = std::clock();
    ofstream outputTimeFile;
    size_t pos = outputFileName.rfind(".");
    string outputTimeFileName = outputFileName.substr(0,pos) + "_time" + outputFileName.substr(pos);
    outputTimeFile.open(outputTimeFileName.c_str());
    outputTimeFile << "# Info, Time [s]" << endl;
  #endif

  conf.prepareInitialValues(initialFile);
  // Get the masses, positions and velocities for each object.
  int nbrBodies = conf.getNbrBodies();
  vector<double> mass = conf.getInitialMass();
  vector<double> positions = conf.getInitialPositions();
  vector<double> velocities = conf.getInitialVelocities();
  // Positions that will not be updated during the calculation loop
  vector<double> fixedPositions(positions);

  #ifdef DEBUG
    cout << "Number of bodies: " << nbrBodies << endl;
  #endif

  #ifdef WRITE_OUTPUT
    ofstream outputFile;
    outputFile.open(outputFileName.c_str());
    outputFile.precision(12);
    outputFile << "# Time, Mass, X position, Y position, X velocity, Y velocity" << endl;
    for (int i=0; i<nbrBodies; i++) {
      outputFile << 0 << ", " << mass[i] << ", " << positions[2*i]/UA << ", " << positions[2*i+1]/UA << ", " << velocities[2*i] << ", " << velocities[2*i+1] << endl;
    }
  #endif

  #ifdef WRITE_TIME
    double loadingTime = (std::clock() - startTotal) / (double) CLOCKS_PER_SEC;
    outputTimeFile << "Loading data, " << loadingTime  << std::endl;
    startSimulation = std::clock();
  #endif

  for(double t=0; t<finalTime; t+=dt) {
    #ifdef DEBUG
      cout << "Start loops for time " << t+dt << endl;
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
              dvx += day*dt * (fixedPositions[2*j] - positions[2*i]) * G*mass[j]/(distance*distance*distance);
              dvy += day*dt * (fixedPositions[2*j+1] - positions[2*i+1]) * G*mass[j]/(distance*distance*distance);
            }
          }
        }

        // Now we update the position of the body i
        velocities[2*i] += dvx;
        velocities[2*i+1] += dvy;
        positions[2*i] += day*dt * velocities[2*i];
        positions[2*i+1] += day*dt * velocities[2*i+1];

      }
    }

    iteration++;

    #ifdef WRITE_TIME
      if(iteration%samplingFreq == 0) {
        double iterationTime = (std::clock() - startTimeIteration) / (double) CLOCKS_PER_SEC;
        outputTimeFile << "Iteration " << t+dt << ", " << iterationTime << endl;
      }
    #endif

    #ifdef WRITE_OUTPUT
      if(iteration%samplingFreq == 0) {
        for (int k = 0; k < nbrBodies;k++) {
          outputFile << t+dt << ", " << mass[k] << ", " << positions[2*k]/UA << ", " <<  positions[2*k+1]/UA << ", " << velocities[2*k] << ", " <<  velocities[2*k+1] << std::endl;
        }
      }
    #endif
  }

  #ifdef WRITE_TIME
    double simulationTime = (std::clock() - startSimulation) / (double) CLOCKS_PER_SEC;
    double totalTime = (std::clock() - startTotal) / (double) CLOCKS_PER_SEC;
    outputTimeFile << "Simulation time, " << simulationTime  << std::endl;
    outputTimeFile << "Total time, " << totalTime  << std::endl;
    outputTimeFile.close();
  #endif

  #ifdef WRITE_OUTPUT
    outputFile.close();
  #endif

  exit(0);

}
