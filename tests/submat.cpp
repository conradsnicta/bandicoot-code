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
  
  if( (nrows <= 2) || (ncols <= 3) )
    {
    cout << "need larger dimensions" << endl;
    return -1;
    }
    
  fmat A(nrows,ncols);
  A.print("A:");
  
  A.fill(-1);
  A.print("A:");
  
  A(span(1,nrows-2),span(2,ncols-2)).zeros();
  A.print("A:");
  
  A(span(1,nrows-2),span(2,ncols-2)).ones();
  A.print("A:");
  
  A(span(1,nrows-2),span(2,ncols-2)) += 10;
  A.print("A:");
  
  A(span(1,nrows-2),span(2,ncols-2)) -= 10;
  A.print("A:");
  
  A(span(1,nrows-2),span(2,ncols-2)) *= 10;
  A.print("A:");
  
  A(span(1,nrows-2),span(2,ncols-2)) /= 10;
  A.print("A:");
  
  
  
  fmat tmp(nrows-2,ncols-3);
  tmp.fill(123);
  tmp.print("tmp:");
  
  A(span(1,nrows-2),span(2,ncols-2)) = tmp;
  A.print("A:");
  
  A(span(1,nrows-2),span(2,ncols-2)) += tmp;
  A.print("A:");
  
  A(span(1,nrows-2),span(2,ncols-2)) -= tmp;
  A.print("A:");
  
  A(span(1,nrows-2),span(2,ncols-2)) %= tmp;
  A.print("A:");
  
  A(span(1,nrows-2),span(2,ncols-2)) /= tmp;
  A.print("A:");
  
  fmat B(2,3); B.fill(-2);
  B.print("B:");
  
  B = A(span::all,span::all);
  B.print("B:");
  
  B = A(span(1,nrows-2),span(2,ncols-2));
  B.print("B:");
  
  B += A(span(1,nrows-2),span(2,ncols-2));
  B.print("B:");
  
  B -= A(span(1,nrows-2),span(2,ncols-2));
  B.print("B:");
  
  B %= A(span(1,nrows-2),span(2,ncols-2));
  B.print("B:");
  
  B /= A(span(1,nrows-2),span(2,ncols-2));
  B.print("B:");
  
  return 0;
  }
