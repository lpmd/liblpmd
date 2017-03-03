/*
 *
 *
 *
 */

#ifndef __BINARYMATRIX_H__
#define __BINARYMATRIX_H__

class BinaryMatrix
{
 public:
    BinaryMatrix(int N): N(N) 
    { 
     A = new int[N*N];
    }

    BinaryMatrix(const BinaryMatrix & s)
    {
     N = s.LinearSize();
     A = new int[N*N];
     for (int j=0;j<N;++j) 
         for (int i=0;i<N;++i) Set(i, j, s.Get(i, j));
    }

    int Get(int i, int j) const { return A[i+j*N]; }

    void Set(int i, int j, int value) { A[i+j*N] = value; }

    BinaryMatrix & operator=(const BinaryMatrix & m)
    {
     if (&m != this)
     {
      if (m.LinearSize() != N)
      {
       delete [] A;
       A = new int[N*N];
      }
      for (int j=0;j<N;++j) 
         for (int i=0;i<N;++i) Set(i, j, m.Get(i, j));
     }
     return (*this);
    }

    virtual ~BinaryMatrix() { delete [] A; }

    int LinearSize() const { return N; }

 private:
    int N;
    int * A;
};

#endif

