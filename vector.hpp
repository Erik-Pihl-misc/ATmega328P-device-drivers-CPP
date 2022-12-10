/********************************************************************************
* vector.hpp: Implementering av dynamiska vektorer via klassen vector.
********************************************************************************/
#ifndef VECTOR_HPP_
#define VECTOR_HPP_

/* Inkluderingsdirektiv: */
#include "misc.hpp"

/********************************************************************************
* vector: Generisk klass för dynamiska vektorer av valfri datatyp.
********************************************************************************/
template<class T>
class vector
{
protected:
   T* data_ = nullptr; /* Pekare till ett fält innehållande lagrad data. */
   size_t size_ = 0;   /* Vektorns storlek, dvs. antalet lagrade element. */
public:

   /*****************************************************************************
   * vector: Tom konstruktor, initierar en ny tom vektor. 
   *****************************************************************************/
   vector(void) { }

   /*****************************************************************************
   * vector: Konstruktor, initierar ny vektor av angiven storlek med angivet
   *         startvärde.
   * 
   *         - start_size: Vektorns nya storlek (antalet element).
   *         - start_val : Referens till startvärde för samtliga element 
   *                       (default = 0).
   *****************************************************************************/
   vector(const size_t start_size,
          const T& start_val = static_cast<T>(0))
   {
      this->resize(start_size, start_val);
      return;
   }

   /*****************************************************************************
   * ~vector: Destruktor, frigör minne allokerat för vektorn innan radering.
   *****************************************************************************/
   ~vector(void) 
   {
      this->clear();
      return;
   } 

   /*****************************************************************************
   * data: Returnerar en pekare till innehållet lagrat i angiven vektor.
   *****************************************************************************/
   T* data(void) const
   {
      return this->data_;
   }

   /*****************************************************************************
   * size: Returnerar arrayens storlek (antalet element) i angiven vektor.
   *****************************************************************************/
   size_t size(void) const 
   { 
      return this->size_; 
   }

   /*****************************************************************************
   * begin: Returnerar adressen till det första elementet i angiven vektor.
   *****************************************************************************/
   T* begin(void) const
   {
      return this->data_;
   }

   /*****************************************************************************
   * end: Returnerar adressen direkt efter det sista elementet i angiven vektor.
   *****************************************************************************/
   T* end(void) const
   {
      return this->data_ + this->size_;
   }

   /*****************************************************************************
   * last: Returnerar adressen till det sista elementet i angiven vektor.
   *****************************************************************************/
   T* last(void) const
   {
      return this->data_ ? this->end() - 1 : nullptr;
   }

   /*****************************************************************************
   * clear: Tömmer angiven vektor.
   *****************************************************************************/
   void clear(void)
   {
      free(this->data_);
      this->data_ = nullptr;
      this->size_ = 0;
      return;
   }

   /*****************************************************************************
   * resize: Ändrar storlek på angiven vektor med angivet startvärde, där
   *         startvärdet är satt till 0 som default. Ifall vektorns storlek
   *         lyckas ändras till önskad storlek returneras 0, annars felkod 1.
   * 
   *         - new_size : Vektorns nya storlek (antalet element).
   *         - start_val: Referens till startvärde för respektive element 
   *                      (default = 0).
   *****************************************************************************/
   int resize(const size_t new_size,
              const T& start_val = static_cast<T>(0))
   {
      if (new_size == 0)
      {
         this->clear();
         return 0;
      }

      auto copy = (T*)realloc(this->data_, sizeof(T) * new_size);
      if (!copy) return 1;
      this->data_ = copy;
      this->size_ = new_size;

      for (auto& i : *this)
      {
         i = start_val;
      }

      return 0;
   }

   /*****************************************************************************
   * push: Lägger till ett nytt element längst bak i angiven vektor. Ifall
   *       minnesallokeringen lyckas så returneras 0, annars felkod 1.
   * 
   *       - new_element: Referens till det nya element som skall läggas till.
   *****************************************************************************/
   int push(const T& new_element)
   {
      auto copy = (T*)realloc(this->data_, sizeof(T) * (this->size_ + 1));
      if (!copy) return 1;
      copy[this->size_++] = new_element;
      this->data_ = copy;
      return 0;
   }

   /*****************************************************************************
   * pop: Tar bort det sista elementet i angiven vektor om ett sådant finns.
   *      Om högst ett element finns lagrat i vektor så töms den helt.
   *****************************************************************************/
   void pop(void)
   {
      if (this->size <= 1)
      {
         this->clear();
      }
      else
      {
         auto copy = (T*)realloc(this->data_, sizeof(T) * (this->size_ - 1));
         if (!copy) return;
         this->data_ = copy;
         this->size_--;
      }

      return;
   }
};

#endif /* VECTOR_HPP_ */