/*
   Sensore gesture
   Futura Academy: https://academy.futuranet.it/
   Webinar 2: Esp32 Futura Demoboard
   Dario Ciceri
   Canale YouTube: https://www.youtube.com/channel/UCuPuHsNjWX7huiztYu9ROQA
   Profilo Instagram: https://www.instagram.com/_dario.ciceri_/
*/

#include "Arduino.h"
#include "Gesture.h"
#include <SparkFun_APDS9960.h>

// Global variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();

Gesture::Gesture(int LIGHT_INT_HIGH, int LIGHT_INT_LOW, int PROX_INT_HIGH, int PROX_INT_LOW, OLED *o) {
  this->LIGHT_INT_HIGH  = LIGHT_INT_HIGH;
  this->LIGHT_INT_LOW = LIGHT_INT_LOW;
  this->PROX_INT_HIGH = PROX_INT_HIGH;
  this->PROX_INT_LOW = PROX_INT_LOW;
  oled = o;
}

void Gesture::init() {
  Serial.println("_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-");
  lightSensorInterruptEnabled = true;
  proximitySensorInterruptEnabled = true;
  gestureSensorInterruptEnabled = true;
  intPin = 27;
  pinMode(intPin, INPUT);
  isr_flag = 0;

  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println("Sensore gesture connesso!");
  } else {
    Serial.println("Sensore gesture non trovato! Controllare bene le connesioni!");
    while (1) delay(100);
  }

  if ( apds.enableGestureSensor(gestureSensorInterruptEnabled) ) {
    Serial.println(F("Sensore gesture: operativo!"));
  } else {
    Serial.println(F("Sensore gesture: in stato di errore!"));
  }

  // Set high and low interrupt thresholds
  if ( !apds.setLightIntLowThreshold(LIGHT_INT_LOW) ) {
    Serial.println(F("Sensore luminosità: errore durante la scrittura della soglia bassa!"));
  }
  if ( !apds.setLightIntHighThreshold(LIGHT_INT_HIGH) ) {
    Serial.println(F("Sensore luminosità: errore durante la scrittura della soglia alta!"));
  }

  // Start running the APDS-9960 light sensor
  if ( apds.enableLightSensor(lightSensorInterruptEnabled) ) {
    Serial.println(F("Sensore di luminosità: operativo!"));
  } else {
    Serial.println(F("Sensore di luminosità: in stato di errore!"));
  }

  // Read high and low interrupt thresholds
  if ( !apds.getLightIntLowThreshold(threshold) ) {
    Serial.println(F("Sensore di luminosità: errore durante la lettura della soglia bassa!"));
  } else {
    Serial.print(F("Sensore di luminosità: soglia bassa: "));
    Serial.println(threshold);
  }
  if ( !apds.getLightIntHighThreshold(threshold) ) {
    Serial.println(F("Sensore di luminosità: errore durante la lettura della soglia alta!"));
  } else {
    Serial.print(F("Sensore di luminosità: soglia alta: "));
    Serial.println(threshold);
  }

  // Enable interrupts
  if ( !apds.setAmbientLightIntEnable(1) ) {
    Serial.println(F("Sensore di luminosità: errore durante l'abilitazione degli interrupt!"));
  }

  // Adjust the Proximity sensor gain
  if ( !apds.setProximityGain(PGAIN_2X) ) {
    Serial.println(F("Sensore di prossimità: qualcosa è andato storto durante l'impostazione di PGAIN!"));
  }

  // Set proximity interrupt thresholds
  if ( !apds.setProximityIntLowThreshold(PROX_INT_LOW) ) {
    Serial.println(F("Sensore di prossimità: errore durante la scrittura della soglia bassa!"));
  }
  if ( !apds.setProximityIntHighThreshold(PROX_INT_HIGH) ) {
    Serial.println(F("Sensore di prossimità: errore durante la scrittura della soglia alta!"));
  }

  // Start running the APDS-9960 proximity sensor (interrupts)
  if ( apds.enableProximitySensor(proximitySensorInterruptEnabled) ) {
    Serial.println(F("Sensore di prossimità: operativo!"));
  } else {
    Serial.println(F("Sensore di prossimità: in stato di errore!"));
  }

  // Wait for initialization and calibration to finish
  delay(500);
}

void Gesture::handleGesture() {
  if ( apds.isGestureAvailable() ) {
    Serial.println();
    Serial.println("Dati sensore gesture:");
    oled->reset();
    switch ( apds.readGesture() ) {
      case DIR_UP:
        oled->showArrow(0);
        Serial.println("SU");
        break;
      case DIR_DOWN:
        oled->showArrow(1);
        Serial.println("GIU'");
        break;
      case DIR_LEFT:
        oled->showArrow(2);
        Serial.println("SINISTRA");
        break;
      case DIR_RIGHT:
        oled->showArrow(3);
        Serial.println("DESTRA");
        break;
      case DIR_NEAR:
        oled->showNear();
        Serial.println("VICINO");
        break;
      case DIR_FAR:
        oled->showFar();
        Serial.println("LONTANO");
        break;
      default:
        Serial.println("STANBY");
    }
  }
}
