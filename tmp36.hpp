/********************************************************************************
* tmp36.hpp: Innehåller drivrutiner för temperatursensor TMP36, som används för
*            mätning samt utskrift av rumstemperaturen. Den analoga insignalen
*            från TMP36 AD-omvandlas till en digital motsvarighet, som används
*            för att beräkna aktuell temperatur. Utskrift sker till en seriell
*            terminal via USART.
********************************************************************************/
#ifndef TMP36_HPP_
#define TMP36_HPP_

/* Inkluderingsdirektiv: */
#include "adc.hpp"
#include "serial.hpp"

/********************************************************************************
* tmp36: Klass för implementering av temperatursensor TMP36, som används för
*        mätning samt utskrift av rumstemperaturen. Vid avläsning AD-omvandlas
*        den analoga insignalen till en digital motsvarighet. Utefter detta
*        värde beräknas den analoga inspänningen Uin via nedanstående formel:
*
*        Uin = ADC_result / ADC_MAX * Vcc,
*
*        där ADC_result är den AD-omvandlade insignalen (0 - 1023),
*        ADC_MAX är högsta möjliga digitala signal (1023) och Vcc är
*        mikrodatorns matningsspänning (5 V).
*
*        Temperaturen T beräknas utefter detta värde via nedanstående formel:
*
*        T = 100 * Uin - 50,
*
*        där Uin utgör analog inspänning avläst från temperatursensor TMP36.
********************************************************************************/
class tmp36
{
private:
   adc adc_;                         /* AD-omvandlare, omvandlar analog insignal från TMP36. */
   static constexpr auto VCC_ = 5.0; /* 5 V matningsspänning. */

public:

   /********************************************************************************
   * tmp36: Defaultkonstruktor, initierar tomt objekt.
   ********************************************************************************/
   tmp36(void) { }

   /********************************************************************************
   * tmp36: Initierar pin ansluten till temperatursensor TMP36 för mätning samt 
   *        utskrift av rumstemperaturen. Seriell överföring initieras också med 
   *        en baud rate (överföringshastighet) på 9600 kbps.
   *
   *        - pin : Analog pin A0 - A5 som temperatursensorn är ansluten till.
   ********************************************************************************/
   tmp36(const uint8_t pin)
   {
      this->init(pin);
      return;
   }

   /********************************************************************************
   * init: Initierar pin ansluten till temperatursensor TMP36 för mätning samt 
   *       utskrift av rumstemperaturen. Seriell överföring initieras också med 
   *       en baud rate (överföringshastighet) på 9600 kbps.
   *
   *      - pin : Analog pin A0 - A5 som temperatursensorn är ansluten till.
   ********************************************************************************/
   void init(const uint8_t pin)
   {
      this->adc_.init(pin);
      serial::init();
      return;
   }

   /********************************************************************************
   * get_input_voltage: Returnerar inspänningen från angiven tempsensor genom att 
   *                    läsa av insignalen, omvandla till motsvarande digitala
   *                    värde och sedan beräkna motsvarande inspänning.
   *                    Värdet beräknas efter en matningsspänning på 5 V,
   ********************************************************************************/
   double get_input_voltage(void) const
   {
      return this->adc_.duty_cycle() * this->VCC_;
   }

   /********************************************************************************
   * get_temperature: Returnerar aktuell rumstemperatur via avläsning av angiven
   *                  temperatursensor TMP36.
   ********************************************************************************/
   double get_temperature(void) const
   {
      return 100 * this->get_input_voltage() - 50;
   }

   /********************************************************************************
   * print_temperature: Skriver ut aktuell rumstemperatur avläst av 
   *                    temperatursensor TMP36.
   ********************************************************************************/
   void print_temperature(void) const
   {
      serial::print("Temperature: ");
      serial::print(this->get_temperature());
      serial::print(" degrees Celcius.\n");
      return;
   }

   /********************************************************************************
   * print_voltage: Skriver ut inspänning avläst av temperatursensor TMP36 mätt i V.
   ********************************************************************************/
   void print_voltage(void) const
   {
      serial::print("Voltage: ");
      serial::print(this->get_input_voltage());
      serial::print(" V.\n");
      return;
   }
};

#endif /* TMP36_HPP_ */