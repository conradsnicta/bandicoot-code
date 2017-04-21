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
  coot::fmat X;
  
  // A.print("A:");
  
  cout << "A(0): " << A(0) << endl;

  
  arma::wall_clock timer;
  
//   cout << "sum_all_1" << endl;
//   timer.tic();
//   
//   for(size_t i=0; i<N; ++i)
//     {
//     A.get_sum_all_1(X);
//     }
//   
//   cout << "timer.toc(): " << timer.toc() << endl;
//   X.print("X:");
//   X.fill(-1);
  
  
  cout << "sum_all_2" << endl;
  timer.tic();
  
  for(size_t i=0; i<N; ++i)
    {
    A.get_sum_all_2(X);
    }
  
  cout << "timer.toc(): " << timer.toc() << endl;
  X.print("X:");
  X.fill(-1);


  cout << "sum_all_3" << endl;
  timer.tic();
  
  
  for(size_t i=0; i<N; ++i)
    {
    A.get_sum_all_3(X);
    }
  
  cout << "timer.toc(): " << timer.toc() << endl;
  X.print("X:");
  X.fill(-1);
  
  
//   cout << "sum_all_4" << endl;
//   timer.tic();
//   
//   
//   for(size_t i=0; i<N; ++i)
//     {
//     A.get_sum_all_4(X);
//     }
//   
//   cout << "timer.toc(): " << timer.toc() << endl;
//   X.print("X:");
  
  
  
  
  cout << "single precision arma::accu" << endl;
  
  arma::fmat AA(nrows,ncols); AA.fill(2);
  arma::fmat XX;
  
  timer.tic();
  
  for(size_t i=0; i<N; ++i)
    {
    XX = arma::accu(AA);
    }
  
  cout << "timer.toc(): " << timer.toc() << endl;
  XX.print("XX:");
  

  {
  arma::mat AA(nrows,ncols); AA.fill(2);
  arma::mat XX;
  
  cout << "double precision arma::accu" << endl;
  timer.tic();
  
  for(size_t i=0; i<N; ++i)
    {
    XX = arma::accu(AA);
    }
  
  cout << "timer.toc(): " << timer.toc() << endl;
  XX.print("XX:");
  }
  
  return 0;
  }
