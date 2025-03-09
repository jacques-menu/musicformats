/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrParts___
#define ___msrParts___

#include <map>

#include "msrPartGroupElements.h"

#include "msrTypesForwardDeclarations.h"

#include "msrClefsKeysTimeSignatures.h"
#include "msrPartGroups.h"
#include "msrRepeatsEnumTypes.h"
#include "msrStavesEnumTypes.h"
#include "msrStaves.h"
#include "msrTupletFactors.h"
#include "msrUseSymbols.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrPart : public msrPartGroupElement
{
  public:

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPart> create (
                            int                   inputLineNumber,
                            const std::string&    partID,
                            const S_msrPartGroup& partUpLinkToPartGroup);

    SMARTP<msrPart> createPartNewbornClone (
                            const S_msrPartGroup& partGroupClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrPart (
                            int                   inputLineNumber,
                            const std::string&    partID,
                            const S_msrPartGroup& partUpLinkToPartGroup);

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
                            const S_msrPartGroup& partGroup)
                              { fPartUpLinkToPartGroup = partGroup; }

    S_msrPartGroup        getPartUpLinkToPartGroup () const
                              { return fPartUpLinkToPartGroup; }

    // part absolute number

    int                   getPartAbsoluteNumber () const
                              { return fPartAbsoluteNumber; }

    // part ID

    void                  setPartID (const std::string& partID)
                              { fPartID = partID; }

    std::string           getPartID () const
                              { return fPartID; }

    // part names

    void                  setPartMsrName (const std::string& partMsrName);

    std::string           getPartMsrName () const
                              { return fPartMsrName; }

    void                  setPartName (const std::string& partName)
                              { fPartName = partName; }

    std::string           getPartName () const
                              { return fPartName; }

    std::string           getPartAlphabeticName () const
                              { return fPartAlphabeticName; }

    void                  setPartNameDisplayText (
                            const std::string& partNameDisplayText)
                              { fPartNameDisplayText = partNameDisplayText; }

    std::string           getPartNameDisplayText () const
                              { return fPartNameDisplayText; }

    void                  setPartAbbreviation (
                            const std::string& partAbbreviation)
                              { fPartAbbreviation = partAbbreviation; }

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

    std::string           getPartIDAndName () const;

    std::string           fetchPartCombinedName () const;

    void                  setPartInstrumentNamesMaxLengthes ();

    // voices

    const std::map <int, std::map <int, S_msrVoice>>&
                          getPartStaffVoicesMap () const
                              { return fPartStaffVoicesMap; }

//     const std::map <int, S_msrVoice>&
//                           getfPartRegularVoicesMap () const
//                               { return fPartRegularVoicesMap; }

//     const std::vector <std::vector <S_msrVoice>>&
//                           getPartStavesAndVoicesVector () const
//                               { return fPartStavesAndVoicesVector; }

    // regular voices counter

    void                  incrementPartRegularVoicesCounter ()
                              { ++fPartRegularVoicesCounter; }

    int                   getPartRegularVoicesCounter () const
                              { return fPartRegularVoicesCounter; }

    int                   getPartMinimumVoiceNumber () const
                              { return fPartMinimumVoiceNumber; }
    int                   getPartMaximumVoiceNumber () const
                              { return fPartMaximumVoiceNumber; }

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

    // measure number

    void                  setPartCurrentMeasureNumber (
                            const std::string& measureNumber)
                              {
                                fPartCurrentMeasureNumber =
                                  measureNumber;
                              }

    const std::string     getPartCurrentMeasureNumber () const
                              { return fPartCurrentMeasureNumber; }

    // clef, key, time signature

    S_msrClef             getPartCurrentClef () const
                              { return fPartCurrentClef; }

    S_msrKey              getPartCurrentKey  () const
                              { return fPartCurrentKey; }

    void                  setPartCurrentTimeSignature (
                            const S_msrTimeSignature& timeSignature)
                              { fPartCurrentTimeSignature = time; }

    S_msrTimeSignature    getPartCurrentTimeSignature () const
                              { return fPartCurrentTimeSignature; }

    // transposition

    S_msrTransposition    getPartCurrentTransposition () const
                              { return fPartCurrentTransposition; }

    // staff details

    S_msrStaffDetails     getCurrentPartStaffDetails () const
                              { return fCurrentPartStaffDetails; }

    void                  setPartNumberOfMeasures (
                            size_t partNumberOfMeasures);

    const size_t          getPartNumberOfMeasures () const
                              { return fPartNumberOfMeasures; }

    const std::vector <msrWholeNotes>&
                          getPartMeasuresWholeNotesVector () const
                              { return fPartMeasuresWholeNotesVector; }

    // harmonies staff and voice

    void                  setPartHarmoniesStaff (
                            const S_msrStaff& harmoniesStaff)
                              { fPartHarmoniesStaff = harmoniesStaff; }

    S_msrStaff            getPartHarmoniesStaff () const
                              { return fPartHarmoniesStaff; }

    void                  setPartHarmoniesVoice (
                            const S_msrVoice& harmoniesVoice)
                              { fPartHarmoniesVoice = harmoniesVoice; }

    S_msrVoice            getPartHarmoniesVoice () const
                              { return fPartHarmoniesVoice; }

    // figured bass staff and voice

    void                  setPartFiguredBassStaff (
                            const S_msrStaff& figuredBassStaff)
                              { fPartFiguredBassStaff = figuredBassStaff; }

    S_msrStaff            getPartFiguredBassStaff () const
                              { return fPartFiguredBassStaff; }

    void                  setPartFiguredBassVoice (
                            const S_msrVoice& figuredBassVoice)
                              { fPartFiguredBassVoice = figuredBassVoice; }

    S_msrVoice            getPartFiguredBassVoice () const
                              { return fPartFiguredBassVoice; }

    // staves map

    const std::map <int, S_msrStaff>&
                          getPartStavesMap () const
                              { return fPartStavesMap; }

    // part drawing measure position

    void                  setPartCurrentDrawingPositionInMeasure (
                            int                  inputLineNumber,
                            const msrWholeNotes& positionInMeasure);

    void                  resetPartCurrentDrawingPositionInMeasure (
                            int inputLineNumber);

    void                  incrementPartCurrentDrawingPositionInMeasure (
                            int                  inputLineNumber,
                            const msrWholeNotes& wholeNotesDelta);

    void                  decrementPartCurrentDrawingPositionInMeasure (
                            int                  inputLineNumber,
                            const msrWholeNotes& wholeNotesDelta);

    msrWholeNotes         getPartCurrentDrawingPositionInMeasure () const
                              { return fPartCurrentDrawingPositionInMeasure; }

    // part shortest note

    void                  setPartShortestNoteWholeNotes (
                            const msrWholeNotes& wholeNotes);

    msrWholeNotes         getPartShortestNoteWholeNotes () const
                              { return fPartShortestNoteWholeNotes; }

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

    msrWholeNotes         fetchPartMeasuresWholeNotesVectorAt (
                            int inputLineNumber,
                            int indexValue) const;

    void                  registerOrdinalMeasureNumberWholeNotes (
                            int                  inputLineNumber,
                            int                  measureOrdinalNumber,
                            const msrWholeNotes& wholeNotes);

    // path shortest note

    void                  registerShortestNoteInPartIfRelevant (
                            const S_msrNote& note);

    // measures

    void                  cascadeCreateAMeasureAndAppendItInPart (
                            int                    inputLineNumber,
                            int                    previousMeasureEndInputLineNumber,
                            const std::string&     measureNumber,
                            msrMeasureImplicitKind measureImplicitKind);

    void                  setNextMeasureNumberInPart (
                            int                inputLineNumber,
                            const std::string& nextMeasureNumber);

    // clef, key, time signature

//     void                  appendClefKeyTimeSignatureGroupToPart (
//                             const S_msrClefKeyTimeSignatureGroup& clefKeyTimeSignatureGroup);

    void                  appendClefToPart (
                            int              groupInputLineNumber,
                            const S_msrClef& clef);

    void                  appendKeyToPart (
                            int             groupInputLineNumber,
                            const S_msrKey& key);

    void                  appendTimeSignatureToPart (
                            int                       groupInputLineNumber,
                            const S_msrTimeSignature& timeSignature);

//     void                  appendTimeSignatureToPartClone (
//                             const S_msrTimeSignature& timeSignature);

    void                  appendClefKeyTimeSignatureGroupToPartClone (
                            const S_msrClefKeyTimeSignatureGroup& clefKeyTimeSignatureGroup);

    // tempo

    void                  appendTempoToPart (
                            const S_msrTempo& tempo);

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
                            const S_msrStaff& staff);

    S_msrStaff            fetchStaffFromPart (int staffNumber);

    void                  sortStavesByIncreasingNumber ();

    // voices

    void                  registerVoiceInPartVoicesList (
                            const S_msrVoice& voice);

    void                  registerVoiceInPartStaffVoicesMap (
                            const S_msrVoice& voice);

//     void                  registerVoiceInRegularVoicesMap (
//                             const S_msrVoice& voice);

    void                  displayPartStaffVoicesMap (
                            int                inputLineNumber,
                            const std::string& context) const;

//     void                  displayPartRegularVoicesMap (
//                             int                inputLineNumber,
//                             const std::string& context) const;

//     void                  displayPartStavesAndVoicesVector (
//                             int                inputLineNumber,
//                             const std::string& context) const;

    // rehearsal marks

    void                  appendRehearsalMarkToPart (
                            const S_msrRehearsalMark& rehearsalMark);

    // transposition

    void                  appendTranspositionToPart (
                            const S_msrTransposition& transposition);

    // staff details

    void                  appendStaffDetailsToPart (
                            const S_msrStaffDetails& staffDetails);

    // tuplets JMI ???

    // bar lines

    void                  appendBarLineToPart (
                            const S_msrBarLine& barLine);

    void                  insertHiddenMeasureAndBarLineInPartClone (
                            int             inputLineNumber,
                            const msrWholeNotes& positionInMeasure);

    // breaks

    void                  appendLineBreakToPart (
                            const S_msrLineBreak& lineBreak);
    void                  appendPageBreakToPart (
                            const S_msrPageBreak& pageBreak);

    // repeats

    void                  handleRepeatStartInPart (
                            int inputLineNumber);

    void                  handleRepeatEndInPart (
                            int                inputLineNumber,
                            const std::string& measureNumber,
                            int                repeatTimes);

    void                  handleRepeatEndingStartInPart (
                            int inputLineNumber);

    void                  handleRepeatEndingEndInPart (
                            int                 inputLineNumber,
                            const std::string&  repeatEndingNumber, // may be "1, 2"
                            msrRepeatEndingKind repeatEndingKind);

    /* JMI ???
    void                  finalizeRepeatEndInPart (
                            int    inputLineNumber,
                            const std::string& measureNumber,
                            int    repeatTimes);
                            */

    void                  appendRepeatCloneToPart (
                            int                inputLineNumber,
                            const S_msrRepeat& repeatCLone);

    void                  appendRepeatEndingCloneToPart (
                            const S_msrRepeatEnding& repeatEndingClone);

    // multiple measure rests

    void                  cascadeAppendMultipleMeasureRestToPart (
                            int               inputLineNumber,
                            int               multipleMeasureRestMeasuresNumber,
                            int               multipleMeasureRestSlashesNumber,
                            msrUseSymbolsKind multipleMeasureRestUseSymbolsKind);

    void                  appendPendingMultipleMeasureRestsToPart (
                            int inputLineNumber);

    void                  replicateLastAppendedMeasureInPart (
                            int inputLineNumber,
                            int replicatasNumber);

    void                  appendEmptyMeasuresToPart (
                            int                inputLineNumber,
                            const std::string& previousMeasureNumber,
                            int                measureRestsNumber);

    void                  appendMultipleMeasureRestCloneToPart (
                            int                          inputLineNumber,
                            const S_msrMultipleMeasureRest& multipleMeasureRests);

    // measure repeats

    void                  cascadeCreateAMeasureRepeatAndAppendItToPart (
                            int inputLineNumber,
                            int measureRepeatMeasuresNumber,
                            int measureRepeatSlashesNumber);

    void                  cascadeCreateAMeasureRepeatAndAppendItToPart (
                            int                    inputLineNumber,
                            int                    previousMeasureEndInputLineNumber,
                            const std::string&     measureNumber,
                            msrMeasureImplicitKind measureImplicitKind);

    void                  appendPendingMeasureRepeatToPart (
                            int inputLineNumber);

//     void                  appendMeasureRepeatCloneToPart ( JMI UNUSED v0.9.66
//                             int                              inputLineNumber,
//                             const S_msrMultipleMeasureRest& multipleMeasureRests);

    // frames

    // JMI ???

    // harmonies

    S_msrVoice            createPartHarmoniesVoice (
                            int                inputLineNumber,
                            const std::string& currentMeasureNumber);

    void                  appendHarmonyToPart (
                            int                  inputLineNumber,
                            const S_msrHarmony&  harmony,
                            const msrWholeNotes& positionInMeasureToAppendAt);

    void                  appendHarmoniesListToPart (
                            int                             inputLineNumber,
                            const std::list <S_msrHarmony>& harmoniesList,
                            const msrWholeNotes&            positionInMeasureToAppendAt);

    // figured bass

    S_msrVoice            createPartFiguredBassVoice (
                            int                inputLineNumber,
                            const std::string& currentMeasureNumber);

    void                  appendFiguredBassToPart (
                            int                     inputLineNumber,
                            const S_msrFiguredBass& figuredBass,
                            const msrWholeNotes&    positionInMeasureToAppendAt);

    void                  appendFiguredBassesListToPart (
                            int                                inputLineNumber,
                            const std::list <S_msrFiguredBass>& figuredBasssesList,
                            const msrWholeNotes&               positionInMeasureToAppendAt);

//     void                  appendFiguredBassToPart ( // JMI v0.9.67 HARMFUL
//                             const S_msrVoice&       figuredBassSupplierVoice,
//                             const S_msrFiguredBass& figuredBass);

    void                  appendFiguredBassToPartClone (
                            const S_msrVoice&       figuredBassSupplierVoice,
                            const S_msrFiguredBass& figuredBass);

    // scordaturas

    void                  appendScordaturaToPart (
                            const S_msrScordatura& scordatura);

    // accordion registration

    void                  appendAccordionRegistrationToPart (
                            const S_msrAccordionRegistration&
                              accordionRegistration);

    // harp pedals tuning

    void                  appendHarpPedalsTuningToPart (
                            const S_msrHarpPedalsTuning&
                              harpPedalsTuning);

    // work around LilyPond_Issue_34

    void                  addSkipGraceNotesGroupAheadOfVoicesClonesIfNeeded (
                            const S_msrVoice&           graceNotesGroupOriginVoice,
                            const S_msrGraceNotesGroup& skipGraceNotesGroup);

//     // backup
//
//     void                  handleBackupInPart (
//                             int             inputLineNumber,
//                             const msrWholeNotes& backupStepLength);
//
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

    void                  printPartMeasuresWholeNotesVector (
                            std::ostream&      os,
                            int                fieldWidth, // USEFUL ??? JMI v0.9.67
                            const std::string& context) const;

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

    void                  printSummary (std::ostream& os) const override;

    void                  printSlices (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks

    S_msrPartGroup        fPartUpLinkToPartGroup;

    // part ID and names

    std::string           fPartID; // native

    std::string           fPartMsrName;
                            // may be different than fPartID if renamed,
                            // coined in constructor

    std::string           fPartName;           // from '<part-name/>'
    std::string           fPartAlphabeticName; // from '<part-name/>'

    std::string           fPartNameDisplayText;

    std::string           fPartAbbreviation;
    std::string           fPartAbbreviationDisplayText;

    // part absolute number

    int                   fPartAbsoluteNumber;

    // part instrument names

    std::string           fPartInstrumentName;
    std::string           fPartInstrumentAbbreviation;

    // parts counter

    static int            sPartsCounter;

    // clef, key, time signature

    S_msrClef             fPartCurrentClef;

    S_msrKey              fPartCurrentKey;

    S_msrTimeSignature    fPartCurrentTimeSignature;

    // staves

    std::list <S_msrStaff> fPartAllStavesList;

    std::list <S_msrStaff> fPartRegularStavesList;

    std::list <S_msrStaff> fPartNonHarmoniesNorFiguredBassStavesList;

    std::map <int, S_msrStaff>
                          fPartStavesMap;

    // voices

    std::list <S_msrVoice>
                          fPartVoicesList;

    int                   fPartRegularVoicesCounter;

    int                   fPartMinimumVoiceNumber;
    int                   fPartMaximumVoiceNumber;

    // indexes are staff number and voice number
    std::map <int, std::map <int, S_msrVoice>>
                          fPartStaffVoicesMap;
//     std::map <int, S_msrVoice>
//                           fPartRegularVoicesMap;

//     std::vector <std::vector <S_msrVoice>>
//                           fPartStavesAndVoicesVector;
    // measures

    std::string           fPartCurrentMeasureNumber;

    size_t                fPartNumberOfMeasures;

    std::vector <msrWholeNotes>
                          fPartMeasuresWholeNotesVector;

    // harmonies

    S_msrStaff            fPartHarmoniesStaff;
    S_msrVoice            fPartHarmoniesVoice;

    // figured bass

    S_msrStaff            fPartFiguredBassStaff;
    S_msrVoice            fPartFiguredBassVoice;

    // part shortest note

    // fPartShortestNoteWholeNotes and fPartShortestNoteTupletFactor
    // are used to compute a number of divisions per quarter note
    // if needed, such as when generating MusicXML from MSR
    msrWholeNotes         fPartShortestNoteWholeNotes;
    msrTupletFactor       fPartShortestNoteTupletFactor;

    // transposition

    S_msrTransposition    fPartCurrentTransposition;

    // multiple measure rests

    Bool                  fPartContainsMultipleMeasureRests;

    // measures slices sequence

    S_msrMeasuresSlicesSequence
                          fPartMeasuresSlicesSequence;

  public:

    // public work services
    // ------------------------------------------------------

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
                            const S_msrStaff& staff);

  private:

    // private work fields
    // ------------------------------------------------------

    // staff details

    S_msrStaffDetails     fCurrentPartStaffDetails;

    // current drawing measure position, for mxsr2msr

    msrWholeNotes         fPartCurrentDrawingPositionInMeasure;
};
typedef SMARTP<msrPart> S_msrPart;
EXP std::ostream& operator << (std::ostream& os, const S_msrPart& elt);


}


#endif // ___msrParts___
