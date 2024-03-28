/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...
#include <algorithm>    // for_each

#include "visitor.h"

#include "mfStringsHandling.h"

#include "lpsrScheme.h"

#include "oahOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string lpsrCommentedKindAsString (
  lpsrCommentedKind commentedKind)
{
  std::string result;

  switch (commentedKind) {
    case lpsrCommentedKind::kCommentedYes:
      result = "kCommentedYes";
      break;
    case lpsrCommentedKind::kCommentedNo:
      result = "kCommentedNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const lpsrCommentedKind& elt)
{
  os << lpsrCommentedKindAsString (elt);
  return os;
}

std::string lpsrEndOfLineKindAsString (
  lpsrEndOfLineKind endOfLineKind)
{
  std::string result;

  switch (endOfLineKind) {
    case lpsrEndOfLineKind::kEndOfLineOnce:
      result = "kEndOfLineOnce";
      break;
    case lpsrEndOfLineKind::kEndOfLineTwice:
      result = "kEndOfLineTwice";
      break;
    case lpsrEndOfLineKind::kEndOfLineNone:
      result = "kEndOfLineNone";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const lpsrEndOfLineKind& elt)
{
  os << lpsrEndOfLineKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_lpsrSchemeVariable lpsrSchemeVariable::create (
  int               inputLineNumber,
  lpsrCommentedKind commentedKind,
  const std::string&     variableName,
  const std::string&     value,
  const std::string&     comment,
  lpsrEndOfLineKind endOfLineKind)
{
  lpsrSchemeVariable* obj =
    new lpsrSchemeVariable (
      inputLineNumber,
      commentedKind, variableName, value, comment, endOfLineKind);
  assert (obj != nullptr);
  return obj;
}

lpsrSchemeVariable::lpsrSchemeVariable (
  int               inputLineNumber,
  lpsrCommentedKind commentedKind,
  const std::string&     variableName,
  const std::string&     value,
  const std::string&     comment,
  lpsrEndOfLineKind endOfLineKind)
    : lpsrElement (inputLineNumber)
{
  fCommentedKind = commentedKind;

  fVariableName  = variableName;
  fVariableValue = value;

  fComment       = comment;

  fEndOfLineKind      = endOfLineKind;
}

lpsrSchemeVariable::~lpsrSchemeVariable ()
{}

void lpsrSchemeVariable::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrSchemeVariable::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrSchemeVariable>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeVariable>*> (v)) {
        S_lpsrSchemeVariable elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrSchemeVariable::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrSchemeVariable::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrSchemeVariable::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrSchemeVariable>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeVariable>*> (v)) {
        S_lpsrSchemeVariable elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrSchemeVariable::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lpsrSchemeVariable::browseData (basevisitor* v)
{}

void lpsrSchemeVariable::print (std::ostream& os) const
{
  os <<
    "[SchemeVariable" <<
    std::endl;

  ++gIndenter;

  // escape quotes if any
  std::string variableName;
  std::string variableValue;

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

  os << std::left <<
    std::setw (fieldWidth) <<
    "variableName" << ": " <<
    "\"" << variableName << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "variableValue" << ": " <<
    "\"" << variableValue << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fCommentedKind" << ": " <<
    fCommentedKind <<
    std::endl <<

  // backSlashKindAsString ??? JMI
  // varValSeparatorKindAsString ??? JMI
  // quotesKindAsString ??? JMI

    std::setw (fieldWidth) <<
    "fEndOfLineKind" << ": " <<
    lpsrEndOfLineKindAsString (fEndOfLineKind) <<
    std::endl <<
    ']';

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_lpsrSchemeVariable& schemeVariable)
{
  schemeVariable->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrSchemeFunction lpsrSchemeFunction::create (
  int           inputLineNumber,
  const std::string& functionName,
  const std::string& functionDescription,
  const std::string& functionCode)
{
  lpsrSchemeFunction* obj =
    new lpsrSchemeFunction (
      inputLineNumber,
      functionName, functionDescription, functionCode);
  assert (obj != nullptr);
  return obj;
}

lpsrSchemeFunction::lpsrSchemeFunction (
  int           inputLineNumber,
  const std::string& functionName,
  const std::string& functionDescription,
  const std::string& functionCode)
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
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrSchemeFunction::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrSchemeFunction>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeFunction>*> (v)) {
        S_lpsrSchemeFunction elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrSchemeFunction::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrSchemeFunction::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrSchemeFunction::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrSchemeFunction>*
    p =
      dynamic_cast<visitor<S_lpsrSchemeFunction>*> (v)) {
        S_lpsrSchemeFunction elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrSchemeFunction::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lpsrSchemeFunction::browseData (basevisitor* v)
{}

void lpsrSchemeFunction::print (std::ostream& os) const
{
  os <<
    "SchemeFunction" <<
    std::endl;

  ++gIndenter;

  // print resulting strings
  os <<
    "fFunctionName: \"" << fFunctionName << "\"" <<
    std::endl <<
    "fFunctionDescription: \"" << fFunctionDescription << "\"" <<
    std::endl <<
    "fFunctionCode: \"" << fFunctionCode << "\"" <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_lpsrSchemeFunction& schemeFunction)
{
  schemeFunction->print (os);
  return os;
}


}
