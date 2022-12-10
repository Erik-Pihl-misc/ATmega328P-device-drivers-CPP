/********************************************************************************
* led.hpp: Inneh�ller drivrutiner f�r lysdioder och andra digitala utportar 
*          via klassen led.
********************************************************************************/
#ifndef LED_HPP_
#define LED_HPP_

/* Inkluderingsdirektiv: */
#include "misc.hpp"

/********************************************************************************
* led: Klass inneh�llande drivrutiner f�r lysdioder och andra digitala utportar.
********************************************************************************/
class led
{
private:
   uint8_t pin_ = 0;              /* Lysdiodens pin-nummer p� aktuell I/O-port. */
   volatile uint8_t* ddr_ = 0;    /* Pekare till datariktningsregister. */
   volatile uint8_t* output_ = 0; /* Pekare till dataregister (f�r t�ndning/sl�ckning). */
   volatile uint8_t* input_ = 0;  /* Pekare till pinregister (f�r l�sning av insignaler). */

public:

   /********************************************************************************
   * led: Defaultkonstruktor, initierar tomt objekt.
   ********************************************************************************/
   led(void) { }

   /********************************************************************************
   * led: Initierar ny lysdiod p� angiven pin.
   *
   *      - pin      : Lysdiodens pin-nummer p� Arduino Uno, exempelvis 8.
   *                   Alternativt kan motsvarande port-nummer p� ATmega328P
   *                   anges, exempelvis B0 f�r pin 8 eller D2 f�r pin 2.
   *      - start_val: Lysdiodens startv�rde (default = 0, dvs. sl�ckt).
   ********************************************************************************/
   led(const uint8_t pin,
       const uint8_t start_val = 0)
   {
      this->init(pin, start_val);
      return;
   }

   /********************************************************************************
   * ~led: Nollst�ller lysdiod samt motsvarande pin.
   ********************************************************************************/
   ~led(void)
   {
      this->clear();
      return;
   }

   /********************************************************************************
   * pin: Returnerar lysdiodens pin-nummer.
   ********************************************************************************/
   uint8_t pin(void) const
   {
      return this->pin_;
   }

   /********************************************************************************
   * enabled: Indikerar ifall lysdioden �r t�nd.
   ********************************************************************************/
   bool enabled(void) const
   {
      return *(this->input_) & (1 << this->pin_);
   }

   /********************************************************************************
   * clear: Nollst�ller lysdiod samt motsvarande pin.
   ********************************************************************************/
   void clear(void)
   {
      *(this->ddr_) &= ~(1 << this->pin_);
      *(this->output_) &= ~(1 << this->pin_);
      
      this->pin_ = 0;
      this->ddr_ = 0;
      this->output_ = 0;
      this->input_ = 0;
      return;
   }

   /********************************************************************************
   * init: Initierar ny lysdiod p� angiven pin.
   *
   *       - pin      : Lysdiodens pin-nummer p� Arduino Uno, exempelvis 8.
   *                    Alternativt kan motsvarande port-nummer p� ATmega328P
   *                    anges, exempelvis B0 f�r pin 8 eller D2 f�r pin 2.
   *       - start_val: Lysdiodens startv�rde (default = 0, dvs. sl�ckt).
   ********************************************************************************/
   void init(const uint8_t pin,
             const uint8_t start_val = 0)
   {
      if (pin >= 0 && pin <= 7)
      {
         this->pin_ = pin;
         this->ddr_ = &DDRD;
         this->output_ = &PORTD;
         this->input_ = &PIND;
      }
      else if (pin >= 8 && pin <= 13)
      {
         this->pin_ = pin - 8;
         this->ddr_ = &DDRB;
         this->output_ = &PORTB;
         this->input_ = &PINB;
      }
      else if (pin >= 14 && pin <= 19)
      {
         this->pin_ = pin - 14;
         this->ddr_ = &DDRC;
         this->output_ = &PORTC;
         this->input_ = &PINC;
      }

      *(this->ddr_) |= (1 << this->pin_);
      if (start_val) this->on();
      return;
   }

   /********************************************************************************
   * on: T�nder angiven lysdiod.
   ********************************************************************************/
   void on(void)
   {
      *(this->output_) |= (1 << this->pin_);
      return;
   }

   /********************************************************************************
   * off: Sl�cker angiven lysdiod.
   ********************************************************************************/
   void off(void)
   {
      *(this->output_) &= ~(1 << this->pin_);
      return;
   }

   /********************************************************************************
   * toggle: Togglar utsignalen p� angiven lysdiod. Om lysdioden �r sl�ckt vid
   *         anropet s� t�nds den. P� samma s�tt g�ller att om lysdioden �r t�nd
   *         vid anropet s� sl�cks den.
   ********************************************************************************/
   void toggle(void)
   {
      *(this->input_) = (1 << this->pin_);
      return;
   }

   /********************************************************************************
   * blink: Blinkar lysdiod en g�ng med angiven blinkhastighet.
   *
   *        - blink_speed_ms: Referens till blinkhastigheten m�tt i millisekunder.
   ********************************************************************************/
   void blink(const uint16_t& blink_speed_ms)
   {
      this->toggle();
      misc::delay_ms(blink_speed_ms);
      return;
   }
};

#endif /* LED_HPP_ */