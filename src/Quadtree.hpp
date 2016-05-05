#ifndef QUADTREE_H
#define QUADTREE_H

#include "Body.hpp"
#include "Node.hpp"

class Quadtree {
public:
  // Empty Constructor
  Quadtree();
  // Constructor with position, size and time step
  Quadtree(double x, double y, double w, double h, double timeStep);
  // Constructor with position, size, time step, number of process and precision (theta)
  Quadtree(double x, double y, double w, double h, double timeStep, int numberProcs, double precision);
  // Destructor
  ~Quadtree();

  // Clean the Quadree
  void cleanQuadtree();

  // Find in which quadrant of a node the body should go. Then it inserts it.
  void findQuadrant(Body &body, Node &node);

  // Insert the body into a node
  void insertBody(Body &body, Node &node);

  // Create the leaves with the current node as the parent
  void createLeaves(Node &node);

  // Update the size of the root
  void updateRootSize(double w, double h);

  // Printing function for debug
  void print(std::ostream & os);  

  // Variables
  double dt;        // Time step
  double theta;     // Precision parameter
  int nbrProcs;     // Number of process
  Node root;        // Root of the Quadtree
};

// Overloading the << operator
std::ostream& operator<< (std::ostream & out, Quadtree const& qtree);

#endif /* QUADTREE_H */
