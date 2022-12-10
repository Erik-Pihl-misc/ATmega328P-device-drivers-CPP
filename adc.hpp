/********************************************************************************
* adc.hpp: Innehåller funktionalitet för enkel implementering av AD-omvandling
*          för analoga signaler via klassen adc. AD-omvandling sker via 
*          inläsning av signaler från analoga pinnar A0 - A5 på  Arduino Uno,
*          vilket motsvarar PORTC0 - PORTC5 på ATmega328P.
*
*          Analoga insignaler mellan 0 - 5 V AD-omvandlas till digitala
*          motsvarigheter mellan 0 - 1023. Duty cycle kan användas för
*          PWM-generering och beräknas enligt nedan:
*
*                         Duty cycle = ADC_result / ADC_MAX,
*
*          där ADC_result är resultat avläst från AD-omvandlaren OCH ADC_MAX
*          utgör högsta möjliga avlästa värde, vilket är 1023.0.
********************************************************************************/
#ifndef ADC_HPP_
#define ADC_HPP_

/********************************************************************************
* adc: Klass för implementering av AD-omvandlare, som möjliggör avläsning
*      av insignaler från analoga pinnar, beräkning av on- och off-tid för
*      PWM-generering (tiden som PWM-styrd utsignal ska vara hög respektive låg).
********************************************************************************/
class adc
{
private:
   uint8_t pin_ = 0;                        /* Analog pin som skall användas för avläsning. */
   uint16_t pwm_on_us_ = 0;                 /* On-tid för PWM-generering i mikrosekunder. */
   uint16_t pwm_off_us_ = 0;                /* Off-tid för PWM-generering i mikrosekunder. */
   static constexpr auto ADC_MAX_ = 1023.0; /* Högsta digitala värde vid AD-omvandling. */
   static constexpr auto VCC_ = 5.0;        /* 5 V matningsspänning. */
public:

   /********************************************************************************
   * adc: Defaultkonstruktor, initierar tomt objekt.
   ********************************************************************************/
   adc(void) { }

   /********************************************************************************
   * adc: Initierar analog pin för avläsning och AD-omvandling av insignaler.
   *
   *      - pin : Analog pin som skall läsas för AD-omvandling, som antingen kan
   *              anges som ett tal mellan 0 - 5 eller via konstanter A0 - A5
                  (som motsvarar heltal 14 - 19).
   ********************************************************************************/
   adc(const uint8_t pin) 
   { 
      this->init(pin); 
      return;
   }

   /********************************************************************************
   * pin: Returnerar använd analog pin.
   ********************************************************************************/
   uint8_t pin(void) const
   {
      return this->pin_;
   }

   /********************************************************************************
   * pwm_on_us: Returnerar aktuell on-tid för PWM-generering i mikrosekunder.
   ********************************************************************************/
   uint16_t pwm_on_us(void) const
   {
      return this->pwm_on_us_;
   }

   /********************************************************************************
   * pwm_off_us: Returnerar aktuell off-tid för PWM-generering i mikrosekunder.
   ********************************************************************************/
   uint16_t pwm_off_us(void) const
   {
      return this->pwm_off_us_;
   }

   /********************************************************************************
   * max_val: Returnerar högsta möjliga AD-omvandlade värde.
   ********************************************************************************/
   double max_val(void) const
   {
      return this->ADC_MAX_;
   }

   /********************************************************************************
   * init: Initierar analog pin för avläsning och AD-omvandling av insignaler.
   *
   *       - pin : Analog pin A0 - A5 som skall läsas för AD-omvandling.
   ********************************************************************************/
   void init(const uint8_t pin)
   {
      if (pin >= 0 && pin <= 5)
      {
         this->pin_ = pin;
      }
      else if (pin >= 14 && pin <= 19)
      {
         this->pin_ = pin - 14;
      }
      
      static_cast<void>(this->read());
      return;
   }

   /********************************************************************************
   * read: Läser av en analog insignal och returnerar motsvarande digitala
   *       motsvarighet mellan 0 - 1023.
   ********************************************************************************/
   uint16_t read(void) const
   {
      ADMUX = (1 << REFS0) | this->pin_;
      ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
      while ((ADCSRA & (1 << ADIF)) == 0);
      ADCSRA = (1 << ADIF);
      return ADC;
   }

   /********************************************************************************
   * duty_cycle: Läser av en analog insignal och returnerar motsvarande duty cycle
   *             som ett flyttal mellan 0 - 1.
   ********************************************************************************/
   double duty_cycle(void) const
   {
      return this->read() / this->ADC_MAX_;
   }

   /********************************************************************************
   * get_pwm_values: Läser av en analog insignal och beräknar on- och off-tid för
   *                 för PWM-generering, avrundat till närmaste heltal.
   *
   *                 - pwm_period_us: PWM-perioden (on-tid + off-tid) mätt i
   *                                  mikrosekunder (default = 10 000 us).
   ********************************************************************************/
   void get_pwm_values(const uint16_t pwm_period_us = 10000)
   {
      this->pwm_on_us_ = static_cast<uint16_t>(this->duty_cycle() * pwm_period_us + 0.5);
      this->pwm_off_us_ = pwm_period_us - this->pwm_on_us_;
      return;
   }

   /********************************************************************************
   * get_input_voltage: Returnerar inspänningen på angiven analog pin genom att
   *                    läsa av insignalen, omvandla till motsvarande digitala
   *                    värde och sedan beräkna motsvarande inspänning.
   *
   *                    Värdet beräknas efter en matningsspänning på 5 V, vilket
   *                    medför en analog insignal mellan 0 - 5 V. Genom att avläsa 
   *                    duty cycle (mellan 0 - 1) så erhålls motsvarande analoga 
   *                    inspänning Uin, som returneras.
   ********************************************************************************/
   double get_input_voltage(void) const
   {
      return this->duty_cycle() * VCC_;
   }

   /********************************************************************************
   * get_temperature: Returnerar aktuell rumstemperatur via avläsning av temperatur-
   *                  sensor TMP36, som måste vara ansluten till angiven pin.
   *
   *                  Temperaturen T beräknas via följande formel:
   *                  T = 100 * Uin - 50,
   *                  där Uin är aktuell analog inspänning avläst från 
   *                  temperatursensor TMP36.
   ********************************************************************************/
   double get_temperature(void) const
   {
      return 100 * this->get_input_voltage() - 50;
   }

};

#endif /* ADC_HPP_ */