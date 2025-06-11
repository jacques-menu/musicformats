/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsrEventsCollections___
#define ___mxsrEventsCollections___

// #include <iomanip>
// #include <string>
#include <list>
#include <map>

#include "exports.h"
#include "smartpointer.h"

// #include "mfBasicTypes.h"
// #include "mfBool.h"
// #include "mfIndentedTextOutput.h"
// #include "mfWrappedValueForArithmetic.h"
#include "mfStringsHandling.h"

// #include "mxsrBasicTypes.h"
#include "mxsrChordEvents.h"
#include "mxsrCueEvents.h"
#include "mxsrGraceEvents.h"
#include "mxsrMeasureRepeatEvents.h"
#include "mxsrMultipleMeasureRestEvents.h"
#include "mxsrNoteEvents.h"
#include "mxsrStaffChangeEvents.h"
#include "mxsrTupletEvents.h"

// #include "msrTupletFactors.h"


using namespace MusicXML2;

namespace MusicFormats
{

//------------------------------------------------------------------------
class EXP mxsrEventsCollection : public smartable
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

    Bool                  getThereIsAnImplicitInitialForwardRepeat () const
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
                          fetchMultipleMeasureRestBeginAtMeasureNumber (
                            const mfMeasureNumber& measureNumber) const;

    S_mxsrMultipleMeasureRestEvent
                          fetchMultipleMeasureRestEndAtMeasureNumber (
                            const mfMeasureNumber& measureNumber) const;

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

    S_mxsrMeasureRepeatEvent
                          createAMeasureRepeatEnd (
                            const std::string&       partName,
                            const mfMeasureNumber&   measureNumber,
                            int                      measureRepeatNumber,
                            int                      measureRepeatSlashes,
                            const mfInputLineNumber& eventInputLineNumber);

    void                  registerMeasureRepeatEnd (
                            S_mxsrMeasureRepeatEvent measureRepeatEvent);

    void                  createAMeasureRepeatEndAndRegisterIt (
                            const std::string&       partName,
                            const mfMeasureNumber&   measureNumber,
                            int                      measureRepeatNumber,
                            int                      measureRepeatSlashes,
                            const mfInputLineNumber& eventInputLineNumber);

    S_mxsrMeasureRepeatEvent
                          fetchMeasureRepeatBeginAtMeasureNumber (
                            const mfMeasureNumber& measureNumber) const;

    S_mxsrMeasureRepeatEvent
                          fetchMeasureRepeatEndAtMeasureNumber (
                            const mfMeasureNumber& measureNumber) const;

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
    // per measure, hence two maps, indexed by bare measure number
    std::map <mfMeasureNumber, S_mxsrMultipleMeasureRestEvent>
                          fMultipleMeasureRestBeginsMap,
                          fMultipleMeasureRestEndsMap;

    // measure repeat events
    // ------------------------------------------------------

    // there can be only one measure repeat begin and one measure repeat end
    // per measure, hence two maps, indexed by bare measure number
    std::map <mfMeasureNumber, S_mxsrMeasureRepeatEvent>
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


#endif // ___mxsrEventsCollections___
