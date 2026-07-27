#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "random_util.h"

class IsingModel {
    private:
        int L;
        double kT;

        std::vector<int> SpinConfiguration;

        std::vector<double> P_flip;

        // Quantities
        std::vector<int> E;
        std::vector<int> E2;
        std::vector<int> M;
        std::vector<int> M2;

        int currentE;
        int currentM;

        double averageE = 0.0;
        double averageM = 0.0;
        double averageE2 = 0.0;
        double averageM2 = 0.0;

        void AttemptFlip();

    public:

        inline int idx(int i, int j) const {
            return i * L + j;
        }

        IsingModel(int L, double kT);

        void Initialize();

        void MetropolisSweep(int Nwarmup, int Nmcs, int Nmeas);

        int Energy() const;

        int Magnetization () const;

        // Printout
        void PrintLattice() const;

        void PrintQuantities() const;

};