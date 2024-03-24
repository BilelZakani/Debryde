#include <Arduino.h>
#include <OneWire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define PI 3.1415926535897932384626433832795


float getTemp(void);


Adafruit_MPU6050 mpu;



int DS18S20_Pin = 2; //DS18S20 Signal pin on digital 2


int sensorValueA0 = 0; 
int sensorValueA1 = 0; 
int sensorValueA2 = 0;
 
void setup() 
{
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial.println("init...");
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}

void loop() 
{ 
  // DeepSleep
  unsigned int sleepCounter;
for (sleepCounter = 7.5; sleepCounter > 0; sleepCounter--)
{
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  
}
  
  
  // Capteur de turbidité 
  int sensorValue = analogRead(A0);// read the input on analog pin 0:
  float voltage = sensorValue * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  
  // Capteur de température 
  float temperature = getTemp();
  Serial.println(temperature);
  delay(100); //just here to slow down the output so it is easier to read


  sensorValueA0 =voltage  ;
  sensorValueA1 = temperature;
  sensorValueA2 = analogRead(A2);
  
  
  Serial1.print("{ID:4,A0:");   // ID différent pour chaque groupe
  Serial1.print(sensorValueA0);
  Serial1.print(",A1:");
  Serial1.print(sensorValueA1);
  Serial1.print(",A2:");
  Serial1.print(sensorValueA2);
  Serial1.println("}");
 
  delay(3000);


  Serial.print(" Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(" \n");
  Serial.print(" Angle X: ");
  Serial.print(acos(a.acceleration.x/9.81)*360/(2*PI));
   Serial.print(" \n");
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
   Serial.print(" \n");
  Serial.print(" Angle Y: ");
  Serial.print(acos(a.acceleration.y/9.81)*360/(2*PI));
   Serial.print(" \n");
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
   Serial.print(" \n");
  Serial.print(" Angle Z: ");
  Serial.print(acos(a.acceleration.z/9.81)*360/(2*PI));
   Serial.print(" \n");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");
  delay(500);
}

float getTemp()
{
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad


  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;

  


  return TemperatureSum;
}
