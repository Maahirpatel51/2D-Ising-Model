#pragma once

#include <random>

namespace Random {

    // Returns unique random engine for each thread
    inline std::mt19937_64& get_engine() {
        thread_local std::mt19937_64 engine(std::random_device{}());
        return engine;
    }

    // Get random integer
    inline int get_int(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return(dist(get_engine()));
    }

    // Get random real 
    inline double get_real(double min, double max) {
        std::uniform_real_distribution<double> dist(min, max);;
        return(dist(get_engine()));

    }
}