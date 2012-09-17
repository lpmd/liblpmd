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
 nc = cols;
 nr = rows;
 values = new double*[rows];
 for (long i=0;i<cols;++i) col_labels.Append(" ? ");
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
 for (long i=0;i<nc;++i) col_labels.Append(m.GetLabel(i));
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

double Matrix::Det() const
{
 //only for 2x2 and 3x3 matrix, no more sofisticated algorithms... yet.
 if (nr!=nc || (nc>3 || nr >3)) return -1;
 if (nc==2) return Get(0,0)*Get(1,1)-Get(0,1)*Get(1,0);
 else if(nc==3)
 {
  double a = Get(0,0)*(Get(1,1)*Get(2,2)-Get(1,2)*Get(2,1));
  double b = Get(0,1)*(Get(1,0)*Get(2,2)-Get(1,2)*Get(2,0));
  double c = Get(0,2)*(Get(1,0)*Get(2,1)-Get(1,1)*Get(2,0));
  return a-b+c;
 }
 else return -1;
}

void Matrix::Inverse()
{
 if(nc!=nr || (nc>3 || nr >3))
 {
  std::cerr << "Error with inverse matrix operation" << '\n';
  exit(0);
 }
 double det = (*this).Det();
 if(det == 0)
 {
  std::cerr << "Error with inverse matrix operation, det = 0" << '\n';
  exit(0);
 }
 if(nc==2)
 {
  double ap = Get(1,1)/det;
  double bp = -Get(0,1)/det;
  double cp = -Get(1,0)/det;
  double dp = Get(0,0)/det;
  (*this).Set(0,0,ap);
  (*this).Set(0,1,bp);
  (*this).Set(1,0,cp);
  (*this).Set(1,1,dp);
 }
 else if(nc==3)
 {
  //link : http://www.dr-lex.be/random/matrix_inv.html
  double ap =  Get(2,2)*Get(1,1)-Get(2,1)*Get(1,2); //a33a22-a32a23
  double bp =-(Get(2,2)*Get(0,1)-Get(2,1)*Get(0,2));//-(a33a12-a32a13)
  double cp =  Get(1,2)*Get(0,1)-Get(1,1)*Get(0,2); //a23a12-a22a13
  double dp =-(Get(2,2)*Get(1,0)-Get(2,0)*Get(1,2));//-(a33a21-a31a23)
  double ep =  Get(2,2)*Get(0,0)-Get(2,0)-Get(0,2); //a33a11-a31a13
  double fp =-(Get(1,2)*Get(0,0)-Get(1,0)*Get(0,2));//-(a23a11-a21a13) 
  double gp =  Get(2,1)*Get(1,0)-Get(2,0)*Get(1,1); //a32a21-a31a22
  double hp =-(Get(2,1)*Get(0,0)*Get(2,0)*Get(0,1));//-(a32a11-a31a12)
  double ip =  Get(1,1)*Get(0,0)-Get(1,0)*Get(0,1); //a22a11-a21a12
  (*this).Set(0,0,ap/det);
  (*this).Set(0,1,bp/det);
  (*this).Set(0,2,cp/det);
  (*this).Set(1,0,dp/det);
  (*this).Set(1,1,ep/det);
  (*this).Set(1,2,fp/det);
  (*this).Set(2,0,gp/det);
  (*this).Set(2,1,hp/det);
  (*this).Set(2,2,ip/det);
 }
 else 
 {
  std::cerr << "Error with inverse matrix operation" << '\n';
  exit(0);
 }
}

Matrix & Matrix::operator=(const Matrix & m)
{
 if ((nr == m.Rows()) && (nc == m.Columns()))
 {
  for (long i=0;i<nc;++i) col_labels.Append(m.GetLabel(i));
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
  for (long i=0;i<nc;++i) col_labels.Append(m.GetLabel(i));
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
  for (long i=0;i<Columns();++i) col_labels.Append(m.GetLabel(i));
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

std::ostream & lpmd::operator<<(std::ostream & os, const Matrix & m)
{
 os << "#"; // << std::setfill(' ');
 for (long i=0;i<m.Columns();++i) os << std::setw(19) << m.GetLabel(i) << " ";
 os << '\n';
 for (long j=0;j<m.Rows();++j)
 {
  for (long i=0;i<m.Columns();++i)
  {
   os.setf(std::ios::scientific);
   //os.setf(std::ios::showpoint);
   os.precision(15);
   //os << os.width(20);
   os << "   " << m.Get(i, j);
   /*
   os.setf(std::ios::fixed);
   //os.setf(std::ios::showpoint);
   os << std::setw(19);
   // os << std::setfill(' ');
   os << m.Get(i, j);
   */
  }
  os << '\n';
 }
 return os;
}

