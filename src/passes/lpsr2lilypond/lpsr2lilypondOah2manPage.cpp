/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <regex>

#include "lpsr2lilypondOah2manPage.h"

#include "mfStaticSettings.h"

#include "oahOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
lpsr2lilypondOah2manPage::lpsr2lilypondOah2manPage (
  const S_oahHandler& handler,
  std::ostream&       manPageOutputStream)
    : oah2manPage (
        handler,
        manPageOutputStream)
{}

lpsr2lilypondOah2manPage::~lpsr2lilypondOah2manPage ()
{}

//________________________________________________________________________
void lpsr2lilypondOah2manPage::visitStart (S_lilypondScoreOutputKindAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting lilypondScoreOutputKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fManPageOutputStream <<
    ".HP" <<
    std::endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), std::regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), std::regex ("-"), "\\-") <<
    "\\fR, " <<
    std::endl <<

    ".IP" <<
    std::endl <<
    elt->getDescription () <<
    std::endl;
}

void lpsr2lilypondOah2manPage::visitEnd (S_lilypondScoreOutputKindAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting lilypondScoreOutputKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondOah2manPage::visitStart (S_lilypondTransposePartNameAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting lilypondTransposePartNameAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fManPageOutputStream <<
    ".HP" <<
    std::endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), std::regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), std::regex ("-"), "\\-") <<
    "\\fR, " <<
    std::endl <<

    ".IP" <<
    std::endl <<
    elt->getDescription () <<
    std::endl;
}

void lpsr2lilypondOah2manPage::visitEnd (S_lilypondTransposePartNameAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting lilypondTransposePartNameAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondOah2manPage::visitStart (S_lilypondAbsoluteOctaveEntryAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting lilypondAbsoluteOctaveEntryAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fManPageOutputStream <<
    ".HP" <<
    std::endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), std::regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), std::regex ("-"), "\\-") <<
    "\\fR, " <<
    std::endl <<

    ".IP" <<
    std::endl <<
    elt->getDescription () <<
    std::endl;
}

void lpsr2lilypondOah2manPage::visitEnd (S_lilypondAbsoluteOctaveEntryAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting lilypondAbsoluteOctaveEntryAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondOah2manPage::visitStart (S_lilypondRelativeOctaveEntryAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting lilypondRelativeOctaveEntryAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fManPageOutputStream <<
    ".HP" <<
    std::endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), std::regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), std::regex ("-"), "\\-") <<
    "\\fR, " <<
    std::endl <<

    ".IP" <<
    std::endl <<
    elt->getDescription () <<
    std::endl;
}

void lpsr2lilypondOah2manPage::visitEnd (S_lilypondRelativeOctaveEntryAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting lilypondRelativeOctaveEntryAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondOah2manPage::visitStart (S_lilypondFixedOctaveEntryAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting lilypondFixedOctaveEntryAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fManPageOutputStream <<
    ".HP" <<
    std::endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), std::regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), std::regex ("-"), "\\-") <<
    "\\fR, " <<
    std::endl <<

    ".IP" <<
    std::endl <<
    elt->getDescription () <<
    std::endl;
}

void lpsr2lilypondOah2manPage::visitEnd (S_lilypondFixedOctaveEntryAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting lilypondFixedOctaveEntryAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondOah2manPage::visitStart (S_lilypondAccidentalStyleKindAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting lilypondAccidentalStyleKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
  fManPageOutputStream <<
    ".HP" <<
    std::endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), std::regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), std::regex ("-"), "\\-") <<
    "\\fR, " <<
    std::endl <<

    ".IP" <<
    std::endl <<
    elt->getDescription () <<
    std::endl;
}

void lpsr2lilypondOah2manPage::visitEnd (S_lilypondAccidentalStyleKindAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting lilypondAccidentalStyleKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondOah2manPage::visitStart (S_lilypondChordsDisplayAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting lilypondChordsDisplayAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fManPageOutputStream <<
    ".HP" <<
    std::endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), std::regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), std::regex ("-"), "\\-") <<
    "\\fR, " <<
    std::endl <<

    ".IP" <<
    std::endl <<
    elt->getDescription () <<
    std::endl;
}

void lpsr2lilypondOah2manPage::visitEnd (S_lilypondChordsDisplayAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting lilypondChordsDisplayAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondOah2manPage::visitStart (S_lilypondLyricsNotesDurationsKindAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting lilypondLyricsNotesDurationsKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fManPageOutputStream <<
    ".HP" <<
    std::endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), std::regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), std::regex ("-"), "\\-") <<
    "\\fR, " <<
    std::endl <<

    ".IP" <<
    std::endl <<
    elt->getDescription () <<
    std::endl;

}

void lpsr2lilypondOah2manPage::visitEnd (S_lilypondLyricsNotesDurationsKindAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting lilypondLyricsNotesDurationsKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondOah2manPage::visitStart (S_oahMidiTempoAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting oahMidiTempoAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fManPageOutputStream <<
    ".HP" <<
    std::endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), std::regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), std::regex ("-"), "\\-") <<
    "\\fR, " <<
    std::endl <<

    ".IP" <<
    std::endl <<
    elt->getDescription () <<
    std::endl;
}

void lpsr2lilypondOah2manPage::visitEnd (S_oahMidiTempoAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting oahMidiTempoAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}


} // namespace
