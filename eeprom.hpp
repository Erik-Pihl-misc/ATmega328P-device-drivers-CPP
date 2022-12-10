/********************************************************************************
* eeprom.hpp: Innehåller drivrutiner för skrivning samt läsning till och från
*             EEPROM-minnet.
********************************************************************************/
#ifndef EEPROM_HPP_
#define EEPROM_HPP_

/* Inkluderingsdirektiv: */
#include "misc.hpp"
#include "word.hpp"

/********************************************************************************
* eeprom: Namnrymd innehållande drivrutiner för skrivning och läsning till 
*         och från EEPROM-minnet.
********************************************************************************/
namespace eeprom
{
   static constexpr auto ADDRESS_MIN = 0;    /* Lägsta adress i EEPROM-minnet. */
   static constexpr auto ADDRESS_MAX = 1023; /* Högsta adress i EEPROM-minnet. */

   /********************************************************************************
   * write_byte: Skriver en byte bestående av ett osignerat heltal till angiven
   *             adress i EEPROM-minnet. Vid lyckad skrivning returneras 0,
   *             annars returneras felkod 1.
   *
   *             - address: Adressen i EEPROM-minnet som angiven data ska lagras på.
   *             - data   : Datan som ska skrivas.
   ********************************************************************************/
   int write_byte(const uint16_t address, 
                  const uint8_t data);

   /********************************************************************************
   * write_word: Skriver två byte bestående av ett osignerat heltal till angiven
   *             samt efterföljande adress i EEPROM-minnet. Vid lyckad skrivning
   *             returneras 0, annars returneras felkod 1.
   *
   *             - address_low: Den lägre adressen i EEPROM-minnet som angiven
   *                            data ska lagras på.
   *             - data       : Datan som ska skrivas.
   ********************************************************************************/
   int write_word(const uint16_t address_low, 
                  const uint16_t data);

   /********************************************************************************
   * read_byte: Läser en byte på angiven adress i EEPROM-minnet och returnerar
   *            detta som ett osignerat heltal. Vid misslyckad läsning returneras 0.
   *
   *            - address: Adressen i EEPROM-minnet som ska läsas av.
   ********************************************************************************/
   uint8_t read_byte(const uint16_t address);

   /********************************************************************************
   * read_word: Läser två byte på angiven samt efterföljande adress i EEPROM-minnet
   *            och returnerar detta som ett osignerat heltal.  Vid misslyckad 
   *            läsning returneras 0.
   *
   *            - address_low: Den lägre adressen i EEPROM-minnet som ska läsas av.
   ********************************************************************************/
   uint16_t read_word(const uint16_t address_low);
}

#endif /* EEPROM_HPP_ */