/********************************************************************************
* serial.cpp: Innehåller drivrutiner för seriell överföring via USART.
********************************************************************************/
#include "serial.hpp"

/********************************************************************************
* init: Initierar USART för seriell överföring med angiven baud rate.
*
*       - baud_rate: Överföringshastighet mätt i kbps (default = 9600 kbps).
********************************************************************************/
void serial::init(const uint32_t baud_rate_kbps)
{
   static bool serial_initialized = false;
   if (serial_initialized) return;

   UCSR0B = (1 << TXEN0);
   UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
      
   if (baud_rate_kbps == 0 || baud_rate_kbps == 9600)
   {
      UBRR0 = 103;
   }
   else
   {
      UBRR0 = static_cast<uint8_t>(F_CPU / (16.0 * baud_rate_kbps) - 1 + 0.5);
   }

   UDR0 = '\r';
   serial_initialized = true;
   return;
}

/********************************************************************************
* print: Skriver ut text via seriell överföring.
*
*        - s: Pekare till det textstycke som ska skrivas ut.
********************************************************************************/
void serial::print(const char* s)
{
   for (const char* i = s; *i; ++i)
   {
      serial::print(*i);

      if (*i == '\n')
      {
         serial::print('\r');
      }
   }
   return;
}

/********************************************************************************
* print: Skriver ut ett signerat heltal via seriell överföring.
*
*        - number: Heltalet som ska skrivas ut.
********************************************************************************/
void serial::print(const int32_t number)
{
   char s[20] = { '\0' };
   sprintf(s, "%ld", number);
   serial::print(s);
   return;
}

/********************************************************************************
* print_unsigned: Skriver ut ett osignerat heltal via seriell överföring.
*
*                 - number: Heltalet som ska skrivas ut.
********************************************************************************/
void serial::print_unsigned(const uint32_t number)
{
   char s[20] = { '\0' };
   sprintf(s, "%ld", number);
   serial::print(s);
   return;
}

/********************************************************************************
* print: Skriver ut ett flyttal via seriell överföring.
*
*        - number: Flyttalet som ska skrivas ut.
********************************************************************************/
void serial::print(const double number)
{
   const auto integer = static_cast<int32_t>(number);
   int32_t decimal = 0;
   char s[20] = { '\0' };
      
   if (integer >= 0)
   {
      decimal = static_cast<int32_t>((number - integer) * 100 + 0.5);
   }
   else
   {
      decimal = static_cast<int32_t>((integer - number) * 100 + 0.5);
   }

   sprintf(s, "%ld.%ld", integer, decimal);
   serial::print(s);
   return;
}

/********************************************************************************
* print: Skriver ut ett enskilt tecken via seriell överföring.
*
*        - character: Det tecken som ska skrivas ut.
********************************************************************************/
void serial::print(const char character)
{
   while ((UCSR0A & (1 << UDRE0)) == 0);
   UDR0 = character;
   return;
}