#ifndef PARALLEL_QUADTREE_H
#define PARALLEL_QUADTREE_H

#include "Body.hpp"
#include "Node.hpp"
#include "Quadtree.hpp"

/*    EXPLANATIONS
This class is the child class of the Quadtree. It's using the same functions
as the class Quadtree to calculate the forces. The new part is mainly the
assignement of the bodies to the nodes. Indeed, we want here to have some
load-balancing, i.e. not all the bodies will be in the same process.
*/

class ParallelQuadtree: public Quadtree {

public:

  // Empty Constructor
  ParallelQuadtree();
  // Constructor with position, size and time step
  ParallelQuadtree(double x, double y, double max, double min, double timeStep, double precision, int nProcs);
  // Destructor
  ~ParallelQuadtree();

  // Find the local nodes for each of the process
  std::vector< std::vector< Node* > > localNodes(int *nbrBodiesPerNode);

  // Assign a node to a process
  void assignNodes(int *nbrBodiesPerNode, std::vector< std::vector< Node* > > &assignedNodes, Node &node);

  // Calculate all the accelerations starting from a node with the given rank
  void calculateAllAccelerationsFromNode(Node &node, int &myRank);

  // Override the function in Quadtree
  // Collect all the bodies in the tree (After updating their positions & velocities)
  void collect(std::vector<double> &data, Node &node);

  // Override of the printing functions

  // Printing function
  void print(std::ostream & os);

  // Printing function
  void print(std::ostream & os, double const& scale);

  // Printing function
  void print(std::ostream & os, double const& scale, double const& size);

  // Variables
  int nbrProcs;                     // Number of process
  int minNbrBodiesPerNode;          // Minimum number of bodies per node
  int maxNbrBodiesPerNode;          // Maximum number of bodies per node
  int minBlockSize;                 // Minimum block size
  int maxBlockSize;                 // Maximum block size
};

// Overloading the << operator
std::ostream& operator<< (std::ostream & out, ParallelQuadtree const& pqtree);

#endif /* PARALLEL_QUADTREE_H */
