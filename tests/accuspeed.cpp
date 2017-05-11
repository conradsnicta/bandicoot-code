#include <iostream>
#include <bandicoot>
#include <armadillo>

using namespace std;

int
main(int argc, char** argv)
  {
  if(argc < 4)
    {
    cout << "usage: " << argv[0] << " n_rows n_cols N" << endl;
    return -1;
    }
  
  const coot::uword n_rows = atoi(argv[1]);
  const coot::uword n_cols = atoi(argv[2]);
  const coot::uword N      = atoi(argv[3]);
  
  cout << "n_rows: " << n_rows << endl;
  cout << "n_cols: " << n_cols << endl;
  cout << "N:      " << N      << endl;
  
  coot::fmat A(n_rows,n_cols);
  
  cout << "element init" << endl;
//   for(coot::uword i=0; i<A.n_elem; ++i)
//     {
//     A(i) = i+1;
//     }
  A.fill(-1);
  
  float junk = accu(A); // force OpenCL runtime to init
  cout << "junk: " << junk << endl;
  
  arma::fmat B(n_rows,n_cols);
  
  A.read_device_mem(B.memptr(), B.n_elem);
  
  arma::wall_clock timer1;
  arma::wall_clock timer2;
  
  double elapsed1 = 0;
  double elapsed2 = 0;
  
  float result_A = 0;
  float result_B = 0;
  
  timer1.tic();
  
  for(coot::uword i=0; i<N; ++i)
    {
    result_A += coot::accu(A);
    //result_A += coot::accu_simple(A);
    }
  
  elapsed1 = timer1.toc();
  
  cout << "elapsed1: " << elapsed1 << "   result_A: " << result_A << endl;
  
  
  timer2.tic();
  
  for(coot::uword i=0; i<N; ++i)
    {
    result_B += arma::accu(B);
    }
  
  elapsed2 = timer2.toc();
  
  cout << "elapsed2: " << elapsed2 << "   result_B: " << result_B << endl;
  
  
  return 0;
  }
