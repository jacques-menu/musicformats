/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

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

#include "stringMatcherBasicTypes.h"
#include "stringMatcherDriver.h"

#include "stringMatcherInterpreterOah.h"

#include "stringMatcherWae.h"


using namespace std;

using namespace MusicFormats;

//_______________________________________________________________________________
/* this class is purely virtual
*/
stringMatcherNode::stringMatcherNode ()
{}

stringMatcherNode::~stringMatcherNode ()
{}

std::ostream& operator<< (std::ostream& os, const S_stringMatcherNode& elt)
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
/* this class if purely virtual
S_stringMatcherMonadicOperator stringMatcherMonadicOperator::create (
  const S_stringMatcherNode& operand)
{
  stringMatcherMonadicOperator* obj =
    new stringMatcherMonadicOperator (
      operand);
  assert (obj != nullptr);
  return obj;
}
*/

stringMatcherMonadicOperator::stringMatcherMonadicOperator (
  const S_stringMatcherNode& operand)
{
  fOperand = operand;
}

stringMatcherMonadicOperator::~stringMatcherMonadicOperator ()
{}

// std::string stringMatcherMonadicOperator::asString () const
// {
//   stringstream ss;
//
//   ss <<
//     " ! " <<
//     fOperand->asString ();
//
//   return ss.str ();
// }
//
// std::string stringMatcherMonadicOperator::asStringWithFullParentheses () const
// {
//   stringstream ss;
//
//   ss <<
//     "( ! " <<
//     fOperand->asStringWithFullParentheses () <<
//     ')';
//
//   return ss.str ();
// }
//
// void stringMatcherMonadicOperator::print (std::ostream& os) const
// {
//   os <<
//     "Not" <<
//     endl;
//
//   ++gIndenter;
//
//   os <<
//     fOperand <<
//     std::endl;
//
//   --gIndenter;
// }

std::ostream& operator<< (std::ostream& os, const S_stringMatcherMonadicOperator& elt)
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
/* this class if purely virtual
S_stringMatcherOr stringMatcherDyadicOperator::create (
  const S_stringMatcherNode& leftOperand,
  const S_stringMatcherNode& rightOperand)
{
  stringMatcherDyadicOperator* obj =
    new stringMatcherDyadicOperator (
      leftOperand,
      rightOperand);
  assert (obj != nullptr);
  return obj;
}
*/

stringMatcherDyadicOperator::stringMatcherDyadicOperator (
  const S_stringMatcherNode& leftOperand,
  const S_stringMatcherNode& rightOperand)
{
  fLeftOperand = leftOperand;
  fRightOperand = rightOperand;
}

stringMatcherDyadicOperator::~stringMatcherDyadicOperator ()
{}

std::ostream& operator<< (std::ostream& os, const S_stringMatcherOr& elt)
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
S_stringMatcherNot stringMatcherNot::create (
  const S_stringMatcherNode& operand)
{
  stringMatcherNot* obj =
    new stringMatcherNot (
      operand);
  assert (obj != nullptr);
  return obj;
}

stringMatcherNot::stringMatcherNot (
  const S_stringMatcherNode& operand)
  : stringMatcherMonadicOperator (
      operand)
{}

stringMatcherNot::~stringMatcherNot ()
{}

Bool stringMatcherNot::stringMatches (const std::string& theString) const
{
  return
    ! fOperand->stringMatches (theString);
}

std::string stringMatcherNot::asString () const
{
  stringstream ss;

  ss <<
    "! " <<
    fOperand->asString ();

  return ss.str ();
}

std::string stringMatcherNot::asStringWithFullParentheses () const
{
  stringstream ss;

  ss <<
    "( ! " <<
    fOperand->asStringWithFullParentheses () <<
    ')';

  return ss.str ();
}

void stringMatcherNot::print (std::ostream& os) const
{
  os <<
    "Not" <<
    endl;

  ++gIndenter;

  os <<
    fOperand <<
    std::endl;

  --gIndenter;
}

std::ostream& operator<< (std::ostream& os, const S_stringMatcherNot& elt)
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
S_stringMatcherOr stringMatcherOr::create (
  const S_stringMatcherNode& leftOperand,
  const S_stringMatcherNode& rightOperand)
{
  stringMatcherOr* obj =
    new stringMatcherOr (
      leftOperand,
      rightOperand);
  assert (obj != nullptr);
  return obj;
}

stringMatcherOr::stringMatcherOr (
  const S_stringMatcherNode& leftOperand,
  const S_stringMatcherNode& rightOperand)
  : stringMatcherDyadicOperator (
      leftOperand,
      rightOperand)
{}

stringMatcherOr::~stringMatcherOr ()
{}

Bool stringMatcherOr::stringMatches (const std::string& theString) const
{
  return
    fLeftOperand->stringMatches (theString)
      ||
    fRightOperand->stringMatches (theString);
}

std::string stringMatcherOr::asString () const
{
  stringstream ss;

  ss <<
    fLeftOperand->asString () <<
    " | " <<
    fRightOperand->asString ();

  return ss.str ();
}

std::string stringMatcherOr::asStringWithFullParentheses () const
{
  stringstream ss;

  ss <<
    '(' <<
    fLeftOperand->asStringWithFullParentheses () <<
    " | " <<
    fRightOperand->asStringWithFullParentheses () <<
    ')';

  return ss.str ();
}

void stringMatcherOr::print (std::ostream& os) const
{
  os <<
    "Or" <<
    endl;

  ++gIndenter;

  os <<
    fLeftOperand <<
    std::endl <<
    "|" <<
    std::endl <<
    fLeftOperand <<
    std::endl;

  --gIndenter;
}

// std::ostream& operator<< (std::ostream& os, const S_stringMatcherOr& elt)
// {
//   if (elt) {
//     elt->print (os);
//   }
//   else {
//     os << "*** NULL ***" << endl;
//   }
//
//   return os;
// }

//_______________________________________________________________________________
S_stringMatcherXor stringMatcherXor::create (
  const S_stringMatcherNode& leftOperand,
  const S_stringMatcherNode& rightOperand)
{
  stringMatcherXor* obj =
    new stringMatcherXor (
      leftOperand,
      rightOperand);
  assert (obj != nullptr);
  return obj;
}

stringMatcherXor::stringMatcherXor (
  const S_stringMatcherNode& leftOperand,
  const S_stringMatcherNode& rightOperand)
  : stringMatcherDyadicOperator (
      leftOperand,
      rightOperand)
{}

stringMatcherXor::~stringMatcherXor ()
{}

Bool stringMatcherXor::stringMatches (const std::string& theString) const
{
  return
    fLeftOperand->stringMatches (theString)
      ^
    fRightOperand->stringMatches (theString);
}

std::string stringMatcherXor::asString () const
{
  stringstream ss;

  ss <<
    fLeftOperand->asString () <<
    " ^ " <<
    fRightOperand->asString ();

  return ss.str ();
}

std::string stringMatcherXor::asStringWithFullParentheses () const
{
  stringstream ss;

  ss <<
    '(' <<
    fLeftOperand->asStringWithFullParentheses () <<
    " ^ " <<
    fRightOperand->asStringWithFullParentheses () <<
    ')';

  return ss.str ();
}

void stringMatcherXor::print (std::ostream& os) const
{
  os <<
    "Xor" <<
    endl;

  ++gIndenter;

  os <<
    fLeftOperand <<
    std::endl <<
    "^" <<
    std::endl <<
    fLeftOperand <<
    std::endl;

  --gIndenter;
}

// std::ostream& operator<< (std::ostream& os, const S_stringMatcherXor& elt)
// {
//   if (elt) {
//     elt->print (os);
//   }
//   else {
//     os << "*** NULL ***" << endl;
//   }
//
//   return os;
// }

//_______________________________________________________________________________
S_stringMatcherAnd stringMatcherAnd::create (
  const S_stringMatcherNode& leftOperand,
  const S_stringMatcherNode& rightOperand)
{
  stringMatcherAnd* obj =
    new stringMatcherAnd (
      leftOperand,
      rightOperand);
  assert (obj != nullptr);
  return obj;
}

stringMatcherAnd::stringMatcherAnd (
  const S_stringMatcherNode& leftOperand,
  const S_stringMatcherNode& rightOperand)
  : stringMatcherDyadicOperator (
      leftOperand,
      rightOperand)
{}

stringMatcherAnd::~stringMatcherAnd ()
{}

Bool stringMatcherAnd::stringMatches (const std::string& theString) const
{
  return
    fLeftOperand->stringMatches (theString)
      &&
    fRightOperand->stringMatches (theString);
}

std::string stringMatcherAnd::asString () const
{
  stringstream ss;

  ss <<
    fLeftOperand->asString () <<
    " & " <<
    fRightOperand->asString ();

  return ss.str ();
}

std::string stringMatcherAnd::asStringWithFullParentheses () const
{
  stringstream ss;

  ss <<
    '(' <<
    fLeftOperand->asStringWithFullParentheses () <<
    " & " <<
    fRightOperand->asStringWithFullParentheses () <<
    ')';

  return ss.str ();
}

void stringMatcherAnd::print (std::ostream& os) const
{
  os <<
    "And" <<
    endl;

  ++gIndenter;

  os <<
    fLeftOperand <<
    std::endl <<
    "|" <<
    std::endl <<
    fLeftOperand <<
    std::endl;

  --gIndenter;
}

// std::ostream& operator<< (std::ostream& os, const S_stringMatcherAnd& elt)
// {
//   if (elt) {
//     elt->print (os);
//   }
//   else {
//     os << "*** NULL ***" << endl;
//   }
//
//   return os;
// }

//_______________________________________________________________________________
S_stringMatcherString stringMatcherString::create (
  const std::string& theString)
{
  stringMatcherString* obj =
    new stringMatcherString (
      theString);
  assert (obj != nullptr);
  return obj;
}

stringMatcherString::stringMatcherString (
  const std::string& theString)
{
  fString = theString;
}

stringMatcherString::~stringMatcherString ()
{}

Bool stringMatcherString::stringMatches (const std::string& theString) const
{
  Bool result;

/*
  // a strings list to collect the results
  std::list<S_oahFindMatch> foundElementsList;

  // delegate this to the handler
  fetchAtomUpLinkToHandler ()->
    findInFindableElement (
      mfStringToLowerCase (theString),
      foundElementsList,
      os);

  //  print the found strings
  size_t foundMatchesListSize =
    foundElementsList.size ();

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
      iBegin = foundElementsList.begin (),
      iEnd   = foundElementsList.end (),
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

  return result;
}

std::string stringMatcherString::asString () const
{
  return fString;
}

std::string stringMatcherString::asStringWithFullParentheses () const
{
  stringstream ss;

  ss <<
    '(' <<
    fString <<
    ")";

  return ss.str ();
}

void stringMatcherString::print (std::ostream& os) const
{
  os <<
    "stringMatcherString:" <<
    std::endl;

  ++gIndenter;
  os <<
    asString () <<
    endl;
  --gIndenter;
}

// std::ostream& operator<< (std::ostream& os, const S_stringMatcherString& elt)
// {
//   if (elt) {
//     elt->print (os);
//   }
//   else {
//     os << "*** NULL ***" << endl;
//   }
//
//   return os;
// }

// //______________________________________________________________________________
// // explicit mfStack template specializations definitions
// template <>
// mfStack<S_stringMatcherExpression>::mfStack ()
// {}
//
// template <>
// mfStack<S_stringMatcherExpression>::~mfStack ()
// {}
//
// template <>
// void mfStack<S_stringMatcherExpression>::print (std::ostream& os) const
// {}
//
// template <>
// void mfStack<S_stringMatcherExpression>::printWithContext (
//   const std::string& context,
// 	char               evidencer,
//   std::ostream&      os) const
// {}

// //______________________________________________________________________________
// // explicit mfList template specializations definitions
// template <>
// mfList<S_stringMatcherExpression>::mfList ()
// {}
//
// template <>
// mfList<S_stringMatcherExpression>::~mfList ()
// {}

// //______________________________________________________________________________
// void initializestringMatcherBasicTypes ()
// {
//   // protect library against multiple initializations
//   static Bool pPrivateThisMethodHasBeenRun (false);
//
//   if (! pPrivateThisMethodHasBeenRun) {
// #ifdef MF_TRACE_IS_ENABLED
//     if (gEarlyOptions.getEarlyTraceOah () && ! gEarlyOptions.getEarlyQuietOption ()) {
//       gLog <<
//         "Initializing stringMatcher basic types handling" <<
//         endl;
//     }
// #endif // MF_TRACE_IS_ENABLED
//
//     pPrivateThisMethodHasBeenRun = true;
//   }
// }
