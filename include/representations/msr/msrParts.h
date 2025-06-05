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

#include "msrClefKeyTimeSignatureGroups.h"
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

    // creation
    // ------------------------------------------------------

    static SMARTP<msrPart> create (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& partMusicXMLID);

    static SMARTP<msrPart> create (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string&    partMusicXMLID,
                            const S_msrPartGroup& partUpLinkToPartGroup);

    SMARTP<msrPart> createPartNewbornClone (
                            const S_msrPartGroup& partGroupClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrPart (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& partMusicXMLID);

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

    // part sequential number

    int                   getPartSequentialNumber () const
                              { return fPartSequentialNumber; }

    // part MusicXML ID

    void                  setPartMusicXMLID (const std::string& partMusicXMLID)
                              { fPartMusicXMLID = partMusicXMLID; }

    std::string           getPartMusicXMLID () const
                              { return fPartMusicXMLID; }

    // part path-like name

    void                  setPartPathLikeName (const std::string& partPathLikeName);

    std::string           getPartPathLikeName () const
                              { return fPartPathLikeName; }

    // part names

    void                  setPartName (const std::string& partName)
                              { fPartName = partName; }

    std::string           getPartName () const
                              { return fPartName; }

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

    std::string           fetchPartIDAndName () const;

    std::string           fetchPartNameForTrace () const;

    void                  setPartInstrumentNamesMaxLengthes ();

    // staves

    const std::map <int, S_msrStaff>&
                          getPartStavesMap () const
                              { return fPartStavesMap; }

    std::map <int, S_msrStaff>*
                          getPartStavesMapPtr ()
                              { return &fPartStavesMap; }


    // voices

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

//     void                  setPartFirstMeasureNumber (
//                             const std::string& measureNumber)
//                               { fPartFirstMeasureNumber = measureNumber; }

    const std::string     getPartFirstMeasureNumber () const
                              { return fPartFirstMeasureNumber; }

    void                  setPartCurrentMeasureNumber (
                            const std::string& measureNumber);

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

    const std::vector <mfWholeNotes>&
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

    // part drawing measure position

    void                  setPartCurrentDrawingPositionInMeasure (
                            const mfInputLineNumber& inputLineNumber,
                            const mfPositionInMeasure& positionInMeasure);

    void                  resetPartCurrentDrawingPositionInMeasure (
                            const mfInputLineNumber& inputLineNumber);

    void                  incrementPartCurrentDrawingPositionInMeasure (
                            const mfInputLineNumber& inputLineNumber,
                            const mfWholeNotes& wholeNotesDelta);

    void                  decrementPartCurrentDrawingPositionInMeasure (
                            const mfInputLineNumber& inputLineNumber,
                            const mfWholeNotes& wholeNotesDelta);

    mfPositionInMeasure   getPartCurrentDrawingPositionInMeasure () const
                              { return fPartCurrentDrawingPositionInMeasure; }

    // part shortest note

    void                  setPartShortestNoteWholeNotes (
                            const mfWholeNotes& wholeNotes);

    mfWholeNotes          getPartShortestNoteWholeNotes () const
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
                            const mfInputLineNumber& inputLineNumber);

/* JMI
    void                  addAVoiceToStavesThatHaveNone (
                            const mfInputLineNumber& inputLineNumber);
*/

    // whole notes durations

    mfWholeNotes          fetchPartMeasuresWholeNotesVectorAt (
                            const mfInputLineNumber& inputLineNumber,
                            int indexValue) const;

    void                  registerOrdinalMeasureNumberWholeNotes (
                            const mfInputLineNumber& inputLineNumber,
                            int                 measureOrdinalNumber,
                            const mfWholeNotes& wholeNotes);

    // path shortest note

    void                  registerShortestNoteInPartIfRelevant (
                            const S_msrNote& note);

    // measures

    void                  cascadeCreateAMeasureAndAppendItInPart (
                            const mfInputLineNumber& inputLineNumber,
                            int                    previousMeasureEndInputLineNumber,
                            const std::string&     measureNumber,
                            msrMeasureImplicitKind measureImplicitKind);

    void                  setNextMeasureNumberInPart (
                            const mfInputLineNumber& inputLineNumber,
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

    S_msrStaff            addRegularStaffToPartByItsNumber (
                            const mfInputLineNumber& inputLineNumber,
                            msrStaffKind staffKind,
                            int          staffNumber);

    S_msrStaff            addHarmoniesStaffToPart (
                            const mfInputLineNumber& inputLineNumber);

    S_msrStaff            addHFiguredBassStaffToPart (
                            const mfInputLineNumber& inputLineNumber);

    void                  addStaffToPartCloneByItsNumber (
                            const S_msrStaff& staff);

    S_msrStaff            fetchStaffFromPart (int staffNumber);

    void                  sortStavesByIncreasingNumber ();

    // voices

    void                  registerVoiceInPartVoicesList (
                            const S_msrVoice& voice);

//     void                  registerVoiceInRegularVoicesMap (
//                             const S_msrVoice& voice);

    void                  displayPartStavesMap (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& context) const;

//     void                  displayPartRegularVoicesMap (
//                             const mfInputLineNumber& inputLineNumber,
//                             const std::string& context) const;

//     void                  displayPartStavesAndVoicesVector (
//                             const mfInputLineNumber& inputLineNumber,
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
                            const mfInputLineNumber& inputLineNumber,
                            const mfPositionInMeasure& positionInMeasure);

    // breaks

    void                  appendLineBreakToPart (
                            const S_msrLineBreak& lineBreak);
    void                  appendPageBreakToPart (
                            const S_msrPageBreak& pageBreak);

    // repeats

    void                  cascadeHandleRepeatStartInPart (
                            const mfInputLineNumber& inputLineNumber);

    void                  cascadeHandleRepeatEndInPart (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& measureNumber,
                            int                repeatTimes);

    void                  cascadeHandleRepeatEndingStartInPart (
                            const mfInputLineNumber& inputLineNumber);

    void                  cascadeHandleRepeatEndingEndInPart (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string&  repeatEndingNumber, // may be "1, 2"
                            msrRepeatEndingKind repeatEndingKind);

    /* JMI ???
    void                  finalizeRepeatEndInPart (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& measureNumber,
                            int    repeatTimes);
                            */

    void                  appendRepeatCloneToPart (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrRepeat& repeatCLone);

    void                  appendRepeatEndingCloneToPart (
                            const S_msrRepeatEnding& repeatEndingClone);

    // multiple measure rests

    void                  cascadeAppendMultipleMeasureRestToPart (
                            const mfInputLineNumber& inputLineNumber,
                            int               multipleMeasureRestMeasuresNumber,
                            int               multipleMeasureRestSlashesNumber,
                            msrUseSymbolsKind multipleMeasureRestUseSymbolsKind);

    void                  appendPendingMultipleMeasureRestsToPart (
                            const mfInputLineNumber& inputLineNumber);

    void                  replicateLastAppendedMeasureInPart (
                            const mfInputLineNumber& inputLineNumber,
                            int replicatasNumber);

    void                  appendEmptyMeasuresToPart (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& previousMeasureNumber,
                            int                measureRestsNumber);

    void                  appendMultipleMeasureRestCloneToPart (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMultipleMeasureRest& multipleMeasureRests);

    // measure repeats

    void                  cascadeCreateAMeasureRepeatAndAppendItToPart (
                            const mfInputLineNumber& inputLineNumber,
                            int measureRepeatMeasuresNumber,
                            int measureRepeatSlashesNumber);

    void                  cascadeCreateAMeasureRepeatAndAppendItToPart (
                            const mfInputLineNumber& inputLineNumber,
                            int                    previousMeasureEndInputLineNumber,
                            const std::string&     measureNumber,
                            msrMeasureImplicitKind measureImplicitKind);

    void                  appendPendingMeasureRepeatToPart (
                            const mfInputLineNumber& inputLineNumber);

//     void                  appendMeasureRepeatCloneToPart ( JMI UNUSED 0.9.66
//                             int                              inputLineNumber,
//                             const S_msrMultipleMeasureRest& multipleMeasureRests);

    // frames

    // JMI ???

    // harmonies

    S_msrVoice            createPartHarmoniesVoice (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& currentMeasureNumber);

    void                  appendHarmonyToPart (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrHarmony&        harmony,
                            const mfPositionInMeasure& positionInMeasureToAppendAt);

    void                  appendHarmoniesListToPart (
                            int                             inputLineNumber,
                            const std::list <S_msrHarmony>& harmoniesList,
                            const mfPositionInMeasure&      positionInMeasureToAppendAt);

    // figured bass

    S_msrVoice            createPartFiguredBassVoice (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& currentMeasureNumber);

    void                  appendFiguredBassToPart (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrFiguredBass&    figuredBass,
                            const mfPositionInMeasure& positionInMeasureToAppendAt);

    void                  cascadeAppendFiguredBassesListToPart (
                            int                                 inputLineNumber,
                            const std::list <S_msrFiguredBass>& figuredBasssesList,
                            const mfPositionInMeasure&          positionInMeasureToAppendAt);

//     void                  appendFiguredBassToPart ( // JMI 0.9.67 HARMFUL
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
//                             const mfInputLineNumber& inputLineNumber,
//                             const mfWholeNotes& backupStepLength);
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
                            int                fieldWidth, // USEFUL ??? JMI 0.9.67
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

    std::string           fPartMusicXMLID; // native

    std::string           fPartPathLikeName;
                            // we use a pathname analogy so identify
                            // parts, staves and voices,
                            // for example in the generated LilyPont code
                            // this name is coined in the constructor

    std::string           fPartName;           // from '<part-name/>'

    std::string           fPartNameDisplayText;

    std::string           fPartAbbreviation;
    std::string           fPartAbbreviationDisplayText;

    // part absolute number

    int                   fPartSequentialNumber;

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

    std::list <S_msrStaff>
                          fPartAllStavesList;

    std::list <S_msrStaff>
                          fPartRegularStavesList;

    std::map <int, S_msrStaff>
                          fPartStavesMap;

    std::list <S_msrStaff>
                          fPartNonHarmoniesNorFiguredBassStavesList;

    // voices

    std::list <S_msrVoice>
                          fPartVoicesList;

    int                   fPartRegularVoicesCounter;

    int                   fPartMinimumVoiceNumber;
    int                   fPartMaximumVoiceNumber;

//     std::map <int, S_msrVoice>
//                           fPartRegularVoicesMap;

//     std::vector <std::vector <S_msrVoice>>
//                           fPartStavesAndVoicesVector;
    // measures

    std::string           fPartFirstMeasureNumber;
    std::string           fPartCurrentMeasureNumber;

    size_t                fPartNumberOfMeasures;

    std::vector <mfWholeNotes>
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
    mfWholeNotes          fPartShortestNoteWholeNotes;
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
                            const mfInputLineNumber& inputLineNumber);

    void                  finalizePart (
                            const mfInputLineNumber& inputLineNumber);

    void                  finalizePartClone (
                            const mfInputLineNumber& inputLineNumber);

    void                  finalizePartAndAllItsMeasures (
                            const mfInputLineNumber& inputLineNumber);

    void                  collectPartMeasuresSlices (
                            const mfInputLineNumber& inputLineNumber);

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

    mfPositionInMeasure   fPartCurrentDrawingPositionInMeasure;
};

using S_msrPart = SMARTP<msrPart>;

EXP std::ostream& operator << (std::ostream& os, const S_msrPart& elt);


}


#endif // ___msrParts___
