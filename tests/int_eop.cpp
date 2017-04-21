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
    cout << "usage: " << argv[0] << " nrows ncols N" << endl;
    return -1;
    }
  
  uword nrows = atoi(argv[1]);
  uword ncols = atoi(argv[2]);
  uword N     = atoi(argv[3]);
  
  cout << "nrows: " << nrows << endl;
  cout << "ncols: " << ncols << endl;
  cout << "N:     " << N    << endl;
  
  coot::imat A(nrows,ncols);  A.fill(2);
  coot::imat B(nrows,ncols);  B.fill(6);
  
  A.print("A:");
  B.print("B:");
  
  coot::imat C;
  
  {
  cout << endl;
  cout << "=" << endl;
  
  C = +B;
  C.print("1 C:");
  
  C = -B;
  C.print("2 C:");

  C = 2 + B;
  C.print("3 C:");
  
  C = B + 2;
  C.print("4 C:");
  
  C = 2 - B;
  C.print("5 C:");
  
  C = B - 2;
  C.print("6 C:");
  
  C = 2 * B;
  C.print("7 C:");
  
  C = B * 2;
  C.print("8 C:");
  
  C = 2 / B;
  C.print("9 C:");
  
  C = B / 2;
  C.print("10 C:");
  
  C = square(B);
  C.print("11 C:");
  
  C = sqrt(B);
  C.print("12 C:");
  }
  
  return -1;
  
  {
  cout << endl;
  cout << "+=" << endl;
  
  C.ones(B.n_rows, B.n_cols);
  C += +B;
  C.print("1 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C += -B;
  C.print("2 C:");

  C.ones(B.n_rows, B.n_cols);
  C += 2 + B;
  C.print("3 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C += B + 2;
  C.print("4 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C += 2 - B;
  C.print("5 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C += B - 2;
  C.print("6 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C += 2 * B;
  C.print("7 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C += B * 2;
  C.print("8 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C += 2 / B;
  C.print("9 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C += B / 2;
  C.print("10 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C += square(B);
  C.print("11 C:");
  }
  
  
  {
  cout << endl;
  cout << "-=" << endl;
  
  C.ones(B.n_rows, B.n_cols);
  C -= +B;
  C.print("1 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C -= -B;
  C.print("2 C:");

  C.ones(B.n_rows, B.n_cols);
  C -= 2 + B;
  C.print("3 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C -= B + 2;
  C.print("4 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C -= 2 - B;
  C.print("5 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C -= B - 2;
  C.print("6 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C -= 2 * B;
  C.print("7 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C -= B * 2;
  C.print("8 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C -= 2 / B;
  C.print("9 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C -= B / 2;
  C.print("10 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C -= square(B);
  C.print("11 C:");
  }
  
  
  {
  cout << endl;
  cout << "%=" << endl;
  
  C.ones(B.n_rows, B.n_cols);
  C %= +B;
  C.print("1 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C %= -B;
  C.print("2 C:");

  C.ones(B.n_rows, B.n_cols);
  C %= 2 + B;
  C.print("3 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C %= B + 2;
  C.print("4 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C %= 2 - B;
  C.print("5 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C %= B - 2;
  C.print("6 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C %= 2 * B;
  C.print("7 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C %= B * 2;
  C.print("8 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C %= 2 / B;
  C.print("9 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C %= B / 2;
  C.print("10 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C %= square(B);
  C.print("11 C:");
  }
  
  
  {
  cout << endl;
  cout << "/=" << endl;
  
  C.ones(B.n_rows, B.n_cols);
  C /= +B;
  C.print("1 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C /= -B;
  C.print("2 C:");

  C.ones(B.n_rows, B.n_cols);
  C /= 2 + B;
  C.print("3 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C /= B + 2;
  C.print("4 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C /= 2 - B;
  C.print("5 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C /= B - 2;
  C.print("6 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C /= 2 * B;
  C.print("7 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C /= B * 2;
  C.print("8 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C /= 2 / B;
  C.print("9 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C /= B / 2;
  C.print("10 C:");
  
  C.ones(B.n_rows, B.n_cols);
  C /= square(B);
  C.print("11 C:");
  }
  
  
  return 0;
  }
