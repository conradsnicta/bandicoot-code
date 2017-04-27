#include <iostream>
#include <bandicoot>


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
  

  coot::fmat A(nrows,ncols);  A.fill(0);
  
  coot::wall_clock timer;
  
  timer.tic();
  
  for(size_t i=0; i<N; ++i)
    {
    size_t count = 0;
    
    for(size_t col=0; col < ncols; ++col)
    for(size_t row=0; row < nrows; ++row)
      {
      A(row,col) = count;
      count++;
      }
    }
  
  cout << "timer.toc(): " << timer.toc() << endl;
  
  timer.tic();
  
  for(size_t i=0; i<N; ++i)
    {
    size_t count = 0;
    
    for(size_t col=0; col < ncols; ++col)
    for(size_t row=0; row < nrows; ++row)
      {
      A(coot::span(row),coot::span(col)) = count;
      count++;
      }
    }
  
  cout << "timer.toc(): " << timer.toc() << endl;
  
  return 0;
  }
