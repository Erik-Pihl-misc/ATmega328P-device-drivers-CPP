/********************************************************************************
* pwm.hpp: Inneh�ller drivrutiner f�r PWM-styrning av en godtycklig utenhet,
*          s�som en eller flera lysdioder.
********************************************************************************/
#ifndef PWM_HPP_
#define PWM_HPP_

/* Inkluderingsdirektiv: */
#include "misc.hpp"
#include "adc.hpp"

/********************************************************************************
* pwm: Klass f�r PWM-kontrollers, som m�jligg�r PWM-styrning av en godtycklig
*      utenhet, exempelvis en eller flera lysdioder implementerat via ett
*      led-objekt eller en vektor inneh�llande pekare till multipla led-objekt.
*      PWM-styrning kan ske via en analog insignal s�som en potentiometer eller
*      genom att direkt v�lja duty cycle.
********************************************************************************/
template<class T>
class pwm
{
private:
   adc input_;                              /* Analog inenhet, s�som en potentiometer. */
   uint16_t period_us_ = 0;                 /* Periodtid f�r PWM m�tt i mikrosekunder. */
   T* output_ = nullptr;                    /* Pekare till ansluten utenhet. */
   void (T::*output_high_)(void) = nullptr; /* Pekare till funktion f�r att t�nda ansluten utenhet. */
   void (T::*output_low_)(void) = nullptr;  /* Pekare till funktion f�r att sl�cka ansluten utenhet. */
   bool enabled_ = true;                    /* Enable-signal f�r kontroll av PWM-generering. */

public:
   /********************************************************************************
   * pwm: Defaultkonstruktor, initierar tomt objekt.
   ********************************************************************************/
   pwm(void) { }

   /********************************************************************************
   * pwm: Initierar PWM-kontroller f�r PWM-styrning av angiven utenhet via
   *      angiven analog insignal. Som default �r PWM-styrning aktiverat med
   *      en periodtid p� 1000 us.
   *
   *      - input_pin  : Pekare till analog pin som utg�r insignal.
   *      - output     : Pekare till utenheten som ska styras med PWM.
   *      - output_high: Funktionspekare f�r aktivering av angiven utenhet.
   *      - output_low : Funktionspekare f�r inaktivering av angiven utenhet.
   *      - period_us  : Periodtid f�r PWM i mikrosekunder (default = 1000 us).
   ********************************************************************************/
   pwm(uint8_t input_pin,
       T* output,
       void (T::*output_high)(void),
       void (T::*output_low)(void),
       const uint16_t period_us = 1000)
   {
      this->init(input_pin, output, output_high, output_low, period_us);
      return;
   }

   /********************************************************************************
   * ~pwm: Nollst�ller angiven PWM-kontroller innan radering.
   ********************************************************************************/
   ~pwm(void)
   {
      this->clear();
      return;
   }

   /********************************************************************************
   * pwm: Kopieringskonstruktor raderad.
   ********************************************************************************/
   pwm(pwm&) = delete;       

   /********************************************************************************
   * pwm: Tilldelningsoperator raderad.
   ********************************************************************************/
   pwm& operator= (pwm&) = delete;

   /********************************************************************************
   * is_enabled: Indikerar ifall PWM-kontrollern �r aktiverad eller inte.
   ********************************************************************************/
   bool enabled(void) const
   {
      return this->enabled_;
   }

   /********************************************************************************
   * period_us: Returnerar aktuell periodtid f�r PWM m�tt i millusekunder.
   ********************************************************************************/
   uint16_t period_us(void) const
   {
      return this->period_us_;
   }

   /********************************************************************************
   * init: Initierar PWM-kontroller f�r PWM-styrning av angiven utenhet via
   *       angiven analog insignal. Som default �r PWM-styrning aktiverat med
   *       en periodtid p� 1000 us.
   *
   *       - input_pin  : Pekare till analog pin som utg�r insignal.
   *       - output     : Pekare till utenheten som ska styras med PWM.
   *       - output_high: Funktionspekare f�r aktivering av angiven utenhet.
   *       - output_low : Funktionspekare f�r inaktivering av angiven utenhet.
   *       - period_us  : Periodtid f�r PWM i mikrosekunder (default = 1000 us).
   ********************************************************************************/
   void init(uint8_t input_pin,
             T* output,
             void (T::*output_high)(void),
             void (T::*output_low)(void),
             const uint16_t period_us = 1000)
   {
      this->input_.init(input_pin);
      this->output_ = output;
      this->output_high_ = output_high;
      this->output_low_ = output_low;
      this->period_us_ = period_us;
      return;
   }

   /********************************************************************************
   * clear: Nollst�ller angiven PWM-kontroller.
   ********************************************************************************/
   void clear(void)
   {
      this->output_ = nullptr;
      this->output_high_ = nullptr;
      this->output_low_ = nullptr;
      this->enabled_ = false;
      return;
   }

   /********************************************************************************
   * enable: Aktiverar angiven PWM-kontroller s� att ansluten utenhet kan styras
   *         via PWM-generering.
   ********************************************************************************/
   void enable(void)
   {
      this->enabled_ = true;
      return;
   }

   /********************************************************************************
   * disable: Inaktiverar angiven PWM-kontroller, vilket medf�r att PWM-styrning
   *          av ansluten utenhet inte kan genomf�ras.
   ********************************************************************************/
   void disable(void)
   {
      this->enabled_ = false;
      (this->output_->*this->output_low_)();
      return;
   }

   /********************************************************************************
   * toggle: Togglar aktivering av angiven PWM-kontroller.
   ********************************************************************************/
   void toggle(void)
   {
      if (this->enabled_)
      {
         this->disable();
      }
      else
      {
         this->enable();
      }
      return;
   }

   /********************************************************************************
   * run: K�r angiven PWM-kontroller under en period och styr ansluten utenhet
   *      via avl�sning av ansluten analog insignal, f�rutsatt att PWM-kontrollern 
   *      �r aktiverad.
   ********************************************************************************/
   void run(void)
   {
      if (!this->enabled_) return;
      this->input_.get_pwm_values(this->period_us_);

      (this->output_->*this->output_high_)();
      misc::delay_us(this->input_.pwm_on_us());
      (this->output_->*this->output_low_)();
      misc::delay_us(this->input_.pwm_off_us());
      return;
   }

   /********************************************************************************
   * run_with_duty_cycle: K�r angiven PWM-kontroller under en period och styr
   *                      ansluten utenhet med angiven duty cycle, f�rutsatt att
   *                      PWM-kontrollern �r aktiverad. Duty cycle m�ste anges
   *                      som ett flyttal mellan 0 - 1, vilket motsvarar 0 - 100 %.
   *
   *                      - duty_cycle: Duty cycle, allts� andelen av aktuell
   *                                    periodtid som ansluten utenhet ska vara
   *                                    aktiverad, mellan 0 - 1.
   ********************************************************************************/
   void run_with_duty_cycle(const double duty_cycle)
   {
      if (!this->enabled_ || duty_cycle < 0 || duty_cycle > 1) return;
      const auto on_time = static_cast<uint16_t>(this->period_us_ * duty_cycle + 0.5); 
      const auto off_time = this->period_us_ - on_time;

      (this->output_->*this->output_high_)();
      misc::delay_us(on_time);
      (this->output_->*this->output_low_)();
      misc::delay_us(off_time);
      return;
   }
};

#endif /* PWM_HPP_ */