#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <ctime>

#include "Configuration.cpp"

#define DEBUG
#define WRITE_OUTPUT
#define WRITE_TIME

using namespace std;

int main(int argc, char* argv[])
{

  // Gravity constant
  const double G = 6.674e-11;
  // If an object is at a distance more than maxDistance, we don't compute the force.
  const double maxDistance = 1e4;

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
  double startSampling = conf.get<double>("startSampling");
  string initialFile = conf.get<string>("initialFile");
  string outputFileName = conf.get<string>("outputFile");

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

    for (unsigned int i=0; i<velocities.size(); i++) {
      cout << velocities[i] << endl;
    }
  #endif

  #ifdef WRITE_OUTPUT
    ofstream outputFile;
    outputFile.open(outputFileName.c_str());
    outputFile.precision(12);
    outputFile << "# Time, X position, Y position" << endl;
    for (int i=0; i<nbrBodies; i++) {
      outputFile << 0 << ", " << positions[2*i] << ", " << positions[2*i+1] << endl;
    }
  #endif

  #ifdef WRITE_TIME
    std::clock_t start = std::clock();
    ofstream outputTimeFile;
    size_t pos = outputFileName.find(".");
    string outputTimeFileName = outputFileName.substr(0,pos) + "_time" + outputFileName.substr(pos);
    outputTimeFile.open(outputTimeFileName.c_str());
    outputTimeFile << "# Info, Time [s]" << endl;
  #endif

  for(double t=0; t<finalTime; t+=dt) {
    #ifdef DEBUG
      cout << "Start loops for time " << t+dt << endl;
    #endif

    #ifdef WRITE_TIME
      std::clock_t startTimeIteration = std::clock();
    #endif

    for(int i=0; i<nbrBodies; i++) {
      for(int j=0; j<nbrBodies; j++) {
        double distance = sqrt(pow(fixedPositions[2*j]-positions[2*i],2) + pow(fixedPositions[2*j+1]-positions[2*i+1],2));

        if (distance > maxDistance) {
          velocities[2*i] += dt * (fixedPositions[2*j] - positions[2*i]) * G*mass[j]/(distance*distance*distance);
          velocities[2*i+1] += dt * (fixedPositions[2*j+1] - positions[2*i+1]) * G*mass[j]/(distance*distance*distance);
        }
      }
      // Now we update the position of the body i
      positions[2*i] += dt * velocities[2*i];
  	  positions[2*i+1] += dt * velocities[2*i+1];
    }
    // Update the fixed positions
    fixedPositions = positions;

    #ifdef WRITE_TIME
      double iterationTime = (std::clock() - startTimeIteration) / (double) CLOCKS_PER_SEC;
      outputTimeFile << "Iteration " << t+dt << ", " << iterationTime << endl;
    #endif

    #ifdef WRITE_OUTPUT
    for (int k = 0; k < nbrBodies;k++) {
      outputFile << t+dt << "," << positions[2*k] << "," <<  positions[2*k+1] << std::endl;
    }
    #endif
  }

  #ifdef WRITE_TIME
    double simulationTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    outputTimeFile << "Total time, " << simulationTime  << std::endl;
  #endif
}
