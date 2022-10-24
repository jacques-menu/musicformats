/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...
#include <algorithm>    // for_each

#include "visitor.h"

#include "mfStringsHandling.h"

#include "lpsrScheme.h"

#include "oahOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
string const lpsrSchemeVariable::g_SchemeVariableNoUnit    = "";
string const lpsrSchemeVariable::g_SchemeVariableNoComment = "";

S_lpsrSchemeVariable lpsrSchemeVariable::create (
  int               inputLineNumber,
  lpsrCommentedKind commentedKind,
  const string&     variableName,
  const string&     value,
  const string&     comment,
  lpsrEndlKind      endlKind)
{
  lpsrSchemeVariable* o =
    new lpsrSchemeVariable (
      inputLineNumber,
      commentedKind, variableName, value, comment, endlKind);
  assert (o != nullptr);
  return o;
}

lpsrSchemeVariable::lpsrSchemeVariable (
  int               inputLineNumber,
  lpsrCommentedKind commentedKind,
  const string&     variableName,
  const string&     value,
  const string&     comment,
  lpsrEndlKind      endlKind)
    : lpsrElement (inputLineNumber)
{
  fCommentedKind = commentedKind;

  fVariableName  = variableName;
  fVariableValue = value;

  fComment       = comment;

  fEndlKind      = endlKind;
}

lpsrSchemeVariable::~lpsrSchemeVariable ()
{}

void lpsrSchemeVariable::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrSchemeVariable::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrSchemeVariable>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeVariable>*> (v)) {
        S_lpsrSchemeVariable elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrSchemeVariable::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrSchemeVariable::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrSchemeVariable::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrSchemeVariable>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeVariable>*> (v)) {
        S_lpsrSchemeVariable elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrSchemeVariable::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrSchemeVariable::browseData (basevisitor* v)
{}

string lpsrSchemeVariable::commentedKindAsString (
  lpsrCommentedKind commentedKind)
{
  string result;

  switch (commentedKind) {
    case lpsrSchemeVariable::kCommentedYes:
      result = "commentedYes";
      break;
    case lpsrSchemeVariable::kCommentedNo:
      result = "commentedNo";
      break;
  } // switch

  return result;
}

string lpsrSchemeVariable::endlKindAsString (
  lpsrEndlKind endlKind)
{
  string result;

  switch (endlKind) {
    case lpsrSchemeVariable::kEndlOnce:
      result = "endlOnce";
      break;
    case lpsrSchemeVariable::kEndlTwice:
      result = "endlTwice";
      break;
    case lpsrSchemeVariable::kEndlNone:
      result = "endlNone";
      break;
  } // switch

  return result;
}

void lpsrSchemeVariable::print (ostream& os) const
{
  os <<
    "SchemeVariable" <<
    endl;

  ++gIndenter;

  // escape quotes if any
  string variableName;
  string variableValue;

  for_each (
    fVariableName.begin (),
    fVariableName.end (),
    mfStringQuoteEscaper (variableName));
  for_each (
    fVariableValue.begin (),
    fVariableValue.end (),
    mfStringQuoteEscaper (variableValue));

  // print resulting strings
  const int fieldWidth = 15;

  os << left <<
    setw (fieldWidth) <<
    "variableName" << " : " <<
    "\"" << variableName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "variableValue" << " : " <<
    "\"" << variableValue << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "commentedKind" << " : " <<
    commentedKindAsString (fCommentedKind) <<
    endl <<

  // backSlashKindAsString ??? JMI
  // varValSeparatorKindAsString ??? JMI
  // quotesKindAsString ??? JMI

    setw (fieldWidth) <<
    "endlKind" << " : " <<
    endlKindAsString (fEndlKind) <<
    endl;

  --gIndenter;
}

ostream& operator << (ostream& os, const S_lpsrSchemeVariable& schemeVariable)
{
  schemeVariable->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrSchemeFunction lpsrSchemeFunction::create (
  int           inputLineNumber,
  const string& functionName,
  const string& functionDescription,
  const string& functionCode)
{
  lpsrSchemeFunction* o =
    new lpsrSchemeFunction (
      inputLineNumber,
      functionName, functionDescription, functionCode);
  assert (o != nullptr);
  return o;
}

lpsrSchemeFunction::lpsrSchemeFunction (
  int           inputLineNumber,
  const string& functionName,
  const string& functionDescription,
  const string& functionCode)
    : lpsrElement (inputLineNumber)
{
  fFunctionName        = functionName;
  fFunctionDescription = functionDescription;
  fFunctionCode        = functionCode;
  }

lpsrSchemeFunction::~lpsrSchemeFunction ()
{}

void lpsrSchemeFunction::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrSchemeFunction::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrSchemeFunction>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeFunction>*> (v)) {
        S_lpsrSchemeFunction elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrSchemeFunction::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrSchemeFunction::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrSchemeFunction::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrSchemeFunction>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeFunction>*> (v)) {
        S_lpsrSchemeFunction elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrSchemeFunction::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrSchemeFunction::browseData (basevisitor* v)
{}

void lpsrSchemeFunction::print (ostream& os) const
{
  os <<
    "SchemeFunction" <<
    endl;

  ++gIndenter;

  // print resulting strings
  os <<
    "function name : \"" << fFunctionName << "\"" <<
    endl <<
    "function description: \"" << fFunctionDescription << "\"" <<
    endl <<
    "function code: \"" << fFunctionCode << "\"" <<
    endl;

  --gIndenter;
}

ostream& operator << (ostream& os, const S_lpsrSchemeFunction& schemeFunction)
{
  schemeFunction->print (os);
  return os;
}


}
