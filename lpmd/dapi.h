/*
 *
 *
 *
 */

#include <lpmd/capi.h>

#define GetArray(...) ASet_GetArray(aset, __VA_ARGS__)

#define GetArrays(...) ASet_GetArraysByName(aset, __VA_ARGS__)

#define GetTotalArray(...) ASet_GetTotalArray(aset, __VA_ARGS__)

#define GetTotalArrays(...) ASet_GetTotalArrays(aset, __VA_ARGS__)

#define LogMessage(format, ...) ASet_LogMessage(aset, format, __VA_ARGS__)

#define AddAtom(...) ASet_AddAtom(aset, cell, __VA_ARGS__)

#define Distance(dr, pi, pj) (Cell_OrthoDistance(cell, (dr), (pi), (pj)))

#define SquareModule(vec) (((vec)[0]*(vec)[0]+(vec)[1]*(vec)[1]+(vec)[2]*(vec)[2]))

#define CenterOfMass(cm) (ASet_GetCenterOfMass(aset, cm))

#define Cartesian(fc, cart) (Cell_GetCartesian(cell, fc, cart))

#define VectorComponent(x, y, z, q) ((q == 0) ? (x) : ((q == 1) ? (y) : (z)))

#define InsideNode(vec) Cell_AtomBelongsToRank(aset, cell, (vec))

#define Random() mt_drand()

// This is where all the Vector() magic happens :)
typedef double t_innervector[3];

inline double * Vector(double * v_array, int nreg, double x, double y, double z)
{
 v_array[nreg*3+0] = x;
 v_array[nreg*3+1] = y;
 v_array[nreg*3+2] = z;
 return (v_array+nreg*3);
}

inline static const char * VectorFormat(const char * format, double * v)
{
 static char vec_format_buffer[1024];
 snprintf(vec_format_buffer, 1024, format, v[0], v[1], v[2]);
 return (const char *)(vec_format_buffer);
}

inline double Dot(double * a, double * b)
{
 return (a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
}

inline double * Cross(double * v_array, int nreg, double * a, double * b)
{
 double cx = b[1]*a[2]-b[2]*a[1];
 double cy = b[2]*a[0]-b[0]*a[2];
 double cz = b[0]*a[1]-b[1]*a[0];
 return Vector(v_array, nreg, cx, cy, cz);
}

// Matrix allocation
inline RawMatrix MatrixFromHeap(int nc, int nr)
{
 RawMatrix tmp;
 Matrix_Initialize(&tmp, nc, nr);
 return tmp;
}

// Parsing functions for arguments
inline void ParseIntegerKeyword(const char * value, long * ret) { *ret = atoi(value); }

inline void ParseRealKeyword(const char * value, double * ret) { *ret = atof(value); }

inline void ParseVectorKeyword(const char * value, t_innervector * ret)
{
 char vbuffer[10240];
 memcpy(vbuffer, value, strlen(value)+1);
 for (int i=0;i<strlen(vbuffer);++i)
 {
  if (((vbuffer[i] == '<') || (vbuffer[i] == '>')) || (vbuffer[i] == ',')) vbuffer[i] = ' ';
 }
 for (int q=0;q<3;++q)
 {
  double x;
  sscanf(vbuffer, "%lf", &x);
  (*ret)[q] = x;
 }
}

inline void ParseStringKeyword(const char * value, const char ** ret)
{
 int l = strlen(value);
 *ret = (const char *)(malloc(sizeof(char)*(l+1)));
 memcpy((void *)(*ret), (void *)(value), l+1);
}

inline void ParseTagKeyword(const char * value, int * ret) 
{ 
 int k = LookupTag(value);
 if (k != -1) *ret = k;
 else *ret = RegisterTag(value);
}

inline void ParseBooleanKeyword(const char * value, int * ret) { *ret = (strcmp(value, "true") ? 0 : 1); }

/* Memory management */

inline void Deallocate(void * ptr)
{
 // FIXME: Needs to check with the garbage collector, when we have one!
 free(ptr);
}

