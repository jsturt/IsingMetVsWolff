#include "lattice.h"

Lattice::Lattice(const unsigned int& xLength, const unsigned int& yLength) :
  m_X(xLength), m_Y(yLength)
{
  // random int generator w/ random seed
  std::random_device rd;
  auto gen = std::bind(std::uniform_int_distribution<>(0,1),std::mt19937(rd()));
  

  // Initialise lattice with random spin configuration
  for(unsigned int iy=0; iy<m_Y; iy++)
  {
    for(unsigned int ix=0; ix<m_X; ix++)
    {
      m_Sites.push_back({((bool) gen()), ix, iy});
    }
  }
}

Lattice::~Lattice()
{

}

Site& Lattice::GetSite(const unsigned int& xPos, const unsigned int& yPos)
{
  return m_Sites.at(xPos + (yPos * m_X));
}

std::vector<Site> Lattice::GetNeighbours(const Site& site)
{
  std::vector<Site> neighbours;
  // need to use PBC => under/over - flows of indices need to map correctly 
  //const unsigned int index = site.x + (site.y * m_X);
  //up
  neighbours.push_back(GetSite(site.x, (site.y-1)%m_Y));
  //down
  neighbours.push_back(GetSite(site.x, (site.y+1)%m_Y));
  //left
  neighbours.push_back(GetSite((site.x-1)%m_X, site.y));
  //right
  neighbours.push_back(GetSite((site.x+1)%m_X, site.y));
  
 // for(auto &elem : neighbours)
 // {
 //   std::cout<<elem.spin<<" ";
 // }
 // std::cout<<std::endl;
  return neighbours;
}

const void Lattice::PrintLattice() 
{
  for(unsigned int i=0; i<m_Y; i++)
  {
    for(unsigned int k=0; k<m_X; k++)
      std::cout<< m_Sites[k + (i*m_X)].spin<<" ";
    std::cout<<"\n";
  }
}

float Lattice::EnergyNN(bool localSpin, Site& site, float& J)
{
  // Taking localSpin as an argument allows spins to be compared without being set in the lattice
  float cost = 0;
  for(auto &elem : GetNeighbours(site))
  {
    cost += -1.0f * J * ((2 * elem.spin)-1) * ((2 * localSpin)-1);
  }
  return cost;
}

float Lattice::Magnetisation()
{
  float sum = 0;
  for(auto &elem : m_Sites)
  {
    sum += ((2 * elem.spin)-1);
  }
  return sum / (m_X * m_Y);
}

float Lattice::ConfigurationEnergy(float J)
{
  float energy=0;
  for(auto &elem : m_Sites)
  {
    energy += EnergyNN(elem.spin, elem, J);
  }
  // Div by 2 to avoid double counting
  return energy/(2 * m_X * m_Y);
}
