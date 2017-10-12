#include <Arduino.h>
#include <Wire.h>
#include <SPI.h> 
#include <String.h>
#include <math.h>

#ifndef __DFRobot_EINKBWR_IL0376F_H
#define __DFRobot_EINKBWR_IL0376F_H

//speed
#define DFR_W21_SPI_SPEED   0x02
//Pin_operation
#define DFR_GT30_CS_0         digitalWrite(DFR_GT30_CS,LOW)
#define DFR_GT30_CS_1         digitalWrite(DFR_GT30_CS,HIGH)
#define DFR_W21_CS_0          digitalWrite(DFR_W21_CS,LOW)
#define DFR_W21_CS_1          digitalWrite(DFR_W21_CS,HIGH)
#define DFR_W21_DC_0          digitalWrite(DFR_W21_DC,LOW)
#define DFR_W21_DC_1          digitalWrite(DFR_W21_DC,HIGH)
//base_address
#define UNICODETOGB2312_ADDR  ((uint32_t)0x267B06)//Gb2312 coding base address
#define ASC0808D2HZ_ADDR      ((uint32_t)0x0066c0)//7*8 ASCII base address
#define ASC0812M2ZF_ADDR      ((uint32_t)0x066d40)//6*12 ASCII base address
#define GBEX0816ZF_ADDR       ((uint32_t)0x27BFA8)//8*16 ASCII base address
#define JFLS1516HZ_ADDR       ((uint32_t)0x21E72C)//16*16 chinese base address
#define JFLS1516HZBD_ADDR     ((uint32_t)0x22242C)//16*16 chinese punctuation base address
//scan_direction
#define EINK_SCAN_DIR1        ((uint8_t)2<<2)//From left to right, top to bottom
#define EINK_SCAN_DIR2        ((uint8_t)0<<2)//From left to right, down to the top
#define EINK_SCAN_DIR3        ((uint8_t)3<<2)//From right to left, top to bottom
#define EINK_SCAN_DIR4        ((uint8_t)1<<2)//From right to left, down to top
//The_font_size
#define CHARACTER_TYPE_8    0//7*8 ASCII
#define CHARACTER_TYPE_12   1//6*12 ASCII
#define CHARACTER_TYPE_16   2//8*16 ASCII
#define CHINESE_TYPE_1616   3//16*16 Chinese  
//The_input_type
#define CHARACTER_TYPE_UNICODE 0
#define CHARACTER_TYPE_UTF8    1
#define CHARACTER_TYPE_GB2312  2
//color
#define LUCENCY  0
#define WHITE    1
#define BLACK    2
#define RED      3

typedef bool InkScreen_Error;

//eINK important parameters 
typedef struct{
    uint16_t highly;
    uint16_t width;
    uint8_t scandir;
}eInk_dev;
extern eInk_dev eInkdev;

class DFRobot_EINKBWR_IL0376F
{
public:
    DFRobot_EINKBWR_IL0376F();
    ~DFRobot_EINKBWR_IL0376F();
    void begin(const char cs_W21, const char cs_GT30, const char dc, const char busy);
    void picDisplay(const unsigned char *pic_bw, const unsigned char *pic_red);
    void disRefresh();
    void disStr(uint8_t x, uint8_t y, char *ch, uint8_t color);
    uint8_t character_type = CHARACTER_TYPE_UTF8;
    InkScreen_Error clear(uint8_t color);
    InkScreen_Error drawPoint(const unsigned char x, const unsigned char y, const unsigned char color);
    InkScreen_Error drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);
    InkScreen_Error drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);
    InkScreen_Error drawCircle(uint16_t xc, uint16_t yc, uint16_t r, uint16_t fill, uint8_t color);
    InkScreen_Error rectangleFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);
private:
    void spiDelay(unsigned char xsp);
    void spiRead(uint32_t addr, uint8_t len, char *ch);
    void spiWriteByte(unsigned char value);
    void wirteCmd(unsigned char command);
    void wirteData(unsigned char data);
    void setWindow(uint16_t x, uint16_t y);
    void powerOn(void);
    void powerOff(void);
    void stateScan(void);
    uint32_t GB2312_addr(char *ch, uint8_t type);
    InkScreen_Error showStr(uint8_t x, uint8_t y, uint8_t size, char *ch, uint8_t color);
    InkScreen_Error getLattice(char *gb2312, uint8_t size, char *ch2);
    InkScreen_Error unicodeToGB2312(char *unicode, char *GB2312);
    InkScreen_Error drawCirclePoint( uint16_t xc, uint16_t yc, uint16_t x, uint16_t y,  uint8_t color);
    uint8_t DFR_W21_CS;
    uint8_t DFR_W21_DC;
    uint8_t DFR_W21_BUSY;
    uint8_t DFR_GT30_CS;
};

#endif
