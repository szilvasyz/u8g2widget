#include "Arduino.h"
#include "U8g2lib.h"
#include "mylcd16.h"

#include "u8g2widget.h"


#define DISP1_ADDR 0x3D
#define DISP2_ADDR 0x3C


U8G2_SSD1309_128X64_NONAME0_F_HW_I2C u8g2(U8G2_R0, /* reset */ U8X8_PIN_NONE);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g22(U8G2_R0, /* reset */ U8X8_PIN_NONE);


int getValue();
String getStrValue();

u8g2widget* widgets[] = {
  new u8g2widgetText(20, 0, 60, 25, u8g2_font_10x20_mf, "Hello"),
  new u8g2widgetTextLeft(80, 0, 28, 15, u8g2_font_4x6_tf, "World"),
  new u8g2widgetTextRight(80, 15, 28, 15, u8g2_font_4x6_tf, getStrValue),
  new u8g2widgetHDiscGauge(20, 50, 88, 14, u8g2_font_tiny5_tr, 100, 1, -40, 200, getValue),
  new u8g2widgetVDiscGauge(0, 0, 20, 64, u8g2_font_tiny5_tr, 100, 0, -4, 114, getValue),
  new u8g2widgetVBar(120, 63, 8, -63, getValue),
  new u8g2widgetArcBar(30, 25, 60, 25, 5, 225, -270, getValue),
  nullptr
};


u8g2widget* widgets2[] = {
  new u8g2widgetText(20, 0, 60, 25, mylcd16, getStrValue),
  new u8g2widgetTextLeft(80, 0, 28, 15, u8g2_font_4x6_tf, "World"),
  new u8g2widgetTextRight(80, 15, 28, 15, u8g2_font_4x6_tf, "World"),
  new u8g2widgetHDiscGauge(20, 30, 88, 20, u8g2_font_tiny5_tr, 100, 0, 0, 100, getValue),
  new u8g2widgetVDiscGauge(108, 0, 20, 64, u8g2_font_tiny5_tr, 100, 1, 20, 80, getValue),
  new u8g2widgetHBar(0, 55, 50, 8, getValue),
  nullptr
};


const float degconv = ((2 * PI) / 360.0);
int v = 0;
int dv = 1;


int getValue() { return v; }
String getStrValue() { return String(v); }


void setup() {

  Serial.begin(115200);

  u8g2.setI2CAddress(DISP1_ADDR << 1);
  u8g22.setI2CAddress(DISP2_ADDR << 1);

  u8g2.begin();
  u8g22.begin();

  u8g2.setBusClock(1000000);
  u8g22.setBusClock(1000000);
  
  u8g22.setFont(u8g2_font_10x20_mf);

  //u8g2widgetDrawFrame = 1;

  Serial.println(u8g2.getBusClock());
  Serial.println(u8g22.getBusClock());
}


void loop() {

  u8g2widget** w;

  for (w = widgets; *w; w++) (*w)->update();
  u8g2.clearBuffer();
  for (w = widgets; *w; w++) (*w)->show(&u8g2);
  u8g2.sendBuffer();

  for (w = widgets2; *w; w++) (*w)->update();
  u8g22.clearBuffer();
  for (w = widgets2; *w; w++) (*w)->show(&u8g22);
  u8g22.sendBuffer();

  if (dv > 0) {
    if (v >= 120)
      dv = -1;
    else
      v += dv;
  }
  else {
    if (v <= 0)
      dv = 1;
    else
      v += dv;
  }

  delay(20);

}