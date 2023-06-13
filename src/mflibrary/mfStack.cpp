/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "smartpointer.h"

#include "mfIndentedTextOutput.h"
#include "mfStringsHandling.h"

#include "mfStack.h"


namespace MusicFormats
{

//______________________________________________________________________________
template <typename ELEMENT_TYPE>
SMARTP<mfStack<ELEMENT_TYPE> > mfStack<ELEMENT_TYPE>::create ()
{
  mfStack<ELEMENT_TYPE>* obj = new
    mfStack<ELEMENT_TYPE> ();
  assert (obj != nullptr);
  return obj;
}

template <typename ELEMENT_TYPE>
SMARTP<mfStack<ELEMENT_TYPE> > mfStack<ELEMENT_TYPE>::create (
	const ELEMENT_TYPE& element)
{
  mfStack<ELEMENT_TYPE>* obj = new
    mfStack<ELEMENT_TYPE> (
    	element);
  assert (obj != nullptr);
  return obj;
}

template <typename ELEMENT_TYPE>
mfStack<ELEMENT_TYPE>::mfStack ()
{}

template <typename ELEMENT_TYPE>
mfStack<ELEMENT_TYPE>::mfStack (
	const ELEMENT_TYPE& element)
{
  fElementsStdList.push_back (element);
}

template <typename ELEMENT_TYPE>
mfStack<ELEMENT_TYPE>::~mfStack<ELEMENT_TYPE> ()
{}

//______________________________________________________________________________
template <typename ELEMENT_TYPE>
Bool mfStack<ELEMENT_TYPE>::contains (
	const ELEMENT_TYPE& value) const
{
  Bool result;

  for (ELEMENT_TYPE element : fElementsStdList) {
    if (element == value) {
      result = true;
      break;
    }
  } // for

  return result;
}

//______________________________________________________________________________
template <typename ELEMENT_TYPE>
void mfStack<ELEMENT_TYPE>::print (std::ostream& os) const
{
  os <<
    "[mfStack<ELEMENT_TYPE>" <<
    ", fElementsStackName: " << fElementsStackName << ':';

  if (fElementsStdList.size ()) {
  	os << std::endl;

    ++gIndenter;

		for (ELEMENT_TYPE element : fElementsStdList) {
			os <<
				element->asString () <<
				std::endl;
		} // for

    --gIndenter;
  }

  else {
    os <<
      " [EMPTY]" <<
      std::endl;
  }

	os << ']' << std::endl;
}

template <typename ELEMENT_TYPE>
void mfStack<ELEMENT_TYPE>::printWithContext (
  const std::string& context,
	char               evidencer,
  std::ostream&      os) const
{
	size_t
		theElementsStdListSize =
			fElementsStdList.size ();

  os <<
    "The part groups stack contains " <<
    "[mfStack<ELEMENT_TYPE>" <<
    ", fElementsStackName: \"" << fElementsStackName << "\"" <<
    ", " <<
    mfSingularOrPlural (
    	theElementsStdListSize, "element", "elements") <<
    " -- " <<
    context <<
    ':';

  if (fElementsStdList.size ()) {
  	os << std::endl;

    ++gIndenter;

		int counter = theElementsStdListSize - 1;
    for (ELEMENT_TYPE element : fElementsStdList) {
      os <<
        counter << ": " << evidencer <<
        std::endl;

			++gIndenter;
      os <<
        element <<
        std::endl;
			--gIndenter;

      --counter;
    } // for

    --gIndenter;
  }

  else {
    os <<
      " [EMPTY]" <<
      std::endl;
  }

  os << ']' << std::endl;
}

template <typename ELEMENT_TYPE>
std::ostream& operator << (std::ostream& os, const mfStack<ELEMENT_TYPE>& elt)
{
  elt.print (os);
  return os;
}

template <typename ELEMENT_TYPE>
std::ostream& operator << (std::ostream& os, const SMARTP<mfStack<ELEMENT_TYPE> >& elt)
{
  elt->print (os);
  return os;
}


}
