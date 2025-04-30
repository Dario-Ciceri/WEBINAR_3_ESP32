/*
   Sensore fulmini
   Futura Academy: https://academy.futuranet.it/
   Webinar 2: Esp32 Futura Demoboard
   Dario Ciceri
   Canale YouTube: https://www.youtube.com/channel/UCuPuHsNjWX7huiztYu9ROQA
   Profilo Instagram: https://www.instagram.com/_dario.ciceri_/
*/

#include "Arduino.h"
#include "Lightning.h"
#include "SparkFun_AS3935.h"

#define AS3935_ADDR 0x03
#define INDOOR 0x12
#define OUTDOOR 0xE
#define LIGHTNING_INT 0x08
#define DISTURBER_INT 0x04
#define NOISE_INT 0x01

#define PIN 35

SparkFun_AS3935 lightning(AS3935_ADDR);

void Lightning::init()
{
  Serial.println("_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-");
  pinMode(PIN,INPUT);
  noiseFloor = 2;
  watchDogVal = 2;
  spike = 2;
  lightningThresh = 1;
  // This variable holds the number representing the lightning or non-lightning
  // event issued by the lightning detector.
  intVal = 0;
  strikeDistance = -1;
  strikeEnergy = -1;

  //Wire.begin();
  if ( !lightning.begin() ) {
    Serial.println ("Sensore fulmini non trovato! Controllare bene le connessioni!");
    while(1) delay(100);
  }
  else
    Serial.println("Sensore fulmini connesso!");
  // "Disturbers" are events that are false lightning events. If you find
  // yourself seeing a lot of disturbers you can have the chip not report those
  // events on the interrupt lines.
  lightning.maskDisturber(true);
  int maskVal = lightning.readMaskDisturber();
  Serial.print("Filtro falsi positivi: ");
  if (maskVal == 1)
    Serial.println("ATTIVO");
  else if (maskVal == 0)
    Serial.println("DISATTIVATO");

  // The lightning detector defaults to an indoor setting (less
  // gain/sensitivity), if you plan on using this outdoors
  // uncomment the following line:
  //lightning.setIndoorOutdoor(OUTDOOR);
  int enviVal = lightning.readIndoorOutdoor();
  Serial.print("Modalità di lettura: ");
  if ( enviVal == INDOOR )
    Serial.println("INTERNO");
  else if ( enviVal == OUTDOOR )
    Serial.println("ESTERNO");
  else
    Serial.println(enviVal, BIN);

  // Noise floor setting from 1-7, one being the lowest. Default setting is
  // two. If you need to check the setting, the corresponding function for
  // reading the function follows.

  lightning.setNoiseLevel(noiseFloor);

  int noiseVal = lightning.readNoiseLevel();
  Serial.print("Livelo di attenuazione del rumore: ");
  Serial.println(noiseVal);

  // Watchdog threshold setting can be from 1-10, one being the lowest. Default setting is
  // two. If you need to check the setting, the corresponding function for
  // reading the function follows.

  lightning.watchdogThreshold(watchDogVal);

  int watchVal = lightning.readWatchdogThreshold();
  Serial.print("Livello soglia contro falsi positivi: ");
  Serial.println(watchVal);

  // Spike Rejection setting from 1-11, one being the lowest. Default setting is
  // two. If you need to check the setting, the corresponding function for
  // reading the function follows.
  // The shape of the spike is analyzed during the chip's
  // validation routine. You can round this spike at the cost of sensitivity to
  // distant events.

  lightning.spikeRejection(spike);

  int spikeVal = lightning.readSpikeRejection();
  Serial.print("Livello filtro per routine di analisi fulmine: ");
  Serial.println(spikeVal);

  // This setting will change when the lightning detector issues an interrupt.
  // For example you will only get an interrupt after five lightning strikes
  // instead of one. Default is one, and it takes settings of 1, 5, 9 and 16.
  // Followed by its corresponding read function. Default is zero.

  lightning.lightningThreshold(lightningThresh);

  uint8_t lightVal = lightning.readLightningThreshold();
  Serial.print("Numero di positivi prima di confermare il fulmine: ");
  Serial.println(lightVal);

  // When the strikeDistance to the storm is estimated, it takes into account other
  // lightning that was sensed in the past 15 minutes. If you want to reset
  // time, then you can call this function.

  //lightning.clearStatistics();

  // The power down function has a BIG "gotcha". When you wake up the board
  // after power down, the internal oscillators will be recalibrated. They are
  // recalibrated according to the resonance frequency of the antenna - which
  // should be around 500kHz. It's highly recommended that you calibrate your
  // antenna before using these two functions, or you run the risk of schewing
  // the timing of the chip.

  //lightning.powerDown();
  //delay(1000);
  //if( lightning.wakeUp() )
  // Serial.println("Successfully woken up!");
  //else
  //Serial.println("Error recalibrating internal osciallator on wake up.");

  // Set too many features? Reset them all with the following function.
  //lightning.resetSettings();

}

void Lightning::read() {
  intVal = lightning.readInterruptReg();
  if (intVal == NOISE_INT) {
    Serial.println();
    Serial.println("Dati sensore fulmini:");
    Serial.println("Rumore");
  }
  else if (intVal == DISTURBER_INT) {
    Serial.println();
    Serial.println("Dati sensore fulmini:");
    Serial.println("Falso positivo");
  }
  else if (intVal == LIGHTNING_INT) {
    Serial.println();
    Serial.println("Dati sensore fulmini:");
    Serial.println("Fulmine rilevato!");
    // Lightning! Now how far away is it? strikeDistance estimation takes into
    // account previously seen events.
    strikeDistance = lightning.distanceToStorm();
    Serial.print("Distanza approssimativa: ");
    Serial.print(strikeDistance);
    Serial.println("km!");

    // "Lightning Energy" and I do place into quotes intentionally, is a pure
    // number that does not have any physical meaning.
    strikeEnergy = lightning.lightningEnergy();
    Serial.print("Energia: ");
    Serial.println(strikeEnergy);
  }
}
