#ifndef NODE_H
#define NODE_H

#include "Body.hpp"

class Node {

public:
  // Empty Constructor
  Node();
  // Constructor with position and size
  Node(double x, double y, double w, double h);
  // Constructor with parent, position and size
  Node(Node *ptr, double x, double y, double w, double h);
  // Destructor
  ~Node();

  // Delete the nodes
  void deleteNodes();

  // Add the first body
  void addFirstBody(Body &body);

  // Update the node when another body is inserted
  void insertBodyAndUpdateNode(Body &body);

  // Variables for the node
  double xCenter;     // X position of the center of the node
  double yCenter;     // Y position of the center of the node
  double width;       // Width of the node
  double height;      // Height of the node
  // Variables for the center of mass
  Body localBody;     // Local body
  double mass;        // Total mass inside the node
  double xPos;        // X position of the center of mass
  double yPos;        // Y position of the center of mass
  int nbrBodies;      // Number of bodies inside the node
  bool containsBody;  // Say if the Node contains a body
  // Variables for the Quadtree
  Node* parent;       // Parent node in the Quadtree
  Node* northEast;    // Node in the North-East direction
  Node* northWest;    // Node in the North-West direction
  Node* southEast;    // Node in the South-East direction
  Node* southWest;    // Node in the South-West direction
  bool isLeaf;        // Say if the tree is a leaf
  int depth;          // Depth of the node in the Quadtree


};

// Overloading the << operator
std::ostream& operator<< (std::ostream & out, Node const& node);

#endif /* NODE_H */
