/*
 *
 *
 *
 */

#ifndef __LPMD_BASICARRAY_H__
#define __LPMD_BASICARRAY_H__

namespace lpmd
{

 template <typename T> class BasicArray
 {
  public:
    virtual ~BasicArray() { }

    virtual T & operator[](long int i) = 0;

    virtual const T & operator[](long int i) const = 0;

    virtual long int Size() const = 0;

    virtual void Append(const T & x) = 0;

    virtual void AppendUnique(const T & x) = 0;

    virtual void Clear() = 0;

    virtual void Delete(long int i) = 0;

    virtual long int Find(const T & t) = 0;

    virtual bool Contains(const T & t) = 0;
 };

}  // lpmd

#endif

