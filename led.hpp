/********************************************************************************
* led.hpp: Innehåller drivrutiner för lysdioder och andra digitala utportar 
*          via klassen led.
********************************************************************************/
#ifndef LED_HPP_
#define LED_HPP_

/* Inkluderingsdirektiv: */
#include "misc.hpp"

/********************************************************************************
* led: Klass innehållande drivrutiner för lysdioder och andra digitala utportar.
********************************************************************************/
class led
{
private:
   uint8_t pin_ = 0;              /* Lysdiodens pin-nummer på aktuell I/O-port. */
   volatile uint8_t* ddr_ = 0;    /* Pekare till datariktningsregister. */
   volatile uint8_t* output_ = 0; /* Pekare till dataregister (för tändning/släckning). */
   volatile uint8_t* input_ = 0;  /* Pekare till pinregister (för läsning av insignaler). */

public:

   /********************************************************************************
   * led: Defaultkonstruktor, initierar tomt objekt.
   ********************************************************************************/
   led(void) { }

   /********************************************************************************
   * led: Initierar ny lysdiod på angiven pin.
   *
   *      - pin      : Lysdiodens pin-nummer på Arduino Uno, exempelvis 8.
   *                   Alternativt kan motsvarande port-nummer på ATmega328P
   *                   anges, exempelvis B0 för pin 8 eller D2 för pin 2.
   *      - start_val: Lysdiodens startvärde (default = 0, dvs. släckt).
   ********************************************************************************/
   led(const uint8_t pin,
       const uint8_t start_val = 0)
   {
      this->init(pin, start_val);
      return;
   }

   /********************************************************************************
   * ~led: Nollställer lysdiod samt motsvarande pin.
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
   * enabled: Indikerar ifall lysdioden är tänd.
   ********************************************************************************/
   bool enabled(void) const
   {
      return *(this->input_) & (1 << this->pin_);
   }

   /********************************************************************************
   * clear: Nollställer lysdiod samt motsvarande pin.
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
   * init: Initierar ny lysdiod på angiven pin.
   *
   *       - pin      : Lysdiodens pin-nummer på Arduino Uno, exempelvis 8.
   *                    Alternativt kan motsvarande port-nummer på ATmega328P
   *                    anges, exempelvis B0 för pin 8 eller D2 för pin 2.
   *       - start_val: Lysdiodens startvärde (default = 0, dvs. släckt).
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
   * on: Tänder angiven lysdiod.
   ********************************************************************************/
   void on(void)
   {
      *(this->output_) |= (1 << this->pin_);
      return;
   }

   /********************************************************************************
   * off: Släcker angiven lysdiod.
   ********************************************************************************/
   void off(void)
   {
      *(this->output_) &= ~(1 << this->pin_);
      return;
   }

   /********************************************************************************
   * toggle: Togglar utsignalen på angiven lysdiod. Om lysdioden är släckt vid
   *         anropet så tänds den. På samma sätt gäller att om lysdioden är tänd
   *         vid anropet så släcks den.
   ********************************************************************************/
   void toggle(void)
   {
      *(this->input_) = (1 << this->pin_);
      return;
   }

   /********************************************************************************
   * blink: Blinkar lysdiod en gång med angiven blinkhastighet.
   *
   *        - blink_speed_ms: Referens till blinkhastigheten mätt i millisekunder.
   ********************************************************************************/
   void blink(const uint16_t& blink_speed_ms)
   {
      this->toggle();
      misc::delay_ms(blink_speed_ms);
      return;
   }
};

#endif /* LED_HPP_ */