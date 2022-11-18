/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrStaves___
#define ___msrStaves___

#include "msrElements.h"

#include "msrClefs.h"
#include "msrKeys.h"
#include "msrRepeats.h"
#include "msrRepeatsEnumTypes.h"
#include "msrStavesDetails.h"
#include "msrStavesEnumTypes.h"
#include "msrTimeSignatures.h"
#include "msrTranspositions.h"
#include "msrTupletFactors.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrStaff : public msrElement
{
  public:

    // constants
    // ------------------------------------------------------

    static const int K_NO_STAFF_NUMBER;

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStaff> create (
                            int          inputLineNumber,
                            msrStaffKind staffKind,
                            int          staffNumber,
                            const S_msrPart&    staffUpLinkToPart);

    SMARTP<msrStaff> createStaffNewbornClone (
                            const S_msrPart& containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrStaff (
                            int          inputLineNumber,
                            msrStaffKind staffKind,
                            int          staffNumber,
                            const S_msrPart&    staffUpLinkToPart);

    virtual               ~msrStaff ();

  private:

    // private initialization
    // ------------------------------------------------------

    void                  initializeStaff ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLink

    S_msrPart             getStaffUpLinkToPart () const
                              { return fStaffUpLinkToPart; }

    // staff kind

    msrStaffKind          getStaffKind () const
                              { return fStaffKind; }

    // staff number and name

    int                   getStaffNumber () const
                              { return fStaffNumber; }

    std::string           getStaffName () const
                              { return fStaffName; }

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

    const std::map<int, S_msrVoice>&
                          getStaffVoiceNumbersToAllVoicesMap () const
                              { return fStaffVoiceNumbersToAllVoicesMap; }

    const std::map<int, S_msrVoice>&
                          getStaffVoiceNumbersToRegularVoicesMap () const
                              { return fStaffVoiceNumbersToRegularVoicesMap; }

    const std::list<S_msrVoice>&
                          getStaffRegularVoicesList () const
                              { return fStaffRegularVoicesList; }

    const std::list<S_msrVoice>&
                          getStaffAllVoicesVector () const
                              { return fStaffAllVoicesList; }

    // staff shortest note

    void                  setStaffShortestNoteDuration (
                            const Rational& duration);

    Rational              getStaffShortestNoteDuration () const
                              { return fStaffShortestNoteDuration; }

    void                  setStaffShortestNoteTupletFactor (
                            const msrTupletFactor& noteTupletFactor);

    const msrTupletFactor&
                          getStaffShortestNoteTupletFactor () const
                              { return fStaffShortestNoteTupletFactor; }


    const std::vector<std::list<S_msrMeasure> >&
                          getStaffMeasuresFlatListsVector () const
                              { return fStaffMeasuresFlatListsVector; }

    // measures slices sequence

    S_msrMeasuresSlicesSequence
                          getStaffMeasuresSlicesSequence () const
                              { return fStaffMeasuresSlicesSequence; }

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

    void                  appendClefToStaff (S_msrClef clef);

    void                  appendKeyToStaff (S_msrKey  key);

    void                  appendTimeSignatureToStaff (S_msrTimeSignature timeSignature);
    void                  appendTimeSignatureToStaffClone (S_msrTimeSignature timeSignature);

    // tempo

    void                  appendTempoToStaff (S_msrTempo tempo);

    // rehearsal marks

    void                  appendRehearsalMarkToStaff (
                            const S_msrRehearsalMark& rehearsalMark);

    // breaks

    void                  appendLineBreakToStaff (S_msrLineBreak lineBreak);
    void                  appendPageBreakToStaff (S_msrPageBreak pageBreak);

    // dal segno

    void                  insertHiddenMeasureAndBarLineInStaffClone (
                            int             inputLineNumber,
                            const Rational& measurePosition);

    // transposition

    void                  appendTranspositionToStaff (
                            const S_msrTransposition& transpose);

    // voices

    S_msrVoice            createRegularVoiceInStaffByItsNumber (
                            int           inputLineNumber,
                            int           voiceNumber,
                            const std::string& currentMeasureNumber);

    void                  registerVoiceInStaff (
                            int        inputLineNumber,
                            const S_msrVoice& voice);

    void                  registerVoiceInStaffClone (
                            int        inputLineNumber,
                            const S_msrVoice& voice);

    S_msrVoice            fetchRegularVoiceFromStaffByItsNumber (
                            int inputLineNumber,
                            int voiceNumber);

    S_msrVoice            fetchFirstRegularVoiceFromStaff (
                            int inputLineNumber);

    void                  assignSequentialNumbersToRegularVoicesInStaff (
                            int inputLineNumber);

    // measures

    void                  createAMeasureAndAppendItToStaff (
                            int           inputLineNumber,
                            int           previousMeasureEndInputLineNumber,
                            const std::string& measureNumber,
                            msrMeasureImplicitKind
                                          measureImplicitKind);

    void                  setNextMeasureNumberInStaff (
                            int           inputLineNumber,
                            const std::string& nextMeasureNumber);

    // repeats

    void                  handleRepeatStartInStaff (
                            int inputLineNumber);

    void                  handleRepeatEndInStaff (
                            int           inputLineNumber,
                            const std::string& measureNumber,
                            int           repeatTimes);

    void                  handleRepeatEndingStartInStaff (
                            int inputLineNumber);

    void                  handleRepeatEndingEndInStaff (
                            int           inputLineNumber,
                            const std::string& repeatEndingNumber, // may be "1, 2"
                            msrRepeatEndingKind
                                          repeatEndingKind);

/* JMI
    void                  finalizeRepeatEndInStaff (
                            int           inputLineNumber,
                            const std::string& measureNumber,
                            int           repeatTimes);
    */

    void                  createMeasureRepeatFromItsFirstMeasuresInStaff (
                            int inputLineNumber,
                            int measureRepeatMeasuresNumber,
                            int measureRepeatSlashesNumber);

    void                  appendPendingMeasureRepeatToStaff (
                            int inputLineNumber);

    void                  appendMultipleFullBarRestsToStaff ( // JMI UNUSED
                            int inputLineNumber,
                            int multipleFullBarRestsMeasuresNumber);

    void                  appendPendingMultipleFullBarRestsToStaff (
                            int inputLineNumber);

    void                  replicateLastAppendedMeasureInStaff (
                            int inputLineNumber,
                            int replicatasNumber);

    void                  addEmptyMeasuresToStaff (
                            int           inputLineNumber,
                            const std::string& previousMeasureNumber,
                            int           emptyMeasuresNumber);

    void                  appendMultipleFullBarRestsCloneToStaff (
                            int                       inputLineNumber,
                            const S_msrMultipleFullBarRests& multipleFullBarRests);

    void                  createBeatRepeatFromItsFirstMeasuresInStaff (
                            int inputLineNumber,
                            int beatRepeatMeasuresNumber,
                            int beatRepeatSlashesNumber);

    void                  appendRepeatCloneToStaff (
                            int         inputLineNumber,
                            const S_msrRepeat& repeatCLone);

    void                  appendRepeatEndingCloneToStaff (
                            const S_msrRepeatEnding& repeatEndingClone);

    // bar lines

    void                  appendBarLineToStaff (S_msrBarLine barLine);

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

    std::string           staffNumberAsString () const;

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

    void                  printShort (std::ostream& os) const override;

    void                  printSummary (std::ostream& os) const override;

    void                  printSlices (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks

    S_msrPart             fStaffUpLinkToPart;

    // staff name
    std::string           fStaffName;

    // staff kind
    msrStaffKind          fStaffKind;

    // staff number
    int                   fStaffNumber;

    // staff instrument name
    std::string           fStaffInstrumentName;
    std::string           fStaffInstrumentAbbreviation;

    /*
      There are 'regular', harmonies and figured voices.
      A voice in and 'All' or 'Any' data structure can be any of these.
    */

    // the staff regular (i.e. not harmonies nor figured bass) voices
    static int            gStaffMaxRegularVoices;

    // we need to sort the voices by increasing voice numbers,
    // but with harmonies voices right before the corresponding regular voices
    std::list<S_msrVoice>      fStaffAllVoicesList;

    // harmonies and figured bass elements should be placed
    // in the first regular voice of the staff, hence:
    std::list<S_msrVoice>      fStaffRegularVoicesList;

    // the mapping of all the voices in the staff,
    // including harmonies and figured bass voices
    std::map<int, S_msrVoice>  fStaffVoiceNumbersToAllVoicesMap;

    // the mapping of voice numbers to regular voices
    std::map<int, S_msrVoice>  fStaffVoiceNumbersToRegularVoicesMap;

    // part shortest note

    // fStaffShortestNoteDuration and fStaffShortestNoteTupletFactor
    // are used in the time-oriented handling of staff measures slices
    Rational              fStaffShortestNoteDuration;
    msrTupletFactor       fStaffShortestNoteTupletFactor;

    // measures flat std::list
    // i.e. without segments nor repeats,
    // gathered from the staff's voices fVoiceInitialElementsList and fVoiceLastSegment
    // by finalizeMeasure()
    std::vector<std::list<S_msrMeasure> >
                          fStaffMeasuresFlatListsVector;

    // multiple full-bar rests

    Bool                  fStaffContainsMultipleFullBarRests;

    // measures slices sequence

    S_msrMeasuresSlicesSequence
                          fStaffMeasuresSlicesSequence;

  public:

    // public work services
    // ------------------------------------------------------

    // clef, key, time signature

    void                  setStaffCurrentClef (S_msrClef clef);

    S_msrClef             getStaffCurrentClef () const
                              { return fStaffCurrentClef; }

    void                  setStaffCurrentKey (S_msrKey key);

    S_msrKey              getStaffCurrentKey  () const
                              { return fStaffCurrentKey; }

    void                  setStaffCurrentTime (S_msrTimeSignature timeSignature);

    S_msrTimeSignature    getStaffCurrentTime () const
                              { return fStaffCurrentTimeSignature; }

    // finalization

    void                  finalizeLastAppendedMeasureInStaff (
                            int inputLineNumber);

    void                  finalizeStaff (
                            int inputLineNumber);

    void                  collectStaffMeasuresIntoFlatListsVector (
                            int inputLineNumber);

    void                  collectStaffMeasuresSlices (
                            int inputLineNumber);

  private:

    // private work services
    // ------------------------------------------------------

    // voice registration

    void                  registerVoiceInStaffAllVoicesList (
                            const S_msrVoice& voice);

    void                  registerVoiceByItsNumber (
                            int        inputLineNumber,
                            const S_msrVoice& voice);

    void                  registerRegularVoiceByItsNumber (
                            int        inputLineNumber,
                            const S_msrVoice& regularVoice,
                            int        voiceNumber);

    void                  registerHarmoniesVoiceByItsNumber (
                            int        inputLineNumber,
                            const S_msrVoice& voice);

    void                  registerFiguredBassVoiceByItsNumber (
                            int        inputLineNumber,
                            const S_msrVoice& voice);

    void                  registerPartLevelVoiceInStaff ( // JMI rename for harmonies and figured bass ???
                            int        inputLineNumber,
                            const S_msrVoice& voice);

    // transposition

    S_msrTransposition        getStaffCurrentTranspose () const
                              { return fStaffCurrentTranspose; }

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

    // staff details

    S_msrStaffDetails     fCurrentStaffStaffDetails;

    // transposition

    S_msrTransposition    fStaffCurrentTranspose;
};
typedef SMARTP<msrStaff> S_msrStaff;
EXP std::ostream& operator << (std::ostream& os, const S_msrStaff& elt);


}


#endif
