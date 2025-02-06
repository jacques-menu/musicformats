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
#include "mfInputLinesRange.h"


using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
/*
  score events are collected in mxsr2msrSkeletonBuilder
  to simplify mxsr2msrSkeletonPopulator

  these events are known to occurr one or more notes too late for most of them,
  such as in chords, tuplets and staff changes

  avoid start and stop in naming, to avoid confusion with MusicXML concepts
  the airplane analogy is to avoid confusion with a *sequence* of notes
  displayed on another staff
*/

//------------------------------------------------------------------------
class EXP mxsrEvent : public smartable
{
/* this class is purely virtual
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrEvent> create (
                            int               eventSequentialNumber,
                            mfInputLineNumber eventInputLineNumber);
*/

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrEvent (
                            int eventSequentialNumber,
                            mfInputLineNumber eventInputLineNumber);

    virtual               ~mxsrEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getEventSequentialNumber () const
                              { return fEventSequentialNumber; }

    mfInputLineNumber     getEventInputLineNumber () const
                              { return fEventInputLineNumber; }

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

    mfInputLineNumber     fEventInputLineNumber;
};
typedef SMARTP<mxsrEvent> S_mxsrEvent;
// EXP std::ostream& operator << (std::ostream& os, const S_mxsrEvent& elt);
// EXP std::ostream& operator << (std::ostream& os, const mxsrEvent& elt);

//________________________________________________________________________
// bool compareEventsByIncreasingSequentialNumber (
//   S_mxsrEvent& first,
//   S_mxsrEvent& second);
//
// bool compareEventsByIncreasingInputLineNumber (
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
class EXP mxsrPartEvent : public mxsrEvent
{
/* this class is purely virtual
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrPartEvent> create (
                            int                eventSequentialNumber,
                            const std::string& partName,
                            const std::string& measureNumber,
                            int                eventInputLineNumber);
*/

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrPartEvent (
                            int                eventSequentialNumber,
                            const std::string& partName,
                            const std::string& measureNumber,
                            int                eventInputLineNumber);

    virtual               ~mxsrPartEvent ();

  public:

    // set and get
    // ------------------------------------------------------

     std::string          getPartName () const
                              { return fPartName; }

     std::string          getMeasureNumber () const
                              { return fMeasureNumber; }

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

    std::string           fPartName;
    std::string           fMeasureNumber;
};
typedef SMARTP<mxsrPartEvent> S_mxsrPartEvent;
// EXP std::ostream& operator << (std::ostream& os, const S_mxsrPartEvent& elt);
// EXP std::ostream& operator << (std::ostream& os, const mxsrPartEvent& elt);

//________________________________________________________________________
enum class mxsrMeasureRepeatEventKind {
  kMeasureRepeatEvent_NONE,
  kMeasureRepeatEventBegin, kMeasureRepeatEventEnd
};

std::string mxsrMeasureRepeatEventKindAsString (
  mxsrMeasureRepeatEventKind measureRepeatEventKind);

std::ostream& operator << (std::ostream& os, const mxsrMeasureRepeatEventKind& elt);

//------------------------------------------------------------------------
class EXP mxsrMeasureRepeatEvent : public mxsrPartEvent
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrMeasureRepeatEvent> create (
                            int                        eventSequentialNumber,
                            const std::string&         partName,
                            const std::string&         measureNumber,
                            mxsrMeasureRepeatEventKind measureRepeatEventKind,
                            int                        repeatNumber,
                            mfInputLineNumber          eventInputLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrMeasureRepeatEvent (
                            int                        eventSequentialNumber,
                            const std::string&         partName,
                            const std::string&         measureNumber,
                            mxsrMeasureRepeatEventKind measureRepeatEventKind,
                            int                        repeatNumber,
                            mfInputLineNumber          eventInputLineNumber);

    virtual               ~mxsrMeasureRepeatEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    mxsrMeasureRepeatEventKind
                          getMeasureRepeatEventKind () const
                              { return fMeasureRepeatEventKind; }

    int                   getRepeatNumber () const
                              { return fRepeatNumber; }

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

  protected:

    // protected fields
    // ------------------------------------------------------

    mxsrMeasureRepeatEventKind
                          fMeasureRepeatEventKind;
    int                   fRepeatNumber;

};
typedef SMARTP<mxsrMeasureRepeatEvent> S_mxsrMeasureRepeatEvent;
// EXP std::ostream& operator << (std::ostream& os, const S_mxsrPartEvent& elt);
// EXP std::ostream& operator << (std::ostream& os, const mxsrPartEvent& elt);

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
                            int noteStaffNumber,
                            int noteVoiceNumber,
                            int eventInputLineNumber);
*/

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrNoteEvent (
                            int eventSequentialNumber,
                            int noteSequentialNumber,
                            int noteStaffNumber,
                            int eventVoiceNumber,
                            int eventInputLineNumber);

    virtual               ~mxsrNoteEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getNoteSequentialNumber () const
                              { return fNoteSequentialNumber; }

    int                   getNoteStaffNumber () const
                              { return fNoteStaffNumber; }

    int                   getNoteVoiceNumber () const
                              { return fNoteVoiceNumber; }

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
    int                   fNoteStaffNumber;
    int                   fNoteVoiceNumber;
};
typedef SMARTP<mxsrNoteEvent> S_mxsrNoteEvent;
// EXP std::ostream& operator << (std::ostream& os, const S_mxsrNoteEvent& elt);
// EXP std::ostream& operator << (std::ostream& os, const mxsrNoteEvent& elt);

//________________________________________________________________________
enum class mxsrStaffChangeEventKind {
  kStaffChangeEvent_NONE,
  kStaffChangeEventTakeOff, kStaffChangeEventLanding
};

std::string mxsrStaffChangeEventKindAsString (
  mxsrStaffChangeEventKind noteStaffChangeEventKind);

std::ostream& operator << (std::ostream& os, const mxsrStaffChangeEventKind& elt);

//------------------------------------------------------------------------
class EXP mxsrStaffChangeEvent : public mxsrNoteEvent
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrStaffChangeEvent> create (
                            int                      eventSequentialNumber,
                            int                      noteSequentialNumber,
                            int                      noteStaffNumber,
                            int                      noteVoiceNumber,
                            mxsrStaffChangeEventKind staffChangeEventKind,
                            int                      takeOffStaffNumber,
                            int                      landingStaffNumber,
                            int                      takeOffInputLineNumber,
                            int                      landingInputLineNumber,
                            int                      eventInputLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrStaffChangeEvent (
                            int                      eventSequentialNumber,
                            int                      noteSequentialNumber,
                            int                      noteStaffNumber,
                            int                      noteVoiceNumber,
                            mxsrStaffChangeEventKind staffChangeEventKind,
                            int                      takeOffStaffNumber,
                            int                      landingStaffNumber,
                            int                      takeOffInputLineNumber,
                            int                      landingInputLineNumber,
                            int                      eventInputLineNumber);

    virtual               ~mxsrStaffChangeEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    mxsrStaffChangeEventKind
                          getStaffChangeEventKind () const
                              { return fStaffChangeEventKind; }

    // the staff change voice number is the one of the note
    // upon which the staff change occurds
    int                   getNoteVoiceNumber () const
                              { return fNoteVoiceNumber; }

    int                   getTakeOffStaffNumber () const
                              { return fTakeOffStaffNumber; }

    int                   getLandingStaffNumber () const
                              { return fLandingStaffNumber; }

    int                   getTakeOffInputLineNumber () const
                              { return fTakeOffInputLineNumber; }

    int                   getLandingInputLineNumber () const
                              { return fLandingInputLineNumber; }

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
    int                   fTakeOffInputLineNumber;

    int                   fLandingStaffNumber;
    int                   fLandingInputLineNumber;
};
typedef SMARTP<mxsrStaffChangeEvent> S_mxsrStaffChangeEvent;
EXP std::ostream& operator << (std::ostream& os, const S_mxsrStaffChangeEvent& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrStaffChangeEvent& elt);

// bool compareStaffChangeEventsByIncreasingInputLineNumber (
//   S_mxsrStaffChangeEvent& first,
//   S_mxsrStaffChangeEvent& second);

//________________________________________________________________________
enum class mxsrGraceEventKind {
  kGraceEvent_NONE,
  kGraceEventBegin, kGraceEventEnd
};

std::string mxsrGraceEventKindAsString (
  mxsrGraceEventKind noteGraceEventKind);

std::ostream& operator << (std::ostream& os, const mxsrGraceEventKind& elt);

//------------------------------------------------------------------------
class EXP mxsrGraceEvent : public mxsrNoteEvent
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrGraceEvent> create (
                            int                eventSequentialNumber,
                            int                noteSequentialNumber,
                            int                noteStaffNumber,
                            int                noteVoiceNumber,
                            mxsrGraceEventKind graceEventKind,
                            int                eventInputLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrGraceEvent (
                            int                eventSequentialNumber,
                            int                noteSequentialNumber,
                            int                noteStaffNumber,
                            int                noteVoiceNumber,
                            mxsrGraceEventKind graceEventKind,
                            int                eventInputLineNumber);

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

//________________________________________________________________________
enum class mxsrCueEventKind {
  kCueEvent_NONE,
  kCueEventBegin, kCueEventEnd
};

std::string mxsrCueEventKindAsString (
  mxsrCueEventKind noteCueEventKind);

std::ostream& operator << (std::ostream& os, const mxsrCueEventKind& elt);

//------------------------------------------------------------------------
class EXP mxsrCueEvent : public mxsrNoteEvent
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrCueEvent> create (
                            int              eventSequentialNumber,
                            int              noteSequentialNumber,
                            int              noteStaffNumber,
                            int              noteVoiceNumber,
                            mxsrCueEventKind cueEventKind,
                            int              eventInputLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrCueEvent (
                            int              eventSequentialNumber,
                            int              noteSequentialNumber,
                            int              noteStaffNumber,
                            int              noteVoiceNumber,
                            mxsrCueEventKind cueEventKind,
                            int              eventInputLineNumber);

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

//________________________________________________________________________
enum class mxsrChordEventKind {
  kChordEvent_NONE,
  kChordEventBegin, kChordEventEnd
};

std::string mxsrChordEventKindAsString (
  mxsrChordEventKind noteChordEventKind);

std::ostream& operator << (std::ostream& os, const mxsrChordEventKind& elt);

//------------------------------------------------------------------------
class EXP mxsrChordEvent : public mxsrNoteEvent
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrChordEvent> create (
                            int                eventSequentialNumber,
                            int                noteSequentialNumber,
                            int                noteStaffNumber,
                            int                noteVoiceNumber,
                            mxsrChordEventKind chordEventKind,
                            int                eventInputLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrChordEvent (
                            int                eventSequentialNumber,
                            int                noteSequentialNumber,
                            int                noteStaffNumber,
                            int                noteVoiceNumber,
                            mxsrChordEventKind chordEventKind,
                            int                eventInputLineNumber);

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

//________________________________________________________________________
enum class mxsrTupletEventKind {
  kTupletEvent_NONE,
  kTupletEventBegin, kTupletEventEnd
};

std::string mxsrTupletEventKindAsString (
  mxsrTupletEventKind noteTupletEventKind);

std::ostream& operator << (std::ostream& os, const mxsrTupletEventKind& elt);

//------------------------------------------------------------------------
class EXP mxsrTupletEvent : public mxsrNoteEvent
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrTupletEvent> create (
                            int                 eventSequentialNumber,
                            int                 noteSequentialNumber,
                            int                 noteStaffNumber,
                            int                 noteVoiceNumber,
                            mxsrTupletEventKind tupletEventKind,
                            int                 tupletNumber,
                            int                 eventInputLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrTupletEvent (
                            int                 eventSequentialNumber,
                            int                 noteSequentialNumber,
                            int                 noteStaffNumber,
                            int                 noteVoiceNumber,
                            mxsrTupletEventKind tupletEventKind,
                            int                 tupletNumber,
                            int                 eventInputLineNumber);

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

    // measure repeat events
    void                  registerMeasureRepeatBegin (
                            const std::string&         partName,
                            const std::string&         measureNumber,
                            int                        repeatNumber,
                            mfInputLineNumber          eventInputLineNumber);

    void                  registerMeasureRepeatEnd (
                            const std::string&         partName,
                            const std::string&         measureNumber,
                            int                        repeatNumber,
                            mfInputLineNumber          eventInputLineNumber);

    S_mxsrMeasureRepeatEvent
                          fetchMeasureRepeatBeginAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    S_mxsrMeasureRepeatEvent
                          fetchMeasureRepeatEndAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    // staff change events
    void                  registerStaffChangeTakeOff (
                            int noteSequentialNumber,
                            int noteStaffNumber,
                            int noteVoiceNumber,
                            int takeOffStaffNumber,
                            int landingStaffNumber,
                            int takeOffInputLineNumber,
                            int landingInputLineNumber,
                            int eventInputLineNumber);

    void                  registerStaffChangeLanding (
                            int noteSequentialNumber,
                            int noteStaffNumber,
                            int noteVoiceNumber,
                            int takeOffStaffNumber,
                            int landingStaffNumber,
                            int takeOffInputLineNumber,
                            int landingInputLineNumber,
                            int eventInputLineNumber);

    S_mxsrStaffChangeEvent
                          fetchStaffChangeLandingAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    S_mxsrStaffChangeEvent
                          fetchStaffChangeTakeOffAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    // grace note events
    void                  registerGraceBegin (
                            int noteSequentialNumber,
                            int noteStaffNumber,
                            int noteVoiceNumber,
                            int eventInputLineNumber);

    void                  registerGraceEnd (
                            int noteSequentialNumber,
                            int noteStaffNumber,
                            int noteVoiceNumber,
                            int eventInputLineNumber);

    S_mxsrGraceEvent      fetchGraceBeginAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    S_mxsrGraceEvent      fetchGraceEndAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    // cue note events
    void                  registerCueBegin (
                            int noteSequentialNumber,
                            int noteStaffNumber,
                            int noteVoiceNumber,
                            int eventInputLineNumber);

    void                  registerCueEnd (
                            int noteSequentialNumber,
                            int noteStaffNumber,
                            int noteVoiceNumber,
                            int eventInputLineNumber);

    S_mxsrCueEvent        fetchCueBeginAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    S_mxsrCueEvent        fetchCueEndAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    // chord events
    void                  registerChordBegin (
                            int noteSequentialNumber,
                            int noteStaffNumber,
                            int noteVoiceNumber,
                            int eventInputLineNumber);

    void                  registerChordEnd (
                            int noteSequentialNumber,
                            int noteStaffNumber,
                            int noteVoiceNumber,
                            int eventInputLineNumber);

    S_mxsrChordEvent      fetchChordBeginAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    S_mxsrChordEvent      fetchChordEndAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    // tuplet events
    void                  registerTupletBeginEvent (
                            int noteSequentialNumber,
                            int noteStaffNumber,
                            int noteVoiceNumber,
                            int tupletNumber,
                            int eventInputLineNumber);

    void                  registerTupletEndEvent (
                            int noteSequentialNumber,
                            int noteStaffNumber,
                            int noteVoiceNumber,
                            int tupletNumber,
                            int eventInputLineNumber);

    void                  fetchTupletBeginsList (
                            int                            noteSequentialNumber,
                            std::list <S_mxsrTupletEvent>& recipientTupletBeginsList);

    void                  fetchTupletEndsList (
                            int                            noteSequentialNumber,
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

    void                  prinMeasureRepeatEvents (std::ostream& os) const;
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

    // measure repeat events
      // there can be only one measure repeat begin and one measure repeat end per measure,
      // hence two maps, indexed by measure number
    std::map <std::string, S_mxsrMeasureRepeatEvent>
                          fMeasureRepeatBeginsMap,
                          fMeasureRepeatEndsMap;

    // staff change events
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

    // chord events
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
