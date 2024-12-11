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
enum class mxsrStaffChangeEventKind {
  kEventStaffChange_NONE,
  kEventStaffChangeTakeOff, kEventStaffChangeLanding
};

std::string mxsrStaffChangeEventKindAsString (
  mxsrStaffChangeEventKind noteStaffChangeEventKind);

std::ostream& operator << (std::ostream& os, const mxsrStaffChangeEventKind& elt);

//________________________________________________________________________
enum class mxsrChordEventKind {
  kEventChord_NONE,
  kEventChordBegin, kEventChordEnd
};

std::string mxsrChordEventKindAsString (
  mxsrChordEventKind noteChordEventKind);

std::ostream& operator << (std::ostream& os, const mxsrChordEventKind& elt);

//________________________________________________________________________
enum class mxsrTupletEventKind {
  kEventTuplet_NONE,
  kEventTupletBegin, kEventTupletEnd
};

std::string mxsrTupletEventKindAsString (
  mxsrTupletEventKind noteTupletEventKind);

std::ostream& operator << (std::ostream& os, const mxsrTupletEventKind& elt);

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
                            int eventSequentialNumber,
                            int noteSequentialNumber,
                            int noteEventStaffNumber,
                            int noteEventVoiceNumber,
                            int eventInputStartLineNumber,
                            int eventInputEndLineNumber);
*/

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrNoteEvent (
                            int eventSequentialNumber,
                            int noteSequentialNumber,
                            int noteEventStaffNumber,
                            int noteEventVoiceNumber,
                            int eventInputStartLineNumber,
                            int eventInputEndLineNumber);

    virtual               ~mxsrNoteEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getNoteSequentialNumber () const
                              { return fNoteSequentialNumber; }

    int                   getNoteEventStaffNumber () const
                              { return fNoteEventStaffNumber; }

    int                   getNoteEventVoiceNumber () const
                              { return fNoteEventVoiceNumber; }

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

    // the staff and voice numbers of the note upon which the event occurs
    int                   fNoteEventStaffNumber;
    int                   fNoteEventVoiceNumber;
};
typedef SMARTP<mxsrNoteEvent> S_mxsrNoteEvent;
// EXP std::ostream& operator << (std::ostream& os, const S_mxsrNoteEvent& elt);
// EXP std::ostream& operator << (std::ostream& os, const mxsrNoteEvent& elt);

//------------------------------------------------------------------------
class EXP mxsrStaffChangeEvent : public mxsrNoteEvent
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrStaffChangeEvent> create (
                            int                      eventSequentialNumber,
                            int                      noteSequentialNumber,
                            int                      noteEventStaffNumber,
                            int                      noteEventVoiceNumber,
                            mxsrStaffChangeEventKind staffChangeEventKind,
                            int                      takeOffStaffNumber,
                            int                      landingStaffNumber,
                            int                      takeOffInputStartLineNumber,
                            int                      landingInputStartLineNumber,
                            int                      eventInputStartLineNumber,
                            int                      eventInputEndLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrStaffChangeEvent (
                            int                      eventSequentialNumber,
                            int                      noteSequentialNumber,
                            int                      noteEventStaffNumber,
                            int                      noteEventVoiceNumber,
                            mxsrStaffChangeEventKind staffChangeEventKind,
                            int                      takeOffStaffNumber,
                            int                      landingStaffNumber,
                            int                      takeOffInputStartLineNumber,
                            int                      landingInputStartLineNumber,
                            int                      eventInputStartLineNumber,
                            int                      eventInputEndLineNumber);

    virtual               ~mxsrStaffChangeEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    mxsrStaffChangeEventKind
                          getStaffChangeEventKind () const
                              { return fStaffChangeEventKind; }

    // the staff change voice number is the one of the note
    // upon which the staff change occurds
    int                   getStaffChangeVoicefNumber () const
                              { return fNoteEventVoiceNumber; }

    int                   getTakeOffStaffNumber () const
                              { return fTakeOffStaffNumber; }

    int                   getLandingStaffNumber () const
                              { return fLandingStaffNumber; }

    int                   getTakeOffInputStartLineNumber () const
                              { return fTakeOffInputStartLineNumber; }

    int                   getLandingInputStartLineNumber () const
                              { return fLandingInputStartLineNumber; }

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

    // the staff change voice number is the one of the note
    // upon which the staff change occurds
//     int                   fStaffChangeVoicefNumber;

    // a staff change event is attached to the take off note,
    // so as to know early that there is staff change

    // the staff numbers differ for take off and landing

    // the take off staff number is that of the note event,
    // and we need to store the landing staff number
    int                   fTakeOffStaffNumber;
    int                   fTakeOffInputStartLineNumber;

    int                   fLandingStaffNumber;
    int                   fLandingInputStartLineNumber;
};
typedef SMARTP<mxsrStaffChangeEvent> S_mxsrStaffChangeEvent;
EXP std::ostream& operator << (std::ostream& os, const S_mxsrStaffChangeEvent& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrStaffChangeEvent& elt);

bool compareStaffChangeEventsByIncreasingInputStartLineNumber (
  S_mxsrStaffChangeEvent& first,
  S_mxsrStaffChangeEvent& second);

//------------------------------------------------------------------------
class EXP mxsrChordEvent : public mxsrNoteEvent
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrChordEvent> create (
                            int                eventSequentialNumber,
                            int                noteSequentialNumber,
                            int                noteEventStaffNumber,
                            int                noteEventVoiceNumber,
                            mxsrChordEventKind chordEventKind,
                            int                eventInputStartLineNumber,
                            int                eventInputEndLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrChordEvent (
                            int                eventSequentialNumber,
                            int                noteSequentialNumber,
                            int                noteEventStaffNumber,
                            int                noteEventVoiceNumber,
                            mxsrChordEventKind chordEventKind,
                            int                eventInputStartLineNumber,
                            int                eventInputEndLineNumber);

    virtual               ~mxsrChordEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    mxsrChordEventKind    getChordEventKind () const
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
                            int                 noteEventStaffNumber,
                            int                 noteEventVoiceNumber,
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
                            int                 noteEventStaffNumber,
                            int                 noteEventVoiceNumber,
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

    int                   getCurrentEventSequentialNumber () const
                              { return fCurrentEventSequentialNumber; }

    // implicit initial forward repeat
    void                  setThereIsAnImplicitInitialForwardRepeat ()
                              { fThereIsAnImplicitInitialForwardRepeat = true; }

    Bool                  getInitialRepeatBarIsImplicit () const
                              { return fThereIsAnImplicitInitialForwardRepeat; }

    // staff change events
    const std::map <int, S_mxsrStaffChangeEvent>&
                          getStaffChangeTakeOffsMap () const
                              { return fStaffChangeTakeOffsMap; }

    const std::map <int, S_mxsrStaffChangeEvent>&
                          getStaffChangeLandingsMap () const
                              { return fStaffChangeLandingsMap; }

    const std::list <S_mxsrStaffChangeEvent>&
                          getStaffChangeEventsList () const
                              { return fStaffChangeEventsList; }

    // chord events
    const std::map <int, S_mxsrChordEvent>&
                          getChordsEventMap () const
                              { return fChordsEventsMap; }

    // tuplet events
    const std::list <S_mxsrTupletEvent>&
                          getTupletsBeginsList () const
                              { return fTupletsBeginsList; }

    const std::list <S_mxsrTupletEvent>&
                          getTupletsEndsList () const
                              { return fTupletsEndsList; }

    const std::multimap <int, S_mxsrTupletEvent>&
                          getTupletsEventMultiMap () const
                              { return fTupletsEventsMultiMap; }

    // all events
    const std::list <S_mxsrEvent>&
                          getAllEventsList () const
                              { return fAllEventsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  registerStaffChangeTakeOff (
                            int                      noteSequentialNumber,
                            int                      noteEventStaffNumber,
                            int                      noteEventVoiceNumber,
                            int                      takeOffStaffNumber,
                            int                      landingStaffNumber,
                            int                      takeOffInputStartLineNumber,
                            int                      landingInputStartLineNumber,
                            int                      eventInputStartLineNumber,
                            int                      eventInputEndLineNumber);

    void                  registerStaffChangeLanding (
                            int                      noteSequentialNumber,
                            int                      noteEventStaffNumber,
                            int                      noteEventVoiceNumber,
                            int                      takeOffStaffNumber,
                            int                      landingStaffNumber,
                            int                      takeOffInputStartLineNumber,
                            int                      landingInputStartLineNumber,
                            int                      eventInputStartLineNumber,
                            int                      eventInputEndLineNumber);

    void                  registerChordEvent (
                            int                noteSequentialNumber,
                            int                noteEventStaffNumber,
                            int                noteEventVoiceNumber,
                            mxsrChordEventKind chordEventKind,
                            int                eventInputStartLineNumber,
                            int                eventInputEndLineNumber);

    void                  registerTupletBeginEvent (
                            int                 noteSequentialNumber,
                            int                 noteEventStaffNumber,
                            int                 noteEventVoiceNumber,
                            int                 tupletNumber,
                            int                 eventInputStartLineNumber,
                            int                 eventInputEndLineNumber);

    void                  registerTupletEndEvent (
                            int                 noteSequentialNumber,
                            int                 noteEventStaffNumber,
                            int                 noteEventVoiceNumber,
                            int                 tupletNumber,
                            int                 eventInputStartLineNumber,
                            int                 eventInputEndLineNumber);

    void                  sortTheMxsrEventsLists ();

    // staff changes
    S_mxsrStaffChangeEvent
                          fetchStaffChangeTakeOffAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    S_mxsrStaffChangeEvent
                          fetchStaffChangeLandingAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    // chord events
    S_mxsrChordEvent      fetchChordEventAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    // tuplet events
    void                  fetchTupletBeginsList (
                            int                            noteSequentialNumber,
                            std::list <S_mxsrTupletEvent>& collectedBeginsList);

    void                  fetchTupletEndsList (
                            int                            noteSequentialNumber,
                            std::list <S_mxsrTupletEvent>& collectedEndsList);

  private:

    // private services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;
    std::string           asShortString () const;

    void                  print (std::ostream& os) const;

    void                  printAllEvents (std::ostream& os) const;

    void                  printStaffChangeEvents (std::ostream& os) const;
    void                  printChordEvents (std::ostream& os) const;
    void                  printTupletEvents (std::ostream& os) const;

    void                  printTupletEventsList (
                            std::ostream&                        os,
                            const std::list <S_mxsrTupletEvent>& tupletsList,
                            const std::string&                   context,
                            int                                  inputLineNumber) const;

  private:

    // private fields
    // ------------------------------------------------------

    // events sequential numbering
    int                   fCurrentEventSequentialNumber;

    // initial repeat bar
    Bool                  fThereIsAnImplicitInitialForwardRepeat;

    // staff changes events
      // there can be two staff changes per note,
      // hence these two maps, indexed by note sequential number
    std::map <int, S_mxsrStaffChangeEvent>
                          fStaffChangeTakeOffsMap,
                          fStaffChangeLandingsMap;

    std::list <S_mxsrStaffChangeEvent>
                          fStaffChangeEventsList;

    // chords events
      // there can be only one chord begin or end per note,
      // hence this map, indexed by note sequential number
    std::map <int, S_mxsrChordEvent>
                          fChordsEventsMap;

    // tuplet events
      // there can be several tuplets start and/or stop events per note,
      // hence this multimap, indexed by note sequential number,

    std::list <S_mxsrTupletEvent>
                          fTupletsBeginsList,
                          fTupletsEndsList;

    std::multimap <int, S_mxsrTupletEvent>
                          fTupletsEventsMultiMap;

    // all events
    std::list <S_mxsrEvent>
                          fAllEventsList;
};
typedef SMARTP<mxsrEventsCollection> S_mxsrEventsCollection;
EXP std::ostream& operator << (std::ostream& os, const S_mxsrEventsCollection& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrEventsCollection& elt);

}


#endif // ___mxsrEventsCollection___
