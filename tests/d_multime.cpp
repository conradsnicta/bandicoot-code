#include <iostream>

#include <armadillo>
#include <bandicoot>

int
main(int argc, char** argv)
  {
  std::cout << coot::coot_version::as_string() << std::endl;
  
  if(argc < 3)
    {
    std::cout << "usage: size N" << std::endl;
    return -1;
    }
  
  arma::uword size = atoi(argv[1]);
  arma::uword N    = atoi(argv[2]);
  
  std::cout << "size: " << size << std::endl;
  std::cout << "N:    " << N    << std::endl;
  
  
  //coot::coot_rt.init();
  coot::coot_rt.init("coot_config.txt", true);
  //coot::coot_rt.init(true);
  //coot::coot_rt.init(2,0);
  
  std::cout << std::endl;
  std::cout << "----" << std::endl;
  std::cout << std::endl;
  
  arma::dmat A(size,size, arma::fill::randu); A -= 0.5;
  arma::dmat B(size,size, arma::fill::randu); B -= 0.5;
  arma::dmat X(A.n_rows,B.n_cols);
  
  arma::wall_clock timer1;
  timer1.tic();
  
  for(arma::uword i=0; i<N; ++i)
    {
    X = A*B;
    }
  
  std::cout << "timer1.toc(): " << timer1.toc() << std::endl;
  
  std::cout << std::endl;
  std::cout << "----" << std::endl;
  std::cout << std::endl;
  
  coot::dmat AA(A);
  coot::dmat BB(B);
  coot::dmat XX(AA.n_rows,BB.n_cols);
  
  arma::wall_clock timer2;
  timer2.tic();
  
  for(arma::uword i=0; i<N; ++i)
    {
    XX = AA*BB;
    clFinish(coot::coot_rt.get_cq());
    }
  
  std::cout << "timer2.toc(): " << timer2.toc() << std::endl;
  
  return 0;
  }
