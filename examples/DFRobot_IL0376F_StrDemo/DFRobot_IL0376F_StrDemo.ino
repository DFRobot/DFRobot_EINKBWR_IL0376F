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
//disStr(uint8_t x, uint8_t y, char *ch, uint8_t color)
  //*This function is used to display Chinese and English strings
  //*(x,y):  coordinate (x∈(0,211),y∈(0,103)
  //*ch:     The string to display
  //*color:  BLACK or WHITE or RED
//disRefresh()
  //*This function is used to refresh the screen display
  
DFRobot_EINKBWR_IL0376F eink_IL0376F;

#define EINK_CS  D3
#define Font_CS  D6
#define EINK_DC  D8
#define BUSY     D7

void setup(void){
    Serial.begin(115200);
    //Select the corresponding pins
    eink_IL0376F.begin(EINK_CS, Font_CS, EINK_DC, BUSY);
}

void loop(void){
    //Clear the screen and display white
    eink_IL0376F.clear(WHITE);
    //Displays a string, red font
    eink_IL0376F.disStr(12,12,"DFRobot EINK 1234567890,~!@#$%^&*()-+=",RED);
    //Refresh screen display
    eink_IL0376F.disRefresh();
    delay(3000);

    //Clear the screen and display white
    eink_IL0376F.clear(WHITE);
    //Displays a string, red font
    eink_IL0376F.disStr(12,12,"DFRobot三色电子墨水屏测试程序",RED);
    //Refresh screen display
    eink_IL0376F.disRefresh();
    delay(3000);
}


