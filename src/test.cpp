#include<iostream>

#include "Body.cpp"
#include "Node.cpp"
#include "Quadtree.cpp"

using namespace std;

int main(int argc, char* argv[])
{

  Body body1(1, 33, 14.4, 102, 10.3);

  Body body2(24.5, 35, 14, 102, 10.3);

  Quadtree qtree(0,0, 100, 100, 0.5);

  qtree.insertBody(body1, qtree.root);
  qtree.insertBody(body2, qtree.root);

  cout << qtree << endl;

  //exit(0);
}
