/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...
#include <regex>

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfStringsHandling.h"

#include "oahOah.h"

#include "outputFileOah.h"

#include "oahEarlyOptions.h"

#include "oahAtomsCollection.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
S_outputFileOahGroup gGlobalOutputFileOahGroup;

S_outputFileOahGroup outputFileOahGroup::create ()
{
  outputFileOahGroup* o = new outputFileOahGroup ();
  assert (o != nullptr);

  return o;
}

outputFileOahGroup::outputFileOahGroup ()
  : oahGroup (
    "OutputFile",
    "help-output-file-group", "hofg",
R"()",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeOutputFileOah ();
}

outputFileOahGroup::~outputFileOahGroup ()
{}

void outputFileOahGroup::initializeOutputFileOah ()
{
  // output file name
  // --------------------------------------
  initializeOutputFileNameOptions ();
}

void outputFileOahGroup::initializeOutputFileNameOptions ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
  gLogStream << left <<
    "Creating insider output file subgroup in \"" <<
    fGroupHeader <<
    "\"" <<
    endl;
  }
#endif

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Output file",
        "help-output-file", "hof",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // output filename

  fOutputFileNameStringAtom =
    oahStringAtom::create (
      "output-file-name", "o",
R"(Write output to file FILENAME instead of standard output.)",
      "FILENAME",
      "fOutputFileName",
      fOutputFileName);

  subGroup->
    appendAtomToSubGroup (
      fOutputFileNameStringAtom);

  // auto output filename

  fAutoOutputFileNameAtom =
    oahBooleanAtom::create (
      "auto-output-file-name", "aofn",
R"(This option can only be used when writing to a file.
Writethe output to a file in the current working directory.
The file name is derived from that of the input file:
any suffix after the '.' is replaced by one suited for the output format,
or such a suffix is adde if no '.' is present.)",
      "fAutoOutputFileName",
      fAutoOutputFileName);

  subGroup->
    appendAtomToSubGroup (
      fAutoOutputFileNameAtom);
}

//______________________________________________________________________________
void outputFileOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void outputFileOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void outputFileOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> outputFileOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_outputFileOahGroup>*
    p =
      dynamic_cast<visitor<S_outputFileOahGroup>*> (v)) {
        S_outputFileOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching outputFileOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void outputFileOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> outputFileOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_outputFileOahGroup>*
    p =
      dynamic_cast<visitor<S_outputFileOahGroup>*> (v)) {
        S_outputFileOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching outputFileOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void outputFileOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> outputFileOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void outputFileOahGroup::printOutputFileOahValues (int fieldWidth)
{
  gLogStream <<
    "The outputFile options are:" <<
    endl;

  ++gIndenter;

  // output file
  // --------------------------------------

  gLogStream << left <<
    setw (fieldWidth) << "Output file:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fOutputFileName" << " : \"" <<
    fOutputFileName <<
    "\"" <<
    endl <<
    setw (fieldWidth) << "fAutoOutputFileName" << " : \"" <<
    fAutoOutputFileName <<
    "\"" <<
    endl;

  --gIndenter;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_outputFileOahGroup& elt)
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
S_outputFileOahGroup createGlobalOutputFileOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global outputFile OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalOutputFileOahGroup) {
    // create the global outputFile options group
    gGlobalOutputFileOahGroup =
      outputFileOahGroup::create ();
    assert (gGlobalOutputFileOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalOutputFileOahGroup;
}


}
