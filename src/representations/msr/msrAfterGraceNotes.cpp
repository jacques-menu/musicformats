/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfStringsHandling.h"

#include "msrAfterGraceNotes.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrAfterGraceNotesGroupContents msrAfterGraceNotesGroupContents::create (
  const mfInputLineNumber& inputLineNumber,
  const S_msrVoice&        upLinkToVoice)
{
  msrAfterGraceNotesGroupContents* obj =
    new msrAfterGraceNotesGroupContents (
      inputLineNumber,
      upLinkToVoice);
  assert (obj != nullptr);
  return obj;
}

msrAfterGraceNotesGroupContents::msrAfterGraceNotesGroupContents (
  const mfInputLineNumber& inputLineNumber,
  const S_msrVoice&        upLinkToVoice)
    : msrElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    upLinkToVoice != nullptr,
    "upLinkToVoice is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // set after notes contents's voice upLink
  fUpLinkToVoice = upLinkToVoice;
}

msrAfterGraceNotesGroupContents::~msrAfterGraceNotesGroupContents ()
{}

S_msrPart msrAfterGraceNotesGroupContents::fetchAfterGraceNotesGroupContentsUpLinkToPart () const
{
  S_msrPart result;

  if (fUpLinkToVoice) {
    result =
      fUpLinkToVoice->
        fetchVoiceUpLinkToPart ();
  }

  return result;
}

S_msrAfterGraceNotesGroupContents msrAfterGraceNotesGroupContents::createAfterGraceNotesGroupContentsNewbornClone (
  const S_msrVoice& containingVoice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of after grace notes group";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    containingVoice != nullptr,
    "containingVoice is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrAfterGraceNotesGroupContents
    newbornClone =
      msrAfterGraceNotesGroupContents::create (
        fInputLineNumber,
        containingVoice);

  return newbornClone;
}

void msrAfterGraceNotesGroupContents::appendNoteToAfterGraceNotesGroupContents (
  const S_msrNote& note)
{
  fMeasureElementsList.push_back (note);
}

void msrAfterGraceNotesGroupContents::appendNoteToAfterGraceNotesGroupContents (
  const S_msrChord& chord)
{
  fMeasureElementsList.push_back (chord);
}

void msrAfterGraceNotesGroupContents::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrAfterGraceNotesGroupContents::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrAfterGraceNotesGroupContents>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotesGroupContents>*> (v)) {
        S_msrAfterGraceNotesGroupContents elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrAfterGraceNotesGroupContents::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrAfterGraceNotesGroupContents::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrAfterGraceNotesGroupContents::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrAfterGraceNotesGroupContents>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotesGroupContents>*> (v)) {
        S_msrAfterGraceNotesGroupContents elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrAfterGraceNotesGroupContents::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrAfterGraceNotesGroupContents::browseData (basevisitor* v)
{
  for (S_msrMeasureElement measureElement : fMeasureElementsList) {
    // browse the measure element
    msrBrowser<msrMeasureElement> browser (v);
    browser.browse (*measureElement);
  } // for
}

std::string msrAfterGraceNotesGroupContents::asString () const
{
  std::stringstream ss;

  ss <<
    "[AfterGraceNotesGroupContents" <<
    ", fUpLinkToVoice: " <<
    msrElementAsStringOrNULL (fUpLinkToVoice);

  ss <<
    ", fMeasureElementsList: ";
  if (! fMeasureElementsList.empty ()) {
    std::list <S_msrMeasureElement>::const_iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrMeasureElement measureElement = (*i);

      ss << measureElement->asString ();
      if (++i == iEnd) break;
      ss << "' ";
    } // for
  }

  ss << ']';

  return ss.str ();
}

void msrAfterGraceNotesGroupContents::print (std::ostream& os) const
{
  os <<
    "[AfterGraceNotesGroupContents" <<
    ", " <<
    mfSingularOrPlural (
      fMeasureElementsList.size (), "note", "notes") <<
    ", " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  if (fMeasureElementsList.size ()) {
    std::list <S_msrMeasureElement>::const_iterator
      iBegin = fMeasureElementsList.begin (),
      iEnd   = fMeasureElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrMeasureElement measureElement = (*i);

      os << measureElement;
      if (++i == iEnd) break;
   // JMI   os << std::endl;
    } // for
  }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrAfterGraceNotesGroupContents& elt)
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
S_msrAfterGraceNotesGroup msrAfterGraceNotesGroup::create (
  const mfInputLineNumber& inputLineNumber,
  const S_msrElement& afterGraceNotesGroupElement,
  Bool         afterGraceNotesGroupIsSlashed,
  const S_msrVoice&   upLinkToVoice)
{
  msrAfterGraceNotesGroup* obj =
    new msrAfterGraceNotesGroup (
      inputLineNumber,
      afterGraceNotesGroupElement,
      afterGraceNotesGroupIsSlashed,
      upLinkToVoice);
  assert (obj != nullptr);
  return obj;
}

msrAfterGraceNotesGroup::msrAfterGraceNotesGroup (
  const mfInputLineNumber& inputLineNumber,
  const S_msrElement& afterGraceNotesGroupElement,
  Bool         afterGraceNotesGroupIsSlashed,
  const S_msrVoice&   upLinkToVoice)
    : msrElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    upLinkToVoice != nullptr,
    "upLinkToVoice is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // set after gracenotes group voice upLink
  fAfterGraceNotesGroupUpLinkToVoice =
    upLinkToVoice;

  // pupulate this after grace notes group
  fAfterGraceNotesGroupElement =
    afterGraceNotesGroupElement;

  fAfterGraceNotesGroupIsSlashed =
    afterGraceNotesGroupIsSlashed;

  // create the after grace notes contents
  fAfterGraceNotesGroupContents =
    msrAfterGraceNotesGroupContents::create (
      inputLineNumber,
      upLinkToVoice);
}

msrAfterGraceNotesGroup::~msrAfterGraceNotesGroup ()
{}

S_msrPart msrAfterGraceNotesGroup::fetchAfterGraceNotesGroupUpLinkToPart () const
{
  S_msrPart result;

  if (fAfterGraceNotesGroupUpLinkToVoice) {
    result =
      fAfterGraceNotesGroupUpLinkToVoice->
        fetchVoiceUpLinkToPart ();
  }

  return result;
}

S_msrAfterGraceNotesGroup msrAfterGraceNotesGroup::createAfterGraceNotesGroupNewbornClone (
  const S_msrNote&  noteClone,
  const S_msrVoice& containingVoice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of after grace notes group '" <<
      asString () <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    noteClone != nullptr,
    "noteClone is NULL");

  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    containingVoice != nullptr,
    "containingVoice is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrAfterGraceNotesGroup
    newbornClone =
      msrAfterGraceNotesGroup::create (
        fInputLineNumber,
        noteClone,
        fAfterGraceNotesGroupIsSlashed,
        containingVoice);

  return newbornClone;
}

void msrAfterGraceNotesGroup::appendNoteToAfterGraceNotesGroupContents (
  const S_msrNote& note)
{
  fAfterGraceNotesGroupContents->
    appendNoteToAfterGraceNotesGroupContents (
      note);
}

void msrAfterGraceNotesGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrAfterGraceNotesGroup::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrAfterGraceNotesGroup>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotesGroup>*> (v)) {
        S_msrAfterGraceNotesGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrAfterGraceNotesGroup::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrAfterGraceNotesGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrAfterGraceNotesGroup::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrAfterGraceNotesGroup>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotesGroup>*> (v)) {
        S_msrAfterGraceNotesGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrAfterGraceNotesGroup::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrAfterGraceNotesGroup::browseData (basevisitor* v)
{
  {
    // browse the afterGraceNotesGroup note
    msrBrowser<msrElement> browser (v);
    browser.browse (*fAfterGraceNotesGroupElement);
  }

  {
    // browse the afterGraceNotesGroupContents
    msrBrowser<msrAfterGraceNotesGroupContents> browser (v);
    browser.browse (*fAfterGraceNotesGroupContents);
  }
}

std::string msrAfterGraceNotesGroup::asString () const
{
  std::stringstream ss;

  ss <<
    "[AfterGraceNotesGroup " <<
    ", AfterGraceNotesGroupElement: " <<
    fAfterGraceNotesGroupElement->asString () <<
    ", fAfterGraceNotesGroupContents: " <<
    fAfterGraceNotesGroupContents->asString () <<
    ']';

  return ss.str ();
}

void msrAfterGraceNotesGroup::print (std::ostream& os) const
{
  os <<
    "[AfterGraceNotesGroup" <<
    ", " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  // print the afterGraceNotesGroup element
  os <<
    "Element:" <<
    std::endl;
  ++gIndenter;
  os <<
    fAfterGraceNotesGroupElement;
  --gIndenter;

  os <<
    "fAfterGraceNotesGroupIsSlashed: " <<
    fAfterGraceNotesGroupIsSlashed <<
    std::endl;

  // print the afterGraceNotesGroup contents
  os <<
    fAfterGraceNotesGroupContents;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrAfterGraceNotesGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
