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

#include "mfStringsHandling.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahOah.h"
#include "mxsrGenerationOah.h"

#include "oahEarlyOptions.h"

#include "oahAtomsCollection.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_mxsrGenerationOahGroup gGlobalMxsrGenerationOahGroup;

S_mxsrGenerationOahGroup mxsrGenerationOahGroup::create ()
{
  mxsrGenerationOahGroup* o = new mxsrGenerationOahGroup ();
  assert (o != nullptr);
  return o;
}

mxsrGenerationOahGroup::mxsrGenerationOahGroup ()
  : oahGroup (
    "Msr2mxsr",
    "help-msr-to-mxsr", "hmsr2mxsr",
R"(These options control the way MSR is translated to MXSR.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMxsrGenerationOahGroup ();
}

mxsrGenerationOahGroup::~mxsrGenerationOahGroup ()
{}

void mxsrGenerationOahGroup::initializeCodeGenerationOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Code generation",
        "help-musicxml-code-generation", "hmxmlcg",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // infos
  // --------------------------------------

  fXml2xmlInfos = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "musicxml-generation-infos", "mxmlgi",
R"(Generate initial comments showing the MusicXML generation informations.)",
        "fXml2xmlInfos",
        fXml2xmlInfos));

  // comments
  // --------------------------------------

  fMusicXMLComments = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "musicxml-comments", "xmlcom",
R"(Generate comments showing the structure of the score.)",
        "fMusicXMLComments",
        fMusicXMLComments));
}

void mxsrGenerationOahGroup::initializeMxsrGenerationOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializeMsr2mxsrtracingOah ();
#endif

  // code generation
  // --------------------------------------
  initializeCodeGenerationOptions ();
}

void mxsrGenerationOahGroup::enforceGroupQuietness ()
{}

void mxsrGenerationOahGroup::checkGroupOptionsConsistency ()
{}

void mxsrGenerationOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mxsrGenerationOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_mxsrGenerationOahGroup>*
    p =
      dynamic_cast<visitor<S_mxsrGenerationOahGroup>*> (v)) {
        S_mxsrGenerationOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mxsrGenerationOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void mxsrGenerationOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mxsrGenerationOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_mxsrGenerationOahGroup>*
    p =
      dynamic_cast<visitor<S_mxsrGenerationOahGroup>*> (v)) {
        S_mxsrGenerationOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mxsrGenerationOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void mxsrGenerationOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mxsrGenerationOahGroup::browseData ()" <<
      endl;
  }
#endif
}

void mxsrGenerationOahGroup::printMxsrGenerationOahValues (int fieldWidth)
{
  gLogStream <<
    "The MusicXML options are:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fXml2xmlInfos" << " : " <<
      fXml2xmlInfos <<
      endl <<

    setw (fieldWidth) << "fMusicXMLComments" << " : " <<
      fMusicXMLComments <<
      endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_mxsrGenerationOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_mxsrGenerationOahGroup createGlobalMxsrGenerationOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global msr2mxsr OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMxsrGenerationOahGroup) {
    // create the global options group
    gGlobalMxsrGenerationOahGroup =
      mxsrGenerationOahGroup::create ();
    assert (gGlobalMxsrGenerationOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMxsrGenerationOahGroup;
}


}
