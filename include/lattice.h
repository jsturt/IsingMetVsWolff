#pragma once

#include <vector>
#include <random>
#include <functional>
#include <iostream>
#include <ctime>
#include <cmath>

struct Site
{
  bool spin;
  unsigned int x;
  unsigned int y;
};

class Lattice
{
  public:
    Lattice(const unsigned int& xLength, const unsigned int& yLength);
    ~Lattice();
    Site& GetSite(const unsigned int& xPos, const unsigned int& yPos);
    std::vector<Site> GetNeighbours(const Site& site);
    std::vector<Site> GetLattice() const {return m_Sites;}
    unsigned int GetX() const {return m_X;}
    unsigned int GetY() const {return m_Y;}
    const void PrintLattice(); 
    float EnergyNN(bool localSpin, Site& site, float& J);
    float Magnetisation();
    float ConfigurationEnergy(float J = 1.0f);
  private:
    unsigned int m_X;
    unsigned int m_Y;
    std::vector<Site> m_Sites;
};
