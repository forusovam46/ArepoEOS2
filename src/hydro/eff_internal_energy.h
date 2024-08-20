/* 
 * Extension of AREPO code's ISOTHERM_EQS flag by optional thermal evolution
 * as part of my master's thesis
 * Magdalena Forusova 2024
 */

 #include "../main/allvars.h"

#ifndef EFF_INTERNAL_ENERGY_H
#define EFF_INTERNAL_ENERGY_H

// Function declaration for calculating effective internal energy
double getEffInternalEnergy(InternalEnergyOption option, double effInternalEnergy, double time);

#endif // EFF_INTERNAL_ENERGY_H
