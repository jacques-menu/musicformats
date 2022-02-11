/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <regex>

#include "lpsr2lilypondOah2manPage.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif


#include "oahOah.h"

using namespace std;

namespace MusicFormats
{

//________________________________________________________________________
lpsr2lilypondOah2manPage::lpsr2lilypondOah2manPage (
  const S_oahHandler handler,
  ostream&           manPageOutputStream)
    : oah2manPage (
        handler,
        manPageOutputStream)
{}

lpsr2lilypondOah2manPage::~lpsr2lilypondOah2manPage ()
{}

//________________________________________________________________________
void lpsr2lilypondOah2manPage::visitStart (S_lilypondScoreOutputKindAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting lilypondScoreOutputKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
}

void lpsr2lilypondOah2manPage::visitEnd (S_lilypondScoreOutputKindAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting lilypondScoreOutputKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondOah2manPage::visitStart (S_lilypondTransposePartNameAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting lilypondTransposePartNameAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
}

void lpsr2lilypondOah2manPage::visitEnd (S_lilypondTransposePartNameAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting lilypondTransposePartNameAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondOah2manPage::visitStart (S_lilypondAbsoluteOctaveEntryAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting lilypondAbsoluteOctaveEntryAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
}

void lpsr2lilypondOah2manPage::visitEnd (S_lilypondAbsoluteOctaveEntryAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting lilypondAbsoluteOctaveEntryAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondOah2manPage::visitStart (S_lilypondRelativeOctaveEntryAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting lilypondRelativeOctaveEntryAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
}

void lpsr2lilypondOah2manPage::visitEnd (S_lilypondRelativeOctaveEntryAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting lilypondRelativeOctaveEntryAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondOah2manPage::visitStart (S_lilypondFixedOctaveEntryAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting lilypondFixedOctaveEntryAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
}

void lpsr2lilypondOah2manPage::visitEnd (S_lilypondFixedOctaveEntryAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting lilypondFixedOctaveEntryAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondOah2manPage::visitStart (S_lilypondAccidentalStyleKindAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting lilypondAccidentalStyleKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
  fManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
}

void lpsr2lilypondOah2manPage::visitEnd (S_lilypondAccidentalStyleKindAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting lilypondAccidentalStyleKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondOah2manPage::visitStart (S_lilypondChordsDisplayAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting lilypondChordsDisplayAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
}

void lpsr2lilypondOah2manPage::visitEnd (S_lilypondChordsDisplayAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting lilypondChordsDisplayAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondOah2manPage::visitStart (S_lilypondLyricsDurationsKindAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting lilypondLyricsDurationsKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;

}

void lpsr2lilypondOah2manPage::visitEnd (S_lilypondLyricsDurationsKindAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting lilypondLyricsDurationsKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondOah2manPage::visitStart (S_oahMidiTempoAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting oahMidiTempoAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
}

void lpsr2lilypondOah2manPage::visitEnd (S_oahMidiTempoAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting oahMidiTempoAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}


} // namespace
