#include <iostream>
#include <bandicoot>

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
  
  fmat A(nrows,ncols);
  A.print("A:");
  
  A.zeros();
  A.print("A:");
  cout << "trace(A): " << trace(A) << endl;
  
  
  
  A.fill(2);
  A.print("A:");
  cout << "trace(A): " << trace(A) << endl;
  
  for(uword i=0; i<A.n_elem; ++i)
    {
    A(i) = i;
    }
  
  A.print("A:");
  cout << "trace(A): " << trace(A) << endl;
  
  cout << "trace(A+A): " << trace(A+A) << endl;
  
  return 0;
  }
