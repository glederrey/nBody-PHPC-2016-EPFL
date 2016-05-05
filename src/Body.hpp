#ifndef BODY_H
#define BODY_H

// Class defining body stored in nodes of the tree
class Body {
public:
  // Empty Constructor
  Body();
  // Contructor with mass, positions and velocities
  Body(double m, double x, double y, double vx, double vy);
  // Destructor
  ~Body();

  // Printing function
  void printBody(std::ostream & os);

  // Variables
  double mass;  // Mass of the body
  double xPos;  // X Position of the body
  double yPos;  // Y Position of the body
  double xVel;  // X Velocity of the body
  double yVel;  // Y Velocity of the body
  double xAcc;  // X Acceleration of the body
  double yAcc;  // Y Acceleration of the body
  int process; // Process in which the object is
};

// Overloading the << operator
std::ostream& operator<< (std::ostream & out, Body const& body);

#endif /* BODY_H */
