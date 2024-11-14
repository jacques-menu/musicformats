/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsrEventsCollection___
#define ___mxsrEventsCollection___

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

/*
  score events are collected in mxsr2msrSkeletonBuilder
  to simplify mxsr2msrSkeletonPopulator

  these events are known to occurr one or more notes too late for most of them,
  such as in chords, tuplets and staff changes

  avoid start and stop in naming, to avoid confusion with MusicXML concepts
  the airplane analogy is to avoid confusion with a *sequence* of notes
  displayed on another staff
*/

//________________________________________________________________________
enum class mxsrChordEventKind {
  kEventChord_NONE,
  kEventChordBegin, kEventChordEnd
};

std::string mxsrChordEventKindAsString (
  mxsrChordEventKind noteChordEventKind);

std::ostream& operator << (std::ostream& os, const mxsrChordEventKind& elt);

enum class mxsrTupletEventKind {
  kEventTuplet_NONE,
  kEventTupletBegin, kEventTupletEnd
};

std::string mxsrTupletEventKindAsString (
  mxsrTupletEventKind noteTupletEventKind);

std::ostream& operator << (std::ostream& os, const mxsrTupletEventKind& elt);

enum class mxsrStaffChangeEventKind {
  kEventStaffChange_NONE,
  kEventStaffChangeTakeOff, kEventStaffChangeLanding
};

std::string mxsrStaffChangeEventKindAsString (
  mxsrStaffChangeEventKind noteStaffChangeEventKind);

std::ostream& operator << (std::ostream& os, const mxsrStaffChangeEventKind& elt);

//------------------------------------------------------------------------
class EXP mxsrEvent : public smartable
{
/* this class is purely virtual
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrEvent> create (
                            int eventSequentialNumber,
                            int eventInputStartLineNumber,
                            int eventInputEndLineNumber);
*/

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrEvent (
                            int eventSequentialNumber,
                            int eventInputStartLineNumber,
                            int eventInputEndLineNumber);

    virtual               ~mxsrEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getEventSequentialNumber () const
                              { return fEventSequentialNumber; }

    int                   getEventInputStartLineNumber () const
                              { return fEventInputStartLineNumber; }

    int                   getEventInputEndLineNumber () const
                              { return fEventInputEndLineNumber; }

  public:

    // public services
    // ------------------------------------------------------


  private:

    // private services
    // ------------------------------------------------------


  public:

    // print
    // ------------------------------------------------------

    virtual std::string   asString () const = 0;
    virtual std::string   asShortString () const = 0;

    virtual void          print (std::ostream& os) const = 0;

  protected:

    // protected fields
    // ------------------------------------------------------

    // the input line numbers alone cannot be used as unique identifier,
    // since the end of lines are not mandatory in MusicXML date, hence:
    int                   fEventSequentialNumber;

    int                   fEventInputStartLineNumber;
    int                   fEventInputEndLineNumber;
};
typedef SMARTP<mxsrEvent> S_mxsrEvent;
// EXP std::ostream& operator << (std::ostream& os, const S_mxsrEvent& elt);
// EXP std::ostream& operator << (std::ostream& os, const mxsrEvent& elt);

//________________________________________________________________________
bool compareEventsByIncreasingSequentialNumber (
  S_mxsrEvent& first,
  S_mxsrEvent& second);

bool compareEventsByIncreasingInputStartLineNumber (
  S_mxsrEvent& first,
  S_mxsrEvent& second);

//------------------------------------------------------------------------
class EXP mxsrNoteEvent : public mxsrEvent
{
/* this class is purely virtual
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrNoteEvent> create (
                            int                eventSequentialNumber,
                            int                noteSequentialNumber,
                            int                staffNumber,
                            int                voiceNumber,
                            int                eventInputStartLineNumber,
                            int                eventInputEndLineNumber);
*/

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrNoteEvent (
                            int                eventSequentialNumber,
                            int                noteSequentialNumber,
                            int                staffNumber,
                            int                voiceNumber,
                            int                eventInputStartLineNumber,
                            int                eventInputEndLineNumber);

    virtual               ~mxsrNoteEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getNoteSequentialNumber () const
                              { return fNoteSequentialNumber; }

    int                   getStaffNumber () const
                              { return fStaffNumber; }

    int                   getVoiceNumber () const
                              { return fVoiceNumber; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

//     std::string           asString () const override;
//     std::string           asShortString () const override;
//
//     void                  print (std::ostream& os) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    int                   fNoteSequentialNumber;

    int                   fStaffNumber;
    int                   fVoiceNumber;
};
typedef SMARTP<mxsrNoteEvent> S_mxsrNoteEvent;
// EXP std::ostream& operator << (std::ostream& os, const S_mxsrNoteEvent& elt);
// EXP std::ostream& operator << (std::ostream& os, const mxsrNoteEvent& elt);

//------------------------------------------------------------------------
class EXP mxsrChordEvent : public mxsrNoteEvent
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrChordEvent> create (
                            int                eventSequentialNumber,
                            int                noteSequentialNumber,
                            int                staffNumber,
                            int                voiceNumber,
                            mxsrChordEventKind chordEventKind,
                            int                eventInputStartLineNumber,
                            int                eventInputEndLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrChordEvent (
                            int                eventSequentialNumber,
                            int                noteSequentialNumber,
                            int                staffNumber,
                            int                voiceNumber,
                            mxsrChordEventKind chordEventKind,
                            int                eventInputStartLineNumber,
                            int                eventInputEndLineNumber);

    virtual               ~mxsrChordEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    mxsrChordEventKind
                          getChordEventKind () const
                              { return fChordEventKind; }

  public:

    // public services
    // ------------------------------------------------------


  private:

    // private services
    // ------------------------------------------------------


  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;
    std::string           asShortString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    mxsrChordEventKind    fChordEventKind;
};
typedef SMARTP<mxsrChordEvent> S_mxsrChordEvent;
EXP std::ostream& operator << (std::ostream& os, const S_mxsrChordEvent& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrChordEvent& elt);

//------------------------------------------------------------------------
class EXP mxsrTupletEvent : public mxsrNoteEvent
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrTupletEvent> create (
                            int                 eventSequentialNumber,
                            int                 noteSequentialNumber,
                            int                 staffNumber,
                            int                 voiceNumber,
                            mxsrTupletEventKind tupletEventKind,
                            int                 tupletNumber,
                            int                 eventInputStartLineNumber,
                            int                 eventInputEndLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrTupletEvent (
                            int                 eventSequentialNumber,
                            int                 noteSequentialNumber,
                            int                 staffNumber,
                            int                 voiceNumber,
                            mxsrTupletEventKind tupletEventKind,
                            int                 tupletNumber,
                            int                 eventInputStartLineNumber,
                            int                 eventInputEndLineNumber);

    virtual               ~mxsrTupletEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    mxsrTupletEventKind   getTupletEventKind () const
                              { return fTupletEventKind; }

    int                   getTupletNumber () const
                              { return fTupletNumber; }

  public:

    // public services
    // ------------------------------------------------------


  private:

    // private services
    // ------------------------------------------------------


  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;
    std::string           asShortString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    mxsrTupletEventKind   fTupletEventKind;

    int                   fTupletNumber;
};
typedef SMARTP<mxsrTupletEvent> S_mxsrTupletEvent;
EXP std::ostream& operator << (std::ostream& os, const S_mxsrTupletEvent& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrTupletEvent& elt);

//------------------------------------------------------------------------
class EXP mxsrStaffChangeEvent : public mxsrNoteEvent
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrStaffChangeEvent> create (
                            int                      eventSequentialNumber,
                            int                      noteSequentialNumber,
                            int                      staffNumber,
                            int                      voiceNumber,
                            mxsrStaffChangeEventKind staffChangeEventKind,
                            int                      eventInputStartLineNumber,
                            int                      eventInputEndLineNumber,
                            int                      fromStaffNumber,
                            int                      toStaffNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrStaffChangeEvent (
                            int                      eventSequentialNumber,
                            int                      noteSequentialNumber,
                            int                      staffNumber,
                            int                      voiceNumber,
                            mxsrStaffChangeEventKind staffChangeEventKind,
                            int                      eventInputStartLineNumber,
                            int                      eventInputEndLineNumber,
                            int                      fromStaffNumber,
                            int                      toStaffNumber);

    virtual               ~mxsrStaffChangeEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    mxsrStaffChangeEventKind
                          getStaffChangeEventKind () const
                              { return fStaffChangeEventKind; }

    int                   getFromStaffNumber () const
                              { return fFromStaffNumber; }

    int                   getToStaffNumber () const
                              { return fToStaffNumber; }

  public:

    // public services
    // ------------------------------------------------------


  private:

    // private services
    // ------------------------------------------------------


  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;
    std::string           asShortString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    mxsrStaffChangeEventKind
                          fStaffChangeEventKind;

    int                   fFromStaffNumber;
    int                   fToStaffNumber;
};
typedef SMARTP<mxsrStaffChangeEvent> S_mxsrStaffChangeEvent;
EXP std::ostream& operator << (std::ostream& os, const S_mxsrStaffChangeEvent& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrStaffChangeEvent& elt);

//------------------------------------------------------------------------
class EXP mxsrEventsCollection : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrEventsCollection> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrEventsCollection ();

    virtual               ~mxsrEventsCollection ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setThereIsAnImplicitInitialForwardRepeat ()
                            { fThereIsAnImplicitInitialForwardRepeat = true; }

    Bool                  getInitialRepeatBarIsImplicit () const
                            { return fThereIsAnImplicitInitialForwardRepeat; }

  public:

    // public services
    // ------------------------------------------------------

    void                  registerChordEvent (
                            int                eventSequentialNumber,
                            int                noteSequentialNumber,
                            int                staffNumber,
                            int                voiceNumber,
                            mxsrChordEventKind chordEventKind,
                            int                eventInputStartLineNumber,
                            int                eventInputEndLineNumber);

    void                  registerTupletEvent (
                            int                 eventSequentialNumber,
                            int                 noteSequentialNumber,
                            int                 staffNumber,
                            int                 voiceNumber,
                            mxsrTupletEventKind tupletEventKind,
                            int                 tupletNumber,
                            int                 eventInputStartLineNumber,
                            int                 eventInputEndLineNumber);

    void                  registerStaffChangeEvent (
                            int                      eventSequentialNumber,
                            int                      noteSequentialNumber,
                            int                      staffNumber,
                            int                      voiceNumber,
                            mxsrStaffChangeEventKind staffChangeEventKind,
                            int                      eventInputStartLineNumber,
                            int                      eventInputEndLineNumber,
                            int                      fromStaffNumber,
                            int                      toStaffNumber);

    void                  sortTheAllMxsrEventsList ();

    mxsrChordEventKind    chordEventAtNoteSequentialNumber (
                            int eventSequentialNumber) const;

    mxsrTupletEventKind   tupletEventAtNoteSequentialNumber (
                            int eventSequentialNumber) const;

    mxsrStaffChangeEventKind
                          staffChangeEventAtNoteSequentialNumber (
                            int eventSequentialNumber) const;

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

    // initial repeat bar
    Bool                  fThereIsAnImplicitInitialForwardRepeat;

    // per note event type multimaps
    std::multimap<int, S_mxsrChordEvent>
                          fChordsEventsMultiMap;

    std::multimap<int, S_mxsrTupletEvent>
                          fTupletsEventsMultiMap;

    std::multimap<int, S_mxsrStaffChangeEvent>
                          fStaffChangesEventMultiMap;

    // all events list
    std::list<S_mxsrEvent>
                          fAllEventsList;
};
typedef SMARTP<mxsrEventsCollection> S_mxsrEventsCollection;
EXP std::ostream& operator << (std::ostream& os, const S_mxsrEventsCollection& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrEventsCollection& elt);

}


#endif // ___mxsrEventsCollection___
