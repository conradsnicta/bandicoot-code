#include <iostream>

#include <armadillo>
#include <bandicoot>

int
main()
  {
  std::cout << coot::coot_version::as_string() << std::endl;
  
  coot::coot_rt.init(true);
  
  arma::fmat A(5,5, arma::fill::randu); A -= 0.5;
  arma::fmat B(5,5, arma::fill::randu); B -= 0.5;
  
  A.print("A:");
  B.print("B:");
  
  arma::fmat C = A*B;
  C.print("C:");
  
  
  std::cout << std::endl;
  std::cout << "----" << std::endl;
  std::cout << std::endl;
  
  coot::fmat AA(A);
  coot::fmat BB(B);
  
  AA.print("AA:");
  BB.print("BB:");
  
  //coot::fmat CC = AA+BB;
  coot::fmat CC = AA*BB;
  CC.print("CC:");
  
  return 0;
  }
