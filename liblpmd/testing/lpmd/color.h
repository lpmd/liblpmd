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

#include <lpmd/vector.h>

namespace lpmd
{
 Vector GetSpcColor(int spc);
} // lpmd

#endif
