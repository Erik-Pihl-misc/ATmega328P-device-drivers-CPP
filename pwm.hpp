/********************************************************************************
* pwm.hpp: Innehåller drivrutiner för PWM-styrning av en godtycklig utenhet,
*          såsom en eller flera lysdioder.
********************************************************************************/
#ifndef PWM_HPP_
#define PWM_HPP_

/* Inkluderingsdirektiv: */
#include "misc.hpp"
#include "adc.hpp"

/********************************************************************************
* pwm: Klass för PWM-kontrollers, som möjliggör PWM-styrning av en godtycklig
*      utenhet, exempelvis en eller flera lysdioder implementerat via ett
*      led-objekt eller en vektor innehållande pekare till multipla led-objekt.
*      PWM-styrning kan ske via en analog insignal såsom en potentiometer eller
*      genom att direkt välja duty cycle.
********************************************************************************/
template<class T>
class pwm
{
private:
   adc input_;                              /* Analog inenhet, såsom en potentiometer. */
   uint16_t period_us_ = 0;                 /* Periodtid för PWM mätt i mikrosekunder. */
   T* output_ = nullptr;                    /* Pekare till ansluten utenhet. */
   void (T::*output_high_)(void) = nullptr; /* Pekare till funktion för att tända ansluten utenhet. */
   void (T::*output_low_)(void) = nullptr;  /* Pekare till funktion för att släcka ansluten utenhet. */
   bool enabled_ = true;                    /* Enable-signal för kontroll av PWM-generering. */

public:
   /********************************************************************************
   * pwm: Defaultkonstruktor, initierar tomt objekt.
   ********************************************************************************/
   pwm(void) { }

   /********************************************************************************
   * pwm: Initierar PWM-kontroller för PWM-styrning av angiven utenhet via
   *      angiven analog insignal. Som default är PWM-styrning aktiverat med
   *      en periodtid på 1000 us.
   *
   *      - input_pin  : Pekare till analog pin som utgör insignal.
   *      - output     : Pekare till utenheten som ska styras med PWM.
   *      - output_high: Funktionspekare för aktivering av angiven utenhet.
   *      - output_low : Funktionspekare för inaktivering av angiven utenhet.
   *      - period_us  : Periodtid för PWM i mikrosekunder (default = 1000 us).
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
   * ~pwm: Nollställer angiven PWM-kontroller innan radering.
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
   * is_enabled: Indikerar ifall PWM-kontrollern är aktiverad eller inte.
   ********************************************************************************/
   bool enabled(void) const
   {
      return this->enabled_;
   }

   /********************************************************************************
   * period_us: Returnerar aktuell periodtid för PWM mätt i millusekunder.
   ********************************************************************************/
   uint16_t period_us(void) const
   {
      return this->period_us_;
   }

   /********************************************************************************
   * init: Initierar PWM-kontroller för PWM-styrning av angiven utenhet via
   *       angiven analog insignal. Som default är PWM-styrning aktiverat med
   *       en periodtid på 1000 us.
   *
   *       - input_pin  : Pekare till analog pin som utgör insignal.
   *       - output     : Pekare till utenheten som ska styras med PWM.
   *       - output_high: Funktionspekare för aktivering av angiven utenhet.
   *       - output_low : Funktionspekare för inaktivering av angiven utenhet.
   *       - period_us  : Periodtid för PWM i mikrosekunder (default = 1000 us).
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
   * clear: Nollställer angiven PWM-kontroller.
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
   * enable: Aktiverar angiven PWM-kontroller så att ansluten utenhet kan styras
   *         via PWM-generering.
   ********************************************************************************/
   void enable(void)
   {
      this->enabled_ = true;
      return;
   }

   /********************************************************************************
   * disable: Inaktiverar angiven PWM-kontroller, vilket medför att PWM-styrning
   *          av ansluten utenhet inte kan genomföras.
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
   * run: Kör angiven PWM-kontroller under en period och styr ansluten utenhet
   *      via avläsning av ansluten analog insignal, förutsatt att PWM-kontrollern 
   *      är aktiverad.
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
   * run_with_duty_cycle: Kör angiven PWM-kontroller under en period och styr
   *                      ansluten utenhet med angiven duty cycle, förutsatt att
   *                      PWM-kontrollern är aktiverad. Duty cycle måste anges
   *                      som ett flyttal mellan 0 - 1, vilket motsvarar 0 - 100 %.
   *
   *                      - duty_cycle: Duty cycle, alltså andelen av aktuell
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