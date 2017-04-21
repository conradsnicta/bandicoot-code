#include <iostream>
#include <bandicoot>

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
  
  A.print("A:");
  B.print("B:");
  
  coot::fmat C;
  
  {
  cout << endl;
  cout << "=" << endl;
  
  C = A + B;
  C.print("C:");
  
  C = A - B;
  C.print("C:");

  C = A % B;
  C.print("C:");
  
  C = A / B;
  C.print("C:");
  }
  
  
  {
  cout << endl;
  cout << "+=" << endl;
  
  C.fill(1);
  C += A + B;
  C.print("C:");
  
  C.fill(1);
  C += A - B;
  C.print("C:");

  C.fill(1);
  C += A % B;
  C.print("C:");
  
  C.fill(1);
  C += A / B;
  C.print("C:");
  }
  
  
  {
  cout << endl;
  cout << "-=" << endl;
  
  C.fill(1);
  C -= A + B;
  C.print("C:");
  
  C.fill(1);
  C -= A - B;
  C.print("C:");

  C.fill(1);
  C -= A % B;
  C.print("C:");
  
  C.fill(1);
  C -= A / B;
  C.print("C:");
  }
  
  {
  cout << endl;
  cout << "%=" << endl;
  
  C.fill(1);
  C %= A + B;
  C.print("C:");
  
  C.fill(1);
  C %= A - B;
  C.print("C:");

  C.fill(1);
  C %= A % B;
  C.print("C:");
  
  C.fill(1);
  C %= A / B;
  C.print("C:");
  }
  
  {
  cout << endl;
  cout << "/=" << endl;
  
  C.fill(1);
  C /= A + B;
  C.print("C:");
  
  C.fill(1);
  C /= A - B;
  C.print("C:");

  C.fill(1);
  C /= A % B;
  C.print("C:");
  
  C.fill(1);
  C /= A / B;
  C.print("C:");
  }
  
  return 0;
  }
