#include "ninjaskit/ninjaskit.h"
#include "lcd/lcd.h"


LiquidCrystal lcd({PA, 1}, {PB, 5}, {PA, 3}, {PA, 6}, {PA, 4}, {PA, 2}, {PA, 0});


int main(void)
{
    clock_setup();

    configure_as_output({PB, 0});
    set_pin({PB, 0}, true);

    etk::sleep_ms(300);


    lcd.begin();
    lcd.show_cursor(false);
    lcd.blink_cursor(false);

    etk::sleep_ms(100);

    etk::StaticString<10> ss = "Seconds: ";
    lcd.print(ss);

    while(true)
    {
        lcd.set_cursor(3, ss.length());
        lcd.print(etk::now().seconds());
        etk::sleep_ms(10);
    }
}








