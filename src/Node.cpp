#include <iostream>
#include "Node.hpp"

using namespace std;

// Empty Constructor
Node::Node()
{};

// Constructor with position and size
Node::Node(double x, double y, double w, double h)
:xCenter(x), yCenter(y), width(w), height(h),
mass(0.0), xPos(0.0), yPos(0.0),
nbrBodies(0), containsBody(false),
isLeaf(true), depth(0)
{};

// Constructor with parent, position and size
Node::Node(Node *ptr, double x, double y, double w, double h)
:xCenter(x), yCenter(y), width(w), height(h),
mass(0.0), xPos(0.0), yPos(0.0),
nbrBodies(0), containsBody(false),
parent(ptr), isLeaf(true), depth(ptr->depth+1)
{};

// Destructor
Node::~Node()
{}

// Delete the nodes
void Node::deleteNodes() {
  if(!isLeaf) {
    if(northEast->isLeaf) {
      // If this node is not a leaf but the one under is a leaf, we can just
      // delete the pointers of this node.
      delete northEast;
      delete northWest;
      delete southEast;
      delete southWest;
      isLeaf=true;
    } else {
      // If this node isn't a leaf and the one under either, then we have to
      // tell the node under to delete their Node too.
      northEast->deleteNodes();
      northWest->deleteNodes();
      southEast->deleteNodes();
      southWest->deleteNodes();
    }
  } else {
    // If this node is a leaf, we delete it (Except the first one in the tree)
    if(depth>0) {
      delete this;
    }
  }
}

// Add the first body
void Node::addFirstBody(Body &body) {
  // Just copy the stuff from the body
  localBody = body;
  xPos = body.xPos;
  yPos = body.yPos;
  mass = body.mass;
  containsBody = true;
  nbrBodies = 1;
}

// Update the node when another body is inserted
void Node::insertBodyAndUpdateNode(Body &body) {
  // Update the position
  xPos = xPos * mass/(mass+body.mass) + body.xPos * body.mass/(mass+body.mass);
  yPos = yPos * mass/(mass+body.mass) + body.yPos * body.mass/(mass+body.mass);
  mass = (mass * (double)nbrBodies + body.mass)/(double)(nbrBodies+1);
  nbrBodies++;
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
void Node::print(ostream &os, double scale) {

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
void Node::print(ostream &os, double scale, double size) {

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
