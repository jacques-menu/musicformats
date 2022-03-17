/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw()), set::precision(), ...

#include "mfBool.h"
#include "mfStringsHandling.h"

#include "mfslBasicTypes.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahOah.h"
#include "mfslInterpreterOah.h"

#include "oahWae.h"


using namespace std;

using namespace MusicFormats;

//______________________________________________________________________________
mfslElement::mfslElement (
  int inputLineNumber)
{
  fInputLineNumber = inputLineNumber;
}

mfslElement::~mfslElement ()
{}

string mfslElement::asString () const
{
  // this is overriden all in actual elements
  return "??? mfslElement::asString () ???";
}

void mfslElement::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_mfslElement& elt)
{
  os << elt->asString ();
  return os;
}

//_______________________________________________________________________________
string mfslVariableValueKindAsString (
  mfslVariableValueKind variableValueKind)
{
  string result;

  switch (variableValueKind) {
    case mfslVariableValueKind::kVariableValueNone:
      result = "kVariableValueNone";
      break;
    case mfslVariableValueKind::kVariableValueSuppliedToScript:
      result = "kVariableValueSuppliedToScript";
      break;
    case mfslVariableValueKind::kVariableValueSetInScript:
      result = "kVariableValueSetInScript";
      break;
  } // switch

  return result;
}

ostream& operator<< (ostream& os, const mfslVariableValueKind& elt)
{
  os << mfslVariableValueKindAsString (elt);
  return os;
}

//_______________________________________________________________________________
S_mfslVariable mfslVariable::create (
  const string& variableName)
{
  mfslVariable* o =
    new mfslVariable (
      variableName);
  assert (o != nullptr);
  return o;
}

mfslVariable::mfslVariable (
  const string& variableName)
{
  fVariableName = variableName;

  fVariableValueKind =
    mfslVariableValueKind::kVariableValueNone;
}

mfslVariable::~mfslVariable ()
{}

void mfslVariable::setVariableValue (const string& value)
{
  if (gGlobalMfslInterpreterOahGroup->getTraceSemantics ()) {
    gLogStream <<
      "====> setting the value of variable \"" << fVariableName <<
      "\" to \"" << value << "\"" <<
      endl;
  }

  // is this value in the variable's possible values set?
  set<string>::const_iterator
    it =
      fPossibleValuesSet.find (value);

  if (it == fPossibleValuesSet.end ()) {
    // no, issue error message
    stringstream s;

    s <<
      "variable \"" << fVariableName <<
      " cannot be set to value \"" << value <<
      ", the possible values are: " <<
      possibleValuesAsString ();

    oahError (s.str ());
  }

  switch (fVariableValueKind) {
    case mfslVariableValueKind::kVariableValueNone:
      // store into the variable
      fVariableValue = value;

      fVariableValueKind =
        mfslVariableValueKind::kVariableValueSetInScript;
      break;

    case mfslVariableValueKind::kVariableValueSuppliedToScript:
      {
        // ignore this value, and keep value supplied to the script
        stringstream s;

        s <<
          "variable \"" << fVariableName <<
          " already got value \"" << fVariableValue <<
          "\" supplied to the script"
          ", ingnoring value \"" << value << "\"";

        oahWarning (s.str ());
      }
      break;

    case mfslVariableValueKind::kVariableValueSetInScript:
      {
        // set in script multiple times
        stringstream s;

        s <<
          "variable \"" << fVariableName <<
          "\"" <<
          " already has value \"" << fVariableValue <<
          "\" supplied to the script";

        oahError (s.str ());
      }
      break;
  } // switch
}

string mfslVariable::getVariableValue () const
{
  switch (fVariableValueKind) {
    case mfslVariableValueKind::kVariableValueNone:
      {
        stringstream s;

        s <<
          "variable \"" << fVariableName <<
          "\" has not got any value";

        oahError (s.str ());
      }
      break;

    case mfslVariableValueKind::kVariableValueSuppliedToScript:
      if (gGlobalMfslInterpreterOahGroup->getTraceSemantics ()) {
        gLogStream <<
          "====> variable \"" << fVariableName << fVariableValue <<
          "\", supplied to the script" <<
          endl;
      }
      break;

    case mfslVariableValueKind::kVariableValueSetInScript:
      if (gGlobalMfslInterpreterOahGroup->getTraceSemantics ()) {
        gLogStream <<
          "====> variable \"" << fVariableName <<
          "\" has value \"" << fVariableValue <<
          "\", set in the script" <<
          endl;
      }
      break;
  } // switch

  return fVariableValue;
}

string mfslVariable::getVariableValueWithoutTrace () const
{
  switch (fVariableValueKind) {
    case mfslVariableValueKind::kVariableValueNone:
      {
        stringstream s;

        s <<
          "variable \"" << fVariableName <<
          "\" has not got any value";

        oahError (s.str ());
      }
      break;

    case mfslVariableValueKind::kVariableValueSuppliedToScript:
    case mfslVariableValueKind::kVariableValueSetInScript:
      break;
  } // switch

  return fVariableValue;
}

void mfslVariable::addPossibleValue (const string& value)
{
  // is this value in the variable's possible values set?
  set<string>::const_iterator
    it =
      fPossibleValuesSet.find (value);

  if (it != fPossibleValuesSet.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "variable \"" << fVariableName <<
      " possible value \"" << value <<
      "\" occurs more that once";

    oahError (s.str ());
  }

  else {
    if (gGlobalMfslInterpreterOahGroup->getTraceSemantics ()) {
      gLogStream <<
        "====> adding \"" << value << "\"" <<
        " as possible value of variable \"" << fVariableName << "\"" <<
        endl;
    }

    fPossibleValuesSet.insert (value);
  }
}

Bool mfslVariable::isValuePossible (const string& value)
{
  // is this value in the variable's possible values set?
  set<string>::const_iterator
    it =
      fPossibleValuesSet.find (value);

  return it != fPossibleValuesSet.end ();
}

string mfslVariable::possibleValuesAsString () const
{
  stringstream s;

  unsigned int possibleValuesSetSize =
    fPossibleValuesSet.size ();

  if (possibleValuesSetSize) {
    unsigned int nextToLast =
      possibleValuesSetSize - 1;

    unsigned int count = 0;

    unsigned int namesListMaxLength = 60;
    unsigned int cumulatedLength    = 0;

    for (
      set<string>::const_iterator i =
        fPossibleValuesSet.begin ();
      i != fPossibleValuesSet.end ();
      ++i
    ) {
      string theString = (*i);

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
      }

      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != possibleValuesSetSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

string mfslVariable::asString () const
{
  stringstream s;

  s <<
    "mfslVariable [" <<
    "fVariableName: " << fVariableName <<
    ", " <<
    "fVariableValue: [" << fVariableValue << "]" <<
    "]";

  return s.str ();
}

void mfslVariable::print (ostream& os) const
{
  os <<
    "mfslVariable [" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 19;

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : \"" << fVariableName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fVariableValueKind" << " : " << fVariableValueKind << "" <<
    endl <<
    setw (fieldWidth) <<
    "fVariableValue" << " : \"" << fVariableValue << "\"" <<
    endl;

  unsigned int
    possibleValuesSetSize =
      fPossibleValuesSet.size ();

  os <<
    setw (fieldWidth) <<
    "fPossibleValuesSet" << " : [";
  if (possibleValuesSetSize) {
    int counter = 0;
    for (string value : fPossibleValuesSet) {
      os << value;

      if (++counter < possibleValuesSetSize) {
        os << " | ";
      }
    } // for
  }
  os << "]" << endl;

  --gIndenter;

  os << "]" << endl;
}

ostream& operator<< (ostream& os, const S_mfslVariable& elt)
{
  os << elt->asString ();
  return os;
}

//_______________________________________________________________________________
S_mfslVariablesTable mfslVariablesTable::create ()
{
  mfslVariablesTable* o =
    new mfslVariablesTable ();
  assert (o != nullptr);
  return o;
}

mfslVariablesTable::mfslVariablesTable ()
{}

mfslVariablesTable::~mfslVariablesTable ()
{}

S_mfslVariable mfslVariablesTable::createAndInsertVariable (
  const string& variableName)
{
  S_mfslVariable
    variable =
       mfslVariable::create (
         variableName);

  fVariablesMap [variableName] = variable;

  return variable;
}

S_mfslVariable mfslVariablesTable::lookupVariableByName (
  const string& variableName)
{
  S_mfslVariable result;

  // is this value in the variable's possible values set?
  map<string, S_mfslVariable>::const_iterator
    it =
      fVariablesMap.find (variableName);

  if (it != fVariablesMap.end ()) {
    result = (*it).second;
  }

  return result;
}

S_mfslVariable mfslVariablesTable::checkCaseVariableByName (
  const string& variableName)
{
  S_mfslVariable
    variable =
      lookupVariableByName (variableName);

  if (variable) {
    mfslVariableValueKind
      variableValueKind =
        variable->getVariableValueKind ();

    switch (variableValueKind) {
      case mfslVariableValueKind::kVariableValueNone:
        {
          stringstream s;

          s <<
            "variable \"" << variableName <<
            "\" never got a value";

          oahError (s.str ());
        }
        break;

      case mfslVariableValueKind::kVariableValueSuppliedToScript:
      case mfslVariableValueKind::kVariableValueSetInScript:
        if (gGlobalMfslInterpreterOahGroup->getTraceSemantics ()) {
          // that's fine
          gLogStream <<
            "====> variable \"" << variableName <<
            "\" has value \"" <<
            variable->getVariableValueWithoutTrace () <<
            "\"" <<
            endl;
        }
        break;
    } // switch
  }

  else {
    stringstream s;

    s <<
      "variable \"" << variableName <<
      "\" is unknown";

    oahError (s.str ());
  }

  return variable;
}

string mfslVariablesTable::asString () const
{
  stringstream s;

  s <<
    "mfslVariablesTable [" <<
    "]";

  return s.str ();
}

void mfslVariablesTable::print (ostream& os) const
{
  os <<
    "mfslVariablesTable [" <<
    endl;

  if (fVariablesMap.size ()) {
    ++gIndenter;

    for (pair<string, S_mfslVariable> thePair : fVariablesMap) {
      S_mfslVariable variable = thePair.second;

      os << variable;
    } // for

    --gIndenter;
  }

  os << "]";
}

ostream& operator<< (ostream& os, const S_mfslVariablesTable& elt)
{
  os << elt->asString ();
  return os;
}

//_______________________________________________________________________________
S_mfslScope mfslScope::create (const string& scopeName)
{
  mfslScope* o =
    new mfslScope (
      scopeName);
  assert (o != nullptr);
  return o;
}

mfslScope::mfslScope (const string& scopeName)
{
  fScopeName = scopeName;
}

mfslScope::~mfslScope ()
{}

void mfslScope::registerOptionNamesAndValuesInScope (
  S_oahOptionNameAndValue
    optionNameAndValue)
{
  if (gGlobalMfslInterpreterOahGroup->getTraceSemantics ()) {
    gLogStream <<
      "====> Registering [" <<
      optionNameAndValue <<
      "] in scope \"" <<
      fScopeName <<
      "\"" <<
      endl << endl;
  }

  fScopeOptionsNamesAndValuesVector.push_back (
      optionNameAndValue);
}

// void mfslScope::appendElementToScope (S_mfslElement element)
// {
//   if (gGlobalMfslInterpreterOahGroup->getTraceSemantics ()) {
//     gLogStream <<
//       "Appending MFSL element " <<
//       element <<
//       " to scope \"" << fScopeName << "\"";
//   }
//
//   fScopeElementsList.push_back (element);
// }

string mfslScope::asString () const
{
  stringstream s;

  s <<
    "mfslScope [" <<
    "fScopeName: " << fScopeName <<
    "]";

  return s.str ();
}

void mfslScope::print (ostream& os) const
{
  os <<
    "mfslScope [" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 14;

  os << left <<
    setw (fieldWidth) <<
    "fScopeName:" << " : " << fScopeName <<
    endl;

  // print the scopes option name and value vector
  if (gGlobalMfslInterpreterOahGroup->getTraceSemantics ()) {
    gLogStream <<
      "fScopesStack:" << " : " <<
      endl;

    ++gIndenter;

    for (
      S_oahOptionNameAndValue
        optionNameAndValue :
          fScopeOptionsNamesAndValuesVector
    ) {
      gLogStream <<
        optionNameAndValue <<
        endl;
    } // for

    --gIndenter;

    gLogStream << endl;
  }

//   os << left <<
//     "fScopeElementsList:" <<
//     endl;
//
//   ++gIndenter;
//
//   for (S_mfslElement element : fScopeElementsList) {
//     os << element << endl;
//   } // for
//
//   --gIndenter;

  --gIndenter;

  os << "]";
}

ostream& operator<< (ostream& os, const S_mfslScope& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_mfslCaseStatement mfslCaseStatement::create (
  S_mfslVariable caseVariable)
{
  mfslCaseStatement* o =
    new mfslCaseStatement (
      caseVariable);
  assert (o != nullptr);
  return o;
}

mfslCaseStatement::mfslCaseStatement (
  S_mfslVariable caseVariable)
{
  fCaseVariable = caseVariable;

  // all possible values are initially unused
  for (string value : fCaseVariable->getPossibleValuesSet ()) {
    fCaseUnusedLabels.insert (value);
  }
}

mfslCaseStatement::~mfslCaseStatement ()
{}

void mfslCaseStatement::registerCaseLabelValue (const string& value)
{
  if (! mfStringIsInStringSet (value, fCaseUnusedLabels)) {
    // this label value has already been registered
    stringstream s;

    s <<
      "variable \"" <<
      fCaseVariable->getVariableName () <<
      " value \"" << value <<
      " is already present in this case statement";

    oahError (s.str ());
  }

  // is this value in the variable's possible values set?
  set<string>
    possibleValueSet =
      fCaseVariable->
        getPossibleValuesSet ();

  set<string>::const_iterator
    it =
      possibleValueSet.find (value);

  if (it == possibleValueSet.end ()) {
    // no, issue error message
    stringstream s;

    s <<
      "variable \"" <<
      fCaseVariable->getVariableName () <<
      " cannot have value \"" << value <<
      ", the possible values are: " <<
      fCaseVariable->possibleValuesAsString ();

    oahError (s.str ());
  }

  // value is no longer unused
  fCaseUnusedLabels.erase (value);
}

void mfslCaseStatement::checkThatAllPossibleValuesHaveBeenUsed ()
{
  if (fCaseUnusedLabels.size ()) {
    stringstream s;

    s <<
      "The following values for variable \"" <<
      fCaseVariable->getVariableName () <<
      "\" have not been used in this case statement: " <<
      mfStringSetAsString (fCaseUnusedLabels);

    oahError (s.str ());
  }
}

string mfslCaseStatement::asString () const
{
  stringstream s;

  s <<
    "mfslCaseStatement [" <<
    "]";

  return s.str ();
}

void mfslCaseStatement::print (ostream& os) const
{
  os <<
    "mfslCaseStatement [" <<
    "fCaseVariable: " <<
    fCaseVariable <<
    ", fCaseUnusedLabels: " <<
    mfStringSetAsString (fCaseUnusedLabels) <<
    endl;

  ++gIndenter;

  const int fieldWidth = 17;

  os << left <<
    setw (fieldWidth) <<
    "fCaseVariable" << " : " <<
    fCaseVariable <<
    endl <<

    setw (fieldWidth) <<
    "fCaseUnusedLabels" << " : " <<
    mfStringSetAsString (fCaseUnusedLabels) <<
    endl;

  --gIndenter;

  os << "]";
}

ostream& operator<< (ostream& os, const S_mfslCaseStatement& elt)
{
  os << elt->asString ();
  return os;
}

// //______________________________________________________________________________
// void initializeMFSLBasicTypes ()
// {
//   // protect library against multiple initializations
//   static Bool pPrivateThisMethodHasBeenRun (false);
//
//   if (! pPrivateThisMethodHasBeenRun) {
// #ifdef TRACING_IS_ENABLED
//     if (gGlobalOahEarlyOptions.getEarlyTracingOah () && ! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
//       gLogStream <<
//         "Initializing MFSL basic types handling" <<
//         endl;
//     }
// #endif
//
//     pPrivateThisMethodHasBeenRun = true;
//   }
// }
