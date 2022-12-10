/********************************************************************************
* wdt.hPP: Inneh�ller drivrutiner f�r mikrokontrollerns Watchdog-timer, b�de i
*          System Reset Mode, vilket inneb�r att mikrodatorn �terst�lls vid
*          timeout, samt i Interrupt Mode, vilket inneb�r avbrott vid timeout.
*          Det �r ocks� m�jligt att kombinera tidigare n�mnda modes f�r att
*          avbrott sker, f�ljt av system�terst�llning.
*
*          Avbrottsvektorn f�r timeout-avbrott �r WDT_vect.
********************************************************************************/
#ifndef WDT_HPP_
#define WDT_HPP_

namespace wdt
{
   /********************************************************************************
   * timeout: Enumerationsklass f�r val av timeout f�r mikrodatorns Watchdog-timer.
   ********************************************************************************/
   enum class timeout
   {
      _16_ms   = 0,                                       /* 16 ms. */
      _32_ms   = (1 << WDP0),                             /* 32 ms. */
      _64_ms   = (1 << WDP1),                             /* 64 ms. */
      _128_ms  = (1 << WDP1) | (1 << WDP0),               /* 128 ms. */
      _256_ms  = (1 << WDP2),                             /* 256 ms. */
      _512_ms  = (1 << WDP2) | (1 << WDP0),               /* 512 ms. */
      _1024_ms = (1 << WDP2) | (1 << WDP1),               /* 1024 ms. */
      _2048_ms = (1 << WDP2) | (1 << WDP1) | (1 << WDP0), /* 2048 ms. */
      _4096_ms = (1 << WDP3),                             /* 4096 ms. */
      _8192_ms = (1 << WDP3) | (1 << WDP0),               /* 8192 ms. */
   };

   /********************************************************************************
   * reset: �terst�ller Watchdog-timern, vilket m�ste ske kontinuerligt innan
   *        timern l�per ut f�r att undvika system�terst�llning eller avbrott.
   ********************************************************************************/
   auto reset = [](void)
   {
      asm("CLI");
      asm("WDR");
      MCUSR &= ~(1 << WDRF);
      asm("SEI");
      return;
   };

   /********************************************************************************
   * init: Initierar Watchdog-timern med angiven timeout m�tt i millisekunder.
   *
   *       - timeout_ms: Timeout m�tt i millisekunder.
   ********************************************************************************/
   auto init = [](const timeout timeout_ms)
   {
      wdt::reset();
      asm("CLI");
      WDTCSR = (1 << WDCE) | (1 << WDE);
      WDTCSR = (1 << WDE) | static_cast<uint8_t>(timeout_ms);
      asm("SEI");
      return;
   };

   /********************************************************************************
   * clear: Nollst�ller Watchdog-timern.
   ********************************************************************************/
   auto clear = [](void)
   {
      wdt::reset();
      WDTCSR = 0x00;
      return;
   };

   /********************************************************************************
   * enable_system_reset: Aktiverar Watchdog-timern i System Reset Mode, vilket
   *                      inneb�r att systemet �terst�lls ifall Watchdog-timern
   *                      l�per ut.
   ********************************************************************************/
   auto enable_system_reset = [](void)
   {
      wdt::reset();
      WDTCSR |= (1 << WDE);
      return;
   };

   /********************************************************************************
   * disable_system_reset: Inaktiverar Watchdog-timern i System Reset Mode, vilket
   *                       inneb�r att systemet inte �terst�lls ifall 
   *                       Watchdog-timern l�per ut.
   ********************************************************************************/
   auto disable_system_reset = [](void)
   {
      wdt::reset();
      WDTCSR &= ~(1 << WDE);
      return;
   };

   /********************************************************************************
   * enable_interrupt: Aktiverar Watchdog-timern i Interrupt Mode, vilket inneb�r
   *                   att ett avbrott med avbrottsvektor WDT_vect �ger rum ifall
   *                   Watchdog-timern l�per ut.
   ********************************************************************************/
   auto enable_interrupt = [](void)
   {
      wdt::reset();
      WDTCSR |= (1 << WDIE);
      return;
   };

   /********************************************************************************
   * disable_interrupt: Inaktiverar Watchdog-timern i Interrupt Mode, vilket
   *                    inneb�r att avbrott inte �ger rum ifall Watchdog-timern
   *                    l�per ut.
   ********************************************************************************/
   auto disable_interrupt = [](void)
   {
      wdt::reset();
      WDTCSR &= ~(1 << WDIE);
      return;
   };
}

#endif /* WDT_HPP_ */