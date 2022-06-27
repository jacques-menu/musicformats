/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif



#include "mfStringsHandling.h"


#include "oahOah.h"

#include "oahElements.h"

namespace MusicFormats
{

// layout settings
//______________________________________________________________________________
const int K_OAH_ELEMENTS_INDENTER_OFFSET = 3;
  // indent a bit more for readability

const int K_OAH_FIELD_WIDTH = 40;

//______________________________________________________________________________
string elementValueKindAsString (
  oahElementValueKind elementValueKind)
{
  string result;

  switch (elementValueKind) {
    case oahElementValueKind::kElementValueWithout:
      result = "kElementValueWithout";
      break;
    case oahElementValueKind::kElementValueImplicit:
      result = "kElementValueImplicit";
      break;
    case oahElementValueKind::kElementValueMandatory:
      result = "kElementValueMandatory";
      break;
    case oahElementValueKind::kElementValueOptional:
      result = "kElementValueOptional";
      break;
  } // switch

  return result;
}

string elementVisibilityKindAsString (
  oahElementVisibilityKind elementVisibilityKind)
{
  string result;

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

string elementHelpOnlyKindAsString (
  oahElementHelpOnlyKind elementHelpOnlyKind)
{
  string result;

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

//______________________________________________________________________________
/* this class is purely virtual
S_oahElement oahElement::create (
  const string&            longName,
  const string&            shortName,
  const string&            description,
  oahElementValueKind      elementValueKind,
  oahElementVisibilityKind elementVisibilityKind)
{
  oahElement* o = new
    oahElement (
      longName,
      shortName,
      description,
      elementValueKind,
      elementVisibilityKind);
  assert (o != nullptr);
  return o;
}
*/

oahElement::oahElement (
  const string&            longName,
  const string&            shortName,
  const string&            description,
  oahElementValueKind      elementValueKind,
  oahElementVisibilityKind elementVisibilityKind)
{
  fLongName    = longName;
  fShortName   = shortName;
  fDescription = description;

  fElementValueKind = elementValueKind;

  fElementHelpOnlyKind =
    oahElementHelpOnlyKind::kElementHelpOnlyNo; // default value

  fElementVisibilityKind = elementVisibilityKind;

//   fMultipleOccurrencesAllowed = false; superflous
}

oahElement::~oahElement ()
{}

S_oahElement oahElement::thisElementIfItHasName (
  const string& name)
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

string oahElement::getShortNameOrLongNameIfEmpty () const
{
  string result;

  if (fShortName.size ()) {
    result = fShortName;
  }
  else {
    result = fLongName;
  }

  return result;
}

string oahElement::fetchNames () const
{
  stringstream s;

  if (
    fLongName.size ()
        &&
    fShortName.size ()
  ) {
    if (gGlobalOahOahGroup->getReverseNamesDisplayOrder ()) {
      s <<
        '-' << fShortName <<
        ", " <<
        '-' << fLongName;
    }
    else {
      s <<
        '-' << fLongName <<
        ", " <<
        '-' << fShortName;
    }
  }

  else {
    if (fLongName.size ()) {
      s <<
        '-' << fLongName;
    }
    if (fShortName.size ()) {
      s <<
      '-' << fShortName;
    }
  }

  return s.str ();
}

string oahElement::fetchNamesInColumns (
  int subGroupsShortNameFieldWidth) const
{
  stringstream s;

  if (
    fShortName.size ()
        &&
    fLongName.size ()
    ) {
      if (gGlobalOahOahGroup->getReverseNamesDisplayOrder ()) {
        s << left <<
          setw (subGroupsShortNameFieldWidth) <<
          '-' << fLongName <<
          ", " <<
          '-' << fShortName;
      }
      else {
        s << left <<
          setw (subGroupsShortNameFieldWidth) <<
          '-' << fLongName <<
          ", " <<
          '-' << fShortName;
      }
  }

  else {
    s <<
      setw (subGroupsShortNameFieldWidth);

    if (fLongName.size ()) {
      s <<
        '-' << fLongName;
    }
    if (fShortName.size ()) {
      s << left <<
         '-' << fShortName;
    }
  }

  return s.str ();
}

string oahElement::fetchNamesBetweenQuotes () const
{
  stringstream s;

  s <<
    "'" <<
    fetchNames () <<
    "'";

  return s.str ();
}

string oahElement::fetchNamesBetweenParentheses () const
{
  stringstream s;

  s <<
    '(' <<
    fetchNames () <<
    ")";

  return s.str ();
}

string oahElement::fetchNamesInColumnsBetweenParentheses (
  int subGroupsShortNameFieldWidth) const
{
  stringstream s;

  s <<
    '(' <<
    fetchNamesInColumns (
      subGroupsShortNameFieldWidth) <<
    ")";

  return s.str ();
}

Bool oahElement::findStringInElement (
  const string& lowerCaseString,
  list<string>& foundStringsList,
  ostream&      os) const
{
  Bool result;

  // does this element's long name match?
  Bool longNameMatches =
    mfStringToLowerCase (fLongName).find (lowerCaseString) != string::npos;

  // does this element's short name match?
  Bool shortNameMatches =
    mfStringToLowerCase (fShortName).find (lowerCaseString) != string::npos;

  // does this element's description match?
  Bool descriptionMatches =
    mfStringToLowerCase (fDescription).find (lowerCaseString) != string::npos;

  if (shortNameMatches || longNameMatches || descriptionMatches) {
    stringstream s;

    // add the element's names
    s <<
      fetchNames () <<
      endl;

    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    // add the element's description
    s <<
      gIndenter.indentMultiLineString (
        fDescription);

    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);

    // append the string
    foundStringsList.push_back (s.str ());

    result = true;
  }

  return result;
}

void oahElement::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahElement::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahElement>*
    p =
      dynamic_cast<visitor<S_oahElement>*> (v)) {
        S_oahElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahElement::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahElement::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahElement::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahElement>*
    p =
      dynamic_cast<visitor<S_oahElement>*> (v)) {
        S_oahElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahElement::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahElement::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahElement::browseData ()" <<
      endl;
  }
#endif
}

string oahElement::asLongNamedOptionString () const
{
  if (fLongName.size ()) {
    return asActualLongNamedOptionString ();
  }
  else {
    return asShortNamedOptionString ();
  }
}

string oahElement::asShortString () const
{
  stringstream s;

  s << "'-" << fLongName;

  if (fShortName.size ()) {
    s << ", " << fShortName;
  }

  s << "'" ;

  return s.str ();
}

string oahElement::asString () const
{
  stringstream s;

  s <<
    "'-" << fLongName;

  if (fShortName.size ()) {
    s <<
      ", fShortName";
  }

  s <<
    "': " <<
    fDescription;

  return s.str ();
}

void oahElement::printOptionHeader (ostream& os) const
{
  os <<
    '-' << fLongName <<
    endl <<
    '-' << fShortName <<
    endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;

    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }
}

void oahElement::printOahElementEssentials (
  ostream& os,
  int fieldWidth) const
{
  os << left <<
    setw (fieldWidth) <<
    "longName" << " : " <<
    "\"" << fLongName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "shortName" << " : " <<
    "\"" << fShortName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "description" << " : " <<
    "\"" << fDescription << "\"" <<
    endl <<

/* JMI
  ++gIndenter;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  --gIndenter;
*/

    setw (fieldWidth) <<
    "fElementValueKind" << " : " <<
    elementValueKindAsString (fElementValueKind) <<
    endl <<

    setw (fieldWidth) <<
    "fElementHelpOnlyKind" << " : " <<
    elementHelpOnlyKindAsString (fElementHelpOnlyKind) <<
    endl <<

    setw (fieldWidth) <<
    "fElementVisibilityKind" << " : " <<
    elementVisibilityKindAsString (fElementVisibilityKind) <<
    endl <<

    setw (fieldWidth) <<
    "fMultipleOccurrencesAllowed" << " : " <<
    fMultipleOccurrencesAllowed <<
    endl;
}

void oahElement::printOahElementEssentialsShort (
  ostream& os,
  int fieldWidth) const
{
  os << left <<
    setw (fieldWidth) <<
    fetchNames () << " : " <<
    fDescription <<
    endl;
}

void oahElement::print (ostream& os) const
{
  os <<
    "??? oahElement ???" <<
    endl;

  printOahElementEssentials (os, 40); // JMI
}

void oahElement::printShort (ostream& os) const
{
  os <<
    "??? oahElement ???" <<
    endl;

  printOahElementEssentials (os, 40); // JMI
}

void oahElement::printHelp (ostream& os) const
{
  os <<
    fetchNames () <<
    endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;

    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }
}

ostream& operator<< (ostream& os, const S_oahElement& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
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
  The multiset object uses this expression to determine both the order the elements follow in the container and whether two element keys are equivalent (by comparing them reflexively: they are equivalent if !comp(a,b) && !comp(b,a)).
  This can be a function pointer or a function object (see constructor for an example). This defaults to less<T>, which returns the same as applying the less-than operator (a<b).
  Aliased as member types multiset::key_compare and multiset::value_compare.
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
  S_oahElement  elementUsed,
  const string& nameUsed,
  const string& valueUsed)
{
  oahElementUse* o =
    new oahElementUse (
      elementUsed,
      nameUsed,
      valueUsed);
  assert (o != nullptr);
  return o;
}

oahElementUse::oahElementUse (
  S_oahElement  elementUsed,
  const string& nameUsed,
  const string& valueUsed)
{
  fElementUsed = elementUsed;
  fNameUsed    = nameUsed;
  fValueUsed   = valueUsed;
}

oahElementUse::~oahElementUse ()
{}

string oahElementUse::asString () const
{
  stringstream s;

  s <<
    "Element use" <<
    ": " << fElementUsed->fetchNamesBetweenQuotes () <<
    ", fNameUsed: \"" << fNameUsed << "\"" <<
    ", fValueUsed: \"" << fValueUsed << "\"" <<
    ", elementValueKind: " <<
    elementValueKindAsString (fElementUsed->getElementValueKind ()) <<
    ", oahElementHelpOnlyKind: " <<
    elementHelpOnlyKindAsString (fElementUsed->getElementHelpOnlyKind ()) <<
    ", elementVisibilityKind: " <<
    elementVisibilityKindAsString (fElementUsed->getElementVisibilityKind ()) <<
    ", multipleOccurrencesAllowed: " <<
    fElementUsed->getMultipleOccurrencesAllowed ();

  return s.str ();
}

void oahElementUse::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_oahElementUse& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
  return os;
}


}
