/********************************************************************************
* serial.hpp: Innehåller funktionalitet för seriell överföring via USART.
********************************************************************************/
#ifndef SERIAL_HPP_
#define SERIAL_HPP_

/* Inkluderingsdirektiv: */
#include "misc.hpp"

/********************************************************************************
* serial: Namnrymd innehållande drivrutiner för implementering av seriell
*         överföring via USART.
********************************************************************************/
namespace serial
{
   /********************************************************************************
   * init: Initierar USART för seriell överföring med angiven baud rate.
   *
   *       - baud_rate: Överföringshastighet mätt i kilobits per sekund.
   ********************************************************************************/
   void init(const uint32_t baud_rate_kbps = 9600);

   /********************************************************************************
   * print: Skriver ut text via seriell överföring.
   *
   *        - s: Pekare till det textstycke som ska skrivas ut.
   ********************************************************************************/
   void print(const char* s);

   /********************************************************************************
   * print: Skriver ut ett signerat heltal via seriell överföring.
   *
   *        - number: Heltalet som ska skrivas ut.
   ********************************************************************************/
   void print(const int32_t number);

   /********************************************************************************
   * print_unsigned: Skriver ut ett osignerat heltal via seriell överföring.
   *
   *                 - number: Heltalet som ska skrivas ut.
   ********************************************************************************/
   void print_unsigned(const uint32_t number);

   /********************************************************************************
   * print: Skriver ut ett flyttal via seriell överföring.
   *
   *        - number: Flyttalet som ska skrivas ut.
   ********************************************************************************/
   void print(const double number);

   /********************************************************************************
   * print: Skriver ut ett enskilt tecken via seriell överföring.
   *
   *        - character: Det tecken som ska skrivas ut.
   ********************************************************************************/
   void print(const char character);

   /********************************************************************************
   * print_new_line: Sätter nästa utskrift till längst till vänster på nästa rad.
   ********************************************************************************/
   auto print_new_line = [](void)
   {
      serial::print("\n");
   };
}

#endif /* SERIAL_HPP_ */