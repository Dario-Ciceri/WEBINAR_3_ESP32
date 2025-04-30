/*
   Sensore qualità dell'aria
   Futura Academy: https://academy.futuranet.it/
   Webinar 2: Esp32 Futura Demoboard
   Dario Ciceri
   Canale YouTube: https://www.youtube.com/channel/UCuPuHsNjWX7huiztYu9ROQA
   Profilo Instagram: https://www.instagram.com/_dario.ciceri_/
*/

#ifndef Gesture_h
#define Gesture_h

#include "Arduino.h"
#include "OLED.h"

class Gesture
{
  public:
    Gesture(int LIGHT_INT_HIGH, int LIGHT_INT_LOW, int PROX_INT_HIGH, int PROX_INT_LOW, OLED *o);
    int intPin;
    int isr_flag;
    int LIGHT_INT_HIGH;
    int LIGHT_INT_LOW;
    int PROX_INT_HIGH;
    int PROX_INT_LOW;
    OLED *oled;
    uint16_t ambient_light;
    uint16_t red_light;
    uint16_t green_light;
    uint16_t blue_light;
    uint8_t proximity_data;
    uint16_t threshold;
    bool lightSensorInterruptEnabled;
    bool proximitySensorInterruptEnabled;
    bool gestureSensorInterruptEnabled;
    void init();
    void handleGesture();
};

#endif
