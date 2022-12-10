/********************************************************************************
* eeprom.hpp: Inneh�ller drivrutiner f�r skrivning samt l�sning till och fr�n
*             EEPROM-minnet.
********************************************************************************/
#ifndef EEPROM_HPP_
#define EEPROM_HPP_

/* Inkluderingsdirektiv: */
#include "misc.hpp"
#include "word.hpp"

/********************************************************************************
* eeprom: Namnrymd inneh�llande drivrutiner f�r skrivning och l�sning till 
*         och fr�n EEPROM-minnet.
********************************************************************************/
namespace eeprom
{
   static constexpr auto ADDRESS_MIN = 0;    /* L�gsta adress i EEPROM-minnet. */
   static constexpr auto ADDRESS_MAX = 1023; /* H�gsta adress i EEPROM-minnet. */

   /********************************************************************************
   * write_byte: Skriver en byte best�ende av ett osignerat heltal till angiven
   *             adress i EEPROM-minnet. Vid lyckad skrivning returneras 0,
   *             annars returneras felkod 1.
   *
   *             - address: Adressen i EEPROM-minnet som angiven data ska lagras p�.
   *             - data   : Datan som ska skrivas.
   ********************************************************************************/
   int write_byte(const uint16_t address, 
                  const uint8_t data);

   /********************************************************************************
   * write_word: Skriver tv� byte best�ende av ett osignerat heltal till angiven
   *             samt efterf�ljande adress i EEPROM-minnet. Vid lyckad skrivning
   *             returneras 0, annars returneras felkod 1.
   *
   *             - address_low: Den l�gre adressen i EEPROM-minnet som angiven
   *                            data ska lagras p�.
   *             - data       : Datan som ska skrivas.
   ********************************************************************************/
   int write_word(const uint16_t address_low, 
                  const uint16_t data);

   /********************************************************************************
   * read_byte: L�ser en byte p� angiven adress i EEPROM-minnet och returnerar
   *            detta som ett osignerat heltal. Vid misslyckad l�sning returneras 0.
   *
   *            - address: Adressen i EEPROM-minnet som ska l�sas av.
   ********************************************************************************/
   uint8_t read_byte(const uint16_t address);

   /********************************************************************************
   * read_word: L�ser tv� byte p� angiven samt efterf�ljande adress i EEPROM-minnet
   *            och returnerar detta som ett osignerat heltal.  Vid misslyckad 
   *            l�sning returneras 0.
   *
   *            - address_low: Den l�gre adressen i EEPROM-minnet som ska l�sas av.
   ********************************************************************************/
   uint16_t read_word(const uint16_t address_low);
}

#endif /* EEPROM_HPP_ */