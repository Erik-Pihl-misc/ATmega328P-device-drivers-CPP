/********************************************************************************
* header.hpp: Innehållande diverse definitioner samt deklarationer för
*             implementering av det inbyggda systemet.
********************************************************************************/
#ifndef HEADER_HPP_
#define HEADER_HPP_

/* Inkluderingsdirektiv: */
#include "led.hpp"
#include "button.hpp"
#include "timer.hpp"
#include "serial.hpp"
#include "eeprom.hpp"
#include "wdt.hpp"
#include "pwm.hpp"
#include "led_vector.hpp"

/* Konstanter: */
static constexpr auto TIMEOUT_ADDRESS = 100; /* Lagrar antalet passerade Watchdog timeouts. */
static constexpr auto TIMEOUT_MAX = 5;       /* Maximalt antal timeouts innan programmet låses. */

/* Deklaration av globala objekt: */
extern led l1, l2, l3;
extern led_vector v1;
extern button b1;
extern timer t0, t1;
extern pwm<led_vector> pwm1;

/********************************************************************************
* setup: Initierar det inbyggda systemet.
********************************************************************************/
void setup(void);

#endif /* HEADER_HPP_ */