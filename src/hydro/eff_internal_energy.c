/* 
 * Add Optional Thermal Evolution for Gas via Effective Internal Energy
 * Master's Thesis - Magdalena Forusova, 2024
 * 
 * Functionality:
 * This code introduces a flexible model for thermal evolution in cosmological volume simulations.
 * It provides multiple options for calculating the internal energy of gas, accommodating 
 * scenarios ranging from simple time-dependent evolution to density-dependent models.
 * 
 * Key Features:
 * - **InternalEnergyOption Enum** in allvars.c:
 *   - `NO`: Default behavior as originally in Arepo.
 *   - `CONSTANT`: Internal energy is set to a predefined constant value.
 *   - `LINEAR`: Internal energy evolves linearly with the scale factor, with a fallback to 
 *     piecewise behavior for edge cases.
 *   - `POLYNOMIAL_PIECEWISE`: Internal energy is calculated using a 5th-degree polynomial 
 *     piecewise fit for the scale factor.
 * - **Density Threshold Toggle** and corresponding model prescriptions in `eff_internal_energy.c`
 *   - `threshold_density = true`: Activates density-dependent thermal evolution.
 *     - Above a dynamically calculated density threshold, internal energy follows 
 *       a power-law fit derived from CAMELS CV0 simulations.
 *     - Models baryonic feedback processes to stabilize against unphysical gas collapse 
 *       in high-density regions.
 *   - `threshold_density = false`: Disables density-dependent behavior.
 *     - Applies the selected time-dependent evolution model uniformly, regardless of gas density.
 * 
 * Key Updates:
 * - Added `eff_internal_energy.c` and `eff_internal_energy.h` to handle thermal evolution. Prescriptions were derived from https://users.flatironinstitute.org/~camels/Sims/IllustrisTNG/CV/
 * - Updated `update_primitive_variables` to adjust total energy based on the chosen thermal evolution model.
 * - Introduced a new enum (`InternalEnergyOption`) for easy configuration in `allvars.c`.
 * - This work builds on existing AREPO functionality, maintaining compatibility
 */

#include "../hydro/eff_internal_energy.h"
#include <math.h>

// Toggle to enable or disable density-threshold-based thermal evolution
int threshold_density = 0; // 0 for false, 1 for true - set to `true` for density-dependent evolution, `false` for scale-factor-only evolution

// Calculate the density threshold as a function of the scale factor
// Values derived from CAMELS CV0 simulations from redshifts z=6 to z=0
double calculateDensityThreshold(double time) {
    double threshold_z6 = 3.5;
    double threshold_z0 = 5.9;
    return threshold_z6 + (threshold_z0 - threshold_z6) * (time - 0.1429) / (1.0 - 0.1429);
}

/*
 * Function: getEffInternalEnergy
 * Purpose:
 * - Computes the effective internal energy (U) for a given gas element based on the 
 *   selected mode of thermal evolution and its density.
 * Behavior:
 * - If `threshold_density = true`, uses density-based power-law fits above a critical 
 *   density threshold.
 * - If `threshold_density = false`, applies time-dependent thermal evolution models 
 *   uniformly across all densities.
 */
double getEffInternalEnergy(InternalEnergyOption option, double effInternalEnergy, double time, double density) {
    double U;
    double density_threshold = threshold_density ? calculateDensityThreshold(time) : -1; // Disable threshold if off
    double log_rho = log10(density);

    // Coefficients for density-based power-law fits (slope and intercept)
    // Derived from CAMELS simulation snapshots across redshift
    const double slopes[] = {
        0.49, 0.48, 0.54, 0.53, 0.51, 0.50, 0.51, 0.51, 0.50, 0.50,
        0.49, 0.48, 0.48, 0.47, 0.48, 0.48, 0.49, 0.50, 0.50, 0.52,
        0.53, 0.55, 0.58, 0.59, 0.62, 0.64, 0.67, 0.70, 0.70, 0.67,
        0.67, 0.68, 0.61, 0.56
    };
    const double intercepts[] = {
        0.71, 0.63, 0.23, 0.24, 0.27, 0.26, 0.20, 0.19, 0.17, 0.17,
        0.17, 0.21, 0.18, 0.17, 0.13, 0.06, -0.04, -0.13, -0.15, -0.26,
        -0.38, -0.50, -0.71, -0.82, -1.08, -1.20, -1.41, -1.64, -1.70, -1.56,
        -1.57, -1.67, -1.25, -0.98
    };
    const double time_intervals[] = {
        0.143, 0.167, 0.200, 0.222, 0.250, 0.263, 0.275, 0.289, 0.303, 0.318,
        0.333, 0.350, 0.367, 0.384, 0.404, 0.423, 0.444, 0.466, 0.488, 0.512,
        0.537, 0.564, 0.591, 0.620, 0.651, 0.682, 0.716, 0.751, 0.788, 0.826,
        0.866, 0.909, 0.954, 1.000
    };

    // Determine the appropriate time interval
    int num_intervals = sizeof(slopes) / sizeof(slopes[0]);
    int interval = -1;

    for (int i = 0; i < num_intervals; i++) {
        if (time <= time_intervals[i]) {
            interval = i;
            break;
        }
    }

    // Default fallback if time is out of bounds
    if (interval == -1) {
        U = effInternalEnergy;
    }
    // Case 1: `threshold_density = true`
    // Apply density-dependent EoS above the dynamically determined threshold
    else if (threshold_density && log_rho > density_threshold) {
        double slope = slopes[interval];
        double intercept = intercepts[interval];
        U = pow(10, slope * log_rho + intercept);
    }
    // Case 2: `threshold_density = false`
    // Apply simpler time-dependent models across all regions
    else {
        switch (option) {
            case CONSTANT:
                U = 758.61; // Constant value
                break;

            case LINEAR:
                U = 34.55 + 3453.76 * (time - 0.143); // Linear model
                break;

            case POLYNOMIAL_PIECEWISE:
                U = 34.55 +
                    4119.35 * pow(time - 0.143, 1) +
                    -20953.19 * pow(time - 0.143, 2) +
                    67292.51 * pow(time - 0.143, 3) +
                    -69033.16 * pow(time - 0.143, 4) +
                    21621.37 * pow(time - 0.143, 5); // Polynomial fit
                break;

            default:
                U = effInternalEnergy;
                break;
        }
    }

    return U;
}
