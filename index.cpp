#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "Root4root_VLC.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

Root4root_VLC<LiquidCrystal_I2C> vlc1(&lcd, 10, 2, 0, 0);
Root4root_VLC<LiquidCrystal_I2C> vlc2(&lcd, 5, 1, 11, 0);
Root4root_VLC<LiquidCrystal_I2C> vlc3(&lcd, 5, 1, 11, 1);

char container[6] = {0}; //_TEST\0

void setup()
{
    lcd.init();
    lcd.backlight();
}

void loop()
{
    vlc1.displayString("vlc1");
    vlc1.setLine(1);
    vlc1.displayString("1111111111");
    delay(1000);

    vlc2.displayString("vlc2");
    delay(1000);

    sprintf(container,"%5s", "vlc3");
    vlc3.displayString(container);
    delay(1000);


    vlc2.clear();
    delay(1000);
    vlc3.clear();
    delay(1000);
    vlc1.clear();

    delay(5000);
}
