/* 
 * Add Optional Thermal Evolution for Gas via Effective Internal Energy
 * Master's Thesis - Magdalena Forusova, 2024
 */

#include "../main/allvars.h"

#ifndef EFF_INTERNAL_ENERGY_H
#define EFF_INTERNAL_ENERGY_H

// Function declaration for calculating effective internal energy with density parameter
double getEffInternalEnergy(InternalEnergyOption option, double effInternalEnergy, double time, double density);

// Function declaration for calculating the density threshold
double calculateDensityThreshold(double time);

#endif // EFF_INTERNAL_ENERGY_H
