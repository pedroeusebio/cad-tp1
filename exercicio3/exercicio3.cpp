#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdlib>
#include <chrono>
#define numthreads 8
using namespace std;


void fillArray(char* element, long n, int number) {
  for (int i = 0; i < n; i++ ){
    for(int j = 0; j < n; j++) {
      element[i][j] = number;
    }
  }
}

int main( int argc, const char* argv[]) {
  if(argc < 2) {
    cout<<"Usage is:\n"<<argv[0]<<
      " <amount of numbers to generate>"<<
      endl;
    return 0;
  }
  
  long n = atol(argv[1]);
  char *vector =  new char[n][n];
  fillArray(vector, n, 10);
  for (int i = 0; i < n; i++ ){
    for(int j = 0; j < n; j++) {
      cout << vector[i][j];
    }
  }
}
