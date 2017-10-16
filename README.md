#  FireBeetle Covers-ePaper Display Module Driver
The display is a TFT active matrix electrophoretic display, with interface and a reference system design. The
2.13” active area contains 212×104 pixels, and has 1-bit black/white and highlight red full display capabilities.
An integrated circuit contains gate buffer, source buffer, interface, timing control logic, oscillator, DC-DC,
SRAM, LUT, VCOM and border are supplied with each panel.

![SVG1](https://raw.githubusercontent.com/DFRobot/binaryfiles/master/DFR0511/DFR0511svg1.png)

## DFRobot_EINKBWR_IL0376F Library for Arduino
---------------------------------------------------------

Provide an Arduino library to control the e-ink screen display, via SPI communication.

## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary

Provide an Arduino library to control the e-ink screen display, via SPI communication.

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder

## Methods

```C++	

#include <DFRobot_EINK.h>

/*
 * @brief Select the corresponding pins
 * @param cs_W21  EINK Piece of selected pin
 *     D3: Piece of selected pin
 *     D4: Piece of selected pin
 * @param cs_GT30  FONT Piece of selected pin
 *     D5: Piece of selected pin
 *     D6: Piece of selected pin
 * @param dc  DC Control pin
 *     D8: Piece of selected pin
 * @param busy  Chip status pins
 */
void begin(const char cs_W21, const char cs_GT30, const char dc, busy);

/*
 * @brief Clear the screen(BLACK or WHITE or RED)
 * @param color  The brush color
 *     BLACK: Black brush
 *     WHITE: White brush
 *     RED: The red brush
 */
InkScreen_Error clear(uint8_t color);

/*
 * @brief Image display (212 * 104)
 * @param pic_bw  Black and white part
 * @param pic_red  The red part
 */
void picDisplay(const unsigned char *pic_bw, const unsigned char *pic_red);

/*
 * @brief The refresh screen
 */
void flush(void);

/*
 * @brief Display string
 * @param (x,y)  coordinate (x∈(0,211),y∈(0,103)
 * @param *ch  The string to display
 * @param color  BLACK or WHITE or RED
 */
void disStr(uint8_t x, uint8_t y, char *ch, uint8_t color);

/*
 * @brief Create Point
 * @param (x,y)  coordinate (x∈(0,211),y∈(0,103)
 * @param color  BLACK or WHITE or RED
 */
InkScreen_Error drawPoint(const unsigned char x, const unsigned char y, const unsigned char color);

/*
 * @brief Draw a straight line
 * @param [(x1,y1),(x2,y2)]  The endpoint coordinates (x1/x2∈(0,211),y1/y2∈(0,103))
 * @param color  BLACK or WHITE or RED
 */
InkScreen_Error drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);

/*
 * @brief Draw a rectangular
 * @param (x1,x2)  The starting coordinate of the horizontal coordinate (x1/x2∈(0,211))
 * @param (y1,y2)  The starting coordinate of the vertical coordinate (y1/y2∈(0,103))
 * @param (y1,y2)  The starting coordinate of the vertical coordinate (y1/y2∈(0,103))
 * @param color  BLACK or WHITE or RED
 */
InkScreen_Error drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);

/*
 * @brief Rectangle filling
 * @param (x1,x2)  The starting coordinate of the horizontal coordinate (x1/x2∈(0,211))
 * @param (y1,y2)  The starting coordinate of the vertical coordinate (y1/y2∈(0,103))
 * @param (y1,y2)  The starting coordinate of the vertical coordinate (y1/y2∈(0,103))
 * @param color  BLACK or WHITE or RED
 */
InkScreen_Error rectangleFILL(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);

/*
 * @brief Specify the center and radius to draw the circle
 * @param (xc,yc)  Center coordinates (xc∈(0,211),yc∈(0,103)
 * @param r  radii(r<=52)
 * @param fill  0:One pixel width  1:filling
 * @param color  BLACK or WHITE or RED
 */
InkScreen_Error drawCircle(uint16_t xc, uint16_t yc, uint16_t r, uint16_t fill, uint8_t color);

```

## Compatibility

MCU                | Work Well | Work Wrong | Untested  | Remarks
------------------ | :----------: | :----------: | :---------: | -----
FireBeetle-ESP32  |      √       |             |            | 
FireBeetle-ESP8266  |      √       |             |            | 
FireBeetle-BLE4.1 |              |      √      |            | 
Arduino uno |              |      √      |            | 
Arduino leonardo |              |      √      |            | 

## History

- Sep 29, 2017 - Version 0.2 released.

## Credits

Written by lixin(1035868977@qq.com), 2017. (Welcome to our [website](https://www.dfrobot.com/))
