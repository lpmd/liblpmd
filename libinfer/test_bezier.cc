/*
 *
 *
 *
 */

#include "infer/bezier.h"
#include "infer/state.h"
#include <iostream>

int main()
{
 State s1({ 1, 2, 3 });
 State s2({ 5, 4, -1 });
 Bezier b({ s1, s2 });
 std::cout << b(0.5) << "\n";
}

