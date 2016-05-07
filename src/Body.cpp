#include <iostream>
#include "Body.hpp"

using namespace std;

// Empty Constructor
Body::Body()
{};

// Contructor with mass, positions and velocities
Body::Body(double m, double x, double y, double vx, double vy)
: mass(m), xPos(x), yPos(y), xVel(vx), yVel(vy),
xAcc(0.0), yAcc(0.0), process(0), id(0)
{};

// Contructor with mass, positions, velocities and id
Body::Body(double m, double x, double y, double vx, double vy, int idBody)
: mass(m), xPos(x), yPos(y), xVel(vx), yVel(vy),
xAcc(0.0), yAcc(0.0), process(0), id(idBody)
{};

// Destructor
Body::~Body()
{};

// Print the body
void Body::print(ostream &os) {
  os << this->mass << ", " << this->xPos << ", " << this->yPos;
}

// Print the body
void Body::print(ostream &os, double scale) {
  os << this->mass << ", " << this->xPos/scale << ", " << this->yPos/scale;
}

// Overloading the << operator
ostream& operator<< (ostream & out, Body const& body)
{
  out << "Body: Mass = " << body.mass << ", ";
  out << "Positions = (" <<  body.xPos << ", " <<  body.yPos << "), ";
  out << "Velocities = (" <<  body.xVel << ", " <<  body.yVel << "), ";
  out << "Acceleration = (" <<  body.xAcc << ", " <<  body.yAcc << ")" << endl;
  return out;
};
