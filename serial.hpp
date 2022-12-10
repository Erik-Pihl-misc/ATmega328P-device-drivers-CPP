/********************************************************************************
* serial.hpp: Inneh�ller funktionalitet f�r seriell �verf�ring via USART.
********************************************************************************/
#ifndef SERIAL_HPP_
#define SERIAL_HPP_

/* Inkluderingsdirektiv: */
#include "misc.hpp"

/********************************************************************************
* serial: Namnrymd inneh�llande drivrutiner f�r implementering av seriell
*         �verf�ring via USART.
********************************************************************************/
namespace serial
{
   /********************************************************************************
   * init: Initierar USART f�r seriell �verf�ring med angiven baud rate.
   *
   *       - baud_rate: �verf�ringshastighet m�tt i kilobits per sekund.
   ********************************************************************************/
   void init(const uint32_t baud_rate_kbps = 9600);

   /********************************************************************************
   * print: Skriver ut text via seriell �verf�ring.
   *
   *        - s: Pekare till det textstycke som ska skrivas ut.
   ********************************************************************************/
   void print(const char* s);

   /********************************************************************************
   * print: Skriver ut ett signerat heltal via seriell �verf�ring.
   *
   *        - number: Heltalet som ska skrivas ut.
   ********************************************************************************/
   void print(const int32_t number);

   /********************************************************************************
   * print_unsigned: Skriver ut ett osignerat heltal via seriell �verf�ring.
   *
   *                 - number: Heltalet som ska skrivas ut.
   ********************************************************************************/
   void print_unsigned(const uint32_t number);

   /********************************************************************************
   * print: Skriver ut ett flyttal via seriell �verf�ring.
   *
   *        - number: Flyttalet som ska skrivas ut.
   ********************************************************************************/
   void print(const double number);

   /********************************************************************************
   * print: Skriver ut ett enskilt tecken via seriell �verf�ring.
   *
   *        - character: Det tecken som ska skrivas ut.
   ********************************************************************************/
   void print(const char character);

   /********************************************************************************
   * print_new_line: S�tter n�sta utskrift till l�ngst till v�nster p� n�sta rad.
   ********************************************************************************/
   auto print_new_line = [](void)
   {
      serial::print("\n");
   };
}

#endif /* SERIAL_HPP_ */