/*
 *
 *
 *
 */

#ifndef __BLOCK_H__
#define __BLOCK_H__

template <class T> class Block
{
 public:
    Block(long int size): n(size)
    {
     p = new T[size];
    }

    virtual ~Block() { delete [] p; }

    Block(const Block<T> & block): n(block.Size())
    {
     p = new T[block.Size()];
     for (long int i=0;i<n;++i) p[i] = block[i];
    }

    Block<T> & operator=(const Block<T> & block)
    {
     if (&block != this)
     {
      delete [] p;
      p = new T[block.Size()];
      for (long int i=0;i<n;++i) p[i] = block[i];
     }
     return (*this);
    }

    const T & operator[](long int i) const { return p[i]; }

    T & operator[](long int i) { return p[i]; }
 
    long int Size() const { return n; }

 private:
    T * p;
    long int n;
};

#endif

