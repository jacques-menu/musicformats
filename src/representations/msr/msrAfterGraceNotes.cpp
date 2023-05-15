/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

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
  int               inputLineNumber,
  const S_msrVoice& afterGraceNotesGroupContentsUpLinkToVoice)
{
  msrAfterGraceNotesGroupContents* obj =
    new msrAfterGraceNotesGroupContents (
      inputLineNumber,
      afterGraceNotesGroupContentsUpLinkToVoice);
  assert (obj != nullptr);
  return obj;
}

msrAfterGraceNotesGroupContents::msrAfterGraceNotesGroupContents (
  int               inputLineNumber,
  const S_msrVoice& afterGraceNotesGroupContentsUpLinkToVoice)
    : msrElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    afterGraceNotesGroupContentsUpLinkToVoice != nullptr,
    "afterGraceNotesGroupContentsUpLinkToVoice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // set after notes contents's voice upLink
  fAfterGraceNotesGroupContentsUpLinkToVoice =
    afterGraceNotesGroupContentsUpLinkToVoice;
}

msrAfterGraceNotesGroupContents::~msrAfterGraceNotesGroupContents ()
{}

S_msrPart msrAfterGraceNotesGroupContents::fetchAfterGraceNotesGroupContentsUpLinkToPart () const
{
  S_msrPart result;

  if (fAfterGraceNotesGroupContentsUpLinkToVoice) {
    result =
      fAfterGraceNotesGroupContentsUpLinkToVoice->
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
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrAfterGraceNotesGroupContents
    newbornClone =
      msrAfterGraceNotesGroupContents::create (
        fInputStartLineNumber,
        containingVoice);

  return newbornClone;
}

void msrAfterGraceNotesGroupContents::appendNoteToAfterGraceNotesGroupContents (
  const S_msrNote& note)
{
  fAfterGraceNotesGroupContentsNotesList.push_back (note);
}

void msrAfterGraceNotesGroupContents::appendNoteToAfterGraceNotesGroupContents (
  const S_msrChord& chord)
{
  fAfterGraceNotesGroupContentsNotesList.push_back (chord);
}

void msrAfterGraceNotesGroupContents::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrAfterGraceNotesGroupContents::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
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
            __FILE__, __LINE__,
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
      __FILE__, __LINE__,
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
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrAfterGraceNotesGroupContents::browseData (basevisitor* v)
{
  std::list<S_msrMeasureElement>::const_iterator i;

  for (
    i = fAfterGraceNotesGroupContentsNotesList.begin ();
    i != fAfterGraceNotesGroupContentsNotesList.end ();
    ++i
  ) {
    // browse the note
    msrBrowser<msrMeasureElement> browser (v);
    browser.browse (*(*i));
  } // for
}

std::string msrAfterGraceNotesGroupContents::asShortString () const
{
  std::stringstream ss;

  ss <<
    "AfterGraceNotesGroupContents" <<
    ", " <<
    mfSingularOrPlural (
      fAfterGraceNotesGroupContentsNotesList.size (), "note", "notes");


  if (fAfterGraceNotesGroupContentsNotesList.size ()) {
    std::list<S_msrMeasureElement>::const_iterator
      iBegin = fAfterGraceNotesGroupContentsNotesList.begin (),
      iEnd   = fAfterGraceNotesGroupContentsNotesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      ss << (*i)->asShortString ();
      if (++i == iEnd) break;
      ss << ' ';
    } // for
  }

  return ss.str ();
}

void msrAfterGraceNotesGroupContents::print (std::ostream& os) const
{
  os <<
    "[AfterGraceNotesGroupContents" <<
    ", " <<
    mfSingularOrPlural (
      fAfterGraceNotesGroupContentsNotesList.size (), "note", "notes") <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  if (fAfterGraceNotesGroupContentsNotesList.size ()) {
    std::list<S_msrMeasureElement>::const_iterator
      iBegin = fAfterGraceNotesGroupContentsNotesList.begin (),
      iEnd   = fAfterGraceNotesGroupContentsNotesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
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
  int          inputLineNumber,
  const S_msrElement& afterGraceNotesGroupElement,
  Bool         afterGraceNotesGroupIsSlashed,
  const S_msrVoice&   afterGraceNotesGroupUpLinkToVoice)
{
  msrAfterGraceNotesGroup* obj =
    new msrAfterGraceNotesGroup (
      inputLineNumber,
      afterGraceNotesGroupElement,
      afterGraceNotesGroupIsSlashed,
      afterGraceNotesGroupUpLinkToVoice);
  assert (obj != nullptr);
  return obj;
}

msrAfterGraceNotesGroup::msrAfterGraceNotesGroup (
  int          inputLineNumber,
  const S_msrElement& afterGraceNotesGroupElement,
  Bool         afterGraceNotesGroupIsSlashed,
  const S_msrVoice&   afterGraceNotesGroupUpLinkToVoice)
    : msrElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    afterGraceNotesGroupUpLinkToVoice != nullptr,
    "afterGraceNotesGroupUpLinkToVoice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // set after gracenotes group voice upLink
  fAfterGraceNotesGroupUpLinkToVoice =
    afterGraceNotesGroupUpLinkToVoice;

  // pupulate this after grace notes group
  fAfterGraceNotesGroupElement =
    afterGraceNotesGroupElement;

  fAfterGraceNotesGroupIsSlashed =
    afterGraceNotesGroupIsSlashed;

  // create the after grace notes contents
  fAfterGraceNotesGroupContents =
    msrAfterGraceNotesGroupContents::create (
      inputLineNumber,
      afterGraceNotesGroupUpLinkToVoice);
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
      asShortString () <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    noteClone != nullptr,
    "noteClone is null");

  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrAfterGraceNotesGroup
    newbornClone =
      msrAfterGraceNotesGroup::create (
        fInputStartLineNumber,
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
      __FILE__, __LINE__,
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
            __FILE__, __LINE__,
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
      __FILE__, __LINE__,
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
            __FILE__, __LINE__,
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

std::string msrAfterGraceNotesGroup::asShortString () const
{
  std::stringstream ss;

  ss <<
    "AfterGraceNotesGroup " <<
    ", AfterGraceNotesGroupElement: " <<
    "JMI ???fAfterGraceNotesGroupElement->asShortString ()" <<
    ", fAfterGraceNotesGroupContents: " <<
    fAfterGraceNotesGroupContents->asShortString ();

  return ss.str ();
}

void msrAfterGraceNotesGroup::print (std::ostream& os) const
{
  os <<
    "[AfterGraceNotesGroup" <<
    ", line " << fInputStartLineNumber <<
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
