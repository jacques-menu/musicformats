/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "lpsrVariables.h"

#include "oahOah.h"

#include "lpsrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_lpsrVariableUseCommand lpsrVariableUseCommand::create (
  int           inputLineNumber,
  const string& variableName)
{
  lpsrVariableUseCommand* o =
    new lpsrVariableUseCommand (
      inputLineNumber, variableName);
  assert (o != nullptr);
  return o;
}

lpsrVariableUseCommand::lpsrVariableUseCommand (
  int           inputLineNumber,
  const string& variableName)
    : lpsrElement (inputLineNumber)
{
  fVariableName = variableName;
}

lpsrVariableUseCommand::~lpsrVariableUseCommand()
{}

void lpsrVariableUseCommand::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrVariableUseCommand::acceptIn()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrVariableUseCommand>*
    p =
      dynamic_cast<visitor<S_lpsrVariableUseCommand>*> (v)) {
        S_lpsrVariableUseCommand elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrVariableUseCommand::visitStart()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrVariableUseCommand::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrVariableUseCommand::acceptOut()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrVariableUseCommand>*
    p =
      dynamic_cast<visitor<S_lpsrVariableUseCommand>*> (v)) {
        S_lpsrVariableUseCommand elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrVariableUseCommand::visitEnd()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrVariableUseCommand::browseData (basevisitor* v)
{}

void lpsrVariableUseCommand::print (ostream& os) const
{
  os << "VariableUseCommand" << endl;
  ++gIndenter;
  os << fVariableName << endl;
  --gIndenter;
}

ostream& operator<< (ostream& os, const S_lpsrVariableUseCommand& nstf)
{
  nstf->print (os);
  return os;
}


}
