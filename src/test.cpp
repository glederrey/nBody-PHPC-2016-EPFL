#include<iostream>

#include "Body.cpp"
#include "Node.cpp"

using namespace std;

int main(int argc, char* argv[])
{

  Body body(100, 12.2, 14.4, 102, 10.3);

  cout << body << endl;

  Node node(1, 2, 3.3, 4.4);

  cout << node << endl;

  //exit(0);
}
