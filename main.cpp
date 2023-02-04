#include <iostream>
#include "lattice.h"
#include "metropolis.h"

int main(int argc, char* argv[])
{
  /*
  Want some sudocode for how the program will work

  // H = -2 hbar J SUM_{ij} (S_i S_j) 
  // wrap hbar up into J => user specifies hbar J 

  // Create a lattice, lattice stores sites (struct of coords and spin direction)
  unsigned int xnum = 100;
  unsigned int ynum = 100;

  Lattice lattice(xnum, ynum);

  // Pick a site at random => pass Lattice by reference to the "solver"?
  // Where do I want to do the stepping / main loop?
  // Have main loop in main.cpp, which calls metropolis.Step();
  // Since we pass lattice by reference, we can query the lattice at any step to retrieve physical quantities  
  // Within solver: 
      // Calculate energy delta of flipping this spin
      // if delta < 0, accept spin flip, flip the spin, if not flip it with prob e^(-beta * dE)
  
  Metropolis metropolis(lattice);

  for(int i=0; i<steps; i++)
  {
    metropolis.Step();
    std::cout<<lattice.GetMag()<<std::endl;
  }

  // could also have a step-by function to iterate by n steps
  
  metropolis.StepBy(1000);

  // add a "snapshot" method to lattice & metropolis

  std::cout<<"Magnetisation after "<<steps<<" steps : "<<lattice.GetMag()<<std::endl;

  */

  unsigned int N = 25;
  Lattice lattice(N,N);
  Metropolis met(lattice);

  std::cout<<lattice.Magnetisation()<<std::endl;

  float kT = 1.0f;
  float kTmax = 5.0f;
  // t steps
  unsigned int numSteps = 20;
  // samples per step for average
  unsigned int numSamplesMax = 100000;
  unsigned int numSamplesMin = 64;
  // MCS per sample 
  unsigned int MCSPerSample = 20;
  // number of MCS to equilibiriate
  unsigned int numEq = 100;
  // min error
  float minError = 0.0005f;

  std::cout<<"Magnetisation|kT|sigma_M\n";
  for(unsigned int i=0; i<numSteps; i++)
  {
    //float kTval = kT + (((kTmax-kT) / numSteps)*i);
    //met.Step(minError, numSamplesMin, numSamplesMax, MCSPerSample, numEq, kTval, 1, false);
    
    met.Step(minError, numSamplesMin, (i+1)*1000, MCSPerSample, numEq, 4.0f, 1, false);
  }




  return 0;
} 
