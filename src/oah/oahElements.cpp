/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "mfPreprocessorSettings.h"
#include "mfStringsHandling.h"

#include "oahOah.h"

#include "oahConstants.h"
#include "oahEarlyOptions.h"

#include "oahElements.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string oahElementValueKindAsString (
  oahElementValueKind elementValueKind)
{
  std::string result;

  switch (elementValueKind) {
    case oahElementValueKind::kElementValue_UNKNOWN_:
      result = "*kElementValue_UNKNOWN_*";
      break;
    case oahElementValueKind::kElementValueLess:
      result = "kElementValueLess";
      break;
//     case oahElementValueKind::kElementValueImplicit:
//       result = "kElementValueImplicit";
//       break;
    case oahElementValueKind::kElementValueFitted:
      result = "kElementValueFitted";
      break;
    case oahElementValueKind::kElementValueDefaulted:
      result = "kElementValueDefaulted";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const oahElementValueKind& elt)
{
  os << oahElementValueKindAsString (elt);
  return os;
}

std::string oahElementVisibilityKindAsString (
  oahElementVisibilityKind elementVisibilityKind)
{
  std::string result;

  switch (elementVisibilityKind) {
    case oahElementVisibilityKind::kElementVisibilityNone:
      result = "kElementVisibilityNone";
      break;

    case oahElementVisibilityKind::kElementVisibilityWhole:
      result = "kElementVisibilityWhole";
      break;

    case oahElementVisibilityKind::kElementVisibilityHeaderOnly:
      result = "kElementVisibilityHeaderOnly";
      break;

    case oahElementVisibilityKind::kElementVisibilityHidden:
      result = "kElementVisibilityHidden";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const oahElementVisibilityKind& elt)
{
  os << oahElementVisibilityKindAsString (elt);
  return os;
}

std::string oahElementHelpOnlyKindAsString (
  oahElementHelpOnlyKind elementHelpOnlyKind)
{
  std::string result;

  switch (elementHelpOnlyKind) {
    case oahElementHelpOnlyKind::kElementHelpOnlyYes:
      result = "kElementHelpOnlyYes";
      break;
    case oahElementHelpOnlyKind::kElementHelpOnlyNo:
      result = "kElementHelpOnlyNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const oahElementHelpOnlyKind& elt)
{
  os << oahElementHelpOnlyKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_oahFindStringMatch oahFindStringMatch::create (
  const std::string& elementName,
  const std::string& foundString,
  const std::string& containingFindableElementInfo)
{
  oahFindStringMatch* obj = new
    oahFindStringMatch (
      elementName,
      foundString,
      containingFindableElementInfo);
  assert (obj != nullptr);
  return obj;
}

oahFindStringMatch::oahFindStringMatch (
  const std::string& elementName,
  const std::string& foundString,
  const std::string& containingFindableElementInfo)
    : fElementName (elementName),
      fFoundString (foundString),
      fContainingFindableElementInfo (containingFindableElementInfo)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating oahFindStringMatch" <<
      ", fFoundString: " << fFoundString <<
      ", fContainingFindableElementInfo: " <<
      fContainingFindableElementInfo;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

oahFindStringMatch::~oahFindStringMatch ()
{}

void oahFindStringMatch::print (std::ostream& os) const
{
  os <<
    "oahFindStringMatch:" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 22;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fElementName" << ": " << fElementName <<
    std::endl <<
    "fFoundString" << ": " << fFoundString <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fContainingFindableElementInfo" << ": " << fContainingFindableElementInfo <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_oahFindStringMatch& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const oahFindStringMatch& elt)
{
  elt.print (os);
  return os;
}

std::ostream& operator << (std::ostream& os, const oahFindStringMatch* elt)
{
  elt->print (os);
  return os;
}

//__________________________xg____________________________________________________
/* this class is purely virtual
S_oahFindableElement oahFindableElement::create ()
{
  oahFindableElement* obj =
    new oahFindableElement ();
  assert (obj != nullptr);
  return obj;
}
*/

oahFindableElement::oahFindableElement ()
{}

oahFindableElement::~oahFindableElement ()
{}

void oahFindableElement::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahFindableElement& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const oahFindableElement& elt)
{
  elt.print (os);
  return os;
}

//______________________________________________________________________________
/* this class is purely virtual
S_oahElement oahElement::create (
  const std::string&       longName,
  const std::string&       shortName,
  const std::string&       description,
  oahElementValueKind      elementValueKind,
  oahElementVisibilityKind elementVisibilityKind)
{
  oahElement* obj = new
    oahElement (
      longName,
      shortName,
      description,
      elementValueKind,
      elementVisibilityKind);
  assert (obj != nullptr);
  return obj;
}
*/

oahElement::oahElement (
  const std::string&       longName,
  const std::string&       shortName,
  const std::string&       description,
  oahElementValueKind      elementValueKind,
  oahElementVisibilityKind elementVisibilityKind)
{
  fLongName = longName;
  fShortName = shortName;
  fDescription = description;

  fElementValueKind = elementValueKind;

  fElementHelpOnlyKind =
    oahElementHelpOnlyKind::kElementHelpOnlyNo; // default value

  fElementVisibilityKind = elementVisibilityKind;
}

oahElement::oahElement ()
{
  fLongName = "";
  fShortName = "";
  fDescription = "";

  fElementValueKind =
    oahElementValueKind::kElementValue_UNKNOWN_; // default value

  fElementHelpOnlyKind =
    oahElementHelpOnlyKind::kElementHelpOnlyNo; // default value

  fElementVisibilityKind =
    oahElementVisibilityKind::kElementVisibilityNone; // default value;
}

oahElement::~oahElement ()
{}

S_oahElement oahElement::thisElementIfItHasName (
  const std::string& name)
{
  S_oahElement result;

  if (
    name == fLongName
     ||
    name == fShortName
  ) {
    result = this;
  }

  return result;
}

std::string oahElement::getShortNameOrLongNameIfEmpty () const
{
  std::string result;

  if (fShortName.size ()) {
    result = fShortName;
  }
  else {
    result = fLongName;
  }

  return result;
}

std::string oahElement::fetchNames () const
{
  std::stringstream ss;

  if (
    fLongName.size ()
        &&
    fShortName.size ()
  ) {
    if (gOahOahGroup->getReverseNamesDisplayOrder ()) {
      ss <<
        '-' << fShortName <<
        ", " <<
        '-' << fLongName;
    }
    else {
      ss <<
        '-' << fLongName <<
        ", " <<
        '-' << fShortName;
    }
  }

  else {
    if (fLongName.size ()) {
      ss <<
        '-' << fLongName;
    }
    if (fShortName.size ()) {
      ss <<
      '-' << fShortName;
    }
  }

  return ss.str ();
}

std::string oahElement::fetchNamesInColumns (
  int subGroupsShortNameFieldWidth) const
{
  std::stringstream ss;

  if (
    fShortName.size ()
        &&
    fLongName.size ()
    ) {
      if (gOahOahGroup->getReverseNamesDisplayOrder ()) {
        ss << std::left <<
          std::setw (subGroupsShortNameFieldWidth) <<
          '-' << fLongName <<
          ", " <<
          '-' << fShortName;
      }
      else {
        ss << std::left <<
          std::setw (subGroupsShortNameFieldWidth) <<
          '-' << fLongName <<
          ", " <<
          '-' << fShortName;
      }
  }

  else {
    ss <<
      std::setw (subGroupsShortNameFieldWidth);

    if (fLongName.size ()) {
      ss <<
        '-' << fLongName;
    }
    if (fShortName.size ()) {
      ss << std::left <<
         '-' << fShortName;
    }
  }

  return ss.str ();
}

std::string oahElement::fetchNamesBetweenQuotes () const
{
  std::stringstream ss;

  ss <<
    '\'' <<
    fetchNames () <<
    '\'';

  return ss.str ();
}

std::string oahElement::fetchNamesBetweenParentheses () const
{
  std::stringstream ss;

  ss <<
    '(' <<
    fetchNames () <<
    ")";

  return ss.str ();
}

std::string oahElement::fetchNamesInColumnsBetweenParentheses (
  int subGroupsShortNameFieldWidth) const
{
  std::stringstream ss;

  ss <<
    '(' <<
    fetchNamesInColumns (
      subGroupsShortNameFieldWidth) <<
    ")";

  return ss.str ();
}

Bool oahElement::findStringInFindableElement (
  const std::string&               lowerCaseString,
  std::list <S_oahFindStringMatch>& foundMatchesList,
  std::ostream&                    os) const
{
  Bool result;

  if (elementMatchesString (lowerCaseString)) {
    // append the match to foundStringsList
    foundMatchesList.push_back (
      oahFindStringMatch::create (
        fetchNames (),
        fDescription,
        containingFindableElementAsString ()));

    result = true;
  }

  return result;
}

// void oahElement::acceptIn (basevisitor* v)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gOahOahGroup->getTraceOahVisitors ()) {
//     gLog <<
//       ".\\\" ==> oahElement::acceptIn ()" <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   if (visitor<S_oahElement>*
//     p =
//       dynamic_cast<visitor<S_oahElement>*> (v)) {
//         S_oahElement elem = this;
//
// #ifdef MF_TRACE_IS_ENABLED
//         if (gOahOahGroup->getTraceOahVisitors ()) {
//           gLog <<
//             ".\\\" ==> Launching oahElement::visitStart ()" <<
//             std::endl;
//         }
// #endif // MF_TRACE_IS_ENABLED
//         p->visitStart (elem);
//   }
// }
//
// void oahElement::acceptOut (basevisitor* v)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gOahOahGroup->getTraceOahVisitors ()) {
//     gLog <<
//       ".\\\" ==> oahElement::acceptOut ()" <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   if (visitor<S_oahElement>*
//     p =
//       dynamic_cast<visitor<S_oahElement>*> (v)) {
//         S_oahElement elem = this;
//
// #ifdef MF_TRACE_IS_ENABLED
//         if (gOahOahGroup->getTraceOahVisitors ()) {
//           gLog <<
//             ".\\\" ==> Launching oahElement::visitEnd ()" <<
//             std::endl;
//         }
// #endif // MF_TRACE_IS_ENABLED
//         p->visitEnd (elem);
//   }
// }
//
// void oahElement::browseData (basevisitor* v)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gOahOahGroup->getTraceOahVisitors ()) {
//     gLog <<
//       ".\\\" ==> oahElement::browseData ()" <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
// }

Bool oahElement::fetchElementsMatchingString (
	const std::string&       lowerCaseString,
	std::list <S_oahElement>& foundElementsList,
	std::ostream&            os) const
{
  Bool result;

  if (elementMatchesString (lowerCaseString)) {
    // append the match to foundStringsList
//     foundElementsList.push_back (// JMI 0.9.71 OHA_OAH
//       oahFindStringMatch::create (
//         fetchNames (),
//         fDescription,
//         containingFindableElementAsString ()));

    result = true;
  }

  return result;
}

Bool oahElement::elementMatchesString (
  const std::string& lowerCaseString) const
{
  // does this element's long name match?
  Bool longNameMatches =
    mfStringToLowerCase (fLongName).find (lowerCaseString) != std::string::npos;

  // does this element's short name match?
  Bool shortNameMatches =
    mfStringToLowerCase (fShortName).find (lowerCaseString) != std::string::npos;

  // does this element's description match?
  Bool descriptionMatches =
    mfStringToLowerCase (fDescription).find (lowerCaseString) != std::string::npos;

  return
    shortNameMatches || longNameMatches || descriptionMatches;
}

std::string oahElement::asLongNamedOptionString () const
{
  if (fLongName.size ()) {
    return asActualLongNamedOptionString ();
  }
  else {
    return asShortNamedOptionString ();
  }
}

std::string oahElement::asShortString () const // JMI 0.9.67
{
  std::stringstream ss;

  ss << "'-" << fLongName;

  if (fShortName.size ()) {
    ss << ", " << fShortName;
  }

  ss << '\'' ;

  return ss.str ();
}

std::string oahElement::asString () const
{
  std::stringstream ss;

  ss <<
    "'-" << fLongName;

  if (fShortName.size ()) {
    ss <<
      ", fShortName";
  }

  ss <<
    "': " <<
    fDescription;

  return ss.str ();
}

void oahElement::printOptionHeader (std::ostream& os) const
{
  os <<
    '-' << fLongName <<
    std::endl <<
    '-' << fShortName <<
    std::endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    gIndenter.indentMultiLineString (
      fDescription,
      os);

    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }
}

void oahElement::printOahElementEssentials (
  std::ostream& os,
  int fieldWidth) const
{
  os << std::left <<
    std::setw (fieldWidth) <<
    fetchNames () << ": " <<
    fDescription <<
    std::endl;
}
void oahElement::printOahElementEssentialsFull (
  std::ostream& os,
  int fieldWidth) const
{
  os << std::left <<
    std::setw (fieldWidth) <<
    "longName" << ": " <<
    "\"" << fLongName << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "shortName" << ": " <<
    "\"" << fShortName << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "description" << ": " <<
    "\"" << fDescription << "\"" <<
    std::endl <<

/* JMI
  ++gIndenter;
  gIndenter.indentMultiLineString (
    fDescription,
    os);
  --gIndenter;
*/

    std::setw (fieldWidth) <<
    "fElementValueKind" << ": " <<
    oahElementValueKindAsString (fElementValueKind) <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fElementHelpOnlyKind" << ": " <<
    oahElementHelpOnlyKindAsString (fElementHelpOnlyKind) <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fElementVisibilityKind" << ": " <<
    oahElementVisibilityKindAsString (fElementVisibilityKind) <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMultipleOccurrencesAllowed" << ": " <<
    fMultipleOccurrencesAllowed <<
    std::endl;
}

void oahElement::print (std::ostream& os) const
{
  os <<
    "[??? oahElement ???]" <<
    std::endl;

  printOahElementEssentials (os, 40); // JMI 0.9.66
}

void oahElement::printFull (std::ostream& os) const
{
  print (os);
}

void oahElement::printHelp (std::ostream& os) const
{
  os <<
    fetchNames () <<
    std::endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    gIndenter.indentMultiLineString (
      fDescription,
      os);

    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }
}

// const std::string oahElement::containingFindableElementAsString () const
// {
//   std::stringstream ss;
//
//   ss <<
//     fetchNames () << ": " <<
//     fDescription;
//
//   return ss.str ();
// }

std::ostream& operator << (std::ostream& os, const S_oahElement& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const oahElement& elt)
{
  elt.print (os);
  return os;
}
//______________________________________________________________________________
bool compareOahElements::operator() (
  const S_oahElement firstElement,
  const S_oahElement secondElement) const
{
/*
  Compare:
  A binary predicate that takes two arguments of the same type as the elements and returns a Bool. The expression comp(a,b), where comp is an object of this type and a and b are key values, shall return true if a is considered to go before b in the strict weak ordering the function defines.
  The std::multiset object uses this expression to determine both the order the elements follow in the container and whether two element keys are equivalent (by comparing them reflexively: they are equivalent if !comp(a,b) && !comp(b,a)).
  This can be a function pointer or a function object (see constructor for an example). This defaults to less<T>, which returns the same as applying the less-than operator (a<b).
  Aliased as member types std::multiset::key_compare and std::multiset::value_compare.
*/

  // let's decide that nullptr (which shouldn't occur too often...)
  // should go after all non null S_oahElement values

  bool result;

  if (firstElement) {
    if (secondElement) {
      result =
        firstElement->getLongName () < secondElement->getLongName ();
    }
    else {
      result = true;
    }
  }
  else {
    result = false;
  }

  return result;
}

//______________________________________________________________________________
S_oahElementUse oahElementUse::create (
  const S_oahElement&  elementUsed,
  const std::string& nameUsed,
  const std::string& valueUsed)
{
  oahElementUse* obj =
    new oahElementUse (
      elementUsed,
      nameUsed,
      valueUsed);
  assert (obj != nullptr);
  return obj;
}

oahElementUse::oahElementUse (
  const S_oahElement&  elementUsed,
  const std::string& nameUsed,
  const std::string& valueUsed)
{
  fElementUsed = elementUsed;
  fNameUsed = nameUsed;
  fValueUsed = valueUsed;
}

oahElementUse::~oahElementUse ()
{}

std::string oahElementUse::asString () const
{
  std::stringstream ss;

  ss <<
    "Element use" <<
    ": " << fElementUsed->fetchNamesBetweenQuotes () <<
    ", fNameUsed: \"" << fNameUsed << "\"" <<
    ", fValueUsed: \"" << fValueUsed << "\"" <<
    ", elementValueKind: " <<
    oahElementValueKindAsString (fElementUsed->getElementValueKind ()) <<
    ", oahElementHelpOnlyKind: " <<
    oahElementHelpOnlyKindAsString (fElementUsed->getElementHelpOnlyKind ()) <<
    ", elementVisibilityKind: " <<
    oahElementVisibilityKindAsString (fElementUsed->getElementVisibilityKind ()) <<
    ", multipleOccurrencesAllowed: " <<
    fElementUsed->getMultipleOccurrencesAllowed ();

  return ss.str ();
}

void oahElementUse::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahElementUse& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
