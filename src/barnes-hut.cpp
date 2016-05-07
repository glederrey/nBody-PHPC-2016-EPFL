#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <ctime>

#include "Configuration.cpp"
#include "Body.cpp"
#include "Node.cpp"
#include "Quadtree.cpp"

#define AU 1.4960e11
#define day 24*60*60
#define G 6.674e-11

// These variables {VAR} can be written in the makefile with -D{VAR}
//#define DEBUG
//#define WRITE_OUTPUT
//#define WRITE_TIME
//#define WRITE_QUADTREE

using namespace std;

int main(int argc, char* argv[])
{

  string fileName;

  if(argc > 1) {
    fileName.append(argv[1]);
  } else {
    fileName = "config_bh.init";
  }

  Configuration conf(fileName);

  double dt = conf.get<double>("dt");
  double finalTime = conf.get<double>("finalTime");
  #ifdef WRITE_OUTPUT
    int samplingFreq = conf.get<int>("samplingFreq");
  #endif
  string initialFile = conf.get<string>("initialFile");
  string outputFileName = conf.get<string>("outputFile");
  double maxSize = conf.get<double>("size");
  double theta = conf.get<double>("theta");
  int iteration = 0;
  #ifdef WRITE_TIME
    clock_t startTotal;
    clock_t startSimulation;
    clock_t startBuildTree;
  #endif

  #ifdef WRITE_TIME
    startTotal = clock();
    ofstream outputTimeFile;
    size_t pos = outputFileName.rfind(".");
    string outputTimeFileName = outputFileName.substr(0,pos) + "_time" + outputFileName.substr(pos);
    outputTimeFile.open(outputTimeFileName.c_str());
    outputTimeFile << "# Info, Time [s]" << endl;
  #endif

  #ifdef WRITE_QUADTREE
    ofstream outputQTFile;
    size_t pos2 = outputFileName.rfind(".");
    string outputQTFileName = outputFileName.substr(0,pos2) + "_quadtree" + outputFileName.substr(pos2);
    outputQTFile.open(outputQTFileName.c_str());
  #endif


  conf.prepareInitialValues(initialFile);
  // Get the masses, positions and velocities for each object.
  int nbrBodies = conf.getNbrBodies();
  vector<double> mass = conf.getInitialMass();
  vector<double> positions = conf.getInitialPositions();
  vector<double> velocities = conf.getInitialVelocities();

  #ifdef DEBUG
    cout << "Number of bodies: " << nbrBodies << endl;
  #endif

  #ifdef WRITE_TIME
    startBuildTree = clock();
  #endif

  // Create the Quadtree
  Quadtree qtree(0.0, 0.0, 2.0*maxSize*AU, 2.0*maxSize*AU, dt, theta);
  for(int i=0; i<nbrBodies; i++) {
    Body body(mass[i], positions[2*i], positions[2*i+1], velocities[2*i], velocities[2*i+1], i);
    qtree.insertBody(body, qtree.root);
  }

  #ifdef WRITE_TIME
    double buildingTime = (clock() - startBuildTree) / (double) CLOCKS_PER_SEC;
    outputTimeFile << "Building tree, " << buildingTime  << std::endl;
  #endif

  #ifdef DEBUG
    cout << "Number of bodies in the QT: " << qtree.root.nbrBodies << endl;
  #endif

  #ifdef WRITE_OUTPUT
    ofstream outputFile;
    outputFile.open(outputFileName.c_str());
    outputFile.precision(12);
    outputFile << "# Time, Mass, X position, Y position" << endl;
    for (int i=0; i<nbrBodies; i++) {
      outputFile << 0 << ", " << mass[i] << ", " << positions[2*i]/AU << ", " << positions[2*i+1]/AU << endl;
    }
    outputFile.flush();
  #endif

  #ifdef WRITE_QUADTREE
    outputQTFile << 0 << ", ";
    qtree.print(outputQTFile, AU, 0.1*AU);
    outputQTFile.flush();
  #endif

  #ifdef WRITE_TIME
    double loadingTime = (clock() - startTotal) / (double) CLOCKS_PER_SEC;
    outputTimeFile << "Loading data, " << loadingTime  << std::endl;
    startSimulation = clock();
  #endif

  vector<double> data;
  for(double t=0; t<finalTime; t+=dt) {
    #ifdef DEBUG
      cout << "Start loops for time " << t+dt << endl;
    #endif

    #ifdef WRITE_TIME
      clock_t startTimeIteration = clock();
    #endif

    qtree.calculateAllAccelerationsFromNode(qtree.root);
    qtree.updateBodies(qtree.root);
    qtree.collect(data, qtree.root);
    qtree.cleanQuadtree();

    // Rebuild the Quadtree
    #ifdef WRITE_TIME
      if(iteration%samplingFreq == 0) {
        startBuildTree = clock();
      }
    #endif

    // Create the Quadtree
    for(int i=0; i<nbrBodies; i++) {
      cout << i << endl;
      Body body(data[5*i], data[5*i+1], data[5*i+2], data[5*i+3], data[5*i+4], i);
      cout << qtree.root.depth << endl;
      cout << "PROUT" << endl;
      qtree.insertBody(body, qtree.root);
    }

    #ifdef WRITE_TIME
      if(iteration%samplingFreq == 0) {
        buildingTime = (clock() - startBuildTree) / (double) CLOCKS_PER_SEC;
        outputTimeFile << "Iteration " << t+dt << " Building Tree, " << buildingTime << endl;
      }
    #endif

    data.clear();

    iteration++;

    #ifdef WRITE_TIME
      if(iteration%samplingFreq == 0) {
        double iterationTime = (clock() - startTimeIteration) / (double) CLOCKS_PER_SEC;
        outputTimeFile << "Iteration " << t+dt << ", " << iterationTime << endl;
      }
    #endif

    #ifdef WRITE_OUTPUT
    if(iteration%samplingFreq == 0) {
      for (int k = 0; k < nbrBodies;k++) {
        outputFile << t+dt << ", " << data[5*k] << ", " << data[5*k+1]/AU << ", " <<  data[5*k+2]/AU << std::endl;
      }
      outputFile.flush();
    }
    #endif
  }

  #ifdef WRITE_TIME
    double simulationTime = (clock() - startSimulation) / (double) CLOCKS_PER_SEC;
    double totalTime = (clock() - startTotal) / (double) CLOCKS_PER_SEC;
    outputTimeFile << "Simulation time, " << simulationTime  << std::endl;
    outputTimeFile << "Total time, " << totalTime  << std::endl;
    outputTimeFile.close();
  #endif

  #ifdef WRITE_OUTPUT
    outputFile.close();
  #endif


  exit(0);

}
