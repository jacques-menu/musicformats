/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsr2msrEvents___
#define ___mxsr2msrEvents___

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
enum class mxsrGraceEventKind {
  kEventGrace_NONE,
  kEventGraceBegin, kEventGraceEnd
};

std::string mxsrGraceEventKindAsString (
  mxsrGraceEventKind noteGraceEventKind);

std::ostream& operator << (std::ostream& os, const mxsrGraceEventKind& elt);

//________________________________________________________________________
enum class mxsrCueEventKind {
  kEventCue_NONE,
  kEventCueBegin, kEventCueEnd
};

std::string mxsrCueEventKindAsString (
  mxsrCueEventKind noteCueEventKind);

std::ostream& operator << (std::ostream& os, const mxsrCueEventKind& elt);

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
                            int eventInputEndLineNumber,
                            int eventNoteSequentialNumber);
*/

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrEvent (
                            int eventSequentialNumber,
                            int eventInputStartLineNumber,
                            int eventInputEndLineNumber,
                            int eventNoteSequentialNumber);

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

    int                   getEventNoteSequentialNumber () const
                              { return fEventNoteSequentialNumber; }

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

    int                   fEventNoteSequentialNumber;
};
typedef SMARTP<mxsrEvent> S_mxsrEvent;
// EXP std::ostream& operator << (std::ostream& os, const S_mxsrEvent& elt);
// EXP std::ostream& operator << (std::ostream& os, const mxsrEvent& elt);

//________________________________________________________________________
// bool compareEventsByIncreasingSequentialNumber (
//   S_mxsrEvent& first,
//   S_mxsrEvent& second);
//
// bool compareEventsByIncreasingInputStartLineNumber (
//   const S_mxsrEvent& first,
//   const S_mxsrEvent& second);
//
// bool operator< (
//   const S_mxsrEvent& first,
//   const S_mxsrEvent& second);

/*
Coming on to what you want to achieve, I see two ways of doing so:

  std::map<int,int,compareByVal> hash;

  1:
    Copy all the elements of the map to a std::vector and sort that:

    std::vector<std::pair<int,int> > v(hash.begin(), hash.end());

    std::sort(v.begin(), v.end(), [](const auto& a, const auto& b)
      { return a.second < b.second; });

  2:
    Copy all the elements of the map to another map with keys as values and values as keys.
    If values of your map are not unique, you can use a std::multimap instead.
*/


//------------------------------------------------------------------------
class EXP mxsrNoteEvent : public mxsrEvent
{
/* this class is purely virtual
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrNoteEvent> create (
                            int eventSequentialNumber,
                            int eventNoteSequentialNumber,
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
                            int eventNoteSequentialNumber,
                            int noteEventStaffNumber,
                            int noteEventVoiceNumber,
                            int eventInputStartLineNumber,
                            int eventInputEndLineNumber);

    virtual               ~mxsrNoteEvent ();

  public:

    // set and get
    // ------------------------------------------------------

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
                            int                      eventNoteSequentialNumber,
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
                            int                      eventNoteSequentialNumber,
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

// bool compareStaffChangeEventsByIncreasingInputStartLineNumber (
//   S_mxsrStaffChangeEvent& first,
//   S_mxsrStaffChangeEvent& second);

//------------------------------------------------------------------------
class EXP mxsrGraceEvent : public mxsrNoteEvent
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrGraceEvent> create (
                            int                eventSequentialNumber,
                            int                eventNoteSequentialNumber,
                            int                noteEventStaffNumber,
                            int                noteEventVoiceNumber,
                            mxsrGraceEventKind graceEventKind,
                            int                eventInputStartLineNumber,
                            int                eventInputEndLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrGraceEvent (
                            int                eventSequentialNumber,
                            int                eventNoteSequentialNumber,
                            int                noteEventStaffNumber,
                            int                noteEventVoiceNumber,
                            mxsrGraceEventKind graceEventKind,
                            int                eventInputStartLineNumber,
                            int                eventInputEndLineNumber);

    virtual               ~mxsrGraceEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    mxsrGraceEventKind
                          getGraceEventKind () const
                              { return fGraceEventKind; }

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

    mxsrGraceEventKind    fGraceEventKind;
};
typedef SMARTP<mxsrGraceEvent> S_mxsrGraceEvent;
EXP std::ostream& operator << (std::ostream& os, const S_mxsrGraceEvent& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrGraceEvent& elt);

//------------------------------------------------------------------------
class EXP mxsrCueEvent : public mxsrNoteEvent
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrCueEvent> create (
                            int              eventSequentialNumber,
                            int              eventNoteSequentialNumber,
                            int              noteEventStaffNumber,
                            int              noteEventVoiceNumber,
                            mxsrCueEventKind cueEventKind,
                            int              eventInputStartLineNumber,
                            int              eventInputEndLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrCueEvent (
                            int              eventSequentialNumber,
                            int              eventNoteSequentialNumber,
                            int              noteEventStaffNumber,
                            int              noteEventVoiceNumber,
                            mxsrCueEventKind cueEventKind,
                            int              eventInputStartLineNumber,
                            int              eventInputEndLineNumber);

    virtual               ~mxsrCueEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    mxsrCueEventKind
                          getCueEventKind () const
                              { return fCueEventKind; }

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

    mxsrCueEventKind  fCueEventKind;
};
typedef SMARTP<mxsrCueEvent> S_mxsrCueEvent;
EXP std::ostream& operator << (std::ostream& os, const S_mxsrCueEvent& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrCueEvent& elt);

//------------------------------------------------------------------------
class EXP mxsrChordEvent : public mxsrNoteEvent
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrChordEvent> create (
                            int                eventSequentialNumber,
                            int                eventNoteSequentialNumber,
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
                            int                eventNoteSequentialNumber,
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
                            int                 eventNoteSequentialNumber,
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
                            int                 eventNoteSequentialNumber,
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

    // grace events
    const std::map <int, S_mxsrGraceEvent>&
                          getGraceBeginsMap () const
                              { return fGraceBeginsMap; }

    const std::map <int, S_mxsrGraceEvent>&
                          getGraceEndsMap () const
                              { return fGraceEndsMap; }

    // cue events
    const std::map <int, S_mxsrCueEvent>&
                          getCueBeginsMap () const
                              { return fCueBeginsMap; }

    const std::map <int, S_mxsrCueEvent>&
                          getCueEndsMap () const
                              { return fCueEndsMap; }

    // chord events
    const std::map <int, S_mxsrChordEvent>&
                          getChordBeginsMap () const
                              { return fChordBeginsMap; }

    const std::map <int, S_mxsrChordEvent>&
                          getChordEndsMap () const
                              { return fChordEndsMap; }

    // tuplet events
    const std::list <S_mxsrTupletEvent>&
                          getTupletBeginsList () const
                              { return fTupletBeginsList; }

    const std::list <S_mxsrTupletEvent>&
                          getTupletEndsList () const
                              { return fTupletEndsList; }

    const std::multimap <int, S_mxsrTupletEvent>&
                          getTupletEventMultiMap () const
                              { return fTupletEventsMultiMap; }

    // all events
    const std::list <S_mxsrEvent>&
                          getAllEventsList () const
                              { return fAllEventsList; }

  public:

    // public services
    // ------------------------------------------------------

    // staff changes
    void                  registerStaffChangeTakeOff (
                            int eventNoteSequentialNumber,
                            int noteEventStaffNumber,
                            int noteEventVoiceNumber,
                            int takeOffStaffNumber,
                            int landingStaffNumber,
                            int takeOffInputStartLineNumber,
                            int landingInputStartLineNumber,
                            int eventInputStartLineNumber,
                            int eventInputEndLineNumber);

    void                  registerStaffChangeLanding (
                            int eventNoteSequentialNumber,
                            int noteEventStaffNumber,
                            int noteEventVoiceNumber,
                            int takeOffStaffNumber,
                            int landingStaffNumber,
                            int takeOffInputStartLineNumber,
                            int landingInputStartLineNumber,
                            int eventInputStartLineNumber,
                            int eventInputEndLineNumber);

    S_mxsrStaffChangeEvent
                          fetchStaffChangeLandingAtNoteSequentialNumber (
                            int eventNoteSequentialNumber) const;

    S_mxsrStaffChangeEvent
                          fetchStaffChangeTakeOffAtNoteSequentialNumber (
                            int eventNoteSequentialNumber) const;

    // grace note events
    void                  registerGraceBegin (
                            int eventNoteSequentialNumber,
                            int noteEventStaffNumber,
                            int noteEventVoiceNumber,
                            int eventInputStartLineNumber,
                            int eventInputEndLineNumber);

    void                  registerGraceEnd (
                            int eventNoteSequentialNumber,
                            int noteEventStaffNumber,
                            int noteEventVoiceNumber,
                            int eventInputStartLineNumber,
                            int eventInputEndLineNumber);

    S_mxsrGraceEvent      fetchGraceBeginAtNoteSequentialNumber (
                            int eventNoteSequentialNumber) const;

    S_mxsrGraceEvent      fetchGraceEndAtNoteSequentialNumber (
                            int eventNoteSequentialNumber) const;

    // cue note events
    void                  registerCueBegin (
                            int eventNoteSequentialNumber,
                            int noteEventStaffNumber,
                            int noteEventVoiceNumber,
                            int eventInputStartLineNumber,
                            int eventInputEndLineNumber);

    void                  registerCueEnd (
                            int eventNoteSequentialNumber,
                            int noteEventStaffNumber,
                            int noteEventVoiceNumber,
                            int eventInputStartLineNumber,
                            int eventInputEndLineNumber);

    S_mxsrCueEvent    fetchCueBeginAtNoteSequentialNumber (
                            int eventNoteSequentialNumber) const;

    S_mxsrCueEvent    fetchCueEndAtNoteSequentialNumber (
                            int eventNoteSequentialNumber) const;

    // chord events
    void                  registerChordBegin (
                            int eventNoteSequentialNumber,
                            int noteEventStaffNumber,
                            int noteEventVoiceNumber,
                            int eventInputStartLineNumber,
                            int eventInputEndLineNumber);

    void                  registerChordEnd (
                            int eventNoteSequentialNumber,
                            int noteEventStaffNumber,
                            int noteEventVoiceNumber,
                            int eventInputStartLineNumber,
                            int eventInputEndLineNumber);

    S_mxsrChordEvent      fetchChordBeginAtNoteSequentialNumber (
                            int eventNoteSequentialNumber) const;

    S_mxsrChordEvent      fetchChordEndAtNoteSequentialNumber (
                            int eventNoteSequentialNumber) const;

    // tuplet events
    void                  registerTupletBeginEvent (
                            int eventNoteSequentialNumber,
                            int noteEventStaffNumber,
                            int noteEventVoiceNumber,
                            int tupletNumber,
                            int eventInputStartLineNumber,
                            int eventInputEndLineNumber);

    void                  registerTupletEndEvent (
                            int eventNoteSequentialNumber,
                            int noteEventStaffNumber,
                            int noteEventVoiceNumber,
                            int tupletNumber,
                            int eventInputStartLineNumber,
                            int eventInputEndLineNumber);

    void                  fetchTupletBeginsList (
                            int                            eventNoteSequentialNumber,
                            std::list <S_mxsrTupletEvent>& recipientTupletBeginsList);

    void                  fetchTupletEndsList (
                            int                            eventNoteSequentialNumber,
                            std::list <S_mxsrTupletEvent>& recipientTupletEndsList);

    // sort the MXSR events lists
    void                  sortTheMxsrEventsLists ();

  private:

    // private services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;
    std::string           asShortString () const;

    void                  print (std::ostream& os) const;

    void                  printAllEventsList (std::ostream& os) const;

    void                  printStaffChangeEvents (std::ostream& os) const;
    void                  printGraceEvents (std::ostream& os) const;
    void                  printCueEvents (std::ostream& os) const;
    void                  printChordEvents (std::ostream& os) const;
    void                  printTupletEvents (std::ostream& os) const;

    void                  printTupletEventsList (
                            std::ostream&                        os,
                            const std::list <S_mxsrTupletEvent>& tupletEventsList,
                            const std::string&                   context) const;

  private:

    // private fields
    // ------------------------------------------------------

    // events sequential numbering
    int                   fCurrentEventSequentialNumber;

    // initial repeat bar
    Bool                  fThereIsAnImplicitInitialForwardRepeat;

    // all events
    std::list <S_mxsrEvent>
                          fAllEventsList;

    // staff changes events
      // there can be two staff changes per note,
      // hence these two maps, indexed by note sequential number
    std::map <int, S_mxsrStaffChangeEvent>
                          fStaffChangeTakeOffsMap,
                          fStaffChangeLandingsMap;

    std::list <S_mxsrStaffChangeEvent>
                          fStaffChangeEventsList;

    // grace notes events
      // there can be only one grace note begin and one chord end per note,
      // hence two maps, indexed by note sequential number
    std::map <int, S_mxsrGraceEvent>
                          fGraceBeginsMap,
                          fGraceEndsMap;

    // cue notes events
      // there can be only one cue note begin and one chord end per note,
      // hence two maps, indexed by note sequential number
    std::map <int, S_mxsrCueEvent>
                          fCueBeginsMap,
                          fCueEndsMap;

    // chords events
      // there can be only one chord begin and one chord end per note,
      // hence two maps, indexed by note sequential number
    std::map <int, S_mxsrChordEvent>
                          fChordBeginsMap,
                          fChordEndsMap;

    // tuplet events
      // there can be several tuplets start and/or stop events per note,
      // hence two lists and a multimap, indexed by note sequential number,

    std::list <S_mxsrTupletEvent>
                          fTupletBeginsList,
                          fTupletEndsList;

    std::multimap <int, S_mxsrTupletEvent>
                          fTupletEventsMultiMap;
};
typedef SMARTP<mxsrEventsCollection> S_mxsrEventsCollection;
EXP std::ostream& operator << (std::ostream& os, const S_mxsrEventsCollection& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrEventsCollection& elt);

}


#endif // ___mxsr2msrEvents___
