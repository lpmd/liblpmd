//
//
//

#include <lpmd/color.h>

using namespace lpmd;

/* 
Color lpmd::ColorFromScalar(const double & x)
{
 double R,G,B,A=0.5;
 if      (0<=x && x<=A){ R=1.0-x/A; G=x/A; B=0; }
 else if (A<=x && x<=1.0){ R=0; G=(x-1)/(A-1); B=(x-A)/(1-A); }
 else {R=0; G=0.8; B=0.8;}
 Color clr(R,G,B);
 return clr;
}
*/

Color lpmd::ColorFromScalar(const double & x)
{
 if (x < 0.0) return Color(0.0, 0.0, 1.0);
 else if ((x >= 0.0) && (x < 0.25)) return Color(0.0, 4.0*x, 1.0);
 else if ((x >= 0.25) && (x < 0.5)) return Color(0.0, 1.0, 1.0-4.0*(x-0.25));
 else if ((x >= 0.5) && (x < 0.75)) return Color(4.0*(x-0.5), 1.0, 0.0);
 else if ((x >= 0.75) && (x <= 1.0)) return Color(1.0, 1.0-4.0*(x-0.75), 0.0);
 return Color(1.0, 0.0, 0.0);
}

