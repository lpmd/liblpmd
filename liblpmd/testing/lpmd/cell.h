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

#include <iostream>
#include <cmath>

namespace lpmd
{
 class Vector;                // forward declaration

/**
 *  Cell representa una celda tridimensional en el espacio, generada por tres vectores (no necesariamente ortogonales).
 */
class Cell
{
 public:
    //Constructores y Destructores
    Cell();
    Cell(const Vector & a, const Vector & b, const Vector & c);
    Cell(double a, double b, double c, double alpha=M_PI/2.0e0, double beta=M_PI/2.0e0, double gamma=M_PI/2.0e0);
    Cell(const Cell & c);

    virtual ~Cell();

    //
    Cell & operator=(const Cell & c);

    //Settings and Gettings
    void SetPeriodicity(int axis, bool value);
    bool Periodic(int axis) const;
    Vector GetVector(int i) const;
    void SetVector(int i, Vector a);
    void ReSet(double a, double b, double c, double alpha=M_PI/2.0e0, double beta=M_PI/2.0e0, double gamma=M_PI/2.0e0);

    // Transformacion de coordenadas
    void UpdateTransfMatrix();
//FIXME : Comentado por nuevo vector.h
    //    Vector ScaleByCell(const Vector & v) const;
    void ConvertToInternal(Vector & v) const;
    void ConvertToExternal(Vector & v) const;

    //
    Vector Displacement(const Vector & a, const Vector & b) const;

    //Extra Functions
    double Volume() const;
//Comentadas las 3 por el nuevo vector.h
//    void Scale(int i, double f);
//    void ScalePercent(int i, double p);
//    void ScaleVector(int i, lpmd::Vector s);

    friend std::ostream & operator<<(std::ostream &, const Cell &);

  private:
    class CellImpl * impl;
};

} // lpmd

#endif

