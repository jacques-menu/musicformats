/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrVoices___
#define ___msrVoices___

#include <map>
#include <stack>

#include "msrTypesForwardDeclarations.h"

#include "msrClefsKeysTimeSignatures.h"
#include "msrElements.h"
#include "msrMeasures.h"
#include "msrMeasuresSlices.h"
#include "msrRepeatsEnumTypes.h"
#include "msrSegnos.h"
#include "msrTupletFactors.h"
#include "msrUseSymbols.h"


namespace MusicFormats
{

//______________________________________________________________________________
// PRE-declarations for class mutual dependencies

class   msrClefKeyTimeSignatureGroup;
typedef SMARTP<msrClefKeyTimeSignatureGroup> S_msrClefKeyTimeSignatureGroup;

// voice kinds
//______________________________________________________________________________
enum class msrVoiceKind {
  kVoiceKindRegular,
  kVoiceKindDynamics,
  kVoiceKindHarmonies,  // for MusicXML <harmony/>, LilyPond ChordNames
  kVoiceKindFiguredBass // for MusicXML <figured-bass/>, LilyPond FiguredBass
};

std::string msrVoiceKindAsString (
  msrVoiceKind voiceKind);

std::ostream& operator << (std::ostream& os, const msrVoiceKind& elt);

std::string msrVoiceKindAsStringForPrint (
  msrVoiceKind voiceKind);

//______________________________________________________________________________

// data types
// ------------------------------------------------------

enum class msrVoiceRepeatPhaseKind {
  kVoiceRepeatPhaseNone,
  kVoiceRepeatPhaseAfterCommonPart,
  kVoiceRepeatPhaseAfterHookedEnding,
  kVoiceRepeatPhaseAfterHooklessEnding
};

extern std::string msrVoiceRepeatPhaseKindAsString (
  msrVoiceRepeatPhaseKind afterRepeatComponentPhaseKind);

std::ostream& operator << (std::ostream& os, const msrVoiceRepeatPhaseKind& elt);

enum class msrVoiceFinalizationStatusKind { // JMI ??? v0.9.66
  kKeepVoice,
  kEraseVoice
};

extern std::string msrVoiceFinalizationStatusKindAsString (
  msrVoiceFinalizationStatusKind voiceFinalizationStatusKind);

std::ostream& operator << (std::ostream& os, const msrVoiceFinalizationStatusKind& elt);

enum class msrVoiceCreateInitialLastSegmentKind {
  kCreateInitialLastSegmentYes,
  kCreateInitialLastSegmentNo
};

extern std::string msrVoiceFinalizationStatusKindAsString (
  msrVoiceCreateInitialLastSegmentKind voiceCreateInitialLastSegmentKind);

std::ostream& operator << (std::ostream& os, const msrVoiceCreateInitialLastSegmentKind& elt);

//______________________________________________________________________________
class EXP msrVoice : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrVoice> create (
                            int               inputLineNumber,
                            msrVoiceKind      voiceKind,
                            int               voiceNumber,
                            msrVoiceCreateInitialLastSegmentKind
                                              voiceCreateInitialLastSegmentKind,
                            const S_msrStaff& voiceUpLinkToStaff);

    SMARTP<msrVoice> createVoiceNewbornClone (
                            const S_msrStaff& containingStaff);

    SMARTP<msrVoice> createVoiceDeepClone (
                            int               inputLineNumber,
                            msrVoiceKind      voiceKind,
                            int               voiceNumber,
                            const S_msrStaff& containingStaff);

    // creation from the API
    // ------------------------------------------------------

    static SMARTP<msrVoice> createRegularVoice (
                            int               inputLineNumber,
                            int               voiceNumber,
                            const S_msrStaff& voiceUpLinkToStaff);

    static SMARTP<msrVoice> createHarmoniesVoice (
                            int               inputLineNumber,
                            int               voiceNumber,
                            const S_msrStaff& voiceUpLinkToStaff);

    static SMARTP<msrVoice> createFiguredBassVoice (
                            int               inputLineNumber,
                            int               voiceNumber,
                            const S_msrStaff& voiceUpLinkToStaff);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    // for regular voices
                          msrVoice (
                            int               inputLineNumber,
                            msrVoiceKind      voiceKind,
                            int               voiceNumber,
                            msrVoiceCreateInitialLastSegmentKind
                                              voiceCreateInitialLastSegmentKind,
                            const S_msrStaff& voiceUpLinkToStaff);

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

    S_msrStaff            getVoiceUpLinkToStaff () const
                              { return fVoiceUpLinkToStaff; }

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

    void                  setRegularVoiceOrdinalNumberInPart (int voiceOrdinalNumber)
                              { fRegularVoiceOrdinalNumberInPart = voiceOrdinalNumber; }

    int                   getRegularVoiceOrdinalNumberInPart () const
                              { return fRegularVoiceOrdinalNumberInPart; }

    void                  setRegularVoiceStaffSequentialNumber (
                            int regularVoiceStaffSequentialNumber);

    int                   getRegularVoiceStaffSequentialNumber () const
                              { return fRegularVoiceStaffSequentialNumber; }

    // voice names

    void                  setVoiceNamesFromNumber (
                            int inputLineNumber,
                            int voiceNumber);

    std::string           getVoiceName () const
                              { return fVoiceName; }

    std::string           getVoiceAlphabeticName () const
                              { return fVoiceAlphabeticName; }

    // harmonies

// JMI
    S_msrVoice            getRegularVoiceForwardLinkToHarmoniesVoice () const
                              { return fRegularVoiceForwardLinkToHarmoniesVoice; }

    S_msrVoice            getHarmoniesVoiceRegularVoiceBackwardLink () const
                              { return fHarmoniesVoiceRegularVoiceBackwardLink; }

    // figured bass

    S_msrVoice            getRegularVoiceForwardLinkToFiguredBassVoice () const
                              { return fRegularVoiceForwardLinkToFiguredBassVoice; }

    S_msrVoice            getFiguredBassVoiceRegularVoiceBackwardLink () const
                              { return fFiguredBassVoiceRegularVoiceBackwardLink; }
//

    // stanzas

    const std::map <std::string, S_msrStanza>&
                          getVoiceStanzasMap () const
                              { return fVoiceStanzasMap; }

    // voice last segment

    void                  setVoiceLastSegmentInVoiceClone (
                            const S_msrSegment& segment);

    void                  appendSegmentToVoiceClone (
                            const S_msrSegment& segment);

    S_msrSegment          getVoiceLastSegment () const
                              { return fVoiceLastSegment; }

    // voice last appended note

    S_msrNote             getVoiceLastAppendedNote () const
                              { return fVoiceLastAppendedNote; }

    // voice shortest note

    void                  setVoiceShortestNoteWholeNotes (
                            const mfWholeNotes& wholeNotes);

    mfWholeNotes          getVoiceShortestNoteWholeNotes () const
                              { return fVoiceShortestNoteWholeNotes; }

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

    int                   getVoiceActualFiguredBassesCounter () const
                              { return fVoiceActualFiguredBassesCounter; }

    // has music been inserted in the voice?

//     void                  setVoiceIsMusicallyEmpty ()
//                               { fVoiceIsMusicallyEmpty = false; }

    Bool                  getVoiceIsMusicallyEmpty () const
                              { return fVoiceIsMusicallyEmpty; }

    // regular measure ends detection

    void                  setWholeNotesSinceLastRegularMeasureEnd (
                            int                 inputLineNumber,
                            const mfWholeNotes& wholeNotes);

    mfWholeNotes          getWholeNotesSinceLastRegularMeasureEnd () const
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

    void                  setVoiceContainsMultipleMeasureRests (
                            int inputLineNumber);

    Bool                  getVoiceContainsMultipleMeasureRests () const
                              { return fVoiceContainsMultipleMeasureRests; }

    // measure repeats

    void                  setVoiceContainsMeasureRepeats (
                            int inputLineNumber);

    Bool                  getVoiceContainsMeasureRepeats () const
                              { return fVoiceContainsMeasureRepeats; }

    // measures flat list

    const std::list <S_msrMeasure>&
                          getVoiceMeasuresFlatList () const
                              { return fVoiceMeasuresFlatList; }

    // measures slices sequence

    S_msrMeasuresSlicesSequence
                          getVoiceMeasuresSlicesSequence () const
                              { return fVoiceMeasuresSlicesSequence; }

  public:

    // comparisons
    // ------------------------------------------------------

    // voices ordering in staves
    static bool           compareVoicesByIncreasingNumber (
                            const S_msrVoice& first,
                            const S_msrVoice& second);

    static bool           compareVoicesToHaveHarmoniesAboveCorrespondingVoice (
                            const S_msrVoice& first,
                            const S_msrVoice& second);

    static bool           compareVoicesToHaveFiguredBassesBelowCorrespondingVoice (
                            const S_msrVoice& first,
                            const S_msrVoice& second);

  public:

    // public services
    // ------------------------------------------------------

    // upLinks

    S_msrPart             fetchVoiceUpLinkToPart () const;

    S_msrPartGroup        fetchVoiceUpLinkToPartGroup () const;

    S_msrScore            fetchVoiceUpLinkToScore () const;

    // identity

    void                  changeVoiceIdentity ( // after a deep clone
                            int voiceNumber);

    // print layout MusicXML specific??? JMI
     void                 appendMusicXMLPrintLayoutToVoice (
                            const S_msrMusicXMLPrintLayout& musicXMLPrintLayout);

    // measures

    S_msrMeasure          cascadeCreateAMeasureAndAppendItInVoice (
                            int                    inputLineNumber,
                            int                    previousMeasureEndInputLineNumber,
                            const std::string&     measureNumber,
                            msrMeasureImplicitKind measureImplicitKind);

    void                  setNextMeasureNumberInVoice (
                            int                inputLineNumber,
                            const std::string& nextMeasureNumber);

    // forward

    void                  cascadeAppendPaddingNoteToVoice (
                            int                  inputLineNumber,
                            const mfWholeNotes& forwardStepLength);

    // backup and padding

//     void                  padUpToPositionInMeasureInVoice (
//                             int             inputLineNumber,
//                             const mfWholeNotes& wholeNotesPositionInMeasure);

//     void                  casadeBackupByWholeNotesStepLengthInVoice (
//                             int                  inputLineNumber,
//                             const mfWholeNotes& backupTargetMeasureElementPositionInMeasure);

    // clef, key, time signature

    void                  appendClefKeyTimeSignatureGroupToVoice (
                            const S_msrClefKeyTimeSignatureGroup&
                              clefKeyTimeSignatureGroup);

//     void                  appendClefToVoice (
//                             const S_msrClef& clef);
//
//     void                  appendKeyToVoice  (
//                             const S_msrKey&  key);

    void                  appendTimeSignatureToVoice (
                            const S_msrTimeSignature& timeSignature);

    void                  appendTimeSignatureToVoiceClone (
                            const S_msrTimeSignature& timeSignature);

    // dal segno

    void                  insertHiddenMeasureAndBarLineInVoiceClone (
                            int                        inputLineNumber,
                            const mfPositionInMeasure& positionInMeasure);

    // notes

    S_msrNote             fetchVoiceFirstNonGraceNote () const;

    void                  registerShortestNoteInVoiceIfRelevant (
                            const S_msrNote& note);

//     void                  registerNoteAsVoiceLastAppendedNote (
//                             const S_msrNote& note);

    // harmonies

    S_msrVoice            createRegularVoiceHarmoniesVoice (
                            int                inputLineNumber,
                            const std::string& currentMeasureNumber);

    // figured bass

//     S_msrVoice            createRegularVoiceFiguredBassVoice (
//                             int                inputLineNumber,
//                             const std::string& currentMeasureNumber);

    // transposition

    void                  appendTranspositionToVoice (
                            const S_msrTransposition& transpose);

    // staff details

    void                  appendStaffDetailsToVoice (
                            const S_msrStaffDetails& staffDetails);

    // tempo

    void                  appendTempoToVoice (
                            const S_msrTempo& tempo);

    // rehearsal marks

    void                  appendRehearsalMarkToVoice (
                            const S_msrRehearsalMark& rehearsalMark);

    // octave shifts

    void                  appendOctaveShiftToVoice (
                            const S_msrOctaveShift& octaveShift);

    // scordaturas

    void                  appendScordaturaToVoice (
                            const S_msrScordatura& scordatura);

    // accordion registration

    void                  appendAccordionRegistrationToVoice (
                            const S_msrAccordionRegistration&
                              accordionRegistration);

    // harp pedals tuning

    void                  appendHarpPedalsTuningToVoice (
                            const S_msrHarpPedalsTuning&
                              harpPedalsTuning);

    // staff change

    void                  appendVoiceStaffChangeToVoice (
                            const S_msrVoiceStaffChange& voiceStaffChange);

    // notes

    void                  appendNoteToVoice      (
                            const S_msrNote& note);
    void                  appendNoteToVoiceClone (
                            const S_msrNote& note);

    // tremolos

    void                  appendDoubleTremoloToVoice (
                            const S_msrDoubleTremolo& doubleTremolo);

    // chords

    void                  appendChordToVoice  (
                            const S_msrChord& chord);

    // tuplets

    void                  appendTupletToVoice (
                            const S_msrTuplet& tuplet);

    void                  registerTupletNoteInVoice (
                            const S_msrNote& note);

    // harmonies

    void                  cascadeAppendHarmonyToVoice (
                            int                        inputLineNumber,
                            const S_msrHarmony&        harmony,
                            const mfPositionInMeasure& positionInMeasureToAppendAt);

    void                  appendHarmoniesListToVoice (
                            int                             inputLineNumber,
                            const std::list <S_msrHarmony>& harmoniesList,
                            const mfPositionInMeasure&      positionInMeasureToAppendAt);

    void                  cascadeAppendHarmonyToVoiceClone (
                            const S_msrHarmony& harmony);

    // figured bass

    void                  appendFiguredBassToVoice (
                            int                       inputLineNumber,
                            const S_msrFiguredBass&   figuredBass,
                            const mfPositionInMeasure& positionInMeasureToAppendAt);

    void                  cascadeAppendFiguredBassesListToVoice (
                            int                                 inputLineNumber,
                            const std::list <S_msrFiguredBass>& figuredBasssesList,
                            const mfPositionInMeasure&          positionInMeasureToAppendAt);

    void                  appendFiguredBassToVoiceClone (
                            const S_msrFiguredBass& figuredBass);

    // frames

    void                  appendFrameToVoice (
                            const S_msrFrame& frame);

    void                  appendFrameToVoiceClone (
                            const S_msrFrame& frame);

    // grace notes

    void                  addGraceNotesGroupBeforeAheadOfVoiceIfNeeded (
                            const S_msrGraceNotesGroup& graceNotesGroup);

    /*
    void                  appendAfterGraceNotesToVoice (
                            const S_msrAfterGraceNotes& afterGraceNotes);

    void                  prependAfterGraceNotesToVoice (
                            const S_msrAfterGraceNotes& afterGraceNotes);
                            */

    // lyrics

    void                  appendSyllableToVoice ( // JMI
                            int                  inputLineNumber,
                            const std::string&   stanzaNumber,
                            const std::string&   stanzaName,
                            const S_msrSyllable& syllable);

    // bar checks

    void                  appendBarCheckToVoice (
                            const S_msrBarCheck& barCheck);

    // bar number checks

    void                  appendBarNumberCheckToVoice (
                            const S_msrBarNumberCheck& barNumberCheck);

    // breaks

    void                  appendLineBreakToVoice (
                            const S_msrLineBreak& lineBreak);
    void                  appendPageBreakToVoice (
                            const S_msrPageBreak& pageBreak);

    // bar lines

    void                  prependBarLineToVoice (
                            const S_msrBarLine& barLine);
    void                  appendBarLineToVoice (
                            const S_msrBarLine& barLine);

    // segno

    void                  appendSegnoToVoice (
                            const S_msrSegno& segno);

    // coda

    void                  appendCodaToVoice (
                            const S_msrCoda& coda);

    // eyeglasses

    void                  appendEyeGlassesToVoice (
                            const S_msrEyeGlasses& eyeGlasses);

    // pedal

    void                  appendPedalToVoice (
                            const S_msrPedal& pedal);

    // damp

    void                  appendDampToVoice (
                            const S_msrDamp& damp);

    // damp all

    void                  appendDampAllToVoice (
                            const S_msrDampAll& dampAll);

    // other elements

//     void                  prependOtherElementToVoice (
//                             const S_msrMeasureElement& elem);
//     void                  appendOtherElementToVoice (
//                             const S_msrMeasureElement& elem);
//                             // for other types of elements not known
//                             // in this header file, such as LPSR elements

    // last measure in voice

    S_msrMeasure          fetchVoiceLastMeasure (
                            int inputLineNumber) const;

    // last element in voice

    S_msrMeasureElement   fetchVoiceLastMeasureElement ( // JMI ??? v0.9.66
                            int inputLineNumber) const;

    // removing elements from voice

//     void                  removeNoteFromVoice (
//                             int              inputLineNumber,
//                             const S_msrNote& note);

//     void                  removeElementFromVoice (
//                             int                 inputLineNumber,
//                             const S_msrElement& element);

//     S_msrMeasure          removeLastMeasureFromVoice (
//                             int inputLineNumber);

    // segments

    void                  createNewLastSegmentForVoice (
                            int                inputLineNumber,
                            const std::string& context);

    void                  createNewLastSegmentFromItsFirstMeasureForVoice (
                            int                 inputLineNumber,
                            const S_msrMeasure& firstMeasure,
                            const std::string&  context);

    // repeats

    void                  handleRepeatEndInVoice (
                            int                inputLineNumber,
                            const std::string& measureNumber,
                            int                repeatTimes);

    void                  handleRepeatEndingStartInVoice (
                            int inputLineNumber);

    void                  handleRepeatEndingEndInVoice (
                            int                 inputLineNumber,
                            const std::string&  repeatEndingNumber, // may be "1, 2"
                            msrRepeatEndingKind repeatEndingKind);

    void                  handleRepeatCommonPartStartInVoiceClone (
                            int inputLineNumber);

    void                  handleRepeatCommonPartEndInVoiceClone (
                            int inputLineNumber);

    void                  handleRepeatEndingStartInVoiceClone (
                            int                 inputLineNumber,
                            msrRepeatEndingKind repeatEndingKind,
                            const std::string&  repeatEndingNumber); // may be "1, 2"

    void                  handleRepeatEndingEndInVoiceClone (
                            int                 inputLineNumber,
                            const std::string&  repeatEndingNumber, // may be "1, 2"
                            msrRepeatEndingKind repeatEndingKind);

    void                  handleSegmentCloneEndInVoiceClone (
                            int                 inputLineNumber,
                            const S_msrSegment& segmentClone);

/* JMI
    void                  finalizeRepeatEndInVoice (
                            int                inputLineNumber,
                            const std::string& measureNumber,
                            int                repeatTimes);
                            */

    void                  handleRepeatStartInVoice (
                            int inputLineNumber);

    void                  handleRepeatStartInVoiceClone (
                            int                inputLineNumber,
                            const S_msrRepeat& repeat);

    void                  handleRepeatEndInVoiceClone (
                            int inputLineNumber);

    void                  appendRepeatCloneToVoiceClone (
                            int                inputLineNumber,
                            const S_msrRepeat& repeatCLone);

    void                  appendRepeatEndingCloneToVoice (
                            const S_msrRepeatEnding& repeatEndingClone);

    // multiple measure rests

//     void                  cascadeAppendMultipleMeasureRestToVoiceElementsList (
//                             const S_msrMultipleMeasureRest& multipleMeasureRest);

    void                  createAMultipleMeasureRestAndAppendItToVoice (
                            int               inputLineNumber,
                            int               multipleMeasureRestMeasuresNumber,
                            int               multipleMeasureRestSlashesNumber,
                            msrUseSymbolsKind multipleMeasureRestUseSymbolsKind);

    void                  cascadeAppendMultipleMeasureRestToVoice (
                            int                             inputLineNumber,
                            const S_msrMultipleMeasureRest& multipleMeasureRest);

    void                  replicateLastAppendedMeasureInVoice (
                            int inputLineNumber,
                            int replicatasNumber);

    void                  appendEmptyMeasuresToVoice (
                            int                inputLineNumber,
                            const std::string& previousMeasureNumber,
                            int                emptyMeasuresNumber);

    void                  appendPendingMultipleMeasureRestsToVoice (
                            int inputLineNumber);

    void                  handleMultipleMeasureRestsStartInVoiceClone (
                            int                          inputLineNumber,
                            const S_msrMultipleMeasureRest& multipleMeasureRest);

    void                  handleMultipleMeasureRestsEndInVoiceClone (
                            int inputLineNumber);

//     void                  handleMultipleMeasureRestsContentsStartInVoiceClone (
//                             int inputLineNumber);
//
//     void                  handleMultipleMeasureRestsContentsEndInVoiceClone (
//                             int inputLineNumber);

    void                  appendMultipleMeasureRestCloneToVoiceClone ( // JMI ???
                            int                          inputLineNumber,
                            const S_msrMultipleMeasureRest& multipleMeasureRestClone);

    // measure repeats

    void                  cascadeCreateAMeasureRepeatAndAppendItToVoice (
                            int inputLineNumber,
                            int measureRepeatMeasuresNumber,
                            int measureRepeatSlashesNumber);

    void                  appendMeasureRepeatToVoiceElementsList (
                            const S_msrMeasureRepeat& beatRepeat);

    void                  appendMeasureRepeatReplicaToVoice (
                            int inputLineNumber);

    void                  appendMeasureRepeatToVoice (
                            int                       inputLineNumber,
                            const S_msrMeasureRepeat& beatRepeat);

    void                  appendPendingMeasureRepeatToVoice (
                            int inputLineNumber);

    void                  createMeasureRepeatAndAppendItToVoiceClone (
                            int inputLineNumber,
                            int measureRepeatMeasuresNumber,
                            int measureRepeatSlashesNumber);

    void                  handleMeasureRepeatStartInVoiceClone (
                            int                       inputLineNumber,
                            const S_msrMeasureRepeat& beatRepeat);

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
                            int                       inputLineNumber,
                            const S_msrMeasureRepeat& beatRepeatClone);

    // stanzas

    S_msrStanza           addStanzaToVoiceByItsNumber (
                            int                inputLineNumber,
                            const std::string& stanzaNumber);

    void                  addStanzaToVoiceWithoutCatchUp ( // JMI
                            const S_msrStanza& stanza);

    void                  addStanzaToVoice (
                            const S_msrStanza& stanza);

    S_msrStanza           createStanzaInVoiceIfNotYetDone (
                            int                inputLineNumber,
                            const std::string& stanzaNumber,
                            const std::string& stanzaName);

    S_msrStanza           fetchStanzaInVoice (
                            int                inputLineNumber,
                            const std::string& stanzaNumber,
                            const std::string& stanzaName);

    // strings

    // voice position

    void                  incrementCurrentVoicePosition (
                            const mfWholeNotes& delta)
                              {
                                fCurrentVoicePosition += delta;
                              }

    mfWholeNotes          getCurrentVoicePosition () const
                              { return fCurrentVoicePosition; }

    const mfMoment&      getCurrentVoiceMoment () const
                              { return fCurrentVoiceMoment; }

    // beams begin, continue and end check

    void                  checkBeamNumber (
                            const S_msrBeam& beam,
                            const S_msrNote& note);

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
                            int                inputLineNumber,
                            const std::string& context);

    void                  moveVoiceLastSegmentToInitialVoiceElementsIfRelevant (
                            int                inputLineNumber,
                            const std::string& context);

    // repeats

    S_msrRepeat           createARepeatAndStackIt (
                            int                inputLineNumber,
                            const std::string& context);

    S_msrRepeat           createARepeatCloneAndStackIt (
                            int                inputLineNumber,
                            const S_msrRepeat& repeat,
                            const std::string& context);

    void                  pushRepeatOntoVoiceRepeatsStack (
                            int                inputLineNumber,
                            const S_msrRepeat& repeat,
                            const std::string& context);

    void                  popRepeatFromVoiceRepeatsStack (
                            int                inputLineNumber,
                            const S_msrRepeat& repeat,
                            const std::string& context);

    void                  appendRepeatToInitialVoiceElements (
                            int                inputLineNumber,
                            const S_msrRepeat& repeat,
                            const std::string& context);

    void                  appendRepeatCloneToInitialVoiceElements (
                            int                inputLineNumber,
                            const S_msrRepeat& repeatCLone,
                            const std::string& context);

    void                  handleVoiceLevelRepeatStart (
                            int inputLineNumber);

    void                  handleNestedRepeatStartInVoice (
                            int inputLineNumber);

    void                  handleVoiceLevelRepeatEndWithoutStart (
                            int                inputLineNumber,
                            const std::string& measureNumber,
                            int                repeatTimes);

    void                  handleVoiceLevelContainingRepeatEndWithoutStart (
                            int                inputLineNumber,
                            const std::string& measureNumber,
                            int                repeatTimes);

    void                  handleVoiceLevelRepeatEndWithStart (
                            int                inputLineNumber,
                            const std::string& measureNumber,
                            int                repeatTimes);

    void                  handleVoiceLevelRepeatEndingStartWithoutExplicitStart (
                            int          inputLineNumber,
                            S_msrRepeat& currentRepeat);

    void                  handleVoiceLevelRepeatEndingStartWithExplicitStart (
                            int inputLineNumber);

    void                  nestContentsIntoNewRepeatInVoice (
                            int inputLineNumber);

    void                  handleNestedRepeatEndingStartInVoice (
                            int inputLineNumber);

    void                  handleNestedRepeatEndInVoice (
                            int                inputLineNumber,
                            const std::string& measureNumber,
                            int                repeatTimes);

    void                  moveVoiceInitialElementsToRepeatCommonPart (
                            int                          inputLineNumber,
                            const S_msrRepeatCommonPart& repeatCommonPart,
                            const std::string&           context);

    void                  moveVoiceLastSegmentToRepeatCommonPart (
                            int                          inputLineNumber,
                            const S_msrRepeatCommonPart& repeatCommonPart,
                            const std::string&           context);

    void                  moveAllVoiceContentsToRepeatCommonPart (
                            int                          inputLineNumber,
                            const S_msrRepeatCommonPart& repeatCommonPart,
                            const std::string&           context);

    void                  moveVoiceLastSegmentToRepeatEnding (
                            int                      inputLineNumber,
                            const S_msrRepeatEnding& repeatEnding,
                            const std::string&       context);

    void                  handleHookedRepeatEndingEndInVoice (
                            int                inputLineNumber,
                            const std::string& repeatEndingNumber); // may be "1, 2"

    void                  handleHooklessRepeatEndingEndInVoice (
                            int                inputLineNumber,
                            const std::string& repeatEndingNumber); // may be "1, 2"

    void                  handleHookedRepeatEndingEndInVoiceClone (
                            int                inputLineNumber,
                            const std::string& repeatEndingNumber); // may be "1, 2"

    void                  handleHooklessRepeatEndingEndInVoiceClone (
                            int                inputLineNumber,
                            const std::string& repeatEndingNumber); // may be "1, 2"

    // multiple measure rests

//     void                  cascadeAppendMultipleMeasureRestToInitialVoiceElements (
//                             int                              inputLineNumber,
//                             const S_msrMultipleMeasureRest& multipleMeasureRest,
//                             const std::string&               context);

    // measure repeats

    void                  appendMeasureRepeatToInitialVoiceElements (
                            int                       inputLineNumber,
                            const S_msrMeasureRepeat& measureRepeat,
                            const std::string&        context);

    // beats repeats

    void                  appendBeatRepeatToInitialVoiceElements (
                            int                    inputLineNumber,
                            const S_msrBeatRepeat& beatRepeat,
                            const std::string&     context);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           fetchVoiceNumberAsString () const;

    std::string           regularVoiceStaffSequentialNumberAsString () const;

    std::string           asShortString () const override;
    std::string           asString () const override;

    virtual void          displayVoice (
                            int                inputLineNumber,
                            const std::string& context) const;

    void                  displayVoiceRepeatsStackMultipleMeasureRestsMeasureRepeatAndVoice (
                            int                inputLineNumber,
                            const std::string& context);

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks

    S_msrStaff            fVoiceUpLinkToStaff;

// JMI
    // two-way links
    S_msrVoice            fRegularVoiceForwardLinkToHarmoniesVoice;
    S_msrVoice            fHarmoniesVoiceRegularVoiceBackwardLink;

    S_msrVoice            fRegularVoiceForwardLinkToFiguredBassVoice;
    S_msrVoice            fFiguredBassVoiceRegularVoiceBackwardLink;
//

    // voice kind

    msrVoiceKind          fVoiceKind;

    // musically empty voices

    Bool                  fVoiceIsMusicallyEmpty;

    // regular measure ends detection

    mfWholeNotes          fWholeNotesSinceLastRegularMeasureEnd;

     // stanzas

    std::map <std::string, S_msrStanza>
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
    std::list <S_msrMeasure>
                          fVoiceMeasuresFlatList;

    // measures slices sequence
    S_msrMeasuresSlicesSequence
                          fVoiceMeasuresSlicesSequence;

    // notes
    // fVoiceLastAppendedNote is used to build chords upon their second note
    S_msrNote             fVoiceLastAppendedNote;

    // part shortest note

    // fVoiceShortestNoteWholeNotes and fVoiceShortestNoteTupletFactor
    // are used to compute a number of divisions per quarter note
    // if needed, such as when generating MusicXML from MSR
    mfWholeNotes          fVoiceShortestNoteWholeNotes;
    msrTupletFactor       fVoiceShortestNoteTupletFactor;

    // repeats

  protected:

    // protected fields
    // ------------------------------------------------------

    // voice numbers

    // voice numbers in MusicXML may be greater than 4
    // and there can be holes
    int                   fVoiceNumber;


    // the handling of tuplets in mxsr2msrTranslator.cpp
    // need to store the yet incomplete tuplets
    // due to the order in which their elements are present in the MusicXML data,
    // see mxsr2msrSkeletonPopulator.h
    int                   fRegularVoiceOrdinalNumberInPart;

    // there can only be 4 regular voices in a staff
    // (those that can contain beamed notes)
    // and we need a number for the orientation of beams
    // for use by regular voices only (i.e. not harmonies nor figures basses voices)
    int                   fRegularVoiceStaffSequentialNumber;

    // voice name

    std::string           fVoiceName;
    std::string           fVoiceAlphabeticName;

  public:

    // public work services
    // ------------------------------------------------------

    // clef, key, time signature

    void                  setVoiceCurrentClef (
                            const S_msrClef& clef);

    S_msrClef             getVoiceCurrentClef () const
                              { return fVoiceCurrentClef; }

    S_msrClef             getVoiceFirstClef () const
                              { return fVoiceFirstClef; }

    void                  setVoiceCurrentKey (
                            const S_msrKey& key);

    S_msrKey              getVoiceCurrentKey  () const
                              { return fVoiceCurrentKey; }

    void                  setVoiceCurrentTimeSignature (
                            const S_msrTimeSignature& timeSignature);

    S_msrTimeSignature    getVoiceCurrentTimeSignature () const
                              { return fVoiceCurrentTimeSignature; }

    // current measure number

    const std::string     getVoiceCurrentMeasureNumber () const
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
                            int                inputLineNumber,
                            const std::string& context);

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
                            const S_msrMeasure& measure);

    const S_msrMeasure&   getVoiceFirstMeasure () const
                              { return fVoiceFirstMeasure; }

    void                  appendMeasureCloneToVoiceClone (
                            int                 inputLineNumber,
                            const S_msrMeasure& measureClone);

    // last appended measure
    void                  setVoiceLastAppendedMeasure (
                            const S_msrMeasure& measure);

    const S_msrMeasure&   getVoiceLastAppendedMeasure () const
                              { return fVoiceLastAppendedMeasure; }

  private:

    // private work services
    // ------------------------------------------------------

    void                  displayVoiceRepeatsStackSummary (
                            int                inputLineNumber,
                            const std::string& context);

    void                  displayVoiceRepeatsStack (
                            int                inputLineNumber,
                            const std::string& context);

    void                  displayVoiceRepeatsStackAndVoice (
                            int                inputLineNumber,
                            const std::string& context);

    void                  displayVoiceMultipleMeasureRests (
                            int                inputLineNumber,
                            const std::string& context);

    void                  displayVoiceMultipleMeasureRestsAndVoice (
                            int                inputLineNumber,
                            const std::string& context);

    void                  displayVoiceMeasureRepeat (
                            int                inputLineNumber,
                            const std::string& context);

    void                  displayVoiceMeasureRepeatAndVoice (
                            int                inputLineNumber,
                            const std::string& context);

    void                  displayVoiceMeasuresFlatList (
                            int fieldWidth) const;


  private:

    // private work fields
    // ------------------------------------------------------

    // voice initial elements list

    std::list <S_msrVoiceElement>
                          fVoiceInitialElementsList;

    // voice first and last segments

    // fVoiceLastSegment contains the music
    // not yet stored in fVoiceInitialElementsList,
    // it is thus logically the end of the latter,
    // and is created implicitly for every voice.
    S_msrSegment          fVoiceLastSegment;

    // fVoiceFirstSegment is used to work around LilyPond_Issue_34
    S_msrSegment          fVoiceFirstSegment;

    // counters

    int                   fVoiceActualNotesCounter;
    int                   fVoiceRestsCounter;
    int                   fVoiceSkipsCounter;
    int                   fVoiceActualHarmoniesCounter;
    int                   fVoiceActualFiguredBassesCounter;

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

    std::string           fVoiceCurrentMeasureNumber;

    int                   fVoiceCurrentMeasureOrdinalNumber;

    int                   fVoiceCurrentMeasurePuristNumber;
                            // this is a 'purist' measure number,
                            // that starts at 0 if there is an anacrusis,
                            // and 1 otherwise,
                            // and is shared by incomplete (sub)measure fragments
    int                   fVoiceFirstMeasurePuristNumber;
// JMI    int                   fVoiceLastMeasurePuristNumber;

    // a stack is needed to handle pending repeats, which can be nested
    std::list <S_msrRepeat>
                          fVoicePendingRepeatsStack;

    // multiple measure rests

    Bool                  fOnGoingMultipleMeasureRest;

    // fVoicePendingMultipleMeasureRests is either null
    // or the last msrMultipleMeasureRest created
    S_msrMultipleMeasureRest fVoiceCurrentMultipleMeasureRest;

    // fVoiceMultipleMeasureRestsWaitingForItsNextMeasureNumber is either null
    // or the last msrMultipleMeasureRest created and appended to the voice,
    // but with its next measure number not yet set
    S_msrMultipleMeasureRest
                          fVoiceMultipleMeasureRestsWaitingForItsNextMeasureNumber;
    int                   fVoiceRemainingMultipleMeasureRests;

    Bool                  fVoiceContainsMultipleMeasureRests;

    // measure repeats

    // fVoicePendingMeasureRepeat is either null
    // or the last msrMeasureRepeat created,
    // but not yet appended to the voice
    S_msrMeasureRepeat    fVoicePendingMeasureRepeat;

    Bool                  fVoiceContainsMeasureRepeats;

    // current voice position

    mfWholeNotes          fCurrentVoicePosition;
    mfMoment             fCurrentVoiceMoment;

    // voice finalization

    Bool                  fVoiceHasBeenFinalized;

    // beams begin, continue and end check

    std::list <int>        fVoiceBeamNumbersStack;

    // calls counter (for debug)
    int                   fCallsCounter;
};
typedef SMARTP<msrVoice> S_msrVoice;
EXP std::ostream& operator << (std::ostream& os, const S_msrVoice& elt);


}


#endif // ___msrVoices___
