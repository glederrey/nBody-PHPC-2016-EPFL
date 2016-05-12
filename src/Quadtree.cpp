#include <iostream>
#include <math.h>
#include <vector>
#include "Quadtree.hpp"

using namespace std;

// Empty Constructor
Quadtree::Quadtree()
{};

// Constructor with position, size and time step
Quadtree::Quadtree(double x, double y, double max, double min, double timeStep, double precision)
: dt(timeStep), theta(precision), minSize(min)
{
  // We need to put 2 times the size because we want to go from -size to +size
  root = Node(x, y, 2.0*max, 2.0*max);
}

// Destructor
Quadtree::~Quadtree()
{};

// Clean the Quadtree
void Quadtree::cleanQuadtree(){
  // Delete the nodes recursively
  root.deleteNodes();

  root.isLeaf = true;
  root.mass = 0.0;
  root.xPosCM = 0.0;
  root.yPosCM = 0.0;
  root.nbrBodies = 0;
  root.depth = 0;
  root.containsBody = false;
}

// Find in which quadrant of a node the body should go. Then it inserts it.
void Quadtree::findQuadrant(Body &body, Node &node)
{
  if(body.xPos > node.xCenter) { // Body to the East of the node
    if (body.yPos > node.yCenter) { // Body to the North of the node
      // Insert in the North-East node
      insertBody(body, *node.northEast);
    } else { // Body to the South of the node
      // Insert in the South-East node
      insertBody(body, *node.southEast);
    }
  } else { // Body to the West of the node
    if (body.yPos > node.yCenter) { // Body to the North of the node
      // Insert in the North-West node
      insertBody(body, *node.northWest);
    } else { // Body to the South of the node
      // Insert in the South-West node
      insertBody(body, *node.southWest);
    }
  }
}

// Insert the body into a node
void Quadtree::insertBody(Body &body, Node &node) {
  // If the node is a leaf and contains a body. 4 leaves will be created and the
  // two bodies will be replaced
  if(node.isLeaf && node.containsBody) {
    // Test if the node is still big enough
    if(0.5*(node.height+node.width) > minSize) {
      // First we create the leaves for this node
      createLeaves(node);
      // Find where the body in the actual node will go
      findQuadrant(node.localBody, node);
      // Add the body to this node
      node.insertBodyAndUpdateNode(body);
      // Find where this body needs to go
      findQuadrant(body, node);
    } else {

      #ifdef DEBUG
        cout << "Two bodies are colliding!" << endl;
        cout << "  Body being inserted. " << body;
        cout << "  Body in the node.    " << node.localBody;
      #endif

      // We will collide the two bodies.
      node.localBody.collide(body);

      #ifdef DEBUG
        cout << "  Result:              " << node.localBody << endl;
      #endif

      // All the parents node had +1 on their number of body, we need to remove it
      node.parent->removeBody();
    }
  } else if(node.isLeaf && !node.containsBody) {
    // In this case, we can just add the body to the leaf
    node.addFirstBody(body);
  } else {
      // This node is not a Leaf, so we can update the node and continue the search
      node.insertBodyAndUpdateNode(body);
      findQuadrant(body, node);
  }
}

// Create the leaves with the current node as the parent
void Quadtree::createLeaves(Node &node) {
  // This node won't be a leaf anymore
  node.isLeaf = false;
  //node.containsBody = false;

  // Create the 4 leaves (With the parent)
  node.northEast = new Node(&node, node.xCenter + 0.25*node.width, node.yCenter + 0.25*node.height, 0.5*node.width, 0.5*node.height);
  node.northWest = new Node(&node, node.xCenter - 0.25*node.width, node.yCenter + 0.25*node.height, 0.5*node.width, 0.5*node.height);
  node.southEast = new Node(&node, node.xCenter + 0.25*node.width, node.yCenter - 0.25*node.height, 0.5*node.width, 0.5*node.height);
  node.southWest = new Node(&node, node.xCenter - 0.25*node.width, node.yCenter - 0.25*node.height, 0.5*node.width, 0.5*node.height);
}

// Update positions, velocity and acceleration of the bodies
void Quadtree::updateBodies(Node &node) {
  // If the node is a leaf and it contains a body, we can update it
  if (node.isLeaf && node.containsBody) {
    node.localBody.update(dt);
  } else if (!node.isLeaf) {
    // If the node isn't a leaf, we have to update the bodides in its children
    updateBodies(*node.northEast);
    updateBodies(*node.northWest);
    updateBodies(*node.southEast);
    updateBodies(*node.southWest);
  }
}

  // Calculate all the accelerations between a body and a node and all of his children
void Quadtree::calculateAllAccelerations(Body &body, Node &node) {
  // First, we check the distance between the body and the node
  double dist = node.distance(body);
  // Now, if the node isn't a leaf and has a smaller precision than the
  // requested, it will calculate the acceleration from the node without
  // recursion.
  if(!node.isLeaf && 0.5*(node.width+node.height)/dist <= theta) {
    // Calculate the acceleration between a node and a body
    node.applyForcesOnBody(body);
  } else if(!node.isLeaf && 0.5*(node.width+node.height)/dist > theta) {
    // If the precision is big enough, we continue the recursion
    calculateAllAccelerations(body, *node.northEast);
    calculateAllAccelerations(body, *node.northWest);
    calculateAllAccelerations(body, *node.southEast);
    calculateAllAccelerations(body, *node.southWest);
  } else if (node.isLeaf && node.containsBody) {
    node.localBody.applyForcesOnBody(body);
  }
}

// Calculate all the accelerations starting from a node
void Quadtree::calculateAllAccelerationsFromNode(Node &node) {
  // If the node contains a Body and is a leaf, we can calculate the
  // acceleration of its body with the whole tree
  if(node.containsBody && node.isLeaf) {
    calculateAllAccelerations(node.localBody, root);
  } else if (!node.isLeaf) {
    // We call this function recursively
    calculateAllAccelerationsFromNode(*node.northEast);
    calculateAllAccelerationsFromNode(*node.northWest);
    calculateAllAccelerationsFromNode(*node.southEast);
    calculateAllAccelerationsFromNode(*node.southWest);
  }
}

// Collect all the bodies in the tree (After updating their positions & velocities)
void Quadtree::collect(vector<double>& data, Node &node) {
  if(node.isLeaf && node.containsBody) {
    // We put 6 information in this data array
    // mass, positions, velocities and ID
    data.push_back(node.localBody.mass);
    data.push_back(node.localBody.xPos);
    data.push_back(node.localBody.yPos);
    data.push_back(node.localBody.xVel);
    data.push_back(node.localBody.yVel);
    data.push_back(node.localBody.id);
  } else if(!node.isLeaf && node.containsBody) {
    collect(data, *node.northEast);
    collect(data, *node.northWest);
    collect(data, *node.southEast);
    collect(data, *node.southWest);
  }
}

// Check if the body is lost in the space
bool Quadtree::checkIfBodyIsLost(Body &body) {
  // A body is lost if it's outside of the root node.
  if(fabs(body.xPos) > this->root.xCenter + this->root.width/2.0 || fabs(body.yPos) > this->root.yCenter + this->root.height/2.0) {
    return true;
  } else {
    return false;
  }
}

// Printing function
void Quadtree::print(ostream &os) {
  this->root.print(os);
}

// Printing function
void Quadtree::print(ostream &os, double const& scale) {
  this->root.print(os, scale);
}

// Printing function
void Quadtree::print(ostream &os, double const& scale, double const& size) {
  this->root.print(os, scale, size);
}

// Overloading the << operator
ostream& operator<< (ostream & out, Quadtree const& qtree)
{
  out << "Quadtree: Root: " << endl;
  out << qtree.root << endl;
  return out;
};
