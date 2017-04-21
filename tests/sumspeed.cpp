#include <iostream>
#include <bandicoot>

#include <armadillo>

using namespace std;

int
main(int argc, char** argv)
  {
  cout << coot::coot_version::as_string() << endl;
  
  if(argc < 3)
    {
    cout << "usage: " << argv[0] << " size N" << endl;
    return -1;
    }
  
  size_t size = atoi(argv[1]);
  size_t N    = atoi(argv[2]);
  
  cout << "size: " << size << endl;
  cout << "N:    " << N   << endl;
  
  coot::fmat P(size,size);  P.fill(2);
  coot::fmat Q;
  
  arma::wall_clock timer;
  
  
  cout << "timer.tic()" << endl;
  timer.tic();
  
  for(size_t i=0; i<N; ++i)
    {
    P.get_colwise_sum(Q);
    //P.get_rowwise_sum(Q);
    }
  
  cout << "timer.toc(): " << timer.toc() << endl;
  
  arma::fmat PP(size,size); PP.fill(1);
  arma::fmat QQ;
  
  timer.tic();
  
  for(size_t i=0; i<N; ++i)
    {
    QQ = sum(PP);
    //QQ = sum(PP,1);
    }
  
  cout << "timer.toc(): " << timer.toc() << endl;
  
  return 0;
  }
