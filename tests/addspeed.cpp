#include <iostream>
#include <bandicoot>

#include <armadillo>

using namespace std;

int
main(int argc, char** argv)
  {
  cout << coot::coot_version::as_string() << endl;
  
  if(argc < 4)
    {
    cout << "usage: " << argv[0] << " nrows ncols N" << endl;
    return -1;
    }
  
  size_t nrows = atoi(argv[1]);
  size_t ncols = atoi(argv[2]);
  size_t N     = atoi(argv[3]);
  
  cout << "nrows: " << nrows << endl;
  cout << "ncols: " << ncols << endl;
  cout << "N:     " << N    << endl;
  
  coot::fmat A(nrows,ncols);  A.fill(2);
  coot::fmat B(nrows,ncols);  B.fill(4);
  coot::fmat C;
  
  arma::wall_clock timer;
  
  timer.tic();
  
  for(size_t i=0; i<N; ++i)
    {
    C = A + B;
    }
  
  cout << "timer.toc(): " << timer.toc() << endl;
  
  arma::fmat AA(nrows,ncols);  AA.fill(2);
  arma::fmat BB(nrows,ncols);  BB.fill(4);
  arma::fmat CC;
  
  timer.tic();
  
  for(size_t i=0; i<N; ++i)
    {
    CC = AA + BB;
    }
  
  cout << "timer.toc(): " << timer.toc() << endl;
  
  return 0;
  }
