/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfEnumAll___
#define ___mfEnumAll___


namespace MusicFormats
{

//______________________________________________________________________________
/*
  https://stackoverflow.com/questions/8498300/allow-for-range-based-for-with-enum-classes
*/
template< typename T >
class   mfEnumAll
{
  public:

    class   Iterator
    {
      public:

        Iterator (int value)
          : fIterationIndex (value)
            {}

        T operator* (void) const
            { return (T) fIterationIndex; }

        void operator++ (void)
            {  ++fIterationIndex; }

        Bool operator != (Iterator rhs)
            {  return fIterationIndex != rhs.fIterationIndex; }

      private:

        int fIterationIndex;
     };
};

template< typename T >
typename mfEnumAll<T>::Iterator begin (mfEnumAll<T>)
{
  return typename mfEnumAll<T>::Iterator ((int)T::AllFirst);
}

template< typename T >
typename mfEnumAll<T>::Iterator end (mfEnumAll<T>)
{
  return typename mfEnumAll<T>::Iterator (((int)T::AllLast) + 1);
}


}


#endif // ___mfEnumAll___
