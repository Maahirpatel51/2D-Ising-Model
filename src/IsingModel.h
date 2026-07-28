#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <cmath>
#include <algorithm>
#include "random_util.h"

class IsingModel {
    private:
        int L;
        double kT;

        // Metropolis data structures
        std::vector<int> SpinConfiguration;
        std::vector<double> P_flip;

        // Wolff data structures
        std::vector<bool> ClusterMember;
        std::stack<int> ClusterStack;
        double P_add;

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
        

        // Algorithm helper functions
        void MetropolisUpdate(int i, int j);
        void WolffUpdate();

        // Save lattice to .txt file
        void SaveFrame();
        int frame = 0;

    public:

        // Export toggle
        bool Export = false;

        // Export warmup data toggle
        bool ExportWarmup = false;

        // 2D -> 1D vector indexing function
        inline int idx(int i, int j) const {
            return i * L + j;
        }

        // Constructor
        IsingModel(int L, double kT);

        // Lattice initialization
        void Initialize();

        // Metropolis algorithm implementation
        void Metropolis(int Nwarmup, int Nmcs, int Nmeas);

        // Wolff algorithm implementation
        void Wolff(int Nwarmup, int Nmcs, int Nmeas);

        // Compute energy
        int Energy() const;

        // Compute magnetization
        int Magnetization() const;

        // Printout
        void PrintLattice() const;
        void PrintQuantities() const;

};