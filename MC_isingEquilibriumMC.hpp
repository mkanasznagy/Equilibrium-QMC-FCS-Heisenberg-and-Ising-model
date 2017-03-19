//
//  MC_isingEquilibriumMC.hpp
//  
//
//  Created by Marton Kanasz-Nagy on 11/2/16.
//
//

#ifndef MC_isingEquilibriumMC_hpp
#define MC_isingEquilibriumMC_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include "MC_random.hpp"
#include "MC_finiteTemperatureEquilibriumQMC.hpp"



// --------------------------------------------------------------------------------------------
// Child class of squareLatticeFiniteTemperatureEquilibriumQMC implementing an equilibrium
// Monte Carlo calculation of the AFM Ising model in zero magnetic field, using the Wolff
// cluster algorithm. Lattice size is set in MC_coord.hpp by the global variable LATTICE_SIZE.
// --------------------------------------------------------------------------------------------
// This algorithm was written using online tutorials about the Wolff algorithm, e.g.
// http://www.physics.buffalo.edu/phy411-506-2004/Topic3/topic3-lec7.pdf
// http://csml.northwestern.edu/resources/Reprints/lnp_color.pdf
// http://www.helsinki.fi/~rummukai/simu/cluster.pdf
// Since most details are online, I will only write short comments to the code.
// ------------------------------------------------------------------------------------------

class isingEquilibriumMC : public squareLatticeFiniteTemperatureEquilibriumQMC {
protected:
    double J;                               // AFM coupling constant (J>0 for AFM order): H = J sum_<i,j> S_i^z * S_j^z
    double T;                               // temperature
    
private:
    double pAdd;                            // probability of adding AFM alligned neighbors to the cluster (Wolff algorithm), p=1-exp(-J/2T)
    double energy;                          // energy of the current quantum state
    vector< vector<int> > nearestNeighbors; // nearestNeighbors[i] is a vector containing all the nearest neighbors of site 'i'
    vector<bool> isMember;                  // auxiliary array for bookkeeping which sites belong the actually updated cluster
    
public:
    // constructor and destructor
    isingEquilibriumMC(double Jval=1., double Tval=100.);
    ~isingEquilibriumMC();
    
    // reset data and reinitialize simulation
    void reset(double Jval, double Tval);
    
    // QMC routines
    void thermalize(int nBurnInSteps);
    void run(int nMonteCarloSteps);
    
    // get basic info
    inline double getJ(void) const {return this->J;};
    inline double getT(void) const {return this->T;};
    
    // print out all data, using the operator << of the parent class as well
    friend ostream& operator<<(ostream &, const isingEquilibriumMC &);
    
private:
    // initialize the private variables
    void initialize(void);
    
    // check consistency of inner variables
    bool check(void) const;
    
    // one Monte Carlo step of the Wang cluster algorithm
    void wangUpdate(void);
    
    // add AFM ordered neighboring sites of site 'site' to the cluster, with probability (Wang algorithm)
    void growCluster(const int siteInd);
    
    // one Monte Carlo step using the Metropolis update of a single spin
    // much slower dynamics than that generated by 'wangUpdate', especially close to the ordering temperature.
    // in the current implementation, it is not even optimized.
    // it will be useful to test the results of the algorithm using the routine 'wangUpdate'.
    void singleSpinFlipUpdate(void);
    
    // calculate the energy of the current spin state
    double getCurrentEnergy(void) const;
    
    // update average energy and magnetic susceptibility
    void updateAvgEnergy(void);
    void updateMagneticSusceptibility(void);
    void updateSpecificHeat(void);
};



#endif /* MC_isingEquilibriumMC_hpp */
