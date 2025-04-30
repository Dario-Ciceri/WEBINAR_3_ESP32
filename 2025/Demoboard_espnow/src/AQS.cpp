/*
   Sensore qualità dell'aria
   Futura Academy: https://academy.futuranet.it/
   Webinar 2: Esp32 Futura Demoboard
   Dario Ciceri
   Canale YouTube: https://www.youtube.com/channel/UCuPuHsNjWX7huiztYu9ROQA
   Profilo Instagram: https://www.instagram.com/_dario.ciceri_/
*/

#include "Arduino.h"
#include <Wire.h>
#include "AQS.h"
#include "SparkFun_SGP30_Arduino_Library.h"

SGP30 sgp30; //create an object of the SGP30 class
SGP30ERR error;
SGP30ERR rawError;
SGP30ERR aqError;

void AQS::init() {
  Serial.println("_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-");
  Wire.begin();
  if (sgp30.begin() == false) {
    Serial.println("Sensore qualità dell'aria non trovato! Controllare bene le connessioni!");
    while(1) delay(100);
  }
  Serial.println("Sensore qualità dell'aria connesso!");
  //Get SGP30's ID
  sgp30.getSerialID();
  //Get version number
  sgp30.getFeatureSetVersion();
  Serial.print("SerialID: 0x");
  Serial.print((unsigned long)sgp30.serialID, HEX);
  Serial.print("\tFeature Set Version: 0x");
  Serial.println(sgp30.featureSetVersion, HEX);
  Serial.println("Avvio procedura di self test e calibrazione del sensore!");
  //measureTest() should not be called after a call to initAirQuality()
  error = sgp30.measureTest();
  if (error == SGP30_SUCCESS) {
    Serial.println("Self test eseguito con successo!");
  }
  else if (error == SGP30_ERR_BAD_CRC) {
    Serial.println("CRC Failed!");
  }
  else if (error == SGP30_ERR_I2C_TIMEOUT) {
    Serial.println("I2C Timed out!");
  }
  else if (error == SGP30_SELF_TEST_FAIL) {
    Serial.println("Self test fallito!");
  }
  sgp30.initAirQuality();
  delay(1000);
  calibrate();
}

void AQS::calibrate() {
  Serial.println("Calibrazione sensore qualità dell'aria in corso:");
  for (int i = 0; i < 30; i++) {
    sgp30.measureAirQuality();
    if (i % 2 == 0)
      Serial.print("_");
    else
      Serial.print("-");
    delay(1000);
  }
  Serial.println("_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-");
  Serial.println("Sensore qualità dell'aria calibrato!");
  read();
}

SensorData AQS::read() {
  //First fifteen readings will be
  //CO2: 400 ppm  TVOC: 0 ppb
  //measure CO2 and TVOC levels
  SensorData data;

  aqError = sgp30.measureAirQuality();
  data.co2 = sgp30.CO2;
  data.tvoc = sgp30.TVOC;
  rawError = sgp30.measureRawSignals();
  data.h2 = sgp30.H2;
  data.ethanol = sgp30.ethanol;

  if (aqError == SGP30_SUCCESS && rawError == SGP30_SUCCESS) {
    Serial.println();
    Serial.println("Dati sensore qualità dell'aria:");
    Serial.print("CO2: ");
    Serial.print(data.co2);
    Serial.print(" ppm\tTVOC: ");
    Serial.print(data.tvoc);
    Serial.println(" ppb");
    //get raw values for H2 and Ethanol
    Serial.print("Raw H2: ");
    Serial.print(data.h2);
    Serial.print(" \tRaw Ethanol: ");
    Serial.println(data.ethanol);
  } else if (aqError == SGP30_ERR_BAD_CRC || rawError == SGP30_ERR_BAD_CRC) {
    Serial.println();
    Serial.println("Dati sensore qualità dell'aria:");
    Serial.println("CRC Failed!");
  }
  else if (aqError == SGP30_ERR_I2C_TIMEOUT || rawError == SGP30_ERR_I2C_TIMEOUT) {
    Serial.println();
    Serial.println("Dati sensore qualità dell'aria:");
    Serial.println("I2C Timed out!");
  }

  return data;
}

