//
//
//

#include <lpmd/matrix.h>

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace lpmd;

Matrix::Matrix(long cols, long rows)
{
 nc = cols;
 nr = rows;
 values = new double*[rows];
 for (long i=0;i<cols;++i) col_labels.push_back(" ? ");
 for (long j=0;j<rows;++j)
 {
  values[j] = new double[cols]; 
  for (long i=0;i<cols;++i) values[j][i] = 0.0e0;
 }
}

Matrix::Matrix()
{
 nc = 0;
 nr = 0;
 values = NULL;
}

Matrix::Matrix(const Matrix & m)
{
 nc = m.Columns();
 nr = m.Rows();
 values = new double*[nr];
 for (long i=0;i<nc;++i) col_labels.push_back(m.GetLabel(i));
 for (long j=0;j<nr;++j)
 {
  values[j] = new double[nc]; 
  for (long i=0;i<nc;++i) values[j][i] = m.Get(i, j);
 }
}

Matrix::~Matrix() 
{
 if (values != NULL)
 {
  for (long j=0;j<Rows();++j) delete [] values[j];
  delete [] values;
  values = NULL;
 }
}

long Matrix::Rows() const { return nr; }

long Matrix::Columns() const { return nc; }

double Matrix::Get(long col, long row) const { return values[row][col]; }

void Matrix::Set(long col, long row, double v) { values[row][col] = v; }

void Matrix::SetLabel(long col, std::string lbl) { col_labels[col] = lbl; }

std::string Matrix::GetLabel(long col) const { return col_labels[col]; }

Matrix & Matrix::operator=(const Matrix & m)
{
 if ((nr == m.Rows()) && (nc == m.Columns()))
 {
  for (long i=0;i<nc;++i) col_labels.push_back(m.GetLabel(i));
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
  }
  nc = m.Columns();
  nr = m.Rows();
  values = new double*[nr];
  for (long i=0;i<nc;++i) col_labels.push_back(m.GetLabel(i));
  for (long j=0;j<nr;++j)
  {
   values[j] = new double[nc]; 
   for (long i=0;i<nc;++i) values[j][i] = m.Get(i, j);
  }
 }
 return (*this);
}

Matrix & Matrix::operator+=(const Matrix & m)
{
 if ((Rows() == 0) && (Columns() == 0)) return (this->operator=(m));
 else if ((Rows() == m.Rows()) && (Columns() == m.Columns()))
 {
  for (long i=0;i<Columns();++i) col_labels.push_back(m.GetLabel(i));
  for (long j=0;j<m.Rows();++j)
    for (long i=0;i<m.Columns();++i) Set(i, j, Get(i, j) + m.Get(i, j));
  return (*this);
 }
 else 
 {
  std::cerr << "[Warning] Matrix sizes differ, Rows: " << Rows() << " != " << m.Rows() << ", Columns: " << Columns() << " != " << m.Columns() << '\n';
  throw MatrixException();
 }
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
 else 
 {
  std::cerr << "[Warning] Matrix sizes differ, Rows: " << a.Rows() << " != " << b.Rows() << ", Columns: " << a.Columns() << " != " << b.Columns() << '\n';
  throw MatrixException();
 }
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

std::ostream & lpmd::operator<<(std::ostream & os, const Matrix & m)
{
 os << "#"; // << std::setfill(' ');
 for (long i=0;i<m.Columns();++i) os << std::setw(19) << m.GetLabel(i) << " ";
 os << '\n';
 for (long j=0;j<m.Rows();++j)
 {
  for (long i=0;i<m.Columns();++i)
  {
   os.setf(std::ios::fixed);
   //os.setf(std::ios::showpoint);
   os << std::setw(19);
   // os << std::setfill(' ');
   os << m.Get(i, j);
  }
  os << '\n';
 }
 return os;
}

