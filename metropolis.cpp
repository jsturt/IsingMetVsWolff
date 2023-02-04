#include "metropolis.h"

Metropolis::Metropolis(Lattice& lattice) :
  m_Lattice(lattice), m_Mt(std::random_device()()),
  m_GenX(0, m_Lattice.GetX()-1), m_GenY(0, m_Lattice.GetY()-1), m_Prob(0, 1.0f)
{
  
}

Metropolis::~Metropolis()
{

}

void Metropolis::MCS(float kT, float J, bool save)
{
  // sample random site
  unsigned int x = m_GenX(m_Mt);
  unsigned int y = m_GenY(m_Mt);
  float dE = 0.0f;

  Site& site = m_Lattice.GetSite(x,y);

  float beta = 1.0f/kT;
  
  std::cout<<"sample x: "<<x<<"\n";
  std::cout<<"sample y: "<<y<<"\n";

  std::cout<<"spin: "<<site.spin<<"\n";
  std::cout<<"energy of nn: "<<m_Lattice.EnergyNN(site.spin, site, J)<<"\n";
  std::cout<<"energy of nn if spin was flipped: "<<m_Lattice.EnergyNN(!site.spin, site, J)<<"\n";

  dE = m_Lattice.EnergyNN(!site.spin, site, J) - m_Lattice.EnergyNN(site.spin, site, J);
  std::cout<<"dE : "<<dE<<"\n";

  if(dE < 0.0f)
  {
    std::cout<<"flipping spin"<<std::endl;
    site.spin ^= true;
  }
  else if(exp( -1.0f * beta * dE ) > m_Prob(m_Mt) ) 
  {
    std::cout<<"flipping spin by detailed balance"<<std::endl;
    site.spin ^= true;
  }

  // If neither above condition is met then the spin is not flipped
   
  std::cout<<"final spin: "<<site.spin<<"\n";
  std::cout<<"final energy of nn: "<<m_Lattice.EnergyNN(site.spin, site, J)<<"\n";

  if(save)
  {
    m_LatticeHistory.push_back(m_Lattice);
  }

}

void Metropolis::MCSBy(unsigned int n, float kT, float J, bool save)
{
  // disable cout while stepping multiple times 
  std::cout.setstate(std::ios_base::failbit);
  for(unsigned int i=0; i<n; i++)
  {
    MCS(kT, J, save);
  }
  std::cout.clear();
}

void Metropolis::Step(float errorMin, unsigned int samplesMin, unsigned int samplesMax, unsigned int nMCS, unsigned int nEqMCS, float kT, float J, bool save)
{

  // Equilibriate at given kT, J
  MCSBy(nEqMCS, kT, J, save);

  // Perform samples
  float sum=0, sum_sqd=0, avg_mag=0, avg=0, std_err=0;
  unsigned int nSamples=0;
  while(nSamples<samplesMax+1)
  {
    // Take MCS for each sample
    MCSBy(nMCS, kT, J, save);
    // Calculate average Quantities (magnetisation, etc...)
    avg_mag = m_Lattice.Magnetisation();
    sum += avg_mag;
    sum_sqd += avg_mag*avg_mag;
    // Increment num samples
    ++nSamples;
    // Check every 64 samples if the std dev is below the target (minError) and that the min samples has been passed
    if(nSamples >= samplesMin && !(nSamples&0x3))
    {
      avg = sum / nSamples;
      std_err = sqrt(((sum_sqd/nSamples) - avg*avg) / nSamples);
      if(std_err < errorMin) break;
    }
  }
    avg = sum / nSamples;
    std_err = sqrt(((sum_sqd/nSamples) - avg*avg) / nSamples);
    std::cout<<std_err<<" "<<nSamples<<"\n";
    //std::cout<<avg<<" "<<kT<<" "<<std_err<<"\n";
}

void Metropolis::WriteLattice(std::string filepath)
{
  if(!m_LatticeHistory.size())
  {
    std::cout<<"|WriteLattice Error| no saved configuration"<<!m_LatticeHistory.size()<<std::endl;
    return;
  }
  std::ofstream File; 
  File.open(filepath);

  // for each lattice in vec of lattices
  //    for each site in the lattice

  for(auto &elem : m_LatticeHistory)
  {
    for(auto &site : elem.GetLattice())
    {
      File<<site.spin;
    }
    File<<std::endl;
  }
}

void Metropolis::WriteM(std::string filepath)
{
  if(!m_LatticeHistory.size())
  {
    std::cout<<"|WriteM Error| no saved configuration"<<!m_LatticeHistory.size()<<std::endl;
    return;
  }
  std::ofstream File; 
  File.open(filepath);

  // for each lattice in vec of lattices
  //    for each site in the lattice

  for(auto &elem : m_LatticeHistory)
  {
    File<<elem.Magnetisation()<<std::endl;
  }
}

void Metropolis::WriteE(std::string filepath, float J)
{
  if(!m_LatticeHistory.size())
  {
    std::cout<<"|WriteM Error| no saved configuration"<<!m_LatticeHistory.size()<<std::endl;
    return;
  }
  std::ofstream File; 
  File.open(filepath);

  // for each lattice in vec of lattices
  //    for each site in the lattice

  for(auto &elem : m_LatticeHistory)
  {
    File<<elem.ConfigurationEnergy(J)<<std::endl;
  }
}

