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

#include "exports.h"
#include "smartpointer.h"

#include "mfBasicTypes.h"
#include "mfBool.h"
#include "mfIndentedTextOutput.h"
#include "mfWrappedValueForArithmetic.h"

#include "mxsrBasicTypes.h"

#include "msrTupletFactors.h"


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

    static SMARTP<mxsrEvent>
                          create (
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);
*/

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrEvent (
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

    virtual               ~mxsrEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    const mxsrEventSequentialNumber&
                          getEventSequentialNumber () const
                              { return fEventSequentialNumber; }

    const mfInputLineNumber&
                          getEventInputLineNumber () const
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
    // since the end of lines are not mandatory in MusicXML data, hence:
    mxsrEventSequentialNumber
                          fEventSequentialNumber;

    mfInputLineNumber     fEventInputLineNumber;
};

using S_mxsrEvent = SMARTP<mxsrEvent>;

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

    static SMARTP<mxsrPartEvent>
                          create (
                            const std::string&       partName,
                            const mfMeasureNumber&   measureNumber,
                            const mxsrNoteSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber& eventInputLineNumber);
*/

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrPartEvent (
                            const std::string&       partName,
                            const mfMeasureNumber&   measureNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber& eventInputLineNumber);

    virtual               ~mxsrPartEvent ();

  public:

    // set and get
    // ------------------------------------------------------

     const std::string&   getPartName () const
                              { return fPartName; }

     const mfMeasureNumber&
                          getMeasureNumber () const
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
    mfMeasureNumber       fMeasureNumber;
};

using S_mxsrPartEvent = SMARTP<mxsrPartEvent>;

// EXP std::ostream& operator << (std::ostream& os, const S_mxsrPartEvent& elt);
// EXP std::ostream& operator << (std::ostream& os, const mxsrPartEvent& elt);

//________________________________________________________________________
enum class mxsrMultipleMeasureRestEventKind {
  kMultipleMeasureRestEvent_NONE,
  kMultipleMeasureRestEventBegin, kMultipleMeasureRestEventEnd
};

std::string mxsrMultipleMeasureRestEventKindAsString (
  mxsrMultipleMeasureRestEventKind multipleMeasureRestEventKind);

std::ostream& operator << (std::ostream& os, const mxsrMultipleMeasureRestEventKind& elt);

//------------------------------------------------------------------------
class EXP mxsrMultipleMeasureRestEvent : public mxsrPartEvent
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrMultipleMeasureRestEvent>
                          create (
                            mxsrMultipleMeasureRestEventKind multipleMeasureRestEventKind,
                            const std::string&               partName,
                            const mfMeasureNumber&           measureNumber,
                            int                              multipleMeasureRestNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

//     static SMARTP<mxsrMultipleMeasureRestEvent>
//                           createAMultipleMeasureRestEnd (
//                             const std::string&       partName,
//                             const mfMeasureNumber&   measureNumber,
//                             int                      multipleMeasureRestNumber,
//                             const mfInputLineNumber& eventInputLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrMultipleMeasureRestEvent (
                            mxsrMultipleMeasureRestEventKind multipleMeasureRestEventKind,
                            const std::string&               partName,
                            const mfMeasureNumber&           measureNumber,
                            int                              multipleMeasureRestNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

    virtual               ~mxsrMultipleMeasureRestEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    mxsrMultipleMeasureRestEventKind
                          getMultipleMeasureRestEventKind () const
                              { return fMultipleMeasureRestEventKind; }

    int                   getMultipleMeasureRestNumber () const
                              { return fMultipleMeasureRestNumber; }

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

    mxsrMultipleMeasureRestEventKind
                          fMultipleMeasureRestEventKind;

    int                   fMultipleMeasureRestNumber;

};

using S_mxsrMultipleMeasureRestEvent = SMARTP<mxsrMultipleMeasureRestEvent>;

EXP std::ostream& operator << (std::ostream& os, const mxsrMultipleMeasureRestEvent& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrMultipleMeasureRestEvent& elt);

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

    static SMARTP<mxsrMeasureRepeatEvent>
                          create (
                            mxsrMeasureRepeatEventKind measureRepeatEventKind,
                            const std::string&         partName,
                            const mfMeasureNumber&     measureNumber,
                            int                        measureRepeatNumber,
                            int                        measureRepeatSlashes,
                            const mxsrEventSequentialNumber&                        eventSequentialNumber,
                            const mfInputLineNumber&          eventInputLineNumber);

//     static SMARTP<mxsrMeasureRepeatEvent>
//                           createAMeasureRepeatEnd (
//                             const std::string&       partName,
//                             const mfMeasureNumber&   measureNumber,
//                             int                      measureRepeatNumber,
//                             int                      measureRepeatSlashes,
//                             const mfInputLineNumber& eventInputLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrMeasureRepeatEvent (
                            mxsrMeasureRepeatEventKind measureRepeatEventKind,
                            const std::string&         partName,
                            const mfMeasureNumber&     measureNumber,
                            int                        measureRepeatNumber,
                            int                        measureRepeatSlashes,
                            const mxsrEventSequentialNumber&                        eventSequentialNumber,
                            const mfInputLineNumber&          eventInputLineNumber);

    virtual               ~mxsrMeasureRepeatEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    mxsrMeasureRepeatEventKind
                          getMeasureRepeatEventKind () const
                              { return fMeasureRepeatEventKind; }

    int                   getMeasureRepeatSlashes () const
                              { return fMeasureRepeatSlashes; }

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

    int                   fMeasureRepeatSlashes;
};

using S_mxsrMeasureRepeatEvent = SMARTP<mxsrMeasureRepeatEvent>;

EXP std::ostream& operator << (std::ostream& os, const S_mxsrPartEvent& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrPartEvent& elt);

//------------------------------------------------------------------------
class EXP mxsrNoteEvent : public mxsrEvent
{
/* this class is purely virtual
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrNoteEvent>
                          create (
                            const mxsrNoteSequentialNumber&  noteSequentialNumber,
                            const mfStaffNumber&             noteStaffNumber,
                            const mfVoiceNumber&             noteVoiceNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);
*/

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrNoteEvent (
                            const mxsrNoteSequentialNumber&  noteSequentialNumber,
                            const mfStaffNumber&             noteStaffNumber,
                            const mfVoiceNumber&             noteVoiceNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

    virtual               ~mxsrNoteEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    const mxsrNoteSequentialNumber&
                         getNoteSequentialNumber () const
                              { return fNoteSequentialNumber; }

    const mfStaffNumber&  getNoteStaffNumber () const
                              { return fNoteStaffNumber; }

    const mfVoiceNumber&  getNoteVoiceNumber () const
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

    mxsrNoteSequentialNumber
                          fNoteSequentialNumber;

    // the staff and voice numbers of the note upon which the event occurs
    mfStaffNumber         fNoteStaffNumber;
    mfVoiceNumber         fNoteVoiceNumber;
};

using S_mxsrNoteEvent = SMARTP<mxsrNoteEvent>;

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

    static SMARTP<mxsrStaffChangeEvent>
                          create (
                            mxsrStaffChangeEventKind         staffChangeEventKind,
                            const mxsrNoteSequentialNumber & noteSequentialNumber,
                            const mfStaffNumber&             noteStaffNumber,
                            const mfVoiceNumber&             noteVoiceNumber,
                            const mfStaffNumber&             takeOffStaffNumber,
                            const mfInputLineNumber&         takeOffInputLineNumber, // superflous ???
                            const mfStaffNumber&             landingStaffNumber,
                            const mfInputLineNumber&         landingInputLineNumber, // superflous ???
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrStaffChangeEvent (
                            mxsrStaffChangeEventKind         staffChangeEventKind,
                            const mxsrNoteSequentialNumber & noteSequentialNumber,
                            const mfStaffNumber&             noteStaffNumber,
                            const mfVoiceNumber&             noteVoiceNumber,
                            const mfStaffNumber&             takeOffStaffNumber,
                            const mfInputLineNumber&         takeOffInputLineNumber, // superflous ???
                            const mfStaffNumber&             landingStaffNumber,
                            const mfInputLineNumber&         landingInputLineNumber, // superflous ???
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

    virtual               ~mxsrStaffChangeEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    mxsrStaffChangeEventKind
                          getStaffChangeEventKind () const
                              { return fStaffChangeEventKind; }

    // the staff change voice number is the one of the note
    // upon which the staff change occurds
    const mfVoiceNumber&         getNoteVoiceNumber () const
                              { return fNoteVoiceNumber; }

    const mfStaffNumber&  getTakeOffStaffNumber () const
                              { return fTakeOffStaffNumber; }

    const mfInputLineNumber&
                          getTakeOffInputLineNumber () const
                              { return fTakeOffInputLineNumber; }

    const mfStaffNumber&  getLandingStaffNumber () const
                              { return fLandingStaffNumber; }

    const mfInputLineNumber&
                          getLandingInputLineNumber () const
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
    mfStaffNumber         fTakeOffStaffNumber;
    mfInputLineNumber     fTakeOffInputLineNumber;

    mfStaffNumber         fLandingStaffNumber;
    mfInputLineNumber     fLandingInputLineNumber;
};

using S_mxsrStaffChangeEvent = SMARTP<mxsrStaffChangeEvent>;

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

    static SMARTP<mxsrGraceEvent>
                          create (
                            mxsrGraceEventKind               graceEventKind,
                            const mxsrNoteSequentialNumber&  noteSequentialNumber,
                            const mfStaffNumber&             noteStaffNumber,
                            const mfVoiceNumber&             noteVoiceNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrGraceEvent (
                            mxsrGraceEventKind               graceEventKind,
                            const mxsrNoteSequentialNumber&  noteSequentialNumber,
                            const mfStaffNumber&             noteStaffNumber,
                            const mfVoiceNumber&             noteVoiceNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

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

using S_mxsrGraceEvent = SMARTP<mxsrGraceEvent>;

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

    static SMARTP<mxsrCueEvent>
                          create (
                            mxsrCueEventKind                 cueEventKind,
                            const mxsrNoteSequentialNumber&  noteSequentialNumber,
                            const mfStaffNumber&             noteStaffNumber,
                            const mfVoiceNumber&             noteVoiceNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrCueEvent (
                            mxsrCueEventKind                 cueEventKind,
                            const mxsrNoteSequentialNumber&  noteSequentialNumber,
                            const mfStaffNumber&             noteStaffNumber,
                            const mfVoiceNumber&             noteVoiceNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

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

    mxsrCueEventKind      fCueEventKind;
};

using S_mxsrCueEvent = SMARTP<mxsrCueEvent>;

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

    static SMARTP<mxsrChordEvent>
                          create (
                            mxsrChordEventKind               chordEventKind,
                            const mxsrNoteSequentialNumber&  noteSequentialNumber,
                            const mfStaffNumber&             noteStaffNumber,
                            const mfVoiceNumber&             noteVoiceNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrChordEvent (
                            mxsrChordEventKind               chordEventKind,
                            const mxsrNoteSequentialNumber&  noteSequentialNumber,
                            const mfStaffNumber&             noteStaffNumber,
                            const mfVoiceNumber&             noteVoiceNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

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

using S_mxsrChordEvent = SMARTP<mxsrChordEvent>;

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

    static SMARTP<mxsrTupletEvent>
                          create (
                            mxsrTupletEventKind              tupletEventKind,
                            const mxsrTupletNumber&          tupletNumber,
                            const msrTupletFactor&           tupletFactor,
                            const mxsrNoteSequentialNumber&  noteSequentialNumber,
                            const mfStaffNumber&             noteStaffNumber,
                            const mfVoiceNumber&             noteVoiceNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrTupletEvent (
                            mxsrTupletEventKind              tupletEventKind,
                            const mxsrTupletNumber&          tupletNumber,
                            const msrTupletFactor&           tupletFactor,
                            const mxsrNoteSequentialNumber&  noteSequentialNumber,
                            const mfStaffNumber&             noteStaffNumber,
                            const mfVoiceNumber&             noteVoiceNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

    virtual               ~mxsrTupletEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    mxsrTupletEventKind   getTupletEventKind () const
                              { return fTupletEventKind; }

    const mxsrTupletNumber&
                          getTupletNumber () const
                              { return fTupletNumber; }

    const msrTupletFactor&
                          getTupletFactor () const
                              { return fTupletFactor; }

  public:

    // public services
    // ------------------------------------------------------

    Bool                  tupletEventIsForTupletNumber (int tupletNumber)
                              { return fTupletNumber == tupletNumber; }


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

    mxsrTupletNumber      fTupletNumber;
    msrTupletFactor       fTupletFactor;
};

using S_mxsrTupletEvent = SMARTP<mxsrTupletEvent>;

EXP std::ostream& operator << (std::ostream& os, const S_mxsrTupletEvent& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrTupletEvent& elt);

//------------------------------------------------------------------------
class EXP mxsrEventsCollection : public smartable
// class mxsrEventsCollection : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrEventsCollection>
                          create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrEventsCollection ();

    virtual               ~mxsrEventsCollection ();

  public:

    // set and get
    // ------------------------------------------------------

    const mxsrEventSequentialNumber&
                          getCurrentEventSequentialNumber () const
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
    const std::map <mxsrNoteSequentialNumber, S_mxsrGraceEvent>&
                          getGraceBeginsMap () const
                              { return fGraceBeginsMap; }

    const std::map <mxsrNoteSequentialNumber, S_mxsrGraceEvent>&
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
    const std::multimap <int, S_mxsrTupletEvent>&
                          getTupletBeginsMultiMap () const
                              { return fTupletBeginsMultiMap; }

    const std::multimap <int, S_mxsrTupletEvent>&
                          getTupletEndsMultiMap () const
                              { return fTupletEndsMultiMap; }

    // all events
    const std::list <S_mxsrEvent>&
                          getAllEventsList () const
                              { return fAllEventsList; }

  public:

    // public services
    // ------------------------------------------------------

    // measure measure rest events
    // ------------------------------------------------------

    S_mxsrMultipleMeasureRestEvent
                          createAMultipleMeasureRestBegin (
                            const std::string&       partName,
                            const mfMeasureNumber&   measureNumber,
                            int                      multipleMeasureRestNumber,
                            const mfInputLineNumber& eventInputLineNumber);

    void                  registerMultipleMeasureRestBegin (
                            S_mxsrMultipleMeasureRestEvent multipleMeasureRestBeginEvent);

//     S_mxsrMultipleMeasureRestEvent
//                           createAMultipleMeasureRestEnd (
//                             const std::string&       partName,
//                             const mfMeasureNumber&   measureNumber,
//                             int                      multipleMeasureRestNumber,
//                             const mfInputLineNumber& eventInputLineNumber);
//
//     void                  registerMultipleMeasureRestEnd (
//                             S_mxsrMultipleMeasureRestEvent multipleMeasureRestEndEvent);

    void                  createAMultipleMeasureRestEndAndRegisterIt (
                            const std::string&       partName,
                            const mfMeasureNumber&   measureNumber,
                            int                      multipleMeasureRestNumber,
                            const mfInputLineNumber& eventInputLineNumber);

    S_mxsrMultipleMeasureRestEvent
                          fetchMultipleMeasureRestBeginAtBareMeasureNumber (
                            const std::string& bareMeasureNumber) const;

    S_mxsrMultipleMeasureRestEvent
                          fetchMultipleMeasureRestEndAtBareMeasureNumber (
                            const std::string& bareMeasureNumber) const;

    // measure repeat events
    // ------------------------------------------------------

    S_mxsrMeasureRepeatEvent
                          createAMeasureRepeatBegin (
                            const std::string&       partName,
                            const mfMeasureNumber&   measureNumber,
                            int                      measureRepeatNumber,
                            int                      measureRepeatSlashes,
                            const mfInputLineNumber& eventInputLineNumber);

    void                  registerMeasureRepeatBegin (
                            S_mxsrMeasureRepeatEvent measureRepeatBeginEvent);

//     S_mxsrMeasureRepeatEvent
//                           createAMeasureRepeatEnd (
//                             const std::string&       partName,
//                             const mfMeasureNumber&   measureNumber,
//                             int                      measureRepeatNumber,
//                             int                      measureRepeatSlashes,
//                             const mfInputLineNumber& eventInputLineNumber);
//
//     void                  registerMeasureRepeatEnd (
//                             S_mxsrMeasureRepeatEvent measureRepeatEvent);

    void                  createAMeasureRepeatEndAndRegisterIt (
                            const std::string&       partName,
                            const mfMeasureNumber&   measureNumber,
                            int                      measureRepeatNumber,
                            int                      measureRepeatSlashes,
                            const mfInputLineNumber& eventInputLineNumber);

    S_mxsrMeasureRepeatEvent
                          fetchMeasureRepeatBeginAtBareMeasureNumber (
                            const std::string& bareMeasureNumber) const;

    S_mxsrMeasureRepeatEvent
                          fetchMeasureRepeatEndAtBareMeasureNumber (
                            const std::string& bareMeasureNumber) const;

    // staff change events
    // ------------------------------------------------------

    void                  registerStaffChangeTakeOff (
                            const mxsrNoteSequentialNumber& noteSequentialNumber,
                            const mfStaffNumber&            noteStaffNumber,
                            const mfVoiceNumber&            noteVoiceNumber,
                            const mfStaffNumber&            takeOffStaffNumber,
                            const mfInputLineNumber&        takeOffInputLineNumber,
                            const mfStaffNumber&            landingStaffNumber,
                            const mfInputLineNumber&        landingInputLineNumber,
                            const mfInputLineNumber&        eventInputLineNumber);

    void                  registerStaffChangeLanding (
                            const mxsrNoteSequentialNumber& noteSequentialNumber,
                            const mfStaffNumber&            noteStaffNumber,
                            const mfVoiceNumber&            noteVoiceNumber,
                            const mfStaffNumber&            takeOffStaffNumber,
                            const mfInputLineNumber&        takeOffInputLineNumber,
                            const mfStaffNumber&            landingStaffNumber,
                            const mfInputLineNumber&        landingInputLineNumber,
                            const mfInputLineNumber&        eventInputLineNumber);

    S_mxsrStaffChangeEvent
                          fetchStaffChangeLandingAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    S_mxsrStaffChangeEvent
                          fetchStaffChangeTakeOffAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    // grace note events
    // ------------------------------------------------------

    void                  registerGraceBegin (
                            const mxsrNoteSequentialNumber& noteSequentialNumber,
                            const mfStaffNumber&            noteStaffNumber,
                            const mfVoiceNumber&            noteVoiceNumber,
                            const mfInputLineNumber&        eventInputLineNumber);

    void                  registerGraceEnd (
                            const mxsrNoteSequentialNumber& noteSequentialNumber,
                            const mfStaffNumber&            noteStaffNumber,
                            const mfVoiceNumber&            noteVoiceNumber,
                            const mfInputLineNumber&        eventInputLineNumber);

    S_mxsrGraceEvent      fetchGraceBeginAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    S_mxsrGraceEvent      fetchGraceEndAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    // cue note events
    // ------------------------------------------------------

    S_mxsrCueEvent        createACueBeginEvent (
                            const mxsrNoteSequentialNumber& noteSequentialNumber,
                            const mfStaffNumber&            noteStaffNumber,
                            const mfVoiceNumber&            noteVoiceNumber,
                            const mfInputLineNumber&        eventInputLineNumber);

    void                  registerCueBeginEvent (
                            S_mxsrCueEvent cueBeginEvent);

    S_mxsrCueEvent        createACueEndEvent (
                            const mxsrNoteSequentialNumber& noteSequentialNumber,
                            const mfStaffNumber&            noteStaffNumber,
                            const mfVoiceNumber&            noteVoiceNumber,
                            const mfInputLineNumber&        eventInputLineNumber);

    void                  registerCueEndEvent (
                            S_mxsrCueEvent cueEndEvent);

    S_mxsrCueEvent        fetchCueBeginAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    S_mxsrCueEvent        fetchCueEndAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    // chord events
    // ------------------------------------------------------

    S_mxsrChordEvent      createAChordBeginEvent (
                            const mxsrNoteSequentialNumber& noteSequentialNumber,
                            const mfStaffNumber&            noteStaffNumber,
                            const mfVoiceNumber&            noteVoiceNumber,
                            const mfInputLineNumber&        eventInputLineNumber);

    void                  registerChordBeginEvent (
                            const S_mxsrChordEvent& chordBeginEvent);

    S_mxsrChordEvent      createAChordEndEvent (
                            const mxsrNoteSequentialNumber& noteSequentialNumber,
                            const mfStaffNumber&            noteStaffNumber,
                            const mfVoiceNumber&            noteVoiceNumber,
                            const mfInputLineNumber&        eventInputLineNumber);

    void                  registerChordEndEvent (
                            const S_mxsrChordEvent& chordEndEvent);


    S_mxsrChordEvent      fetchChordBeginAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    S_mxsrChordEvent      fetchChordEndAtNoteSequentialNumber (
                            int noteSequentialNumber) const;

    // tuplet events
    // ------------------------------------------------------

    S_mxsrTupletEvent     createATupletBeginEvent (
                            const mxsrTupletNumber&         tupletNumber,
                            const msrTupletFactor&          tupletFactor,
                            const mxsrNoteSequentialNumber& noteSequentialNumber,
                            const mfStaffNumber&            noteStaffNumber,
                            const mfVoiceNumber&            noteVoiceNumber,
                            const mfInputLineNumber&        eventInputLineNumber);

    void                  registerTupletBeginEvent (
                            S_mxsrTupletEvent tupletBeginEvent);

    S_mxsrTupletEvent     createATupletEndEvent (
                            const mxsrTupletNumber&         tupletNumber,
                            const msrTupletFactor&          tupletFactor,
                            const mxsrNoteSequentialNumber& noteSequentialNumber,
                            const mfStaffNumber&            noteStaffNumber,
                            const mfVoiceNumber&            noteVoiceNumber,
                            const mfInputLineNumber&        eventInputLineNumber);

    void                  registerTupletEndEvent (
                            S_mxsrTupletEvent tupletEndEvent);

    S_mxsrTupletEvent     fetchTupletBeginForTupletNumber (
                            int tupletBareNumber);

    void                  fetchTupletBeginsAtNoteSequentialNumber (
                            int                            noteSequentialNumber,
                            std::list <S_mxsrTupletEvent>& recipientTupletBeginsList);

    void                  fetchTupletEndsListAtNoteSequentialNumber (
                            int                            noteSequentialNumber,
                            std::list <S_mxsrTupletEvent>& recipientTupletEndsList);

    // sort the MXSR events lists
    // ------------------------------------------------------

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

    void                  printMultipleMeasureRestEvents (std::ostream& os) const;

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
    // ------------------------------------------------------

    mxsrEventSequentialNumber
                          fCurrentEventSequentialNumber;

    // initial repeat bar
    // ------------------------------------------------------

    Bool                  fThereIsAnImplicitInitialForwardRepeat;

    // all events
    // ------------------------------------------------------

    std::list <S_mxsrEvent>
                          fAllEventsList;

    // measure repeat events
    // ------------------------------------------------------

    // there can be only one measure repeat begin and one measure repeat end
    // per measure, hence two maps,
    // indexed by bare measure number, i.e. an std::string,
    // since using mfMeasureNumber as indexed would imply useless copies
    // into the map indexes
    std::map <std::string, S_mxsrMultipleMeasureRestEvent>
                          fMultipleMeasureRestBeginsMap,
                          fMultipleMeasureRestEndsMap;

    // measure repeat events
    // ------------------------------------------------------

    // there can be only one measure repeat begin and one measure repeat end
    // per measure, hence two maps,
    // indexed by bare measure number, i.e. an std::string,
    // since using mfMeasureNumber as indexed would imply useless copies
    // into the map indexes
    std::map <std::string, S_mxsrMeasureRepeatEvent>
                          fMeasureRepeatBeginsMap,
                          fMeasureRepeatEndsMap;

    // staff change events
    // ------------------------------------------------------

    // there can be two staff changes per note,
    // hence these two maps, indexed by note bare sequential number
    std::map <int, S_mxsrStaffChangeEvent>
                          fStaffChangeTakeOffsMap,
                          fStaffChangeLandingsMap;

    std::list <S_mxsrStaffChangeEvent>
                          fStaffChangeEventsList;

    // grace notes events
    // ------------------------------------------------------

    // there can be only one grace note begin and one chord end per note,
    // hence these two maps, indexed by note bare sequential number
    std::map <mxsrNoteSequentialNumber, S_mxsrGraceEvent>
                          fGraceBeginsMap,
                          fGraceEndsMap;

    // cue notes events
    // ------------------------------------------------------

    // there can be only one cue note begin and one chord end per note,
    // hence these two maps, indexed by note bare sequential number
    std::map <int, S_mxsrCueEvent>
                          fCueBeginsMap,
                          fCueEndsMap;

    // chord events
    // ------------------------------------------------------

    // there can be only one chord begin and one chord end per note,
    // hence these two maps, indexed by note bare sequential number
    std::map <int, S_mxsrChordEvent>
                          fChordBeginsMap,
                          fChordEndsMap;

    // tuplet events
    // ------------------------------------------------------

    // there can be several tuplet start and/or stop events per note,
    // hence these two multimaps, indexed by note bare sequential number
    std::multimap <int, S_mxsrTupletEvent>
                          fTupletBeginsMultiMap,
                          fTupletEndsMultiMap;

    // we need to fetch a tuplet event by its tuplet number,
    // hence the two maps, indexed by tuplet number
    std::map <int, S_mxsrTupletEvent>
                          fTupletBeginNumbersMap,
                          fTupletEndNumbersMap;
};

using S_mxsrEventsCollection = SMARTP<mxsrEventsCollection>;

EXP std::ostream& operator << (std::ostream& os, const S_mxsrEventsCollection& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrEventsCollection& elt);


}


#endif // ___mxsr2msrEvents___
