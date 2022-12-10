/********************************************************************************
* word.hpp: Inneh�ller funktionalitet f�r att anv�nda ord (16-bitars tal) som
*         en enhet eller separata byte (tv� 8-bitars tal).
********************************************************************************/
#ifndef WORD_HPP_
#define WORD_HPP_

/* Inkluderingsdirektiv: */
#include "misc.hpp"

/********************************************************************************
* segmented_word: Strukt f�r att erh�lla ett ord (ett 16-bitars tal) i form
*                 av tv� separata byte (tv� 8-bitars tal).
********************************************************************************/
struct segmented_word
{
   uint8_t low = 0;  /* De �tta minst signifikanta bitarna. */
   uint8_t high = 0; /* De �tta mest signifikanta bitarna. */

   /********************************************************************************
   * segmented_word: Defaultkonstruktor, initierar  nytt segmenterat ord.
   ********************************************************************************/
   segmented_word(void) { }
};

/********************************************************************************
* word: Union f�r att kunna erh�lla ett tal som ett ord (ett 16-bitars tal)
*       eller i form av tv� byte (tv� 8-bitars tal).
********************************************************************************/
union word
{
   segmented_word segmented; /* Det 16-bitars talet uppdelat i tv� 8-bitars tal. */
   uint16_t whole = 0;       /* Det 16-bitars talet som en enda helhet. */

   /********************************************************************************
   * word: Defaultkonstruktor, initierar ord med startv�rde 0.
   ********************************************************************************/
   word(void) { }

   /********************************************************************************
   * word: Initierar ord som lagrar angivet 16-bitars tal, vilket m�jligg�r att 
   *       detta kan anv�ndas som en 16-bitars enhet eller som tv� separata 
   *       8-bitars tal.
   *
   *       - num: Det 16-bitars tal som ska lagras.
   ********************************************************************************/
   word(const uint16_t num)
   {
      this->whole = num;
      return;
   }
};

#endif /* WORD_HPP_ */