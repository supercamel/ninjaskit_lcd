/*
    Ninjaskit LCD Driver
    
    Copyright (C) 2016 Samuel Cowen

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "lcd.h"

LiquidCrystal::LiquidCrystal(gpio_pin rs, gpio_pin enable, gpio_pin rw,
                             gpio_pin d0, gpio_pin d1, gpio_pin d2, gpio_pin d3)
{
    rs_pin = rs;
    enable_pin = enable;
    rw_pin = rw;

    data_pins[0] = d0;
    data_pins[1] = d1;
    data_pins[2] = d2;
    data_pins[3] = d3;

    display_function = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;
}

void LiquidCrystal::begin()
{
    configure_as_output(rs_pin);
    configure_as_output(enable_pin);
    configure_as_output(rw_pin);

    for(int i = 0; i < 4; i++)
        configure_as_output(data_pins[i]);

    set_pin(rs_pin, LOW);
    set_pin(enable_pin, LOW);
    set_pin(rw_pin, LOW);

    write_bits(0x03);
    etk::sleep_us(4500);
    write_bits(0x03);
    etk::sleep_us(500);
    write_bits(0x03);
    etk::sleep_us(250);
    write_bits(0x02);
    etk::sleep_ms(2);
    write_bits(0x02);
    etk::sleep_us(150);
    write_bits(LCD_2LINE);
    etk::sleep_us(150);

    command(LCD_DISPLAYCONTROL);
    clear();
    command(LCD_ENTRYMODESET | LCD_ENTRYLEFT);
    display();
}


void LiquidCrystal::command(uint8 cmd)
{
    send(cmd, false);
}

void LiquidCrystal::clear()
{
    command(LCD_CLEARDISPLAY);
}

void LiquidCrystal::home()
{
    command(LCD_RETURNHOME);
}

void LiquidCrystal::set_cursor(uint8 row, uint8 col)
{
	uint8 row_offsets[4];
	row_offsets[0] = 0x0;
    row_offsets[1] = 0x40;
    row_offsets[2] = 0x14;
    row_offsets[3] = 0x54;
    
    command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void LiquidCrystal::display()
{
    display_control |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | display_control);
}

void LiquidCrystal::no_display()
{
    display_control &= ~LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | display_control);
}

void LiquidCrystal::show_cursor(bool on)
{
    if(on)
        display_control |= LCD_CURSORON;
    else
        display_control &= ~LCD_CURSORON;

    command(LCD_DISPLAYCONTROL | display_control);
}

void LiquidCrystal::blink_cursor(bool on)
{
    if(on)
        display_control |= LCD_BLINKON;
    else
        display_control &= ~LCD_BLINKON;

    command(LCD_DISPLAYCONTROL | display_control);
}


void LiquidCrystal::send(uint8 cmd, bool mode)
{
    set_pin(rs_pin, mode);
    etk::Bits<uint8> b(cmd);

    for (int i = 4; i < 8; i++)
        set_pin(data_pins[i-4], b.read_bit(i));

    pulse();

    for (int i = 0; i < 4; i++)
        set_pin(data_pins[i], b.read_bit(i));

    pulse();
    busy_wait();
}

void LiquidCrystal::write_bits(uint8 value)
{
    etk::Bits<uint8> b(value);
    for (int i = 0; i < 4; i++)
        set_pin(data_pins[i], b.read_bit(i));

    pulse();
}

void LiquidCrystal::pulse()
{
    etk::sleep_us(1);
    set_pin(enable_pin, HIGH);
    etk::sleep_us(1);
    set_pin(enable_pin, LOW);
    etk::sleep_us(100);
}

void LiquidCrystal::busy_wait()
{
    bool busy = true;
    configure_as_input(data_pins[3]);

    set_pin(rs_pin, LOW);
    set_pin(rw_pin, HIGH);

    while(busy)
    {
        set_pin(enable_pin, HIGH);
        etk::sleep_us(1);
        busy = read_pin(data_pins[3]);
        set_pin(enable_pin, LOW);
    }

    set_pin(rw_pin, LOW);
    configure_as_output(data_pins[3]);
}

