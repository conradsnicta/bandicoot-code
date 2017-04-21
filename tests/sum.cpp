#include <iostream>
#include <bandicoot>

using namespace std;
using namespace coot;

// bandicoot_runtime_t bandicoot_runtime;

int
main()
  {
  fmat P(8,9);  P.fill(0);
  fmat Q(6,5);  Q.fill(-1);
  
  for(uword i=0; i<P.n_elem; ++i)
    {
    P(i) = i+1;
    }
  
  P.print("P:");
  Q.print("Q:");
  
  P.get_colwise_sum(Q);
  Q.print("Q:");
  
  P.get_rowwise_sum(Q);
  Q.print("Q:");
  
  P.get_colwise_sum_submat(Q, 1, 1, P.n_rows-2, P.n_cols-2 );
  Q.print("Q:");
  
  P.get_rowwise_sum_submat(Q, 1, 1, P.n_rows-2, P.n_cols-2 );
  Q.print("Q:");
  

  return 0;
  }
