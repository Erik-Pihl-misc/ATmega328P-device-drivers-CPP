/********************************************************************************
* eeprom.cpp: Innehåller drivrutiner för skrivning och läsning till och från
*             EEPROM-minnet.
********************************************************************************/
#include "eeprom.hpp"

/********************************************************************************
* write_byte: Skriver en byte bestående av ett osignerat heltal till angiven
*             adress i EEPROM-minnet. Vid lyckad skrivning returneras 0,
*             annars returneras felkod 1.
*
*             - address: Adressen i EEPROM-minnet som angiven data ska lagras på.
*             - data   : Datan som ska skrivas.
********************************************************************************/
int eeprom::write_byte(const uint16_t address, 
                       const uint8_t data)
{ 
   if (address > ADDRESS_MAX) return 1;
   while (EECR & (1 << EEPE));
   EEAR = address;
   EEDR = data;

   asm("CLI");
   EECR |= (1 << EEMPE);
   EECR |= (1 << EEPE);
   asm("SEI");
   return 0;
}

/********************************************************************************
* write_word: Skriver två byte bestående av ett osignerat heltal till angiven
*             samt efterföljande adress i EEPROM-minnet. Vid lyckad skrivning
*             returneras 0, annars returneras felkod 1.
*
*             - address_low: Den lägre adressen i EEPROM-minnet som angiven
*                            data ska lagras på.
*             - data       : Datan som ska skrivas.
********************************************************************************/
int eeprom::write_word(const uint16_t address_low, 
                       const uint16_t data)
{
   if (address_low > ADDRESS_MAX - 1) return 1;
   word number(data);
   eeprom::write_byte(address_low, number.segmented.low);
   eeprom::write_byte(address_low, number.segmented.high);
   return 0;
}

/********************************************************************************
* read_byte: Läser en byte på angiven adress i EEPROM-minnet och returnerar
*            detta som ett osignerat heltal. Vid misslyckad läsning returneras 0.
*
*            - address: Adressen i EEPROM-minnet som ska läsas av.
********************************************************************************/
uint8_t eeprom::read_byte(const uint16_t address)
{
   if (address > ADDRESS_MAX) return 0;
   while (EECR & (1 << EEPE));
   EEAR = address;
   EECR |= (1 << EERE);
   return EEDR;
}

/********************************************************************************
* read_word: Läser två byte på angiven samt efterföljande adress i EEPROM-minnet
*            och returnerar detta som ett osignerat heltal.  Vid misslyckad 
*            läsning returneras 0.
*
*            - address_low: Den lägre adressen i EEPROM-minnet som ska läsas av.
********************************************************************************/
uint16_t eeprom::read_word(const uint16_t address_low)
{
   if (address_low > ADDRESS_MAX - 1) return 0;
   word number;
   number.segmented.low = eeprom::read_byte(address_low);
   number.segmented.high = eeprom::read_byte(address_low + 1);
   return number.whole;
}