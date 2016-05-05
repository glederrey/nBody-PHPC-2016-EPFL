#include<iostream>

#include "Body.cpp"
#include "Node.cpp"
#include "Quadtree.cpp"

using namespace std;

int main(int argc, char* argv[])
{

  Body body1(1, 33, 14.4, 102, 10.3);

  body1.print(cout);

  Body body2(24.5, 35, 14, 102, 10.3);
  Body body3(24.5, -30, 30, 102, 10.3);
  Body body4(24.5, -10, 10, 102, 10.3);

  Quadtree qtree(0,0, 100, 100, 0.5);

  qtree.insertBody(body1, qtree.root);
  qtree.insertBody(body2, qtree.root);
  qtree.insertBody(body3, qtree.root);
  qtree.insertBody(body4, qtree.root);

  qtree.print(cout);
  cout << qtree << endl;

  //exit(0);
}
