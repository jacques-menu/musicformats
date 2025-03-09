/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "smartpointer.h"

#include "mfIndentedTextOutput.h"
#include "mfStringsHandling.h"

#include "mfPair.h"


namespace MusicFormats
{

// //______________________________________________________________________________
// template <typename FIRST_TYPE, typename SECOND_TYPE>
// SMARTP<mfPair<FIRST_TYPE, SECOND_TYPE>> mfPair<FIRST_TYPE, SECOND_TYPE>::create ()
// {
//   mfPair<FIRST_TYPE, SECOND_TYPE>* obj = new
//     mfPair<FIRST_TYPE, SECOND_TYPE> ();
//   assert (obj != nullptr);
//   return obj;
// }
//
// template <typename FIRST_TYPE, typename SECOND_TYPE>
// SMARTP<mfPair<FIRST_TYPE, SECOND_TYPE>> mfPair<FIRST_TYPE, SECOND_TYPE>::create (
// 	const FIRST_TYPE, SECOND_TYPE& element)
// {
//   mfPair<FIRST_TYPE, SECOND_TYPE>* obj = new
//     mfPair<FIRST_TYPE, SECOND_TYPE> (
//     	element);
//   assert (obj != nullptr);
//   return obj;
// }
//
// template <typename FIRST_TYPE, typename SECOND_TYPE>
// mfPair<FIRST_TYPE, SECOND_TYPE>::mfPair ()
// {}
//
// template <typename FIRST_TYPE, typename SECOND_TYPE>
// mfPair<FIRST_TYPE, SECOND_TYPE>::mfPair (
// 	const FIRST_TYPE, SECOND_TYPE& element)
// {
//   fElementsStdList.push_back (element);
// }
//
// template <typename FIRST_TYPE, typename SECOND_TYPE>
// mfPair<FIRST_TYPE, SECOND_TYPE>::~mfPair<FIRST_TYPE, SECOND_TYPE> ()
// {}
//
// //______________________________________________________________________________
// template <typename FIRST_TYPE, typename SECOND_TYPE>
// Bool mfPair<FIRST_TYPE, SECOND_TYPE>::contains (
// 	const FIRST_TYPE, SECOND_TYPE& value) const
// {
//   Bool result;
//
//   for (FIRST_TYPE, SECOND_TYPE element : fElementsStdList) {
//     if (element == value) {
//       result = true;
//       break;
//     }
//   } // for
//
//   return result;
// }
//
// //______________________________________________________________________________
// template <typename FIRST_TYPE, typename SECOND_TYPE>
// void mfPair<FIRST_TYPE, SECOND_TYPE>::print (std::ostream& os) const
// {
//   os <<
//     "[mfPair<FIRST_TYPE, SECOND_TYPE>" <<
//     ", fElementsPairName: " << fElementsPairName << ':';
//
//   if (fElementsStdList.size ()) {
//   	os << std::endl;
//
//     ++gIndenter;
//
// 		for (FIRST_TYPE, SECOND_TYPE element : fElementsStdList) {
// 			os <<
// 				element->asString () <<
// 				std::endl;
// 		} // for
//
//     --gIndenter;
//   }
//
//   else {
//     os <<
//       " [EMPTY]" <<
//       std::endl;
//   }
//
// 	os << ']' << std::endl;
// }
//
// template <typename FIRST_TYPE, typename SECOND_TYPE>
// void mfPair<FIRST_TYPE, SECOND_TYPE>::printWithContext (
//   const std::string& context,
// 	char               evidencer,
//   std::ostream&      os) const
// {
// 	size_t
// 		theElementsStdListSize =
// 			fElementsStdList.size ();
//
//   os <<
//     "The part groups stack contains " <<
//     "[mfPair<FIRST_TYPE, SECOND_TYPE>" <<
//     ", fElementsPairName: \"" << fElementsPairName << "\"" <<
//     ", " <<
//     mfSingularOrPlural (
//     	theElementsStdListSize, "element", "elements") <<
//     " -- " <<
//     context <<
//     ':';
//
//   if (fElementsStdList.size ()) {
//   	os << std::endl;
//
//     ++gIndenter;
//
// 		int counter = theElementsStdListSize - 1;
//     for (FIRST_TYPE, SECOND_TYPE element : fElementsStdList) {
//       os <<
//         counter << ": " << evidencer <<
//         std::endl;
//
// 			++gIndenter;
//       os <<
//         element <<
//         std::endl;
// 			--gIndenter;
//
//       --counter;
//     } // for
//
//     --gIndenter;
//   }
//
//   else {
//     os <<
//       " [EMPTY]" <<
//       std::endl;
//   }
//
//   os << ']' << std::endl;
// }
//
// template <typename FIRST_TYPE, typename SECOND_TYPE>
// std::ostream& operator << (std::ostream& os, const mfPair<FIRST_TYPE, SECOND_TYPE>& elt)
// {
//   elt.print (os);
//   return os;
// }
//
// template <typename FIRST_TYPE, typename SECOND_TYPE>
// std::ostream& operator << (std::ostream& os, const SMARTP<mfPair<FIRST_TYPE, SECOND_TYPE>>& elt)
// {
//   elt->print (os);
//   return os;
// }


}
