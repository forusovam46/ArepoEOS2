/* 
 * Extension of AREPO code's ISOTHERM_EQS flag by optional thermal evolution
 * as part of my master's thesis
 * Magdalena Forusova 2024
 */
 
#include "../hydro/eff_internal_energy.h"
#include <math.h>

double getEffInternalEnergy(InternalEnergyOption option, double effInternalEnergy, double time) {
    double U;
    double a_0 = 0.14;

    switch (option) {
        case CONSTANT:
            // Option a) constant
            // U(a) = 758.61
            U = 758.61;
            break;

        case LINEAR:
            // Option b) linear
            // U(a) = { 34.55 if a < 0.14 else 34.55 + 3453.76*(a - 0.14)^1 }
            if (time <= a_0) {
                U = 34.55;
            } else {
                U = 34.55 + 3453.76 * (time - a_0);
            }
            break;

        case POLYNOMIAL_PIECEWISE:
            // Option c) polynomial piecewise fit
            // U(a) = { 34.55 if a < 0.14 else 34.55 + 4119.35*(a - 0.14)^1 + -20953.19*(a - 0.14)^2 + 67292.51*(a - 0.14)^3 + -69033.16*(a - 0.14)^4 + 21621.37*(a - 0.14)^5 }
            if (time <= a_0) {
                U = 34.55;
            } else {
                U = 34.55 +
                    4119.35 * pow(time - a_0, 1) +
                    -20953.19 * pow(time - a_0, 2) +
                    67292.51 * pow(time - a_0, 3) +
                    -69033.16 * pow(time - a_0, 4) +
                    21621.37 * pow(time - a_0, 5);
            }
            break;

        default:
            U = effInternalEnergy;
            break;
    }

    return U;
}
