/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrSegments___
#define ___msrSegments___

#include "msrVoiceElements.h"

// #include "msrClefKeyTimeSignatureGroup.h"
#include "msrClefsKeysTimeSignatures.h"
// #include "msrClefsKeysTimeSignatures.h"
// #include "msrClefsKeysTimeSignatures.h"
#include "msrEyeGlasses.h"
#include "msrLyrics.h"
#include "msrMeasures.h"
#include "msrMoments.h"
#include "msrMultipleFullBarRests.h"
// #include "msrClefsKeysTimeSignatures.h"


namespace MusicFormats
{

//______________________________________________________________________________
// PRE-declarations for class mutual dependencies

class   msrClefKeyTimeSignatureGroup;
typedef SMARTP<msrClefKeyTimeSignatureGroup> S_msrClefKeyTimeSignatureGroup;

//______________________________________________________________________________
class EXP msrSegment : public msrVoiceElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSegment> create (
                            int        inputLineNumber,
                            const S_msrVoice& segmentUpLinkToVoice);

    SMARTP<msrSegment> createSegmentNewbornClone (
                            const S_msrVoice& containingVoice);

    SMARTP<msrSegment> createSegmentDeepClone (
                            const S_msrVoice& containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSegment (
                            int        inputLineNumber,
                            const S_msrVoice& segmentUpLinkToVoice);

    virtual               ~msrSegment ();

  private:

    // private initialization
    // ------------------------------------------------------

    void                  initializeSegment ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink

    S_msrVoice            getSegmentUpLinkToVoice () const
                              { return fSegmentUpLinkToVoice; }

    // number

    int                   getSegmentAbsoluteNumber () const
                              { return fSegmentAbsoluteNumber; }

    // measures elements

    const std::list<S_msrSegmentElement>&
                          getSegmentElementsList () const
                              { return fSegmentElementsList; }

    // measures

    void                  setSegmentFirstMeasure (
                            const S_msrMeasure& measure);

    S_msrMeasure          getSegmentFirstMeasure () const
                              { return fSegmentFirstMeasure; }

    void                  setSegmentLastMeasure (
                            const S_msrMeasure& measure);

    S_msrMeasure          getSegmentLastMeasure () const
                              { return fSegmentLastMeasure; }

    // measures flat list

    const std::list<S_msrMeasure>&
                          getSegmentMeasuresFlatList () const
                              { return fSegmentMeasuresFlatList; }

/* JMI
    // segment shortest note

    void                  setSegmentShortestNoteWholeNotes (
                            const msrWholeNotes& wholeNotes);

    msrWholeNotes         getSegmentShortestNoteWholeNotes () const
                              { return fSegmentShortestNoteWholeNotes; }

    void                  setSegmentShortestNoteTupletFactor (
                            const msrTupletFactor& noteTupletFactor);

    const msrTupletFactor&
                          getSegmentShortestNoteTupletFactor () const
                              { return fSegmentShortestNoteTupletFactor; }
*/

  public:

    // public services
    // ------------------------------------------------------

    // upLinks

    S_msrStaff            fetchSegmentUpLinkToStaff () const;

    S_msrPart             fetchSegmentUpLinkToPart () const;

    S_msrPartGroup        fetchSegmentUpLinkToPartGroup () const;

    S_msrScore            fetchSegmentUpLinkToScore () const;

    // print layout MusicXML specific??? JMI

    void                  appendMusicXMLPrintLayoutToSegment (
                            const S_msrMusicXMLPrintLayout& musicXMLPrintLayout);

    // backup and padding

//     void                  padUpToMeasurePositionInSegment (
//                             int             inputLineNumber,
//                             const msrWholeNotes& wholeNotes);

    void                  backupByWholeNotesStepLengthInSegment (
                            int     inputLineNumber,
                            const msrWholeNotes&
                                    backupTargetMeasurePosition);

    void                  appendPaddingNoteToSegment (
                            int             inputLineNumber,
                            const msrWholeNotes& forwardStepLength);

    // measures

    S_msrMeasure          fetchLastMeasureFromSegment (
                            int                inputLineNumber,
                            const std::string& context);

    S_msrMeasure          createAMeasureAndAppendItToSegment (
                            int           inputLineNumber,
                            int           previousMeasureEndInputLineNumber,
                            const std::string& measureNumber,
                            msrMeasureImplicitKind
                                          measureImplicitKind);

    void                  setNextMeasureNumberInSegment (
                            int           inputLineNumber,
                            const std::string& nextMeasureNumber);

    void                  appendMeasureToSegment (
                            const S_msrMeasure& measure);

    void                  prependMeasureToSegment (
                            const S_msrMeasure& measure);

    void                  appendMultipleFullBarRestsToSegment (
                            const S_msrMultipleFullBarRests& multipleFullBarRests);

    // clef, key, time signature

    void                  appendClefKeyTimeSignatureGroupToSegment (
                            const S_msrClefKeyTimeSignatureGroup&
                              clefKeyTimeSignatureGroup);

//     void                  appendClefToSegment (
//                             const S_msrClef& clef);
//     void                  prependClefToSegment (
//                             const S_msrClef& clef);
//
//     void                  appendKeyToSegment (
//                             const S_msrKey& key);
//
//     void                  appendTimeSignatureToSegment (
//                             const S_msrTimeSignature& timeSignature);

    void                  appendTimeSignatureToSegmentClone (
                            const S_msrTimeSignature& timeSignature);

    // dal segno

    void                  insertHiddenMeasureAndBarLineInSegmentClone (
                            int             inputLineNumber,
                            const msrWholeNotes& measurePosition);

    // transposition

    void                  appendTranspositionToSegment (
                            const S_msrTransposition& transpose);

    // staff details

    void                  appendStaffDetailsToSegment (
                            const S_msrStaffDetails& staffDetails);

    // tempo

    void                  appendTempoToSegment (
                            const S_msrTempo& tempo);

    // rehearsal marks

    void                  appendRehearsalMarkToSegment (
                            const S_msrRehearsalMark& rehearsalMark);

    // octave shifts

    void                  appendOctaveShiftToSegment (
                            const S_msrOctaveShift& octaveShift);

    // scordaturas

    void                  appendScordaturaToSegment (
                            const S_msrScordatura& scordatura);

    // accordion registration

    void                  appendAccordionRegistrationToSegment (
                            const S_msrAccordionRegistration&
                              accordionRegistration);

    // harp pedals tuning

    void                  appendHarpPedalsTuningToSegment (
                            const S_msrHarpPedalsTuning&
                              harpPedalsTuning);

    // harmonies

    void                  appendHarmonyToSegment (
                            int                  inputLineNumber,
                            const S_msrHarmony&  harmony,
                            const msrWholeNotes& measurePositionToAppendAt);

    void                  appendHarmoniesListToSegment (
                            int                            inputLineNumber,
                            const std::list<S_msrHarmony>& harmoniesList,
                            const msrWholeNotes&           measurePositionToAppendAt);

    void                  appendHarmonyToSegmentClone (
                            const S_msrHarmony& harmony);

    // frames

    void                  appendFrameToSegment (
                            const S_msrFrame& frame);

    void                  appendFrameToSegmentClone (
                            const S_msrFrame& frame);

    // figured bass

    void                  appendFiguredBassToSegment (
                            int                     inputLineNumber,
                            const S_msrFiguredBass& figuredBass,
                            const msrWholeNotes&    measurePositionToAppendAt);

    void                  appendFiguredBassesListToSegment (
                            int                                inputLineNumber,
                            const std::list<S_msrFiguredBass>& figuredBasssesList,
                            const msrWholeNotes&               measurePositionToAppendAt);

    void                  appendFiguredBassToSegmentClone (
                            const S_msrFiguredBass& figuredBass);

    // staff change

    void                  appendVoiceStaffChangeToSegment (
                            const S_msrVoiceStaffChange& voiceStaffChange);

    // notes

    void                  appendNoteToSegment (
                            const S_msrNote& note,
                            const msrWholeNotes& partDrawingMeasurePosition);

    void                  appendNoteToSegmentClone (const S_msrNote& note);

    // tremolos

    void                  appendDoubleTremoloToSegment (
                            const S_msrDoubleTremolo& doubleTremolo);

    // chords

    void                  appendChordToSegment (const S_msrChord& chord);

    // tuplets

    void                  appendTupletToSegment (const S_msrTuplet& tuplet);

    // bar lines

    void                  appendBarLineToSegment (
                            const S_msrBarLine& barLine);

    void                  prependBarLineToSegment (
                            const S_msrBarLine& barLine);

    // bar checks

    void                  appendBarCheckToSegment (
                            const S_msrBarCheck& barCheck);

    // bar number checks

    void                  appendBarNumberCheckToSegment (
                            const S_msrBarNumberCheck& barNumberCheck);

    // breaks

    void                  appendLineBreakToSegment (
                            const S_msrLineBreak& lineBreak);
    void                  appendPageBreakToSegment (
                            const S_msrPageBreak& pageBreak);

    // segno

    void                  appendSegnoToSegment (const S_msrSegno& segno);

    // coda

    void                  appendCodaToSegment (const S_msrCoda& coda);

    // eyeglasses

    void                  appendEyeGlassesToSegment (
                            const S_msrEyeGlasses& eyeGlasses);

    // pedal

    void                  appendPedalToSegment (const S_msrPedal& pedal);

    // damp

    void                  appendDampToSegment (
                            const S_msrDamp& damp);

    // damp all

    void                  appendDampAllToSegment (
                            const S_msrDampAll& dampAll);

    // grace notes

    /* JMI
    void                  addGraceNotesGroupAheadOfSegmentIfNeeded (
                            const S_msrGraceNotesGroup& graceNotesGroup);

    void                  appendAfterGraceNotesToSegment (
                            const S_msrAfterGraceNotes& afterGraceNotes);

    void                  prependAfterGraceNotesToSegment (
                            const S_msrAfterGraceNotes& afterGraceNotes);
                            */

    // other elements

//     void                  prependOtherElementToSegment (
//                             const S_msrMeasureElement& elem);
//     void                  appendOtherElementToSegment (
//                             const S_msrMeasureElement& elem);

    // removing elements

    void                  removeNoteFromSegment (
                            int       inputLineNumber,
                            const S_msrNote& note);

    void                  removeElementFromSegment (
                            int          inputLineNumber,
                            const S_msrElement& element);

    S_msrMeasure          removeLastMeasureFromSegment (
                            int                inputLineNumber,
                            const std::string& context);

    void                  finalizeAllTheMeasuresOfSegment (
                            int inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortString () const override;
    std::string           asString () const override;

    void                  displaySegment (
                            int                inputLineNumber,
                            const std::string& context);

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

  private:

    // private services
    // ------------------------------------------------------

    // measures
    void                  assertSegmentLastMeasureIsNotNull (
                            int inputLineNumber) const;

    // necessary due to the complexity of repeats management
    void                  assertSegmentElementsListIsNotEmpty (
                            int inputLineNumber) const;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fSegmentUpLinkToVoice;

    // absolute number, shared by newborn clones and deep copies
    int                   fSegmentAbsoluteNumber;

    // the measures elements in the segment contain the mmusic
    std::list<S_msrSegmentElement>
                          fSegmentElementsList;

    // measures flat list
    // including those not in non-measure segment elements,
    std::list<S_msrMeasure>
                          fSegmentMeasuresFlatList;

    // debug number, unique for every msrSegment instance
    static int            sSegmentDebugNumber;
    int                   fSegmentDebugNumber;

/* JMI
    // segment shortest note

    // fPartShortestNoteWholeNotes and fPartShortestNoteTupletFactor
    // are used to compute a number of divisions per quarter note
    // if needed, such as when generating MusicXML from MSR
    msrWholeNotes         fSegmentShortestNoteWholeNotes;
    msrTupletFactor       fSegmentShortestNoteTupletFactor;
    */

  private:

    // private work fields
    // ------------------------------------------------------

    // counter
    static int            sSegmentsCounter;

    // measures
    S_msrMeasure          fSegmentFirstMeasure;
    S_msrMeasure          fSegmentLastMeasure;
};
typedef SMARTP<msrSegment> S_msrSegment;
EXP std::ostream& operator << (std::ostream& os, const S_msrSegment& elt);


}


#endif // ___msrSegments___
