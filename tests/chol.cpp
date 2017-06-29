#include <iostream>

#include <armadillo>
#include <bandicoot>

int
main()
  {
  std::cout << coot::coot_version::as_string() << std::endl;
  
  //coot::coot_rt.init();
  coot::coot_rt.init("coot_config.txt", true);
  //coot::coot_rt.init(true);
  //coot::coot_rt.init(2,0);
  
  const coot::uword N = 200;
  arma::dmat A(N,N, arma::fill::randu); A -= 0.5;
  A.print("A:");
  
  arma::dmat X = A*A.t();
  X.print("X:");
  
  arma::dmat R;
  arma::chol(R,X);
  R.print("R:");
  
  std::cout << std::endl;
  std::cout << "----" << std::endl;
  std::cout << std::endl;
  
  coot::dmat AA(A);
  
  AA.print("AA:");
  
  coot::dmat XX = AA*AA.t();
  XX.print("XX:");
  
  coot::dmat RR;
  coot::chol(RR,XX);
  RR.print("RR:");
  
  
  return 0;
  }
