/********************************************************************************
* isr.cpp: Innehåller avbrottsrutiner.
********************************************************************************/
#include "header.hpp"

/********************************************************************************
* ISR (PCINT0_vect): Avbrottsrutin som äger rum vid nedtryckning/uppsläppning
*                    av tryckknapp b1 ansluten till pin 13 (PORTB5).
*                    Vid nedtryckning återställs Watchdog-timern, vilket
*                    skrivs ut i ansluten seriell terminal. Däremot vid
*                    uppsläppning görs ingenting.
*
*                    Oavsett vad som orsakade avbrottet inaktiveras PCI-avbrott
*                    på I/O-port B i 300 millisekunder via timer 0 för att
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
* ISR (TIMER0_OVF_vect): Avbrottsrutin som äger rum vid overflow av Timer 0,
*                        dvs. uppräkning till 256, vilket sker var 0.128:e
*                        millisekund när timern är aktiverad.
*
*                        Timern räknas upp via uppräkning av varje passerat
*                        avbrott. När timern löper ut (när beräknat antal
*                        avbrott för specificerad tid har räknats upp) så
*                        återaktiveras PCI-avbrott på I/O-port B (som har
*                        stängts av i 300 millisekunder för att undvika
*                        multipla avbrott orsakat av kontaktstudsar), följt
*                        av att timern stängs av.
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
* ISR (TIMER1_COMPA_vect): Avbrottsrutin som äger rum vid uppräkning till 256 av
*                          Timer 1 i CTC Mode, vilket sker var 0.128:e
*                          millisekund när timern är aktiverad.
*
*                          Timern räknas upp via uppräkning av varje passerat
*                          avbrott. När timern löper ut (var 50:e millisekund
*                          när timern är aktiverad) togglas lysdiod l1.
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
* ISR (WDT_vect): Avbrottsrutin som äger rum vid Watchdog timeout, vilket sker
*                 om Watchdog-timern inte blir återsälld var 1024:e millisekund.
*                 Antalet timeouts räknas upp och skrivs ut i ansluten seriell
*                 terminal. När maximalt antal timeouts har genomförts låses
*                 systemet i ett tillstånd där lysdioden ansluten till pin 8
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
