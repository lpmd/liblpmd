/*
 *
 *
 *
 */

#ifndef __LPMD_ARRAY_H__
#define __LPMD_ARRAY_H__

#include <vector>
#include <algorithm>

namespace lpmd
{

template <typename T> class Array: private std::vector<T>
{
 public:
  Array() { }

  Array(long int is): std::vector<T>(is) { }

  Array(long int is, const T & t): std::vector<T>(is, t) { }

  Array(const T * buffer, long int s): std::vector<T>(s)
  {
   for (long int i=0;i<s;++i) (static_cast<std::vector<T> &>(*this))[i] = buffer[i];
  }

  inline T & operator[](long int i) { return (static_cast<std::vector<T> &>(*this)).operator[](i); }

  inline const T & operator[](long int i) const { return (static_cast<const std::vector<T> &>(*this)).operator[](i); }

  inline long int Size() const { return static_cast<const std::vector<T> &>(*this).size(); }

  inline void Append(const T & x) { push_back(x); }
  inline void AppendUnique(const T & x) 
  {
   if (Find(x) == -1) Append(x);
  }

  inline void Clear() { (static_cast<std::vector<T> &>(*this)).clear(); }

  inline void Delete(long int i) 
  { 
   typename std::vector<T>::iterator it=(static_cast<std::vector<T> &>(*this).begin()+i);
   static_cast<std::vector<T> &>(*this).erase(it);
  }

  inline long int Find(const T & t)
  {
   typename std::vector<T>::iterator b=(static_cast<std::vector<T> &>(*this).begin());
   typename std::vector<T>::iterator e=(static_cast<std::vector<T> &>(*this).end());
   typename std::vector<T>::iterator it=std::find(b, e, t);
   if (it != e) return (it-b);
   else return -1;
  }
};

template <typename T> class Array<T &>: private std::vector<T *>
{
 public:
  Array() { }

  inline T & operator[](long int i) { return *((static_cast<std::vector<T *> &>(*this)).operator[](i)); }

  inline const T & operator[](long int i) const { return *((static_cast<const std::vector<T *> &>(*this)).operator[](i)); }

  inline long int Size() const { return (static_cast<const std::vector<T *> &>(*this)).size(); }
  
  inline long int Size() { return (static_cast<std::vector<T *> &>(*this)).size(); }

  inline void Append(T & x) { push_back(&x); }

  inline void AppendUnique(T & x) 
  {
   if (Find(x) == -1) Append(x);
  }

  inline void Clear() { static_cast<std::vector<T *> &>(*this).clear(); }

  inline void Delete(long int i) 
  { 
   typename std::vector<T *>::iterator it=(static_cast<std::vector<T *> &>(*this).begin()+i);
   static_cast<std::vector<T *> &>(*this).erase(it);
  }

  inline long int Find(const T & t)
  {
   typename std::vector<T *>::iterator b=(static_cast<std::vector<T *> &>(*this).begin());
   typename std::vector<T *>::iterator e=(static_cast<std::vector<T *> &>(*this).end());
   typename std::vector<T *>::iterator it=std::find(b, e, &t);
   if (it != e) return (it-b);
   else return -1;
  }
};

} // lpmd

#endif

