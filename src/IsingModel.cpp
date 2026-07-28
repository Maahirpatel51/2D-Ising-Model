
#include "IsingModel.h"

// Parameterized constructor
IsingModel::IsingModel(int L, double kT) :
    L(L),
    kT(kT), 
    SpinConfiguration(L*L),
    P_flip {
        1.0,
        1.0,
        1.0,
        std::min(1.0, std::exp(-4.0 / kT)),
        std::min(1.0, std::exp(-8.0 / kT))
    },
    P_add(1.0 - std::exp(-2.0 / kT))
    {}

// Initialize lattice with random spins
void IsingModel::Initialize() {
    for (int i = 0; i < L * L; i++) {
        
        int r = Random::get_int(0, 1);
        
        if (r == 0) {
            r = -1;
        }

        SpinConfiguration[i] = r;
    }

}

// Helper function to flip spin and update quantities
void IsingModel::MetropolisUpdate(int i, int j) {

    // Update indices to satisfy boundary conditions
    int ip = (i + 1) % L;
    int im = (i - 1 + L) % L;
    int jp = (j + 1) % L;
    int jm = (j - 1 + L) % L;

    // Compute change in energy associated with flipping a spin
    int spin_ij = SpinConfiguration[idx(i, j)];
    int DeltaE = 2 * spin_ij * (
        SpinConfiguration[idx(im, j)] +
        SpinConfiguration[idx(i, jp)] +
        SpinConfiguration[idx(ip, j)] +
        SpinConfiguration[idx(i, jm)]
    );

    // Generate random probability
    double r = Random::get_real(0.0, 1.0);
    
    // Metropolis probability condition
    if (P_flip[(DeltaE + 8) / 4] > r) {
        
        // Flip spin
        SpinConfiguration[idx(i, j)] = -spin_ij;

        // Update current energy and magnetization 
        currentE += DeltaE; 
        currentM += (-2) * spin_ij;
    }
}

// Metropolis algorithm
void IsingModel::Metropolis(int Nwarmup, int Nmcs, int Nmeas) {

    // Reset quantities
    E.clear();
    E2.clear();
    M.clear();
    M2.clear();
    averageE = 0.0;
    averageE2 = 0.0;
    averageM = 0.0;
    averageM2 = 0.0;

    // Get energy and magnetization after initialization
    currentE = Energy();
    currentM = Magnetization();

    // Compute number of sweeps
    const int WarmupSweeps = Nwarmup * L * L;
    const int MeasurementSweeps = Nmcs * L * L;
    const int MeasureEvery = MeasurementSweeps / Nmeas;

    // Perform warmup sweeps
    for (int n = 0; n < WarmupSweeps; n++) {

        // Generate random indices
        int i = Random::get_int(0, L - 1);
        int j = Random::get_int(0, L - 1);
        
        MetropolisUpdate(i, j);

        if (ExportWarmup && Export) {
            if (n % MeasureEvery == 0) {
                SaveFrame();
            }
        }
    }

    // Perform measurement sweeps
    for (int n = 0; n < MeasurementSweeps; n++) {
       
        // Generate random indices
        int i = Random::get_int(0, L - 1);
        int j = Random::get_int(0, L - 1);

        MetropolisUpdate(i, j);

        // Add measurements
        if (n % MeasureEvery == 0) {
            E.push_back(currentE);
            E2.push_back(currentE * currentE);
            M.push_back(currentM);
            M2.push_back(currentM * currentM);

            if (Export) {
                SaveFrame();
            }
        }
    }

    // Compute averages
    for (int i = 0; i < Nmeas; i++) {
        averageE += E[i];
        averageE2 += E2[i];
        averageM += std::abs(M[i]);
        averageM2 += std::abs(M2[i]);
    }

    averageE /= Nmeas;
    averageE2 /= Nmeas;
    averageM /= Nmeas;
    averageM2 /= Nmeas;
}

void IsingModel::WolffUpdate() {

    std::fill(ClusterMember.begin(), ClusterMember.end(), false);

    // Select a seed
    int seed = Random::get_int(0, L * L - 1);

    // Push seed to stack
    ClusterStack.push(seed);
    ClusterMember[seed] = true;
    
    // Loop until stack is empty
    while (!ClusterStack.empty()) {

        // Remove top element
        int site = ClusterStack.top();
        ClusterStack.pop();

        // Site indices
        int i = site / L;
        int j = site % L;

        // Get neighbours
        int ip = (i + 1) % L;
        int im = (i - 1 + L) % L;
        int jp = (j + 1) % L;
        int jm = (j - 1 + L) % L;
        int neighbours[4] = {idx(im,j), idx(ip,j), idx(i,jm), idx(i,jp)};

        // Loop through neighbours
        for (auto n : neighbours) {

            // Verify if neighbour is not in cluster
            if (!ClusterMember[n]) {

                // Verify if site and neighbour have the same spin
                if (SpinConfiguration[n] == SpinConfiguration[site]) {
                    
                    // Generate random probability
                    double r = Random::get_real(0.0, 1.0);

                    // Add neighbour to cluster
                    if (r < P_add) {
                        ClusterStack.push(n);
                        ClusterMember[n] = true;
                    }
                }
            }
        }
    }

    // Flip cluster
    for (int i = 0; i < L * L; i++) {
        if (ClusterMember[i]) {
            SpinConfiguration[i] *= -1;
        }
    }
}

void IsingModel::Wolff(int Nwarmup, int Nmcs, int Nmeas) {
    
    // Reset quantities
    E.clear();
    E2.clear();
    M.clear();
    M2.clear();
    averageE = 0.0;
    averageE2 = 0.0;
    averageM = 0.0;
    averageM2 = 0.0;

    currentE = Energy();
    currentM = Magnetization();

    ClusterMember.resize(L * L);
    
    const int MeasureEvery = Nmcs / Nmeas;
        
    // Warmup
    for (int n = 0; n < Nwarmup; n++) {
        
        WolffUpdate();

        currentE = Energy();
        currentM = Magnetization();

        if (ExportWarmup && Export) {
            if (n % MeasureEvery == 0) {
                SaveFrame();
            }
        }
    }

    for (int n = 0; n < Nmcs; n++) {

        WolffUpdate();

        currentE = Energy();
        currentM = Magnetization();

        if (n % MeasureEvery == 0) {

            E.push_back(currentE);
            E2.push_back(currentE * currentE);
            M.push_back(currentM);
            M2.push_back(currentM * currentM);

            if (Export) {
                SaveFrame();
            }
        }
    }

    // Compute averages
    for (int i = 0; i < Nmeas; i++) {
        averageE += E[i];
        averageE2 += E2[i];
        averageM += std::abs(M[i]);
        averageM2 += std::abs(M2[i]);
    }

    averageE /= Nmeas;
    averageE2 /= Nmeas;
    averageM /= Nmeas;
    averageM2 /= Nmeas;
}

// Compute spin energy in lattice
int IsingModel::Energy() const {
    
    int E = 0;

    for (int i = 0; i < L; i++) {

        const int ip = (i + 1) % L;

        for (int j = 0; j < L; j++) {

            const int jp = (j + 1) % L;

            E -= SpinConfiguration[idx(i, j)] * (SpinConfiguration[idx(i, jp)] + SpinConfiguration[idx(ip, j)]);
        }
    }

    return E;
}

// Compute magnetization in lattice
int IsingModel::Magnetization() const {

    int M = 0;

    for (int i = 0; i < L * L; i++) {
        M += SpinConfiguration[i];
    }

    return M;
}

void IsingModel::SaveFrame() {

    std::ofstream file("../export/frame_" + std::to_string(frame) + ".txt");

    for (int i = 0; i < L; i++) {
        for (int j = 0; j < L; j++) {
            file << SpinConfiguration[idx(i, j)] << " ";
        }

        file << "\n";
    }

    frame++;

}

// Display lattice in terminal
void IsingModel::PrintLattice() const {
    for (int i = 0; i < L * L; i++) {
        
        if (i % L == 0) {
            std::cout << "\n";
        }

        std::cout << SpinConfiguration[i] << " ";
    }

    std::cout << "\n";

}

// Print quantities
void IsingModel::PrintQuantities() const {

    std::cout << "<E> = " << averageE << std::endl;
    std::cout << "<E>/N = " << averageE / (L * L) << std::endl;
    std::cout << "<E^2>/N = " << averageE2 / (L * L) << std::endl;

    std::cout << "<|M|> = " << averageM << std::endl;
    std::cout << "<|M|>/N = " << averageM / (L * L) << std::endl;
    std::cout << "<|M^2|>/N = " << averageM2 / (L * L) << std::endl;

    double Cv = (averageE2 - averageE * averageE) / (L * L * kT * kT);
    std::cout << "Heat Capacity (C_v) = " << Cv << std::endl;



}