#include <iostream>
#include <bandicoot>

#include <armadillo>

using namespace std;
using namespace coot;

int
main(int argc, char** argv)
  {
  cout << "Bandicoot version " << coot_version::as_string() << endl;
  
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
  
  if( (nrows <= 2) || (ncols <= 2) )
    {
    cout << "need larger dimensions" << endl;
    return -1;
    }
    
  fmat A(nrows,ncols);  A.fill(-1);
  //A.print("A:");
  
  fmat tmp(nrows-2,ncols-2);
  tmp.fill(123);
  //tmp.print("tmp:");
  
  for(uword i=0; i<N; ++i)
    {
    A(span(1,nrows-2),span(1,ncols-2)) = tmp;
    }
  
  //A.print("A:");
  
  return 0;
  }
