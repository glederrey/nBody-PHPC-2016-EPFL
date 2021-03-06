#ifndef BODY_H
#define BODY_H

#define G 6.674e-11

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

  // Distance between this body and the one in the parameter
  double distance(Body &body);

  // Function that will apply "its" forces to the body in parameter
  void applyForcesOnBody(Body &body);

  // Update the Body with the acceleration and dt
  void update(double &dt);

  // Collide the body in the parameter into this body and return the id that will disappear
  int collide(Body &body);

  // Printing function to get the information (Used to print in file)
  void print(std::ostream & os);

  // Printing function to get the information (Used to print in file)
  void print(std::ostream & os, double const& scale);

  // Variables
  double mass;  // Mass of the body
  double xPos;  // X Position of the body
  double yPos;  // Y Position of the body
  double xVel;  // X Velocity of the body
  double yVel;  // Y Velocity of the body
  double xAcc;  // X Acceleration of the body
  double yAcc;  // Y Acceleration of the body
  int id;       // ID of the body (Useful when we try to distinguish two bodies)
  // Only used by the Barnes-Hut parallel
  int process;  // Process in which the object is

};

// Overloading the << operator (Used to print in file)
std::ostream& operator<< (std::ostream & out, Body const& body);

#endif /* BODY_H */
