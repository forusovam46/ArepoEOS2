Arepo public version
====================

AREPO is a massively parallel code for gravitational n-body 
systems and hydrodynamics, both on Newtonian as well as 
cosmological background. It is a flexible code that can be 
applied to a variety of different types of simulations, offering 
a number of sophisticated simulation algorithms. An description 
of the numerical algorithms employed by the code is given in the 
original code papers (Springel 2010, MNRAS, 401, 791; 
Pakmor et al. 2011, MNRAS, 418, 1392; Pakmor and Springel 2013, 
MNRAS, 432, 176; Pakmor et al. 2016, MNRAS,455,1134) and the 
release paper of this version (Weinberger et al. 2019). 

A user guide can be found under `/documentation`, which also 
includes a 'getting started' section, which is recommended for 
new users. An html version of the user guide can be created using
sphinx (https://www.sphinx-doc.org) by typing

    cd ./documentation/
    make html
    
and displayed by opening `./documentation/build/html/index.html`.

A full version of the user guide is also available on the Arepo 
homepage.

---

This EOS2 Version (Master's Thesis 2024)
-----------------------------------------------------------------

This version of AREPO includes experimental features developed as part of a master's thesis. These features aim to extend AREPO's capabilities for modeling thermal evolution in cosmological volume simulations but require further testing and refinement to achieve a fully working implementation.

1. **Experimental Thermal Evolution Models:**
   - Introduced a model for effective internal energy, allowing simulation of thermal evolution with options for density-dependent (in this version for now only as a power-law threshold to keep gas from collapsing in halos in high-density regions) and time-dependent behaviors.
   - Added a new `InternalEnergyOption` enum:
     - `NO`: Default behavior.
     - `CONSTANT`: Fixed internal energy.
     - `LINEAR`: Internal energy evolves linearly with time.
     - `POLYNOMIAL_PIECEWISE`: A polynomial fit for time-dependent internal energy evolution.
   - Included a toggle (`threshold_density`) for density-dependent evolution based on CAMELS CV0 simulations.

2. **Limitations and Future Work:**
   - This implementation is a preliminary attempt and may not produce stable or physically accurate results.
   - Further development and validation are necessary to refine the thermal evolution models.

3. **Configuration Notes:**
   - This version is designed to operate with gamma close to 1 e.g., `GAMMA = 1.001` (for quasi-isothermal behavior - we reset the total energy after each timestep to ensure the specific internal energy or sound speed) and non-radiative simulations pre-settings. Ensure that the configuration file (`Config.sh`) is updated accordingly. Then you pick your `InternalEnergyOption` in `/src/main/allvars.c`. You can change the concrete prescriptions of the model in `eff_internal_energy.c`. The one implemented in the code were derived from simulation outputs of CAMELS CV0 simulations https://users.flatironinstitute.org/~camels/Sims/IllustrisTNG/CV/

4. **Files Added/Modified:**
   - `eff_internal_energy.c` and `eff_internal_energy.h` for internal energy calculations.
   - Modifications to `update_primitive_variables` to integrate the experimental thermal evolution models.

For further details, refer to the thesis documentation. <TODO add link> Users are advised to treat this version as a testing ground for experimental features.
