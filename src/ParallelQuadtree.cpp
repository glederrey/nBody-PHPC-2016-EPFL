// Empty Constructor
ParallelQuadtree::ParallelQuadtree()
: Quadtree(), nbrProcs(0)
{}
// Constructor with position, size and time step
ParallelQuadtree::ParallelQuadtree(double x, double y, double max, double min, double timeStep, double precision, int nProcs)
: Quadtree(x, y, max, min, timeStep, precision), nbrProcs(nProcs)
{}
// Destructor
ParallelQuadtree::~ParallelQuadtree()
{}
