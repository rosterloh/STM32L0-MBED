#include "EaEpaper.h"

#define EA_IMG_BUF_SZ (387) // 172 x 18 / 8 = 387

static uint8_t _oldImage[EA_IMG_BUF_SZ];
static uint8_t _newImage[EA_IMG_BUF_SZ];

EaEpaper::EaEpaper(PinName p_on, PinName reset, PinName busy, PinName cs,
                   PinName mosi, PinName miso, PinName sck,
                   const char* name):
    epd_(EPD_2_7,
         p_on,  // panel_on_pin
         reset, // reset_pin
         busy, // busy_pin
         cs,  // chip_select_pin
         mosi,  // mosi
         miso,  // miso
         sck),  // sck
    GraphicsDisplay(name)
{
    draw_mode = NORMAL;
    memset(_oldImage, 0, EA_IMG_BUF_SZ);
    memset(_newImage, 0, EA_IMG_BUF_SZ);
}


int EaEpaper::width()
{
    return 172;
}

int EaEpaper::height()
{
    return 18;
}


// erase pixel after power up
void EaEpaper::clear()
{
    epd_.begin();
    epd_.setFactor(readTemperature()/100);
    epd_.clear();
    epd_.end();

    memset(_oldImage, 0, EA_IMG_BUF_SZ);
    memset(_newImage, 0, EA_IMG_BUF_SZ);
}

// update screen  _newImage -> _oldImage
void EaEpaper::write_disp(void)
{
    epd_.begin();
    epd_.setFactor(readTemperature()/100);
    epd_.image(_oldImage, _newImage);
    epd_.end();

    memcpy(_oldImage, _newImage, EA_IMG_BUF_SZ);
}

// read LM75A sensor on board to calculate display speed
int32_t EaEpaper::readTemperature()
{
    char buf[2];
    int32_t t = 0;
    char reg = LM75A_CMD_TEMP;

    i2c_.write(LM75A_ADDRESS, &reg, 1);
    i2c_.read(LM75A_ADDRESS, buf, 2);

    t = ((buf[0] << 8) | (buf[1]));

    return ((t * 100) >> 8);
}

// set one pixel in buffer _newImage

void EaEpaper::pixel(int x, int y, int color)
{
    // first check parameter
    if(x > 263 || y > 175 || x < 0 || y < 0) return;

    if(draw_mode == NORMAL) {
        if(color == 0)
            _newImage[(x / 8) + ((y-1) * 33)] &= ~(1 << (x%8));  // erase pixel
        else
            _newImage[(x / 8) + ((y-1) * 33)] |= (1 << (x%8));   // set pixel
    } else { // XOR mode
        if(color == 1)
            _newImage[(x / 8) + ((y-1) * 33)] ^= (1 << (x%8));   // xor pixel
    }
}

// clear screen
void EaEpaper::cls(void)
{
    memset(_newImage, 0, EA_IMG_BUF_SZ);  // clear display buffer
}

// print line
void EaEpaper::line(int x0, int y0, int x1, int y1, int color)
{
    int   dx = 0, dy = 0;
    int   dx_sym = 0, dy_sym = 0;
    int   dx_x2 = 0, dy_x2 = 0;
    int   di = 0;

    dx = x1-x0;
    dy = y1-y0;

    if (dx > 0) {
        dx_sym = 1;
    } else {
        dx_sym = -1;
    }

    if (dy > 0) {
        dy_sym = 1;
    } else {
        dy_sym = -1;
    }

    dx = dx_sym*dx;
    dy = dy_sym*dy;

    dx_x2 = dx*2;
    dy_x2 = dy*2;

    if (dx >= dy) {
        di = dy_x2 - dx;
        while (x0 != x1) {

            pixel(x0, y0, color);
            x0 += dx_sym;
            if (di<0) {
                di += dy_x2;
            } else {
                di += dy_x2 - dx_x2;
                y0 += dy_sym;
            }
        }
        pixel(x0, y0, color);
    } else {
        di = dx_x2 - dy;
        while (y0 != y1) {
            pixel(x0, y0, color);
            y0 += dy_sym;
            if (di < 0) {
                di += dx_x2;
            } else {
                di += dx_x2 - dy_x2;
                x0 += dx_sym;
            }
        }
        pixel(x0, y0, color);
    }
}

// print rect
void EaEpaper::rect(int x0, int y0, int x1, int y1, int color)
{

    if (x1 > x0) line(x0,y0,x1,y0,color);
    else  line(x1,y0,x0,y0,color);

    if (y1 > y0) line(x0,y0,x0,y1,color);
    else line(x0,y1,x0,y0,color);

    if (x1 > x0) line(x0,y1,x1,y1,color);
    else  line(x1,y1,x0,y1,color);

    if (y1 > y0) line(x1,y0,x1,y1,color);
    else line(x1,y1,x1,y0,color);
}

// print filled rect
void EaEpaper::fillrect(int x0, int y0, int x1, int y1, int color)
{
    int l,c,i;
    if(x0 > x1) {
        i = x0;
        x0 = x1;
        x1 = i;
    }

    if(y0 > y1) {
        i = y0;
        y0 = y1;
        y1 = i;
    }

    for(l = x0; l<= x1; l ++) {
        for(c = y0; c<= y1; c++) {
            pixel(l,c,color);
        }
    }
}

// print circle
void EaEpaper::circle(int x0, int y0, int r, int color)
{
    int x = -r, y = 0, err = 2-2*r, e2;
    do {
        pixel(x0-x, y0+y,color);
        pixel(x0+x, y0+y,color);
        pixel(x0+x, y0-y,color);
        pixel(x0-x, y0-y,color);
        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    } while (x <= 0);

}

// print filled circle
void EaEpaper::fillcircle(int x0, int y0, int r, int color)
{
    int x = -r, y = 0, err = 2-2*r, e2;
    do {
        line(x0-x, y0-y, x0-x, y0+y, color);
        line(x0+x, y0-y, x0+x, y0+y, color);
        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    } while (x <= 0);
}

// set drawing mode
void EaEpaper::setmode(int mode)
{
    draw_mode = mode;
}

// set cursor position
void EaEpaper::locate(int x, int y)
{
    char_x = x;
    char_y = y;
}

// calc char columns
int EaEpaper::columns()
{
    return width() / font[1];
}

// calc char rows
int EaEpaper::rows()
{
    return height() / font[2];
}

// print char
int EaEpaper::_putc(int value)
{
    if (value == '\n') {    // new line
        char_x = 0;
        char_y = char_y + font[2];
        if (char_y >= height() - font[2]) {
            char_y = 0;
        }
    } else {
        character(char_x, char_y, value);
    }
    return value;
}

// paint char out of font
void EaEpaper::character(int x, int y, int c)
{
    unsigned int hor,vert,offset,bpl,j,i,b;
    unsigned char* zeichen;
    unsigned char z,w;

    if ((c < 31) || (c > 127)) return;   // test char range

    // read font parameter from start of array
    offset = font[0];                    // bytes / char
    hor = font[1];                       // get hor size of font
    vert = font[2];                      // get vert size of font
    bpl = font[3];                       // bytes per line

    if (char_x + hor > width()) {
        char_x = 0;
        char_y = char_y + vert;
        if (char_y >= height() - font[2]) {
            char_y = 0;
        }
    }

    zeichen = &font[((c -32) * offset) + 4]; // start of char bitmap
    w = zeichen[0];                          // width of actual char
    // construct the char into the buffer
    for (j=0; j<vert; j++) {  //  vert line
        for (i=0; i<hor; i++) {   //  horz line
            z =  zeichen[bpl * i + ((j & 0xF8) >> 3)+1];
            b = 1 << (j & 0x07);
            if (( z & b ) == 0x00) {
                pixel(x+i,y+j,0);
            } else {
                pixel(x+i,y+j,1);
            }

        }
    }

    char_x += w;
}

// set actual font
void EaEpaper::set_font(unsigned char* f)
{
    font = f;
}

void EaEpaper::print_bm(Bitmap bm, int x, int y)
{
    int h,v,b;
    char d;

    for(v=0; v < bm.ySize; v++) {   // lines
        for(h=0; h < bm.xSize; h++) { // pixel
            if(h + x > width()) break;
            if(v + y > height()) break;
            d = bm.data[bm.Byte_in_Line * v + ((h & 0xF8) >> 3)];
            b = 0x80 >> (h & 0x07);
            if((d & b) == 0) {
                pixel(x+h,y+v,0);
            } else {
                pixel(x+h,y+v,1);
            }
        }
    }

}
