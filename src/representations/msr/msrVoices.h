#ifndef ___msrVoices___
#define ___msrVoices___

#include <stack>

#include "msrBeatRepeats.h"
#include "msrChords.h"
#include "msrClefs.h"
#include "msrCodas.h"
#include "msrElements.h"
#include "msrEyeGlasses.h"
#include "msrKeys.h"
#include "msrLyrics.h"
#include "msrMeasures.h"
#include "msrMeasureElements.h"
#include "msrMeasureRepeats.h"
#include "msrNotes.h"
#include "msrParts.h"
#include "msrPartGroups.h"
#include "msrRepeats.h"
#include "msrFullBarRests.h"
#include "msrScores.h"
#include "msrSegments.h"
#include "msrSegnos.h"
#include "msrStaves.h"
#include "msrStavesDetails.h"
#include "msrTimeSignatures.h"
#include "msrTranspositions.h"
#include "msrTuplets.h"


namespace MusicFormats
{

//______________________________________________________________________________
class msrStaff;
typedef SMARTP<msrStaff> S_msrStaff;

class msrFullBarRests;
typedef SMARTP<msrFullBarRests> S_msrFullBarRests;

class msrVoiceStaffChange;
typedef SMARTP<msrVoiceStaffChange> S_msrVoiceStaffChange;

class msrBeatRepeat;
typedef SMARTP<msrBeatRepeat> S_msrBeatRepeat;

class msrMeasureRepeat;
typedef SMARTP<msrMeasureRepeat> S_msrMeasureRepeat;

class msrRepeatCommonPart;
typedef SMARTP<msrRepeatCommonPart> S_msrRepeatCommonPart;

class msrRepeat;
typedef SMARTP<msrRepeat> S_msrRepeat;

class msrRepeatEnding;
typedef SMARTP<msrRepeatEnding> S_msrRepeatEnding;

class msrRepeatDescr;
typedef SMARTP<msrRepeatDescr> S_msrRepeatDescr;

class msrDoubleTremolo;
typedef SMARTP<msrDoubleTremolo> S_msrDoubleTremolo;

class msrGraceNotesGroup;
typedef SMARTP<msrGraceNotesGroup> S_msrGraceNotesGroup;

class msrFrame;
typedef SMARTP<msrFrame> S_msrFrame;

class msrHarmony;
typedef SMARTP<msrHarmony> S_msrHarmony;

class msrFiguredBassElement;
typedef SMARTP<msrFiguredBassElement> S_msrFiguredBassElement;

class msrTempo;
typedef SMARTP<msrTempo> S_msrTempo;

class msrRehearsalMark;
typedef SMARTP<msrRehearsalMark> S_msrRehearsalMark;

class msrLineBreak;
typedef SMARTP<msrLineBreak> S_msrLineBreak;

class msrPageBreak;
typedef SMARTP<msrPageBreak> S_msrPageBreak;

class msrStanza;
typedef SMARTP<msrStanza> S_msrStanza;

class msrSyllable;
typedef SMARTP<msrSyllable> S_msrSyllable;

//______________________________________________________________________________

// data types
// ------------------------------------------------------

enum class msrVoiceRepeatPhaseKind {
  kVoiceRepeatPhaseNone,
  kVoiceRepeatPhaseAfterCommonPart,
  kVoiceRepeatPhaseAfterHookedEnding,
  kVoiceRepeatPhaseAfterHooklessEnding
};

extern string voiceRepeatPhaseKindAsString (
  msrVoiceRepeatPhaseKind afterRepeatComponentPhaseKind);

ostream& operator<< (ostream& os, msrVoiceRepeatPhaseKind& elt);

enum class msrVoiceFinalizationStatusKind { // JMI ???
  kKeepVoice,
  kEraseVoice
};

extern string voiceFinalizationStatusKindAsString (
  msrVoiceFinalizationStatusKind voiceFinalizationStatusKind);

ostream& operator<< (ostream& os, msrVoiceFinalizationStatusKind& elt);

enum class msrVoiceCreateInitialLastSegmentKind {
  kCreateInitialLastSegmentYes,
  kCreateInitialLastSegmentNo
};

extern string voiceFinalizationStatusKindAsString (
  msrVoiceCreateInitialLastSegmentKind voiceCreateInitialLastSegmentKind);

ostream& operator<< (ostream& os, msrVoiceCreateInitialLastSegmentKind& elt);

//________________________________________________________________________
class EXP msrRepeatDescr : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrRepeatDescr> create (
                            int         repeatDescrStartInputLineNumber,
                            S_msrRepeat repeatDescrRepeat);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrRepeatDescr (
                            int         repeatDescrStartInputLineNumber,
                            S_msrRepeat fRepeatDescrRepeat);

    virtual               ~msrRepeatDescr ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrRepeat           getRepeatDescrRepeat () const
                              { return fRepeatDescrRepeat; }

    void                  setRepeatDescrStartInputLineNumber (
                            int inputLineNumber)
                              {
                                fRepeatDescrStartInputLineNumber =
                                  inputLineNumber;
                              }

    int                   getRepeatDescrStartInputLineNumber () const
                              {
                                return
                                  fRepeatDescrStartInputLineNumber;
                              }

  public:

    // public services
    // ------------------------------------------------------

    string                repeatDescrAsString () const;

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // the repeat
    S_msrRepeat           fRepeatDescrRepeat;

    // its start input line number
    int                   fRepeatDescrStartInputLineNumber;
};
typedef SMARTP<msrRepeatDescr> S_msrRepeatDescr;
EXP ostream& operator<< (ostream& os, const S_msrRepeatDescr& elt);

//______________________________________________________________________________
class EXP msrVoice : public msrElement
{
  public:

    // constants
    // ------------------------------------------------------

    static const int K_NO_VOICE_NUMBER;
    static const int K_VOICE_HARMONIES_VOICE_BASE_NUMBER;
    static const int K_VOICE_FIGURED_BASS_VOICE_BASE_NUMBER;

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrVoice> create (
                            int          inputLineNumber,
                            msrVoiceKind voiceKind,
                            int          voiceNumber,
                            msrVoiceCreateInitialLastSegmentKind
                                         voiceCreateInitialLastSegmentKind,
                            S_msrStaff   voiceStaffUpLink);

    SMARTP<msrVoice> createVoiceNewbornClone (
                            S_msrStaff containingStaff);

    SMARTP<msrVoice> createVoiceDeepClone (
                            int          inputLineNumber,
                            msrVoiceKind voiceKind,
                            int          voiceNumber,
                            S_msrStaff   containingStaff);

    // creation from the API
    // ------------------------------------------------------

    static SMARTP<msrVoice> createRegularVoice (
                            int        inputLineNumber,
                            int        voiceNumber,
                            S_msrStaff voiceStaffUpLink);

    static SMARTP<msrVoice> createHarmoniesVoice (
                            int        inputLineNumber,
                            int        voiceNumber,
                            S_msrStaff voiceStaffUpLink);

    static SMARTP<msrVoice> createFiguredBassVoice (
                            int        inputLineNumber,
                            int        voiceNumber,
                            S_msrStaff voiceStaffUpLink);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    // for regular voices
                          msrVoice (
                            int          inputLineNumber,
                            msrVoiceKind voiceKind,
                            int          voiceNumber,
                            msrVoiceCreateInitialLastSegmentKind
                                         voiceCreateInitialLastSegmentKind,
                            S_msrStaff   voiceStaffUpLink);

                          msrVoice (
                            int          inputLineNumber,
                            msrVoiceKind voiceKind,
                            int          voiceNumber);

    // destructor
    virtual               ~msrVoice ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeVoice (
                            msrVoiceCreateInitialLastSegmentKind
                             voiceCreateInitialLastSegmentKind);

  public:

    // set and get
    // ------------------------------------------------------

    // uplink

    S_msrStaff            getVoiceStaffUpLink () const
                              { return fVoiceStaffUpLink; }

    // voice kind

    void                  setVoiceKind (msrVoiceKind voiceKind)
                            { fVoiceKind = voiceKind; }

    msrVoiceKind          getVoiceKind () const
                              { return fVoiceKind; }

    // voice numbers

    void                  setVoiceNumber (int voiceNumber)
                              { fVoiceNumber = voiceNumber; }

    int                   getVoiceNumber () const
                              { return fVoiceNumber; }

    void                  setRegularVoiceStaffSequentialNumber (
                            int regularVoiceStaffSequentialNumber);

    int                   getRegularVoiceStaffSequentialNumber () const
                              { return fRegularVoiceStaffSequentialNumber; }

    // voice name

    void                  setVoiceNameFromNumber (
                            int inputLineNumber,
                            int voiceNumber);

    string                getVoiceName () const
                              { return fVoiceName; }

    // harmonies

// JMI
    S_msrVoice            getRegularVoiceHarmoniesVoiceForwardLink () const
                              { return fRegularVoiceHarmoniesVoiceForwardLink; }

    S_msrVoice            getHarmoniesVoiceRegularVoiceBackwardLink () const
                              { return fHarmoniesVoiceRegularVoiceBackwardLink; }

    // figured bass

    S_msrVoice            getRegularVoiceFiguredBassVoiceForwardLink () const
                              { return fRegularVoiceFiguredBassVoiceForwardLink; }

    S_msrVoice            getFiguredBassVoiceRegularVoiceBackwardLink () const
                              { return fFiguredBassVoiceRegularVoiceBackwardLink; }
//

    // stanzas

    const map<string, S_msrStanza>&
                          getVoiceStanzasMap () const
                              { return fVoiceStanzasMap; }

    // voice last segment

    void                  setVoiceLastSegmentInVoiceClone (
                            S_msrSegment segment);

    void                  appendSegmentToVoiceClone (
                            S_msrSegment segment);

    S_msrSegment          getVoiceLastSegment () const
                              { return fVoiceLastSegment; }

    // voice last appended note

    S_msrNote             getVoiceLastAppendedNote () const
                              { return fVoiceLastAppendedNote; }

    // voice shortest note

    void                  setVoiceShortestNoteDuration (
                            const rational& duration);

    rational              getVoiceShortestNoteDuration () const
                              { return fVoiceShortestNoteDuration; }

    void                  setVoiceShortestNoteTupletFactor (
                            const msrTupletFactor& noteTupletFactor);

    const msrTupletFactor&
                          getVoiceShortestNoteTupletFactor () const
                              { return fVoiceShortestNoteTupletFactor; }

    // counters

    int                   getVoiceActualNotesCounter () const
                              { return fVoiceActualNotesCounter; }

    int                   getVoiceRestsCounter () const
                              { return fVoiceRestsCounter; }

    int                   getVoiceSkipsCounter () const
                              { return fVoiceSkipsCounter; }

    int                   getVoiceActualHarmoniesCounter () const
                              { return fVoiceActualHarmoniesCounter; }

    int                   getVoiceActualFiguredBassElementsCounter () const
                              { return fVoiceActualFiguredBassElementsCounter; }

    // has music been inserted in the voice?

    void                  setMusicHasBeenInsertedInVoice ()
                              { fMusicHasBeenInsertedInVoice = true; }

    Bool                  getMusicHasBeenInsertedInVoice () const
                              { return fMusicHasBeenInsertedInVoice; }

    // regular measure ends detection

    void                  setWholeNotesSinceLastRegularMeasureEnd (
                            int             inputLineNumber,
                            const rational& value);

    rational              getWholeNotesSinceLastRegularMeasureEnd () const
                              { return fWholeNotesSinceLastRegularMeasureEnd; }

    // incomplete measures after repeats detection

    void                  setCurrentVoiceRepeatPhaseKind (
                            int      inputLineNumber,
                            msrVoiceRepeatPhaseKind
                                     afterRepeatComponentPhaseKind);

    msrVoiceRepeatPhaseKind
                          getCurrentVoiceRepeatPhaseKind () const
                              { return fCurrentVoiceRepeatPhaseKind; }


    // rests measures

    void                  setVoiceContainsFullBarRests (
                            int inputLineNumber);

    Bool                  getVoiceContainsFullBarRests () const
                              { return fVoiceContainsFullBarRests; }

    // measure repeats

    void                  setVoiceContainsMeasureRepeats (
                            int inputLineNumber);

    Bool                  getVoiceContainsMeasureRepeats () const
                              { return fVoiceContainsMeasureRepeats; }

    // measures flat list

    const list<S_msrMeasure>&
                          getVoiceMeasuresFlatList () const
                              { return fVoiceMeasuresFlatList; }

    // measures slices sequence

    S_msrMeasuresSlicesSequence
                          getVoiceMeasuresSlicesSequence () const
                              { return fVoiceMeasuresSlicesSequence; }

  public:

    // public services
    // ------------------------------------------------------

    // upLinks

    S_msrPart             fetchVoicePartUpLink () const;

    S_msrPartGroup        fetchVoicePartGroupUpLink () const;

    S_msrScore            fetchVoiceScoreUpLink () const;

    // identity

    void                  changeVoiceIdentity ( // after a deep clone
                            int voiceNumber);

    // print layout MusicXML specific??? JMI
     void                 appendPrintLayoutToVoice (
                            S_msrPrintLayout printLayout);

    // measures

    S_msrMeasure          createAMeasureAndAppendItToVoice (
                            int           inputLineNumber,
                            int           previousMeasureEndInputLineNumber,
                            const string& measureNumber,
                            msrMeasureImplicitKind
                                          measureImplicitKind);

    void                  setNextMeasureNumberInVoice (
                            int           inputLineNumber,
                            const string& nextMeasureNumber);

    // forward

    void                  appendPaddingNoteToVoice (
                            int             inputLineNumber,
                            const rational& forwardStepLength);

    // backup and padding

    void                  padUpToPositionInMeasureInVoice (
                            int             inputLineNumber,
                            const rational& wholeNotesPositionInMeasure);

    void                  backupByWholeNotesStepLengthInVoice (
                            int             inputLineNumber,
                            const rational& backupTargetMeasureElementPositionInMeasure);

    // clef, key, time signature

    void                  appendClefToVoice (S_msrClef clef);

    void                  appendKeyToVoice  (S_msrKey  key);

    void                  appendTimeSignatureToVoice (
                            S_msrTimeSignature timeSignature);
    void                  appendTimeSignatureToVoiceClone (
                            S_msrTimeSignature timeSignature);

    // dal segno

    void                  insertHiddenMeasureAndBarLineInVoiceClone (
                            int             inputLineNumber,
                            const rational& positionInMeasure);

    // notes

    S_msrNote             fetchVoiceFirstNonGraceNote () const;

    void                  registerShortestNoteInVoiceIfRelevant (S_msrNote note);

    void                  registerNoteAsVoiceLastAppendedNote (S_msrNote note);

    // harmonies

    S_msrVoice            createRegularVoiceHarmoniesVoice (
                            int           inputLineNumber,
                            const string& currentMeasureNumber);

    // figured bass

    S_msrVoice            createRegularVoiceFiguredBassVoice (
                            int           inputLineNumber,
                            const string& currentMeasureNumber);

    // transpose

    void                  appendTransposeToVoice (
                            S_msrTranspose transpose);

    // staff details

    void                  appendStaffDetailsToVoice (
                            S_msrStaffDetails staffDetails);

    // tempo

    void                  appendTempoToVoice (S_msrTempo tempo);

    // rehearsal marks

    void                  appendRehearsalMarkToVoice (
                            S_msrRehearsalMark rehearsalMark);

    // octave shifts

    void                  appendOctaveShiftToVoice (
                            S_msrOctaveShift octaveShift);

    // scordaturas

    void                  appendScordaturaToVoice (
                            S_msrScordatura scordatura);

    // accordion registration

    void                  appendAccordionRegistrationToVoice (
                            S_msrAccordionRegistration
                              accordionRegistration);

    // harp pedals tuning

    void                  appendHarpPedalsTuningToVoice (
                            S_msrHarpPedalsTuning
                              harpPedalsTuning);

    // staff change

    void                  appendVoiceStaffChangeToVoice (
                            S_msrVoiceStaffChange voiceStaffChange);

    // notes

    void                  appendNoteToVoice      (S_msrNote note);
    void                  appendNoteToVoiceClone (S_msrNote note);

    // tremolos

    void                  appendDoubleTremoloToVoice (
                            S_msrDoubleTremolo doubleTremolo);

    // chords

    void                  appendChordToVoice  (S_msrChord chord);

    // tuplets

    void                  appendTupletToVoice (S_msrTuplet tuplet);

    // harmonies

    void                  appendHarmonyToVoice (
                            S_msrHarmony harmony);

    void                  appendHarmonyToVoiceClone (
                            S_msrHarmony harmony);

    // figured bass

    void                  appendFiguredBassElementToVoice (
                            S_msrFiguredBassElement figuredBassElement);

    void                  appendFiguredBassElementToVoiceClone (
                            S_msrFiguredBassElement figuredBassElement);

    // frames

    void                  appendFrameToVoice (
                            S_msrFrame frame);

    void                  appendFrameToVoiceClone (
                            S_msrFrame frame);

    // grace notes

    void                  addGraceNotesGroupBeforeAheadOfVoiceIfNeeded (
                            S_msrGraceNotesGroup graceNotesGroup);

    /*
    void                  appendGraceNotesToVoice (
                            S_msrGraceNotes graceNotes);

    void                  appendAfterGraceNotesToVoice (
                            S_msrAfterGraceNotes afterGraceNotes);

    void                  prependAfterGraceNotesToVoice (
                            S_msrAfterGraceNotes afterGraceNotes);
                            */

    // lyrics

    void                  appendSyllableToVoice ( // JMI
                            int           inputLineNumber,
                            const string& stanzaNumber,
                            const string& stanzaName,
                            S_msrSyllable syllable);

    // bar checks

    void                  appendBarCheckToVoice (
                            S_msrBarCheck barCheck);

    // bar number checks

    void                  appendBarNumberCheckToVoice (
                            S_msrBarNumberCheck barNumberCheck);

    // breaks

    void                  appendLineBreakToVoice (S_msrLineBreak lineBreak);
    void                  appendPageBreakToVoice (S_msrPageBreak pageBreak);

    // bar lines

    void                  prependBarLineToVoice (S_msrBarLine barLine);
    void                  appendBarLineToVoice (S_msrBarLine barLine);

    // segno

    void                  appendSegnoToVoice (S_msrSegno segno);

    // coda

    void                  appendCodaToVoice (S_msrCoda coda);

    // eyeglasses

    void                  appendEyeGlassesToVoice (
                            S_msrEyeGlasses eyeGlasses);

    // pedal

    void                  appendPedalToVoice (S_msrPedal pedal);

    // damp

    void                  appendDampToVoice (S_msrDamp damp);

    // damp all

    void                  appendDampAllToVoice (S_msrDampAll dampAll);

    // other elements

    void                  prependOtherElementToVoice (
                            S_msrMeasureElement elem);
    void                  appendOtherElementToVoice (
                            S_msrMeasureElement elem);
                            // for other types of elements not known
                            // in this header file, such as LPSR elements

    // last measure in voice

    S_msrMeasure          fetchVoiceLastMeasure (
                            int inputLineNumber) const;

    // last element in voice

    S_msrMeasureElement   fetchVoiceLastElement ( // JMI ???
                            int inputLineNumber) const;

    // removing elements from voice

    void                  removeNoteFromVoice (
                            int       inputLineNumber,
                            S_msrNote note);

    void                  removeElementFromVoice (
                            int          inputLineNumber,
                            S_msrElement element);

    S_msrMeasure          removeLastMeasureFromVoice (
                            int inputLineNumber);

    // segments

    void                  createNewLastSegmentForVoice (
                            int           inputLineNumber,
                            const string& context);

    void                  createNewLastSegmentFromItsFirstMeasureForVoice (
                            int           inputLineNumber,
                            S_msrMeasure  firstMeasure,
                            const string& context);

    // repeats

    void                  handleRepeatEndInVoice (
                            int           inputLineNumber,
                            const string& measureNumber,
                            int           repeatTimes);

    void                  handleRepeatEndingStartInVoice (
                            int inputLineNumber);

    void                  handleRepeatEndingEndInVoice (
                            int           inputLineNumber,
                            const string& repeatEndingNumber, // may be "1, 2"
                            msrRepeatEndingKind
                                          repeatEndingKind);

    void                  handleRepeatCommonPartStartInVoiceClone (
                            int inputLineNumber);

    void                  handleRepeatCommonPartEndInVoiceClone (
                            int inputLineNumber);

    void                  handleRepeatEndingStartInVoiceClone (
                            int           inputLineNumber,
                            msrRepeatEndingKind
                                          repeatEndingKind,
                            const string& repeatEndingNumber); // may be "1, 2"

    void                  handleRepeatEndingEndInVoiceClone (
                            int           inputLineNumber,
                            const string& repeatEndingNumber, // may be "1, 2"
                            msrRepeatEndingKind
                                          repeatEndingKind);

    void                  handleSegmentCloneEndInVoiceClone (
                            int          inputLineNumber,
                            S_msrSegment segmentClone);

/* JMI
    void                  finalizeRepeatEndInVoice (
                            int    inputLineNumber,
                            const string& measureNumber,
                            int    repeatTimes);
                            */

    void                  handleRepeatStartInVoice (
                            int inputLineNumber);

    void                  handleRepeatStartInVoiceClone (
                            int         inputLineNumber,
                            S_msrRepeat repeat);

    void                  handleRepeatEndInVoiceClone (
                            int inputLineNumber);

    void                  appendRepeatCloneToVoiceClone (
                            int         inputLineNumber,
                            S_msrRepeat repeatCLone);

    void                  appendRepeatEndingCloneToVoice (
                            S_msrRepeatEnding repeatEndingClone);

    // full-bar rests

    void                  appendFullBarRestsToVoiceElementsList (
                            S_msrFullBarRests fullBarRests);

    void                  createFullBarRestsInVoice (
                            int inputLineNumber,
                            int fullBarRestsNumber);

    void                  appendFullBarRestsToVoice (
                            int                   inputLineNumber,
                            S_msrFullBarRests fullBarRests);

    void                  replicateLastAppendedMeasureInVoice (
                            int inputLineNumber,
                            int replicatasNumber);

    void                  addEmptyMeasuresToVoice (
                            int           inputLineNumber,
                            const string& previousMeasureNumber,
                            int           fullBarRestsNumber);

    void                  appendPendingFullBarRestsToVoice (
                            int inputLineNumber);

    void                  handleFullBarRestsStartInVoiceClone (
                            int                   inputLineNumber,
                            S_msrFullBarRests fullBarRests);

    void                  handleFullBarRestsEndInVoiceClone (
                            int inputLineNumber);

    void                  handleFullBarRestsContentsStartInVoiceClone (
                            int inputLineNumber);

    void                  handleFullBarRestsContentsEndInVoiceClone (
                            int inputLineNumber);

    void                  appendFullBarRestsCloneToVoiceClone ( // JMI ???
                            int                   inputLineNumber,
                            S_msrFullBarRests fullBarRestsClone);

    // measure repeats

    void                  appendMeasureRepeatToVoiceElementsList (
                            S_msrMeasureRepeat beatRepeat);

    void                  appendMeasureRepeatReplicaToVoice (
                            int inputLineNumber);

    void                  createMeasureRepeatFromItsFirstMeasures (
                            int inputLineNumber,
                            int beatRepeatMeasuresNumber,
                            int beatRepeatSlashesNumber);

    void                  appendMeasureRepeatToVoice (
                            int                inputLineNumber,
                            S_msrMeasureRepeat beatRepeat);

    void                  appendPendingMeasureRepeatToVoice (
                            int inputLineNumber);

    void                  createMeasureRepeatAndAppendItToVoiceClone (
                            int inputLineNumber,
                            int beatRepeatMeasuresNumber,
                            int beatRepeatSlashesNumber);

    void                  handleMeasureRepeatStartInVoiceClone (
                            int                inputLineNumber,
                            S_msrMeasureRepeat beatRepeat);

    void                  handleMeasureRepeatEndInVoiceClone (
                            int inputLineNumber);

    void                  handleMeasureRepeatPatternStartInVoiceClone (
                            int inputLineNumber);

    void                  handleMeasureRepeatPatternEndInVoiceClone (
                            int inputLineNumber);

    void                  handleMeasureRepeatReplicasStartInVoiceClone (
                            int inputLineNumber);

    void                  handleMeasureRepeatReplicasEndInVoiceClone (
                            int inputLineNumber);

    void                  appendMeasureRepeatCloneToVoiceClone ( // JMI ???
                            int                inputLineNumber,
                            S_msrMeasureRepeat beatRepeatClone);

    // stanzas

    S_msrStanza           addStanzaToVoiceByItsNumber (
                            int           inputLineNumber,
                            const string& stanzaNumber);

    void                  addStanzaToVoiceWithoutCatchUp ( // JMI
                            S_msrStanza stanza);

    void                  addStanzaToVoice (
                            S_msrStanza stanza);

    S_msrStanza           createStanzaInVoiceIfNotYetDone (
                            int           inputLineNumber,
                            const string& stanzaNumber,
                            const string& stanzaName);

    S_msrStanza           fetchStanzaInVoice (
                            int           inputLineNumber,
                            const string& stanzaNumber,
                            const string& stanzaName);

    // strings

    // position in voice

    void                  incrementCurrentPositionInVoice (
                            const rational& delta)
                              {
                                fCurrentPositionInVoice += delta;
                                fCurrentPositionInVoice.rationalise ();
                              }

    rational              getCurrentPositionInVoice () const
                              { return fCurrentPositionInVoice; }

    const msrMoment&      getCurrentMomentInVoice () const
                              { return fCurrentMomentInVoice; }

    // beams begin, continue and end check

    void                  checkBeamNumber (S_msrBeam beam, S_msrNote note);

    // finalization

    void                  finalizeLastAppendedMeasureInVoice (
                            int inputLineNumber);

    void                  finalizeVoice (
                            int inputLineNumber);

    void                  finalizeVoiceAndAllItsMeasures (
                            int inputLineNumber);

    void                  collectVoiceMeasuresIntoFlatList (
                            int inputLineNumber);

    void                  collectVoiceMeasuresIntoMeasuresSlicesSequence (
                            int inputLineNumber);

  private:

    // private services
    // ------------------------------------------------------

    // voice last segment

    void                  appendVoiceLastSegmentToInitialVoiceElements (
                            int           inputLineNumber,
                            const string& context);

    void                  moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
                            int           inputLineNumber,
                            const string& context);

    // harmonies

    void                  appendHarmonyToHarmoniesVoice (
                            S_msrHarmony harmony);

    // figured bass elements

    void                  appendFiguredBassElementToFiguredBassVoice (
                            S_msrFiguredBassElement figuredBassElement);

    // repeats

    S_msrRepeat           createARepeatAndStackIt (
                            int           inputLineNumber,
                            const string& context);

    S_msrRepeat           createARepeatCloneAndStackIt (
                            int           inputLineNumber,
                            S_msrRepeat   repeat,
                            const string& context);

    void                  popRepeatFromRepeatDescrsStack (
                            int           inputLineNumber,
                            S_msrRepeat   repeat,
                            const string& context);

    void                  pushRepeatOntoRepeatDescrsStack (
                            int           inputLineNumber,
                            S_msrRepeat   repeat,
                            const string& context);

    void                  appendRepeatToInitialVoiceElements (
                            int           inputLineNumber,
                            S_msrRepeat   repeat,
                            const string& context);

    void                  appendRepeatCloneToInitialVoiceElements (
                            int           inputLineNumber,
                            S_msrRepeat   repeatCLone,
                            const string& context);

    void                  handleVoiceLevelRepeatStart (
                            int inputLineNumber);

    void                  handleNestedRepeatStartInVoice (
                            int inputLineNumber);

    void                  handleVoiceLevelRepeatEndWithoutStart (
                            int           inputLineNumber,
                            const string& measureNumber,
                            int           repeatTimes);

    void                  handleVoiceLevelContainingRepeatEndWithoutStart (
                            int           inputLineNumber,
                            const string& measureNumber,
                            int           repeatTimes);

    void                  handleVoiceLevelRepeatEndWithStart (
                            int           inputLineNumber,
                            const string& measureNumber,
                            int           repeatTimes);

    void                  handleVoiceLevelRepeatEndingStartWithoutExplicitStart (
                            int         inputLineNumber,
                            S_msrRepeat currentRepeat);

    void                  handleVoiceLevelRepeatEndingStartWithExplicitStart (
                            int inputLineNumber);

    void                  nestContentsIntoNewRepeatInVoice (
                            int inputLineNumber);

    void                  handleNestedRepeatEndingStartInVoice (
                            int inputLineNumber);

    void                  handleNestedRepeatEndInVoice (
                            int           inputLineNumber,
                            const string& measureNumber,
                            int           repeatTimes);

    void                  moveVoiceInitialElementsToRepeatCommonPart (
                            int                   inputLineNumber,
                            S_msrRepeatCommonPart repeatCommonPart,
                            const string&         context);

    void                  moveVoiceLastSegmentToRepeatCommonPart (
                            int                   inputLineNumber,
                            S_msrRepeatCommonPart repeatCommonPart,
                            const string&         context);

    void                  moveAllVoiceContentsToRepeatCommonPart (
                            int                   inputLineNumber,
                            S_msrRepeatCommonPart repeatCommonPart,
                            const string&         context);

    void                  moveVoiceLastSegmentToRepeatEnding (
                            int               inputLineNumber,
                            S_msrRepeatEnding repeatEnding,
                            const string&     context);

    void                  handleHookedRepeatEndingEndInVoice (
                            int           inputLineNumber,
                            const string& repeatEndingNumber); // may be "1, 2"

    void                  handleHooklessRepeatEndingEndInVoice (
                            int           inputLineNumber,
                            const string& repeatEndingNumber); // may be "1, 2"

    void                  handleHookedRepeatEndingEndInVoiceClone (
                            int           inputLineNumber,
                            const string& repeatEndingNumber); // may be "1, 2"

    void                  handleHooklessRepeatEndingEndInVoiceClone (
                            int           inputLineNumber,
                            const string& repeatEndingNumber); // may be "1, 2"

    // full-bar rests

    void                  appendFullBarRestsToInitialVoiceElements (
                            int                   inputLineNumber,
                            S_msrFullBarRests fullBarRests,
                            const string&         context);

    // measure repeats

    void                  appendMeasureRepeatToInitialVoiceElements (
                            int                inputLineNumber,
                            S_msrMeasureRepeat measureRepeat,
                            const string&      context);

    // beats repeats

    void                  appendBeatRepeatToInitialVoiceElements (
                            int             inputLineNumber,
                            S_msrBeatRepeat beatRepeat,
                            const string&   context);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                voiceNumberAsString () const;

    string                regularVoiceStaffSequentialNumberAsString () const;

    string                asShortString () const override;
    string                asString () const override;

    virtual void          displayVoice (
                            int           inputLineNumber,
                            const string& context) const;

    void                  displayVoiceRepeatsStackFullBarRestsMeasureRepeatAndVoice (
                            int           inputLineNumber,
                            const string& context);

    void                  print (ostream& os) const override;

    void                  printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks

    S_msrStaff            fVoiceStaffUpLink;

// JMI
    // two-way links
    S_msrVoice            fRegularVoiceHarmoniesVoiceForwardLink;
    S_msrVoice            fHarmoniesVoiceRegularVoiceBackwardLink;

    S_msrVoice            fRegularVoiceFiguredBassVoiceForwardLink;
    S_msrVoice            fFiguredBassVoiceRegularVoiceBackwardLink;
//

    // voice kind

    msrVoiceKind          fVoiceKind;

    // musically empty voices

    Bool                  fMusicHasBeenInsertedInVoice;

    // regular measure ends detection

    rational              fWholeNotesSinceLastRegularMeasureEnd;

     // stanzas

    map<string, S_msrStanza>
                          fVoiceStanzasMap;

    // measures

    // fVoiceFirstMeasure is used to number voice upbeats as measure 0
    S_msrMeasure          fVoiceFirstMeasure;

    // fVoiceCurrentMeasure contains the last measure appended to the voice
    S_msrMeasure          fVoiceLastAppendedMeasure;

    // measures flat list
    // i.e. without segments nor repeats,
    // gathered from fVoiceInitialElementsList and fVoiceLastSegment
    // by finalizeMeasure()
    list<S_msrMeasure>    fVoiceMeasuresFlatList;

    // measures slices sequence
    S_msrMeasuresSlicesSequence
                          fVoiceMeasuresSlicesSequence;

    // notes
    // fVoiceLastAppendedNote is used to build chords upon their second note
    S_msrNote             fVoiceLastAppendedNote;

    // part shortest note

    // fVoiceShortestNoteDuration and fVoiceShortestNoteTupletFactor
    // are used to compute a number of divisions per quarter note
    // if needed, such as when generating MusicXML from MSR
    rational              fVoiceShortestNoteDuration;
    msrTupletFactor       fVoiceShortestNoteTupletFactor;

    // repeats

  protected:

    // protected fields
    // ------------------------------------------------------

    // voice numbers

    // voice numbers in MusicXML may be greater than 4
    // and there can be holes
    int                   fVoiceNumber;

    // there can only be 4 regular voices in a staff
    // (those that can contain beamed notes)
    // and we need a number for the orientation of beams
    int                   fRegularVoiceStaffSequentialNumber;

    // voice name

    string                fVoiceName;

  public:

    // public work services
    // ------------------------------------------------------

    // clef, key, time signature

    void                  setVoiceCurrentClef (S_msrClef clef);

    S_msrClef             getVoiceCurrentClef () const
                              { return fVoiceCurrentClef; }

    S_msrClef             getVoiceFirstClef () const
                              { return fVoiceFirstClef; }

    void                  setVoiceCurrentKey (S_msrKey key);

    S_msrKey              getVoiceCurrentKey  () const
                              { return fVoiceCurrentKey; }

    void                  setVoiceCurrentTimeSignature (
                            S_msrTimeSignature timeSignature);

    S_msrTimeSignature    getVoiceCurrentTimeSignature () const
                              { return fVoiceCurrentTimeSignature; }

    // current measure number

    const string          getVoiceCurrentMeasureNumber () const
                              { return fVoiceCurrentMeasureNumber; }

    // ordinal measure number

    void                  setVoiceCurrentMeasureOrdinalNumber (
                            int measureOrdinalNumber)
                              {
                                fVoiceCurrentMeasureOrdinalNumber =
                                  measureOrdinalNumber;
                              }

    int                   incrementVoiceCurrentMeasureOrdinalNumber ()
                              { return ++fVoiceCurrentMeasureOrdinalNumber; }

    int                   getVoiceCurrentMeasureOrdinalNumber () const
                              { return fVoiceCurrentMeasureOrdinalNumber; }

    // measure purist number

    void                  incrementVoiceCurrentMeasurePuristNumber (
                            int           inputLineNumber,
                            const string& context);

    void                  setVoiceCurrentMeasurePuristNumber (
                            int measurePuristNumber)
                              {
                                fVoiceCurrentMeasurePuristNumber =
                                  measurePuristNumber;
                              }

    const int             getVoiceCurrentMeasurePuristNumber () const
                              { return fVoiceCurrentMeasurePuristNumber; }

    const int             getVoiceFirstMeasurePuristNumber () const
                              { return fVoiceFirstMeasurePuristNumber; }
/*
    const int             getVoiceLastMeasurePuristNumber () const
                              { return fVoiceLastMeasurePuristNumber; }
*/

    // measures

    void                  setVoiceFirstMeasure (
                            S_msrMeasure measure)
                              { fVoiceFirstMeasure = measure; }

    const S_msrMeasure    getVoiceFirstMeasure () const
                              { return fVoiceFirstMeasure; }

    const S_msrMeasure    getVoiceLastAppendedMeasure () const
                              { return fVoiceLastAppendedMeasure; }

    void                  appendMeasureCloneToVoiceClone (
                            int          inputLineNumber,
                            S_msrMeasure measureClone);

  private:

    // private work services
    // ------------------------------------------------------

    void                  displayVoiceRepeatsStackSummary (
                            int           inputLineNumber,
                            const string& context);

    void                  displayVoiceRepeatsStack (
                            int           inputLineNumber,
                            const string& context);

    void                  displayVoiceRepeatsStackAndVoice (
                            int           inputLineNumber,
                            const string& context);

    void                  displayVoiceFullBarRests (
                            int           inputLineNumber,
                            const string& context);

    void                  displayVoiceFullBarRestsAndVoice (
                            int           inputLineNumber,
                            const string& context);

    void                  displayVoiceMeasureRepeat (
                            int           inputLineNumber,
                            const string& context);

    void                  displayVoiceMeasureRepeatAndVoice (
                            int           inputLineNumber,
                            const string& context);

    void                  displayVoiceMeasuresFlatList () const;


  private:

    // private work fields
    // ------------------------------------------------------

    // voice initial elements list

    list<S_msrVoiceElement>
                          fVoiceInitialElementsList;

    // voice first and last segments

    // fVoiceLastSegment contains the music
    // not yet stored in fVoiceInitialElementsList,
    // it is thus logically the end of the latter,
    // and is created implicitly for every voice.
    // It is needed 'outside' of the 'list<S_msrElement>'
    // because it is not a mere S_msrElement, but a S_msrSegment
    S_msrSegment          fVoiceLastSegment;

    // fVoiceFirstSegment is used to work around LilyPond issue #34
    S_msrSegment          fVoiceFirstSegment;

    // counters

    static int            gVoicesCounter;

    int                   fVoiceActualNotesCounter;
    int                   fVoiceRestsCounter;
    int                   fVoiceSkipsCounter;
    int                   fVoiceActualHarmoniesCounter;
    int                   fVoiceActualFiguredBassElementsCounter;

    // incomplete measures after repeats detection
    msrVoiceRepeatPhaseKind
                          fCurrentVoiceRepeatPhaseKind;


    // clef, key, time signature
    // needed at the voice level, because adding such to voice clones
    // is done directly, without going down the part-staff-voice hierarchy
    S_msrClef             fVoiceCurrentClef;

    S_msrKey              fVoiceCurrentKey;

    S_msrTimeSignature    fVoiceCurrentTimeSignature;

    // the first in the voice is handled specifically for LilyPond code generation
    S_msrClef             fVoiceFirstClef;

    // measures

    string                fVoiceCurrentMeasureNumber;

    int                   fVoiceCurrentMeasureOrdinalNumber;

    int                   fVoiceCurrentMeasurePuristNumber;
                            // this is a 'purist' measure number,
                            // that starts at 0 if there is an anacrusis,
                            // and 1 otherwise,
                            // and is shared by incomplete (sub)measure fragments
    int                   fVoiceFirstMeasurePuristNumber;
// JMI    int                   fVoiceLastMeasurePuristNumber;

    // a stack is needed to handle pending repeats, which can be nested
    list<S_msrRepeatDescr>
                          fVoicePendingRepeatDescrsStack;

    // full-bar rests

    // fVoicePendingFullBarRests is either null
    // or the last msrFullBarRests created,
    // but not yet appended to the voice
    S_msrFullBarRests fVoicePendingFullBarRests;

    // fVoiceFullBarRestsWaitingForItsNextMeasureNumber is either null
    // or the last msrFullBarRests created and appended to the voice,
    // but with its next measure number not yet set
    S_msrFullBarRests fVoiceFullBarRestsWaitingForItsNextMeasureNumber;
    int                   fVoiceRemainingFullBarRests;

    Bool                  fVoiceContainsFullBarRests;

    // measure repeats

    // fVoicePendingMeasureRepeat is either null
    // or the last msrMeasureRepeat created,
    // but not yet appended to the voice
    S_msrMeasureRepeat    fVoicePendingMeasureRepeat;

    Bool                  fVoiceContainsMeasureRepeats;

    // current position in voice

    rational              fCurrentPositionInVoice;
    msrMoment             fCurrentMomentInVoice;

    // voice finalization

    Bool                  fVoiceHasBeenFinalized;

    // beams begin, continue and end check

    stack<int>            fVoiceBeamNumbersStack;

    // calls counter (for debug)
    int                   fCallsCounter;
};
typedef SMARTP<msrVoice> S_msrVoice;
EXP ostream& operator<< (ostream& os, const S_msrVoice& elt);


}


#endif
