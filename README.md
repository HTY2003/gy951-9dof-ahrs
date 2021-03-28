GY-951 AHRS
---

**9 Degree of Measurement Attitude and Heading Reference System** for GY-951, both the version with the HMC5883L magnetometer and IST8310 magnetometer.

This is a modified version of the [Razor AHRS code written by ptrbrtz](https://github.com/Razor-AHRS/razor-9dof-ahrs), since the GY was a clone of the Sparkfun Razor, but the HMC5883L on the GY was replaced with the IST8310 which the Razor does not have.

Calibration and usage is mostly the same.

Tutorial
---

You find a [detailed tutorial in the original Wiki](https://github.com/ptrbrtz/razor-9dof-ahrs/wiki/Tutorial).

### Razor AHRS Firmware and *Processing* Test Sketch

Select your hardware in `Arduino/Razor_AHRS/Razor_AHRS.ino` under `"USER SETUP AREA"` / `"HARDWARE OPTIONS"`.
Upload the firmware using *Arduino*.  
Run `Processing/Razor_AHRS_test/Razor_AHRS_test.pde` using *Processing*.
