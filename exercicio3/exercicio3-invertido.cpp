#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdlib>
#include <chrono>
#define numthreads 8
using namespace std;


void fillArray(int** element, long n, int number) {
  for (int i = 0; i < n; i++ ){
    for(int j = 0; j < n; j++) {
      element[i][j] = number;
    }
  }
}


void multiplyArray(int** result, int** vectorX, int** vectorY, long n){
  for(int i = 0 ; i < n;i++){
    for(int j = 0; j < n; j++){
      result[i][j] = 0;
      for(int k=0; k < n; k++) {
        result[i][j] = result[i][j] + vectorX[i][k] * vectorY[k][j];
      }
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
  int **vectorX = new int*[n];
  int **vectorY = new int*[n];
  int **vectorZ = new int*[n];
  for(int i = 0; i < n; i++) {
    vectorX[i] = new int[n];
    vectorY[i] = new int[n];
    vectorZ[i] = new int[n];
  }
  fillArray(vectorX, n, 2);
  fillArray(vectorY, n, 1);

  multiplyArray(vectorZ, vectorX, vectorY, n);

  delete [] vectorX;
  delete [] vectorY;
  delete [] vectorZ;

  return 0;
}
