#include <time.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>

using namespace std;

class RandGen {
private:
    static int Initialized;
    void InitRandomSeed();

public:
    RandGen();
    void SetSeed(int seed);
    int RandInt(int max);
    int RandInt(int low, int max);
    double RandReal();
    double RandReal(double low, double high);
};

int RandGen::Initialized = 0;

void RandGen::InitRandomSeed() {
    if (!Initialized) {
        Initialized = 1;
        time_t now;
        time(&now);
        srand(int((sin(double(now)) * 1000000)));
    }
}

RandGen::RandGen() {
    InitRandomSeed();
}

void RandGen::SetSeed(int seed) {
    if (!Initialized) {
        Initialized = 1;
        srand(seed);
    }
}

int RandGen::RandInt(int max) {
    return int(RandReal() * max);
}

int RandGen::RandInt(int low, int max) {
    return low + RandInt(max - low + 1);
}

double RandGen::RandReal() {
    return rand() / (double(RAND_MAX) + 1);
}

double RandGen::RandReal(double low, double high) {
    double width = fabs(high - low);
    double thelow = low < high ? low : high;
    return RandReal() * width + thelow;
}
