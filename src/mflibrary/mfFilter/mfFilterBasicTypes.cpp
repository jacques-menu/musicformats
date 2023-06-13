/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw()), set::precision(), ...

#include "mfAssert.h"
#include "mfBool.h"
#include "mfStringsHandling.h"

#include "mfPreprocessorSettings.h"

#include "oahOah.h"

#include "mfFilterBasicTypes.h"
#include "mfFilterDriver.h"

#include "mfFilterInterpreterOah.h"

#include "mfFilterWae.h"


using namespace std;

using namespace MusicFormats;

//_______________________________________________________________________________
S_mfFilterExpression mfFilterExpression::create ()
{
  mfFilterExpression* obj =
    new mfFilterExpression ();
  assert (obj != nullptr);
  return obj;
}

mfFilterExpression::mfFilterExpression ()
{}

mfFilterExpression::~mfFilterExpression ()
{}

void mfFilterExpression::appendTerm (
  S_mfFilterTerm& term)
{
  fORedTermsList.push_back (term);
};

Bool mfFilterExpression::stringIsFoundInExpression (std::string theString) const
{
  Bool result;

  for (S_mfFilterTerm term : fORedTermsList) {
    if (term->stringIsFoundInTerm (theString)) {
      result = true;
      break;
    }
  } // for

  return result;
}

std::string mfFilterExpression::asString () const
{
  stringstream ss;

  int theORedTermsListSize = fORedTermsList.size ();
  int counter = 0;

  for (S_mfFilterTerm term : fORedTermsList) {
    counter++;

    ss << term->asString ();

    if (counter < theORedTermsListSize) {
      ss << " | ";
    }
  } // for

  return ss.str ();
}

std::string mfFilterExpression::asStringWithFullParentheses () const
{
  stringstream ss;

  int theORedTermsListSize = fORedTermsList.size ();
  int counter = 0;

  ss << '(';

  for (S_mfFilterTerm term : fORedTermsList) {
    counter++;

    ss << term->asStringWithFullParentheses ();

    if (counter < theORedTermsListSize) {
      ss << " | ";
    }
  } // for

  ss << ')';

  return ss.str ();
}

void mfFilterExpression::print (std::ostream& os) const
{
  os <<
    "Expression" <<
    endl;

  int theORedFactorsListSize = fORedTermsList.size ();
  int counter = 0;

  ++gIndenter;

  for (S_mfFilterTerm term : fORedTermsList) {
    counter++;

    os << term;

    if (counter < theORedFactorsListSize) {
      os <<
        '|' <<
        std::endl;
    }
  } // for

  --gIndenter;
}

std::ostream& operator<< (std::ostream& os, const S_mfFilterExpression& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NULL ***" << endl;
  }

  return os;
}

//_______________________________________________________________________________
S_mfFilterTerm mfFilterTerm::create ()
{
  mfFilterTerm* obj =
    new mfFilterTerm ();
  assert (obj != nullptr);
  return obj;
}

mfFilterTerm::mfFilterTerm ()
{}

mfFilterTerm::~mfFilterTerm ()
{}

void mfFilterTerm::appendFactor (
  S_mfFilterFactor& factor)
{
  fANDedFactorsList.push_back (factor);
};

Bool mfFilterTerm::stringIsFoundInTerm (std::string theString) const
{
  Bool result;

  for (S_mfFilterFactor factor : fANDedFactorsList) {
    if (factor->stringIsFoundInFactor (theString)) {
      result = true;
      break;
    }
  } // for

  return result;
}

std::string mfFilterTerm::asString () const
{
  stringstream ss;

  int theANDedFactorsListSize = fANDedFactorsList.size ();
  int counter = 0;

  for (S_mfFilterFactor factor : fANDedFactorsList) {
    counter++;

    ss << factor->asString ();

    if (counter < theANDedFactorsListSize) {
      ss << " & ";
    }
  } // for

  return ss.str ();
}

std::string mfFilterTerm::asStringWithFullParentheses () const
{
  stringstream ss;

  int theANDedFactorsListSize = fANDedFactorsList.size ();
  int counter = 0;

  ss << '(';

  for (S_mfFilterFactor factor : fANDedFactorsList) {
    counter++;

    ss << factor->asStringWithFullParentheses ();

    if (counter < theANDedFactorsListSize) {
      ss << " & ";
    }
  } // for

  ss << ')';

  return ss.str ();
}

void mfFilterTerm::print (std::ostream& os) const
{
  os <<
    "Term" <<
    endl;

  int theAndedFactorsListSize = fANDedFactorsList.size ();
  int counter = 0;

  ++gIndenter;

  for (S_mfFilterFactor factor : fANDedFactorsList) {
    counter++;

    os << factor;

    if (counter < theAndedFactorsListSize) {
      os <<
        '&' <<
        std::endl;
    }
  } // for

  --gIndenter;
}

std::ostream& operator<< (std::ostream& os, const S_mfFilterTerm& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NULL ***" << endl;
  }

  return os;
}

//_______________________________________________________________________________
mfFilterFactor::mfFilterFactor ()
{}

mfFilterFactor::~mfFilterFactor ()
{}

EXP std::ostream& operator<< (std::ostream& os, const S_mfFilterFactor& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NULL ***" << endl;
  }

  return os;
}

//_______________________________________________________________________________
S_mfFilterSimpleFactor mfFilterSimpleFactor::create (
  std::string theString,
  Bool        negated)
{
  mfFilterSimpleFactor* obj =
    new mfFilterSimpleFactor (
      theString,
      negated);
  assert (obj != nullptr);
  return obj;
}

mfFilterSimpleFactor::mfFilterSimpleFactor (
  std::string theString,
  Bool        negated)
{
  fString = theString;
  fNegated = negated;
}

mfFilterSimpleFactor::~mfFilterSimpleFactor ()
{}

Bool mfFilterSimpleFactor::stringIsFoundInSimpleFactor (std::string theString) const
{
  Bool result;

/*
  // a strings list to collect the results
  std::list<S_oahFindMatch> foundMatchesList;

  // delegate this to the handler
  fetchAtomUpLinkToHandler ()->
    findInFindableElement (
      mfStringToLowerCase (theString),
      foundMatchesList,
      os);

  //  print the found strings
  size_t foundMatchesListSize =
    foundMatchesList.size ();

  int saveIndent = gIndenter.getIndentation ();

  gIndenter.resetToZero ();

  os <<
    mfSingularOrPlural (
      foundMatchesListSize, "occurrence", "occurrences") <<
    " of string \"" <<
    theString <<
    "\" " <<
    mfSingularOrPluralWithoutNumber (
      foundMatchesListSize, "has", "have") <<
    " been found";

  if (foundMatchesListSize) {
    os <<
      ":" <<
      std::endl;

    ++gIndenter;

    std::list<S_oahFindMatch>::const_iterator
      iBegin = foundMatchesList.begin (),
      iEnd   = foundMatchesList.end (),
      i      = iBegin;

    int counter = 0;

    for ( ; ; ) {
      oahFindMatch* theFindMatch = (*i);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        theFindMatch != nullptr,
        "theFindMatch is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      std::string
        elementName =
          theFindMatch->
            getElementName (),
        foundString =
          theFindMatch->
            getFoundString (),
        containingFindableElementInfo =
          theFindMatch->
            getContainingFindableElementInfo ();

      ++counter;

      os << std::right <<
        std::setw (2) << counter << ": " << containingFindableElementInfo <<
        std::endl;

      ++gIndenter;

      os << elementName << std::endl;

      // indent a bit more for readability
      gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

      gIndenter.indentMultiLineString (
        foundString,
        os);

      // unindent a bit more for readability
      gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
      --gIndenter;

      if (++i == iEnd) break;

      os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os << std::endl;
  }
*/

  if (fNegated) {
    result = ! result;
  }

  return result;
}

std::string mfFilterSimpleFactor::asString () const
{
  stringstream ss;

  if (fNegated) {
    ss <<
      "! ";
  }

  ss <<
    fString;

  return ss.str ();
}

std::string mfFilterSimpleFactor::asStringWithFullParentheses () const
{
  stringstream ss;

  if (fNegated) {
    ss <<
      "( ! ";
  }
  else {
    ss <<
    '(';
  }

  ss <<
    fString;

  if (fNegated) {
    ss <<
      ") ";
  }
  else {
    ss <<
      ")";
  }

  return ss.str ();
}

void mfFilterSimpleFactor::print (std::ostream& os) const
{
  os <<
    "SimpleFactor:" <<
    std::endl;

  ++gIndenter;
  os <<
    asString () <<
    endl;
  --gIndenter;
}

std::ostream& operator<< (std::ostream& os, const S_mfFilterSimpleFactor& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NULL ***" << endl;
  }

  return os;
}

//_______________________________________________________________________________
S_mfFilterSubExpression mfFilterSubExpression::create (
  S_mfFilterExpression expression)
{
  mfFilterSubExpression* obj =
    new mfFilterSubExpression (
      expression);
  assert (obj != nullptr);
  return obj;
}

mfFilterSubExpression::mfFilterSubExpression (
  S_mfFilterExpression expression)
{
  fExpression = expression;
}

mfFilterSubExpression::~mfFilterSubExpression ()
{}

Bool mfFilterSubExpression::stringIsFoundInSubExpression (std::string theString) const
{
  return
    fExpression->
      stringIsFoundInExpression (theString);
}

std::string mfFilterSubExpression::asString () const
{
  stringstream ss;

  ss <<
    '(' <<
    fExpression->asString () <<
    ')';

  return ss.str ();
}

std::string mfFilterSubExpression::asStringWithFullParentheses () const
{
  return asString ();
}

void mfFilterSubExpression::print (std::ostream& os) const
{
  os <<
    "SubExpression:" <<
    std::endl;

  ++gIndenter;
  os <<
    fExpression <<
    std::endl;
  --gIndenter;
}

std::ostream& operator<< (std::ostream& os, const S_mfFilterSubExpression& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NULL ***" << endl;
  }

  return os;
}

//______________________________________________________________________________
void testMfFind (std::ostream& os)
{
  S_mfFilterExpression
    theExpression =
      mfFilterExpression::create ();

  {
    S_mfFilterTerm
      term =
        mfFilterTerm::create ();

    {
      S_mfFilterFactor
        simpleFactor =
          mfFilterSimpleFactor::create (
            "xml", true);

      term->appendFactor (simpleFactor);
    }

    {
      S_mfFilterFactor
        simpleFactor =
          mfFilterSimpleFactor::create (
            "lilypond", false);

      term->appendFactor (simpleFactor);
    }

    theExpression->appendTerm (term);
  }

  {
    S_mfFilterTerm
      term =
        mfFilterTerm::create ();

    {
      S_mfFilterFactor
        simpleFactor =
          mfFilterSimpleFactor::create (
            "ignore", true);

      term->appendFactor (simpleFactor);
    }

    {
      S_mfFilterFactor
        simpleFactor =
          mfFilterSimpleFactor::create (
            "msr", false);

      term->appendFactor (simpleFactor);
    }

    theExpression->appendTerm (term);
  }

  os <<
    "theExpression:" <<
    std::endl;

  ++gIndenter;
  os <<
    theExpression <<
    std::endl;
  --gIndenter;

  os <<
    "as string:" <<
    std::endl;

  ++gIndenter;
  os <<
    theExpression->asString () <<
    std::endl <<
    std::endl;
  --gIndenter;

  os <<
    "as string with full parentheses:" <<
    std::endl;

  ++gIndenter;
  os <<
    theExpression->asStringWithFullParentheses () <<
    std::endl;
  --gIndenter;
}

//______________________________________________________________________________
// explicit mfStack template specializations definitions
template <>
mfStack<S_mfFilterExpression>::mfStack ()
{}

template <>
mfStack<S_mfFilterExpression>::~mfStack ()
{}

template <>
void mfStack<S_mfFilterExpression>::print (std::ostream& os) const
{}

template <>
void mfStack<S_mfFilterExpression>::printWithContext (
  const std::string& context,
	char               evidencer,
  std::ostream&      os) const
{}

//______________________________________________________________________________
// explicit mfList template specializations definitions
template <>
mfList<S_mfFilterExpression>::mfList ()
{}

template <>
mfList<S_mfFilterExpression>::~mfList ()
{}

// //______________________________________________________________________________
// void initializemfFilterBasicTypes ()
// {
//   // protect library against multiple initializations
//   static Bool pPrivateThisMethodHasBeenRun (false);
//
//   if (! pPrivateThisMethodHasBeenRun) {
// #ifdef MF_TRACE_IS_ENABLED
//     if (gEarlyOptions.getEarlyTraceOah () && ! gEarlyOptions.getEarlyQuietOption ()) {
//       gLog <<
//         "Initializing mfFilter basic types handling" <<
//         endl;
//     }
// #endif // MF_TRACE_IS_ENABLED
//
//     pPrivateThisMethodHasBeenRun = true;
//   }
// }
