#include <array>
#include <iostream>

int main(){
  std::array<int, 4> a1;
  a1 = {1,2,3,4};
  
  std::array<int,4> a2{a1};
  
  for(int i = 0; i < a2.size(); i++){
    std::cout<<a2[i]<<" ";
  }
  
  a2[2] = 6;
  
  std::cout<<"\n";
  for(int data: a2){
    std::cout<<data<<" ";
  }
  
  return 0;
  
}
