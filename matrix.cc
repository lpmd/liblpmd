//
//
//

#include <lpmd/matrix.h>
#include <lpmd/error.h>

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace lpmd;

Matrix::Matrix(long cols, long rows)
{
 columns = cols;
 rows = rows;
 values = new double*[rows];
 label = new const char*[cols];
 for (long i=0;i<cols;++i) label[i] = " ? ";
 for (long j=0;j<rows;++j)
 {
  values[j] = new double[cols]; 
  for (long i=0;i<cols;++i) values[j][i] = 0.0e0;
 }
}

Matrix::Matrix()
{
 columns = 0;
 rows = 0;
 values = NULL;
 label = NULL;
}

Matrix::Matrix(const Matrix & m)
{
 columns = m.Columns();
 rows = m.Rows();
 values = new double*[rows];
 label = new const char*[columns];
 for (long i=0;i<columns;++i) label[i] = m.GetLabel(i);
 for (long j=0;j<rows;++j)
 {
  values[j] = new double[columns]; 
  for (long i=0;i<columns;++i) values[j][i] = m.Get(i, j);
 }
}

Matrix::Matrix(const RawMatrix & m)
{
 columns = m.columns;
 rows = m.rows;
 values = new double*[rows];
 label = new const char*[columns];
 for (long i=0;i<columns;++i) label[i] = m.label[i];
 for (long j=0;j<rows;++j)
 {
  values[j] = new double[columns];
  for (long i=0;i<columns;++i) values[j][i] = m.values[j][i];
 }
}

Matrix::~Matrix()
{
 if (values != NULL)
 {
  for (long j=0;j<Rows();++j) delete [] values[j];
  delete [] values;
  values = NULL;
  delete [] label;
 }
}

long Matrix::Rows() const { return rows; }

long Matrix::Columns() const { return columns; }

double Matrix::Get(long col, long row) const { return values[row][col]; }

void Matrix::Set(long col, long row, double v) { values[row][col] = v; }

void Matrix::SetLabel(long col, const char * lbl) { label[col] = lbl; }

const char * Matrix::GetLabel(long col) const { return label[col]; }

Matrix & Matrix::operator=(const Matrix & m)
{
 if ((rows == m.Rows()) && (columns == m.Columns()))
 {
  delete [] label;
  label = new const char*[columns];
  for (long i=0;i<columns;++i) label[i] = m.GetLabel(i);
  for (long j=0;j<m.Rows();++j)
  {
   for (long i=0;i<m.Columns();++i) Set(i, j, m.Get(i, j));
  }
 }
 else
 {
  if (values != NULL)
  {
   for (long j=0;j<Rows();++j) delete [] values[j];
   delete [] values;
   values = NULL;
   delete [] label;
   label = NULL;
  }
  columns = m.Columns();
  rows = m.Rows();
  values = new double*[rows];
  label = new const char*[columns];
  for (long i=0;i<columns;++i) label[i] = m.GetLabel(i);
  for (long j=0;j<rows;++j)
  {
   values[j] = new double[columns]; 
   for (long i=0;i<columns;++i) values[j][i] = m.Get(i, j);
  }
 }
 return (*this);
}

Matrix & Matrix::operator+=(const Matrix & m)
{
 if ((Rows() == 0) && (Columns() == 0)) return (this->operator=(m));
 else if ((Rows() == m.Rows()) && (Columns() == m.Columns()))
 {
  delete [] label;
  label = NULL;
  for (long i=0;i<Columns();++i) label[i] = m.GetLabel(i);
  for (long j=0;j<m.Rows();++j)
    for (long i=0;i<m.Columns();++i) Set(i, j, Get(i, j) + m.Get(i, j));
  return (*this);
 }
 else throw InvalidOperation("Adding matrices with different size");
}

Matrix lpmd::operator+(const Matrix & a, const Matrix & b)
{
 if ((a.Rows() == b.Rows()) && (a.Columns() == b.Columns()))
 {
  Matrix s = a;
  for (long j=0;j<a.Rows();++j)
    for (long i=0;i<a.Columns();++i) s.Set(i, j, s.Get(i, j) + b.Get(i, j));
  return s;
 }
 else throw InvalidOperation("Adding matrices with different size");
}

Matrix lpmd::operator*(const Matrix & a, double f)
{
 Matrix s = a;
 for (long j=0;j<a.Rows();++j)
   for (long i=0;i<a.Columns();++i) s.Set(i, j, s.Get(i, j)*f);
 return s;
}

Matrix lpmd::operator/(const Matrix & a, double f)
{
 Matrix s = a;
 for (long j=0;j<a.Rows();++j)
   for (long i=0;i<a.Columns();++i) s.Set(i, j, s.Get(i, j)/f);
 return s;
}

