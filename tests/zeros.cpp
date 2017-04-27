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
  A.print("A:");
  
  A.zeros();
  A.print("A:");

  A.ones();
  A.print("A:");
    
  A.eye();
  A.print("A:");
  
  
  return 0;
  }
