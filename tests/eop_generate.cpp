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
    cout << "usage: " << argv[0] << " nrows ncols" << endl;
    return -1;
    }
  
  uword nrows = atoi(argv[1]);
  uword ncols = atoi(argv[2]);
  
  cout << "nrows: " << nrows << endl;
  cout << "ncols: " << ncols << endl;
  
  coot::fmat A(nrows, ncols); 
  A.fill(-2);

  std::string kernel = generate_kernel(-A);

  std::cout << "Kernel:\n" << kernel;
  
  return 0;
  }
