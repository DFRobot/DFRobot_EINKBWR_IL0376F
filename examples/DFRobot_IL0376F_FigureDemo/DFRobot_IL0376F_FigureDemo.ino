#include "Arduino.h"
#include "DFRobot_EINKBWR_IL0376F.h"

/*****************Keywords instruction*****************/
//LUCENCY--->transparence
//WHITE----->black
//BLACK----->white
//RED------->red
/*****************Function instruction*****************/
//InkScreen_Error clear(uint8_t color)
  //*This function is used to clear the screen
  //*color: BLACK or WHITE or RED
//drawPoint(const unsigned char x, const unsigned char y, const unsigned char color)
  //*This function is used to draw points
  //*(x,y):  coordinate (x∈(0,211),y∈(0,103)
  //*color:  BLACK or WHITE or RED
//drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color)
  //*This function is used to draw lines
  //*[(x1,y1),(x2,y2)]:  The endpoint coordinates (x1/x2∈(0,211),y1/y2∈(0,103))
  //*color:              BLACK or WHITE or RED
//drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color)
  //*This function is used to draw a rectangular border
  //*(x1,x2):  The starting coordinate of the horizontal coordinate (x1/x2∈(0,211))
  //*(y1,y2):  The starting coordinate of the vertical coordinate (y1/y2∈(0,103))
  //*color:    BLACK or WHITE or RED
//rectangleFILL(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);
  //*This function is used to populate the rectangle
  //*(x1,x2):  The starting coordinate of the horizontal coordinate (x1∈(0,211),y1∈(0,103)
  //*(x1,x2):  The starting coordinate of the horizontal coordinate (x1∈(0,211),y1∈(0,103)
  //*(x1,x2):  The starting coordinate of the horizontal coordinate (x1∈(0,211),y1∈(0,103)
  //*(y1,y2):  The starting coordinate of the vertical coordinate (x2∈(0,211),y2∈(0,103)
  //*color:    BLACK or WHITE or RED
//drawCircle(uint16_t xc, uint16_t yc, uint16_t r, uint16_t fill, uint8_t color);
  //*This function is used to draw circles
  //*(xc,yc):  Center coordinates (xc∈(0,211),yc∈(0,103)
  //*r:        radii(r<=52)
  //*fill:     0:Hollow   1:solid
//flush()
  //*This function is used to refresh the screen display
  
DFRobot_EINKBWR_IL0376F eink_IL0376F;

#define EINK_CS  D3
#define Font_CS  D6
#define EINK_DC  D8
#define BUSY     D7

void setup(void)
{
    Serial.begin(115200);
    //Select the corresponding pins
    eink_IL0376F.begin(EINK_CS, Font_CS, EINK_DC, BUSY);
}

void loop(void)
{
    //Clear the screen and display white
    eink_IL0376F.clear(WHITE);
    //Let me draw a red dot
    for(uint8_t x=12,y=12; y<92; y+=2)
    {
        eink_IL0376F.drawPoint(x,y,RED);
    }
    //Draw two lines
    eink_IL0376F.drawLine(24,12,36,92,RED);
    eink_IL0376F.drawLine(36,12,24,92,RED);
    //Draw a red rectangle
    eink_IL0376F.drawRectangle(48,12,98,92,RED);
    //Fill a rectangle with black
    eink_IL0376F.rectangleFill(55,19,91,85,BLACK);
    //Draw a hollow circle
    eink_IL0376F.drawCircle(160,51,40,0,RED);
    //Draw a solid circle
    eink_IL0376F.drawCircle(160,51,30,1,BLACK);
    //Refresh screen display
    eink_IL0376F.flush();
    delay(8000);
    //Clear the screen and display white
}


