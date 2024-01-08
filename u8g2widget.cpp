#include "u8g2widget.h"


int u8g2widgetDrawFrame = 0;


u8g2widget::u8g2widget() {}

u8g2widget::u8g2widget(int x, int y, int w, int h): posx(x), posy(y), width(w), height(h) {}

void u8g2widget::setVal() {}

void u8g2widget::update() {}

void u8g2widget::show(U8G2 *u) {
  if (u8g2widgetDrawFrame) {
    u->drawFrame(
      posx + (width < 0 ? width : 0),
      posy + (height < 0 ? height : 0), 
      width < 0 ? -width : width,
      height < 0 ? -height : height
    );
  }
}

void u8g2widget::setPos(int x, int y) {
  posx = x; posy = y;
}

u8g2widgetFrame::u8g2widgetFrame(int x, int y, int w, int h, int t):
  u8g2widget(x, y, w, h), thickness(t) {}

void u8g2widgetFrame::show(U8G2 *u) {
  int x = posx;
  int y = posy;
  int w = width;
  int h = height;
  int t = thickness;

  if (w < 0) { x += w; w = -w; }
  if (h < 0) { y += h; h = -h; }
  
  while (t && w && h) {
    u->drawFrame(x++, y++, w, h);
    w -= 2; h -= 2; t--;
  }
  u8g2widget::show(u);
}


u8g2widgetText::u8g2widgetText() {}

u8g2widgetText::u8g2widgetText(int x, int y, int w, int h, const uint8_t *f, String t): 
  u8g2widget(x, y, w, h), font(f), text(t) {}

u8g2widgetText::u8g2widgetText(int x, int y, int w, int h, const uint8_t *f, String (*c)()): 
  u8g2widget(x, y, w, h), font(f), callback(c) {}

void u8g2widgetText::moveTo(U8G2 *u) {
  u->setCursor(posx + (width - u->getStrWidth(text.c_str())) / 2,
               posy + (height + u->getAscent() + u->getDescent()) / 2);
}

void u8g2widgetText::setVal(String t) { text = t; }

void u8g2widgetText::update() { if (callback) text = callback(); }

void u8g2widgetText::show(U8G2 *u) {
  if (font) u->setFont(font);
  moveTo(u);
  u->print(text);
  u8g2widget::show(u);
}


void u8g2widgetTextLeft::moveTo(U8G2 *u) {
  u->setCursor(posx,
               posy + (height + u->getAscent() + u->getDescent()) / 2);
}


void u8g2widgetTextRight::moveTo(U8G2 *u) {
  u->setCursor(posx + width - u->getStrWidth(text.c_str()),
               posy + (height + u->getAscent() + u->getDescent()) / 2);
}


u8g2widgetXBMP::u8g2widgetXBMP() {}

u8g2widgetXBMP::u8g2widgetXBMP(int x, int y, int w, int h, int i, const unsigned char* d):
  u8g2widget(x, y, w, h), color(i), dxbm(d) {}

u8g2widgetXBMP::u8g2widgetXBMP(int x, int y, int w, int h, int i, const unsigned char* (*c)()): 
  u8g2widget(x, y, w, h), color(i), callback(c) {}

void u8g2widgetXBMP::setVal(const unsigned char* d) { dxbm = d; }

void u8g2widgetXBMP::update() { if (callback) dxbm = callback(); }

void u8g2widgetXBMP::show(U8G2 *u) {
  int ci = u->getColorIndex();
  u->setColorIndex(color);
  u->drawXBMP(posx, posy, width, height, dxbm);
  u->setColorIndex(ci);
  u8g2widget::show(u);
}


u8g2widgetHBar::u8g2widgetHBar() {}

u8g2widgetHBar::u8g2widgetHBar(int x, int y, int w, int h, int p):
  u8g2widget(x, y, w, h), percent(p) {}

u8g2widgetHBar::u8g2widgetHBar(int x, int y, int w, int h, int (*c)()):
  u8g2widget(x, y, w, h), callback(c) {}

void u8g2widgetHBar::setVal(int p) { percent = p; }

void u8g2widgetHBar::update() { if (callback) percent = callback(); }

void u8g2widgetHBar::show(U8G2 *u) {
  int x = posx;
  int y = posy;
  int w = width * (percent) / 100;
  int h = height;
  if (w < 0) { x += w; w = -w; }
  if (h < 0) { y += h; h = -h; }
  u->drawBox(x, y, w, h);
  u8g2widget::show(u);
}


void u8g2widgetVBar::show(U8G2 *u) {
  int x = posx;
  int y = posy;
  int w = width;
  int h = height * (percent) / 100;
  if (w < 0) { x += w; w = -w; }
  if (h < 0) { y += h; h = -h; }
  u->drawBox(x, y, w, h);
  u8g2widget::show(u);
}


u8g2widgetArcBar::u8g2widgetArcBar() {}

u8g2widgetArcBar::u8g2widgetArcBar(int x, int y, int w, int h, int l, int f0, int ft, int p):
  u8g2widget(x, y, w, h), linewidth(l), fi0(f0), fiTot(ft), percent(p) {}

u8g2widgetArcBar::u8g2widgetArcBar(int x, int y, int w, int h, int l, int f0, int ft, int (*c)()):
  u8g2widget(x, y, w, h), linewidth(l), fi0(f0), fiTot(ft), callback(c) {}

void u8g2widgetArcBar::setVal(int p) { percent = p; }

void u8g2widgetArcBar::update() { if (callback) percent = callback(); }

void u8g2widgetArcBar::show(U8G2 *u) {
  float x1, y1, x2, y2;
  int fi = fi0;
  int s = 1;
  int dfi = 10;
  int fi1 = fi0 + fiTot * (percent) / 100;
  int px = posx + width / 2;
  int py = posy + height / 2;
  int rad0x = width / 2 - linewidth;
  int rad1x = width / 2;
  int rad0y = height / 2 - linewidth;
  int rad1y = height / 2;
      
  if (fiTot < 0) { 
    s = -1;
    dfi = -dfi;
  }

  x1 = cos(fi * 2.0 * PI / 360.0);
  y1 = sin(fi * 2.0 * PI / 360.0);
  fi += dfi;
  
  while (s*fi < s*fi1) {
    x2 = x1; y2 = y1; 
    x1 = cos(fi * 2.0 * PI / 360.0);
    y1 = sin(fi * 2.0 * PI / 360.0);
    fi += dfi;
    
    u->drawTriangle(px + rad0x * x1, py - rad0y * y1, 
                    px + rad0x * x2, py - rad0y * y2,
                    px + rad1x * x2, py - rad1y * y2); 
    u->drawTriangle(px + rad0x * x1, py - rad0y * y1, 
                    px + rad1x * x1, py - rad1y * y1,
                    px + rad1x * x2, py - rad1y * y2); 
  }

  x2 = x1; y2 = y1; fi = fi1;
  x1 = cos(fi * 2.0 * PI / 360.0);
  y1 = sin(fi * 2.0 * PI / 360.0);

  u->drawTriangle(px + rad0x * x1, py - rad0y * y1, 
                  px + rad0x * x2, py - rad0y * y2,
                  px + rad1x * x2, py - rad1y * y2); 
  u->drawTriangle(px + rad0x * x1, py - rad0y * y1, 
                  px + rad1x * x1, py - rad1y * y1,
                  px + rad1x * x2, py - rad1y * y2); 
  u8g2widget::show(u);
}


u8g2widgetHDiscGauge::u8g2widgetHDiscGauge() {}

u8g2widgetHDiscGauge::u8g2widgetHDiscGauge(int x, int y, int w, int h, const uint8_t *f, int r, int m, int v0, int v1, int v):
  u8g2widget(x, y, w, h), font(f), radius(r), mirror(m), vmin(v0), vmax(v1), value(v) {}

u8g2widgetHDiscGauge::u8g2widgetHDiscGauge(int x, int y, int w, int h, const uint8_t *f, int r, int m, int v0, int v1, int (*c)()):
  u8g2widget(x, y, w, h), font(f), radius(r), mirror(m), vmin(v0), vmax(v1), callback(c) {}

void u8g2widgetHDiscGauge::setVal(int v) { value = v; }

void u8g2widgetHDiscGauge::update() { if (callback) value = callback(); }

void u8g2widgetHDiscGauge::show(U8G2 *u) {
  int x0 = posx + width / 2;
  int y0 = posy + (mirror ? radius : height - radius);
  float rx, ry;
  int r, rr;
  int r0, r1;
  int px0, py0;
  int px, py;
  int pxt, pyt;
  char s[10];

  r0 = value - 360.0 * atan(width / 2.0 / (radius - height)) / 2.0 / PI;
  r1 = value + 360.0 * atan(width / 2.0 / (radius - height)) / 2.0 / PI;

  if (r0 < vmin) r0 = vmin;
  if (r1 > vmax) r1 = vmax;

  u->setClipWindow(posx, posy, posx + width, posy + height);
  if (font) u->setFont(font);
  u->drawVLine(posx + width / 2, posy, height);

  px0 = x0 + (radius - 2) * cos(radians(r0 - value + 270));
  py0 = y0 - (mirror ? 2 - radius : radius - 2) * sin(radians(r0 - value + 270));
  
  for (r = r0; r <= r1; r++) {

    rr = r - value + 270;
    rx = cos(radians(rr));
    ry = mirror ? sin(radians(rr)) : -sin(radians(rr));
    px = x0 + (radius - 2) * rx;
    py = y0 + (radius - 2) * ry;

    if (r % 10 == 0) {
      pxt = x0 + (radius - 5) * rx;
      pyt = y0 + (radius - 5) * ry;
      if ((px >= 0) && (py >= 0) && (pxt >= 0) && (pyt >= 0))
        u->drawLine(px, py, pxt, pyt);

      pxt = x0 + (radius - 10 - (mirror ? u->getAscent() : 0)) * rx;
      pyt = y0 + (radius - 10 - (mirror ? u->getAscent() : 0)) * ry;
      sprintf(s, "%d", r);
      u->drawStr(pxt - u->getStrWidth(s) / 2, pyt, s);

    }

    if ((px >= 0) && (py >= 0) && (px0 >= 0) && (py0 >= 0))
      u->drawLine(px, py, px0, py0);

    px0 = px;
    py0 = py;

  }

  u->setMaxClipWindow();
  u8g2widget::show(u);
}


void u8g2widgetVDiscGauge::show(U8G2 *u) {
  int x0 = posx + (mirror ? radius : width - radius);
  int y0 = posy + height / 2;
  float rx, ry;
  int r0, r1;
  int r, rr;
  int px0, py0;
  int px, py;
  int pxt, pyt;
  char s[10];

  r0 = value - 360.0 * atan(height / 2.0 / (radius - width)) / 2.0 / PI;
  r1 = value + 360.0 * atan(height / 2.0 / (radius - width)) / 2.0 / PI;

  if (r0 < vmin) r0 = vmin;
  if (r1 > vmax) r1 = vmax;

  u->setClipWindow(posx, posy, posx + width, posy + height);
  if (font) u->setFont(font);
  u->drawHLine(posx, posy + height / 2, width);
  
  px0 = x0 + (mirror ? 2 - radius : radius - 2) * cos(radians(r0 - value));
  py0 = y0 - (radius - 2) * sin(radians(r0 - value));
  
  for (r = r0; r <= r1; r++) {

    rr = r - value;
    rx = mirror ? -cos(radians(rr)) : cos(radians(rr));
    ry = -sin(radians(rr));
    px = x0 + (radius - 2) * rx;
    py = y0 + (radius - 2) * ry;

    if (r % 10 == 0) {
      pxt = x0 + (radius - 5) * rx;
      pyt = y0 + (radius - 5) * ry;
      if ((px >= 0) && (py >= 0) && (pxt >= 0) && (pyt >= 0))
        u->drawLine(px, py, pxt, pyt);

      pxt = x0 + (radius - 10) * rx;
      pyt = y0 + (radius - 10) * ry;
      sprintf(s, "%d", r);
      u->drawStr(pxt - u->getStrWidth(s) / 2, pyt, s);

    }

    if ((px >= 0) && (py >= 0) && (px0 >= 0) && (py0 >= 0))
      u->drawLine(px, py, px0, py0);

    px0 = px;
    py0 = py;

  }

  u->setMaxClipWindow();
  u8g2widget::show(u);
}

