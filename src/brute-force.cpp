#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <ctime>

#include "Configuration.cpp"

#define UA 1.4960e11
#define year 365*24*60*60
#define G 6.674e-11

#define DEBUG
//#define WRITE_OUTPUT
#define WRITE_TIME

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
  double samplingFreq = conf.get<double>("samplingFreq");
  string initialFile = conf.get<string>("initialFile");
  string outputFileName = conf.get<string>("outputFile");
  double maxSize = conf.get<double>("size");
  int iteration = 0;

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
    std::clock_t start = std::clock();
    ofstream outputTimeFile;
    size_t pos = outputFileName.rfind(".");
    string outputTimeFileName = outputFileName.substr(0,pos) + "_time" + outputFileName.substr(pos);
    cout << outputTimeFileName << endl;
    outputTimeFile.open(outputTimeFileName.c_str());
    outputTimeFile << "# Info, Time [s]" << endl;
  #endif

  for(double t=0; t<finalTime; t+=dt) {
    #ifdef DEBUG
      cout << "Start loops for time " << t+dt << endl;
    #endif

    // First, we check if the planet is outside of the maximum size. If it's the case, we remove it
    for(int i=0; i<nbrBodies; i++) {
      if(fabs(positions[2*i])/UA > maxSize || fabs(positions[2*i+1])/UA > maxSize) {
        #ifdef DEBUG
          cout << "Planet " << i << " is outside of the square! (" << positions[2*i] << ", " << positions[2*i+1] << ")" << endl;
        #endif

        mass.erase(mass.begin() + i);
        positions.erase(positions.begin() + 2*i, positions.begin() + 2*i+2);
        velocities.erase(velocities.begin() + 2*i, velocities.begin() + 2*i+2);
        nbrBodies--;
      }
    }

    // Then, we check if two planets are too close and collapse them
    for(unsigned int i=0; i<mass.size(); i++) {
      for(unsigned int j=i+1; j<mass.size(); j++) {
        double distance = sqrt(pow(positions[2*j]-positions[2*i],2) + pow(positions[2*j+1]-positions[2*i+1],2));
        if(distance < 100000) { // Arbitrary distance
          #ifdef DEBUG
            cout << "Planet " << i << " and Planet " << j << " are collapsing! Distance between them: " << distance << endl;
            cout << "   Position planet " << i << ": (" << positions[2*i]/UA << ", " << positions[2*i+1]/UA << ");" << endl;
            cout << "   Position planet " << j << ": (" << positions[2*j]/UA << ", " << positions[2*j+1]/UA << ") " << endl;
          #endif
           // Update the values. We decide that the density will be the mean between the two planets
          velocities[2*i] = (mass[i]*velocities[2*i] + mass[j]*velocities[2*j])/(mass[i]+mass[j]);
          velocities[2*i+1] = (mass[i]*velocities[2*i+1] + mass[j]*velocities[2*j+1])/(mass[i]+mass[j]);
          mass[i] += mass[j];

          mass.erase(mass.begin() + j);
          positions.erase(positions.begin() + 2*j, positions.begin() + 2*j+2);
          velocities.erase(velocities.begin() + 2*j, velocities.begin() + 2*j+2);
          nbrBodies--;
        }
      }
    }

    // Fix the positions to update the speed and then the positions
    fixedPositions = positions;

    #ifdef WRITE_TIME
      std::clock_t startTimeIteration = std::clock();
    #endif

    for(int i=0; i<nbrBodies; i++) {
      for(int j=0; j<nbrBodies; j++) {
        // Don't take into account the same planet

        if(i!=j) {
          double distance = sqrt(pow(fixedPositions[2*j]-positions[2*i],2) + pow(fixedPositions[2*j+1]-positions[2*i+1],2));
          velocities[2*i] += year*dt * (fixedPositions[2*j] - positions[2*i]) * G*mass[j]/(distance*distance*distance);
          velocities[2*i+1] += year*dt * (fixedPositions[2*j+1] - positions[2*i+1]) * G*mass[j]/(distance*distance*distance);
        }
      }
      // Now we update the position of the body i
      positions[2*i] += year*dt * velocities[2*i];
  	  positions[2*i+1] += year*dt * velocities[2*i+1];
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
