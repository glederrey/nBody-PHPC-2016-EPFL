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
  #if WRITE_OUTPUT || WRITE_TIME || WRITE_QUADTREE
    int samplingFreq = conf.get<int>("samplingFreq");
  #endif
  string initialFile = conf.get<string>("initialFile");
  string outputFileName = conf.get<string>("outputFile");
  double maxSize = conf.get<double>("maxSize");
  double minSize = conf.get<double>("minSize");
  double theta = conf.get<double>("theta");
  int iteration = 0;

  #ifdef WRITE_TIME
    clock_t startTotal;
    clock_t startSimulation;
    clock_t startBuildTree;
    clock_t startTimeIteration;
    double buildingTime;
    double iterationTime;
  #endif

  #ifdef WRITE_TIME
    startTotal = clock();
    ofstream outputTimeFile;
    size_t pos = outputFileName.rfind(".");
    string outputTimeFileName = outputFileName.substr(0,pos) + "_time" + outputFileName.substr(pos);
    outputTimeFile.open(outputTimeFileName.c_str());
    outputTimeFile.precision(12);
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
    double loadingTime = (clock() - startTotal) / (double) CLOCKS_PER_SEC;
    outputTimeFile << "Loading data, " << loadingTime  << std::endl;
    startSimulation = clock();
  #endif

  #ifdef WRITE_TIME
    startBuildTree = clock();
  #endif

  // Create the Quadtree
  Quadtree qtree(0.0, 0.0, maxSize*AU, minSize*AU, day*dt, theta);
  for(int i=0; i<nbrBodies; i++) {
    Body body(mass[i], positions[2*i], positions[2*i+1], velocities[2*i], velocities[2*i+1], i);
    if (qtree.checkIfBodyIsLost(body)) {
      #ifdef DEBUG
        cout << "Body " << i << " is lost in the space! (" << positions[2*i]/AU << ", " << positions[2*i+1]/AU << ")" << endl;
      #endif
    } else {
      qtree.insertBody(body, qtree.root);
    }
  }

  #ifdef WRITE_TIME
    buildingTime = (clock() - startBuildTree) / (double) CLOCKS_PER_SEC;
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
    outputQTFile << endl;
    outputQTFile.flush();
  #endif

  vector<double> data;
  for(double t=0; t+dt<finalTime; t+=dt) {
    #ifdef DEBUG
      cout << "Start loops for time " << t+dt << endl;
    #endif

    #ifdef WRITE_TIME
      if((iteration+1)%samplingFreq == 0) {
        startTimeIteration = clock();
      }
    #endif

    qtree.calculateAllAccelerationsFromNode(qtree.root);
    qtree.updateBodies(qtree.root);
    qtree.collect(data, qtree.root);
    qtree.cleanQuadtree();

    // Rebuild the Quadtree
    #ifdef WRITE_TIME
      if((iteration+1)%samplingFreq == 0) {
        startBuildTree = clock();
      }
    #endif

    // Update the number of bodies
    nbrBodies = data.size()/6;

    // Create the Quadtree
    for(int i=0; i<nbrBodies; i++) {
      Body body(data[6*i], data[6*i+1], data[6*i+2], data[6*i+3], data[6*i+4], data[6*i+5]);
      if (qtree.checkIfBodyIsLost(body)) {
        #ifdef DEBUG
          cout << "Body " << i << " is lost in the space! (" << data[6*i+1]/AU << ", " << data[6*i+2]/AU << ")" << endl;
        #endif
      } else {
        qtree.insertBody(body, qtree.root);
      }
    }

    iteration++;

    #ifdef WRITE_TIME
      if(iteration%samplingFreq == 0) {
        iterationTime = (clock() - startTimeIteration) / (double) CLOCKS_PER_SEC;
        outputTimeFile << "Iteration " << t+dt << ", " << iterationTime << endl;
        buildingTime = (clock() - startBuildTree) / (double) CLOCKS_PER_SEC;
        outputTimeFile << "Iteration " << t+dt << " Building Tree, " << buildingTime << endl;
      }
    #endif

    #ifdef WRITE_OUTPUT
      if(iteration%samplingFreq == 0) {
        mass.resize(nbrBodies);
        positions.resize(2*nbrBodies);
        velocities.resize(2*nbrBodies);

        for(int j=0; j<nbrBodies; j++) {
          mass[data[6*j+5]] = data[6*j];
          positions[2*data[6*j+5]] = data[6*j+1];
          positions[2*data[6*j+5]+1] = data[6*j+2];
          velocities[2*data[6*j+5]] = data[6*j+3];
          velocities[2*data[6*j+5]+1] = data[6*j+4];
        }
        for (int k = 0; k < nbrBodies;k++) {
          outputFile << t+dt << ", " << mass[k] << ", " << positions[2*k]/AU << ", " <<  positions[2*k+1]/AU << std::endl;
        }
        outputFile.flush();
      }
    #endif

    #ifdef WRITE_QUADTREE
      if(iteration%samplingFreq == 0) {
        outputQTFile << t+dt << ", ";
        qtree.print(outputQTFile, AU, 0.1*AU);
        outputQTFile << endl;
        outputQTFile.flush();
      }
    #endif

    data.clear();

  }

  qtree.cleanQuadtree();

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

  #ifdef WRITE_QUADTREE
    outputQTFile.close();
  #endif


  exit(0);

}
