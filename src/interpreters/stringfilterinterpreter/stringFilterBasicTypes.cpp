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

#include "stringFilterBasicTypes.h"
#include "stringFilterDriver.h"

#include "stringFilterInterpreterOah.h"

#include "stringFilterWae.h"


using namespace std;

using namespace MusicFormats;

//_______________________________________________________________________________
/* this class is purely virtual
*/
stringFilterNode::stringFilterNode ()
{}

stringFilterNode::~stringFilterNode ()
{}

std::ostream& operator<< (std::ostream& os, const S_stringFilterNode& elt)
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
S_stringFilterOr stringFilterOr::create (
  const S_stringFilterNode& leftOperand,
  const S_stringFilterNode& rightOperand)
{
  stringFilterOr* obj =
    new stringFilterOr (
      leftOperand,
      rightOperand);
  assert (obj != nullptr);
  return obj;
}

stringFilterOr::stringFilterOr (
  const S_stringFilterNode& leftOperand,
  const S_stringFilterNode& rightOperand)
{
  fLeftOperand = leftOperand;
  fRightOperand = rightOperand;
}

stringFilterOr::~stringFilterOr ()
{}

Bool stringFilterOr::stringMatches (const std::string& theString) const
{
  return
    fLeftOperand->stringMatches (theString)
      ||
    fRightOperand->stringMatches (theString);
}

std::string stringFilterOr::asString () const
{
  stringstream ss;

  ss <<
    fLeftOperand->asString () <<
    " | " <<
    fRightOperand->asString ();

  return ss.str ();
}

std::string stringFilterOr::asStringWithFullParentheses () const
{
  stringstream ss;

  ss <<
    '(' <<
    fLeftOperand->asString () <<
    " | " <<
    fRightOperand->asString () <<
    ')';

  return ss.str ();
}

void stringFilterOr::print (std::ostream& os) const
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

std::ostream& operator<< (std::ostream& os, const S_stringFilterOr& elt)
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
S_stringFilterXor stringFilterXor::create (
  const S_stringFilterNode& leftOperand,
  const S_stringFilterNode& rightOperand)
{
  stringFilterXor* obj =
    new stringFilterXor (
      leftOperand,
      rightOperand);
  assert (obj != nullptr);
  return obj;
}

stringFilterXor::stringFilterXor (
  const S_stringFilterNode& leftOperand,
  const S_stringFilterNode& rightOperand)
{
  fLeftOperand = leftOperand;
  fRightOperand = rightOperand;
}

stringFilterXor::~stringFilterXor ()
{}

Bool stringFilterXor::stringMatches (const std::string& theString) const
{
  return
    fLeftOperand->stringMatches (theString)
      ^
    fRightOperand->stringMatches (theString);
}

std::string stringFilterXor::asString () const
{
  stringstream ss;

  ss <<
    fLeftOperand->asString () <<
    " ^ " <<
    fRightOperand->asString ();

  return ss.str ();
}

std::string stringFilterXor::asStringWithFullParentheses () const
{
  stringstream ss;

  ss <<
    '(' <<
    fLeftOperand->asString () <<
    " ^ " <<
    fRightOperand->asString () <<
    ')';

  return ss.str ();
}

void stringFilterXor::print (std::ostream& os) const
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

std::ostream& operator<< (std::ostream& os, const S_stringFilterXor& elt)
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
S_stringFilterAnd stringFilterAnd::create (
  const S_stringFilterNode& leftOperand,
  const S_stringFilterNode& rightOperand)
{
  stringFilterAnd* obj =
    new stringFilterAnd (
      leftOperand,
      rightOperand);
  assert (obj != nullptr);
  return obj;
}

stringFilterAnd::stringFilterAnd (
  const S_stringFilterNode& leftOperand,
  const S_stringFilterNode& rightOperand)
{
  fLeftOperand = leftOperand;
  fRightOperand = rightOperand;
}

stringFilterAnd::~stringFilterAnd ()
{}

Bool stringFilterAnd::stringMatches (const std::string& theString) const
{
  return
    fLeftOperand->stringMatches (theString)
      &&
    fRightOperand->stringMatches (theString);
}

std::string stringFilterAnd::asString () const
{
  stringstream ss;

  ss <<
    fLeftOperand->asString () <<
    " & " <<
    fRightOperand->asString ();

  return ss.str ();
}

std::string stringFilterAnd::asStringWithFullParentheses () const
{
  stringstream ss;

  ss <<
    '(' <<
    fLeftOperand->asString () <<
    " & " <<
    fRightOperand->asString () <<
    ')';

  return ss.str ();
}

void stringFilterAnd::print (std::ostream& os) const
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

std::ostream& operator<< (std::ostream& os, const S_stringFilterAnd& elt)
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
S_stringFilterNot stringFilterNot::create (
  const S_stringFilterNode& operand)
{
  stringFilterNot* obj =
    new stringFilterNot (
      operand);
  assert (obj != nullptr);
  return obj;
}

stringFilterNot::stringFilterNot (
  const S_stringFilterNode& operand)
{
  fOperand = operand;
}

stringFilterNot::~stringFilterNot ()
{}

Bool stringFilterNot::stringMatches (const std::string& theString) const
{
  return
    fOperand->stringMatches (theString);
}

std::string stringFilterNot::asString () const
{
  stringstream ss;

  ss <<
    " ! " <<
    fOperand->asString ();

  return ss.str ();
}

std::string stringFilterNot::asStringWithFullParentheses () const
{
  stringstream ss;

  ss <<
    "( ! " <<
    fOperand->asString () <<
    ')';

  return ss.str ();
}

void stringFilterNot::print (std::ostream& os) const
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

std::ostream& operator<< (std::ostream& os, const S_stringFilterNot& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NULL ***" << endl;
  }

  return os;
}

// //_______________________________________________________________________________
// S_stringFilterExpression stringFilterExpression::create ()
// {
//   stringFilterExpression* obj =
//     new stringFilterExpression ();
//   assert (obj != nullptr);
//   return obj;
// }
//
// stringFilterExpression::stringFilterExpression ()
// {}
//
// stringFilterExpression::~stringFilterExpression ()
// {}
//
// Bool stringFilterExpression::stringMatches (const std::string& theString) const
// {
//   Bool result;
//
//   for (S_stringFilterTerm term : fORedTermsList) {
//     if (term->stringMatches (theString)) {
//       result = true;
//       break;
//     }
//   } // for
//
//   return result;
// }
//
// std::string stringFilterExpression::asString () const
// {
//   stringstream ss;
//
//   int theORedTermsListSize = fORedTermsList.size ();
//   int counter = 0;
//
//   for (S_stringFilterTerm term : fORedTermsList) {
//     counter++;
//
//     ss << term->asString ();
//
//     if (counter < theORedTermsListSize) {
//       ss << " | ";
//     }
//   } // for
//
//   return ss.str ();
// }
//
// std::string stringFilterExpression::asStringWithFullParentheses () const
// {
//   stringstream ss;
//
//   int theORedTermsListSize = fORedTermsList.size ();
//   int counter = 0;
//
//   ss << '(';
//
//   for (S_stringFilterTerm term : fORedTermsList) {
//     counter++;
//
//     ss << term->asStringWithFullParentheses ();
//
//     if (counter < theORedTermsListSize) {
//       ss << " | ";
//     }
//   } // for
//
//   ss << ')';
//
//   return ss.str ();
// }
//
// void stringFilterExpression::print (std::ostream& os) const
// {
//   os <<
//     "Expression" <<
//     endl;
//
//   int theORedFactorsListSize = fORedTermsList.size ();
//   int counter = 0;
//
//   ++gIndenter;
//
//   for (S_stringFilterTerm term : fORedTermsList) {
//     counter++;
//
//     os << term;
//
//     if (counter < theORedFactorsListSize) {
//       os <<
//         '|' <<
//         std::endl;
//     }
//   } // for
//
//   --gIndenter;
// }
//
// std::ostream& operator<< (std::ostream& os, const S_stringFilterExpression& elt)
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
//
// //_______________________________________________________________________________
// S_stringFilterTerm stringFilterTerm::create ()
// {
//   stringFilterTerm* obj =
//     new stringFilterTerm ();
//   assert (obj != nullptr);
//   return obj;
// }
//
// stringFilterTerm::stringFilterTerm ()
// {}
//
// stringFilterTerm::~stringFilterTerm ()
// {}
//
// Bool stringFilterTerm::stringMatches (const std::string& theString) const
// {
//   Bool result;
//
//   for (S_stringFilterFactor factor : fANDedFactorsList) {
//     if (factor->stringMatches (theString)) {
//       result = true;
//       break;
//     }
//   } // for
//
//   return result;
// }
//
// std::string stringFilterTerm::asString () const
// {
//   stringstream ss;
//
//   int theANDedFactorsListSize = fANDedFactorsList.size ();
//   int counter = 0;
//
//   for (S_stringFilterFactor factor : fANDedFactorsList) {
//     counter++;
//
//     ss << factor->asString ();
//
//     if (counter < theANDedFactorsListSize) {
//       ss << " & ";
//     }
//   } // for
//
//   return ss.str ();
// }
//
// std::string stringFilterTerm::asStringWithFullParentheses () const
// {
//   stringstream ss;
//
//   int theANDedFactorsListSize = fANDedFactorsList.size ();
//   int counter = 0;
//
//   ss << '(';
//
//   for (S_stringFilterFactor factor : fANDedFactorsList) {
//     counter++;
//
//     ss << factor->asStringWithFullParentheses ();
//
//     if (counter < theANDedFactorsListSize) {
//       ss << " & ";
//     }
//   } // for
//
//   ss << ')';
//
//   return ss.str ();
// }
//
// void stringFilterTerm::print (std::ostream& os) const
// {
//   os <<
//     "Term" <<
//     endl;
//
//   int theAndedFactorsListSize = fANDedFactorsList.size ();
//   int counter = 0;
//
//   ++gIndenter;
//
//   for (S_stringFilterFactor factor : fANDedFactorsList) {
//     counter++;
//
//     os << factor;
//
//     if (counter < theAndedFactorsListSize) {
//       os <<
//         '&' <<
//         std::endl;
//     }
//   } // for
//
//   --gIndenter;
// }
//
// std::ostream& operator<< (std::ostream& os, const S_stringFilterTerm& elt)
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
//
// //_______________________________________________________________________________
// stringFilterFactor::stringFilterFactor ()
// {}
//
// stringFilterFactor::~stringFilterFactor ()
// {}
//
// EXP std::ostream& operator<< (std::ostream& os, const S_stringFilterFactor& elt)
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
S_stringFilterString stringFilterString::create (
  const std::string& theString)
{
  stringFilterString* obj =
    new stringFilterString (
      theString);
  assert (obj != nullptr);
  return obj;
}

stringFilterString::stringFilterString (
  const std::string& theString)
{
  fString = theString;
}

stringFilterString::~stringFilterString ()
{}

Bool stringFilterString::stringMatches (const std::string& theString) const
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

  return result;
}

std::string stringFilterString::asString () const
{
  return fString;
}

std::string stringFilterString::asStringWithFullParentheses () const
{
  stringstream ss;

  ss <<
    '(' <<
    fString <<
    ")";

  return ss.str ();
}

void stringFilterString::print (std::ostream& os) const
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

std::ostream& operator<< (std::ostream& os, const S_stringFilterString& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NULL ***" << endl;
  }

  return os;
}

// //_______________________________________________________________________________
// S_stringFilterSubExpression stringFilterSubExpression::create (
//   S_stringFilterExpression expression)
// {
//   stringFilterSubExpression* obj =
//     new stringFilterSubExpression (
//       expression);
//   assert (obj != nullptr);
//   return obj;
// }
//
// stringFilterSubExpression::stringFilterSubExpression (
//   S_stringFilterExpression expression)
// {
//   fExpression = expression;
// }
//
// stringFilterSubExpression::~stringFilterSubExpression ()
// {}
//
// Bool stringFilterSubExpression::stringMatches (const std::string& theString) const
// {
//   return
//     fExpression->
//       stringMatches (theString);
// }
//
// std::string stringFilterSubExpression::asString () const
// {
//   stringstream ss;
//
//   ss <<
//     '(' <<
//     fExpression->asString () <<
//     ')';
//
//   return ss.str ();
// }
//
// std::string stringFilterSubExpression::asStringWithFullParentheses () const
// {
//   return asString ();
// }
//
// void stringFilterSubExpression::print (std::ostream& os) const
// {
//   os <<
//     "SubExpression:" <<
//     std::endl;
//
//   ++gIndenter;
//   os <<
//     fExpression <<
//     std::endl;
//   --gIndenter;
// }
//
// std::ostream& operator<< (std::ostream& os, const S_stringFilterSubExpression& elt)
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

//______________________________________________________________________________
void testFilter (std::ostream& os)
{
//   S_stringFilterNode
//     theExpression =
//       stringFilterAnd::create ();
//
//   {
//     S_stringFilterNode
//       term =
//         stringFilterNode::create ();
//
//     {
//       S_stringFilterNode
//         simpleFactor =
//           stringFilterString::create (
//             "xml");
//
//     }
//
//     {
//       S_stringFilterNode
//         simpleFactor =
//           stringFilterString::create (
//             "lilypond");
//
//     }
//
//   }
//
//   {
//     S_stringFilterNode
//       term =
//         stringFilterNode::create ();
//
//     {
//       S_stringFilterNode
//         simpleFactor =
//           stringFilterString::create (
//             "ignore");
//
//     }
//
//     {
//       S_stringFilterNode
//         simpleFactor =
//           stringFilterString::create (
//             "msr");
//
//     }
//
//   }
//
//   os <<
//     "theExpression:" <<
//     std::endl;
//
//   ++gIndenter;
//   os <<
//     theExpression <<
//     std::endl;
//   --gIndenter;
//
//   os <<
//     "as string:" <<
//     std::endl;
//
//   ++gIndenter;
//   os <<
//     theExpression->asString () <<
//     std::endl <<
//     std::endl;
//   --gIndenter;
//
//   os <<
//     "as string with full parentheses:" <<
//     std::endl;
//
//   ++gIndenter;
//   os <<
//     theExpression->asStringWithFullParentheses () <<
//     std::endl;
//   --gIndenter;
}

// //______________________________________________________________________________
// // explicit mfStack template specializations definitions
// template <>
// mfStack<S_stringFilterExpression>::mfStack ()
// {}
//
// template <>
// mfStack<S_stringFilterExpression>::~mfStack ()
// {}
//
// template <>
// void mfStack<S_stringFilterExpression>::print (std::ostream& os) const
// {}
//
// template <>
// void mfStack<S_stringFilterExpression>::printWithContext (
//   const std::string& context,
// 	char               evidencer,
//   std::ostream&      os) const
// {}

// //______________________________________________________________________________
// // explicit mfList template specializations definitions
// template <>
// mfList<S_stringFilterExpression>::mfList ()
// {}
//
// template <>
// mfList<S_stringFilterExpression>::~mfList ()
// {}

// //______________________________________________________________________________
// void initializestringFilterBasicTypes ()
// {
//   // protect library against multiple initializations
//   static Bool pPrivateThisMethodHasBeenRun (false);
//
//   if (! pPrivateThisMethodHasBeenRun) {
// #ifdef MF_TRACE_IS_ENABLED
//     if (gEarlyOptions.getEarlyTraceOah () && ! gEarlyOptions.getEarlyQuietOption ()) {
//       gLog <<
//         "Initializing stringFilter basic types handling" <<
//         endl;
//     }
// #endif // MF_TRACE_IS_ENABLED
//
//     pPrivateThisMethodHasBeenRun = true;
//   }
// }
