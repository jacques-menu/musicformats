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

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahOah.h"

#include "mfslBasicTypes.h"
#include "mfslInterpreterOah.h"

#include "mfslWae.h"


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
string mfslVariableKindAsString (
  mfslVariableKind variableKind)
{
  string result;

  switch (variableKind) {
    case mfslVariableKind::kVariableChoice:
      result = "kVariableChoice";
      break;
    case mfslVariableKind::kVariablePath:
      result = "kVariablePath";
      break;
  } // switch

  return result;
}

ostream& operator<< (ostream& os, const mfslVariableKind& elt)
{
  os << mfslVariableKindAsString (elt);
  return os;
}

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
  const string&    variableName,
  mfslVariableKind variableKind)
{
  mfslVariable* o =
    new mfslVariable (
      variableName,
      variableKind);
  assert (o != nullptr);
  return o;
}

mfslVariable::mfslVariable (
  const string&    variableName,
  mfslVariableKind variableKind)
{
  fVariableName = variableName;
  fVariableKind = variableKind;

  fVariableValueKind =
    mfslVariableValueKind::kVariableValueNone;
}

mfslVariable::~mfslVariable ()
{}

void mfslVariable::setVariableValue (const string& value)
{
  if (gGlobalMfslInterpreterOahGroup->getTraceSemantics ()) {
    gLogStream <<
      "====> Setting the value of " <<
      fVariableKind <<
      " variable \"" <<
      fVariableName <<
      "\" to \"" <<
      value <<
      "\"" <<
      endl << endl;
  }

  // is this value in the variable's possible values set?
  set<string>::const_iterator
    it =
      fPossibleValuesSet.find (value);

  if (it == fPossibleValuesSet.end ()) {
    // no, issue error message
    stringstream s;

    s <<
      fVariableKind <<
      " variable \"" <<
      fVariableName <<
      " cannot be set to value \"" <<
      value <<
      ", the possible values are: " <<
      possibleValuesAsString ();

    mfslError (
      s.str (),
      yy::location (
        yy::position (),
        yy::position ())); // JMI
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
          fVariableKind <<
          " variable \"" <<
          fVariableName <<
          " already got value \"" <<
          fVariableValue <<
          "\" supplied to the script"
          ", ingnoring value \"" <<
          value <<
          "\"";

        mfslWarning (
          s.str (),
          yy::location (
            yy::position (),
            yy::position ())); // JMI
      }
      break;

    case mfslVariableValueKind::kVariableValueSetInScript:
      {
        // set in script multiple times
        stringstream s;

        s <<
          fVariableKind <<
          " variable \"" <<
          fVariableName <<
          "\"" <<
          " already has value \"" <<
          fVariableValue <<
          "\" supplied to the script";

        mfslError (
          s.str (),
          yy::location (
            yy::position (),
            yy::position ())); // JMI
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
          fVariableKind <<
          " variable \"" <<
          fVariableName <<
          "\" has not got any value";

        mfslError (
          s.str (),
          yy::location (
            yy::position (),
            yy::position ())); // JMI
      }
      break;

    case mfslVariableValueKind::kVariableValueSuppliedToScript:
      if (gGlobalMfslInterpreterOahGroup->getTraceSemantics ()) {
        gLogStream <<
          "====> " <<
          fVariableKind <<
          " Variable \"" <<
          fVariableName <<
          ", " <<
          fVariableValue <<
          "\", supplied to the script" <<
          endl << endl;
      }
      break;

    case mfslVariableValueKind::kVariableValueSetInScript:
      if (gGlobalMfslInterpreterOahGroup->getTraceSemantics ()) {
        gLogStream <<
          "====>" <<
          fVariableKind <<
          "  Variable \"" <<
          fVariableName <<
          "\" has value \"" <<
          fVariableValue <<
          "\", set in the script" <<
          endl << endl;
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
          fVariableKind <<
          " variable \"" <<
          fVariableName <<
          "\" has not got any value";

        mfslError (
          s.str (),
          yy::location (
            yy::position (),
            yy::position ())); // JMI
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
      fVariableKind <<
      " variable \"" <<
      fVariableName <<
      " possible value \"" <<
      value <<
      "\" occurs more that once";

    mfslError (
      s.str (),
      yy::location (
        yy::position (),
        yy::position ())); // JMI
  }

  else {
    if (gGlobalMfslInterpreterOahGroup->getTraceSemantics ()) {
      gLogStream <<
        "====> Adding \"" <<
        value <<
        "\"" <<
        " as possible value of " <<
        fVariableKind <<
        " variable \"" <<
        fVariableName <<
        "\"" <<
        endl << endl;
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
    "fVariableKind: " << fVariableKind <<
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
    "fVariableKind" << " : \"" << fVariableKind << "\"" <<
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
         variableName,
         mfslVariableKind::kVariableChoice);

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

          mfslError (
            s.str (),
            yy::location (
              yy::position (),
              yy::position ())); // JMI
        }
        break;

      case mfslVariableValueKind::kVariableValueSuppliedToScript:
      case mfslVariableValueKind::kVariableValueSetInScript:
        if (gGlobalMfslInterpreterOahGroup->getTraceSemantics ()) {
          // that's fine
          gLogStream <<
            "====> Variable \"" <<
            variableName <<
            "\" has value \"" <<
            variable->getVariableValueWithoutTrace () <<
            "\"" <<
            endl << endl;
        }
        break;
    } // switch
  }

  else {
    stringstream s;

    s <<
      "variable \"" << variableName <<
      "\" is unknown";

    mfslError (
      s.str (),
      yy::location (
        yy::position (),
        yy::position ())); // JMI
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

void mfslScope::registerOptionsInScope (
  S_oahOption
    option)
{
  if (gGlobalMfslInterpreterOahGroup->getTraceSemantics ()) {
    gLogStream <<
      "====> Registering [" <<
      option <<
      "] in scope \"" <<
      fScopeName <<
      "\"" <<
      endl << endl;
  }

  fScopeOptionsVector.push_back (
    option);
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

  const int fieldWidth = 34;

  os << left <<
    setw (fieldWidth) <<
    "fScopeName" << " : " << fScopeName <<
    endl;

  // print the scopes option name and value vector
  if (gGlobalMfslInterpreterOahGroup->getTraceSemantics ()) {
    gLogStream <<
      "fScopeOptionsVector" << " : " <<
      endl;

    ++gIndenter;

    for (S_oahOption option : fScopeOptionsVector) {
      gLogStream << option;
    } // for

    --gIndenter;
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

    mfslError (
      s.str (),
      yy::location (
        yy::position (),
        yy::position ())); // JMI
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

    mfslError (
      s.str (),
      yy::location (
        yy::position (),
        yy::position ())); // JMI
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

    mfslError (
      s.str (),
      yy::location (
        yy::position (),
        yy::position ())); // JMI
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

//_______________________________________________________________________________
S_mfslAllStatement mfslAllStatement::create (
  S_mfslVariable allVariable)
{
  mfslAllStatement* o =
    new mfslAllStatement (
      allVariable);
  assert (o != nullptr);
  return o;
}

mfslAllStatement::mfslAllStatement (
  S_mfslVariable allVariable)
{
  fAllVariable = allVariable;

  // all possible values are initially unused
  for (string value : fAllVariable->getPossibleValuesSet ()) {
    fAllUnusedLabels.insert (value);
  }
}

mfslAllStatement::~mfslAllStatement ()
{}

void mfslAllStatement::registerAllLabelValue (const string& value)
{
  if (! mfStringIsInStringSet (value, fAllUnusedLabels)) {
    // this label value has already been registered
    stringstream s;

    s <<
      "variable \"" <<
      fAllVariable->getVariableName () <<
      " value \"" << value <<
      " is already present in this all statement";

    mfslError (
      s.str (),
      yy::location (
        yy::position (),
        yy::position ())); // JMI
  }

  // is this value in the variable's possible values set?
  set<string>
    possibleValueSet =
      fAllVariable->
        getPossibleValuesSet ();

  set<string>::const_iterator
    it =
      possibleValueSet.find (value);

  if (it == possibleValueSet.end ()) {
    // no, issue error message
    stringstream s;

    s <<
      "variable \"" <<
      fAllVariable->getVariableName () <<
      " cannot have value \"" << value <<
      ", the possible values are: " <<
      fAllVariable->possibleValuesAsString ();

    mfslError (
      s.str (),
      yy::location (
        yy::position (),
        yy::position ())); // JMI
  }

  // value is no longer unused
  fAllUnusedLabels.erase (value);
}

void mfslAllStatement::checkThatAllPossibleValuesHaveBeenUsed ()
{
  if (fAllUnusedLabels.size ()) {
    stringstream s;

    s <<
      "The following values for variable \"" <<
      fAllVariable->getVariableName () <<
      "\" have not been used in this all statement: " <<
      mfStringSetAsString (fAllUnusedLabels);

    mfslError (
      s.str (),
      yy::location (
        yy::position (),
        yy::position ())); // JMI
  }
}

string mfslAllStatement::asString () const
{
  stringstream s;

  s <<
    "mfslAllStatement [" <<
    "]";

  return s.str ();
}

void mfslAllStatement::print (ostream& os) const
{
  os <<
    "mfslAllStatement [" <<
    "fAllVariable: " <<
    fAllVariable <<
    ", fAllUnusedLabels: " <<
    mfStringSetAsString (fAllUnusedLabels) <<
    endl;

  ++gIndenter;

  const int fieldWidth = 17;

  os << left <<
    setw (fieldWidth) <<
    "fAllVariable" << " : " <<
    fAllVariable <<
    endl <<

    setw (fieldWidth) <<
    "fAllUnusedLabels" << " : " <<
    mfStringSetAsString (fAllUnusedLabels) <<
    endl;

  --gIndenter;

  os << "]";
}

ostream& operator<< (ostream& os, const S_mfslAllStatement& elt)
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
