/*
   OLED
   Futura Academy: https://academy.futuranet.it/
   Webinar 2: Esp32 Futura Demoboard
   Dario Ciceri
   Canale YouTube: https://www.youtube.com/channel/UCuPuHsNjWX7huiztYu9ROQA
   Profilo Instagram: https://www.instagram.com/_dario.ciceri_/
*/

#ifndef OLED_h
#define OLED_h

#include "Arduino.h"

class OLED
{
  public:
    void init();
    void logo();
    void showArrow(int rot);
    void showNear();
    void showFar();
    void show3dCube();
    void reset();
    void clear();
    void displayText(int x, int y, String text);
  private:
    void draw_vertices();
    void draw_wireframe();
};

#endif
