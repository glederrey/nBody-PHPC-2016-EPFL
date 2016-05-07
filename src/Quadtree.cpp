#include <iostream>
#include <math.h>
#include <vector>
#include "Quadtree.hpp"

using namespace std;

// Empty Constructor
Quadtree::Quadtree()
{};

// Constructor with position, size and time step
Quadtree::Quadtree(double x, double y, double w, double h, double timeStep, double precision)
: dt(timeStep), theta(precision)
{
  root = Node(x, y, w, h);
}

// Constructor with position, size, time step, number of process and precision (theta)
Quadtree::Quadtree(double x, double y, double w, double h, double timeStep, int numberProcs, double precision)
: dt(timeStep), theta(precision), nbrProcs(numberProcs)
{
  root = Node(x, y, w, h);
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
  root.xPos = 0.0;
  root.yPos = 0.0;
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
    // First we create the leaves for this node
    createLeaves(node);
    // Find where the body in the actual node will go
    findQuadrant(node.localBody, node);
    // Add the body to this node
    node.insertBodyAndUpdateNode(body);
    // Find where this body needs to go
    findQuadrant(body, node);
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
  node.containsBody = false;

  // Create the 4 leaves (With the parent)
  node.northEast = new Node(&node, node.xCenter + 0.25*node.width, node.yCenter + 0.25*node.height, 0.5*node.width, 0.5*node.height);
  node.northWest = new Node(&node, node.xCenter - 0.25*node.width, node.yCenter + 0.25*node.height, 0.5*node.width, 0.5*node.height);
  node.southEast = new Node(&node, node.xCenter + 0.25*node.width, node.yCenter - 0.25*node.height, 0.5*node.width, 0.5*node.height);
  node.southWest = new Node(&node, node.xCenter - 0.25*node.width, node.yCenter - 0.25*node.height, 0.5*node.width, 0.5*node.height);
}

// Distance between a body and a node
double Quadtree::distance(Body &body, Node &node) {
  return sqrt(pow(body.xPos - node.xPos, 2.0) + pow(body.yPos - node.yPos, 2.0));
}

// Distance between two bodies
double Quadtree::distance(Body &body1, Body &body2) {
  return sqrt(pow(body1.xPos - body2.xPos, 2.0) + pow(body1.yPos - body2.yPos, 2.0));
}

// Acceleration induced by a node on a body
void Quadtree::acceleration(Body &body, Node &node) {

  double dist = distance(body, node);

  double cste = node.mass/pow(dist,3.0);

  body.xAcc += G*(node.xPos - body.xPos)*cste;
  body.yAcc += G*(node.yPos - body.yPos)*cste;
}

// Acceleration induced by a body on another body
void Quadtree::acceleration(Body &body1, Body &body2) {

  if(body1.id != body2.id) { // Avoid the same body

    double dist = distance(body1, body2);

    double cste = body2.mass/pow(dist,3.0);

    body1.xAcc += G*(body2.xPos - body1.xPos)*cste;
    body1.yAcc += G*(body2.yPos - body1.yPos)*cste;
  }
}

// Update positions, velocity and acceleration of the bodies
void Quadtree::updateBodies(Node &node) {
  // If the node is a leaf and it contains a body, we can update it
  if (node.isLeaf && node.containsBody) {
    // x += 0.5*a*dt^2 + v*dt
    node.localBody.xPos += 0.5*node.localBody.xAcc*dt*dt + node.localBody.xVel*dt;
    node.localBody.yPos += 0.5*node.localBody.yAcc*dt*dt + node.localBody.yVel*dt;
    // v += a*dt
    node.localBody.xVel += node.localBody.xAcc*dt;
    node.localBody.yVel += node.localBody.yAcc*dt;
    // a = 0
    node.localBody.xAcc = 0.0;
    node.localBody.yAcc = 0.0;
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
  double dist = distance(body, node);
  // Now, if the node isn't a leaf and has a smaller precision than the
  // requested, it will calculate the acceleration from the node without
  // recursion.
  if(!node.isLeaf && 0.5*(node.width+node.height)/dist <= theta) {
    // Calculate the acceleration between a node and a body
    acceleration(body, node);
  } else if(!node.isLeaf && 0.5*(node.width+node.height)/dist > theta) {
    // If the precision is big enough, we continue the recursion
    acceleration(body, *node.northEast);
    acceleration(body, *node.northWest);
    acceleration(body, *node.southEast);
    acceleration(body, *node.southWest);
  } else if (node.isLeaf && node.containsBody) {
    acceleration(body, node.localBody);
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
void Quadtree::collect(vector<double> data, Node &node) {
  if(node.isLeaf && node.containsBody) {
    // We put 5 information in this data array
    // mass, positions and velocities
    data.push_back(node.localBody.mass);
    data.push_back(node.localBody.xPos);
    data.push_back(node.localBody.yPos);
    data.push_back(node.localBody.xVel);
    data.push_back(node.localBody.yVel);
  } else if(!node.isLeaf) {
    collect(data, *node.northEast);
    collect(data, *node.northWest);
    collect(data, *node.southEast);
    collect(data, *node.northWest);
  }
}

// Printing function
void Quadtree::print(ostream &os) {
  this->root.print(os);
}

// Printing function
void Quadtree::print(ostream &os, double scale) {
  this->root.print(os, scale);
}

// Printing function
void Quadtree::print(ostream &os, double scale, double size) {
  this->root.print(os, scale, size);
}

// Overloading the << operator
std::ostream& operator<< (std::ostream & out, Quadtree const& qtree)
{
  out << "Quadtree: Root: " << endl;
  out << qtree.root << endl;
  return out;
};
