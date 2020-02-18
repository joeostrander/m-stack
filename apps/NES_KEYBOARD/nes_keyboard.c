#include <stddef.h>
#include <stdint.h>
#include <pic16f1455.h>
#include "usb_hid_keys.h"


#define DATA_Get()      (PORTCbits.RC2 & 0x1)
#define CLK_Set()       (LATAbits.LATA4 = 1)
#define CLK_Clear()     (LATAbits.LATA4 = 0)
#define LATCH_Set()     (LATAbits.LATA5 = 1)
#define LATCH_Clear()   (LATAbits.LATA5 = 0)


/*
 #define MB_RD_5_Set()               (LATESET = (1<<5))
#define MB_RD_5_Clear()             (LATECLR = (1<<5))
#define MB_RD_5_Toggle()            (LATEINV= (1<<5))
#define MB_RD_5_Get()               ((PORTE >> 5) & 0x1)
#define MB_RD_5_OutputEnable()      (TRISECLR = (1<<5))
#define MB_RD_5_InputEnable()       (TRISESET = (1<<5))

 another way:
 define led_TRIS               TRISCbits.TRISC2
#define led_LAT                LATCbits.LATC2
#define led_PORT               PORTCbits.RC2
#define led_WPU                WPUCbits.WPUC2
#define led_ANS                ANSELCbits.ANSC2
#define led_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define led_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define led_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define led_GetValue()           PORTCbits.RC2
#define led_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define led_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define led_SetPullup()      do { WPUCbits.WPUC2 = 1; } while(0)
#define led_ResetPullup()    do { WPUCbits.WPUC2 = 0; } while(0)
#define led_SetAnalogMode()  do { ANSELCbits.ANSC2 = 1; } while(0)
#define led_SetDigitalMode() do { ANSELCbits.ANSC2 = 0; } while(0)
 */

#define BUTTON_A        (1<<0)
#define BUTTON_B        (1<<1)
#define BUTTON_SELECT   (1<<2)
#define BUTTON_START    (1<<3)
#define BUTTON_UP       (1<<4)
#define BUTTON_DOWN     (1<<5)
#define BUTTON_LEFT     (1<<6)
#define BUTTON_RIGHT    (1<<7)

//#define KEY_ESC 
//#define KEY_RETURN 
//#define KEY_UP_ARROW 
//#define KEY_DOWN_ARROW 
//#define KEY_LEFT_ARROW 
//#define KEY_RIGHT_ARROW 


#define CLK     GPIO_PIN_XXX
#define LATCH   GPIO_PIN_XXX
#define DATA    GPIO_PIN_XXX





      /*
        NES Word Mapping
        x x x x x x x x
        | | | | | | | |_  A
        | | | | | | |___  B
        | | | | | |_____  SELECT
        | | | | |_______  START
        | | | |_________  UP
        | | |___________  DOWN
        | |_____________  LEFT
        |_______________  RIGHT
     */

/*
    keyboard mapping:
    B – B
    A – A
    Enter – Start
    ESC – Select
    Keypad up – Up
    Keypad left – Left
    Keypad down – Down
    Keypad right – Right
*/

struct KEYMAP
{
	uint8_t button;
	uint16_t key;
};

struct KEYMAP button_key_maps[] =
{
    { BUTTON_A, KEY_A },
    { BUTTON_B, KEY_B },
    { BUTTON_SELECT, KEY_ESC },
    { BUTTON_START, KEY_ENTER },
    { BUTTON_UP, KEY_UP },
    { BUTTON_DOWN, KEY_DOWN },
    { BUTTON_LEFT, KEY_LEFT },
    { BUTTON_RIGHT, KEY_RIGHT },
    
    { NULL, NULL },
};

uint8_t last_reading = 0;

void GPIO_Initialize() 
{
    //ANSELCbits.ANSC2 = 0;
    ANSELC = 0xFF;
    TRISCbits.TRISC4 = 0b0;  //Set RC4 as output
    TRISCbits.TRISC3 = 0b0;  //Set RC3 as output
    TRISCbits.TRISC2 = 0b1; //Set RC2 as input
}

uint8_t read_NESpad() 
{
      /*
        NES Word Mapping
        x x x x x x x x
        | | | | | | | |_  A
        | | | | | | |___  B
        | | | | | |_____  SELECT
        | | | | |_______  START
        | | | |_________  UP
        | | |___________  DOWN
        | |_____________  LEFT
        |_______________  RIGHT
     */

  // Send a HIGH pulse to latch. Make 8 shift register store state
  // of all buttons
//  LATCH_Set();
//  __delay_us(12);
//  LATCH_Clear();

  // Clock the 8 shift register to get the
  // state of the buttons
  uint8_t output = 0x00;
  int i;
  for ( i = 0; i < 8; i++)
  {
      output |= !( DATA_Get()) << i;
      CLK_Set();
      __delay_us(6);
      CLK_Clear();
      __delay_us(6);
  }
  
  return output;
}
