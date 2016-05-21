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

  // Empty Constructor
  ParallelQuadtree();
  // Constructor with position, size and time step
  ParallelQuadtree(double x, double y, double max, double min, double timeStep, double precision);
  // Destructor
  ~ParallelQuadtree();

  // Variables
  int nbrProcs;                     // Number of process
};

// Overloading the << operator
std::ostream& operator<< (std::ostream & out, Quadtree const& qtree);

#endif /* PARALLEL_QUADTREE_H */
