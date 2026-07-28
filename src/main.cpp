#include <iostream>
#include "IsingModel.h"


int main() {

    // Lattice size
    int L = 100;

    // Temperature
    double kT = 2.269;

    // Number of warmup sweeps
    int Nwarmup = 5000;

    // Number of measurable sweeps
    int Nmcs = 200;

    // Number of measurements
    int Nmeas = 200;

    // Create object
    IsingModel model(L, kT);

    // Toggle to include warmup during export
    model.ExportWarmup = false;

    // Initialize lattice
    model.Initialize();

    // Run Metropolis algorithm
    //model.Metropolis(Nwarmup, Nmcs, Nmeas);

    // Run Wolff algorithm
    model.Wolff(Nwarmup, Nmcs, Nmeas);

    // Print quantities
    model.PrintQuantities();
    //model.PrintLattice();

    return 0;
}