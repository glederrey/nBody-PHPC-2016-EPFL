#include <iostream>
#include <math.h>
#include "Node.hpp"

using namespace std;

// Empty Constructor
Node::Node()
{};

// Constructor with position and size
Node::Node(double x, double y, double w, double h)
:xCenter(x), yCenter(y), width(w), height(h),
mass(0.0), xPosCM(0.0), yPosCM(0.0),
nbrBodies(0), containsBody(false),
isLeaf(true), depth(0), root(true)
{};

// Constructor with parent, position and size
Node::Node(Node *ptr, double x, double y, double w, double h)
:xCenter(x), yCenter(y), width(w), height(h),
mass(0.0), xPosCM(0.0), yPosCM(0.0),
nbrBodies(0), containsBody(false),
parent(ptr), isLeaf(true), depth(ptr->depth+1), root(false)
{};

// Destructor
Node::~Node()
{}

// Delete the nodes
void Node::deleteNodes() {

  if(!isLeaf) {
    northEast->deleteNodes();
    northWest->deleteNodes();
    southEast->deleteNodes();
    southWest->deleteNodes();
    this->isLeaf = true;
    northEast = NULL;
    northWest = NULL;
    southEast = NULL;
    southWest = NULL;
  }

  if(depth>0) {
    delete this;
  }
}

// Add the first body
void Node::addFirstBody(Body &body) {
  // Just copy the stuff from the body
  localBody = body;
  xPosCM = body.xPos;
  yPosCM = body.yPos;
  mass = body.mass;
  containsBody = true;
  nbrBodies = 1;
}

// Update the node when another body is inserted
void Node::insertBodyAndUpdateNode(Body &body) {
  // Update the position
  xPosCM = (xPosCM * mass + body.xPos * body.mass)/(mass+body.mass);
  yPosCM = (yPosCM * mass + body.yPos * body.mass)/(mass+body.mass);
  mass += body.mass;//(mass * (double)nbrBodies + body.mass)/(double)(nbrBodies+1);
  nbrBodies++;
}

// Distance between this body and the one in the parameter
double Node::distance(Body &body) {
  return sqrt(pow(body.xPos - this->xPosCM, 2.0) + pow(body.yPos - this->yPosCM, 2.0));
}

// Function that will apply "its" forces to the body in parameter
void Node::applyForcesOnBody(Body &body) {

  double dist = this->distance(body);

  double cste = this->mass/pow(dist,3.0);

  body.xAcc += G*(this->xPosCM - body.xPos)*cste;
  body.yAcc += G*(this->yPosCM - body.yPos)*cste;
}

// Remove 1 body from the number of body recursively
void Node::removeBody() {
  nbrBodies--;
  if(!this->root) {
    this->parent->removeBody();
  }
}

// Printing function
void Node::print(ostream &os) {

  os << this->xCenter << ", " << this->yCenter << ", " << this->width << ", " << this->height;

  // Print the leafs
  if(!this->isLeaf) {
    os << ", ";
    this->northEast->print(os);
    os << ", ";
    this->northWest->print(os);
    os << ", ";
    this->southEast->print(os);
    os << ", ";
    this->southWest->print(os);
  }
}

// Printing function
void Node::print(ostream &os, double const& scale) {

  os << this->xCenter/scale << ", " << this->yCenter/scale << ", " << this->width/scale << ", " << this->height/scale;

  if(!this->isLeaf) {
    os << ", ";
    this->northEast->print(os, scale);
    os << ", ";
    this->northWest->print(os, scale);
    os << ", ";
    this->southEast->print(os, scale);
    os << ", ";
    this->southWest->print(os, scale);
  }
}

// Printing function
void Node::print(ostream &os, double const& scale, double const& size) {

  os << this->xCenter/scale << ", " << this->yCenter/scale << ", " << this->width/scale << ", " << this->height/scale;

  // Print the leafs
  if(!this->isLeaf && this->northEast->testSize(size)) {
    os << ", ";
    this->northEast->print(os, scale, size);
    os << ", ";
    this->northWest->print(os, scale, size);
    os << ", ";
    this->southEast->print(os, scale, size);
    os << ", ";
    this->southWest->print(os, scale, size);
  }
}

// Test if the size is big enough for printing
bool Node::testSize(double size) {
  if (0.5*(this->height + this->width) >= size) {
    return true;
  } else {
    return false;
  }
}

// Overloading the << operator
ostream& operator<< (ostream &out, Node const& node)
{
  out << "Node: Position = (" << node.xCenter << ", " << node.yCenter << "), ";
  out << "Height = " << node.height << ", Width = " << node.width << ", ";
  out << "Mass = " << node.mass << ", nbrBodies = " << node.nbrBodies << ", ";
  out << "isLeaf = " << node.isLeaf << ", containsBody = " << node.containsBody << ", ";
  out << "Depth = " << node.depth << endl;

  // Print the leafs
  if(!node.isLeaf) {
    out << "North-East: " << *node.northEast;
    out << "North-West: " << *node.northWest;
    out << "South-East: " << *node.southEast;
    out << "South-West: " << *node.southWest;
  }

  return out;
}
