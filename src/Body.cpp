#include <iostream>
#include "Body.hpp"

using namespace std;

// Empty Constructor
Body::Body()
{};

// Contructor with mass, positions and velocities
Body::Body(double m, double x, double y, double vx, double vy)
: mass(m), xPos(x), yPos(y), xVel(vx), yVel(vy),
xAcc(0.0), yAcc(0.0), process(0)
{};

// Destructor
Body::~Body()
{};

// Print the body (DEBUG)
void Body::print(ostream &os) {
  os << "Body: Mass = " << this->mass << ", ";
  os << "Positions = (" <<  this->xPos << ", " <<  this->yPos << "), ";
  os << "Velocities = (" <<  this->xVel << ", " <<  this->yVel << "), ";
  os << "Acceleration = (" <<  this->xAcc << ", " <<  this->yAcc << ")" << endl;
}

// Overloading the << operator
ostream& operator<< (ostream & out, Body const& body)
{
  out << body.mass << ", " << body.xPos << ", " << body.yPos;
  return out;
};
