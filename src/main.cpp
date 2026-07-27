#include <iostream>
#include <chrono>
#include "IsingModel.h"


int main() {

    // Lattice size
    int L = 100;

    // Temperature
    double kT = 2.269;

    // Number of warmup sweeps
    int Nwarmup = 1000;

    // Number of measurable sweeps
    int Nmcs = 5000;

    // Number of measurements
    int Nmeas = 100;

    // Create object
    IsingModel model(L, kT);

    // Initialize lattice
    model.Initialize();

    // Start timer
    auto start = std::chrono::high_resolution_clock::now();

    // Run Metropolis algorithm
    //model.Metropolis(Nwarmup, Nmcs, Nmeas);

    // Run Wolff algorithm
    model.Wolff(Nwarmup, Nmcs, Nmeas);

    // Stop timer
    auto stop = std::chrono::high_resolution_clock::now();

    // Print quantities
    model.PrintQuantities();

    // Print time
    std::chrono::duration<double> elapsed = stop - start;
    std::cout << "Time: " << elapsed.count() << " seconds\n";

    return 0;
}