#include <iostream>
#include <bandicoot>

#include <armadillo>

using namespace std;
using namespace coot;

int
main(int argc, char** argv)
  {
  cout << coot::coot_version::as_string() << endl;
  
  if(argc < 4)
    {
    cout << "usage: " << argv[0] << " nrows ncols N" << endl;
    return -1;
    }
  
  uword nrows = atoi(argv[1]);
  uword ncols = atoi(argv[2]);
  uword N     = atoi(argv[3]);
  
  cout << "nrows: " << nrows << endl;
  cout << "ncols: " << ncols << endl;
  cout << "N:     " << N    << endl;
  
  coot::fmat A(nrows,ncols);  A.fill(2);
  coot::fmat B(nrows,ncols);  B.fill(6);
  
  arma::wall_clock timer;
  
  
  cout << "timer.tic()" << endl;
  timer.tic();
  
  
  cout << "START inplace plus scalar" << endl;
  for(uword i=0; i<N; ++i)
    {
    A += 2;
    }
  
  cout << "END inplace plus scalar" << endl;
  
  A.print("A:");
  
  cout << "START inplace plus matrix" << endl;
  
  for(uword i=0; i<N; ++i)
    {
    A += B;
    }
  
  cout << "END inplace plus matrix" << endl;
  
  A.print("A:");
  
  cout << endl;
  cout << "copy via init" << endl;
  coot::fmat C = A;
  C.print("C:");
  
  cout << endl;
  cout << "reset" << endl;
  C.reset();
  C.print("C:");
  
  cout << endl;
  cout << "copy via operator=" << endl;
  C = A;
  C.print("C:");
  
  C.reset();
  C.print("C:");
  
  cout << endl;
  cout << "std::move" << endl;
  C = std::move(A);
  C.print("C:");
  A.print("A:");
  
  
  cout << endl;
  cout << "+=2" << endl;
  C += 2;
  C.print("C:");
  
  cout << endl;
  cout << "-=4" << endl;
  C -= 4;
  C.print("C:");
  
  cout << endl;
  cout << "*=2" << endl;
  C *= 2;
  C.print("C:");
  
  cout << endl;
  cout << "/=2" << endl;
  C /= 2;
  C.print("C:");
  
  cout << endl;
  fmat D(C.n_rows, C.n_cols);
  D.fill(2);
  D.print("D:");
  
  cout << endl;
  cout << "+= D" << endl;
  C += D;
  C.print("C:");
  
  cout << endl;
  cout << "-= D" << endl;
  C -= D;
  C.print("C:");
  
  cout << endl;
  cout << "%= D" << endl;
  C %= D;
  C.print("C:");
  
  cout << endl;
  cout << "/= D" << endl;
  C /= D;
  C.print("C:");
  
  
  fmat F = std::move(D);
  F.print("F:");
  return 0;
  }
