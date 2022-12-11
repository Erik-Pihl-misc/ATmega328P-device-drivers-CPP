/********************************************************************************
* button.hpp: Innehåller funktionalitet för enkel implementering av tryckknappar
*             via klassen button samt associerade funktioner. Denna klass
*             fungerar också utmärkt för andra digitala inportar där insignalen
*             ska kunna läsas av samt avbrott ska kunna genereras vid ett
*             godtyckligt event.
********************************************************************************/
#ifndef BUTTON_HPP_
#define BUTTON_HPP_

/* Inkluderingsdirektiv: */
#include "misc.hpp"

/********************************************************************************
* button: Klass för implementering av tryckknappar och andra digitala inportar.
*         PCI-avbrott kan aktiveras på aktuell pin. Därmed får eventdetektering
*         implementeras av användaren, då PCI-avbrott inte möjliggör kontroll
*         av vilken flank som avbrott ska ske på.
********************************************************************************/
class button
{
private:
   uint8_t pin_ = 0x00;           /* Tryckknappens pin-nummer på aktuell I/O-port. */
   volatile uint8_t* pullup_ = 0; /* Pekare till dataregister (för intern pullup-resistor). */
   volatile uint8_t* input_ = 0;  /* Pekare till pinregister (för läsning av insignaler). */
   volatile uint8_t* pcmsk_ = 0;  /* Pekare till maskregister för aktivering av PCI-avbrott. */
   volatile uint8_t pcint_ = 0;   /* bit för aktivering av avbrottsvektor på aktuell I/O-port. */

public:

   /********************************************************************************
   * button: Defaultkonstruktor, initierar tomt objekt.
   ********************************************************************************/
   button(void) { }

   /********************************************************************************
   * button: Initierar ny tryckknapp på angiven pin.
   *
   *         - pin : Tryckknappens pin-nummer på Arduino Uno, exempelvis 13.
   *                 Alternativt kan motsvarande port-nummer på ATmega328P anges,
   *                 exempelvis B5 för pin 13 eller D3 för pin 3.
   ********************************************************************************/
   button(const uint8_t pin)
   {
      this->init(pin); 
      return;
   }

   /********************************************************************************
   * ~button: Nollställer tryckknapp samt motsvarande pin.
   ********************************************************************************/
   ~button(void)
   {
      this->clear();
      return;
   }

   /********************************************************************************
   * button: Kopieringskonstruktor raderad.
   ********************************************************************************/    
   button(button&) = delete;    

   /********************************************************************************
   * button: Tilldelningsoperator raderad.
   ********************************************************************************/
   button& operator= (button&) = delete;

   /********************************************************************************
   * pin: Returnerar tryckknappens pin-nummer.
   ********************************************************************************/
   uint8_t pin(void) const
   {
      return this->pin_;
   }

   /********************************************************************************
   * clear: Nollställer lysdiod samt motsvarande pin.
   ********************************************************************************/
   void clear(void)
   {
      this->disable_interrupt();
      *(this->pullup_) &= ~(1 << this->pin_);

      this->pin_ = 0;
      this->pullup_ = 0;
      this->input_ = 0;
      this->pcmsk_ = 0;
      this->pcint_ = 0;
      return;
   }

   /********************************************************************************
   * interrupt_enabled: Indikerar ifall PCI-avbrott är aktiverat på knappens pin.
   ********************************************************************************/
   bool interrupt_enabled(void) const
   {
      return *(this->pcmsk_) & (1 << this->pin_);
   }

   /********************************************************************************
   * init: Initierar ny tryckknapp på angiven pin.
   *
   *       - pin : Tryckknappens pin-nummer på Arduino Uno, exempelvis 13.
   *               Alternativt kan motsvarande port-nummer på ATmega328P anges,
   *               exempelvis B5 för pin 13 eller D3 för pin 3.
   ********************************************************************************/
   void init(const uint8_t pin)
   {
      if (pin >= 0 && pin <= 7)
      {
         this->pin_ = pin;
         this->pullup_ = &PORTD;
         this->input_ = &PIND;
         this->pcmsk_ = &PCMSK2;
         this->pcint_ = PCIE2;
      }
      else if (pin >= 8 && pin <= 13)
      {
         this->pin_ = pin - 8;
         this->pullup_ = &PORTB;
         this->input_ = &PINB;
         this->pcmsk_ = &PCMSK0;
         this->pcint_ = PCIE0;
      }
      else if (pin >= 14 && pin <= 19)
      {
         this->pin_ = pin - 14;
         this->pullup_ = &PORTC;
         this->input_ = &PINC;
         this->pcmsk_ = &PCMSK1;
         this->pcint_ = PCIE1;
      }
      
      *(this->pullup_) |= (1 << this->pin_);
      return;
   }

   /********************************************************************************
   * is_pressed: Läser av tryckknappens pin och indikerar ifall denna är nedtryckt. 
   *             I så fall returneras true, annars false.
   ********************************************************************************/
   bool is_pressed(void) const
   {
      return *(this->input_) & (1 << this->pin_);
   }
   
   /********************************************************************************
   * enable_interrupt: Aktiverar PCI-avbrott på angiven tryckknapp så att event på
   *                   tryckknappens pin medför avbrott, både på stigande och 
   *                   fallande flank. Kontroll av flank måste skötas av användaren, 
   *                   förslagsvis via avläsning i motsvarande avbrottsrutin.
   *
   *                   Nedan visas sambandet mellan använd I/O-port samt
   *                   avbrottsvektorn för motsvarande avbrottsrutin:
   *
   *                   I/O-port     pin (Arduino Uno)     Avbrottsvektor
   *                      B              8 - 13             PCINT0_vect
   *                      C             A0 - A5             PCINT1_vect
   *                      D              0 - 7              PCINT2_vect
   ********************************************************************************/
   void enable_interrupt(void)
   {
      asm("SEI");
      PCICR |= (1 << this->pcint_);
      *(this->pcmsk_) |= (1 << this->pin_);
      return;
   }

   /********************************************************************************
   * disable_interrupt: Inaktiverar PCI-avbrott på angiven tryckknapp.
   ********************************************************************************/
   void disable_interrupt(void)
   {
      *(this->pcmsk_) &= ~(1 << this->pin_);
      return;
   }

   /********************************************************************************
   * toggle_interrupt: Togglar aktivering av PCI-avbrott på angiven tryckknapp.
   *                   Om avbrott är aktiverat vid anrop så sker inaktivering.
   *                   På samma sätt gäller att om avbrott är inaktiverat vid anrop
   *                   så sker aktivering.
   ********************************************************************************/
   void toggle_interrupt(void)
   {
      if (this->interrupt_enabled())
      {
         this->disable_interrupt();
      }
      else
      {
         this->enable_interrupt();
      }

      return;
   }

};

#endif /* BUTTON_HPP_ */