/********************************************************************************
* led_vector.hpp: Inneh�ller funktionalitet f�r implementering av vektorer f�r
*                 f�r lagring av pekare till multipla lysioder via klassen
*                 led_vector samt associerade funktioner, vilket m�jligg�r enkel
*                 styrning av lysdioder och andra digitala utportar.
*
*                 Lysdioder kan l�ggas till dynamiskt eller genom att en pekare
*                 till en statisk array inneh�llande lysdiodspekare passeras.
********************************************************************************/
#ifndef LED_VECTOR_HPP_
#define LED_VECTOR_HPP_

/* Inkluderingsdirektiv: */
#include "misc.hpp"
#include "vector.hpp"
#include "led.hpp"

/********************************************************************************
* led_vector: Dynamisk vektor f�r lagring och styrning av led-objekt, vilket
*             kan utg�ras av lysdioder eller andra digitala utportar. Klassen
*             vector �rvs f�r implementering av vektor-operationer, s�som
*             omallokering, push, pop samt frig�rande av minne.
********************************************************************************/
class led_vector : public vector<led*>
{
public:

   /********************************************************************************
   * led_vector: Defaultkonstruktor, initierar ny tom vektor.
   ********************************************************************************/
   led_vector(void) { }

    /********************************************************************************
   * led_vector: Initierar ny vektor med angiven storlek.
   *
   *             - size: Vektorns storlek, dvs. antalet lysdioder den rymmer.
   ********************************************************************************/
   led_vector(const size_t size)
   {
      this->resize(size);
      return;
   }

   /********************************************************************************
   * led_vector: Initierar vektor med en array inneh�llande pekare till angivet
   *             antal lysdioder.
   *      
   *             - leds    : Pekare till array inneh�llande pekare till lysdioder.
   *             - num_leds: Antalet refererade lysdioder i arrayen.
   ********************************************************************************/
   led_vector(led** leds, 
              const size_t num_leds)
   {
      this->init(leds, num_leds);
      return;
   }

   /********************************************************************************
   * ~led_vector: Destruktor, frig�r allokerat minne innan vektorn g�r ur scope.
   ********************************************************************************/
   ~led_vector(void) 
   { 
      this->clear(); 
      return;
   }

   /********************************************************************************
   * leds: Returnerar en pekare till f�ltet inneh�llande lysdioderna. 
   ********************************************************************************/
   struct led** leds(void) const
   {
      return this->data_;
   }

   /********************************************************************************
   * init: Initierar vektor med en array inneh�llande pekare till angivet antal
   *       lysdioder.
   *      
   *       - leds    : Pekare till array inneh�llande pekare till lysdioder.
   *       - num_leds: Antalet refererade lysdioder i arrayen.
   ********************************************************************************/
   void init(led** leds, 
             const size_t num_leds)
   {
      this->data_ = leds;
      this->size_ = num_leds;
      return;
   }

   /********************************************************************************
   * on: T�nder samtliga lysdioder lagrade i angiven vektor.
   ********************************************************************************/
   void on(void)
   {
      for (auto& i : *this)
      {
         i->on();
      }
      return;
   }

   /********************************************************************************
   * off: Sl�cker samtliga lysdioder lagrade i angiven vektor.
   ********************************************************************************/
   void off(void)
   {
      for (auto& i : *this)
      {
         i->off();
      }
      return;
   }

   /********************************************************************************
   * toggle: Togglar samtliga lysdioder lagrade i angiven vektor.
   ********************************************************************************/
   void toggle(void)
   {
      for (auto& i : *this)
      {
         i->toggle();
      }
      return;
   }

   /********************************************************************************
   * blink_collectively: Genomf�r kollektiv (synkroniserad) blinkning av samtliga 
   *                     lysdioder lagrade i angiven vektor.
   *
   *                     - blink_speed_ms: Referens till lysdiodernas
   *                                       blinkhastighet m�tt i millisekunder.
   ********************************************************************************/
   void blink_collectively(const uint16_t& blink_speed_ms)
   {
      this->on();
      misc::delay_ms(blink_speed_ms);
      this->off();
      misc::delay_ms(blink_speed_ms);
      return;
   }

   /********************************************************************************
   * blink_sequentially: Genomf�r sekventiell blinkning av samtliga lysdioder 
   *                     lagrade i angiven vektor. D�rmed blinkar lysdioderna 
   *                     i en sekvens en efter en.
   *
   *                     - blink_speed_ms: Referens till lysdiodernas
   *                                       blinkhastighet m�tt i millisekunder.
   ********************************************************************************/
   void blink_sequentially(const uint16_t& blink_speed_ms)
   {
      this->off();

      for (auto& i : *this)
      {
         i->on();
         misc::delay_ms(blink_speed_ms);
         i->off();
      }

      return;
   }
};

#endif /* LED_VECTOR_HPP_ */