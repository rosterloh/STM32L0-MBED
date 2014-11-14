/**
 ******************************************************************************
 * @file    GDE021A1.cpp
 * @author  Richard Osterloh (richard.osterloh@gmail.com)
 * @version V1.0.0
 * @date    31-October-2014
 * @brief   An interface for the GDE021A1 E-paper Display
 ******************************************************************************
 * An interface for the GDE021A1 E-paper Display
 ******************************************************************************
 */

#include "GDE021A1.h"
#include "Font16.h"

#define EPD_COLOR_BLACK         0x00
#define EPD_COLOR_DARKGRAY      0x55
#define EPD_COLOR_LIGHTGRAY     0xAA
#define EPD_COLOR_WHITE         0xFF

/* Look-up table for the epaper (90 bytes) */
const unsigned char WF_LUT[]={
  0x82,0x00,0x00,0x00,0xAA,0x00,0x00,0x00,
  0xAA,0xAA,0x00,0x00,0xAA,0xAA,0xAA,0x00,
  0x55,0xAA,0xAA,0x00,0x55,0x55,0x55,0x55,
  0xAA,0xAA,0xAA,0xAA,0x55,0x55,0x55,0x55,
  0xAA,0xAA,0xAA,0xAA,0x15,0x15,0x15,0x15,
  0x05,0x05,0x05,0x05,0x01,0x01,0x01,0x01,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x41,0x45,0xF1,0xFF,0x5F,0x55,0x01,0x00,
  0x00,0x00,};

/**
  * @brief  GDE021A1 Registers
  */
#define EPD_REG_0             0x00   /* Status Read */
#define EPD_REG_1             0x01   /* Driver Output Control */
#define EPD_REG_3             0x03   /* Gate driving voltage control */
#define EPD_REG_4             0x04   /* Source driving coltage control */
#define EPD_REG_7             0x07   /* Display Control */
#define EPD_REG_11            0x0B   /* Gate and Sorce non overlap period COntrol */
#define EPD_REG_15            0x0F   /* Gate scan start */
#define EPD_REG_16            0x10   /* Deep Sleep mode setting */
#define EPD_REG_17            0x11   /* Data Entry Mode Setting */
#define EPD_REG_18            0x12   /* SWRESET */
#define EPD_REG_26            0x1A   /* Temperature Sensor Control (Write to Temp Register) */
#define EPD_REG_27            0x1B   /* Temperature Sensor Control(Read from Temp Register) */
#define EPD_REG_28            0x1C   /* Temperature Sensor Control(Write Command  to Temp sensor) */
#define EPD_REG_29            0x1D   /* Temperature Sensor Control(Load temperature register with temperature sensor reading) */
#define EPD_REG_32            0x20   /* Master activation */
#define EPD_REG_33            0x21   /* Display update */
#define EPD_REG_34            0x22   /* Display update control 2 */
#define EPD_REG_36            0x24   /* write RAM */
#define EPD_REG_37            0x25   /* Read RAM */
#define EPD_REG_40            0x28   /* VCOM sense */
#define EPD_REG_41            0x29   /* VCOM Sense duration */
#define EPD_REG_42            0x2A   /* VCOM OTP program */
#define EPD_REG_44            0x2C   /* Write VCOMregister */
#define EPD_REG_45            0x2D   /* Read OTP registers */
#define EPD_REG_48            0x30   /* Program WS OTP */
#define EPD_REG_50            0x32   /* Write LUT register */
#define EPD_REG_51            0x33   /* Read LUT register */
#define EPD_REG_54            0x36   /* Program OTP selection */
#define EPD_REG_55            0x37   /* Proceed OTP selection */
#define EPD_REG_58            0x3A   /* Set dummy line pulse period */
#define EPD_REG_59            0x3B   /* Set Gate line width */
#define EPD_REG_60            0x3C   /* Select Border waveform */
#define EPD_REG_68            0x44   /* Set RAM X - Address Start / End Position */
#define EPD_REG_69            0x45   /* Set RAM Y - Address Start / End Position */
#define EPD_REG_78            0x4E   /* Set RAM X Address Counter */
#define EPD_REG_79            0x4F   /* Set RAM Y Address Counter */
#define EPD_REG_240           0xF0   /* Booster Set Internal Feedback Selection */
#define EPD_REG_255           0xFF   /* NOP */

GDE021A1::GDE021A1(PinName mosi, PinName miso, PinName sclk, PinName cs,
                   PinName cd, PinName busy, PinName pwr, PinName reset,
                   const char* name) :
    _spi(mosi, miso, sclk, cs),
    _cs(cs),
    _cd(cd),
    _busy(busy),
    _pwr(pwr),
    _reset(reset),
    GraphicsDisplay(name)
{
    _pwr = 0;                /** Enable Display */
    _cs = 0;                 /** Set or Reset the control line */
    _cs = 1;
    _reset = 1;              /** EPD reset pin mamagement */
    wait_ms(10);

    _spi.format(8, 3);        /** Setup the spi for 8 bit data, high steady state clock, second edge capture */
    _spi.frequency(4000000); /** 4MHz clock rate */

    _font.table = Font16_Table;
    _font.Width = 11;
    _font.Height = 4;
    _font.Offset = 44; // bytes per char

    registers_init();
}

int GDE021A1::width()
{
    return 172;
}

int GDE021A1::height()
{
    return 18;
}

/**
 * @brief  Clears the hole EPD.
 * @param  None
 * @retval None
 */
void GDE021A1::cls()
{
    uint32_t index = 0;

    set_display_window(0, 0, _width-1, _height-1);

    for(index = 0; index < 3096; index++)
    {
        write_pixel(EPD_COLOR_WHITE);
    }
}

/**
  * @brief  Displays one character.
  * @param  Xpos: start column address.
  * @param  Ypos: the Line where to display the character shape.
  * @param  Ascii: character ascii code, must be between 0x20 and 0x7E.
  * @retval None
  */
void GDE021A1::displayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii)
{
  Ascii -= 32;

  draw_char(Xpos, Ypos, &_font.table[Ascii * (_font.Height * _font.Width)]);
}

/**
  * @brief  Displays characters on the EPD.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Text: Pointer to string to display on EPD
  * @param  Mode: Display mode
  *          This parameter can be one of the following values:
  *            @arg  CENTER_MODE
  *            @arg  RIGHT_MODE
  *            @arg  LEFT_MODE
  * @retval None
  */
void GDE021A1::stringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Text_AlignModeTypdef Mode)
{
    uint16_t refcolumn = 1, i = 0;
    uint32_t size = 0, xsize = 0;
    uint8_t  *ptr = Text;

    /* Get the text size */
    while (*ptr++) size++ ;

    /* Characters number per line */
    xsize = (_width/_font.Width);

    switch (Mode)
    {
    case CENTER_MODE:
      {
        refcolumn = Xpos + ((xsize - size)* _font.Width) / 2;
        break;
      }
    case LEFT_MODE:
      {
        refcolumn = Xpos;
        break;
      }
    case RIGHT_MODE:
      {
        refcolumn =  - Xpos + ((xsize - size)*_font.Width);
        break;
      }
    default:
      {
        refcolumn = Xpos;
        break;
      }
    }

    /** Send the string character by character on EPD */
    while ((*Text != 0) & (((_width - (i*_font.Width)) & 0xFFFF) >= _font.Width))
    {
        displayChar(refcolumn, Ypos, *Text); /** Display one character on EPD */
        refcolumn += _font.Width;            /** Decrement the column position by 16 */
        Text++;                              /** Point on the next character */
        i++;
    }
}

/**
  * @brief  Displays a character on the EPD.
  * @param  Line: Line where to display the character shape
  *          This parameter can be one of the following values:
  *            @arg  0..8: if the Current fonts is Font8
  *            @arg  0..5: if the Current fonts is Font12
  *            @arg  0..3: if the Current fonts is Font16
  *            @arg  0..2: if the Current fonts is Font20
  * @param  ptr: Pointer to string to display on EPD
  * @retval None
  */
void GDE021A1::stringAtLine(uint16_t Line, uint8_t *ptr)
{
    stringAt(0, _font.Height*Line, ptr, LEFT_MODE);
}

/**
  * @brief  Draws an horizontal line.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Length: line length
  * @retval None
  */
void GDE021A1::drawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
    uint32_t index = 0;

    set_display_window(Xpos, Ypos, Xpos + Length, Ypos);

    for(index = 0; index < Length; index++)
    {
      write_pixel(0x3F); /** Prepare the register to write data on the RAM */
    }
}

/**
  * @brief  Draws a vertical line.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Length: line length.
  * @retval None
  */
void GDE021A1::drawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
    uint32_t index = 0;

    set_display_window(Xpos, Ypos, Xpos, Ypos + Length);

    for(index = 0; index < Length; index++)
    {
      write_pixel(0x00); /** Prepare the register to write data on the RAM */
    }
}

/**
  * @brief  Draws a rectangle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Height: rectangle height
  * @param  Width: rectangle width
  * @retval None
  */
void GDE021A1::drawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
    /* Draw horizontal lines */
    drawHLine(Xpos, Ypos, Width);
    drawHLine(Xpos, (Ypos + Height), (Width + 1));

    /* Draw vertical lines */
    drawVLine(Xpos, Ypos, Height);
    drawVLine((Xpos + Width), Ypos , Height);
}

/**
  * @brief  Displays a full rectangle.
  * @param  Xpos: X position.
  * @param  Ypos: Y position.
  * @param  Height: display rectangle height.
  * @param  Width: display rectangle width.
  * @retval None
  */
void GDE021A1::fillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
    uint16_t index = 0;

    /* Set the rectangle */
    set_display_window(Xpos, Ypos, (Xpos + Width), (Ypos + Height));

    for(index = 0; index < 3096; index++)
    {
        write_pixel(EPD_COLOR_BLACK);
    }
}

/**
  * @brief  Draws an Image.
  * @param  Xpos: X position in the EPD
  * @param  Ypos: Y position in the EPD
  * @param  Xsize: X size in the EPD
  * @param  Ysize: Y size in the EPD
  * @param  pdata: Pointer to the Image address
  * @retval None
  */
void GDE021A1::drawImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pdata)
{
    /* Set display window */
    set_display_window(Xpos, Ypos, (Xpos+Ysize-1), (Ypos+(Xsize/4)-1));

    draw_image(Xpos, Ypos, Xsize, Ysize, pdata);

    set_display_window(0, 0, _width, _height);
}

void GDE021A1::splashScreen( void )
{
    cls();
    drawRect(10, 4, 152, 8);
    stringAtLine(1, (uint8_t*)"BUDDI");
    refresh_display();
}

void GDE021A1::refresh( void )
{
    refresh_display();
}

// set cursor position
void GDE021A1::locate(int x, int y)
{
    char_x = x;
    char_y = y;
}

// calc char columns
int GDE021A1::columns()
{
    return width() / _font.Width;
}

// calc char rows
int GDE021A1::rows()
{
    return height() / _font.Height;
}

// print char
int GDE021A1::_putc(int value)
{
    if (value == '\n') {    // new line
        char_x = 0;
        char_y = char_y + _font.Height;
        if (char_y >= height() - _font.Height) {
            char_y = 0;
        }
    } else {
        character(char_x, char_y, value);
    }
    return value;
}

// paint char out of font
void GDE021A1::character(int x, int y, int c)
{
    unsigned int hor,vert,offset,bpl,j,i,b;
    unsigned char* zeichen;
    unsigned char z,w;

    if ((c < 31) || (c > 127)) return;   // test char range

    // read font parameter from start of array
    offset = _font.Offset;               // bytes / char
    hor = _font.Width;                   // get hor size of font
    vert = _font.Height;                 // get vert size of font
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

/**
 * @brief  Writes data to the device
 * @param  RegValue - Register address to write to
 * @retval None
 */
void GDE021A1::write_data( uint16_t RegValue )
{
    _cs = 0;              /** Reset EPD control line CS */
    _cd = 1;              /** Set EPD data/command line DC to High */
    _spi.write(RegValue); /** Send Data */
    _cs = 1;              /** Deselect: Chip Select high */
}

/**
 * @brief  Writes command to selected EPD register.
 * @param  Reg - Address of the selected register
 * @retval None
 */
void GDE021A1::write_reg( uint8_t Reg )
{
    _cs = 0;              /** Reset EPD control line CS */
    _cd = 0;              /** Set EPD data/command line DC to Low */
    _spi.write(Reg);      /** Send Data */
    _cs = 1;              /** Deselect: Chip Select high */
}

/**
 * @brief  Reads data from EPD data register.
 * @param  None
 * @retval Read data.
 */
uint16_t GDE021A1::read_data( void )
{
    _cs = 0;                /** Reset EPD control line CS */
    _cs = 1;                /** Deselect: Chip Select high */
    return _spi.write(0x00); /** Send Data */
}

/**
 * @brief  Initialize the GDE021A1 EPD Component registers.
 * @param  None
 * @retval None
 */
void GDE021A1::registers_init(void)
{
    uint8_t nb_bytes = 0;

    write_reg(EPD_REG_16);  /** Deep sleep mode disable */
    write_data(0x00);
    write_reg(EPD_REG_17);  /** Data Entry Mode Setting */
    write_data(0x03);
    write_reg(EPD_REG_68);  /** Set the RAM X start/end address */
    write_data(0x00);       /** RAM X address start = 00h */
    write_data(0x11);       /** RAM X adress end = 11h (17 * 4pixels by address = 72 pixels) */
    write_reg(EPD_REG_69);  /** Set the RAM Y start/end address */
    write_data(0x00);       /** RAM Y address start = 0 */
    write_data(0xAB);       /** RAM Y adress end = 171 */
    write_reg(EPD_REG_78);  /** Set RAM X Address counter */
    write_data(0x00);
    write_reg(EPD_REG_79);  /** Set RAM Y Address counter */
    write_data(0x00);
    write_reg(EPD_REG_240); /** Booster Set Internal Feedback Selection */
    write_data(0x1F);
    write_reg(EPD_REG_33);  /** Disable RAM bypass and set GS transition to GSA = GS0 and GSB = GS3 */
    write_data(0x03);
    write_reg(EPD_REG_44);  /** Write VCOMregister */
    write_data(0xA0);
    write_reg(EPD_REG_60);  /** Border waveform */
    write_data(0x64);
    write_reg(EPD_REG_50);  /** Write LUT register */

    for (nb_bytes=0; nb_bytes<90; nb_bytes++)
    {
        write_data(WF_LUT[nb_bytes]);
    }
}

/**
 * @brief  Writes 4 dots.
 * @param  HEX_Code: specifies the Data to write.
 * @retval None
 */
void GDE021A1::write_pixel(uint8_t HEX_Code)
{
    write_reg(EPD_REG_36); /** Prepare the register to write data on the RAM */
    write_data(HEX_Code);  /** Send the data to write */
}

/**
 * @brief  Sets a display window.
 * @param  Xpos: specifies the X bottom left position.
 * @param  Ypos: specifies the Y bottom left position.
 * @param  Width: display window width.
 * @param  Height: display window height.
 * @retval None
 */
void GDE021A1::set_display_window(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
    /** Set Y position and the height */
    write_reg(EPD_REG_68);
    write_data(Ypos);
    write_data(Height);
    /** Set X position and the width */
    write_reg(EPD_REG_69);
    write_data(Xpos);
    write_data(Width);
    /** Set the height counter */
    write_reg(EPD_REG_78);
    write_data(Ypos);
    /** Set the width counter */
    write_reg(EPD_REG_79);
    write_data(Xpos);
}

/**
  * @brief  Activates display update sequence.
  * @param  None
  * @retval None
  */
void GDE021A1::refresh_display(void)
{
    write_reg(EPD_REG_34); /** Write on the Display update control register */
    write_data(0xC4);      /** Display update data sequence option */

    /**
     * Launching the update: Nothing should interrupt this sequence in order
     * to avoid display corruption
     */
    write_reg(EPD_REG_32);

    while (_busy != 0);   /** Poll on the BUSY signal and wait for the EPD to be ready */

    _reset = 1;           /** EPD reset pin mamagement */
    wait_ms(10);          /** Add a 10 ms Delay after EPD pin Reset */
}

/**
  * @brief  Disables the clock and the charge pump.
  * @param  None
  * @retval None
  */
void GDE021A1::close_charge_pump(void)
{
    write_reg(EPD_REG_34);  /** Write on the Display update control register */
    write_data(0x03);       /** Disable CP then Disable Clock signal */

    /**
     * Launching the update: Nothing should interrupt this sequence in order
     * to avoid display corruption
     */
    write_reg(EPD_REG_32);
    wait_ms(400);
}

/**
  * @brief  Displays picture..
  * @param  pdata: picture address.
  * @param  Xpos:  Image X position in the EPD
  * @param  Ypos:  Image Y position in the EPD
  * @param  Xsize: Image X size in the EPD
  * @note   Xsize have to be a multiple of 4
  * @param  Ysize: Image Y size in the EPD
  * @retval None
  */
void GDE021A1::draw_image(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pdata)
{
    uint32_t i, j = 0;
    uint8_t pixels_4 = 0;
    uint8_t pixels_4_grey[4];
    uint8_t nb_4_pixels, data_res = 0;

    write_reg(EPD_REG_36);  /** Prepare the register to write data on the RAM */

    if ((Xsize % 8) == 0)   /** X size is a multiple of 8 */
    {
      for (i= 0; i< ((((Ysize) * (Xsize/4)))/2) ; i++)
      {
        /* Get the current data */
        pixels_4 = pdata[i];
        if (pixels_4 !=0)
        {
          /* One byte read codes 8 pixels in 1-bit bitmap */
          for (nb_4_pixels = 0; nb_4_pixels < 2; nb_4_pixels++)
          {
            /* Processing 8 pixels */
            /* Preparing the 4 pixels coded with 4 grey level per pixel
               from a monochrome xbm file */
            for (j= 0; j<4; j++)
            {
              if (((pixels_4) & 0x01) == 1)
              {
                /* Two LSB is coding black in 4 grey level */
                pixels_4_grey[j] &= 0xFC;
              }
              else
              {
                /* Two LSB is coded white in 4 grey level */
                pixels_4_grey[j] |= 0x03;
              }
              pixels_4 = pixels_4 >> 1;
            }

            /* Processing 4 pixels */
            /* Format the data to have the Lower pixel number sent on the MSB for the SPI to fit with the RAM
               EPD topology */
            data_res = pixels_4_grey[0] << 6 | pixels_4_grey[1] << 4 | pixels_4_grey[2] << 2 | pixels_4_grey[3] << 0;

            /* Send the data to the EPD's RAM through SPI */
            write_data(data_res);
          }
        }
        else
        {
          /* 1 byte read from xbm files is equivalent to 8 pixels in the
             other words 2 bytes to be transferred */
          write_data(0xFF);
          write_data(0xFF);
        }
      }
    }

    /* X size is a multiple of 4 */
    else
    {
      for (i= 0; i< ((((Ysize) * ((Xsize/4)+1))/2)) ; i++)
      {
        /* Get the current data */
        pixels_4 = pdata[i];
        if (((i+1) % (((Xsize/4)+1)/2)) != 0)
        {
          if (pixels_4 !=0)
          {
            /* One byte read codes 8 pixels in 1-bit bitmap */
            for (nb_4_pixels = 0; nb_4_pixels < 2; nb_4_pixels++)
            {
              /* Processing 8 pixels */
              /* Preparing the 4 pixels coded with 4 grey level per pixel
                 from a monochrome xbm file */
              for (j= 0; j<4; j++)
              {
                if (((pixels_4) & 0x01) == 1)
                {
                  /* Two LSB is coding black in 4 grey level */
                  pixels_4_grey[j] &= 0xFC;
                }
                else
                {
                  /* Two LSB is coded white in 4 grey level */
                  pixels_4_grey[j] |= 0x03;
                }
                pixels_4 = pixels_4 >> 1;
              }

              /* Processing 4 pixels */
              /* Format the data to have the Lower pixel number sent on the MSB for the SPI to fit with the RAM
                 EPD topology */
              data_res = pixels_4_grey[0] << 6 | pixels_4_grey[1] << 4 | pixels_4_grey[2] << 2 | pixels_4_grey[3] << 0;

              /* Send the data to the EPD's RAM through SPI */
              write_data(data_res);
            }
          }
          else if (pixels_4 == 0)
          {
            /* One byte read from xbm files is equivalent to 8 pixels in the
               other words Two bytes to be transferred */
            write_data(0xFF);
            write_data(0xFF);
          }
        }

        else if (((i+1) % (((Xsize/4)+1)/2)) == 0)
        {
          if (pixels_4 !=0xf0)
          {
            /* Processing 8 pixels */
            /* Preparing the 4 pixels coded with 4 grey level per pixel
               from a monochrome xbm file */
            for (j= 0; j<4; j++)
            {
              if (((pixels_4) & 0x01) == 1)
              {
                /* 2 LSB is coding black in 4 grey level */
                pixels_4_grey[j] &= 0xFC;
              }
              else
              {
                /* 2 LSB is coded white in 4 grey level */
                pixels_4_grey[j] |= 0x03;
              }
              pixels_4 = pixels_4 >> 1;
            }

            /* Processing 4 pixels */
            /* Format the data to have the Lower pixel number sent on the MSB for the SPI to fit with the RAM
               EPD topology */
            data_res = pixels_4_grey[0] << 6 | pixels_4_grey[1] << 4 | pixels_4_grey[2] << 2 | pixels_4_grey[3] << 0;

            /* Send the data to the EPD's RAM through SPI */
            write_data(data_res);
          }
          else if (pixels_4 == 0xf0)
          {
            /* One byte to be transferred */
            write_data(0xFF);
          }
        }
      }
    }
}

/**
 * @brief  Draws a character on EPD.
 * @param  Xpos: specifies the X position, can be a value from 0 to 171
 * @param  Ypos: specifies the Y position, can be a value from 0 to 17
 * @param  c: pointer to the character data
 * @retval None
 */
void GDE021A1::draw_char(uint16_t Xpos, uint16_t Ypos, const uint8_t *c)
{
    uint32_t index = 0;
    uint32_t data_length = 0;

    /* Set the Character display window */
    set_display_window(Xpos, Ypos, (Xpos + _font.Width - 1), (Ypos + _font.Height - 1));

    data_length = (_font.Height * _font.Width);

    for(index = 0; index < data_length; index++)
    {
        write_pixel(c[index]);
    }
}
