/********************************************************************************
* isr.cpp: Inneh�ller avbrottsrutiner.
********************************************************************************/
#include "header.hpp"

/********************************************************************************
* ISR (PCINT0_vect): Avbrottsrutin som �ger rum vid nedtryckning/uppsl�ppning
*                    av tryckknapp b1 ansluten till pin 13 (PORTB5).
*                    Vid nedtryckning �terst�lls Watchdog-timern, vilket
*                    skrivs ut i ansluten seriell terminal. D�remot vid
*                    uppsl�ppning g�rs ingenting.
*
*                    Oavsett vad som orsakade avbrottet inaktiveras PCI-avbrott
*                    p� I/O-port B i 300 millisekunder via timer 0 f�r att
*                    undvika multipla avbrott orsakade av kontaktstudsar.
********************************************************************************/
ISR (PCINT0_vect)
{
   misc::disable_pin_change_interrupt(io_port::b);
   t0.enable_interrupt();

   if (b1.is_pressed())
   {
      wdt::reset();
      serial::print("Watchdog timer reset!\n");
   }

   return;
}

/********************************************************************************
* ISR (TIMER0_OVF_vect): Avbrottsrutin som �ger rum vid overflow av Timer 0,
*                        dvs. uppr�kning till 256, vilket sker var 0.128:e
*                        millisekund n�r timern �r aktiverad.
*
*                        Timern r�knas upp via uppr�kning av varje passerat
*                        avbrott. N�r timern l�per ut (n�r ber�knat antal
*                        avbrott f�r specificerad tid har r�knats upp) s�
*                        �teraktiveras PCI-avbrott p� I/O-port B (som har
*                        st�ngts av i 300 millisekunder f�r att undvika
*                        multipla avbrott orsakat av kontaktstudsar), f�ljt
*                        av att timern st�ngs av.
********************************************************************************/
ISR (TIMER0_OVF_vect)
{
   t0.count();

   if (t0.elapsed())
   {
      misc::enable_pin_change_interrupt(io_port::b);
      t0.disable_interrupt();
   }

   return;
}

/********************************************************************************
* ISR (TIMER1_COMPA_vect): Avbrottsrutin som �ger rum vid uppr�kning till 256 av
*                          Timer 1 i CTC Mode, vilket sker var 0.128:e
*                          millisekund n�r timern �r aktiverad.
*
*                          Timern r�knas upp via uppr�kning av varje passerat
*                          avbrott. N�r timern l�per ut (var 50:e millisekund
*                          n�r timern �r aktiverad) togglas lysdiod l1.
********************************************************************************/
ISR (TIMER1_COMPA_vect)
{
   t1.count();

   if (t1.elapsed())
   {
      l1.toggle();
   }

   return;
}

/********************************************************************************
* ISR (WDT_vect): Avbrottsrutin som �ger rum vid Watchdog timeout, vilket sker
*                 om Watchdog-timern inte blir �ters�lld var 1024:e millisekund.
*                 Antalet timeouts r�knas upp och skrivs ut i ansluten seriell
*                 terminal. N�r maximalt antal timeouts har genomf�rts l�ses
*                 systemet i ett tillst�nd d�r lysdioden ansluten till pin 8
*                 (PORTB0) blinkar var 50:e millisekund.
********************************************************************************/
ISR (WDT_vect)
{
   static volatile auto system_lockdown = false;

   if (!system_lockdown)
   {
      auto num_timeouts = eeprom::read_byte(TIMEOUT_ADDRESS);

      serial::print("Number of timeouts: ");
      serial::print_unsigned(++num_timeouts);
      serial::print_new_line();

      if (num_timeouts >= TIMEOUT_MAX)
      {
         system_lockdown = true;
         serial::print("Maximum number of timeouts has elapsed!\n");
         serial::print("System lockdown!\n");

         b1.clear();
         t0.clear();
         pwm1.disable();
         t1.enable_interrupt();
      }
      else
      {
         eeprom::write_byte(TIMEOUT_ADDRESS, num_timeouts);
      }
   }

   wdt::enable_interrupt();
   return;
}
