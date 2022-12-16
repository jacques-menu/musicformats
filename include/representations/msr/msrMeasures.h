/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMeasures___
#define ___msrMeasures___

#include "msrMeasuresEnumTypes.h"
#include "msrMeasureElements.h"
#include "msrMusicXMLPrintLayouts.h"
#include "msrMoments.h"
#include "msrNotes.h"
#include "msrMusicXMLPrintLayouts.h"
#include "msrSegments.h"
#include "msrSegmentElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrMeasure : public msrSegmentElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasure> create (
                            int                 inputLineNumber,
                            const std::string&  measureNumber,
                            const S_msrSegment& measureUpLinkToSegment);
                              // no const alias, call argument may be 'this' JMI v0.9.66

    SMARTP<msrMeasure> createMeasureNewbornClone (
                            const S_msrSegment& containingSegment);

    SMARTP<msrMeasure> createMeasureDeepClone (
                            const S_msrSegment& containingSegment);

    SMARTP<msrMeasure> createMeasureCopyWithNotesOnly (
                            const S_msrSegment& containingSegment,
                            const std::string&  measureNumber);

  protected:

    // constructors
    // ------------------------------------------------------

                          msrMeasure (
                            int                 inputLineNumber,
                            const std::string&  measureNumber,
                            const S_msrSegment& measureUpLinkToSegment);

  public:

    // destructor
    // ------------------------------------------------------

    virtual               ~msrMeasure (); // public for use in msdrVoice

  private:

    // private initialization
    // ------------------------------------------------------

    void                  initializeMeasure ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink

    S_msrSegment          getMeasureUpLinkToSegment () const
                              { return fMeasureUpLinkToSegment; }

    // measure numbers

    void                  setMeasureNumber (
                            const std::string& measureNumber);

    std::string           getMeasureNumber () const
                              { return fMeasureNumber; }

    void                  setMeasureOrdinalNumberInVoice (
                            int measureOrdinalNumber);

    int                   getMeasureOrdinalNumberInVoice () const
                              { return fMeasureOrdinalNumberInVoice; }

    void                  setMeasurePuristNumber (
                            int measurePuristNumber);

    int                   getMeasurePuristNumber () const
                              { return fMeasurePuristNumber; }

    void                  setNextMeasureNumber (const std::string& nextMeasureNumber);

    std::string           getNextMeasureNumber () const
                              { return fNextMeasureNumber; }

    void                  setMeasureIsFirstInVoice ();

    Bool                  getMeasureIsFirstInVoice () const
                              { return fMeasureIsFirstInVoice; }

    void                  setMeasureEndInputLineNumber (int value)
                              { fMeasureEndInputLineNumber = value; }

    int                   getMeasureEndInputLineNumber () const
                              { return fMeasureEndInputLineNumber; }

    // measure lengthes, in whole notes

    void                  setFullMeasureWholeNotesDuration (
                            const Rational& wholeNotes);

    Rational              getFullMeasureWholeNotesDuration () const
                              {  return fFullMeasureWholeNotesDuration; }

    void                  setMeasureWholeNotesDuration (
                            int             inputLineNumber,
                            const Rational& wholeNotes);
    void                  incrementMeasureWholeNotesDuration (
                            int             inputLineNumber,
                            const Rational& wholeNotesDelta);

    Rational              getMeasureWholeNotesDuration () const
                              { return fMeasureWholeNotesDuration; }

    // measure kind

    void                  setMeasureKind (
                            msrMeasureKind measureKind);

    msrMeasureKind        getMeasureKind () const
                              { return fMeasureKind; }

    // measure implicit kind

    void                  setMeasureImplicitKind (
                            msrMeasureImplicitKind measureImplicitKind)
                              {
                                fMeasureImplicitKind =
                                  measureImplicitKind;
                              }

    msrMeasureImplicitKind
                          getMeasureImplicitKind () const
                              { return fMeasureImplicitKind; }


    // measure 'first in segment' kind

    void                  setMeasureFirstInSegmentKind (
                            msrMeasureFirstInSegmentKind
                              measureFirstInSegmentKind)
                              {
                                fMeasureFirstInSegmentKind =
                                  measureFirstInSegmentKind;
                              }

    msrMeasureFirstInSegmentKind
                          getMeasureFirstInSegmentKind () const
                              { return fMeasureFirstInSegmentKind; }

    // single-measure rest?


    void                  setMeasureIsAFullBarRest ()
                              { fMeasureIsAFullBarRest = true; }

    Bool                  getMeasureIsAFullBarRest () const
                             { return fMeasureIsAFullBarRest; }

/* JMI
    // measure shortest note

    void                  setMeasureShortestNoteDuration (
                            const Rational& duration);

    Rational              getMeasureShortestNoteDuration () const
                              { return fMeasureShortestNoteDuration; }

    void                  setMeasureShortestNoteTupletFactor (
                            const msrTupletFactor& noteTupletFactor);

    const msrTupletFactor&
                          getMeasureShortestNoteTupletFactor () const
                              { return fMeasureShortestNoteTupletFactor; }
*/

    // measure longest note

    S_msrNote             getMeasureLongestNote () const
                              { return fMeasureLongestNote; }

    // chords handling

    S_msrNote             getMeasureLastHandledNote () const
                              { return fMeasureLastHandledNote; }

    // elements list

    const std::list<S_msrMeasureElement>&
                          getMeasureElementsList () const
                              { return fMeasureElementsList; }

    Bool                  getMeasureContainsMusic () const
                              { return fMeasureContainsMusic; }

    // regular measure ends detection

    void                  setMeasureEndRegularKind (
                            msrMeasureEndRegularKind measureEndRegularKind);

    msrMeasureEndRegularKind
                          getMeasureEndRegularKind () const
                              { return fMeasureEndRegularKind; }

    // repeat context

    void                  setMeasureRepeatContextKind (
                            msrMeasureRepeatContextKind
                              measureRepeatContextKind);

    msrMeasureRepeatContextKind
                          getMeasureRepeatContextKind () const
                              { return fMeasureRepeatContextKind; }

    // voice position

    void                  setMeasureVoicePosition (
                            const Rational& value)
                              { fMeasureVoicePosition = value; }
//     void                  incrementMeasureVoicePosition (
//                             const Rational& wholeNotesDelta);

    Rational              getMeasureVoicePosition () const
                              { return fMeasureVoicePosition; }

    void                  setMeasureVoiceMoment (
                            msrMoment value)
                              { fMeasureVoiceMoment = value; }

    const msrMoment&      getMeasureVoiceMoment () const
                              { return fMeasureVoiceMoment; }

    // notes flat list

    const std::list<S_msrNote>&
                          getMeasureNotesFlatList () const
                              { return fMeasureNotesFlatList; }

  public:

    // fetch
    // ------------------------------------------------------

    // upLinks

    S_msrVoice            fetchMeasureUpLinkToVoice () const;

    S_msrStaff            fetchMeasureUpLinkToStaff () const;

    S_msrPart             fetchMeasureUpLinkToPart () const;

    S_msrPartGroup        fetchMeasureUpLinkToPartGroup () const;

    S_msrScore            fetchMeasureUpLinkToScore () const;

  public:

    // public services
    // ------------------------------------------------------

    // MSR segment elements

    virtual void          appendMeasureElementToSegmentElement (
                            const S_msrMeasureElement& elem) override
                              { appendMeasureElementToMeasure (elem); } // JMI v0.9.66 ???

    // lengthes

    std::string           fullMeasureWholeNotesDurationAsMsrString ();

    // backup and padding

    S_msrNote             createPaddingSkipNoteForVoice (
                            int             inputLineNumber,
                            const Rational& duration,
                            const S_msrVoice&      voice);

    void                  padUpToMeasurePosition (
                            int             inputLineNumber,
                            const Rational& measurePositionToPadUpTo);

    void                  padUpToPositionAtTheEndOfTheMeasure (
                            int             inputLineNumber,
                            const Rational& measurePositionToPadUpTo);

    void                  backupByWholeNotesStepLengthInMeasure (
                            int             inputLineNumber,
                            const Rational& backupTargetMeasureElementMeasurePosition);

    void                  appendPaddingSkipNoteToMeasure (
                            int             inputLineNumber,
                            const Rational& forwardStepLength);

    // print layout

    void                  appendMusicXMLPrintLayoutToMeasure (
                            const S_msrMusicXMLPrintLayout& musicXMLPrintLayout);

    S_msrMusicXMLPrintLayout
                          getMeasureMusicXMLPrintLayout () const
                              { return fMeasureMusicXMLPrintLayout; }

    // clefs

    void                  appendClefToMeasure (
                            const S_msrClef& clef);

    // keys

    void                  appendKeyToMeasure (
                            const S_msrKey& key);

    // time signatures

    void                  appendTimeSignatureToMeasure (
                            const S_msrTimeSignature& timeSignature);

    void                  setFullMeasureWholeNotesDurationFromTime (
                            const S_msrTimeSignature& timeSignature);

    void                  appendTimeSignatureToMeasureClone (
                            const S_msrTimeSignature& timeSignature);

    // dal segno

    void                  insertHiddenMeasureAndBarLineInMeasureClone (
                            int             inputLineNumber,
                            const Rational& measurePosition);

    // transposition

    void                  appendTranspositionToMeasure (
                            const S_msrTransposition& transpostion);

    // staff details

    void                  appendStaffDetailsToMeasure (
                            const S_msrStaffDetails& staffDetails);

    // bar number checks

    void                  appendBarNumberCheckToMeasure (
                            const S_msrBarNumberCheck& barNumberCheck);

    // breaks

    void                  appendLineBreakToMeasure (
                            const S_msrLineBreak& lineBreak);
    void                  appendPageBreakToMeasure (
                            const S_msrPageBreak& pageBreak);

    // tempo

    void                  appendTempoToMeasure (
                            const S_msrTempo& tempo);

    // rehearsal marks

    void                  appendRehearsalMarkToMeasure (
                            const S_msrRehearsalMark& rehearsalMark);

    // octave shifts

    void                  appendOctaveShiftToMeasure (
                            const S_msrOctaveShift& octaveShift);

    // scordaturas

    void                  appendScordaturaToMeasure (
                            const S_msrScordatura& scordatura);

    // accordion registration

    void                  appendAccordionRegistrationToMeasure (
                            const S_msrAccordionRegistration&
                              accordionRegistration);

    // harp pedals tuning

    void                  appendHarpPedalsTuningToMeasure (
                            const S_msrHarpPedalsTuning&
                              harpPedalsTuning);

    // bar lines

    void                  appendBarLineToMeasure (
                            const S_msrBarLine& barLine);
    void                  prependBarLineToMeasure (
                            const S_msrBarLine& barLine);

    // bar checks

    void                  appendBarCheckToMeasure (
                            const S_msrBarCheck& barCheck);

    // staff change

    void                  appendVoiceStaffChangeToMeasure (
                            const S_msrVoiceStaffChange& voiceStaffChange);

    // notes

    void                  appendNoteToMeasure (
                            const S_msrNote& note,
                            const Rational&  partMeasurePosition);

    void                  appendNoteOrPaddingToMeasure (
                            const S_msrNote& note);

    void                  accountForTupletMemberNoteDurationInMeasure (
                            const S_msrNote& note);

    void                  appendPaddingNoteAtTheEndOfMeasure (
                            const S_msrNote& paddingNote);

    void                  appendNoteToMeasureClone (
                            const S_msrNote& note);

    // tremolos

    void                  appendDoubleTremoloToMeasure (
                            const S_msrDoubleTremolo& doubleTremolo);

     // chords

    void                  appendChordToMeasure (
                            const S_msrChord& chord);

    // tuplets

    void                  appendTupletToMeasure (
                            const S_msrTuplet& tuplet);

    // harmonies

    void                  appendHarmonyToMeasure (
                            const S_msrHarmony& harmony);

    void                  appendHarmonyToMeasureClone (
                            const S_msrHarmony& harmony);

    // frames

    void                  appendFrameToMeasure (
                            const S_msrFrame& frame);

    void                  appendFrameToMeasureClone (
                            const S_msrFrame& frame);

    // figured bass

    void                  appendFiguredBassToMeasure (
                            const S_msrFiguredBass& figuredBass);

    void                  appendFiguredBassToMeasureClone (
                            const S_msrFiguredBass& figuredBass);

    // grace notes

    /* JMI
    void                  addGraceNotesGroupAheadOfMeasure (
                            const S_msrGraceNotesGroup& graceNotesGroup);

    void                  appendAfterGraceNotesToMeasure (
                            const S_msrAfterGraceNotes& afterGraceNotes);

    void                  prependAfterGraceNotesToMeasure (
                            const S_msrAfterGraceNotes& afterGraceNotes);
                            */

    // segno

    void                  appendSegnoToMeasure (
                            const S_msrSegno& segno);

    // coda

    void                  appendCodaToMeasure (
                            const S_msrCoda& coda);

    // eyeglasses

    void                  appendEyeGlassesToMeasure (
                            const S_msrEyeGlasses& eyeGlasses);

    // pedal

    void                  appendPedalToMeasure (
                            const S_msrPedal& pedal);

    // damp

    void                  appendDampToMeasure (
                            const S_msrDamp& damp);

    // damp all

    void                  appendDampAllToMeasure (
                            const S_msrDampAll& dampAll);

    // other elements

    void                  prependOtherElementToMeasure (
                            const S_msrMeasureElement& elem);
    void                  appendOtherElementToMeasure (
                            const S_msrMeasureElement& elem);

    // last element of measure

    S_msrElement          getLastElementOfMeasure () const
                              { return fMeasureElementsList.back (); }

     /* JMI v0.9.66
    S_msrElement          removeLastElementFromMeasure (
                            int inputLineNumber);
*/

    // removing elements from measure

    void                  removeNoteFromMeasure (
                            int              inputLineNumber,
                            const S_msrNote& note);

    void                  removeElementFromMeasure (
                            int                 inputLineNumber,
                            const S_msrElement& elem);

    // notes flat list

    void                  appendNoteToMeasureNotesFlatList (
                            const S_msrNote& note);

    // finalization

    void                  determineMeasureKindAndPuristNumber (
                            int     inputLineNumber,
                            msrMeasureRepeatContextKind
                                    measureRepeatContextKind);

    void                  finalizeMeasure (
                            int                         inputLineNumber,
                            msrMeasureRepeatContextKind measureRepeatContextKind,
                            const std::string&          context);

    void                  finalizeMeasureClone (
                            int                 inputLineNumber,
                            const S_msrMeasure& originalMeasure,
                            const S_msrVoice&   voiceClone);

  private:

    // private services
    // ------------------------------------------------------

    void                  appendMeasureElementToMeasure (
                            const S_msrMeasureElement& elem);

    void                  finalizeRegularMeasure (
                            int                         inputLineNumber,
                            msrMeasureRepeatContextKind measureRepeatContextKind,
                            const std::string&          context);

    void                  finalizeHarmonyMeasure (
                            int                         inputLineNumber,
                            msrMeasureRepeatContextKind measureRepeatContextKind,
                            const std::string&          context);

    void                  finalizeFiguredBassMeasure (
                            int                         inputLineNumber,
                            msrMeasureRepeatContextKind measureRepeatContextKind,
                            const std::string&          context);

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

    std::string           asShortStringForMeasuresSlices () const override;

    void                  displayMeasure (
                            int                inputLineNumber,
                            const std::string& context);

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks

    S_msrSegment          fMeasureUpLinkToSegment;

    // elements

    std::list<S_msrMeasureElement>
                          fMeasureElementsList;

//     std::list<msrElementsBase*>
//                           fMeasureElementsBaseList;

    Bool                  fMeasureContainsMusic;

    // measure lengthes, in whole notes

    Rational              fFullMeasureWholeNotesDuration;
                            // meaningfull only
                            // when there is a time signature,
                            // but not for cadenzas

    // measure numbers

    std::string           fMeasureNumber;
    std::string           fNextMeasureNumber;

    int                   fMeasureOrdinalNumberInVoice;

    // purist measure number, forcing anacruses to start at '0' if it's not the case
    // and not shared among repeats components

    int                   fMeasurePuristNumber;

    // debug number, unique for every msrMeasure instance

    static int            gGlobalMeasureDebugNumber;
    int                   fMeasureDebugNumber;

    // measure end line number

    int                   fMeasureEndInputLineNumber;

    // measure lengthes, in whole notes

    Rational              fMeasureWholeNotesDuration;
                            // this increases when musical elements
                            // are appended to the measure

    std::string           measureWholeNotesDurationAsMsrString ();

    // first measure in voice?

    Bool                  fMeasureIsFirstInVoice;

    // measure print layout, MusicXML specific

    S_msrMusicXMLPrintLayout
                          fMeasureMusicXMLPrintLayout;

/* JMI
    // measure shortest note

    // fPartShortestNoteDuration and fPartShortestNoteTupletFactor
    // are used to compute a number of divisions per quarter note
    // if needed, such as when generating MusicXML from MSR
    Rational              fMeasureShortestNoteDuration;
    msrTupletFactor       fMeasureShortestNoteTupletFactor;
*/

    // measure longest note

    S_msrNote             fMeasureLongestNote;

    // measure kind

    msrMeasureKind        fMeasureKind;

    // measure implicit kind

    msrMeasureImplicitKind
                          fMeasureImplicitKind;

    // measure 'first in segment' kind

    msrMeasureFirstInSegmentKind
                          fMeasureFirstInSegmentKind;

    // multiple full-bar rest?

    Bool                  fMeasureIsAFullBarRest;

    // chords handling

    S_msrNote             fMeasureLastHandledNote;

    // regular measure ends detection
    msrMeasureEndRegularKind
                          fMeasureEndRegularKind;

    // repeat context
    msrMeasureRepeatContextKind
                          fMeasureRepeatContextKind;

    // voice position
    Rational              fMeasureVoicePosition;
    msrMoment             fMeasureVoiceMoment;

    // notes flat list
    // this is used to identify solo notes and rests in measures slices
    std::list<S_msrNote>  fMeasureNotesFlatList;

  private:

    // private work services
    // ------------------------------------------------------

    // harmonies
    void                  handleFirstHarmonyInHarmoniesMeasure (
                            int                 inputLineNumber,
                            const S_msrVoice&   voice,
                            const S_msrHarmony& currentHarmony);

    void                  handleSubsequentHarmonyInHarmoniesMeasure (
                            int                 inputLineNumber,
                            const S_msrVoice&   voice,
                            const S_msrHarmony& previousHarmony,
                            const S_msrHarmony& currentHarmony);

    void                  postHandleCurrentHarmonyInHarmoniesMeasure (
                            int                 inputLineNumber,
                            const S_msrVoice&   voice,
                            const S_msrHarmony& currentHarmony);

    void                  finalizeTheHarmoniesInHarmoniesMeasure (
                            int                inputLineNumber,
                            const std::string& context);

    void                  finalizeHarmonyInHarmoniesMeasure (
                            int                 inputLineNumber,
                            const S_msrVoice&   voice,
                            const S_msrHarmony& previousHarmony,
                            const S_msrHarmony& currentHarmony,
                            const std::string&  context);

    // figurd bass
    void                  handleFirstFiguredBassInFiguredBassMeasure (
                            int                     inputLineNumber,
                            const S_msrVoice&       voice,
                            std::list<S_msrMeasureElement>::iterator&
                                                    i,
                            const S_msrFiguredBass& previousFiguredBass,
                            const S_msrFiguredBass& currentFiguredBass,
                            const Rational&         currentFiguredBassMeasurePosition);

    void                  handleSubsequentFiguredBassInFiguredBassMeasure (
                            int                     inputLineNumber,
                            const S_msrVoice&       voice,
                            std::list<S_msrMeasureElement>::iterator&
                                                    i,
                            const S_msrFiguredBass& previousFiguredBass,
                            const S_msrFiguredBass& currentFiguredBass,
                            const Rational&         currentFiguredBassMeasurePosition);

    void                  postHandleCurrentFiguredBassInFiguredBassMeasure (
                            int                     inputLineNumber,
                            const S_msrVoice&       voice,
                            const S_msrFiguredBass& currentFiguredBass);

    void                  finalizeFiguredBassesInFiguredBassMeasure (
                            int                inputLineNumber,
                            const std::string& context);

    // measure elements
    void                  insertElementInMeasureBeforeIterator (
                            int                                      inputLineNumber,
                            std::list<S_msrMeasureElement>::iterator iter,
                            const S_msrMeasureElement&               elem);

    void                  insertElementInMeasureBeforeReverseIterator (
                            int                        inputLineNumber,
                            std::list<S_msrMeasureElement>::reverse_iterator iter,
                            const S_msrMeasureElement& elem);

    void                  appendElementAtTheEndOfMeasure (
                            const S_msrMeasureElement& elem);

    void                  insertElementAtMeasurePosition (
                            int                        inputLineNumber,
                            const Rational&            measurePosition,
                            const S_msrMeasureElement& elem);

    void                  handleEmptyMeasure (
                            int               inputLineNumber,
                            const S_msrVoice& voice);

    void                  handleRegularMeasure (
                            int               inputLineNumber,
                            const S_msrVoice& voice);

    void                  handleIncompleteMeasure (
                            int               inputLineNumber,
                            const S_msrVoice& voice,
                            msrMeasureRepeatContextKind
                                              measureRepeatContextKind,
                            const Rational&   newWholeNotesSinceLastRegularMeasureEnd);

    void                  handleOverFullMeasure (
                            int               inputLineNumber,
                            const S_msrVoice& voice);

    void                  printMeasurePendingMeasureElementsList ();

  private:

    // private work fields
    // ------------------------------------------------------

    // pending measure elements, which may have to be delayed
    // because of <backup />
    std::list<S_msrMeasureElement>
                          fMeasurePendingMeasureElementsList;

    // measure finalization
    Bool                  fMeasureHasBeenFinalized;
    Bool                  fMeasureKindAndPuristNumberHaveBeenDetermined;

    std::string           fMeasureFinalizationContext;
};
typedef SMARTP<msrMeasure> S_msrMeasure;
EXP std::ostream& operator << (std::ostream& os, const S_msrMeasure& elt);


}


#endif
