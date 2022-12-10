/********************************************************************************
* wdt.hPP: Innehåller drivrutiner för mikrokontrollerns Watchdog-timer, både i
*          System Reset Mode, vilket innebär att mikrodatorn återställs vid
*          timeout, samt i Interrupt Mode, vilket innebär avbrott vid timeout.
*          Det är också möjligt att kombinera tidigare nämnda modes för att
*          avbrott sker, följt av systemåterställning.
*
*          Avbrottsvektorn för timeout-avbrott är WDT_vect.
********************************************************************************/
#ifndef WDT_HPP_
#define WDT_HPP_

namespace wdt
{
   /********************************************************************************
   * timeout: Enumerationsklass för val av timeout för mikrodatorns Watchdog-timer.
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
   * reset: Återställer Watchdog-timern, vilket måste ske kontinuerligt innan
   *        timern löper ut för att undvika systemåterställning eller avbrott.
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
   * init: Initierar Watchdog-timern med angiven timeout mätt i millisekunder.
   *
   *       - timeout_ms: Timeout mätt i millisekunder.
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
   * clear: Nollställer Watchdog-timern.
   ********************************************************************************/
   auto clear = [](void)
   {
      wdt::reset();
      WDTCSR = 0x00;
      return;
   };

   /********************************************************************************
   * enable_system_reset: Aktiverar Watchdog-timern i System Reset Mode, vilket
   *                      innebär att systemet återställs ifall Watchdog-timern
   *                      löper ut.
   ********************************************************************************/
   auto enable_system_reset = [](void)
   {
      wdt::reset();
      WDTCSR |= (1 << WDE);
      return;
   };

   /********************************************************************************
   * disable_system_reset: Inaktiverar Watchdog-timern i System Reset Mode, vilket
   *                       innebär att systemet inte återställs ifall 
   *                       Watchdog-timern löper ut.
   ********************************************************************************/
   auto disable_system_reset = [](void)
   {
      wdt::reset();
      WDTCSR &= ~(1 << WDE);
      return;
   };

   /********************************************************************************
   * enable_interrupt: Aktiverar Watchdog-timern i Interrupt Mode, vilket innebär
   *                   att ett avbrott med avbrottsvektor WDT_vect äger rum ifall
   *                   Watchdog-timern löper ut.
   ********************************************************************************/
   auto enable_interrupt = [](void)
   {
      wdt::reset();
      WDTCSR |= (1 << WDIE);
      return;
   };

   /********************************************************************************
   * disable_interrupt: Inaktiverar Watchdog-timern i Interrupt Mode, vilket
   *                    innebär att avbrott inte äger rum ifall Watchdog-timern
   *                    löper ut.
   ********************************************************************************/
   auto disable_interrupt = [](void)
   {
      wdt::reset();
      WDTCSR &= ~(1 << WDIE);
      return;
   };
}

#endif /* WDT_HPP_ */