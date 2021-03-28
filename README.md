GY951 AHRS
---

**9 Degree of Measurement Attitude and Heading Reference System** for GY-951, both the version with the HMC5883L magnetometer and IST8310 magnetometer.

This is a modified version of the Razor AHRS code written by ptrbrtz, since the GY was a clone of the Sparkfun Razor, but the Razor does not have the IST8310.

Calibration and usage is mostly the same.

Tutorial
---

You find a [detailed tutorial in the Wiki](https://github.com/ptrbrtz/razor-9dof-ahrs/wiki/Tutorial).

Quick setup
---

### Razor AHRS Firmware and *Processing* Test Sketch

Select your hardware in `Arduino/Razor_AHRS/Razor_AHRS.ino` under `"USER SETUP AREA"` / `"HARDWARE OPTIONS"`.
Upload the firmware using *Arduino*.  
Run `Processing/Razor_AHRS_test/Razor_AHRS_test.pde` using *Processing*.

### Optional: Mac OS X / Unix / Linux C++ Interface

Compile test program:

    g++ Example.cpp RazorAHRS.cpp -Wall -D_REENTRANT -lpthread -o example

Run it:

    ./example

Sorry, no Windows support. But you could try to compile using cygwin.

### Optional: Android Interface

About Razor AHRS and Android Bluetooth: Bluetooth seems to be even more picky on Android than it is anyway. Be sure to have a look at the section about Android Bluetooth in the [tutorial](https://github.com/ptrbrtz/razor-9dof-ahrs/wiki/Tutorial).

Compiling and running the test app: Open up your Android-ready *Eclipse* and import both projects using `File` → `Import...` → `General` → `Existing Projects into Workspace` with the root folder being `Android/`. Then build and run the test app from *Eclipse*.

Building your own app:

* Your app needs to target Android 2.0 (API Level 5) or later. The RazorAHRS *Library Project* has to be present in your Workspace. Add the library to your app under `Project Properties` → `Android` → `Library`.

* In case you want yaw/heading to reference "true north" and not just magnetic north, you can use the included `DeclinationHelper` class to find out about declination at your current position.
  
* You have to specify these uses-permissions in the AndroidManifest.xml:  
`android.permission.BLUETOOTH` and `android.permission.BLUETOOTH_ADMIN`
        
* If you want to use the DeclinationHelper class you also need:  
`android.permission.ACCESS_FINE_LOCATION` and `android.permission.ACCESS_COARSE_LOCATION`

Donation
---

If the code and the tutorial helped you and you'd like to buy me a beer to say thanks, I'll be happy and say cheers!

PayPal:

[![](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=KNQHN837SKX8Q)
