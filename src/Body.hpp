#ifndef BODY_H
#define BODY_H

// Class defining body stored in nodes of the tree
class Body {
public:
  // Empty Constructor
  Body();
  // Contructor with mass, positions and velocities
  Body(double m, double x, double y, double vx, double vy);
  // Contructor with mass, positions, velocities and id
  Body(double m, double x, double y, double vx, double vy, int idBody);
  // Destructor
  ~Body();

  // Printing function to get the information (Used to print in file)
  void print(std::ostream & os);

  // Printing function to get the information (Used to print in file)
  void print(std::ostream & os, double scale);

  // Variables
  double mass;  // Mass of the body
  double xPos;  // X Position of the body
  double yPos;  // Y Position of the body
  double xVel;  // X Velocity of the body
  double yVel;  // Y Velocity of the body
  double xAcc;  // X Acceleration of the body
  double yAcc;  // Y Acceleration of the body
  int process;  // Process in which the object is
  int id;       // ID of the body (Useful when we try to distinguish two bodies)
};

// Overloading the << operator (Used to print in file)
std::ostream& operator<< (std::ostream & out, Body const& body);

#endif /* BODY_H */
