/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "mfStringsHandling.h"

#include "mfPreprocessorSettings.h"

#include "oahOah.h"

#include "mxsrOah.h"

#include "oahEarlyOptions.h"

#include "oahAtomsCollection.h"

#include "visitor.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________

S_mxsrOahGroup gGlobalMxsrOahGroup;

S_mxsrOahGroup mxsrOahGroup::create ()
{
  mxsrOahGroup* obj = new mxsrOahGroup ();
  assert (obj != nullptr);
  return obj;
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

#ifdef MF_TRACE_IS_ENABLED
void mxsrOahGroup::initializeMxsrTraceOah ()
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
        gTraceOahGroup->getShortTracePrefix (),
        gTraceOahGroup->getLongTracePrefix ());

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
#endif // MF_TRACE_IS_ENABLED

void mxsrOahGroup::initializeMxsrOah ()
{
#ifdef MF_TRACE_IS_ENABLED
  // trace
  // --------------------------------------
  initializeMxsrTraceOah ();
#endif // MF_TRACE_IS_ENABLED
}

void mxsrOahGroup::enforceGroupQuietness ()
{
#ifdef MF_TRACE_IS_ENABLED
//   fTraceMxsrVisitors = false; // JMI v0.9.63
#endif // MF_TRACE_IS_ENABLED
}

void mxsrOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void mxsrOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> mxsrOahGroup::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_mxsrOahGroup>*
    p =
      dynamic_cast<visitor<S_mxsrOahGroup>*> (v)) {
        S_mxsrOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching mxsrOahGroup::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void mxsrOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> mxsrOahGroup::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_mxsrOahGroup>*
    p =
      dynamic_cast<visitor<S_mxsrOahGroup>*> (v)) {
        S_mxsrOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching mxsrOahGroup::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void mxsrOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> mxsrOahGroup::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mxsrOahGroup::displayMxsrOahValues (int fieldWidth)
{
  gLog <<
    "The MusicXML options are:" <<
    std::endl;

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  // trace
  // --------------------------------------

  gLog <<
    "Trace:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
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
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_mxsrOahGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_mxsrOahGroup createGlobalMxsrOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global mxsr OAH group";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
