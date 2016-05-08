#include <iostream>
#include <math.h>
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

  // Distance between this body and the one in the parameter
double Body::distance(Body &body) {
  return sqrt(pow(body.xPos - this->xPos, 2.0) + pow(body.yPos - this->yPos, 2.0));
}

// Function that will apply "its" forces to the body in parameter
void Body::applyForcesOnBody(Body &body) {
  if(body.id != this->id) { // Avoid the same body

    double dist = this->distance(body);

    double cste = this->mass/pow(dist,3.0);

    body.xAcc += G*(this->xPos - body.xPos)*cste;
    body.yAcc += G*(this->yPos - body.yPos)*cste;
  }
}

void Body::update(double &dt) {
  // v += a*dt
  xVel += xAcc*dt;
  yVel += yAcc*dt;
  // x += v*dt
  xPos += xVel*dt;
  yPos += yVel*dt;
  // a = 0
  xAcc = 0.0;
  yAcc = 0.0;
}

// Print the body
void Body::print(ostream &os) {
  os << this->mass << ", " << this->xPos << ", " << this->yPos;
}

// Print the body
void Body::print(ostream &os, double const& scale) {
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
