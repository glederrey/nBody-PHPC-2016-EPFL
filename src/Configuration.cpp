#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>

#include "Configuration.hpp"

// Contructor to load the config file
Configuration::Configuration(const std::string& fileName) {
  ifstream file;
  file.open(fileName.c_str());
  if(!file) {
    cerr << "[ConfigFile] Error in openening file " << fileName << endl;
    exit(1);
  }
  try {
    string lineRead;
    while(getline(file, lineRead)) {
      process(lineRead);
    }
    file.close();
  }catch(std::string e){
    file.close();
    throw e;
  }
}

// Empty Destructor
Configuration::~Configuration(){}

// Process a line of the configuration file
void Configuration::process(const string& lineRead) {
  size_t commentPosition=lineRead.find('#',0);
  if(commentPosition!=0) {
    size_t spacePosition=lineRead.find(' ',0);
  	if(spacePosition!=string::npos){
  	  cerr << "ConfigFile object do not support character ' ' at position " << spacePosition << " in line '" << lineRead << endl;
      exit(1);
  	}
  	size_t equalPosition = lineRead.find('=',1);
  	if(equalPosition==string::npos){
  	  cerr << "Line without '=' " << lineRead << endl;
      exit(1);
  	}else{
  	  string key=lineRead.substr(0,equalPosition);
  	  string value=lineRead.substr(equalPosition+1,lineRead.length());
  	  std::map<std::string, std::string>::const_iterator val = configMap.find(key);
  	  if ( val != configMap.end() ) {
  	    configMap.erase(key);
  	  }
  	  configMap.insert( std::pair<std::string, std::string>(key,value) );
  	}
  }
}

// Get an element of the map
template<typename T> T Configuration::get(const std::string& key) const{
   std::map<std::string, std::string>::const_iterator val = configMap.find(key);
   T out;
   if (val != configMap.end()) {
     std::istringstream iss(val->second);
     iss >> out;
   }else{
     cerr <<  "[ConfigFile] Key missing from file, key= " << key << endl;
     exit(1);
   }
   return out;
 }

void Configuration::prepareInitialValues(const string& fileName) {
  #ifdef DEBUG
    cout << "Please wait while the mass, positions and velocity are loaded." << endl;
  #endif

  // Read the file with the initial values
  ifstream file;
  file.open(fileName.c_str());
  if(!file) {
    cerr << "[ConfigFile] Error in openening file " << fileName << endl;
    exit(1);
  }

  vector<double> values(0);
  string lineRead;
  double val;

  while(std::getline(file, lineRead, ','))
  {
    // Search if there's a comment
    size_t commentPosition = lineRead.find('#',0);
    if(commentPosition!=0) {
      stringstream stream(lineRead);
      while(stream >> val)
      {
        values.push_back(val);
      }
    }
  }

  file.close();
  #ifdef DEBUG
    cout << "The file with the initial values has been loaded. The different vectors will now be created." << endl;
  #endif
  nbrBodies = values.size() / 5;

  if (nbrBodies < 0) {
    cerr << "Wrong number of bodies." << endl;
    exit(1);
  }

  for (int i=0; i<nbrBodies; i++) {
    initialMass.push_back(values[i*5]);
    initialPositions.push_back(values[i*5+1]);
    initialPositions.push_back(values[i*5+2]);
    initialVelocities.push_back(values[i*5+3]);
    initialVelocities.push_back(values[i*5+4]);
  }

  #ifdef DEBUG
    cout << "Loading the initial values is finished." << endl;
  #endif
 }

int Configuration::getNbrBodies() {
   return nbrBodies;
}


vector<double> Configuration::getInitialMass() {
  return initialMass;
}

vector<double> Configuration::getInitialPositions() {
  return initialPositions;
}

vector<double> Configuration::getInitialVelocities() {
  return initialVelocities;
}
