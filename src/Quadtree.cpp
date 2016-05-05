#include <iostream>
#include "Quadtree.hpp"

using namespace std;

// Empty Constructor
Quadtree::Quadtree()
{};

// Constructor with position, size and time step
Quadtree::Quadtree(double x, double y, double w, double h, double timeStep)
: dt(timeStep)
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

// Printing function for debug
void Quadtree::print(ostream &os) {
  os << "Printing Quadtree: Root: ";
  this->root.print(os);
}

// Overloading the << operator
std::ostream& operator<< (std::ostream & out, Quadtree const& qtree)
{
  out << qtree.root << endl;
  return out;
};
