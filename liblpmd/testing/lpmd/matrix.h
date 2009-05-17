//
//
//

#ifndef __LPMD_MATRIX_H__
#define __LPMD_MATRIX_H__

#include <iostream>
#include <lpmd/array.h>

namespace lpmd
{

class Matrix
{
 public:

   Matrix(long cols, long rows);
   Matrix(const Matrix & m);
   Matrix();
   virtual ~Matrix();

   // 
   long Rows() const;
   long Columns() const;

   //
   double Get(long col, long row) const;
   void Set(long col, long row, double v);

   void SetLabel(long col, std::string lbl);
   std::string GetLabel(long col) const;


   // Operador de asignacion
   Matrix & operator=(const Matrix & m);
   Matrix & operator+=(const Matrix & m);

 private:
   long nr, nc;
   double **values;   
   Array<std::string> col_labels;
};

// Sobrecarga operadores aritmeticos 
Matrix operator+(const Matrix & a, const Matrix & b);
Matrix operator*(const Matrix & a, double f);
Matrix operator/(const Matrix & a, double f);

// Operador de salida
std::ostream & operator<<(std::ostream & os, const Matrix & m);

class MatrixException
{
 public:

};

} // lpmd

#endif

