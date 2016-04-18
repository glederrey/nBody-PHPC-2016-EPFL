#include <iostream>
#include <fstream>
#include <vector>

#include "Configuration.cpp"

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
  string outputFile = conf.get<string>("outputFile");

  conf.prepareInitialValues(initialFile);
  // Get the masses, positions and velocities for each object.
  int nbrBodies = conf.getNbrBodies();
  vector<double> masses = conf.getInitialMass();
  vector<double> positions = conf.getInitialPositions();
  vector<double> velocities = conf.getInitialVelocities();

  cout << "Number of bodies: " << nbrBodies << endl;

  for (int i=0; i<velocities.size(); i++) {
    cout << velocities[i] << endl;
  }
/*


  // Read the config file
  std::vector<double> configData;
  configData = readConfigFile(argv[1]);

  // Get the masses, positions and velocities for each object.
  std::vector<double> masses;
  std::vector<double> positions;
  std::vector<double> velocities;


  for(unsigned int i=0; i<configData.size(); i++) {
    std::cout << configData[i] << std::endl;
  }*/
}
