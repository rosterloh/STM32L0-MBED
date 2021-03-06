/**
 ******************************************************************************
 * @file    GDE021A1.h
 * @author  Richard Osterloh (richard.osterloh@gmail.com)
 * @version V1.0.0
 * @date    31-October-2014
 * @brief   An interface for the GDE021A1 E-paper Display
 ******************************************************************************
 * @code
 * #include "mbed.h"
 * #include "GDE021A1.h"
 *
 * GDE021A1 display(p28, p27);
 *
 * int main() {
 *
 *     while(1) {
 *     }
 *
 * }
 * @endcode
 ******************************************************************************
 */

#ifndef GDE021A1_H
#define GDE021A1_H

#include "mbed.h"
//#include "GraphicsDisplay.h"

/**
  * @brief  Line mode structures definition
  */
typedef enum
{
  CENTER_MODE             = 0x01,    /*!< Center mode */
  RIGHT_MODE              = 0x02,    /*!< Right mode  */
  LEFT_MODE               = 0x03     /*!< Left mode   */
} Text_AlignModeTypdef;

/**
  * @brief  Font type information definition
  */
typedef struct _tFont
{
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
  uint16_t Offset;

} sFONT;

class GDE021A1/* : public GraphicsDisplay*/
{
public:
    /**
     * @brief Creates a new GDE021A1 object
     * @param mosi  - SPI MOSI pin
     * @param sclk  - SPI SCLK pin
     * @param cs    - SPI CS pin
     * @param cd    - command/data pin
     * @param busy  - busy pin
     * @param pwr   - power pin
     * @param reset - reset pin
     */
    GDE021A1(PinName mosi, PinName sclk, PinName cs,
             PinName cd, PinName busy, PinName pwr, PinName reset/*,
             const char* name ="E_Paper"*/);

    /** Get the width of the screen in pixel
      *
      * @param
      * @returns width of screen in pixel
      *
      */
    //virtual int width();

    /** Get the height of the screen in pixel
     *
     * @returns height of screen in pixel
     *
     */
    //virtual int height();

    /** Draw a pixel at x,y black or white
     *
     * @param x horizontal position
     * @param y vertical position
     * @param colour ,1 set pixel ,0 erase pixel
     */
    //virtual void pixel(int x, int y, int colour);

    /**
     * @brief  Clears the hole EPD.
     * @param  None
     * @retval None
     */
    void cls();

    /**
      * @brief  Displays one character.
      * @param  Xpos: start column address.
      * @param  Ypos: the Line where to display the character shape.
      * @param  Ascii: character ascii code, must be between 0x20 and 0x7E.
      * @retval None
      */
    void displayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii);

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
    void stringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Text_AlignModeTypdef Mode);

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
    void stringAtLine(uint16_t Line, uint8_t *ptr);
    /**
      * @brief  Draws an horizontal line.
      * @param  Xpos: X position
      * @param  Ypos: Y position
      * @param  Length: line length
      * @retval None
      */
    void drawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);

    /**
      * @brief  Draws a vertical line.
      * @param  Xpos: X position
      * @param  Ypos: Y position
      * @param  Length: line length.
      * @retval None
      */
    void drawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);

    /**
      * @brief  Draws a rectangle.
      * @param  Xpos: X position
      * @param  Ypos: Y position
      * @param  Height: rectangle height
      * @param  Width: rectangle width
      * @retval None
      */
    void drawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);

    /**
      * @brief  Displays a full rectangle.
      * @param  Xpos: X position.
      * @param  Ypos: Y position.
      * @param  Height: display rectangle height.
      * @param  Width: display rectangle width.
      * @retval None
      */
    void fillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);

    /**
      * @brief  Draws an Image.
      * @param  Xpos: X position in the EPD
      * @param  Ypos: Y position in the EPD
      * @param  Xsize: X size in the EPD
      * @param  Ysize: Y size in the EPD
      * @param  pdata: Pointer to the Image address
      * @retval None
      */
    void drawImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pdata);

    void splashScreen(void);

    void refresh(void);

    /** setup cursor position
     *
     * @param x x-position (top left)
     * @param y y-position
     */
    //virtual void locate(int x, int y);

    /** calculate the max number of char in a line
     *
     * @returns max columns
     * depends on actual font size
     */
    //virtual int columns();

    /** calculate the max number of columns
     *
     * @returns max column
     * depends on actual font size
     */
    //virtual int rows();

    /** paint a character on given position out of the active font to the screen buffer
     *
     * @param x x-position (top left)
     * @param y y-position
     * @param c char code
     */
    //virtual void character(int x, int y, int c);

private:
    SPI _spi;
    DigitalOut _cs;
    DigitalOut _cd;
    DigitalIn _busy;
    DigitalOut _pwr;
    DigitalOut _reset;
    sFONT _font;
    static const uint16_t _height = 18;
    static const uint16_t _width = 172;

    /**
     * @brief  Writes data to the device
     * @param  RegValue - Register address to write to
     * @retval None
     */
    void write_data( uint16_t RegValue );

    /**
     * @brief  Writes command to selected EPD register.
     * @param  Reg - Address of the selected register
     * @retval None
     */
    void write_reg( uint8_t Reg );

    /**
     * @brief  Reads data from EPD data register.
     * @param  None
     * @retval Read data.
     */
    uint16_t read_data( void );

    /**
     * @brief  Initialize the GDE021A1 EPD Component registers.
     * @param  None
     * @retval None
     */
    void registers_init(void);

    /**
     * @brief  Writes 4 dots.
     * @param  HEX_Code: specifies the Data to write.
     * @retval None
     */
    void write_pixel(uint8_t HEX_Code);

    /**
     * @brief  Sets a display window.
     * @param  Xpos: specifies the X bottom left position.
     * @param  Ypos: specifies the Y bottom left position.
     * @param  Width: display window width.
     * @param  Height: display window height.
     * @retval None
     */
    void set_display_window(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);

    /**
      * @brief  Activates display update sequence.
      * @param  None
      * @retval None
      */
    void refresh_display(void);

    /**
     * @brief  Disables the clock and the charge pump.
     * @param  None
     * @retval None
     */
    void close_charge_pump(void);

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
    void draw_image(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pdata);

    /**
     * @brief  Draws a character on EPD.
     * @param  Xpos: specifies the X position, can be a value from 0 to 171
     * @param  Ypos: specifies the Y position, can be a value from 0 to 17
     * @param  c: pointer to the character data
     * @retval None
     */
    void draw_char(uint16_t Xpos, uint16_t Ypos, const uint8_t *c);
};

#endif
