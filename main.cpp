#include "ninjaskit/ninjaskit.h"
#include "lcd/lcd.h"


/*
 ~~~~~
 Intro
 ~~~~~
 
 The LCD library uses 4-bit mode and, unlike most Arduino libs, it uses the RW pin to check the busy flag.
 
 This means 7 digital pins are required and you may choose any suitable digital pin. 
 
 In addition to the digital pins, the VO input (for contrast) should be connected to the centre of a 10K pot. 
 The pot should have +3.3V and GND connected to the other leads. This will allow you to turn the contrast up and down as required.
 
 Most LCD screens also need power for the backlight. This examples project uses a digital pin ( PB0 ) 
 to supply power for the backlight. A 220 ohm (or so) resistor in series is required to limit current. 
 
 
 ~~~~~~~~~
 Wiring Up
 ~~~~~~~~~
 
 RS						-				PA1
 E / Enable				-				PB5
 RW						-				PA3
 D4						-				PA6
 D5						-				PA4
 D6						-				PA2
 D7						-				PA0
 A / Backlight +		-				PB0
 K / Backlight -		-				GND
 
 Of course, any of these connections can be easily changed by modifying the source. 
 
*/

LiquidCrystal lcd({PA, 1}, {PB, 5}, {PA, 3}, {PA, 6}, {PA, 4}, {PA, 2}, {PA, 0});


int main(void)
{
    clock_setup();

	//configure backlight pin as a digital output
    configure_as_output({PB, 0});
    //turn on backlight
    set_pin({PB, 0}, true);

	//pause for a bit, to make sure LCD screen has powered up
    etk::sleep_ms(300);

	//perform initialisation
    lcd.begin();

    
    /*
     * LiquidCrystal is streamable, which means you can use << operators to display many kinds of data
     * or, you can just add whatever you want to the print function
     * 
     * examples:
     * 		lcd << "Result: " << 56.3 << "\r\n"; 
     *
     *		lcd.print("Result: ", 56.3, "\r\n");
     *
     */
    
    //create a string and print it to LCD screen
    etk::StaticString<10> ss = "Seconds: ";
    lcd.print(ss);

    while(true)
    {
    	//move down to second row and across so it lines up with end of text
        lcd.set_cursor(1, ss.length());
        
        //print system seconds to LCD. this will write over previous text on the display.
        lcd.print(etk::now().seconds());
        
        //pause for a moment
        etk::sleep_ms(10);
    }
}








