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
  
  coot::fmat  A(n_rows,n_cols);
  coot::fmat AA;
  
  cout << "element init" << endl;
  A.fill(123);
  
  arma::fmat  B(n_rows,n_cols);
  arma::fmat BB;
  
  A.read_device_mem(B.memptr(), B.n_elem);
  
  arma::wall_clock timer1;
  arma::wall_clock timer2;
  
  double elapsed1 = 0;
  double elapsed2 = 0;
  
  AA = coot::sqrt(A);  // warm up OpenCL runtime
  
  timer1.tic();
  
  for(coot::uword i=0; i<N; ++i)
    {
    //AA = coot::sqrt(A);
    //AA = coot::exp(A);
    AA = coot::log(A);
    }
  
  elapsed1 = timer1.toc();
  
  cout << "elapsed1: " << elapsed1 << "   accu(AA): " << accu(AA) << endl;
  
  
  timer2.tic();
  
  for(coot::uword i=0; i<N; ++i)
    {
    //BB = arma::sqrt(B);
    //BB = arma::exp(B);
    BB = arma::log(B);
    }
  
  elapsed2 = timer2.toc();
  
  cout << "elapsed2: " << elapsed2 << "   accu(BB): " << accu(BB) << endl;
  
  
  return 0;
  }
