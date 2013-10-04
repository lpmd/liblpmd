//
//
//

#ifndef __LPMD_MATRIX_H__
#define __LPMD_MATRIX_H__

typedef struct
{
 long rows, columns;
 double ** values;   
 const char ** label;
} RawMatrix;

#ifdef __cplusplus
namespace lpmd
{
 class Matrix: private RawMatrix
 {
  public:
   Matrix(long cols, long rows);
   Matrix(const Matrix & m);
   Matrix(const RawMatrix & m);
   Matrix();
   virtual ~Matrix();

   // 
   long Rows() const;
   long Columns() const;

   //
   double Get(long col, long row) const;
   void Set(long col, long row, double v);

   void SetLabel(long col, const char * lbl);
   const char * GetLabel(long col) const;


   // Operador de asignacion
   Matrix & operator=(const Matrix & m);
   Matrix & operator+=(const Matrix & m);

 };

 // Sobrecarga operadores aritmeticos 
 Matrix operator+(const Matrix & a, const Matrix & b);
 Matrix operator*(const Matrix & a, double f);
 Matrix operator/(const Matrix & a, double f);
} // lpmd
#endif

#endif

