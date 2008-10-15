//
//
//

#include <lpmd/color.h>

using namespace lpmd;

Color::Color (double r, double g, double b):R(r),G(g),B(b){}
Color::~Color(){}

/************************************** Funciones Sobre un Color ****************************************************/
void Color::SetRed(double x)		{ R = x; }
void Color::SetGreen(double x)		{ G = x; }
void Color::SetBlue(double x)		{ B = x; }

double Color::GetRed(void) 	const{	return R; }
double Color::GetGreen(void)	const{	return G; }
double Color::GetBlue(void) 	const{	return B; }
/*****************************************************************************************************************************************/

/*****************************************************************************************/
/*********** SOBRECARGA DE OPERADORES *************/
/*****************************************************************************************/
Color lpmd::operator+(const Color &z, const Color &y){Color x(z.GetRed()+y.GetRed(), z.GetGreen()+y.GetGreen(), z.GetBlue()+y.GetBlue());	return x;}
Color lpmd::operator-(const Color &z, const Color &y){Color x(z.GetRed()-y.GetRed(), z.GetGreen()-y.GetGreen(), z.GetBlue()-y.GetBlue());	return x;}

Color & Color::operator=(const Color & m){ SetRed( m.GetRed() ); SetGreen( m.GetGreen() ); SetBlue( m.GetBlue() ); return (*this);}
Color & Color::operator+=(const Color & m){ SetRed( GetRed() + m.GetRed() ); SetGreen( GetGreen() + m.GetGreen() ); SetBlue( GetBlue() + m.GetBlue() ); return (*this);}
Color & Color::operator-=(const Color & m){ SetRed( GetRed() - m.GetRed() ); SetGreen( GetGreen() - m.GetGreen() ); SetBlue( GetBlue() - m.GetBlue() ); return (*this);}

bool operator == (const Color &z, const Color &w){return bool(z.GetRed()==w.GetRed() && z.GetGreen()==w.GetGreen() && z.GetBlue()==w.GetBlue());}
bool operator != (const Color &z, const Color &w){return bool(z.GetRed()!=w.GetRed() && z.GetGreen()!=w.GetGreen() && z.GetBlue()!=w.GetBlue());}

