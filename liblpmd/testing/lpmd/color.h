//
//
//

#ifndef __LPMD_COLOR_H__
#define __LPMD_COLOR_H__

// Theory of color: http://www.monografias.com/trabajos5/colarq/colarq.shtml#color
/*
Cuando mezclamos pigmentos de distintos valores, el tono resultante será alguno intermedio entre ambos. Así, tenemos cuatro posibilidades de mezclar pigmentos para controlar el valor de los tonos.

    * Agregando blanco, se aumenta el valor.

    * Agregando negro se disminuye el valor.

    * Agregando un gris contrastante, aumenta o disminuye el valor.

    * Agregando un pigmento de valor distinto, se aumenta o disminuye el valor.

No podemos cambiar el valor de un pigmento cromático sin modificar al mismo tiempo otras dimensiones tonales.

    * Agregando negro, blanco o gris, se introduce un componente acromático. El valor se modificara y también el nivel de intensidad. El tono resultante será mas claro o mas oscuro y mas neutral.
*/

/**
 *  \file color.h
 *  \brief Declaración de la clase Color.
 *  \author GNM
 */
#include <iostream>

namespace lpmd
{

/**
 *  Color es usado para colorear los atomos en en lpvisual.
 */
class Color
{
	private:
		double R;
		double G;
		double B;
	public:
		void SetRed(double);
		void SetGreen(double);
		void SetBlue(double);
		
		double GetRed(void) const;
		double GetGreen(void) const;
		double GetBlue(void) const;
		// Operador de asignacion
		Color & operator=(const Color & m);
		Color & operator+=(const Color & m);
		Color & operator-=(const Color & m);
		
		Color(double=0, double=0, double=0);
		~Color(void);
};

// Colores más usuales
const Color white	(1.0, 1.0, 1.0);
const Color black	(0.0, 0.0, 0.0);
const Color red	(1.0, 0.0, 0.0);
const Color green	(0.0, 1.0, 0.0);
const Color blue	(0.0, 0.0, 1.0);

// Sobrecargas (por definir en .cc):
Color operator + (const Color &, const Color &);
Color operator - (const Color &, const Color &);

bool operator == (const Color &, const Color &);
bool operator != (const Color &, const Color &);

} // lpmd

#endif
