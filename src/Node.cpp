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
    // If this node is a leaf, we delete it
    if(depth!=0) {
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

// Printing function for debug
void Node::print(ostream &os) {
  os << "Node: Position = (" << this->xCenter << ", " << this->yCenter << "), ";
  os << "Height = " << this->height << ", Width = " << this->width << ", ";
  os << "Mass = " << this->mass << ", nbrBodies = " << this->nbrBodies << ", ";
  os << "isLeaf = " << this->isLeaf << ", containsBody = " << this->containsBody << ", ";
  os << "Depth = " << this->depth << endl;

  // Print the leafs
  if(!this->isLeaf) {
    os << "North-East: ";
    northEast->print(os);
    os << endl;
    os << "North-West: ";
    northWest->print(os);
    os << endl;
    os << "South-East: ";
    southEast->print(os);
    os << endl;
    os << "South-West: ";
    southWest->print(os);
    os << endl;
  }
}

// Overloading the << operator
ostream& operator<< (ostream &out, Node const& node)
{
  out << node.xCenter << ", " << node.yCenter << ", " << node.width << ", " << node.height;

  // Print the leafs
  if(!node.isLeaf) {
    out << ", ";
    out << *node.northEast;
    out << ", ";
    out << *node.northWest;
    out << ", ";
    out << *node.southEast;
    out << ", ";
    out << *node.southWest;
  }

  return out;
}
