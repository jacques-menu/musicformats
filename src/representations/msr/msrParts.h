#ifndef ___msrParts___
#define ___msrParts___

#include "msrClefs.h"
#include "msrKeys.h"
#include "msrMeasuresSlices.h"
#include "msrPartGroups.h"
#include "msrPartGroupElements.h"
#include "msrRepeats.h"
#include "msrFullMeasureRests.h"
#include "msrStaves.h"
#include "msrStavesDetails.h"
#include "msrTimeSignatures.h"
#include "msrTranspositions.h"


namespace MusicFormats
{

//______________________________________________________________________________
class msrStaff;
typedef SMARTP<msrStaff> S_msrStaff;

class msrStaffDetails;
typedef SMARTP<msrStaffDetails> S_msrStaffDetails;

class msrHarmony;
typedef SMARTP<msrHarmony> S_msrHarmony;

class msrFiguredBassElement;
typedef SMARTP<msrFiguredBassElement> S_msrFiguredBassElement;

class msrGraceNotesGroup;
typedef SMARTP<msrGraceNotesGroup> S_msrGraceNotesGroup;

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
                            int            inputLineNumber,
                            const string&  partID,
                            S_msrPartGroup partPartGroupUpLink);

    SMARTP<msrPart> createPartNewbornClone (
                            S_msrPartGroup partGroupClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrPart (
                            int            inputLineNumber,
                            const string&  partID,
                            S_msrPartGroup partPartGroupUpLink);

    virtual               ~msrPart ();

  private:

    // private initialization
    // ------------------------------------------------------

    void                  initializePart ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks

    void                  setPartPartGroupUpLink (
                            S_msrPartGroup partGroup)
                              { fPartPartGroupUpLink = partGroup; }

    S_msrPartGroup        getPartPartGroupUpLink () const
                              { return fPartPartGroupUpLink; }

    // part absolute number

    int                   getPartAbsoluteNumber () const
                              { return fPartAbsoluteNumber; }

    // ID and name

    void                  setPartID (const string& partID)
                              { fPartID = partID; }

    string                getPartID () const
                              { return fPartID; }

    void                  setPartMsrName (const string& partMsrName);

    string                getPartMsrName () const
                              { return fPartMsrName; }

    void                  setPartName (const string& partName)
                              { fPartName = partName; }

    string                getPartName () const
                              { return fPartName; }

    void                  setPartNameDisplayText (
                            const string& partNameDisplayText)
                              {
                                fPartNameDisplayText =
                                  partNameDisplayText;
                              }

    string                getPartNameDisplayText () const
                              { return fPartNameDisplayText; }

    void                  setPartAbbreviation (
                            const string& partAbbreviation)
                              {
                                fPartAbbreviation =
                                  partAbbreviation;
                              }

    string                getPartAbbreviation () const
                              { return fPartAbbreviation; }

    void                  setPartAbbreviationDisplayText (
                            const string& partAbbreviationDisplayText)
                              {
                                fPartAbbreviationDisplayText =
                                  partAbbreviationDisplayText;
                              }

    string                getPartAbbreviationDisplayText () const
                              { return fPartAbbreviationDisplayText; }

    string                getPartCombinedName () const;

    void                  setPartInstrumentNamesMaxLengthes ();

    // measures

    void                  setPartNumberOfMeasures (
                            unsigned int partNumberOfMeasures);

    const unsigned int    getPartNumberOfMeasures () const
                              { return fPartNumberOfMeasures; }

    const vector<rational>&
                          getPartMeasuresWholeNotesDurationsVector () const
                              { return fPartMeasuresWholeNotesDurationsVector; }

    // instrument name

    void                  setPartInstrumentName (
                            const string& partInstrumentName)
                              { fPartInstrumentName = partInstrumentName; }

    string                getPartInstrumentName () const
                              { return fPartInstrumentName; }

    void                  setPartInstrumentAbbreviation (
                            const string& partInstrumentAbbreviation)
                              {
                                fPartInstrumentAbbreviation =
                                  partInstrumentAbbreviation;
                              }

    string                getPartInstrumentAbbreviation () const
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

    // staves map

    const map<int, S_msrStaff>&
                          getPartStaveNumbersToStavesMap () const
                              { return fPartStaveNumbersToStavesMap; }

    // staff position in measure

    void                  setPartCurrentPositionInMeasure (
                            int             inputLineNumber,
                            const rational& positionInMeasure);

    void                  incrementPartCurrentPositionInMeasure (
                            int             inputLineNumber,
                            const rational& duration);
    void                  decrementPartCurrentPositionInMeasure (
                            int             inputLineNumber,
                            const rational& duration);

    rational              getPartCurrentPositionInMeasure () const
                              { return fPartCurrentPositionInMeasure; }

    // part shortest note

    void                  setPartShortestNoteDuration (
                            const rational& duration);

    rational              getPartShortestNoteDuration () const
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
    S_msrScore            fetchPartScoreUpLink () const;

    void                  assignSequentialNumbersToRegularVoicesInPart (
                            int inputLineNumber);

/* JMI
    void                  addAVoiceToStavesThatHaveNone (
                            int inputLineNumber);
*/

    // whole notes durations

    rational              fetchPartMeasuresWholeNotesDurationsVectorAt (
                            int inputLineNumber,
                            int indexValue) const;

    // path shortest note

    void                  registerShortestNoteInPartIfRelevant (
                            S_msrNote note);

    // measures

    void                  createAMeasureAndAppendItToPart (
                            int           inputLineNumber,
                            const string& measureNumber,
                            msrMeasureImplicitKind
                                          measureImplicitKind);

    void                  setNextMeasureNumberInPart (
                            int           inputLineNumber,
                            const string& nextMeasureNumber);

    void                  registerOrdinalMeasureNumberWholeNotesDuration (
                            int             inputLineNumber,
                            int             measureOrdinalNumber,
                            const rational& wholeNotesDuration);

    // clef, key, time signature

    void                  appendClefToPart (S_msrClef clef);

    void                  appendKeyToPart (S_msrKey  key);

    void                  appendTimeSignatureToPart (
                            S_msrTimeSignature timeSignature);
    void                  appendTimeSignatureToPartClone (
                            S_msrTimeSignature timeSignature);

    // dal segno

    void                  insertHiddenMeasureAndBarLineInPartClone (
                            int             inputLineNumber,
                            const rational& positionInMeasure);

    // transpose

    void                  appendTransposeToPart (
                            S_msrTranspose transpose);

    // staff details

    void                  appendStaffDetailsToPart (
                            S_msrStaffDetails staffDetails);

    // tuplets JMI ???

    // barLines

    void                  appendBarLineToPart (S_msrBarLine barLine);

    // repeats

    void                  handleRepeatStartInPart (
                            int inputLineNumber);

    void                  handleRepeatEndInPart (
                            int           inputLineNumber,
                            const string& measureNumber,
                            int           repeatTimes);

    void                  handleRepeatEndingStartInPart (
                            int inputLineNumber);

    void                  handleRepeatEndingEndInPart (
                            int           inputLineNumber,
                            const string& repeatEndingNumber, // may be "1, 2"
                            msrRepeatEndingKind
                                          repeatEndingKind);

    /* JMI ???
    void                  finalizeRepeatEndInPart (
                            int    inputLineNumber,
                            const string& measureNumber,
                            int    repeatTimes);
                            */

    void                  appendRepeatCloneToPart (
                            int         inputLineNumber,
                            S_msrRepeat repeatCLone);

    void                  appendRepeatEndingCloneToPart (
                            S_msrRepeatEnding repeatEndingCLone);

    // full measure rests

    void                  createFullMeasureRestsInPart (
                            int inputLineNumber,
                            int fullMeasureRestsNumber);

    void                  appendPendingFullMeasureRestsToPart (
                            int inputLineNumber);

    void                  replicateLastAppendedMeasureInPart (
                            int           inputLineNumber);

    void                  addFullMeasureRestsToPart (
                            int           inputLineNumber,
                            const string& previousMeasureNumber,
                            int           fullMeasureRestsNumber);

    void                  appendFullMeasureRestsCloneToPart (
                            int               inputLineNumber,
                            S_msrFullMeasureRests fullMeasureRests);

    // measure repeats

    void                  createMeasureRepeatFromItsFirstMeasuresInPart (
                            int inputLineNumber,
                            int measureRepeatMeasuresNumber,
                            int measureRepeatSlashesNumber);

    void                  appendPendingMeasureRepeatToPart (
                            int inputLineNumber);

    void                  appendMeasureRepeatCloneToPart (
                            int               inputLineNumber,
                            S_msrFullMeasureRests fullMeasureRests);

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
                            const string& currentMeasureNumber);

    void                  appendHarmonyToPart (
                            S_msrVoice  harmonySupplierVoice,
                            S_msrHarmony harmony);

    void                  appendHarmonyToPartClone (
                            S_msrVoice   harmonySupplierVoice,
                            S_msrHarmony harmony);

    // figured bass

    S_msrVoice            createPartFiguredBassVoice (
                            int           inputLineNumber,
                            const string& currentMeasureNumber);

    void                  appendFiguredBassElementToPart (
                            S_msrVoice             figuredBassSupplierVoice,
                           S_msrFiguredBassElement figuredBassElement);

    void                  appendFiguredBassElementToPartClone (
                            S_msrVoice              figuredBassSupplierVoice,
                            S_msrFiguredBassElement figuredBassElement);

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

    void                  addSkipGraceNotesGroupBeforeAheadOfVoicesClonesIfNeeded (
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
                            const rational& backupStepLength);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    void                  printPartMeasuresWholeNotesDurationsVector (
                            ostream&     os,
                            int fieldWidth) const;

    void                  print (ostream& os) const override;

    void                  printShort (ostream& os) const override;

    void                  printSummary (ostream& os) const override;

    void                  printSlices (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks

    S_msrPartGroup        fPartPartGroupUpLink;

    // part ID and name

    string                fPartID; // native

    string                fPartMsrName;
                            // may be different than fPartID
                            // if renamed,
                            // coined in constructor

    string                fPartName; // from '<part-name/>'
    string                fPartNameDisplayText;

    string                fPartAbbreviation;
    string                fPartAbbreviationDisplayText;

    // part absolute number

    int                   fPartAbsoluteNumber;

    // part instrument names

    string                fPartInstrumentName;
    string                fPartInstrumentAbbreviation;

    // staves

    list<S_msrStaff>      fPartAllStavesList;

    list<S_msrStaff>      fPartRegularStavesList;

    list<S_msrStaff>      fPartNonHarmoniesNorFiguredBassStavesList;

    map<int, S_msrStaff>  fPartStaveNumbersToStavesMap;

    // harmonies

    S_msrStaff            fPartHarmoniesStaff;
    S_msrVoice            fPartHarmoniesVoice;

    // figured bass

    S_msrStaff            fPartFiguredBassStaff;
    S_msrVoice            fPartFiguredBassVoice;

    // voices

    list<S_msrVoice>      fPartAllVoicesList;

    // measures

    string                fPartCurrentMeasureNumber;

    int                   fPartNumberOfMeasures;

    vector<rational>      fPartMeasuresWholeNotesDurationsVector; // SUPERFLOUS ??? JMI NOEL

    // clef, key, time signature

    S_msrClef             fPartCurrentClef;

    S_msrKey              fPartCurrentKey;

    S_msrTimeSignature    fPartCurrentTimeSignature;

    // part shortest note

    // fPartShortestNoteDuration and fPartShortestNoteTupletFactor
    // are used to compute a number of divisions per quarter note
    // if needed, such as when generating MusicXML from MSR
    rational              fPartShortestNoteDuration;
    msrTupletFactor       fPartShortestNoteTupletFactor;

    // transpose

    S_msrTranspose        fPartCurrentTranspose;

    // counter

    static int            gPartsCounter;

    // full measure rests

    Bool                  fPartContainsFullMeasureRests;

    // measures slices sequence

    S_msrMeasuresSlicesSequence
                          fPartMeasuresSlicesSequence;

  public:

    // public work services
    // ------------------------------------------------------

    // measure number

    void                  setPartCurrentMeasureNumber (
                            const string& measureNumber)
                              {
                                fPartCurrentMeasureNumber =
                                  measureNumber;
                              }

    const string          getPartCurrentMeasureNumber () const
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

    // transpose

    S_msrTranspose        getPartCurrentTranspose () const
                              { return fPartCurrentTranspose; }

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

//     void                  registerStaffInPartByItsNumber ( JMI ???
//                             int        staffNumber,
//                             S_msrStaff staff);

    // harmonies

    void                  setPartHarmoniesVoiceForwardLink (
                            S_msrVoice voice)
                              { fPartHarmoniesVoiceForwardLink = voice; }

    S_msrVoice            getPartHarmoniesVoiceForwardLink () const
                              { return fPartHarmoniesVoiceForwardLink; }

    void                  setHarmoniesVoicePartBackwardLink (
                            S_msrVoice voice)
                              { fHarmoniesVoicePartBackwardLink = voice; }

    S_msrVoice            getHarmoniesVoicePartBackwardLink () const
                              { return fHarmoniesVoicePartBackwardLink; }

    // figured bass

    void                  setPartFiguredBassVoiceForwardLink (
                            S_msrVoice voice)
                              { fPartFiguredBassVoiceForwardLink = voice; }

    S_msrVoice            getPartFiguredBassVoiceForwardLink () const
                              { return fPartFiguredBassVoiceForwardLink; }

    void                  setFiguredBassVoicePartBackwardLink (
                            S_msrVoice voice)
                              { fFiguredBassVoicePartBackwardLink = voice; }

    S_msrVoice            getFiguredBassVoicePartBackwardLink () const
                              { return fFiguredBassVoicePartBackwardLink; }

    static bool           compareStavesToHaveFiguredBassElementsBelowCorrespondingPart (
                            const S_msrStaff& first,
                            const S_msrStaff& second);

  private:

    // private work fields
    // ------------------------------------------------------

    // staff details

    S_msrStaffDetails     fCurrentPartStaffDetails;

    // position in measure

    rational              fPartCurrentPositionInMeasure;

    // two-way links
    S_msrVoice            fPartHarmoniesVoiceForwardLink;
    S_msrVoice            fHarmoniesVoicePartBackwardLink;

    S_msrVoice            fPartFiguredBassVoiceForwardLink;
    S_msrVoice            fFiguredBassVoicePartBackwardLink;
};
typedef SMARTP<msrPart> S_msrPart;
EXP ostream& operator<< (ostream& os, const S_msrPart& elt);


}


#endif
