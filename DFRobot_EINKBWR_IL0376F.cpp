#include"DFRobot_EINKBWR_IL0376F.h"

eInk_dev eInkdev;
unsigned char  *DF_Display_bw = new unsigned char[2756];
unsigned char  *DF_Display_red = new unsigned char[2756];

DFRobot_EINKBWR_IL0376F::DFRobot_EINKBWR_IL0376F():
    DFR_W21_CS(2), DFR_W21_DC(12), DFR_GT30_CS(4), character_type(CHARACTER_TYPE_UTF8)
{}

DFRobot_EINKBWR_IL0376F::~DFRobot_EINKBWR_IL0376F()
{}

void DFRobot_EINKBWR_IL0376F::begin(const char cs_W21, const char cs_GT30, const char dc, const char busy)
{
    SPI.begin();
    DFR_W21_CS =cs_W21; DFR_W21_DC =dc;  DFR_GT30_CS = cs_GT30;  DFR_W21_BUSY = busy;
    pinMode(DFR_W21_CS, OUTPUT); 
    pinMode(DFR_W21_DC, OUTPUT);
    pinMode(DFR_GT30_CS, OUTPUT);
    pinMode(DFR_W21_BUSY, INPUT);
    eInkdev.highly = 104;//Set screen height
    eInkdev.width = 212;//Set Screen width
    eInkdev.scandir = EINK_SCAN_DIR3;//Set the default scanning direction
}

void DFRobot_EINKBWR_IL0376F::spiDelay(unsigned char xsp){
    unsigned char i;
    while(xsp--)
    {
        for(i=0;i<DFR_W21_SPI_SPEED;i++);
    }
}

void DFRobot_EINKBWR_IL0376F::spiRead(uint32_t addr, uint8_t len, char *ch)
{
    DFR_GT30_CS_0;
    DFR_W21_CS_1;
    SPI.transfer(0x0b);
    SPI.transfer(addr >> 16);
    SPI.transfer(addr >> 8);
    SPI.transfer(addr);
    SPI.transfer(0x00);
    while(len --) {
        *ch = SPI.transfer(0x00);
        ch ++;
    }
    DFR_GT30_CS_1;
}

void DFRobot_EINKBWR_IL0376F::spiWriteByte(unsigned char value){
    SPI.transfer(value);
}

void DFRobot_EINKBWR_IL0376F::wirteCmd(unsigned char command){
    spiDelay(1);
    DFR_W21_CS_0;
    DFR_GT30_CS_1;
    DFR_W21_DC_0;
    SPI.setBitOrder(MSBFIRST);
    spiWriteByte(command);
    DFR_W21_CS_1;
}

void DFRobot_EINKBWR_IL0376F::wirteData(unsigned char data){
    spiDelay(1);
    DFR_W21_CS_0;
    DFR_GT30_CS_1;
    DFR_W21_DC_1;
    //If the scan method is 2 or 4, send a low post, then send a high post
    if(eInkdev.scandir==EINK_SCAN_DIR2 || eInkdev.scandir==EINK_SCAN_DIR4){
        SPI.setBitOrder(LSBFIRST);
    }else{//If the scan is 1 or 3, send the high first and post the low post
        SPI.setBitOrder(MSBFIRST);
    }
    spiWriteByte(data);
    DFR_W21_CS_1;
}

void DFRobot_EINKBWR_IL0376F::setWindow(uint16_t x, uint16_t y){
    uint8_t hres,vres_h,vres_l;
    hres = y/8;
    hres <<= 3; 
    vres_h = x>>8;
    vres_l = x&0xff;
    wirteCmd(0x61);
    wirteData(hres);
    wirteData(vres_h);
    wirteData(vres_l);
    eInkdev.highly = y;
    eInkdev.width = x;
}

void DFRobot_EINKBWR_IL0376F::stateScan(void)
{
    while(1){
        if(digitalRead(DFR_W21_BUSY)==1){
            break;
        }
	}
}

void DFRobot_EINKBWR_IL0376F::powerOn(void)
{
    wirteCmd(0x06);//Set the boost
    wirteData(0x17);
    wirteData(0x17);
    wirteData(0x17);
    wirteCmd(0x04);//Access to electricity
    stateScan();
    wirteCmd(0X00);//Set sweeping direction
    wirteData(0xc3|eInkdev.scandir);
    wirteCmd(0X50);
    wirteData(0x37);
    wirteCmd(0x30);//Set the PLL
    wirteData(0x39);
    setWindow(212,104);//Pixel setting 212*104
    wirteCmd(0x82);//Vcom setting
    wirteData (0x0a);
}

void DFRobot_EINKBWR_IL0376F::powerOff(void)
{
    stateScan();
    wirteCmd(0x12);
    delay(12000);
    wirteCmd(0x82);//to solve Vcom drop
    wirteData(0x00);
    wirteCmd(0x01);//power setting
    wirteData(0x02);
    wirteData(0x00);
    wirteData(0x00);
    wirteData(0x00);
    wirteCmd(0X02);//Power off
    delay(3000);
}


void DFRobot_EINKBWR_IL0376F::drawPicture(const unsigned char *pic_bw = NULL, const unsigned char *pic_red = NULL)
{
    //The image data to be displayed is stored in the cache
    if(pic_bw == NULL){
        for(int i  =0; i < 2756; i++)
            DF_Display_bw[i] = 0;
    }else{
        for(int i  =0; i < 2756; i++)
            DF_Display_bw[i] = pic_bw[i];
    }
    if(pic_red == NULL){
        for(int i  =0; i < 2756; i++)
            DF_Display_red[i] = 0;
    }else{
        for(int i  =0; i < 2756; i++)
            DF_Display_red[i] = pic_red[i];
    }
}

void DFRobot_EINKBWR_IL0376F::flush()
{
    if(eInkdev.scandir == EINK_SCAN_DIR3){
        powerOn();
        wirteCmd(0x10);
        for(int i = 0; i < 2756; i++) wirteData(~DF_Display_bw[i]);
        wirteCmd(0x11);
        wirteCmd(0x13);
        for(int i = 0; i < 2756; i++) wirteData(~DF_Display_red[i]);
        wirteCmd(0x11);
        powerOff();
    }else if(eInkdev.scandir == EINK_SCAN_DIR1){
        int16_t y=2743;
        powerOn();
        wirteCmd(0x10);
        while(y>=0){
            for(int i = 0; i < 13; i++) wirteData(~DF_Display_bw[y+i]);
            y-=13;
        }
        y=2743;
        wirteCmd(0x11);
        wirteCmd(0x13);
        while(y>=0){
            for(int i = 0; i < 13; i++) wirteData(~DF_Display_red[y+i]);
            y-=13;
        }
        wirteCmd(0x11);
        powerOff();
    }else if(eInkdev.scandir == EINK_SCAN_DIR2){
        int16_t y=2755;
        powerOn();
        wirteCmd(0x10);
        while(y>=0){
            for(int i = 0; i < 13; i++) wirteData(~DF_Display_bw[y-i]);
            y-=13;
        }
        y=2755;
        wirteCmd(0x11);
        wirteCmd(0x13);
        while(y>=0){
            for(int i = 0; i < 13; i++) wirteData(~DF_Display_red[y-i]);
            y-=13;
        }
        wirteCmd(0x11);
        powerOff();
    }else if(eInkdev.scandir == EINK_SCAN_DIR4){
        int16_t y=12;
        powerOn();
        wirteCmd(0x10);
        while(y<=2755){
            for(int i = 0; i < 13; i++) wirteData(~DF_Display_bw[y-i]);
            y+=13;
        }
        y=2755;
        wirteCmd(0x11);
        wirteCmd(0x13);
        while(y<=2755){
            for(int i = 0; i < 13; i++) wirteData(~DF_Display_red[y-i]);
            y+=13;
        }
        wirteCmd(0x11);
        powerOff();
    }
}

InkScreen_Error DFRobot_EINKBWR_IL0376F::clear(uint8_t color)
{
    uint8_t bw,red;
    if(color == WHITE){
        bw = 0x00; red = 0x00;
    }else if(color == RED){
        bw = 0x00; red = 0xff;
    }else if(color == BLACK){
        bw = 0xff; red = 0x00;
    }else{
        Serial.println("no color!");
        return false;
    }
    for(int i = 0; i < 2756; i++){
        DF_Display_bw[i]=bw;
        DF_Display_red[i]=red;
    }
    //flush();
    return true;
}

InkScreen_Error DFRobot_EINKBWR_IL0376F::drawPoint(const unsigned char x, const unsigned char y, const unsigned char color)
{
    uint8_t sx,sy;
    uint16_t sby;
    if(x>211 || y>103){
        Serial.println("Out of display!");
        return false;
    }
    sx = 211-x;      sby = sx*13;
    sy = (y+1)/8;    sby += sy;
    sy = (y+1)%8;
    if(color == RED){//Red dot
        if(sy != 0){
            DF_Display_bw[sby] &= (~(int)pow(2,8-sy)); 
            DF_Display_red[sby] |= ((int)pow(2,8-sy)); 
        }else{
            DF_Display_bw[sby-1] &= 0xFE;
            DF_Display_red[sby-1] |= 0x01;
        }
        return true;
    }else if(color == BLACK){//Black spots
        if(sy != 0){
            DF_Display_red[sby] &= (~(int)pow(2,8-sy)); 
            DF_Display_bw[sby] |= ((int)pow(2,8-sy)); 
        }else{
            DF_Display_red[sby-1] &= 0xFE;
            DF_Display_bw[sby-1] |= 0x01;
        }
        return true;
    }else if(color == WHITE){//White dots
        if(sy != 0){
            DF_Display_bw[sby] &= (~(int)pow(2,8-sy)); 
            DF_Display_red[sby] &= (~(int)pow(2,8-sy)); 
        }else{
            DF_Display_bw[sby-1] &= 0xFE;
            DF_Display_red[sby-1] &= 0xFE;
        }
        return true;
    }else if(color == LUCENCY){
        return true;
    }
    else return false;
}

InkScreen_Error DFRobot_EINKBWR_IL0376F::drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color)
{
    InkScreen_Error Status = true;
    uint16_t t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    delta_x=x2-x1;
    delta_y=y2-y1;
    uRow=x1;
    uCol=y1;
    if(delta_x>0){
        incx=1;
    }else if(delta_x==0){
        incx=0;
    }else{
        incx=-1;delta_x=-delta_x;
    }
    if(delta_y>0){
        incy=1;
    }else if(delta_y==0){
        incy=0;
    }else{
        incy=-1;delta_y=-delta_y;
    }
    if( delta_x>delta_y){
        distance=delta_x;
    }else{
        distance=delta_y;
    }
    for(t = 0; t <= distance+1; t++ ){
        Status = drawPoint(uRow, uCol, color);
        if(!Status) return Status;
        xerr+=delta_x ;
        yerr+=delta_y ;
        if(xerr>distance){
            xerr-=distance;
            uRow+=incx;
        }
        if(yerr>distance){
            yerr-=distance;
            uCol+=incy;
        }
    }
    return Status;
}

InkScreen_Error DFRobot_EINKBWR_IL0376F::drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color)
{
    InkScreen_Error Status = true;
    Status = drawLine(x1,y1,x2,y1,color);
    if(!Status) return Status;
    Status = drawLine(x1,y1,x1,y2,color);
    if(!Status) return Status;
    Status = drawLine(x1,y2,x2,y2,color);
    if(!Status) return Status;
    Status = drawLine(x2,y1,x2,y2,color);
    if(!Status) return Status;
    return Status;
}

InkScreen_Error DFRobot_EINKBWR_IL0376F::drawFillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color)
{
    InkScreen_Error Status = true;
    uint8_t sx=x1,sy=y1;
    for(sy = y1; sy <= y2; sy++){
        for(sx = x1; sx <= x2; sx++){
            Status=drawPoint(sx, sy, color);
            if(!Status) return Status;
        }
    }
    return Status;
}

InkScreen_Error DFRobot_EINKBWR_IL0376F::drawCirclePoint( uint16_t xc, uint16_t yc, uint16_t x, uint16_t y, uint8_t color)
{
    InkScreen_Error Status = true;
    uint8_t Ix=1,Iy=1;
    for(uint8_t i = 0; i < 8; i++){
        if(i<4){
            Status = drawPoint(xc + x*Ix, yc + y*Iy, color);
            if(!Status) return Status;
        }else{
            Status = drawPoint(xc + y*Ix, yc + x*Iy, color);
            if(!Status) return Status;
        }
        Ix *= -1;
        if((i+1)%2==0){
            Iy *= -1;
        }
    }
    return Status;
}

InkScreen_Error DFRobot_EINKBWR_IL0376F::drawCircle(uint16_t xc, uint16_t yc, uint16_t r, uint16_t fill, uint8_t color)
{
    InkScreen_Error Status = true;
    int x=0, y=r, yi,d;
    d = 3-2*r;
    if(xc-r<0 || xc+r >= eInkdev.width || yc-r < 0 || yc+r >= eInkdev.highly) return false;
    while(x<=y){
        if(fill){
            for(yi = x; yi <= y; yi++)
                Status = drawCirclePoint(xc, yc, x, yi, color);
        }else{
            Status = drawCirclePoint(xc, yc, x, y, color);
        }
        if(!Status) return Status;
        if(d<0) d = d+4*x+6;
        else {d = d+4*(x-y); y--;}
        x++;
    }
    return Status;
}

InkScreen_Error DFRobot_EINKBWR_IL0376F::unicodeToGB2312(char *unicode, char *GB2312) 
{
    uint32_t addr;
    uint8_t result=0;
    uint32_t h;
    uint32_t code;
    code = *unicode++;
    code = (code<<8) + *unicode;
    if(code<0xa0) result=1;
    else if(code<=0xf7) h=code-160  -1281;
    else if(code<0x2c7) result=1;
    else if(code<=0x2c9) h=code-160-463  -1281;
    else if(code<0x2010) result=1;
    else if(code<=0x2312) h=code-160-463-7494  -1281;
    else if(code<0x2460) result=1;
    else if(code<=0x2642) h=code-160-463-7494-333  -948;
    else if(code<0x3000) result=1;
    else if(code<=0x3017) h=code-160-463-7494-333-2493  -948;
    else if(code<0x3220) result=1;
    else if(code<=0x3229) h=code-160-463-7494-333-2493-520  -428;
    else if(code<0x4e00) result=1;
    else if(code<=0x9b54) h=code-160-463-7494-333-2493-520-7126;
    else if(code<0x9c7c) result=1;
    else if(code<=0x9ce2) h=code-160-463-7494-333-2493-520-7126-295;
    else if(code<0x9e1f) result=1;
    else if(code<=0x9fa0) h=code-160-463-7494-333-2493-520-7126-295-316;
    else if(code<0xe76c) result=1;
    else if(code == 0xe76c) h=code-160-463-7494-333-2493-520-7126-295-316-18379  -22699;
    else if(code<=0xe774) h=code-160-463-7494-333-2493-520-7126-295-316-18379;
    else if(code<0xff00) result=1;
    else if(code<=0xff5f) h=code-160-463-7494-333-2493-520-7126-295-316-18379-6027  +666;
    else if(code<0xffe0) result=1;
    else if(code<0xffe5) h=code-160-463-7494-333-2493-520-7126-295-316-18379-6027-128  +666;
    else if(code==0xffe5) h=code-160-463-7494-333-2493-520-7126-295-316-18379-6027-128  +812631;
    else result=1;
    if(result==0){
        addr = UNICODETOGB2312_ADDR + (h<<1);// +AAA;
        //Get the gb2312 encoding from the address
        spiRead(addr, 2, GB2312);
    } else {
        GB2312[0] = 0xa1;
        GB2312[1] = 0xa1;
    }
    return true;
}

uint32_t DFRobot_EINKBWR_IL0376F::GB2312_addr(char *ch, uint8_t type)
{
    uint32_t temp = 0;
    if(ch[0]<0x80){
        if( ch[0] >= ' ' ){
            temp = ch[0] - ' ';
        }
        if( type == CHARACTER_TYPE_8 ){//7*8 ascii code
            temp = temp*8  + ASC0808D2HZ_ADDR;
        }else if( type == CHARACTER_TYPE_12 ){//6*12 ascii code
            temp = temp*12 + ASC0812M2ZF_ADDR;
        }else if( type == CHARACTER_TYPE_16 ){//8*16 ascii code
            temp = temp*16 + GBEX0816ZF_ADDR;
        }
    }else{
        if(ch[0] >=0xA4 && ch[0] <= 0Xa8 && ch[1] >=0xA1){
            temp = JFLS1516HZ_ADDR;
        }else if(ch[0] >=0xA1 && ch[0] <= 0Xa9 && ch[1] >=0xA1){
            temp =( (ch[0] - 0xA1) * 94 + (ch[1] - 0xA1))*32+ JFLS1516HZBD_ADDR;//Punctuation section
        }else if(ch[0] >=0xB0 && ch[0] <= 0xF7 && ch[1] >=0xA1){
            temp = ((ch[0] - 0xB0) * 94 + (ch[1] - 0xA1)+ 846)*32+ JFLS1516HZ_ADDR;
        }
    }
    return temp;
}

void DFRobot_EINKBWR_IL0376F::disString(uint8_t x, uint8_t y, char *ch, uint8_t color) 
{
    char unicode_hz[2] = {0};
    char gb2312_hz [2] = {0};
    char temp_char[2] = {0};
    char str_null[32] = {0};
    int temp = 0;
    //The input character is in utf-8 format
    if(character_type == CHARACTER_TYPE_UTF8 || character_type == CHARACTER_TYPE_GB2312) {
        while(*ch) {
            if((*ch & 0xf0) == 0xe0) {//Chinese
                temp = (*ch & 0x0f) << 12 | (*(ch + 1) & 0x3f) << 6 | (*(ch + 2) & 0x3f);
                unicode_hz[0] = temp >> 8;
                unicode_hz[1] = temp;
                unicodeToGB2312(unicode_hz, gb2312_hz);
                getLattice(gb2312_hz, CHINESE_TYPE_1616, str_null);//Getting font
                showStr(x, y, CHINESE_TYPE_1616, str_null, color);//Refresh the cache
                x+=16;
                if(x+16>211){
                    x=0; y += 18;
                    if(y+16 > 103) break;
                }
                ch += 3;
            }else if((*ch & 0x80) == 0x80){
                gb2312_hz[0] = *ch;
                gb2312_hz[1] = *(ch+1);
                //unicodeToGB2312(unicode_hz, gb2312_hz);
                getLattice(gb2312_hz, CHINESE_TYPE_1616, str_null);//Getting font
                showStr(x, y, CHINESE_TYPE_1616, str_null, color);//Refresh the cache
                x+=16;
                if(x+16>211){
                    x=0; y += 18;
                    if(y+16 > 103) break;
                }
                ch += 2;
            }else{//ASCII
                temp_char[0] = *ch;
                getLattice(temp_char, CHARACTER_TYPE_16, str_null);//Getting font
                showStr(x, y, CHARACTER_TYPE_16, str_null, color);//Refresh the cache
                x+=8;
                if(x+8>211){
                    x=0; y += 18;
                    if(y+16 > 103) break;
                }
                ch++;
            }
        }
    }else if(character_type == CHARACTER_TYPE_UNICODE){
        //The input character is in unicode format
        Serial.println(strlen(ch));
    }
}

InkScreen_Error DFRobot_EINKBWR_IL0376F::getLattice(char *gb2312, uint8_t size, char *ch2)
{
    if(size == CHINESE_TYPE_1616){//Get 16 * 16 Chinese font
        uint32_t addr = GB2312_addr(gb2312, CHINESE_TYPE_1616);
        spiRead(addr, 32, ch2);
    }else if(size == CHARACTER_TYPE_16){//Gets 8 * 16ASCII character font
        uint32_t addr = GB2312_addr(gb2312, CHARACTER_TYPE_16);
        spiRead(addr, 16, ch2);
    }
}

InkScreen_Error DFRobot_EINKBWR_IL0376F::showStr(uint8_t x, uint8_t y, uint8_t size, char *ch, uint8_t color)
{
    uint8_t sx=x,font;
    uint16_t MAXfont, width;
    //16*16 Chinese display cache refresh
    if(size == CHINESE_TYPE_1616){
        MAXfont = 32; width = 16;
    }else if(size == CHARACTER_TYPE_16){
        //8*16 ASCII display cache refresh
        MAXfont = 16; width = 8;
    }
    for(int j = 0; j < MAXfont; j++){
        font=(uint8_t) ch[j];
        for(int i = 0; i < 8; i++){
            if(font&0x80) drawPoint(sx,y,color);
            else drawPoint(sx,y,LUCENCY);
            font<<=1; sx++;
            if((sx-x)==width){
                sx=x; y++;
            }
        }
    }
}

// !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxwz{|}~
//Pixel cache
//     2743            0
//		|\   |\   |\   |
//		| \  | \  | \  |
//		|  \ |  \ |  \ |
//		|   \|   \|   \|
//     2755            12

