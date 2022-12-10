/********************************************************************************
* misc.hpp: Innehåller diverse bibliotek, definitioner samt enumerationer för
*           implementering av mikrodatorsystemet
********************************************************************************/
#ifndef MISC_HPP_
#define MISC_HPP_

/* Klockfrekvens (behövs för fördröjningsrutiner): */
#define F_CPU 16000000UL /* 16 MHz. */

/* Inkluderingsdirektiv: */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/* Konstanter för port-nummer på ATmega328P samt motsvarande pin-nummer på Arduino Uno: */
static constexpr auto D0 = 0; /* PORTD0 / pin 0. */
static constexpr auto D1 = 1; /* PORTD1 / pin 1. */
static constexpr auto D2 = 2; /* PORTD2 / pin 2. */
static constexpr auto D3 = 3; /* PORTD3 / pin 3. */
static constexpr auto D4 = 4; /* PORTD4 / pin 4. */
static constexpr auto D5 = 5; /* PORTD5 / pin 5. */
static constexpr auto D6 = 6 ;/* PORTD6 / pin 6. */
static constexpr auto D7 = 7; /* PORTD7 / pin 7. */

static constexpr auto B0 = 8;  /* PORTB0 / pin 8. */
static constexpr auto B1 = 9;  /* PORTB1 / pin 9. */
static constexpr auto B2 = 10; /* PORTB2 / pin 10. */
static constexpr auto B3 = 11; /* PORTB3 / pin 11. */
static constexpr auto B4 = 12; /* PORTB4 / pin 12. */
static constexpr auto B5 = 13; /* PORTB5 / pin 13. */

static constexpr auto C0 = 14; /* PORTC0 / pin A0. */
static constexpr auto C1 = 15; /* PORTC1 / pin A1. */
static constexpr auto C2 = 16; /* PORTC2 / pin A2. */
static constexpr auto C3 = 17; /* PORTC3 / pin A3. */
static constexpr auto C4 = 18; /* PORTC4 / pin A4. */
static constexpr auto C5 = 19; /* PORTC5 / pin A5. */

static constexpr auto A0 = 14; /* PORTC0 / pin A0. */
static constexpr auto A1 = 15; /* PORTC1 / pin A1. */
static constexpr auto A2 = 16; /* PORTC2 / pin A2. */
static constexpr auto A3 = 17; /* PORTC3 / pin A3. */
static constexpr auto A4 = 18; /* PORTC4 / pin A4. */
static constexpr auto A5 = 19; /* PORTC5 / pin A5. */

/********************************************************************************
* io_port: Enumerationsklass för val av I/O-port mellan I/O-portar B, C och D.
********************************************************************************/
enum class io_port
{
   b,   /* I/O-port B (pin 8 - 13 på Arduino Uno). */
   c,   /* I/O-port C (pin A0 - A5 på Arduino Uno). */
   d,   /* I/O-port D (pin 0 - 7 på Arduino Uno). */
   none /* Icke-specificerad I/O-port. */
};

/********************************************************************************
* misc: Namnrymd som innehåller diverse funktioner.
********************************************************************************/
namespace misc 
{
   /********************************************************************************
   * delay_ms: Genererar fördröjning mätt i millisekunder.
   *
   *           - delay_time_ms: Referens till angiven fördröjningstid.
   ********************************************************************************/
   auto delay_ms = [](const uint16_t& delay_time_ms)
   {
      for (uint16_t i = 0; i < delay_time_ms; ++i)
      {
         _delay_ms(1);
      }
   };

   /********************************************************************************
   * delay_us: Genererar fördröjning mätt i mikrosekunder.
   *
   *           - delay_time_us: Referens till angiven fördröjningstid.
   ********************************************************************************/
   auto delay_us = [](const uint16_t& delay_time_us)
   {
      for (uint16_t i = 0; i < delay_time_us; ++i)
      {
         _delay_us(1);
      }
   };

   /********************************************************************************
   * enable_all_pin_change_interrupts: Aktiverar PCI-avbrott på alla I/O-portar.
   ********************************************************************************/
   auto enable_all_pin_change_interrupts = []()
   {
      PCICR = (1 << PCIE2) | (1 << PCIE1) | (1 << PCIE0);
   };

   /********************************************************************************
   * disable_all_pin_change_interrupts: Inaktiverar PCI-avbrott på alla I/O-portar.
   ********************************************************************************/
   auto disable_all_pin_change_interrupts = []()
   {
      PCICR = 0;
   };

   /********************************************************************************
   * enable_pin_change_interrupt: Aktiverar PCI-avbrott på angiven I/O-port.
   *
   *                              - io_port: I/O-porten som PCI-avbrott ska
   *                                         aktiveras på.
   ********************************************************************************/
   auto enable_pin_change_interrupt = [](const enum io_port io_port)
   {
      PCICR |= (1 << static_cast<uint8_t>(io_port));
   };

   /********************************************************************************
   * disable_pin_change_interrupt: Inaktiverar PCI-avbrott på angiven I/O-port.
   *
   *                               - io_port: I/O-porten som PCI-avbrott ska
   *                                          inaktiveras på.
   ********************************************************************************/
   auto disable_pin_change_interrupt = [](const enum io_port io_port)
   {
      PCICR &= ~(1 << static_cast<uint8_t>(io_port));
   };
}

#endif /* MISC_HPP_ */