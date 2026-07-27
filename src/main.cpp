#include <iostream>
#include "IsingModel.h"

int main() {

    // Lattice size
    int L = 20;

    // Temperature
    double kT = 2.5;

    // Number of warmup sweeps
    int Nwarmup = 10000;

    // Number of measurable sweeps
    int Nmcs = 100;

    // Number of measurements
    int Nmeas = 20;

    // Create object
    IsingModel model(L, kT);

    // Initialize lattice
    model.Initialize();

    // Run Metropolis algorithm
    model.MetropolisSweep(Nwarmup, Nmcs, Nmeas);

    // Print quantities
    model.PrintQuantities();

    return 0;
}