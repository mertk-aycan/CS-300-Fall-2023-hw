#ifndef _RANDGEN_H
#define _RANDGEN_H

#include <limits.h>


class RandGen {
public:
 
    RandGen();

    int RandInt(int max = INT_MAX);

    int RandInt(int low, int max);

    double RandReal();

   
    double RandReal(double low, double max);

   
    static void SetSeed(int seed);

private:
    static int Initialized;
};

#endif 
