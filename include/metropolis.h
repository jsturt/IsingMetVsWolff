#pragma once

#include "lattice.h"
#include <random>
#include <cmath>
#include <iostream>
#include <string>
#include <fstream>

class Metropolis
{
  public:
    Metropolis(Lattice& lattice);
    ~Metropolis();
    void MCS(float kT, float J = 1, bool save = false);
    void MCSBy(unsigned int n, float kT, float J = 1, bool save = false);
    void Step(float errorMin ,unsigned int samplesMin, unsigned int samplesMax ,unsigned int nMCS, unsigned int nEqMCS, float kT, float J = 1, bool save = false);
    void WriteLattice(std::string filepath);
    void WriteM(std::string filepath);
    void WriteE(std::string filepath, float J = 1);

  private:
    Lattice& m_Lattice;
    std::vector<Lattice> m_LatticeHistory;
    std::mt19937 m_Mt;
    std::uniform_int_distribution<> m_GenX;
    std::uniform_int_distribution<> m_GenY;
    std::uniform_real_distribution<> m_Prob;
};
