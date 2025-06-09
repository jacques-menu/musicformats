/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrSegments___
#define ___msrSegments___

#include "msrVoiceElements.h"

#include "msrClefKeyTimeSignatureGroups.h"
#include "msrEyeGlasses.h"
#include "msrLyrics.h"
#include "msrMeasures.h"
#include "mfMoments.h"
#include "msrMultipleMeasureRests.h"


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
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrVoice& segmentUpLinkToVoice);

    SMARTP<msrSegment> createSegmentNewbornClone (
                            const S_msrVoice& containingVoice);

    SMARTP<msrSegment> createSegmentDeepClone (
                            const S_msrVoice& containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSegment (
                            const mfInputLineNumber& inputLineNumber,
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

    const std::list <S_msrSegmentElement>&
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

    // measures list

    const std::list <S_msrMeasure>&
                          getSegmentMeasureList () const
                              { return fSegmentMeasuresList; }

/* JMI
    // segment shortest note

    void                  setSegmentShortestNoteWholeNotes (
                            const mfWholeNotes& wholeNotes);

    mfWholeNotes          getSegmentShortestNoteWholeNotes () const
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

//     void                  padUpToPositionInMeasureInSegment (
//                             const mfInputLineNumber& inputLineNumber,
//                             const mfWholeNotes& wholeNotes);

//     void                  casadeBackupByWholeNotesStepLengthInSegment (
//                             const mfInputLineNumber& inputLineNumber,
//                             const mfWholeNotes&
//                                     backupTargetMeasureElementPositionInMeasure);

    void                  cascadeAppendPaddingNoteToSegment (
                            const mfInputLineNumber& inputLineNumber,
                            const mfWholeNotes& forwardStepLength);

    // measures

    S_msrMeasure          fetchLastMeasureFromSegment (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& context);

    S_msrMeasure          cascadeCreateAMeasureAndAppendItInSegment (
                            const mfInputLineNumber& inputLineNumber,
                            int                    previousMeasureEndInputLineNumber,
                            const std::string&     measureNumber,
                            msrMeasureImplicitKind measureImplicitKind);

    void                  setNextMeasureNumberInSegment (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& nextMeasureNumber);

    void                  appendMeasureToSegment (
                            const S_msrMeasure& measure);

    void                  prependMeasureToSegment (
                            const S_msrMeasure& measure);

    // measure repeats

    void                  cascadeCreateAMeasureRepeatAndAppendItToSegment (
                            const mfInputLineNumber& inputLineNumber,
                            int                    previousMeasureEndInputLineNumber,
                            const std::string&     measureNumber,
                            msrMeasureImplicitKind measureImplicitKind);

    // multiple measure rests

    Bool                  fOnGoingMultipleMeasureRest;
    S_msrMultipleMeasureRest
                          fCurrentMultipleMeasureRest;

    void                  cascadeAppendMultipleMeasureRestToSegment (
                            const S_msrMultipleMeasureRest& multipleMeasureRest);

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

    void                  appendTimeSignatureToSegment (
                            const S_msrTimeSignature& timeSignature);

    void                  appendTimeSignatureToSegmentClone (
                            const S_msrTimeSignature& timeSignature);

    // dal segno

    void                  insertHiddenMeasureAndBarLineInSegmentClone (
                            const mfInputLineNumber& inputLineNumber,
                            const mfPositionInMeasure& positionInMeasure);

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
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrHarmony&        harmony,
                            const mfPositionInMeasure& positionInMeasureToAppendAt);

    void                  appendHarmoniesListToSegment (
                            const mfInputLineNumber&        inputLineNumber,
                            const std::list <S_msrHarmony>& harmoniesList,
                            const mfPositionInMeasure&      positionInMeasureToAppendAt);

    void                  appendHarmonyToSegmentClone (
                            const S_msrHarmony& harmony);

    // frames

    void                  appendFrameToSegment (
                            const S_msrFrame& frame);

    void                  appendFrameToSegmentClone (
                            const S_msrFrame& frame);

    // figured bass

    void                  appendFiguredBassToSegment (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrFiguredBass&    figuredBass,
                            const mfPositionInMeasure& positionInMeasureToAppendAt);

    void                  cascadeAppendFiguredBassesListToSegment (
                            int                                 inputLineNumber,
                            const std::list <S_msrFiguredBass>& figuredBasssesList,
                            const mfPositionInMeasure&          positionInMeasureToAppendAt);

    void                  appendFiguredBassToSegmentClone (
                            const S_msrFiguredBass& figuredBass);

    // staff change

    void                  appendVoiceStaffChangeToSegment (
                            const S_msrVoiceStaffChange& voiceStaffChange);

    // notes

    void                  appendNoteToSegment (
                            const S_msrNote&  note,
                            const mfPositionInMeasure&
                                              partCurrentDrawingPositionInMeasure);

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

//     void                  removeNoteFromSegment (
//                             const mfInputLineNumber& inputLineNumber,
//                             const S_msrNote& note);

//     void                  removeElementFromSegment (
//                             const mfInputLineNumber& inputLineNumber,
//                             const S_msrElement& element);

//     S_msrMeasure          removeLastMeasureFromSegment (
//                             const mfInputLineNumber& inputLineNumber,
//                             const std::string& context);

    void                  finalizeAllTheMeasuresOfSegment (
                            const mfInputLineNumber& inputLineNumber);

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
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& context);

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

  private:

    // private services
    // ------------------------------------------------------

    // measures
    void                  assertSegmentLastMeasureIsNotNull (
                            const mfInputLineNumber& inputLineNumber) const;

    // necessary due to the complexity of repeats management
    void                  assertSegmentElementsListIsNotEmpty (
                            const mfInputLineNumber& inputLineNumber) const;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fSegmentUpLinkToVoice;

    // absolute number, shared by newborn clones and deep copies
    int                   fSegmentAbsoluteNumber;

    // the measures elements in the segment contain the mmusic
    std::list <S_msrSegmentElement>
                          fSegmentElementsList;

    // segment measures list
    std::list <S_msrMeasure>
                          fSegmentMeasuresList;

    // debug number, unique for every msrSegment instance
    static int            sSegmentDebugNumber;
    int                   fSegmentDebugNumber;

/* JMI
    // segment shortest note

    // fPartShortestNoteWholeNotes and fPartShortestNoteTupletFactor
    // are used to compute a number of divisions per quarter note
    // if needed, such as when generating MusicXML from MSR
    mfWholeNotes          fSegmentShortestNoteWholeNotes;
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

using S_msrSegment = SMARTP<msrSegment>;

EXP std::ostream& operator << (std::ostream& os, const S_msrSegment& elt);


}


#endif // ___msrSegments___
