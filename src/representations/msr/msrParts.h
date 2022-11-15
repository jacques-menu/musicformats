/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrParts___
#define ___msrParts___

#include "msrPartGroupElements.h"

#include "msrTypesForwardDeclarations.h"

#include "msrPartGroups.h"
#include "msrStavesEnumTypes.h"
#include "msrStaves.h"
#include "msrTupletFactors.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrPart : public msrPartGroupElement
{
  public:

    // constants
    // ------------------------------------------------------

    static const int K_PART_HARMONIES_STAFF_NUMBER;
    static const int K_PART_HARMONIES_VOICE_NUMBER;

    static const int K_PART_FIGURED_BASS_STAFF_NUMBER;
    static const int K_PART_FIGURED_BASS_VOICE_NUMBER;

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPart> create (
                            int                  inputLineNumber,
                            const std::string&   partID,
                            const S_msrPartGroup partUpLinkToPartGroup);

    SMARTP<msrPart> createPartNewbornClone (
                            S_msrPartGroup partGroupClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrPart (
                            int                  inputLineNumber,
                            const std::string&   partID,
                            const S_msrPartGroup partUpLinkToPartGroup);

    virtual               ~msrPart ();

  private:

    // private initialization
    // ------------------------------------------------------

    void                  initializePart ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks

    void                  setPartUpLinkToPartGroup (
                            S_msrPartGroup partGroup)
                              { fPartUpLinkToPartGroup = partGroup; }

    S_msrPartGroup        getPartUpLinkToPartGroup () const
                              { return fPartUpLinkToPartGroup; }

    // part absolute number

    int                   getPartAbsoluteNumber () const
                              { return fPartAbsoluteNumber; }

    // ID and name

    void                  setPartID (const std::string& partID)
                              { fPartID = partID; }

    std::string           getPartID () const
                              { return fPartID; }

    void                  setPartMsrName (const std::string& partMsrName);

    std::string           getPartMsrName () const
                              { return fPartMsrName; }

    void                  setPartName (const std::string& partName)
                              { fPartName = partName; }

    std::string           getPartName () const
                              { return fPartName; }

    void                  setPartNameDisplayText (
                            const std::string& partNameDisplayText)
                              {
                                fPartNameDisplayText =
                                  partNameDisplayText;
                              }

    std::string           getPartNameDisplayText () const
                              { return fPartNameDisplayText; }

    void                  setPartAbbreviation (
                            const std::string& partAbbreviation)
                              {
                                fPartAbbreviation =
                                  partAbbreviation;
                              }

    std::string           getPartAbbreviation () const
                              { return fPartAbbreviation; }

    void                  setPartAbbreviationDisplayText (
                            const std::string& partAbbreviationDisplayText)
                              {
                                fPartAbbreviationDisplayText =
                                  partAbbreviationDisplayText;
                              }

    std::string           getPartAbbreviationDisplayText () const
                              { return fPartAbbreviationDisplayText; }

    std::string           getPartCombinedName () const;

    void                  setPartInstrumentNamesMaxLengthes ();

    // measures

    void                  setPartNumberOfMeasures (
                            size_t partNumberOfMeasures);

    const size_t          getPartNumberOfMeasures () const
                              { return fPartNumberOfMeasures; }

    const std::vector<Rational>&
                          getPartMeasuresWholeNotesDurationsVector () const
                              { return fPartMeasuresWholeNotesDurationsVector; }

    // instrument name

    void                  setPartInstrumentName (
                            const std::string& partInstrumentName)
                              { fPartInstrumentName = partInstrumentName; }

    std::string           getPartInstrumentName () const
                              { return fPartInstrumentName; }

    void                  setPartInstrumentAbbreviation (
                            const std::string& partInstrumentAbbreviation)
                              {
                                fPartInstrumentAbbreviation =
                                  partInstrumentAbbreviation;
                              }

    std::string           getPartInstrumentAbbreviation () const
                              { return fPartInstrumentAbbreviation; }

    // harmonies staff and voice

    void                  setPartHarmoniesStaff (
                            S_msrStaff harmoniesStaff)
                              { fPartHarmoniesStaff = harmoniesStaff; }

    S_msrStaff            getPartHarmoniesStaff () const
                              { return fPartHarmoniesStaff; }

    void                  setPartHarmoniesVoice (
                            S_msrVoice harmoniesVoice)
                              { fPartHarmoniesVoice = harmoniesVoice; }

    S_msrVoice            getPartHarmoniesVoice () const
                              { return fPartHarmoniesVoice; }

    // figured bass staff and voice

    void                  setPartFiguredBassStaff (
                            S_msrStaff figuredBassStaff)
                              { fPartFiguredBassStaff = figuredBassStaff; }

    S_msrStaff            getPartFiguredBassStaff () const
                              { return fPartFiguredBassStaff; }

    void                  setPartFiguredBassVoice (
                            S_msrVoice figuredBassVoice)
                              { fPartFiguredBassVoice = figuredBassVoice; }

    S_msrVoice            getPartFiguredBassVoice () const
                              { return fPartFiguredBassVoice; }

    // staves std::map

    const std::map<int, S_msrStaff>&
                          getPartStaveNumbersToStavesMap () const
                              { return fPartStaveNumbersToStavesMap; }

    // staff position in measure

    void                  setPartCurrentMeasurePosition (
                            int             inputLineNumber,
                            const Rational& measurePosition);

    void                  incrementPartCurrentMeasurePosition (
                            int             inputLineNumber,
                            const Rational& duration);
    void                  decrementPartCurrentMeasurePosition (
                            int             inputLineNumber,
                            const Rational& duration);

    Rational              getPartCurrentMeasurePosition () const
                              { return fPartCurrentMeasurePosition; }

    // part shortest note

    void                  setPartShortestNoteDuration (
                            const Rational& duration);

    Rational              getPartShortestNoteDuration () const
                              { return fPartShortestNoteDuration; }

    void                  setPartShortestNoteTupletFactor (
                            const msrTupletFactor& noteTupletFactor);

    const msrTupletFactor&
                          getPartShortestNoteTupletFactor () const
                              { return fPartShortestNoteTupletFactor; }


    // measures slices sequence

    S_msrMeasuresSlicesSequence
                          getPartMeasuresSlicesSequence () const
                              { return fPartMeasuresSlicesSequence; }

  public:

    // public services
    // ------------------------------------------------------

    // uplinks
    S_msrScore            fetchPartUpLinkToScore () const;

    void                  assignSequentialNumbersToRegularVoicesInPart (
                            int inputLineNumber);

/* JMI
    void                  addAVoiceToStavesThatHaveNone (
                            int inputLineNumber);
*/

    // whole notes durations

    Rational              fetchPartMeasuresWholeNotesDurationsVectorAt (
                            int inputLineNumber,
                            int indexValue) const;

    void                  registerOrdinalMeasureNumberWholeNotesDuration (
                            int             inputLineNumber,
                            int             measureOrdinalNumber,
                            const Rational& wholeNotesDuration);

    // path shortest note

    void                  registerShortestNoteInPartIfRelevant (
                            S_msrNote note);

    // measures

    void                  createAMeasureAndAppendItToPart (
                            int           inputLineNumber,
                            int           previousMeasureEndInputLineNumber,
                            const std::string& measureNumber,
                            msrMeasureImplicitKind
                                          measureImplicitKind);

    void                  setNextMeasureNumberInPart (
                            int           inputLineNumber,
                            const std::string& nextMeasureNumber);

    // clef, key, time signature

    void                  appendClefToPart (S_msrClef clef);

    void                  appendKeyToPart (S_msrKey key);

    void                  appendTimeSignatureToPart (
                            S_msrTimeSignature timeSignature);
    void                  appendTimeSignatureToPartClone (
                            S_msrTimeSignature timeSignature);

    // tempo

    void                  appendTempoToPart (S_msrTempo tempo);

    // rehearsal marks

    void                  appendRehearsalMarkToPart (
                            S_msrRehearsalMark rehearsalMark);

    // transposition

    void                  appendTranspositionToPart (
                            S_msrTransposition transposition);

    // staff details

    void                  appendStaffDetailsToPart (
                            S_msrStaffDetails staffDetails);

    // tuplets JMI ???

    // bar lines

    void                  appendBarLineToPart (S_msrBarLine barLine);

    void                  insertHiddenMeasureAndBarLineInPartClone (
                            int             inputLineNumber,
                            const Rational& measurePosition);

    // breaks

    void                  appendLineBreakToPart (S_msrLineBreak lineBreak);
    void                  appendPageBreakToPart (S_msrPageBreak pageBreak);

    // repeats

    void                  handleRepeatStartInPart (
                            int inputLineNumber);

    void                  handleRepeatEndInPart (
                            int           inputLineNumber,
                            const std::string& measureNumber,
                            int           repeatTimes);

    void                  handleRepeatEndingStartInPart (
                            int inputLineNumber);

    void                  handleRepeatEndingEndInPart (
                            int           inputLineNumber,
                            const std::string& repeatEndingNumber, // may be "1, 2"
                            msrRepeatEndingKind
                                          repeatEndingKind);

    /* JMI ???
    void                  finalizeRepeatEndInPart (
                            int    inputLineNumber,
                            const std::string& measureNumber,
                            int    repeatTimes);
                            */

    void                  appendRepeatCloneToPart (
                            int         inputLineNumber,
                            S_msrRepeat repeatCLone);

    void                  appendRepeatEndingCloneToPart (
                            S_msrRepeatEnding repeatEndingCLone);

    // multiple full-bar rests

    void                  appendMultipleFullBarRestsToPart (
                            int inputLineNumber,
                            int multipleFullBarRestsNumber);

    void                  appendPendingMultipleFullBarRestsToPart (
                            int inputLineNumber);

    void                  replicateLastAppendedMeasureInPart (
                            int inputLineNumber,
                            int replicatasNumber);

    void                  addEmptyMeasuresToPart (
                            int           inputLineNumber,
                            const std::string& previousMeasureNumber,
                            int           multipleFullBarRestsNumber);

    void                  appendMultipleFullBarRestsCloneToPart (
                            int               inputLineNumber,
                            S_msrMultipleFullBarRests multipleFullBarRests);

    // measure repeats

    void                  createMeasureRepeatFromItsFirstMeasuresInPart (
                            int inputLineNumber,
                            int measureRepeatMeasuresNumber,
                            int measureRepeatSlashesNumber);

    void                  appendPendingMeasureRepeatToPart (
                            int inputLineNumber);

    void                  appendMeasureRepeatCloneToPart (
                            int               inputLineNumber,
                            S_msrMultipleFullBarRests multipleFullBarRests);

    // staves

    S_msrStaff            addStaffToPartByItsNumber (
                            int          inputLineNumber,
                            msrStaffKind staffKind,
                            int          staffNumber);

    S_msrStaff            addHarmoniesStaffToPart (
                            int inputLineNumber);

    S_msrStaff            addHFiguredBassStaffToPart (
                            int inputLineNumber);

    void                  addStaffToPartCloneByItsNumber (
                            S_msrStaff staff);

    S_msrStaff            fetchStaffFromPart (int staffNumber);

    // voices

    void                  registerVoiceInPartAllVoicesList (
                            S_msrVoice voice);

    // frames

    // JMI ???

    // harmonies

    S_msrVoice            createPartHarmoniesVoice (
                            int           inputLineNumber,
                            const std::string& currentMeasureNumber);

    // figured bass

    S_msrVoice            createPartFiguredBassVoice (
                            int           inputLineNumber,
                            const std::string& currentMeasureNumber);

    void                  appendFiguredBassToPart (
                            S_msrVoice             figuredBassSupplierVoice,
                           S_msrFiguredBass figuredBass);

    void                  appendFiguredBassToPartClone (
                            S_msrVoice              figuredBassSupplierVoice,
                            S_msrFiguredBass figuredBass);

    // scordaturas

    void                  appendScordaturaToPart (
                            S_msrScordatura scordatura);

    // accordion registration

    void                  appendAccordionRegistrationToPart (
                            S_msrAccordionRegistration
                              accordionRegistration);

    // harp pedals tuning

    void                  appendHarpPedalsTuningToPart (
                            S_msrHarpPedalsTuning
                              harpPedalsTuning);

    // work around LilyPond issue #34

    void                  addSkipGraceNotesGroupAheadOfVoicesClonesIfNeeded (
                            S_msrVoice           graceNotesGroupOriginVoice,
                            S_msrGraceNotesGroup skipGraceNotesGroup);

/*
    void                  appendSkipGraceNotesToVoicesClones ( // JMI ???
                            S_msrVoice      graceNotesOriginVoice,
                            S_msrGraceNotes skipGraceNotes);

                            */

    // backup

    void                  handleBackupInPart (
                            int             inputLineNumber,
                            const Rational& backupStepLength);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;

    void                  printPartMeasuresWholeNotesDurationsVector (
                            std::ostream& os,
                            int      fieldWidth) const;

    void                  print (std::ostream& os) const override;

    void                  printShort (std::ostream& os) const override;

    void                  printSummary (std::ostream& os) const override;

    void                  printSlices (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks

    S_msrPartGroup        fPartUpLinkToPartGroup;

    // part ID and name

    std::string           fPartID; // native

    std::string           fPartMsrName;
                            // may be different than fPartID
                            // if renamed,
                            // coined in constructor

    std::string           fPartName; // from '<part-name/>'
    std::string           fPartNameDisplayText;

    std::string           fPartAbbreviation;
    std::string           fPartAbbreviationDisplayText;

    // part absolute number

    int                   fPartAbsoluteNumber;

    // part instrument names

    std::string           fPartInstrumentName;
    std::string           fPartInstrumentAbbreviation;

    // staves

    std::list<S_msrStaff>      fPartAllStavesList;

    std::list<S_msrStaff>      fPartRegularStavesList;

    std::list<S_msrStaff>      fPartNonHarmoniesNorFiguredBassStavesList;

    std::map<int, S_msrStaff>  fPartStaveNumbersToStavesMap;

    // harmonies

    S_msrStaff            fPartHarmoniesStaff;
    S_msrVoice            fPartHarmoniesVoice;

    // figured bass

    S_msrStaff            fPartFiguredBassStaff;
    S_msrVoice            fPartFiguredBassVoice;

    // voices

    std::list<S_msrVoice>      fPartAllVoicesList;

    // measures

    std::string           fPartCurrentMeasureNumber;

    size_t                fPartNumberOfMeasures;

    std::vector<Rational>      fPartMeasuresWholeNotesDurationsVector; // SUPERFLOUS ??? JMI NOEL

    // clef, key, time signature

    S_msrClef             fPartCurrentClef;

    S_msrKey              fPartCurrentKey;

    S_msrTimeSignature    fPartCurrentTimeSignature;

    // part shortest note

    // fPartShortestNoteDuration and fPartShortestNoteTupletFactor
    // are used to compute a number of divisions per quarter note
    // if needed, such as when generating MusicXML from MSR
    Rational              fPartShortestNoteDuration;
    msrTupletFactor       fPartShortestNoteTupletFactor;

    // transposition

    S_msrTransposition    fPartCurrentTransposition;

    // counter

    static int            gPartsCounter;

    // multiple full-bar rests

    Bool                  fPartContainsMultipleFullBarRests;

    // measures slices sequence

    S_msrMeasuresSlicesSequence
                          fPartMeasuresSlicesSequence;

  public:

    // public work services
    // ------------------------------------------------------

    // measure number

    void                  setPartCurrentMeasureNumber (
                            const std::string& measureNumber)
                              {
                                fPartCurrentMeasureNumber =
                                  measureNumber;
                              }

    const std::string          getPartCurrentMeasureNumber () const
                              { return fPartCurrentMeasureNumber; }

    // clef, key, time signature

    S_msrClef             getPartCurrentClef () const
                              { return fPartCurrentClef; }

    S_msrKey              getPartCurrentKey  () const
                              { return fPartCurrentKey; }

    void                  setPartCurrentTime (S_msrTimeSignature timeSignature)
                              { fPartCurrentTimeSignature = time; }

    S_msrTimeSignature    getPartCurrentTime () const
                              { return fPartCurrentTimeSignature; }

    // transposition

    S_msrTransposition    getPartCurrentTransposition () const
                              { return fPartCurrentTransposition; }

    // staff details

    S_msrStaffDetails     getCurrentPartStaffDetails () const
                              { return fCurrentPartStaffDetails; }

    // finalization

    void                  finalizeLastAppendedMeasureInPart (
                            int inputLineNumber);

    void                  finalizePart (
                            int inputLineNumber);

    void                  finalizePartClone (
                            int inputLineNumber);

    void                  finalizePartAndAllItsMeasures (
                            int inputLineNumber);

    void                  collectPartMeasuresSlices (
                            int inputLineNumber);

  private:

    // private work services
    // ------------------------------------------------------

    // staves

    void                  registerStaffInPart (
                            S_msrStaff staff);

    // figured bass

    static bool           compareStavesToHaveFiguredBassesBelowCorrespondingPart (
                            const S_msrStaff& first,
                            const S_msrStaff& second);

  private:

    // private work fields
    // ------------------------------------------------------

    // staff details

    S_msrStaffDetails     fCurrentPartStaffDetails;

    // position in measure

    Rational              fPartCurrentMeasurePosition;
};
typedef SMARTP<msrPart> S_msrPart;
EXP std::ostream& operator << (std::ostream& os, const S_msrPart& elt);


}


#endif
