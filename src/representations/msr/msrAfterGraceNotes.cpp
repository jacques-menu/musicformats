/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"
#include "mfStringsHandling.h"



#include "msrAfterGraceNotes.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrAfterGraceNotesGroupContents msrAfterGraceNotesGroupContents::create (
  int        inputLineNumber,
  S_msrVoice afterGraceNotesGroupContentsUpLinkToVoice)
{
  msrAfterGraceNotesGroupContents* o =
    new msrAfterGraceNotesGroupContents (
      inputLineNumber,
      afterGraceNotesGroupContentsUpLinkToVoice);
  assert (o != nullptr);

  return o;
}

msrAfterGraceNotesGroupContents::msrAfterGraceNotesGroupContents (
  int        inputLineNumber,
  S_msrVoice afterGraceNotesGroupContentsUpLinkToVoice)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    afterGraceNotesGroupContentsUpLinkToVoice != nullptr,
    "afterGraceNotesGroupContentsUpLinkToVoice is null");

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
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Creating a newborn clone of after grace notes group" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrAfterGraceNotesGroupContents
    newbornClone =
      msrAfterGraceNotesGroupContents::create (
        fInputLineNumber,
        containingVoice);

  return newbornClone;
}

void msrAfterGraceNotesGroupContents::appendNoteToAfterGraceNotesGroupContents (
  S_msrNote note)
{
  fAfterGraceNotesGroupContentsNotesList.push_back (note);
}

void msrAfterGraceNotesGroupContents::appendNoteToAfterGraceNotesGroupContents (
  S_msrChord chord)
{
  fAfterGraceNotesGroupContentsNotesList.push_back (chord);
}

void msrAfterGraceNotesGroupContents::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrAfterGraceNotesGroupContents::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrAfterGraceNotesGroupContents>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotesGroupContents>*> (v)) {
        S_msrAfterGraceNotesGroupContents elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrAfterGraceNotesGroupContents::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrAfterGraceNotesGroupContents::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrAfterGraceNotesGroupContents::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrAfterGraceNotesGroupContents>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotesGroupContents>*> (v)) {
        S_msrAfterGraceNotesGroupContents elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrAfterGraceNotesGroupContents::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrAfterGraceNotesGroupContents::browseData (basevisitor* v)
{
  list<S_msrMeasureElement>::const_iterator i;

  for (
    i=fAfterGraceNotesGroupContentsNotesList.begin ();
    i!=fAfterGraceNotesGroupContentsNotesList.end ();
    ++i
  ) {
    // browse the note
    msrBrowser<msrMeasureElement> browser (v);
    browser.browse (*(*i));
  } // for
}

string msrAfterGraceNotesGroupContents::asShortString () const
{
  stringstream s;

  s <<
    "AfterGraceNotesGroupContents" <<
    ", " <<
    mfSingularOrPlural (
      fAfterGraceNotesGroupContentsNotesList.size (), "note", "notes");


  if (fAfterGraceNotesGroupContentsNotesList.size ()) {
    list<S_msrMeasureElement>::const_iterator
      iBegin = fAfterGraceNotesGroupContentsNotesList.begin (),
      iEnd   = fAfterGraceNotesGroupContentsNotesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      s << (*i)->asShortString ();
      if (++i == iEnd) break;
      s << ' ';
    } // for
  }

  return s.str ();
}

void msrAfterGraceNotesGroupContents::print (ostream& os) const
{
  os <<
    "[AfterGraceNotesGroupContents" <<
    ", " <<
    mfSingularOrPlural (
      fAfterGraceNotesGroupContentsNotesList.size (), "note", "notes") <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  if (fAfterGraceNotesGroupContentsNotesList.size ()) {
    list<S_msrMeasureElement>::const_iterator
      iBegin = fAfterGraceNotesGroupContentsNotesList.begin (),
      iEnd   = fAfterGraceNotesGroupContentsNotesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
   // JMI   os << endl;
    } // for
  }

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrAfterGraceNotesGroupContents& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }
  
  return os;
}

//______________________________________________________________________________
S_msrAfterGraceNotesGroup msrAfterGraceNotesGroup::create (
  int          inputLineNumber,
  S_msrElement afterGraceNotesGroupElement,
  Bool         afterGraceNotesGroupIsSlashed,
  S_msrVoice   afterGraceNotesGroupUpLinkToVoice)
{
  msrAfterGraceNotesGroup* o =
    new msrAfterGraceNotesGroup (
      inputLineNumber,
      afterGraceNotesGroupElement,
      afterGraceNotesGroupIsSlashed,
      afterGraceNotesGroupUpLinkToVoice);
  assert (o != nullptr);

  return o;
}

msrAfterGraceNotesGroup::msrAfterGraceNotesGroup (
  int          inputLineNumber,
  S_msrElement afterGraceNotesGroupElement,
  Bool         afterGraceNotesGroupIsSlashed,
  S_msrVoice   afterGraceNotesGroupUpLinkToVoice)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    afterGraceNotesGroupUpLinkToVoice != nullptr,
    "afterGraceNotesGroupUpLinkToVoice is null");

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
  S_msrNote  noteClone,
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Creating a newborn clone of after grace notes group '" <<
      asShortString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    noteClone != nullptr,
    "noteClone is null");

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");

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
  S_msrNote note)
{
  fAfterGraceNotesGroupContents->
    appendNoteToAfterGraceNotesGroupContents (
      note);
}

void msrAfterGraceNotesGroup::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrAfterGraceNotesGroup::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrAfterGraceNotesGroup>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotesGroup>*> (v)) {
        S_msrAfterGraceNotesGroup elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrAfterGraceNotesGroup::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrAfterGraceNotesGroup::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrAfterGraceNotesGroup::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrAfterGraceNotesGroup>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotesGroup>*> (v)) {
        S_msrAfterGraceNotesGroup elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrAfterGraceNotesGroup::visitEnd ()" <<
            endl;
        }
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

string msrAfterGraceNotesGroup::asShortString () const
{
  stringstream s;

  s <<
    "AfterGraceNotesGroup " <<
    ", AfterGraceNotesGroupElement: " <<
    "JMI ???fAfterGraceNotesGroupElement->asShortString ()" <<
    ", fAfterGraceNotesGroupContents: " <<
    fAfterGraceNotesGroupContents->asShortString ();

  return s.str ();
}

void msrAfterGraceNotesGroup::print (ostream& os) const
{
  os <<
    "[AfterGraceNotesGroup" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  // print the afterGraceNotesGroup element
  os <<
    "Element:" <<
    endl;
  ++gIndenter;
  os <<
    fAfterGraceNotesGroupElement;
  --gIndenter;

  os <<
    "fAfterGraceNotesGroupIsSlashed: " <<
    fAfterGraceNotesGroupIsSlashed <<
    endl;

  // print the afterGraceNotesGroup contents
  os <<
    fAfterGraceNotesGroupContents;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrAfterGraceNotesGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }
  
  return os;
}


}
