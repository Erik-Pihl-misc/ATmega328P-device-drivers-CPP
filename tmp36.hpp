/********************************************************************************
* tmp36.hpp: Inneh�ller drivrutiner f�r temperatursensor TMP36, som anv�nds f�r
*            m�tning samt utskrift av rumstemperaturen. Den analoga insignalen
*            fr�n TMP36 AD-omvandlas till en digital motsvarighet, som anv�nds
*            f�r att ber�kna aktuell temperatur. Utskrift sker till en seriell
*            terminal via USART.
********************************************************************************/
#ifndef TMP36_HPP_
#define TMP36_HPP_

/* Inkluderingsdirektiv: */
#include "adc.hpp"
#include "serial.hpp"

/********************************************************************************
* tmp36: Klass f�r implementering av temperatursensor TMP36, som anv�nds f�r
*        m�tning samt utskrift av rumstemperaturen. Vid avl�sning AD-omvandlas
*        den analoga insignalen till en digital motsvarighet. Utefter detta
*        v�rde ber�knas den analoga insp�nningen Uin via nedanst�ende formel:
*
*        Uin = ADC_result / ADC_MAX * Vcc,
*
*        d�r ADC_result �r den AD-omvandlade insignalen (0 - 1023),
*        ADC_MAX �r h�gsta m�jliga digitala signal (1023) och Vcc �r
*        mikrodatorns matningssp�nning (5 V).
*
*        Temperaturen T ber�knas utefter detta v�rde via nedanst�ende formel:
*
*        T = 100 * Uin - 50,
*
*        d�r Uin utg�r analog insp�nning avl�st fr�n temperatursensor TMP36.
********************************************************************************/
class tmp36
{
private:
   adc adc_;                         /* AD-omvandlare, omvandlar analog insignal fr�n TMP36. */
   static constexpr auto VCC_ = 5.0; /* 5 V matningssp�nning. */

public:

   /********************************************************************************
   * tmp36: Defaultkonstruktor, initierar tomt objekt.
   ********************************************************************************/
   tmp36(void) { }

   /********************************************************************************
   * tmp36: Initierar pin ansluten till temperatursensor TMP36 f�r m�tning samt 
   *        utskrift av rumstemperaturen. Seriell �verf�ring initieras ocks� med 
   *        en baud rate (�verf�ringshastighet) p� 9600 kbps.
   *
   *        - pin : Analog pin A0 - A5 som temperatursensorn �r ansluten till.
   ********************************************************************************/
   tmp36(const uint8_t pin)
   {
      this->init(pin);
      return;
   }

   /********************************************************************************
   * init: Initierar pin ansluten till temperatursensor TMP36 f�r m�tning samt 
   *       utskrift av rumstemperaturen. Seriell �verf�ring initieras ocks� med 
   *       en baud rate (�verf�ringshastighet) p� 9600 kbps.
   *
   *      - pin : Analog pin A0 - A5 som temperatursensorn �r ansluten till.
   ********************************************************************************/
   void init(const uint8_t pin)
   {
      this->adc_.init(pin);
      serial::init();
      return;
   }

   /********************************************************************************
   * get_input_voltage: Returnerar insp�nningen fr�n angiven tempsensor genom att 
   *                    l�sa av insignalen, omvandla till motsvarande digitala
   *                    v�rde och sedan ber�kna motsvarande insp�nning.
   *                    V�rdet ber�knas efter en matningssp�nning p� 5 V,
   ********************************************************************************/
   double get_input_voltage(void) const
   {
      return this->adc_.duty_cycle() * this->VCC_;
   }

   /********************************************************************************
   * get_temperature: Returnerar aktuell rumstemperatur via avl�sning av angiven
   *                  temperatursensor TMP36.
   ********************************************************************************/
   double get_temperature(void) const
   {
      return 100 * this->get_input_voltage() - 50;
   }

   /********************************************************************************
   * print_temperature: Skriver ut aktuell rumstemperatur avl�st av 
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
   * print_voltage: Skriver ut insp�nning avl�st av temperatursensor TMP36 m�tt i V.
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