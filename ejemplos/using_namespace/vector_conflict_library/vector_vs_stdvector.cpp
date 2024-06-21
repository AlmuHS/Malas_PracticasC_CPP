#include <iostream>
#include <vector>
#include "vector_size.hpp"

using namespace std;

int main(){
  vector<int> v1; //¿es std::vector o dlib/geometry/vector.h?

  cout<<get_size(v1);

  //v1.push_back(4); 

  return 0;
}
