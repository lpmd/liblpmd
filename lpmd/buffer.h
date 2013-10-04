/*
 *
 *
 *
 */

#ifndef __MEMORY_BUFFER_H__
#define __MEMORY_BUFFER_H__

#include <string.h>
#include <stdlib.h>

#include <lpmd/util.h>

namespace memory
{

 template <typename T> class Buffer
 {
  public:
     Buffer() { area = 0; bsize = 0; managed = true; allowresize = true; }

     Buffer(long size, bool _managed=true)
     {
      area = (T *)(malloc(sizeof(T)*size)); 
      bsize = size;
      managed = _managed;
      allowresize = true;
     }

     Buffer(T * external, long size, bool _managed=false, bool allow_resize=false)
     {
      area = external;
      bsize = size;
      managed = _managed;
      allowresize = allow_resize;
     }

     Buffer(const Buffer<T> & buf)
     {
      bsize = buf.Size();
      if (buf.IsManaged()) 
      {
       area = (T *)(malloc(sizeof(T)*buf.Size()));
       lpmd::ShowWarning("lpmd::Buffer", "Copying "+lpmd::ToString(bsize*sizeof(T))+" bytes (in copy constructor), this may not be optimal.");
       memcpy(area, buf.area, bsize*sizeof(T));
      }
      else
      {
       area = (T *)(buf.RawBuffer());
      }
      managed = buf.managed;
      allowresize = buf.allowresize;
     }

     virtual ~Buffer() { if (managed) free(area); }

     Buffer<T> & operator=(const Buffer<T> & buf)
     {
      if (&buf != this)
      {
       bsize = buf.Size();
       if (buf.IsManaged()) 
       {
        if (managed) free(area);
        area = (T *)(malloc(sizeof(T)*buf.Size()));
        lpmd::ShowWarning("lpmd::Buffer", "Copying "+lpmd::ToString(bsize*sizeof(T))+" bytes (in operator=), this may not be optimal.");
        memcpy(area, buf.area, bsize*sizeof(T));
       }
       else
       {
        area = (T *)(buf.RawBuffer());
       }
       managed = buf.managed;
       allowresize = buf.allowresize;
      }
      return (*this);
     }

     void Resize(long newsize)
     {
      if ((managed) || (allowresize)) 
      {
       area = (T *)(realloc(area, sizeof(T)*newsize));
      }
      bsize = newsize;
     }

     bool IsManaged() const { return managed; }

     long Size() const { return bsize; }

     long Bytes() const { return bsize*sizeof(T); }

     T * RawBuffer() { return area; }

     const T * RawBuffer() const { return area; }

     void Copy(Buffer<T> & src, long bytes=-1)
     {
      if (bytes == -1) bytes = src.Bytes();
      memcpy(area, src.RawBuffer(), bytes);
     }

     void CopyWithOffset(long offset, Buffer<T> & src, long bytes=-1)
     {
      if (bytes == -1) bytes = src.Bytes();
      memcpy(area+offset, src.RawBuffer(), bytes);
     }

     T & operator[](long i) { return area[i]; }
     
     const T & operator[](long i) const { return area[i]; }

  private:
    T * area;
    long bsize;
    bool managed, allowresize;
 };

}

#endif

