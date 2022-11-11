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

#include "msrSegmentElements.h"

#include "msrMeasuresEnumTypes.h"
#include "msrMoments.h"
#include "msrNotes.h"
#include "msrSegments.h"

// #include "msrFiguredBasses.h"
// #include "msrOctaveShifts.h"
// #include "msrParts.h"
// #include "msrScores.h"
// #include "msrSegnos.h"
// #include "msrStaves.h"
// #include "msrTablatures.h"
// #include "msrTuplets.h"
// #include "msrVoices.h"
// #include "msrVoiceStaffChanges.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrMeasure : public msrSegmentElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasure> create (
                            int           inputLineNumber,
                            const string& measureNumber,
                            S_msrSegment  measureUpLinkToSegment);

    SMARTP<msrMeasure> createMeasureNewbornClone (
                            S_msrSegment containingSegment);

    SMARTP<msrMeasure> createMeasureDeepClone (
                            S_msrSegment containingSegment);

    SMARTP<msrMeasure> createMeasureCopyWithNotesOnly (
                            S_msrSegment  containingSegment,
                            const string& measureNumber);

  protected:

    // constructors
    // ------------------------------------------------------

                          msrMeasure (
                            int           inputLineNumber,
                            const string& measureNumber,
                            S_msrSegment  measureUpLinkToSegment);

  public:

    // destructor
    // ------------------------------------------------------

    virtual               ~msrMeasure (); // public for une in msdrVoice

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
                            const string& measureNumber);

    string                getMeasureNumber () const
                              { return fMeasureNumber; }

    void                  setMeasureOrdinalNumberInVoice (
                            int measureOrdinalNumber);

    int                   getMeasureOrdinalNumberInVoice () const
                              { return fMeasureOrdinalNumberInVoice; }

    void                  setMeasurePuristNumber (
                            int measurePuristNumber);

    int                   getMeasurePuristNumber () const
                              { return fMeasurePuristNumber; }

    void                  setNextMeasureNumber (const string& nextMeasureNumber);

    string                getNextMeasureNumber () const
                              { return fNextMeasureNumber; }

    void                  setMeasureFirstInVoice ();

    Bool                  getMeasureFirstInVoice () const
                              { return fMeasureFirstInVoice; }

    void                  setMeasureEndInputLineNumber (int value)
                              { fMeasureEndInputLineNumber = value; }

    int                   getMeasureEndInputLineNumber () const
                              { return fMeasureEndInputLineNumber; }

    // measure lengthes, in whole notes

    void                  setFullMeasureWholeNotesDuration (
                            const Rational& wholeNotes);

    Rational              getFullMeasureWholeNotesDuration () const
                              {  return fFullMeasureWholeNotesDuration; }

    void                  setCurrentMeasureWholeNotesDuration (
                            int             inputLineNumber,
                            const Rational& wholeNotes);
    void                  incrementCurrentMeasureWholeNotesDuration (
                            int             inputLineNumber,
                            const Rational& wholeNotesDelta);

    Rational              getCurrentMeasureWholeNotesDuration () const
                              { return fCurrentMeasureWholeNotesDuration; }

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

    const list<S_msrMeasureElement>&
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

    // position in voice

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

    const list<S_msrNote>&
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
                            S_msrMeasureElement elem) override
                              { appendElementToMeasure (elem); }

    // lengthes

    string                fullMeasureWholeNotesDurationAsMsrString ();

    // backup and padding

    S_msrNote             createPaddingSkipNoteForVoice (
                            int             inputLineNumber,
                            const Rational& duration,
                            S_msrVoice      voice);

    void                  padUpToMeasurePositionInMeasure (
                            int             inputLineNumber,
                            const Rational& wholeNotes);

    void                  backupByWholeNotesStepLengthInMeasure (
                            int             inputLineNumber,
                            const Rational& backupTargetMeasureElementMeasurePosition);

    void                  appendPaddingSkipNoteToMeasure (
                            int             inputLineNumber,
                            const Rational& forwardStepLength);

    // print layout

    void                  appendPrintLayoutToMeasure (
                            S_msrPrintLayout printLayout);

    S_msrPrintLayout      getMeasurePrintLayout () const
                              { return fMeasurePrintLayout; }

    // clefs

    void                  appendClefToMeasure (S_msrClef clef);

    // keys

    void                  appendKeyToMeasure (S_msrKey key);

    // time signatures

    void                  appendTimeSignatureToMeasure (
                            S_msrTimeSignature timeSignature);

    void                  setFullMeasureWholeNotesDurationFromTime (
                            S_msrTimeSignature timeSignature);

    void                  appendTimeSignatureToMeasureClone (
                            S_msrTimeSignature timeSignature);

    // dal segno

    void                  insertHiddenMeasureAndBarLineInMeasureClone (
                            int             inputLineNumber,
                            const Rational& measurePosition);

    // transposition

    void                  appendTranspositionToMeasure (
                            S_msrTransposition transpose);

    // staff details

    void                  appendStaffDetailsToMeasure (
                            S_msrStaffDetails staffDetails);

    // bar number checks

    void                  appendBarNumberCheckToMeasure (
                            S_msrBarNumberCheck barNumberCheck);

    // breaks

    void                  appendLineBreakToMeasure (S_msrLineBreak lineBreak);
    void                  appendPageBreakToMeasure (S_msrPageBreak pageBreak);

    // tempo

    void                  appendTempoToMeasure (S_msrTempo tempo);

    // rehearsal marks

    void                  appendRehearsalMarkToMeasure (
                            S_msrRehearsalMark rehearsalMark);

    // octave shifts

    void                  appendOctaveShiftToMeasure (
                            S_msrOctaveShift octaveShift);

    // scordaturas

    void                  appendScordaturaToMeasure (
                            S_msrScordatura scordatura);

    // accordion registration

    void                  appendAccordionRegistrationToMeasure (
                            S_msrAccordionRegistration
                              accordionRegistration);

    // harp pedals tuning

    void                  appendHarpPedalsTuningToMeasure (
                            S_msrHarpPedalsTuning
                              harpPedalsTuning);

    // bar lines

    void                  appendBarLineToMeasure (S_msrBarLine barLine);
    void                  prependBarLineToMeasure (S_msrBarLine barLine);

    // bar checks

    void                  appendBarCheckToMeasure (S_msrBarCheck barCheck);

    // staff change

    void                  appendVoiceStaffChangeToMeasure (
                            S_msrVoiceStaffChange voiceStaffChange);

    // notes

    void                  appendNoteToMeasure (
                            S_msrNote       note,
                            const Rational& partCurrentMeasurePosition);

    void                  appendNoteOrPaddingToMeasure (
                            S_msrNote note);

    void                  accountForTupletMemberNoteDurationInMeasure (
                            S_msrNote note);

    void                  appendPaddingNoteAtTheEndOfMeasure (
                            S_msrNote paddingNote);

    void                  appendNoteToMeasureClone (S_msrNote note);

    // tremolos

    void                  appendDoubleTremoloToMeasure (
                            S_msrDoubleTremolo doubleTremolo);

     // chords

    void                  appendChordToMeasure (
                            S_msrChord chord);

    // tuplets

    void                  appendTupletToMeasure (
                            S_msrTuplet tuplet);

    // harmonies

    void                  appendHarmonyToMeasure (
                            S_msrHarmony harmony);

    void                  appendHarmonyToMeasureClone (
                            S_msrHarmony harmony);

    // frames

    void                  appendFrameToMeasure (
                            S_msrFrame frame);

    void                  appendFrameToMeasureClone (
                            S_msrFrame frame);

    // figured bass

    void                  appendFiguredBassToMeasure (
                            S_msrFiguredBass figuredBass);

    void                  appendFiguredBassToMeasureClone (
                            S_msrFiguredBass figuredBass);

    // grace notes

    /* JMI
    void                  addGraceNotesGroupAheadOfMeasure (
                            S_msrGraceNotesGroup graceNotesGroup);

    void                  appendGraceNotesToMeasure (
                            S_msrGraceNotes graceNotes);

    void                  appendAfterGraceNotesToMeasure (
                            S_msrAfterGraceNotes afterGraceNotes);

    void                  prependAfterGraceNotesToMeasure (
                            S_msrAfterGraceNotes afterGraceNotes);
                            */

    // segno

    void                  appendSegnoToMeasure (S_msrSegno segno);

    // coda

    void                  appendCodaToMeasure (S_msrCoda coda);

    // eyeglasses

    void                  appendEyeGlassesToMeasure (
                            S_msrEyeGlasses eyeGlasses);

    // pedal

    void                  appendPedalToMeasure (S_msrPedal pedal);

    // damp

    void                  appendDampToMeasure (S_msrDamp damp);

    // damp all

    void                  appendDampAllToMeasure (S_msrDampAll dampAll);

    // other elements

    void                  prependOtherElementToMeasure (
                            S_msrMeasureElement elem);
    void                  appendOtherElementToMeasure (
                            S_msrMeasureElement elem);

    // last element of measure

    S_msrElement          getLastElementOfMeasure () const
                              { return fMeasureElementsList.back (); }

     /* JMI
    S_msrElement          removeLastElementFromMeasure (
                            int inputLineNumber);
*/

    // removing elements from measure

    void                  removeNoteFromMeasure (
                            int       inputLineNumber,
                            S_msrNote note);

    void                  removeElementFromMeasure (
                            int          inputLineNumber,
                            S_msrElement elem);

    // notes flat list

    void                  appendNoteToMeasureNotesFlatList (
                            S_msrNote note);

    // finalization

    void                  determineMeasureKindAndPuristNumber (
                            int     inputLineNumber,
                            msrMeasureRepeatContextKind
                                    measureRepeatContextKind);

    void                  padUpToMeasurePosition (
                            int             inputLineNumber,
                            const Rational& measurePositionToPadUpTo);

    void                  padUpToPositionAtTheEndOfTheMeasure (
                            int             inputLineNumber,
                            const Rational& measurePositionToPadUpTo);

    void                  finalizeMeasure (
                            int                         inputLineNumber,
                            msrMeasureRepeatContextKind measureRepeatContextKind,
                            const string&               context);

    void                  finalizeMeasureClone (
                            int          inputLineNumber,
                            S_msrMeasure originalMeasure,
                            S_msrVoice   voiceClone);

  private:

    // private services
    // ------------------------------------------------------

    void                  appendElementToMeasure (
                            S_msrMeasureElement elem);

    void                  finalizeRegularMeasure (
                            int                         inputLineNumber,
                            msrMeasureRepeatContextKind measureRepeatContextKind,
                            const string&               context);

    void                  finalizeHarmonyMeasure (
                            int                         inputLineNumber,
                            msrMeasureRepeatContextKind measureRepeatContextKind,
                            const string&               context);

    void                  finalizeFiguredBassMeasure (
                            int                         inputLineNumber,
                            msrMeasureRepeatContextKind measureRepeatContextKind,
                            const string&               context);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortString () const override;
    string                asString () const override;

    string                asShortStringForMeasuresSlices () const override;

    void                  displayMeasure (
                            int           inputLineNumber,
                            const string& context);

    void                  print (ostream& os) const override;

    void                  printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks

    S_msrSegment          fMeasureUpLinkToSegment;

    // measure lengthes, in whole notes

    Rational              fFullMeasureWholeNotesDuration;
                            // meaningfull only
                            // when there is a time signature,
                            // but not for cadenzas

    // measure numbers

    string                fMeasureNumber;
    string                fNextMeasureNumber;

    int                   fMeasureOrdinalNumberInVoice;

    // purist measure number, forcing anacruses to start at '0' if it's not the case
    // and not shared among repeats components

    int                   fMeasurePuristNumber;

    // debug number, unique for every msrMeasure instance

    static int            gGlobalMeasureDebugNumber;
    int                   fMeasureDebugNumber;

    // measure end line number

    int                   fMeasureEndInputLineNumber;

    // first measure in voice?

    Bool                  fMeasureFirstInVoice;

    // measure print layout, MusicXML specific

    S_msrPrintLayout      fMeasurePrintLayout;

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

    // elements

    list<S_msrMeasureElement>
                          fMeasureElementsList;

    Bool                  fMeasureContainsMusic;

    // regular measure ends detection
    msrMeasureEndRegularKind
                          fMeasureEndRegularKind;

    // repeat context
    msrMeasureRepeatContextKind
                          fMeasureRepeatContextKind;

    // position in voice
    Rational              fMeasureVoicePosition;
    msrMoment             fMeasureVoiceMoment;

    // notes flat list
    // this is used to identify solo notes and rests in measures slices
    list<S_msrNote>       fMeasureNotesFlatList;

  private:

    // private work services
    // ------------------------------------------------------

    // harmonies
    void                  handleFirstHarmonyInHarmoniesMeasure (
                            int          inputLineNumber,
                            S_msrVoice   voice,
                            S_msrHarmony currentHarmony);

    void                  handleSubsequentHarmonyInHarmoniesMeasure (
                            int          inputLineNumber,
                            S_msrVoice   voice,
                            S_msrHarmony previousHarmony,
                            S_msrHarmony currentHarmony);

    void                  postHandleCurrentHarmonyInHarmoniesMeasure (
                            int          inputLineNumber,
                            S_msrVoice   voice,
                            S_msrHarmony currentHarmony);

    void                  finalizeTheHarmoniesInHarmoniesMeasure (
                            int           inputLineNumber,
                            const string& context);

    void                  finalizeHarmonyInHarmoniesMeasure (
                            int           inputLineNumber,
                            S_msrVoice    voice,
                            S_msrHarmony  previousHarmony,
                            S_msrHarmony  currentHarmony,
                            const string& context);

    // figurd bass
    void                  handleFirstFiguredBassInFiguredBassMeasure (
                            int          inputLineNumber,
                            S_msrVoice   voice,
                            list<S_msrMeasureElement>::iterator&
                                         i,
                            S_msrFiguredBass previousFiguredBass,
                            S_msrFiguredBass currentFiguredBass,
                            const Rational&         currentFiguredBassMeasurePosition);

    void                  handleSubsequentFiguredBassInFiguredBassMeasure (
                            int                     inputLineNumber,
                            S_msrVoice              voice,
                            list<S_msrMeasureElement>::iterator&
                                                    i,
                            S_msrFiguredBass previousFiguredBass,
                            S_msrFiguredBass currentFiguredBass,
                            const Rational&         currentFiguredBassMeasurePosition);

    void                  postHandleCurrentFiguredBassInFiguredBassMeasure (
                            int                     inputLineNumber,
                            S_msrVoice              voice,
                            S_msrFiguredBass currentFiguredBass);

    void                  finalizeFiguredBassesInFiguredBassMeasure (
                            int           inputLineNumber,
                            const string& context);

    // measure elements
    void                  insertElementInMeasureBeforeIterator (
                            int                                 inputLineNumber,
                            list<S_msrMeasureElement>::iterator iter,
                            S_msrMeasureElement                 elem);

    void                  insertElementInMeasureBeforeReverseIterator (
                            int                                 inputLineNumber,
                            list<S_msrMeasureElement>::reverse_iterator iter,
                            S_msrMeasureElement                 elem);

    void                  appendElementAtTheEndOfMeasure (
                            S_msrMeasureElement elem);

    void                  insertElementAtMeasurePosition (
                            int                 inputLineNumber,
                            const Rational&     measurePosition,
                            S_msrMeasureElement elem);

    void                  printMeasurePendingMeasureElementsList ();

  private:

    // private work fields
    // ------------------------------------------------------

    // measure lengthes, in whole notes

    Rational              fCurrentMeasureWholeNotesDuration;
                            // this increases when musical elements
                            // are appended to the measure

    string                currentMeasureWholeNotesDurationAsMsrString ();

    // pending measure elements, which may have to be delayed
    // because of <backup />
    list<S_msrMeasureElement>
                          fMeasurePendingMeasureElementsList;

    // measure finalization
    Bool                  fMeasureHasBeenFinalized;
    Bool                  fMeasureKindAndPuristNumberHaveBeenDetermined;

    string                fMeasureFinalizationContext;
};
typedef SMARTP<msrMeasure> S_msrMeasure;
EXP ostream& operator << (ostream& os, const S_msrMeasure& elt);


}


#endif
