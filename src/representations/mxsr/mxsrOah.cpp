/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "mfStringsHandling.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahOah.h"

#include "mxsrOah.h"

#include "oahEarlyOptions.h"

#include "oahAtomsCollection.h"

#include "visitor.h"


namespace MusicFormats
{

//_______________________________________________________________________________

S_mxsrOahGroup gGlobalMxsrOahGroup;

S_mxsrOahGroup mxsrOahGroup::create ()
{
  mxsrOahGroup* o = new mxsrOahGroup ();
  assert (o != nullptr);
  return o;
}

mxsrOahGroup::mxsrOahGroup ()
  : oahGroup (
    "Mxsr",
    "help-mxsr", "hmxsr",
R"(These options control the way MXSR data are handled.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMxsrOah ();
}

mxsrOahGroup::~mxsrOahGroup ()
{}

#ifdef TRACING_IS_ENABLED
void mxsrOahGroup::initializeMxsrtracingOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "MXSR Trace",
        "help-mxsr-trace", "hmxsrt",
R"()",
        oahElementVisibilityKind::kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // the 'MusicXML' multiplex booleans atom

  S_oahCommonPrefixBooleansAtom
    musicxmlMultiplexBooleansAtom =
      oahCommonPrefixBooleansAtom::create (
        "trace-when-handling-musicxml-data", "twhmd",
        "Trace SHORT_NAME/LONG_NAME when handling MusicXML data.",
        "SHORT_NAME",
        "LONG_NAME",
        gGlobalTracingOahGroup->getShortTracePrefix (),
        gGlobalTracingOahGroup->getLongTracePrefix ());

  subGroup->
    appendAtomToSubGroup (
      musicxmlMultiplexBooleansAtom);

  // encoding

  S_oahBooleanAtom
    traceEncodingAtom =
      oahBooleanAtom::create (
        "trace-encoding", "tenc",
R"(Encoding)",
        "fTraceEncoding",
        fTraceEncoding);
  subGroup->
    appendAtomToSubGroup (
      traceEncodingAtom);
  musicxmlMultiplexBooleansAtom->
    addBooleanAtom (
      traceEncodingAtom);

  // divisions

  S_oahBooleanAtom
    traceDivisionsAtom =
      oahBooleanAtom::create (
        "trace-divisions", "tdivs",
R"(Divisions)",
        "fTraceDivisions",
        fTraceDivisions);
  subGroup->
    appendAtomToSubGroup (
      traceDivisionsAtom);
  musicxmlMultiplexBooleansAtom->
    addBooleanAtom (
      traceDivisionsAtom);

  // backup

  S_oahBooleanAtom
    traceBackupAtom =
      oahBooleanAtom::create (
        "trace-backup", "tbackup",
R"(Backup)",
        "fTraceBackup",
        fTraceBackup);
  subGroup->
    appendAtomToSubGroup (
      traceBackupAtom);

  // forward

  S_oahBooleanAtom
    traceForwardAtom =
      oahBooleanAtom::create (
        "trace-forward", "tforward",
R"(Forward)",
        "fTraceForward",
        fTraceForward);
  subGroup->
    appendAtomToSubGroup (
      traceForwardAtom);

  // MusicXML tree

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-mxsr", "tmxsr",
R"(Write a trace of the MusicXML tree activity to standard error.)",
        "fTraceMxsr",
        fTraceMxsr));

  // MusicXML tree visiting

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
         "trace-mxsr-visitors", "tmxsrvis",
R"(Write a trace of the MusicXML tree visiting activity to standard error.)",
        "fTraceMxsrVisitors",
        fTraceMxsrVisitors));
}
#endif

void mxsrOahGroup::initializeMxsrOah ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  initializeMxsrtracingOah ();
#endif
}

void mxsrOahGroup::enforceGroupQuietness ()
{
#ifdef TRACING_IS_ENABLED
//   fTraceMxsrVisitors = false; // JMI v0.9.63
#endif
}

void mxsrOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void mxsrOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mxsrOahGroup::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_mxsrOahGroup>*
    p =
      dynamic_cast<visitor<S_mxsrOahGroup>*> (v)) {
        S_mxsrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mxsrOahGroup::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void mxsrOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mxsrOahGroup::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_mxsrOahGroup>*
    p =
      dynamic_cast<visitor<S_mxsrOahGroup>*> (v)) {
        S_mxsrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mxsrOahGroup::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void mxsrOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mxsrOahGroup::browseData ()" <<
      std::endl;
  }
#endif
}

void mxsrOahGroup::printMxsrOahValues (int fieldWidth)
{
  gLogStream <<
    "The MusicXML options are:" <<
    std::endl;

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------

  gLogStream <<
    "Trace:" <<
    std::endl;

  ++gIndenter;

  gLogStream << std::left <<
    std::setw (fieldWidth) << "fTraceEncoding" << ": " <<
    fTraceEncoding <<
    std::endl <<

    std::setw (fieldWidth) << "fTraceDivisions" << ": " <<
    fTraceDivisions <<
    std::endl <<

    std::setw (fieldWidth) << "fTraceBackup" << ": " <<
    fTraceBackup <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceForward" << ": " <<
    fTraceForward <<
    std::endl <<

    std::setw (fieldWidth) << "fTraceMxsr" << ": " <<
    fTraceMxsr <<
    std::endl <<
    std::setw (fieldWidth) << "fTraceMxsrVisitors" << ": " <<
    fTraceMxsrVisitors <<
    std::endl;

  --gIndenter;
#endif

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_mxsrOahGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }
  
  return os;
}

//______________________________________________________________________________
S_mxsrOahGroup createGlobalMxsrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global mxsr OAH group" <<
      std::endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMxsrOahGroup) {
    // create the options groups
    gGlobalMxsrOahGroup =
      mxsrOahGroup::create ();
    assert (gGlobalMxsrOahGroup != 0);
  }

  // return global OAH group
  return gGlobalMxsrOahGroup;
}


}
