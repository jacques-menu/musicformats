#ifndef ___msrMeasures___
#define ___msrMeasures___

#include "msrElements.h"

#include "msrBarChecks.h"
#include "msrBarLines.h"
#include "msrBarNumberChecks.h"
#include "msrBreaks.h"
#include "msrDoubleTremolos.h"
#include "msrEyeGlasses.h"
#include "msrFiguredBassElements.h"
#include "msrHarmonies.h"
#include "msrMusicXMLSpecifics.h"
#include "msrParts.h"
#include "msrPartGroups.h"
#include "msrNotes.h"
#include "msrRehearsalMarks.h"
#include "msrScores.h"
#include "msrSegments.h"
#include "msrVoiceStaffChanges.h"
#include "msrStaves.h"
#include "msrTempos.h"
#include "msrTuplets.h"
#include "msrVoices.h"


namespace MusicFormats
{

//______________________________________________________________________________
// pre-declaration
class msrMeasure;
typedef SMARTP<msrMeasure> S_msrMeasure;

//______________________________________________________________________________
class EXP msrMeasure : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrMeasureFirstInSegmentKind {
      kMeasureFirstInSegmentKindUnknown,
      kMeasureFirstInSegmentKindYes,
      kMeasureFirstInSegmentKindNo
    };

    static string measureFirstInSegmentKindAsString (
      msrMeasureFirstInSegmentKind measureFirstInSegmentKind);

    enum msrMeasureRepeatContextKind {
      kMeasureRepeatContextKindUnknown,
      kMeasureRepeatContextKindNone,
      kMeasureRepeatContextKindCommonPartLastMeasure,
      kMeasureRepeatContextKindHookedEndingLastMeasure,
      kMeasureRepeatContextKindHooklessEndingLastMeasure,
      kMeasureRepeatContextKindNextMeasureAfterCommonPart,
      kMeasureRepeatContextKindNextMeasureAfterHookedEnding,
      kMeasureRepeatContextKindNextMeasureAfterHooklessEnding
    };

    static string measureRepeatContextKindAsString (
      msrMeasureRepeatContextKind measureRepeatContextKind);

    enum msrMeasureEndRegularKind {
      kMeasureEndRegularKindUnknown,
      kMeasureEndRegularKindYes,
      kMeasureEndRegularKindNo
    };

    static string measureEndRegularKindAsString (
      msrMeasureEndRegularKind measureEndRegularKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasure> create (
                            int           inputLineNumber,
                            const string& measureNumber,
                            S_msrSegment  measureSegmentUpLink);

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
                            S_msrSegment  measureSegmentUpLink);

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

    S_msrSegment          getMeasureSegmentUpLink () const
                              { return fMeasureSegmentUpLink; }

    // measure numbers

    void                  setMeasureElementMeasureNumber (
                            const string& measureNumber)
                              { fMeasureElementMeasureNumber = measureNumber; }

    string                getMeasureElementMeasureNumber () const
                              { return fMeasureElementMeasureNumber; }

    void                  setMeasureOrdinalNumberInVoice (
                            int measureOrdinalNumber)
                              { fMeasureOrdinalNumberInVoice = measureOrdinalNumber; }

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
                            const rational& wholeNotes);

    rational              getFullMeasureWholeNotesDuration () const
                              {  return fFullMeasureWholeNotesDuration; }

    void                  setCurrentMeasureWholeNotesDuration (
                            int             inputLineNumber,
                            const rational& wholeNotes);
    void                  incrementCurrentMeasureWholeNotesDuration (
                            int             inputLineNumber,
                            const rational& wholeNotesDelta);

    rational              getCurrentMeasureWholeNotesDuration () const
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
                            const rational& duration);

    rational              getMeasureShortestNoteDuration () const
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

    void                  setMeasurePositionInVoice (
                            const rational& value)
                              { fMeasurePositionInVoice = value; }
//     void                  incrementMeasurePositionInVoice (
//                             const rational& wholeNotesDelta);

    rational              getMeasurePositionInVoice () const
                              { return fMeasurePositionInVoice; }

    void                  setMeasureMomentInVoice (
                            msrMoment value)
                              { fMeasureMomentInVoice = value; }

    const msrMoment&      getMeasureMomentInVoice () const
                              { return fMeasureMomentInVoice; }

    // notes flat list

    const list<S_msrNote>&
                          getMeasureNotesFlatList () const
                              { return fMeasureNotesFlatList; }

  public:

    // fetch
    // ------------------------------------------------------

    // upLinks

    S_msrVoice            fetchMeasureVoiceUpLink () const;

    S_msrStaff            fetchMeasureStaffUpLink () const;

    S_msrPart             fetchMeasurePartUpLink () const;

    S_msrPartGroup        fetchMeasurePartGroupUpLink () const;

    S_msrScore            fetchMeasureScoreUpLink () const;

  public:

    // public services
    // ------------------------------------------------------

    // lengthes

    string                fullMeasureWholeNotesDurationAsMsrString ();

    // backup and padding

    S_msrNote             createPaddingSkipNoteForVoice (
                            int             inputLineNumber,
                            const rational& duration,
                            S_msrVoice      voice);

    void                  padUpToPositionInMeasureInMeasure (
                            int             inputLineNumber,
                            const rational& wholeNotes);

    void                  backupByWholeNotesStepLengthInMeasure (
                            int             inputLineNumber,
                            const rational& backupTargetMeasureElementPositionInMeasure);

    void                  appendPaddingSkipNoteToMeasure (
                            int             inputLineNumber,
                            const rational& forwardStepLength);

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

    void                  appendTimeSignatureToMeasure (S_msrTimeSignature timeSignature);

    void                  setFullMeasureWholeNotesDurationFromTime (
                            S_msrTimeSignature timeSignature);

    void                  appendTimeSignatureToMeasureClone (S_msrTimeSignature timeSignature);

    // dal segno

    void                  insertHiddenMeasureAndBarLineInMeasureClone (
                            int             inputLineNumber,
                            const rational& positionInMeasure);

    // transpose

    void                  appendTransposeToMeasure (
                            S_msrTranspose transpose);

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
                            const rational& partCurrentPositionInMeasure);

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

    void                  appendFiguredBassElementToMeasure (
                            S_msrFiguredBassElement figuredBassElement);

    void                  appendFiguredBassElementToMeasureClone (
                            S_msrFiguredBassElement figuredBassElement);

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

    void                  padUpToPositionInMeasure (
                            int             inputLineNumber,
                            const rational& positionInMeasureToPadUpTo);

    void                  padUpToPositionAtTheEndOfTheMeasure (
                            int             inputLineNumber,
                            const rational& positionInMeasureToPadUpTo);

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

    void                  finalizeHarmoniesMeasure (
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

    S_msrSegment          fMeasureSegmentUpLink;

    // measure lengthes, in whole notes

    rational              fFullMeasureWholeNotesDuration;
                            // meaningfull only
                            // when there is a time signature,
                            // but not for cadenzas

    // measure numbers

    string                fMeasureElementMeasureNumber;
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
    rational              fMeasureShortestNoteDuration;
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

    // full-bar rest?

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
    rational              fMeasurePositionInVoice;
    msrMoment             fMeasureMomentInVoice;

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

    void                  finalizeHarmoniesInHarmoniesMeasure (
                            int           inputLineNumber,
                            const string& context);

    void                  finalizeHarmonyInHarmoniesMeasure (
                            int           inputLineNumber,
                            S_msrVoice    voice,
                            S_msrHarmony  previousHarmony,
                            S_msrHarmony  currentHarmony,
                            const string& context);

    // figurd bass
    void                  handleFirstFiguredBassElementInFiguredBassMeasure (
                            int          inputLineNumber,
                            S_msrVoice   voice,
                            list<S_msrMeasureElement>::iterator&
                                         i,
                            S_msrFiguredBassElement previousFiguredBass,
                            S_msrFiguredBassElement currentFiguredBass,
                            const rational&         currentFiguredBassPositionInMeasure);

    void                  handleSubsequentFiguredBassElementInFiguredBassMeasure (
                            int                     inputLineNumber,
                            S_msrVoice              voice,
                            list<S_msrMeasureElement>::iterator&
                                                    i,
                            S_msrFiguredBassElement previousFiguredBass,
                            S_msrFiguredBassElement currentFiguredBass,
                            const rational&         currentFiguredBassPositionInMeasure);

    void                  postHandleCurrentFiguredBassElementInFiguredBassMeasure (
                            int                     inputLineNumber,
                            S_msrVoice              voice,
                            S_msrFiguredBassElement currentFiguredBass);

    void                  finalizeFiguredBassElementsInFiguredBassMeasure (
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

    void                  insertElementAtPositionInMeasure (
                            int                 inputLineNumber,
                            const rational&     positionInMeasure,
                            S_msrMeasureElement elem);

    void                  printMeasurePendingMeasureElementsList ();

  private:

    // private work fields
    // ------------------------------------------------------

    // measure lengthes, in whole notes

    rational              fCurrentMeasureWholeNotesDuration;
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
EXP ostream& operator<< (ostream& os, const S_msrMeasure& elt);


}


#endif
