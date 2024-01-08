#ifndef U8G2WIDGET_H
#define U8G2WIDGET_H

#include "U8g2lib.h"


extern int u8g2widgetDrawFrame;


class u8g2widget {
  protected:
    int posx, posy;
    int width, height;

  public:
    u8g2widget();
    u8g2widget(int x, int y, int w, int h);
    virtual void setVal();
    virtual void update();
    virtual void show(U8G2 *u);
    void setPos(int x, int y);
};


class u8g2widgetFrame : public u8g2widget {       
  using u8g2widget::u8g2widget;    
  protected:   
    int thickness = 1; 

  public:
    u8g2widgetFrame(int x, int y, int w, int h, int t);
//    virtual void update();
    virtual void show(U8G2 *u);
};


class u8g2widgetText : public u8g2widget {       
  protected:   
    const uint8_t *font = nullptr;
    String text; 
    String (*callback)() = nullptr;
    virtual void moveTo(U8G2 *u);

  public:
    u8g2widgetText();
    u8g2widgetText(int x, int y, int w, int h, const uint8_t *f, String t);
    u8g2widgetText(int x, int y, int w, int h, const uint8_t *f, String (*c)());
    virtual void setVal(String t);
    virtual void update();
    virtual void show(U8G2 *u);
};


class u8g2widgetTextLeft : public u8g2widgetText {
  using u8g2widgetText::u8g2widgetText;    
  protected:   
    virtual void moveTo(U8G2 *u);
};


class u8g2widgetTextRight : public u8g2widgetText {       
  using u8g2widgetText::u8g2widgetText;    
  protected:   
    virtual void moveTo(U8G2 *u);
};


class u8g2widgetXBMP : public u8g2widget {       
  protected:   
    int color = 1;
    const unsigned char* dxbm; 
    const unsigned char* (*callback)() = nullptr;

  public:
    u8g2widgetXBMP();
    u8g2widgetXBMP(int x, int y, int w, int h, int i, const unsigned char* d);
    u8g2widgetXBMP(int x, int y, int w, int h, int i, const unsigned char* (*c)());
    virtual void setVal(const unsigned char* d);
    virtual void update();
    virtual void show(U8G2 *u);
};


class u8g2widgetHBar : public u8g2widget {       
  protected:   
    int percent; 
    int (*callback)() = nullptr;

  public:
    u8g2widgetHBar();
    u8g2widgetHBar(int x, int y, int w, int h, int p);
    u8g2widgetHBar(int x, int y, int w, int h, int (*c)());
    virtual void setVal(int p);
    virtual void update();
    virtual void show(U8G2 *u);
};


class u8g2widgetVBar : public u8g2widgetHBar {       
  using u8g2widgetHBar::u8g2widgetHBar;    

  public:
    virtual void show(U8G2 *u);
};


class u8g2widgetArcBar : public u8g2widget {       
  protected:   
    int percent; 
    int (*callback)() = nullptr;
    int linewidth;
    int fi0;
    int fiTot;

  public:
    u8g2widgetArcBar();
    u8g2widgetArcBar(int x, int y, int w, int h, int l, int f0, int ft, int p);
    u8g2widgetArcBar(int x, int y, int w, int h, int l, int f0, int ft, int (*c)());
    virtual void setVal(int p);
    virtual void update();
    virtual void show(U8G2 *u);
};


class u8g2widgetHDiscGauge : public u8g2widget {       
  protected:   
    const uint8_t *font = nullptr;
    int value; 
    int (*callback)() = nullptr;
    int mirror = 0;
    int radius;
    int vmin;
    int vmax;

  public:
    u8g2widgetHDiscGauge();
    u8g2widgetHDiscGauge(int x, int y, int w, int h, const uint8_t *f, int r, int m, int v0, int v1, int v);
    u8g2widgetHDiscGauge(int x, int y, int w, int h, const uint8_t *f, int r, int m, int v0, int v1, int (*c)());
    virtual void setVal(int v);
    virtual void update();
    virtual void show(U8G2 *u);
};


class u8g2widgetVDiscGauge : public u8g2widgetHDiscGauge {       
  using u8g2widgetHDiscGauge::u8g2widgetHDiscGauge;    
  public:
    virtual void show(U8G2 *u);
};


#endif
