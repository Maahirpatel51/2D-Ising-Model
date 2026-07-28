#include <iostream>
#include "IsingModel.h"


int main() {

    // Lattice size
    int L = 100;

    // Temperature
    double kT = 2.269;

    // Number of warmup sweeps
    int Nwarmup = 1000;

    // Number of measurable sweeps
    int Nmcs = 200;

    // Number of measurements
    int Nmeas = 20;
    
    // Create object
    IsingModel model(L, kT);

    // Export toggle
    model.Export = true;
    model.ExportWarmup = false;

    // Initialize lattice
    model.Initialize();

    // Run Metropolis algorithm
    model.Metropolis(Nwarmup, Nmcs, Nmeas);

    // Run Wolff algorithm
    //model.Wolff(Nwarmup, Nmcs, Nmeas);

    // Print quantities
    model.PrintQuantities();
    //model.PrintLattice();

    return 0;
}