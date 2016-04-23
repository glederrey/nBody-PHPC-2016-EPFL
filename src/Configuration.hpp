#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <map>
#include <vector>

using namespace std;
#define VERBOSE

class Configuration
{
  public:

    // Constructor
    Configuration(const string& fileName);

    // Destructor
    ~Configuration();

    // Process a line of the configuration file
    void process(const string& lineRead);

    // Get an element of the map
    template<typename T> T get(const std::string& key) const;

    // Read the initial mass, positions and velocities
    void prepareInitialValues(const string& fileName);

    // Get methods
    int getNbrBodies();
    vector<double> getInitialMass();
    vector<double> getInitialPositions();
    vector<double> getInitialVelocities();

  private:

    map<string, string> configMap;
    int nbrBodies;
    vector<double> initialMass;
    vector<double> initialPositions;
    vector<double> initialVelocities;

};

#endif /* CONFIGURATION_H */
