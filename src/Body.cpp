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
void Body::printBody(ostream &os) {
  os << this->xPos << ", " << this->yPos << ", " << this->process << endl;
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
