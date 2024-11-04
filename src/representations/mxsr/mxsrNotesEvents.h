/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsrNotesEvents___
#define ___mxsrNotesEvents___

#include <iomanip>
#include <string>
#include <list>
#include <map>

#include "smartpointer.h"

#include "mfBool.h"
#include "mfIndentedTextOutput.h"


using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
// data types

// avoid start and stop, to avoid confusion with MusicXML concepts
// the airplane analogy is to avoid confusion with a sequence of notes
// displayed on another staff

enum class mxsrNoteEventKind {
  kNoteEventChordBegin,         kNoteEventChordEnd,
  kNoteEventTupletBegin,        kNoteEventTupletEnd,
  kNoteEventStaffChangeTakeOff, kNoteEventStaffChangeLanding
};

std::string mxsrNoteEventKindAsString (
  mxsrNoteEventKind noteEventKind);

std::ostream& operator << (std::ostream& os, const mxsrNoteEventKind& elt);

//------------------------------------------------------------------------
class EXP mxsrNoteEvent : public smartable
{

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrNoteEvent> create (
                            mxsrNoteEventKind noteEventKind,
                            int               noteSequentialNumber,
                            int               noteInputStartLineNumber,
                            int               noteInputEndLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrNoteEvent (
                            mxsrNoteEventKind noteEventKind,
                            int               noteSequentialNumber,
                            int               noteInputStartLineNumber,
                            int               noteInputEndLineNumber);

    virtual               ~mxsrNoteEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    int                     getNoteSequentialNumber () const
                              { return fNoteSequentialNumber; }

    int                     getNoteInputStartLineNumber () const
                              { return fNoteInputStartLineNumber; }

    int                     getNoteInputEndLineNumber () const
                              { return fNoteInputEndLineNumber; }

  public:

    // public services
    // ------------------------------------------------------


  private:

    // private services
    // ------------------------------------------------------


  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;
    std::string           asShortString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    mxsrNoteEventKind     fNoteEventKind;

    // the input line numbers alone cannot be used as unique identifier,
    // since the end of lines are not mandatory in MusicXML date, hence:
    int                   fNoteSequentialNumber;

    int                   fNoteInputStartLineNumber;
    int                   fNoteInputEndLineNumber;
};
typedef SMARTP<mxsrNoteEvent> S_mxsrNoteEvent;
EXP std::ostream& operator << (std::ostream& os, const S_mxsrNoteEvent& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrNoteEvent& elt);

//________________________________________________________________________
bool compareFrameNotesByIncreasingSequentialNumber (
  S_mxsrNoteEvent& first,
  S_mxsrNoteEvent& second);

//------------------------------------------------------------------------
class EXP mxsrScoreNotesEvents : public smartable
{

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrScoreNotesEvents> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrScoreNotesEvents ();

    virtual               ~mxsrScoreNotesEvents ();

  public:

    // set and get
    // ------------------------------------------------------

//     int                   getNoteSequentialNumber () const
//                             { return fNoteSequentialNumber; }
//
//     int                   getNoteInputStartLineNumber () const
//                             { return fNoteInputStartLineNumber; }
//
//     int                   getNoteInputEndLineNumber () const
//                             { return fNoteInputEndLineNumber; }

  public:

    // public services
    // ------------------------------------------------------

    void                  registerChordBeginEvent (
                            int noteSequentialNumber,
                            int noteInputStartLineNumber,
                            int noteInputEndLineNumber);

    void                  registerChordEndEvent (
                            int noteSequentialNumber,
                            int noteInputStartLineNumber,
                            int noteInputEndLineNumber);

    void                  registerTupletBeginEvent (
                            int noteSequentialNumber,
                            int noteInputStartLineNumber,
                            int noteInputEndLineNumber);

    void                  registerTupletEndEvent (
                            int noteSequentialNumber,
                            int noteInputStartLineNumber,
                            int noteInputEndLineNumber);

    void                  registerStaffChangeTakeOffEvent (
                            int noteSequentialNumber,
                            int noteInputStartLineNumber,
                            int noteInputEndLineNumber);

    void                  registerStaffChangeLandingEvent (
                            int noteSequentialNumber,
                            int noteInputStartLineNumber,
                            int noteInputEndLineNumber);

    void                  sortAllNotesEventsList ();

    Bool                  aChordBeginsAtNoteSequentialNumber (
                            int noteSequentialNumber) const;
    Bool                  aChordEndsAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    Bool                  aTupletBeginsAtNoteSequentialNumber (
                            int noteSequentialNumber) const;
    Bool                  aTupletEndsAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    Bool                  aStaffChangeTakesOffAtNoteSequentialNumber (
                            int noteSequentialNumber) const;
    Bool                  aTStaffChangeLandsAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

  private:

    // private services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;
    std::string           asShortString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // per-event type maps
    std::map<int, S_mxsrNoteEvent>
                          fChordsBeginNotesEventsMap;
    std::map<int, S_mxsrNoteEvent>
                          fChordsEndNotesEventsMap;

    std::map<int, S_mxsrNoteEvent>
                          fTupletsBeginNotesEventsMap;
    std::map<int, S_mxsrNoteEvent>
                          fTupletsEndNotesEventsMap;

    std::map<int, S_mxsrNoteEvent>
                          fStaffChangesTakesOffsPositionsMap;
    std::map<int, S_mxsrNoteEvent>
                          fStaffChangesLandingsPositionsMap;

    // all events list
    std::list<S_mxsrNoteEvent>
                          fAllNotesEventsList;
};
typedef SMARTP<mxsrScoreNotesEvents> S_mxsrScoreNotesEvents;
EXP std::ostream& operator << (std::ostream& os, const S_mxsrScoreNotesEvents& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrScoreNotesEvents& elt);

}


#endif // ___mxsrNotesEvents___
