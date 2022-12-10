/********************************************************************************
* setup.cpp: Innehåller initieringsrutiner.
********************************************************************************/
#include "header.hpp"

/* Globala objekt: */
led l1(8), l2(9), l3(10);
led_vector v1;
button b1(13);
timer t0(timer::sel::timer0, 300);
timer t1(timer::sel::timer1, 50);
pwm<led_vector> pwm1(A0, &v1, &led_vector::on, &led_vector::off);

/********************************************************************************
* setup: Initierar det inbyggda systemet.
********************************************************************************/
void setup(void)
{
   static struct led* leds[] = { &l1, &l2, &l3};
   const size_t num_leds = sizeof(leds) / sizeof(struct led*);

   v1.init(leds, num_leds);
   b1.enable_interrupt();

   serial::init();

   eeprom::write_byte(TIMEOUT_ADDRESS, 0);
   wdt::init(wdt::timeout::_8192_ms);
   wdt::enable_interrupt();

   return;
}