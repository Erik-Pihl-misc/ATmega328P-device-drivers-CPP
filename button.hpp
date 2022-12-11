/********************************************************************************
* button.hpp: Inneh�ller funktionalitet f�r enkel implementering av tryckknappar
*             via klassen button samt associerade funktioner. Denna klass
*             fungerar ocks� utm�rkt f�r andra digitala inportar d�r insignalen
*             ska kunna l�sas av samt avbrott ska kunna genereras vid ett
*             godtyckligt event.
********************************************************************************/
#ifndef BUTTON_HPP_
#define BUTTON_HPP_

/* Inkluderingsdirektiv: */
#include "misc.hpp"

/********************************************************************************
* button: Klass f�r implementering av tryckknappar och andra digitala inportar.
*         PCI-avbrott kan aktiveras p� aktuell pin. D�rmed f�r eventdetektering
*         implementeras av anv�ndaren, d� PCI-avbrott inte m�jligg�r kontroll
*         av vilken flank som avbrott ska ske p�.
********************************************************************************/
class button
{
private:
   uint8_t pin_ = 0x00;           /* Tryckknappens pin-nummer p� aktuell I/O-port. */
   volatile uint8_t* pullup_ = 0; /* Pekare till dataregister (f�r intern pullup-resistor). */
   volatile uint8_t* input_ = 0;  /* Pekare till pinregister (f�r l�sning av insignaler). */
   volatile uint8_t* pcmsk_ = 0;  /* Pekare till maskregister f�r aktivering av PCI-avbrott. */
   volatile uint8_t pcint_ = 0;   /* bit f�r aktivering av avbrottsvektor p� aktuell I/O-port. */

public:

   /********************************************************************************
   * button: Defaultkonstruktor, initierar tomt objekt.
   ********************************************************************************/
   button(void) { }

   /********************************************************************************
   * button: Initierar ny tryckknapp p� angiven pin.
   *
   *         - pin : Tryckknappens pin-nummer p� Arduino Uno, exempelvis 13.
   *                 Alternativt kan motsvarande port-nummer p� ATmega328P anges,
   *                 exempelvis B5 f�r pin 13 eller D3 f�r pin 3.
   ********************************************************************************/
   button(const uint8_t pin)
   {
      this->init(pin); 
      return;
   }

   /********************************************************************************
   * ~button: Nollst�ller tryckknapp samt motsvarande pin.
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
   * clear: Nollst�ller lysdiod samt motsvarande pin.
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
   * interrupt_enabled: Indikerar ifall PCI-avbrott �r aktiverat p� knappens pin.
   ********************************************************************************/
   bool interrupt_enabled(void) const
   {
      return *(this->pcmsk_) & (1 << this->pin_);
   }

   /********************************************************************************
   * init: Initierar ny tryckknapp p� angiven pin.
   *
   *       - pin : Tryckknappens pin-nummer p� Arduino Uno, exempelvis 13.
   *               Alternativt kan motsvarande port-nummer p� ATmega328P anges,
   *               exempelvis B5 f�r pin 13 eller D3 f�r pin 3.
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
   * is_pressed: L�ser av tryckknappens pin och indikerar ifall denna �r nedtryckt. 
   *             I s� fall returneras true, annars false.
   ********************************************************************************/
   bool is_pressed(void) const
   {
      return *(this->input_) & (1 << this->pin_);
   }
   
   /********************************************************************************
   * enable_interrupt: Aktiverar PCI-avbrott p� angiven tryckknapp s� att event p�
   *                   tryckknappens pin medf�r avbrott, b�de p� stigande och 
   *                   fallande flank. Kontroll av flank m�ste sk�tas av anv�ndaren, 
   *                   f�rslagsvis via avl�sning i motsvarande avbrottsrutin.
   *
   *                   Nedan visas sambandet mellan anv�nd I/O-port samt
   *                   avbrottsvektorn f�r motsvarande avbrottsrutin:
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
   * disable_interrupt: Inaktiverar PCI-avbrott p� angiven tryckknapp.
   ********************************************************************************/
   void disable_interrupt(void)
   {
      *(this->pcmsk_) &= ~(1 << this->pin_);
      return;
   }

   /********************************************************************************
   * toggle_interrupt: Togglar aktivering av PCI-avbrott p� angiven tryckknapp.
   *                   Om avbrott �r aktiverat vid anrop s� sker inaktivering.
   *                   P� samma s�tt g�ller att om avbrott �r inaktiverat vid anrop
   *                   s� sker aktivering.
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