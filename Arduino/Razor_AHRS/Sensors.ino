/* This file is part of the Razor AHRS Firmware */

// I2C code to read the sensors

// Sensor I2C addresses
#define ACCEL_ADDRESS ((int16_t)0x53)
#define MAGN_ADDRESS ((int16_t)0x0E)
#define GYRO_ADDRESS ((int16_t)0x68)

// Arduino backward compatibility macros
#if ARDUINO >= 100
#define WIRE_SEND(b) Wire.write((byte)b)
#define WIRE_RECEIVE() Wire.read()
#else
#define WIRE_SEND(b) Wire.send(b)
#define WIRE_RECEIVE() Wire.receive()
#endif

void I2C_Init()
{
  Wire.begin();
}

void Accel_Init()
{
  Wire.beginTransmission(ACCEL_ADDRESS);
  WIRE_SEND(0x2D); // Power register
  WIRE_SEND(0x08); // Measurement mode
  Wire.endTransmission();
  delay(5);
  Wire.beginTransmission(ACCEL_ADDRESS);
  WIRE_SEND(0x31); // Data format register
  WIRE_SEND(0x08); // Set to full resolution
  Wire.endTransmission();
  delay(5);

  // Because our main loop runs at 50Hz we adjust the output data rate to 50Hz (25Hz bandwidth)
  Wire.beginTransmission(ACCEL_ADDRESS);
  WIRE_SEND(0x2C); // Rate
  WIRE_SEND(0x0A); // Set to 100Hz, normal operation
  Wire.endTransmission();
  delay(5);
}

// Reads x, y and z accelerometer registers
void Read_Accel()
{
  int i = 0;
  uint8_t buff[6];

  Wire.beginTransmission(ACCEL_ADDRESS);
  WIRE_SEND(0x32); // Send address to read from
  Wire.endTransmission();

  Wire.beginTransmission(ACCEL_ADDRESS);
  Wire.requestFrom(ACCEL_ADDRESS, 6); // Request 6 bytes
  while (Wire.available())            // ((Wire.available())&&(i<6))
  {
    buff[i] = WIRE_RECEIVE(); // Read one byte
    i++;
  }
  Wire.endTransmission();

  if (i == 6) // All bytes received?
  {
    // No multiply by -1 for coordinate system transformation here, because of double negation:
    // We want the gravity vector, which is negated acceleration vector.
    accel[0] = (int16_t)((((uint16_t)buff[3]) << 8) | buff[2]); // X axis (internal sensor y axis)
    accel[1] = (int16_t)((((uint16_t)buff[1]) << 8) | buff[0]); // Y axis (internal sensor x axis)
    accel[2] = (int16_t)((((uint16_t)buff[5]) << 8) | buff[4]); // Z axis (internal sensor z axis)
  }
  else
  {
    num_accel_errors++;
    if (output_errors)
      Serial.println("!ERR: reading accelerometer");
  }
}

void Magn_Init()
{
  Wire.beginTransmission(MAGN_ADDRESS);
  WIRE_SEND(0x41);
  WIRE_SEND(0x24); //Set averaging to 16x (166Hz)
  Wire.endTransmission();
  delay(5);

  Wire.beginTransmission(MAGN_ADDRESS);
  WIRE_SEND(0x42);
  WIRE_SEND(0xC0); //Set pulse duration to Normal setting (5ms)
  Wire.endTransmission();
  delay(5);
}

void Read_Magn()
{
  int i = 0, stat1 = 0;
  uint8_t buff[6];
  
#if HW__VERSION_CODE == 5883
  Wire.beginTransmission(MAGN_ADDRESS); 
  WIRE_SEND(0x03);  // Send address to read from
  Wire.endTransmission();
  
  Wire.beginTransmission(MAGN_ADDRESS); 
  Wire.requestFrom(MAGN_ADDRESS, 6);  // Request 6 bytes

#elif HW__VERSION_CODE == 8310 
  Wire.beginTransmission(MAGN_ADDRESS); //Set sensor to Single Measurement Mode
  WIRE_SEND(0x0A);
  WIRE_SEND(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(MAGN_ADDRESS);
  WIRE_SEND(0x08);
  Wire.endTransmission();
  Wire.beginTransmission(MAGN_ADDRESS);
  WIRE_SEND(0x07);
  Wire.endTransmission();
  Wire.beginTransmission(MAGN_ADDRESS);
  WIRE_SEND(0x06);
  Wire.endTransmission();
  Wire.beginTransmission(MAGN_ADDRESS);
  WIRE_SEND(0x05);
  Wire.endTransmission();
  Wire.beginTransmission(MAGN_ADDRESS);
  WIRE_SEND(0x04);
  Wire.endTransmission();
  Wire.beginTransmission(MAGN_ADDRESS);
  WIRE_SEND(0x03);
  Wire.endTransmission();
  Wire.beginTransmission(MAGN_ADDRESS);
  Wire.requestFrom(MAGN_ADDRESS, 6);
  Wire.beginTransmission(MAGN_ADDRESS); //Set sensor to Single Measurement Mode
  WIRE_SEND(0x0A);
  WIRE_SEND(0x01);
  Wire.endTransmission();
#endif

  while (Wire.available())
  {
    buff[i] = WIRE_RECEIVE();
    i++;
  }
  if (i == 6)
  {
    magnetom[0] = -1 * (int16_t)((((uint16_t)buff[1]) << 8) | buff[0]);   // X axis (internal sensor -x axis)
    magnetom[1] = (int16_t)(((((uint16_t)buff[3]) << 8) | buff[2])); // Y axis (internal sensor -y axis)
    magnetom[2] = -1 * (int16_t)(((((uint16_t)buff[5]) << 8) | buff[4])); // Z axis (internal sensor -z axis)
  }

  else
  {
    num_magn_errors++;
    if (output_errors)
      Serial.println("!ERR: reading magnetometer");
  }
}

void Gyro_Init()
{
  // Power up reset defaults
  Wire.beginTransmission(GYRO_ADDRESS);
  WIRE_SEND(0x3E);
  WIRE_SEND(0x80);
  Wire.endTransmission();
  delay(5);

  // Select full-scale range of the gyro sensors
  // Set LP filter bandwidth to 42Hz
  Wire.beginTransmission(GYRO_ADDRESS);
  WIRE_SEND(0x16);
  WIRE_SEND(0x1B); // DLPF_CFG = 3, FS_SEL = 3
  Wire.endTransmission();
  delay(5);

  // Set sample rato to 50Hz
  Wire.beginTransmission(GYRO_ADDRESS);
  WIRE_SEND(0x15);
  WIRE_SEND(0x09); //  SMPLRT_DIV = 9 (100Hz)
  Wire.endTransmission();
  delay(5);

  // Set clock to PLL with z gyro reference
  Wire.beginTransmission(GYRO_ADDRESS);
  WIRE_SEND(0x3E);
  WIRE_SEND(0x00);
  Wire.endTransmission();
  delay(5);
}

// Reads x, y and z gyroscope registers
void Read_Gyro()
{
  int i = 0;
  uint8_t buff[6];

  Wire.beginTransmission(GYRO_ADDRESS);
  WIRE_SEND(0x1D); // Sends address to read from
  Wire.endTransmission();

  Wire.beginTransmission(GYRO_ADDRESS);
  Wire.requestFrom(GYRO_ADDRESS, 6); // Request 6 bytes
  while (Wire.available())           // ((Wire.available())&&(i<6))
  {
    buff[i] = WIRE_RECEIVE(); // Read one byte
    i++;
  }
  Wire.endTransmission();

  if (i == 6) // All bytes received?
  {
    gyro[0] = (int16_t)(((((uint16_t)buff[0]) << 8) | buff[1])); // X axis (internal sensor x axis)
    gyro[1] = -1 * (int16_t)(((((uint16_t)buff[2]) << 8) | buff[3])); // Y axis (internal sensor -y axis)
    gyro[2] = (int16_t)(((((uint16_t)buff[4]) << 8) | buff[5])); // Z axis (internal sensor -z axis)
  }
  else
  {
    num_gyro_errors++;
    if (output_errors)
      Serial.println("!ERR: reading gyroscope");
  }
}
