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

#include "mfFindBasicTypes.h"
#include "mfFindDriver.h"

#include "mfFindInterpreterOah.h"

#include "mfFindWae.h"


using namespace std;

using namespace MusicFormats;

//_______________________________________________________________________________
S_mfFindExpression mfFindExpression::create ()
{
  mfFindExpression* obj =
    new mfFindExpression ();
  assert (obj != nullptr);
  return obj;
}

mfFindExpression::mfFindExpression ()
{}

mfFindExpression::~mfFindExpression ()
{}

void mfFindExpression::appendTerm (
  S_mfFindTerm& term)
{
  fORedTermsList.push_back (term);
};

Bool mfFindExpression::stringIsFoundInExpression (std::string theString) const
{
  Bool result;

  for (S_mfFindTerm term : fORedTermsList) {
    if (term->stringIsFoundInTerm (theString)) {
      result = true;
      break;
    }
  } // for

  return result;
}

std::string mfFindExpression::asString () const
{
  stringstream ss;

  int theORedTermsListSize = fORedTermsList.size ();
  int counter = 0;

  for (S_mfFindTerm term : fORedTermsList) {
    counter++;

    ss << term->asString ();

    if (counter < theORedTermsListSize) {
      ss << " | ";
    }
  } // for

  return ss.str ();
}

std::string mfFindExpression::asStringWithFullParentheses () const
{
  stringstream ss;

  int theORedTermsListSize = fORedTermsList.size ();
  int counter = 0;

  ss << '(';

  for (S_mfFindTerm term : fORedTermsList) {
    counter++;

    ss << term->asStringWithFullParentheses ();

    if (counter < theORedTermsListSize) {
      ss << " | ";
    }
  } // for

  ss << ')';

  return ss.str ();
}

void mfFindExpression::print (std::ostream& os) const
{
  os <<
    "Expression" <<
    endl;

  int theORedFactorsListSize = fORedTermsList.size ();
  int counter = 0;

  ++gIndenter;

  for (S_mfFindTerm term : fORedTermsList) {
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

std::ostream& operator<< (std::ostream& os, const S_mfFindExpression& elt)
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
S_mfFindTerm mfFindTerm::create ()
{
  mfFindTerm* obj =
    new mfFindTerm ();
  assert (obj != nullptr);
  return obj;
}

mfFindTerm::mfFindTerm ()
{}

mfFindTerm::~mfFindTerm ()
{}

void mfFindTerm::appendFactor (
  S_mfFindFactor& factor)
{
  fANDedFactorsList.push_back (factor);
};

Bool mfFindTerm::stringIsFoundInTerm (std::string theString) const
{
  Bool result;

  for (S_mfFindFactor factor : fANDedFactorsList) {
    if (factor->stringIsFoundInFactor (theString)) {
      result = true;
      break;
    }
  } // for

  return result;
}

std::string mfFindTerm::asString () const
{
  stringstream ss;

  int theANDedFactorsListSize = fANDedFactorsList.size ();
  int counter = 0;

  for (S_mfFindFactor factor : fANDedFactorsList) {
    counter++;

    ss << factor->asString ();

    if (counter < theANDedFactorsListSize) {
      ss << " & ";
    }
  } // for

  return ss.str ();
}

std::string mfFindTerm::asStringWithFullParentheses () const
{
  stringstream ss;

  int theANDedFactorsListSize = fANDedFactorsList.size ();
  int counter = 0;

  ss << '(';

  for (S_mfFindFactor factor : fANDedFactorsList) {
    counter++;

    ss << factor->asStringWithFullParentheses ();

    if (counter < theANDedFactorsListSize) {
      ss << " & ";
    }
  } // for

  ss << ')';

  return ss.str ();
}

void mfFindTerm::print (std::ostream& os) const
{
  os <<
    "Term" <<
    endl;

  int theAndedFactorsListSize = fANDedFactorsList.size ();
  int counter = 0;

  ++gIndenter;

  for (S_mfFindFactor factor : fANDedFactorsList) {
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

std::ostream& operator<< (std::ostream& os, const S_mfFindTerm& elt)
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
mfFindFactor::mfFindFactor ()
{}

mfFindFactor::~mfFindFactor ()
{}

EXP std::ostream& operator<< (std::ostream& os, const S_mfFindFactor& elt)
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
S_mfFindSimpleFactor mfFindSimpleFactor::create (
  std::string theString,
  Bool        negated)
{
  mfFindSimpleFactor* obj =
    new mfFindSimpleFactor (
      theString,
      negated);
  assert (obj != nullptr);
  return obj;
}

mfFindSimpleFactor::mfFindSimpleFactor (
  std::string theString,
  Bool        negated)
{
  fString = theString;
  fNegated = negated;
}

mfFindSimpleFactor::~mfFindSimpleFactor ()
{}

Bool mfFindSimpleFactor::stringIsFoundInSimpleFactor (std::string theString) const
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

std::string mfFindSimpleFactor::asString () const
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

std::string mfFindSimpleFactor::asStringWithFullParentheses () const
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

void mfFindSimpleFactor::print (std::ostream& os) const
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

std::ostream& operator<< (std::ostream& os, const S_mfFindSimpleFactor& elt)
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
S_mfFindSubExpression mfFindSubExpression::create (
  S_mfFindExpression expression)
{
  mfFindSubExpression* obj =
    new mfFindSubExpression (
      expression);
  assert (obj != nullptr);
  return obj;
}

mfFindSubExpression::mfFindSubExpression (
  S_mfFindExpression expression)
{
  fExpression = expression;
}

mfFindSubExpression::~mfFindSubExpression ()
{}

Bool mfFindSubExpression::stringIsFoundInSubExpression (std::string theString) const
{
  return
    fExpression->
      stringIsFoundInExpression (theString);
}

std::string mfFindSubExpression::asString () const
{
  stringstream ss;

  ss <<
    '(' <<
    fExpression->asString () <<
    ')';

  return ss.str ();
}

std::string mfFindSubExpression::asStringWithFullParentheses () const
{
  return asString ();
}

void mfFindSubExpression::print (std::ostream& os) const
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

std::ostream& operator<< (std::ostream& os, const S_mfFindSubExpression& elt)
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
  S_mfFindExpression
    theExpression =
      mfFindExpression::create ();

  {
    S_mfFindTerm
      term =
        mfFindTerm::create ();

    {
      S_mfFindFactor
        simpleFactor =
          mfFindSimpleFactor::create (
            "xml", true);

      term->appendFactor (simpleFactor);
    }

    {
      S_mfFindFactor
        simpleFactor =
          mfFindSimpleFactor::create (
            "lilypond", false);

      term->appendFactor (simpleFactor);
    }

    theExpression->appendTerm (term);
  }

  {
    S_mfFindTerm
      term =
        mfFindTerm::create ();

    {
      S_mfFindFactor
        simpleFactor =
          mfFindSimpleFactor::create (
            "ignore", true);

      term->appendFactor (simpleFactor);
    }

    {
      S_mfFindFactor
        simpleFactor =
          mfFindSimpleFactor::create (
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
mfStack<S_mfFindExpression>::mfStack ()
{}

template <>
mfStack<S_mfFindExpression>::~mfStack ()
{}

//______________________________________________________________________________
// explicit mfList template specializations definitions
template <>
mfList<S_mfFindExpression>::mfList ()
{}

template <>
mfList<S_mfFindExpression>::~mfList ()
{}

// //______________________________________________________________________________
// void initializemfFindBasicTypes ()
// {
//   // protect library against multiple initializations
//   static Bool pPrivateThisMethodHasBeenRun (false);
//
//   if (! pPrivateThisMethodHasBeenRun) {
// #ifdef MF_TRACE_IS_ENABLED
//     if (gEarlyOptions.getEarlyTraceOah () && ! gEarlyOptions.getEarlyQuietOption ()) {
//       gLog <<
//         "Initializing mfFind basic types handling" <<
//         endl;
//     }
// #endif // MF_TRACE_IS_ENABLED
//
//     pPrivateThisMethodHasBeenRun = true;
//   }
// }
