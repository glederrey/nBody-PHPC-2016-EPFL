#include <iostream>
#include <math.h>
#include <vector>
#include "ParallelQuadtree.hpp"

using namespace std;

// Empty Constructor
ParallelQuadtree::ParallelQuadtree()
: Quadtree(), nbrProcs(0)
{}

// Constructor with position, size and time step
ParallelQuadtree::ParallelQuadtree(double x, double y, double max, double min, double timeStep, double precision, int nProcs)
: Quadtree(x, y, max, min, timeStep, precision), nbrProcs(nProcs)
{}

// Destructor
ParallelQuadtree::~ParallelQuadtree()
{}

// Find the local nodes for each of the process
vector< vector< Node* > > ParallelQuadtree::localNodes(int *nbrBodiesPerNode) {
  // First, we set the Number of Bodies per Node equal to zero
  for(int i=0; i<nbrProcs; i++) {
    nbrBodiesPerNode[i] = 0;
  }

  // Create the vector of vector of assigned Nodes
  vector< vector< Node* > > assignedNodes(nbrProcs);
  // Then we assign the nodes (Starting from the root)
  assignNodes(nbrBodiesPerNode, assignedNodes, root);
  // Finally we return the vector of vector of assigned Nodes
  return assignedNodes;
}

// Assign a node to a process
void ParallelQuadtree::assignNodes(int *nbrBodiesPerNode, vector< vector< Node* > > &assignedNodes, Node &node) {

  maxNbrBodiesPerNode = ceil(root.nbrBodies/nbrProcs);

  // First, we check if the number of bodies in the current node is bigger than
  // the maximum number of bodies per node. If it's the case and the node isn't
  // a leaf, we asssigned the nodes using the children of the current node
  if(node.nbrBodies > maxNbrBodiesPerNode && !node.isLeaf) {
    assignNodes(nbrBodiesPerNode, assignedNodes, *node.northWest);
    assignNodes(nbrBodiesPerNode, assignedNodes, *node.northEast);
    assignNodes(nbrBodiesPerNode, assignedNodes, *node.southWest);
    assignNodes(nbrBodiesPerNode, assignedNodes, *node.southEast);
  } else if (!node.isLeaf) {
    // If the number of bodies in the node is smaller than the max number of bodies
    // per node but the node is not a leaf, we need to check if we insert it here we
    // insert it in the assignedNodes or if we continue further in the tree.

    int process = 0;
    while( (node.nbrBodies + nbrBodiesPerNode[process]) > maxNbrBodiesPerNode && process < nbrProcs ) {
      process++;
      if(process == nbrProcs) {
        break;
      }
    }

    // If we went up to the the number of procs (last process), we assign the
    // children
    if(process==nbrProcs) {
      assignNodes(nbrBodiesPerNode, assignedNodes, *node.northWest);
      assignNodes(nbrBodiesPerNode, assignedNodes, *node.northEast);
      assignNodes(nbrBodiesPerNode, assignedNodes, *node.southWest);
      assignNodes(nbrBodiesPerNode, assignedNodes, *node.southEast);
    } else {
      // If the process is not the last one, we can add the node to this process
      assignedNodes[process].push_back(&node);
      nbrBodiesPerNode[process] = nbrBodiesPerNode[process] + node.nbrBodies;
    }
  } else if (node.isLeaf && node.containsBody) {
    // If the current node is a leaf and contains a body, we add it to the last proces
    assignedNodes[nbrProcs-1].push_back(&node);
    nbrBodiesPerNode[nbrProcs-1] += 1;
  }
}

// Calculate all the accelerations starting from a node
void ParallelQuadtree::calculateAllAccelerationsFromNode(Node &node, int &myRank) {
  // If the node contains a Body and is a leaf, we can calculate the
  // acceleration of its body with the whole tree
  if(node.containsBody && node.isLeaf) {
    node.localBody.process = myRank;
    calculateAllAccelerations(node.localBody, root);
  } else if (!node.isLeaf) {
    // We call this function recursively
    calculateAllAccelerationsFromNode(*node.northEast, myRank);
    calculateAllAccelerationsFromNode(*node.northWest, myRank);
    calculateAllAccelerationsFromNode(*node.southEast, myRank);
    calculateAllAccelerationsFromNode(*node.southWest, myRank);
  }
}

// Collect all the bodies in the tree (After updating their positions & velocities)
void ParallelQuadtree::collect(vector<double>& data, Node &node) {
  if(node.isLeaf && node.containsBody) {
    // We put 6 information in this data array
    // mass, positions, velocities and ID
    data.push_back(node.localBody.mass);
    data.push_back(node.localBody.xPos);
    data.push_back(node.localBody.yPos);
    data.push_back(node.localBody.xVel);
    data.push_back(node.localBody.yVel);

    int nbrIdSmaller = 0;
    for(unsigned int i=0; i<idCollision.size(); i++) {
      if(idCollision[i] < node.localBody.id) {
        nbrIdSmaller++;
      }
    }

    data.push_back(node.localBody.id-nbrIdSmaller);
    data.push_back(node.localBody.process);
  } else if(!node.isLeaf && node.containsBody) {
    collect(data, *node.northEast);
    collect(data, *node.northWest);
    collect(data, *node.southEast);
    collect(data, *node.southWest);
  }
}

// Printing function
void ParallelQuadtree::print(ostream &os) {
  // Print serial
  this->root.print(os, true);
}

// Printing function
void ParallelQuadtree::print(ostream &os, double const& scale) {
  // Print serial
  this->root.print(os, scale, true);
}

// Printing function
void ParallelQuadtree::print(ostream &os, double const& scale, double const& size) {
  // Print Serial
  this->root.print(os, scale, size, true);
}

// Overloading the << operator
ostream& operator<< (ostream & out, ParallelQuadtree const& pqtree)
{
  out << "ParallelQuadtree: Root: " << endl;
  out << pqtree.root << endl;
  return out;
};
