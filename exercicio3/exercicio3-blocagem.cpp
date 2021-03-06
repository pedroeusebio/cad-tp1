#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdlib>
#include <chrono>
#include <algorithm>

#define numthreads 8
using namespace std;


void fillArray(float** element, int n, int number) {
  for (int i = 0; i < n; i++ ){
    for(int j = 0; j < n; j++) {
      element[i][j] = number;
    }
  }
}


void multiplyArray(float** result, float** vectorX, float** vectorY, int n, int nb){
  for(int ii = 0; ii < n ; ii += nb){
    for(int jj = 0; jj < n ; jj += nb){
      for(int kk = 0; kk < n; kk += nb){
        for(int i = ii; i < std::min(n, ii + nb); i++){
          for(int j = jj; j < std::min(n, jj + nb); j++){
            for(int k = kk; k < std::min(n, kk +nb); k++) {
              result[i][j] = result[i][j] + vectorX[i][k] * vectorY[k][j];
            }
          }
        }
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

  int n = atol(argv[1]);
  int nb = atol(argv[2]);
  cout << nb << "," << n << endl;
  float **vectorX = new float*[n];
  float **vectorY = new float*[n];
  float **vectorZ = new float*[n];
  for(int i = 0; i < n; i++) {
    vectorX[i] = new float[n];
    vectorY[i] = new float[n];
    vectorZ[i] = new float[n];
    for(int j = 0; j < n; j++) {
      vectorX[i][j] = 2;
      vectorY[i][j] = 1;
      vectorZ[i][j] = 0;
    }
  }

  multiplyArray(vectorZ, vectorX, vectorY, n, nb);
  for(int i = 0; i < n; i++){
    delete [] vectorX[i];
    delete [] vectorY[i];
    delete [] vectorZ[i];
  }

  delete [] vectorX;
  delete [] vectorY;
  delete [] vectorZ;

  return 0;
}
