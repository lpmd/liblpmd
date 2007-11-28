//
// 
//

/** 
 *  \file  cell.h 
 *  \brief Declaración de la clase Cell.
 *  \author GNM
 */

#ifndef __LPMD_CELL_H__
#define __LPMD_CELL_H__

#include "vector.h"

namespace lpmd
{

/**
 *  Cell representa una celda tridimensional en el espacio, generada por tres vectores (no necesariamente ortogonales).
 */
class Cell
{
 public:
    //Constructores y Destructores
    Cell();
    Cell(Vector a, Vector b, Vector c);
    Cell(double a, double b, double c, double alpha=M_PI/2.0e0, double beta=M_PI/2.0e0, double gamma=M_PI/2.0e0);
    virtual ~Cell();

    //Settings and Gettings
    Vector GetVector(int i) const {return v[i];}
    void SetVector(int i,Vector a) { v[i]=a; UpdateTransfMatrix(); }
    void ReSet(double a,double b,double c,double alpha=M_PI/2.0e0,double beta=M_PI/2.0e0,double gamma=M_PI/2.0e0);

    // Transformacion de coordenadas
    void UpdateTransfMatrix();
    void ConvertToInternal(Vector & v) const;
    void ConvertToExternal(Vector & v) const;

    //Extra Functions
    double Volume() const;

    void Scale(int i, double f);
    void ScalePercent(int i, double p);

    friend std::ostream & operator<<(std::ostream &, const Cell &);

  private:
    Vector v[3];
    double tm[3][3], itm[3][3];
    double nonortg;
};

} // lpmd

#endif

