/********************************************************************************
* timer.hpp: Innehåller funktionalitet för implementering av interruptbaserade
*            timerkretsar via klassen timer. Dessa timerkretsar fungerar också 
*            utmärkt att använda som räknare.
********************************************************************************/
#ifndef TIMER_HPP_
#define TIMER_HPP_

/* Inkluderingsdirektiv: */
#include "misc.hpp"

/********************************************************************************
* timer: Klass för implementering av interruptbaserade timerkretsar, som vid
*        behov kan användas som räknare.
********************************************************************************/
class timer
{
public:
   enum class sel; /* Fördeklaration av enumerationsklass för val av timerkrets. */
private:
   volatile uint32_t counter_ = 0;  /* 32-bitars räknare. */
   uint32_t max_count_ = 0;         /* Maxvärde som uppräkning ska ske till. */
   volatile uint8_t* timsk_ = 0;    /* Pekare till maskregister för aktivering av avbrott. */
   uint8_t timsk_bit_ = 0;          /* Bit för aktivering av avbrott i motsvarande maskregister. */
   enum sel timer_sel_ = sel::none; /* Val av timerkrets. */

   static constexpr auto TIME_BETWEEN_INTERRUPTS_MS_ = 0.128; /* 0.128 ms mellan varje timergenererat avbrott. */

   /********************************************************************************
   * get_max_count: Returnerar antalet timergenererade avbrott som krävs för
   *                angiven tid, avrundad till närmaste heltal.
   *
   *                - time_ms: Önskad tid mätt i millisekunder.
   ********************************************************************************/
   static inline uint32_t get_max_count(const double time_ms)
   {
      return static_cast<uint32_t>(time_ms / timer::TIME_BETWEEN_INTERRUPTS_MS_ + 0.5);
   }

   /********************************************************************************
   * init_circuit: Initierar angiven timerkrets. Timer 0 samt Timer 2 initieras 
   *               i Normal Mode, medan Timer 1 initieras i CTC Mode med uppräkning 
   *               till 256. Vid aktiverat avbrott på godtycklig initierad timer 
   *               sker timergenererat avbrott var 0.128:e millisekund.
   ********************************************************************************/
   void init_circuit(void)
   {
      if (this->timer_sel_ == sel::timer0)
      {
         this->timsk_ = &TIMSK0;
         this->timsk_bit_ = TOIE0;
         TCCR0B = (1 << CS01);
      }
      else if (this->timer_sel_ == sel::timer1)
      {
         this->timsk_ = &TIMSK1;
         this->timsk_bit_ = OCIE1A;
         TCCR1B = (1 << CS11) | (1 << WGM12);
         OCR1A = 256;
      }
      else if (this->timer_sel_ == sel::timer2)
      {
         this->timsk_ = &TIMSK2;
         this->timsk_bit_ = TOIE2;
         TCCR2B = (1 << CS21);
      }

      asm("SEI");
      return;
   }

public:

   /********************************************************************************
   * timer: Defaultkonstruktor, initierar tomt objekt.
   ********************************************************************************/
   timer(void) { }

   /********************************************************************************
   * timer: Initierar ny timerkrets med angiven tid mätt i millisekunder.
   *
   *        - timer_sel: Val av timerkrets.
   *        - time_ms  : Tiden timern ska sättas på mätt i millisekunder.
   ********************************************************************************/
   timer(const sel timer_sel, 
         const double time_ms)
   {
      this->init(timer_sel, time_ms);
      return;
   }

   /********************************************************************************
   * ~timer: Stänger av angiven timerkrets innan den raderas. 
   ********************************************************************************/
   ~timer(void)
   {
      this->reset();
      return;
   }

   /********************************************************************************
   * timer: Kopieringskonstruktor raderad.
   ********************************************************************************/
   timer(timer&) = delete;      

   /********************************************************************************
   * timer: Tilldelningsoperator raderad.
   ********************************************************************************/
   timer& operator= (timer&) = delete;

   /********************************************************************************
   * counter: Returnerar lagrat värde från angiven timers räknare.
   ********************************************************************************/
   uint32_t counter(void) const
   {
      return this->counter_;
   }

   /********************************************************************************
   * max_cout: Returnerar det värde som angiven timer ska räkna upp till.
   ********************************************************************************/
   uint32_t max_count(void) const
   {
      return this->max_count_;
   }

   /********************************************************************************
   * timer_sel: Returnerar använd timerkrets via en enumerator av
   *            enumerationsklassen timer::sel.
   ********************************************************************************/
   enum sel timer_sel(void) const
   {
      return this->timer_sel_;
   }

   /********************************************************************************
   * enabled: Indikerar ifall timergenererat avbrott är aktiverat på angiven timer.
   ********************************************************************************/
   bool interrupt_enabled(void) const
   {
      return *(this->timsk_) & (1 << this->timsk_bit_);
   }

   /********************************************************************************
   * init: Initierar ny timerkrets med angiven tid mätt i millisekunder.
   *
   *       - timer_sel: Val av timerkrets.
   *       - time_ms  : Tiden timern ska sättas på mätt i millisekunder.
   ********************************************************************************/
   void init(const sel timer_sel, 
             const double time_ms)
   {
      this->timer_sel_ = timer_sel;
      this->max_count_ = this->get_max_count(time_ms);
      this->init_circuit();
      return;
   }

    /********************************************************************************
   * clear: Inaktiverar och nollställer angiven timerkrets.
   ********************************************************************************/
   void clear(void)
   {
      this->reset();
      this->max_count_ = 0;
      this->timsk_ = 0;
      this->timsk_bit_ = 0;
      this->timer_sel_ = sel::none;
      return;
   }

   /********************************************************************************
   * enable_interrupt: Aktiverar timergenererat avbrott, som äger rum när timern
   *                   räknar upp till overflow eller specificerat max.
   *
   *                   Timer 0 samt Timer 2 aktiveras i Normal Mode, medan Timer 1
   *                   aktiveras i CTC Mode med uppräkning till 256, vilket gör
   *                   att tiden mellan varje timergenererat avbrott är samma 
   *                   oavsett använd timerkrets.
   *
   *                   Avbrottsvektorer för timerkretsarna deklareras nedan:
   *
   *                   Timerkrets     Avbrottsvektor
   *                     Timer 0     TIMER0_OVF_vect
   *                     Timer 1     TIMER1_COMPA_vect
   *                     Timer 2     TIMER2_OVF_vect
   ********************************************************************************/
   void enable_interrupt(void)
   {
      *this->timsk_ = (1 << this->timsk_bit_);
      return;
   }

   /********************************************************************************
   * disable_interrupt: Inaktiverar timergenererat avbrott på angiven timer.
   ********************************************************************************/
   void disable_interrupt(void)
   {
      *this->timsk_ = 0x00;
      return;
   }

   /********************************************************************************
   * toggle_interrupt: Togglar aktivering av timergenererat avbrott på angiven 
   *                   timer. Om avbrott är aktiverat vid anrop sker inaktivering.
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

   /********************************************************************************
   * count: Räknar upp angiven timer.
   ********************************************************************************/
   void count(void)
   {
      this->counter_++;
      return;
   }

   /********************************************************************************
   * elapsed: Indikerar ifall angiven timer har löpt ut genom att returnera true
   *          eller false. Ifall timern har löpt ut nollställs räknaren inför
   *          nästa uppräkning.
   ********************************************************************************/
   bool elapsed(void)
   {
      if (this->counter_ >= this->max_count_)
      {
         this->counter_ = 0;
         return true;
      }
      else
      {
         return false;
      }
   }

   /********************************************************************************
   * reset: Återställer angiven timer till startläget.
   ********************************************************************************/
    void reset(void)
    {
       this->disable_interrupt();
       this->counter_ = 0;
       return;
    }

   /********************************************************************************
   * set_time_ms: Sätter ny tid på angiven timerkrets mätt i millisekunder.
   * 
   *               - new_time_ms: Tiden timern ska sättas på i millisekunder.
   ********************************************************************************/
   void set_time_ms(const double new_time_ms)
   {
      this->max_count_ = this->get_max_count(new_time_ms);
      return;
   }

   /********************************************************************************
   * set_max_count: Sätter nytt maxvärde för uppräkning av timern när denna ska
   *                användas som en räknare.
   *
   *                - new_max_count: Maxvärde för uppräkningen.
   ********************************************************************************/
   void set_max_count(const uint32_t new_max_count)
   {
      this->max_count_ = new_max_count;
      return;
   }

   /********************************************************************************
   * sel: Enumeration för val av timerkrets.
   ********************************************************************************/
   enum class sel
   {
      timer0, /* Timer 0. */
      timer1, /* Timer 1. */
      timer2, /* Timer 2. */
      none    /* Timer ospecificerad. */
   };
};

#endif /* TIMER_HPP_ */