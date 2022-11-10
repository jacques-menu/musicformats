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

#include "msrStavesEnumTypes.h"

// #include "msrBarChecks.h"
// #include "msrBarLines.h"
// #include "msrBarNumberChecks.h"
// #include "msrBreaks.h"
// #include "msrClefs.h"
// #include "msrDoubleTremolos.h"
// #include "msrInstruments.h"
// #include "msrKeys.h"
// #include "msrMeasuresEnumTypes.h"
// #include "msrMeasures.h"
// #include "msrMeasuresSlices.h"
// #include "msrParts.h"
// #include "msrPartGroups.h"
// #include "msrPrintObjects.h"
// #include "msrRepeats.h"
// #include "msrMultipleFullBarRests.h"
// #include "msrRehearsalMarks.h"
// #include "msrRepeats.h"
// #include "msrScores.h"
// #include "msrStavesDetails.h"
// #include "msrTimeSignatures.h"
// #include "msrTempos.h"
// #include "msrTranspositions.h"
// #include "msrTupletFactors.h"

namespace MusicFormats
{

//______________________________________________________________________________
// PRE-declatations
// class   msrPartGroup;
// typedef SMARTP<msrPartGroup> S_msrPartGroup;
//
// class   msrPart;
// typedef SMARTP<msrPart> S_msrPart;
//
// class   msrStaff;
// typedef SMARTP<msrStaff> S_msrStaff;
//
// class   msrRepeat;
// typedef SMARTP<msrRepeat> S_msrRepeat;
//
class   msrRepeatEnding;
typedef SMARTP<msrRepeatEnding> S_msrRepeatEnding;

// class   msrTempo;
// typedef SMARTP<msrTempo> S_msrTempo;

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
                            S_msrPart    staffUpLinkToPart);

    SMARTP<msrStaff> createStaffNewbornClone (
                            S_msrPart containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrStaff (
                            int          inputLineNumber,
                            msrStaffKind staffKind,
                            int          staffNumber,
                            S_msrPart    staffUpLinkToPart);

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

    string                getStaffName () const
                              { return fStaffName; }

    string                getStaffInstrumentName () const
                              { return fStaffInstrumentName; }

    string                getStaffInstrumentAbbreviation () const
                              { return fStaffInstrumentAbbreviation; }

    // voices

    int                   getStaffRegularVoicesCounter () const
                              { return fStaffRegularVoicesCounter; }

    // staff details

    S_msrStaffDetails     getCurrentStaffStaffDetails () const
                            { return fCurrentStaffStaffDetails; }

    // staff voices

    const map<int, S_msrVoice>&
                          getStaffVoiceNumbersToAllVoicesMap () const
                              { return fStaffVoiceNumbersToAllVoicesMap; }

    const map<int, S_msrVoice>&
                          getStaffVoiceNumbersToRegularVoicesMap () const
                              { return fStaffVoiceNumbersToRegularVoicesMap; }

    const list<S_msrVoice>&
                          getStaffRegularVoicesList () const
                              { return fStaffRegularVoicesList; }

    const list<S_msrVoice>&
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


    const vector<list<S_msrMeasure> >&
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

    void                  registerShortestNoteInStaffIfRelevant (S_msrNote note);

    // staff details

    void                  appendStaffDetailsToStaff (
                            S_msrStaffDetails staffDetails);

    // clef, key, time signature

    void                  appendClefToStaff (S_msrClef clef);

    void                  appendKeyToStaff (S_msrKey  key);

    void                  appendTimeSignatureToStaff (S_msrTimeSignature timeSignature);
    void                  appendTimeSignatureToStaffClone (S_msrTimeSignature timeSignature);

    // tempo

    void                  appendTempoToStaff (S_msrTempo tempo);

    // rehearsal marks

    void                  appendRehearsalMarkToStaff (
                            S_msrRehearsalMark rehearsalMark);

    // breaks

    void                  appendLineBreakToStaff (S_msrLineBreak lineBreak);
    void                  appendPageBreakToStaff (S_msrPageBreak pageBreak);

    // dal segno

    void                  insertHiddenMeasureAndBarLineInStaffClone (
                            int             inputLineNumber,
                            const Rational& measurePosition);

    // transposition

    void                  appendTranspositionToStaff (
                            S_msrTransposition transpose);

    // voices

    S_msrVoice            createRegularVoiceInStaffByItsNumber (
                            int           inputLineNumber,
                            int           voiceNumber,
                            const string& currentMeasureNumber);

    void                  registerVoiceInStaff (
                            int        inputLineNumber,
                            S_msrVoice voice);

    void                  registerVoiceInStaffClone (
                            int        inputLineNumber,
                            S_msrVoice voice);

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
                            const string& measureNumber,
                            msrMeasureImplicitKind
                                          measureImplicitKind);

    void                  setNextMeasureNumberInStaff (
                            int           inputLineNumber,
                            const string& nextMeasureNumber);

    // repeats

    void                  handleRepeatStartInStaff (
                            int inputLineNumber);

    void                  handleRepeatEndInStaff (
                            int           inputLineNumber,
                            const string& measureNumber,
                            int           repeatTimes);

    void                  handleRepeatEndingStartInStaff (
                            int inputLineNumber);

    void                  handleRepeatEndingEndInStaff (
                            int           inputLineNumber,
                            const string& repeatEndingNumber, // may be "1, 2"
                            msrRepeatEndingKind
                                          repeatEndingKind);

/* JMI
    void                  finalizeRepeatEndInStaff (
                            int           inputLineNumber,
                            const string& measureNumber,
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
                            const string& previousMeasureNumber,
                            int           emptyMeasuresNumber);

    void                  appendMultipleFullBarRestsCloneToStaff (
                            int                       inputLineNumber,
                            S_msrMultipleFullBarRests multipleFullBarRests);

    void                  createBeatRepeatFromItsFirstMeasuresInStaff (
                            int inputLineNumber,
                            int beatRepeatMeasuresNumber,
                            int beatRepeatSlashesNumber);

    void                  appendRepeatCloneToStaff (
                            int         inputLineNumber,
                            S_msrRepeat repeatCLone);

    void                  appendRepeatEndingCloneToStaff (
                            S_msrRepeatEnding repeatEndingClone);

    // bar lines

    void                  appendBarLineToStaff (S_msrBarLine barLine);

    // transposition

    void                  appendTranspositionToAllStaffVoices ( // JMI
                            S_msrTransposition transpose);

    // scordaturas

    void                  appendScordaturaToStaff (
                            S_msrScordatura scordatura);

    // accordion registration

    void                  appendAccordionRegistrationToStaff (
                            S_msrAccordionRegistration
                              accordionRegistration);

    // harp pedals tuning

    void                  appendHarpPedalsTuningToStaff (
                            S_msrHarpPedalsTuning
                              harpPedalsTuning);

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

    string                staffNumberAsString () const;

    string                asString () const override;

    void                  print (ostream& os) const override;

    void                  printShort (ostream& os) const override;

    void                  printSummary (ostream& os) const override;

    void                  printSlices (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks

    S_msrPart             fStaffUpLinkToPart;

    // staff name
    string                fStaffName;

    // staff kind
    msrStaffKind          fStaffKind;

    // staff number
    int                   fStaffNumber;

    // staff instrument name
    string                fStaffInstrumentName;
    string                fStaffInstrumentAbbreviation;

    /*
      There are 'regular', harmonies and figured voices.
      A voice in and 'All' or 'Any' data structure can be any of these.
    */

    // the staff regular (i.e. not harmonies nor figured bass) voices
    static int            gStaffMaxRegularVoices;

    // we need to sort the voices by increasing voice numbers,
    // but with harmonies voices right before the corresponding regular voices
    list<S_msrVoice>      fStaffAllVoicesList;

    // harmonies and figured bass elements should be placed
    // in the first regular voice of the staff, hence:
    list<S_msrVoice>      fStaffRegularVoicesList;

    // the mapping of all the voices in the staff,
    // including harmonies and figured bass voices
    map<int, S_msrVoice>  fStaffVoiceNumbersToAllVoicesMap;

    // the mapping of voice numbers to regular voices
    map<int, S_msrVoice>  fStaffVoiceNumbersToRegularVoicesMap;

    // part shortest note

    // fStaffShortestNoteDuration and fStaffShortestNoteTupletFactor
    // are used in the time-oriented handling of staff measures slices
    Rational              fStaffShortestNoteDuration;
    msrTupletFactor       fStaffShortestNoteTupletFactor;

    // measures flat list
    // i.e. without segments nor repeats,
    // gathered from the staff's voices fVoiceInitialElementsList and fVoiceLastSegment
    // by finalizeMeasure()
    vector<list<S_msrMeasure> >
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
                            S_msrVoice voice);

    void                  registerVoiceByItsNumber (
                            int        inputLineNumber,
                            S_msrVoice voice);

    void                  registerRegularVoiceByItsNumber (
                            int        inputLineNumber,
                            S_msrVoice regularVoice,
                            int        voiceNumber);

    void                  registerHarmoniesVoiceByItsNumber (
                            int        inputLineNumber,
                            S_msrVoice voice);

    void                  registerFiguredBassVoiceByItsNumber (
                            int        inputLineNumber,
                            S_msrVoice voice);

    void                  registerPartLevelVoiceInStaff ( // JMI rename for harmonies and figured bass ???
                            int        inputLineNumber,
                            S_msrVoice voice);

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

    S_msrTransposition        fStaffCurrentTranspose;
};
typedef SMARTP<msrStaff> S_msrStaff;
EXP ostream& operator << (ostream& os, const S_msrStaff& elt);


}


#endif
