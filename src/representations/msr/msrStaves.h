/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrStaves___
#define ___msrStaves___

#include "msrElements.h"

#include "msrClefKeyTimeSignatureGroups.h"
#include "msrRepeats.h"
#include "msrRepeatsEnumTypes.h"
#include "msrStavesDetails.h"
#include "msrStavesEnumTypes.h"
#include "msrTranspositions.h"
#include "msrTupletFactors.h"
#include "msrUseSymbols.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrStaff : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStaff> create (
                            const mfInputLineNumber& inputLineNumber,
                            msrStaffKind     staffKind,
                            const mfStaffNumber& staffNumber);

    static SMARTP<msrStaff> create (
                            const mfInputLineNumber& inputLineNumber,
                            msrStaffKind     staffKind,
                            const mfStaffNumber& staffNumber,
                            const S_msrPart& staffUpLinkToPart);

    SMARTP<msrStaff> createStaffNewbornClone (
                            const S_msrPart& containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrStaff (
                            const mfInputLineNumber& inputLineNumber,
                            msrStaffKind     staffKind,
                            const mfStaffNumber& staffNumber);

    virtual               ~msrStaff ();

  private:

    // private initialization
    // ------------------------------------------------------

    void                  initializeStaff ();

    void                  copyStuffFromUpLinkToPartToStaff ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLink

    void                  setStaffUpLinkToPart (const S_msrPart& part);

    S_msrPart             getStaffUpLinkToPart () const
                              { return fStaffUpLinkToPart; }

    // staff kind

    msrStaffKind          getStaffKind () const
                              { return fStaffKind; }

    // staff path-like name

    std::string           getStaffPathLikeName () const
                              { return fStaffPathLikeName; }

    // staff number and names

    void                  setStaffNumber (const mfStaffNumber& staffNumber)
                              { fStaffNumber = staffNumber; }

    mfStaffNumber         getStaffNumber () const
                              { return fStaffNumber; }

    std::string           getStaffInstrumentName () const
                              { return fStaffInstrumentName; }

    std::string           getStaffInstrumentAbbreviation () const
                              { return fStaffInstrumentAbbreviation; }

    // voices

    int                   getStaffRegularVoicesCounter () const
                              { return fStaffRegularVoicesCounter; }

    // staff details

    S_msrStaffDetails     getCurrentStaffStaffDetails () const
                              { return fCurrentStaffStaffDetails; }

    // staff voices

    const std::map <mfVoiceNumber, S_msrVoice>&
                          getStaffAllVoicesMap () const
                              { return fStaffAllVoicesMap; }

    const std::map <mfVoiceNumber, S_msrVoice>&
                          getStaffRegularVoicesMap () const
                              { return fStaffRegularVoicesMap; }

    const std::list <S_msrVoice>&
                          getStaffRegularVoicesList () const
                              { return fStaffRegularVoicesList; }

    const std::list <S_msrVoice>&
                          getStaffAllVoicesList () const
                              { return fStaffAllVoicesList; }

    // staff shortest note

    void                  setStaffShortestNoteWholeNotes (
                            const mfWholeNotes& wholeNotes);

    mfWholeNotes          getStaffShortestNoteWholeNotes () const
                              { return fStaffShortestNoteWholeNotes; }

    void                  setStaffShortestNoteTupletFactor (
                            const msrTupletFactor& noteTupletFactor);

    const msrTupletFactor&
                          getStaffShortestNoteTupletFactor () const
                              { return fStaffShortestNoteTupletFactor; }


    const std::vector <std::list <S_msrMeasure>>&
                          getStaffMeasuresFlatListsVector () const
                              { return fStaffMeasuresFlatListsVector; }

    // measures slices sequence

    S_msrMeasuresSlicesSequence
                          getStaffMeasuresSlicesSequence () const
                              { return fStaffMeasuresSlicesSequence; }

  public:

    // comparisons
    // ------------------------------------------------------

    // staves ordering in parts
    static bool           compareStavesByIncreasingNumber (
                            const S_msrStaff& first,
                            const S_msrStaff& second);

    // figured bass // JMI 0.9.72
    static bool           compareStavesToHaveFiguredBassesBelowCorrespondingPart (
                            const S_msrStaff& first,
                            const S_msrStaff& second);

  public:

    // public services
    // ------------------------------------------------------

    // upLinks

    S_msrPartGroup        fetchStaffUpLinkToPartGroup () const;

    S_msrScore            fetchStaffUpLinkToScore () const;

    // part shortest note

    void                  registerShortestNoteInStaffIfRelevant (const S_msrNote& note);

    // staff details

    void                  appendStaffDetailsToStaff (
                            const S_msrStaffDetails& staffDetails);

    // clef, key, time signature

//     void                  appendClefKeyTimeSignatureGroupToStaff (
//                             const S_msrClefKeyTimeSignatureGroup& clefKeyTimeSignatureGroup);

    void                  appendClefToStaff (
                            int              groupInputLineNumber,
                            const S_msrClef& clef);

    void                  appendKeyToStaff (
                            int             groupInputLineNumber,
                            const S_msrKey& key);

    void                  appendTimeSignatureToStaff (
                            int                       groupInputLineNumber,
                            const S_msrTimeSignature& timeSignature);

//     void                  appendTimeSignatureToStaffClone (
//                             const S_msrTimeSignature& timeSignature);

    void                  appendClefKeyTimeSignatureGroupToStaffClone (
                            const S_msrClefKeyTimeSignatureGroup& clefKeyTimeSignatureGroup);

    // tempo

    void                  appendTempoToStaff (
                            const S_msrTempo& tempo);

    // rehearsal marks

    void                  appendRehearsalMarkToStaff (
                            const S_msrRehearsalMark& rehearsalMark);

    // breaks

    void                  appendLineBreakToStaff (
                            const S_msrLineBreak& lineBreak);
    void                  appendPageBreakToStaff (
                            const S_msrPageBreak& pageBreak);

    // dal segno

    void                  insertHiddenMeasureAndBarLineInStaffClone (
                            const mfInputLineNumber& inputLineNumber,
                            const mfPositionInMeasure& positionInMeasure);

    // transposition

    void                  appendTranspositionToStaff (
                            const S_msrTransposition& transpose);

    // voices

    S_msrVoice            createRegularVoiceInStaffByItsNumber (
                            const mfInputLineNumber& inputLineNumber,
                            const mfVoiceNumber&     voiceNumber,
                            const mfMeasureNumber&   currentMeasureNumber,
                            const S_msrPartGroup&    partUpLinkToPartGroup);

    void                  registerVoiceInStaff (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrVoice& voice);

    void                  registerVoiceInStaffClone (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrVoice& voice);

    S_msrVoice            fetchRegularVoiceFromStaffByItsNumber (
                            const mfInputLineNumber& inputLineNumber,
                            const mfVoiceNumber& voiceNumber);

    S_msrVoice            fetchFirstRegularVoiceFromStaff (
                            const mfInputLineNumber& inputLineNumber);

    void                  assignSequentialNumbersToRegularVoicesInStaff (
                            const mfInputLineNumber& inputLineNumber);

    // measures

    void                  cascadeCreateAMeasureAndAppendItInStaff (
                            const mfInputLineNumber& inputLineNumber,
                            int                    previousMeasureEndInputLineNumber,
                            const mfMeasureNumber& measureNumber,
                            msrMeasureImplicitKind measureImplicitKind);

    void                  cascadeNetNextMeasureNumberInStaff (
                            const mfInputLineNumber& inputLineNumber,
                            const mfMeasureNumber&   nextMeasureNumber);

    // measure repeats

    void                  cascadeCreateAMeasureRepeatAndAppendItToStaff (
                            const mfInputLineNumber& inputLineNumber,
                            int measureRepeatMeasuresNumber,
                            int measureRepeatSlashesNumber);

    void                  appendPendingMeasureRepeatToStaff (
                            const mfInputLineNumber& inputLineNumber);

    // measure rests

    // repeats

    void                  cascadeHandleRepeatStartInStaff (
                            const mfInputLineNumber& inputLineNumber);

    void                  cascadeHandleRepeatEndInStaff (
                            const mfInputLineNumber& inputLineNumber,
                            const mfMeasureNumber&   measureNumber,
                            int                      repeatTimes);

    void                  cascadeHandleRepeatEndingStartInStaff (
                            const mfInputLineNumber& inputLineNumber);

    void                  cascadeHandleRepeatEndingEndInStaff (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string&       repeatEndingNumber, // may be "1, 2"
                            msrRepeatEndingKind      repeatEndingKind);

/* JMI
    void                  finalizeRepeatEndInStaff (
                            const mfInputLineNumber& inputLineNumber,
                            const mfMeasureNumber& measureNumber,
                            int                repeatTimes);
    */

    // multiple measure rests

    void                  cascadeAppendMultipleMeasureRestToStaff (
                            const mfInputLineNumber& inputLineNumber,
                            int               multipleMeasureRestMeasuresNumber,
                            int               multipleMeasureRestSlashesNumber,
                            msrUseSymbolsKind multipleMeasureRestUseSymbolsKind);

    void                  appendPendingMultipleMeasureRestsToStaff (
                            const mfInputLineNumber& inputLineNumber);

    // other

    void                  replicateLastAppendedMeasureInStaff (
                            const mfInputLineNumber& inputLineNumber,
                            int replicatasNumber);

    void                  cascadeAppendEmptyMeasuresToStaff (
                            const mfInputLineNumber& inputLineNumber,
                            const mfMeasureNumber&   previousMeasureNumber,
                            int                      emptyMeasuresNumber);

    void                  appendMultipleMeasureRestCloneToStaff (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMultipleMeasureRest& multipleMeasureRests);

    void                  appendRepeatCloneToStaff (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrRepeat& repeatCLone);

    void                  appendRepeatEndingCloneToStaff (
                            const S_msrRepeatEnding& repeatEndingClone);

    // beat repeats
    void                  createABeatRepeatFromItsFirstMeasuresInStaff ( // JMI 0.9.72
                            const mfInputLineNumber& inputLineNumber,
                            int beatRepeatMeasuresNumber,
                            int beatRepeatSlashesNumber);

    // bar lines

    void                  appendBarLineToStaff (
                            const S_msrBarLine& barLine);

    // transposition

    void                  appendTranspositionToAllStaffVoices ( // JMI
                            const S_msrTransposition& transpose);

    // scordaturas

    void                  appendScordaturaToStaff (
                            const S_msrScordatura& scordatura);

    // accordion registration

    void                  appendAccordionRegistrationToStaff (
                            const S_msrAccordionRegistration&
                              accordionRegistration);

    // harp pedals tuning

    void                  appendHarpPedalsTuningToStaff (
                            const S_msrHarpPedalsTuning& harpPedalsTuning);

    // strings

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    // staff number

    std::string           fetchStaffNumberAsString () const;

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

    void                  printSummary (std::ostream& os) const override;

    void                  printSlices (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks

    S_msrPart             fStaffUpLinkToPart;

    // staff path-like name
    std::string           fStaffPathLikeName;

    // staff kind
    msrStaffKind          fStaffKind;

    // staff number
    mfStaffNumber         fStaffNumber;

    // staff instrument name
    std::string           fStaffInstrumentName;
    std::string           fStaffInstrumentAbbreviation;

    /*
      There are 'regular', harmonies and figured voices.
      A voice in and 'All' or 'Any' data structure can be any of these.
    */

    // the staff regular (i.e. not harmonies nor figured bass) voices
    static int            sStaffMaxRegularVoices;

    // we need to sort the voices by increasing voice numbers,
    // but with harmonies voices right before the corresponding regular voices
    std::list <S_msrVoice> fStaffAllVoicesList;

    // harmonies and figured bass elements should be placed
    // in the first regular voice of the staff, hence:
    std::list <S_msrVoice> fStaffRegularVoicesList;

    // the mapping of all the voices in the staff,
    // including harmonies and figured bass voices
    std::map <mfVoiceNumber, S_msrVoice>
                          fStaffAllVoicesMap;

    // the mapping of voice numbers to regular voices
    std::map <mfVoiceNumber, S_msrVoice>
                          fStaffRegularVoicesMap;

    // part shortest note

    // fStaffShortestNoteWholeNotes and fStaffShortestNoteTupletFactor
    // are used in the time-oriented handling of staff measures slices
    mfWholeNotes          fStaffShortestNoteWholeNotes;
    msrTupletFactor       fStaffShortestNoteTupletFactor;

    // measures flat vector
    // i.e. without segments nor repeats,
    // gathered from the staff's voices fVoiceInitialElementsList and fVoiceLastSegment
    // by finalizeMeasure()
    std::vector <std::list <S_msrMeasure>>
                          fStaffMeasuresFlatListsVector;

    // multiple measure rests

    Bool                  fStaffContainsMultipleMeasureRests;

    // measures slices sequence

    S_msrMeasuresSlicesSequence
                          fStaffMeasuresSlicesSequence;

  public:

    // public work services
    // ------------------------------------------------------

    // clef, key, time signature

    void                  setStaffCurrentClef (
                            const S_msrClef& clef);

    S_msrClef             getStaffCurrentClef () const
                              { return fStaffCurrentClef; }

    void                  setStaffCurrentKey (
                            const S_msrKey& key);

    S_msrKey              getStaffCurrentKey  () const
                              { return fStaffCurrentKey; }

    void                  setStaffCurrentTimeSignature (
                            const S_msrTimeSignature& timeSignature);

    S_msrTimeSignature    getStaffCurrentTimeSignature () const
                              { return fStaffCurrentTimeSignature; }

    // finalization

    void                  finalizeLastAppendedMeasureInStaff (
                            const mfInputLineNumber& inputLineNumber);

    void                  finalizeStaff (
                            const mfInputLineNumber& inputLineNumber);

    void                  collectStaffMeasuresIntoFlatListsVector (
                            const mfInputLineNumber& inputLineNumber);

    void                  collectStaffMeasuresSlices (
                            const mfInputLineNumber& inputLineNumber);

  private:

    // private work services
    // ------------------------------------------------------

    // voice registration

    void                  registerVoiceInStaffAllVoicesList (
                            const S_msrVoice& voice);

    void                  registerVoiceByItsNumber (
                            const mfInputLineNumber& inputLineNumber,
                            const mfStaffNumber& staffNumber,
                            const S_msrVoice& voice);

    void                  registerRegularVoiceByItsNumber (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrVoice& regularVoice,
                            const mfVoiceNumber&     voiceNumber);

    void                  registerHarmoniesVoiceByItsNumber (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrVoice& voice);

    void                  registerFiguredBassVoiceByItsNumber (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrVoice& voice);

    void                  registerPartLevelVoiceInStaff ( // JMI rename for harmonies and figured bass ???
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrVoice& voice);

    // transposition

    S_msrTransposition    getStaffCurrentTransposition () const
                              { return fStaffCurrentTransposition; }

  private:

    // private work fields
    // ------------------------------------------------------

    // we need to handle the regular voice specifically
    // to assign them sequencing numbers from 1 to gMaxStaffVoices,
    // needed to set the beams orientation (up or down)
    int                   fStaffRegularVoicesCounter;

    // clef, key, time signature

    S_msrClef             fStaffCurrentClef;

    S_msrKey              fStaffCurrentKey;

    S_msrTimeSignature    fStaffCurrentTimeSignature;

    S_msrClefKeyTimeSignatureGroup
                          fCurrentClefKeyTimeSignatureGroup;

    // staff details

    S_msrStaffDetails     fCurrentStaffStaffDetails;

    // transposition

    S_msrTransposition    fStaffCurrentTransposition;
};

using S_msrStaff = SMARTP<msrStaff>;

EXP std::ostream& operator << (std::ostream& os, const S_msrStaff& elt);


}


#endif // ___msrStaves___
