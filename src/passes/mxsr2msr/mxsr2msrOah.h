/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsr2msrOah___
#define ___mxsr2msrOah___

#include "mfPreprocessorSettings.h"

#include "msrClefsKeysTimeSignatures.h"
#include "msrSegnos.h"

#include "oahAtomsCollection.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrReplaceClefAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrReplaceClefAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::map <msrClefKind, msrClefKind>&
                                               clefKindToClefKindMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrReplaceClefAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::map <msrClefKind, msrClefKind>&
                                               clefKindToClefKindMapVariable);

    virtual               ~msrReplaceClefAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::map <msrClefKind, msrClefKind>&
                          getClefKindToClefKindMapVariable () const
                              { return fClefKindToClefKindMapVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::map <msrClefKind, msrClefKind>&
                          fClefKindToClefKindMapVariable;
};
typedef SMARTP<msrReplaceClefAtom> S_msrReplaceClefAtom;
EXP std::ostream& operator << (std::ostream& os, const S_msrReplaceClefAtom& elt);

//______________________________________________________________________________
class EXP mxsr2msrOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mxsr2msrOahGroup> create (
                            const S_oahHandler& handler);

  private:

    // initialisation
    // ------------------------------------------------------

    void                  createTheMxsr2msrPrefixes (
                            const S_oahHandler& handler);

    void                  initializeMxsr2msrOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsr2msrOahGroup (
                            const S_oahHandler& handler);

    virtual               ~mxsr2msrOahGroup ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceGroupQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkGroupOptionsConsistency () override;

  public:

    // set and get
    // ------------------------------------------------------

    // use MXSR events (temporary from 0.9.72)
    // --------------------------------------

    void                  setUseMxsrEvents ()
                              { fUseMxsrEvents = true; }
    Bool                  getUseMxsrEvents () const
                              { return fUseMxsrEvents; }

    // prefix options
    // --------------------------------------

    S_oahPrefix           getShortIgnoreRedundantPrefix () const
                              { return fShortIgnoreRedundantPrefix; }
    S_oahPrefix           getLongIgnoreRedundantPrefix () const
                              { return fLongIgnoreRedundantPrefix; }

    S_oahPrefix           getShortDelayRestsPrefix () const
                              { return fShortDelayRestsPrefix; }
    S_oahPrefix           getLongDelayRestsPrefix () const
                              { return fLongDelayRestsPrefix; }

    // header
    // --------------------------------------

    Bool                  getUseLyricistsAsPoets () const
                              { return fUseLyricistsAsPoets; }

    // parts
    // --------------------------------------

    const std::set <std::string>&
                          getPartsIgnoreIDSet () const
                              { return fPartsIgnoreIDSet; }
    const std::set <std::string>&
                          getMusicXMLPartsKeepIDSet () const
                              { return fMusicXMLPartsKeepIDSet; }

    const std::set <std::string>&
                          getMusicXMLPartsIgnoreNameSet () const
                              { return fMusicXMLPartsIgnoreNameSet; }
    const std::set <std::string>&
                          getMusicXMLPartsKeepNameSet () const
                              { return fMusicXMLPartsKeepNameSet; }

    // for checkOptionsConsistency()
    S_oahStringSetElementAtom
                          getIgnoreMusicXMLPartIDAtom () const
                              { return fIgnoreMusicXMLPartIDAtom; }
    S_oahStringSetElementAtom
                          getKeepMusicXMLPartIDAtom () const
                              { return fKeepMusicXMLPartIDAtom; }

    S_oahStringSetElementAtom
                          getIgnoreMusicXMLPartNameAtom () const
                              { return fIgnoreMusicXMLPartNameAtom; }
    S_oahStringSetElementAtom
                          getKeepMusicXMLPartNameAtom () const
                              { return fKeepMusicXMLPartNameAtom; }

    // clefs, keys, time signatures
    // --------------------------------------

    const std::map <msrClefKind, msrClefKind>&
                          getReplaceClefKindToClefKindMapVariable () const
                              { return fReplaceClefKindToClefKindMapVariable; }

    Bool                  getIgnoreRedundantClefs () const
                              { return fIgnoreRedundantClefs; }
    Bool                  getIgnoreRedundantKeys () const
                              { return fIgnoreRedundantKeys; }
    Bool                  getIgnoreRedundantTimes () const
                              { return fIgnoreRedundantTimes; }

    const S_oahBooleanAtom&
                          getIgnoreRedundantClefsAtom () const
                              { return fIgnoreRedundantClefsAtom; }
    const S_oahBooleanAtom&
                          getIgnoreRedundantKeysAtom () const
                              { return fIgnoreRedundantKeysAtom; }
    const S_oahBooleanAtom&
                          getIgnoreRedundantTimesAtom () const
                              { return fIgnoreRedundantTimesAtom; }

    // page and line breaks
    // --------------------------------------

    Bool                  getIgnoreMusicXMLPageBreaks () const
                              { return fIgnoreMusicXMLPageBreaks; }

    // line breaks
    // --------------------------------------

    Bool                  getIgnoreMusicXMLLineBreaks () const
                              { return fIgnoreMusicXMLLineBreaks; }

    // measures
    // --------------------------------------

    const std::map <std::string,int>&
                          getMeasuresToBeReplicatedStringToIntMap () const
                              { return fMeasuresToBeReplicatedStringToIntMap; }

    const std::map <std::string,int>&
                          getAddEmptyMeasuresStringToIntMap () const
                              { return fAddEmptyMeasuresStringToIntMap; }

    // notes
    // --------------------------------------

    Bool                  getDelayRestsDynamics () const
                              { return fDelayRestsDynamics; }
    Bool                  getDelayRestsWords () const
                              { return fDelayRestsWords; }
    Bool                  getDelayRestsBeams () const
                              { return fDelayRestsBeams; }
    Bool                  getDelayRestsSlurs () const
                              { return fDelayRestsSlurs; }
    Bool                  getDelayRestsLigatures () const
                              { return fDelayRestsLigatures; }
    Bool                  getDelayRestsPedals () const
                              { return fDelayRestsPedals; }
    Bool                  getDelayRestsSlashes () const
                              { return fDelayRestsSlashes; }
    Bool                  getDelayRestsWedges () const
                              { return fDelayRestsWedges; }

    Bool                  getSlashAllGraceNotes () const
                              { return fSlashAllGraceNotes; }
    Bool                  getSlurAllGraceNotes () const
                              { return fSlurAllGraceNotes; }
    Bool                  getBeamAllGraceNotes () const
                              { return fBeamAllGraceNotes; }

    // articulations
    // --------------------------------------

    Bool                  getIgnoreArticulations () const
                              { return fIgnoreArticulations; }

    // ornaments
    // --------------------------------------

    Bool                  getIgnoreOrnaments () const
                              { return fIgnoreOrnaments; }

    // words conversions
    // --------------------------------------

    Bool                  getIgnoreMusicXMLWords () const
                              { return fIgnoreMusicXMLWords; }

    const std::set <std::string>&
                          getBoldWordsSet () const
                              { return fBoldWordsSet; }
    S_oahStringSetElementAtom
                          getBoldWordsAtom () const
                              { return fBoldWordsAtom; }

    const std::set <std::string>&
                          getItalicWordsSet () const
                              { return fItalicWordsSet; }
    S_oahStringSetElementAtom
                          getItalicWordsAtom () const
                              { return fItalicWordsAtom; }

    const std::set <std::string>&
                          getWordsToBePlacedAboveSet () const
                              { return fWordsToBePlacedAboveSet; }
    S_oahStringSetElementAtom
                          getWordsToBePlacedAboveAtom () const
                              { return fWordsToBePlacedAboveAtom; }

    const std::set <std::string>&
                          getWordsToBePlacedBelowSet () const
                              { return fWordsToBePlacedBelowSet; }
    S_oahStringSetElementAtom
                          getWordsToBePlacedBelowAtom () const
                              { return fWordsToBePlacedBelowAtom; }

    Bool                  getAddMsrWordsFromTheMusicXMLLyrics () const
                              { return fAddMsrWordsFromTheMusicXMLLyrics; }

    const std::set <std::string>&
                          getWordsToRehearsalMarkSet () const
                              { return fWordsToRehearsalMarkSet; }
    S_oahStringSetElementAtom
                          getWordsToRehearsalMarkAtom () const
                              { return fWordsToRehearsalMarkAtom; }

    const std::set <std::string>&
                          getWordsToSegnoSet () const
                              { return fWordsToSegnoSet; }
    S_oahStringSetElementAtom
                          getWordsToSegnoAtom () const
                              { return fWordsToSegnoAtom; }

    const std::set <std::string>&    getWordsToDalSegnoSet () const
                              { return fWordsToDalSegnoSet; }
    S_oahStringSetElementAtom
                          getWordsToDalSegnoAtom () const
                              { return fWordsToDalSegnoAtom; }

    const std::set <std::string>&
                          getWordsToDalSegnoAlFineSet () const
                              { return fWordsToDalSegnoAlFineSet; }
    S_oahStringSetElementAtom
                          getWordsToDalSegnoAlFineAtom () const
                              { return fWordsToDalSegnoAlFineAtom; }

    const std::set <std::string>&
                          getWordsToDalSegnoAlCodaSet () const
                              { return fWordsToDalSegnoAlCodaSet; }
    S_oahStringSetElementAtom
                          getWordsToSegnoAlCodaAtom () const
                              { return fWordsToDalSegnoAlCodaAtom; }

    const std::set <std::string>&
                          getWordsToCodaFirstSet () const
                              { return fWordsToCodaFirstSet; }
    S_oahStringSetElementAtom
                          getWordsToCodFirstaAtom () const
                              { return fWordsToCodaFirstAtom; }

    const std::set <std::string>&
                          getWordsToCodaSecondSet () const
                              { return fWordsToCodaSecondSet; }
    S_oahStringSetElementAtom
                          getWordsToCodaSecondAtom () const
                              { return fWordsToCodaSecondAtom; }

    const std::set <std::string>&
                          getWordsToCrescSet () const
                              { return fWordsToCrescSet; }
    S_oahStringSetElementAtom
                          getWordsToCrescAtom () const
                              { return fWordsToCrescAtom; }

    const std::set <std::string>&
                          getWordsToDecrescSet () const
                              { return fWordsToDecrescSet; }
    S_oahStringSetElementAtom
                          getWordsToDecrescAtom () const
                              { return fWordsToDecrescAtom; }

    // ties
    // --------------------------------------

    Bool                  getIgnoreTies () const
                              { return fIgnoreTies; }

    // dynamics
    // --------------------------------------

    Bool                  getIgnoreDynamics () const
                              { return fIgnoreDynamics; }

    // slurs
    // --------------------------------------

    Bool                  getIgnoreSlurs () const
                              { return fIgnoreSlurs; }

    // wedges
    // --------------------------------------

    Bool                  getIgnoreWedges () const
                              { return fIgnoreWedges; }

    // lyrics
    // --------------------------------------

    Bool                  getIgnoreLyrics () const
                              { return fIgnoreLyrics; }

    // harmonies
    // --------------------------------------

    Bool                  getIgnoreHarmonies () const
                              { return fIgnoreHarmonies; }

    // figured bass
    // --------------------------------------

    Bool                  getIgnoreFiguredBasses () const
                              { return fIgnoreFiguredBasses; }

    // dynamics and wedges
    // --------------------------------------

    Bool                  getAllDynamicsBelow () const
                              { return fAllDynamicsBelow; }
    Bool                  getAllWedgesBelow () const
                              { return fAllWedgesBelow; }

    // combined options, cubase
    // --------------------------------------

    void                  setCubase ()
                              { fCubase = true; }
    Bool                  getCubase () const
                              { return fCubase; }

#ifdef MF_TRACE_IS_ENABLED
    // specific trace
    // --------------------------------------

    // encoding
    void                  setTraceEncoding ()
                              { fTraceEncoding = true; }
    Bool                  getTraceEncoding () const
                              { return fTraceEncoding; }

    // divisions
    void                  setTraceDivisions ()
                              { fTraceDivisions = true; }
    Bool                  getTraceDivisions () const
                              { return fTraceDivisions; }

    // backup & forward
    void                  setTraceBackup ()
                              { fTraceBackup = true; }
    Bool                  getTraceBackup () const
                              { return fTraceBackup; }

    void                  setTraceForward ()
                              { fTraceForward = true; }
    Bool                  getTraceForward () const
                              { return fTraceForward; }

    // display
    // --------------------------------------

    void                  setDisplayMxsr ()
                              { fDisplayMxsr = true; }
    Bool                  getDisplayMxsr () const
                              { return fDisplayMxsr; }

    S_oahBooleanAtom      getDisplayMxsrAtom () const
                            { return fDisplayMxsrAtom; }

    // specific trace JMI move to traceOah
    // --------------------------------------

    void                  setTraceMxsr ()
                              { fTraceMxsr = true; }
    Bool                  getTraceMxsr () const
                              { return fTraceMxsr; }

    // visitors
    void                  setTraceMxsrVisitors ()
                              { fTraceMxsrVisitors = true; }
    Bool                  getTraceMxsrVisitors () const
                              { return fTraceMxsrVisitors; }
#endif // MF_TRACE_IS_ENABLED

  public:

    // public services
    // ------------------------------------------------------

    Bool                  wordsIsToBeConvertedToBold (
                            const std::string& wordsValue);

    Bool                  wordsIsToBeConvertedToItalic (
                            const std::string& wordsValue);

    Bool                  wordsIsToBePlacedAbove (
                            const std::string& wordsValue);

    Bool                  wordsIsToBePlacedBelow (
                            const std::string& wordsValue);

    Bool                  wordsIsToBeConvertedToTempo (
                            const std::string& wordsValue);

    Bool                  wordsIsToBeConvertedToRehearsalMark (
                            const std::string& wordsValue);

    Bool                  wordsIsToBeConvertedToSegno (
                            const std::string& wordsValue);

    Bool                  wordsIsToBeConvertedToDalSegno (
                            const std::string& wordsValue);

    Bool                  wordsIsToBeConvertedToDalSegnoAlFine (
                            const std::string& wordsValue);

    Bool                  wordsIsToBeConvertedToDalSegnoAlCoda (
                            const std::string& wordsValue);

    Bool                  wordsIsToBeConvertedToCodaFirst (
                            const std::string& wordsValue);
    Bool                  wordsIsToBeConvertedToCodaSecond (
                            const std::string& wordsValue);

    Bool                  wordsIsToBeConvertedToCresc (
                            const std::string& wordsValue);

    Bool                  wordsIsToBeConvertedToDecresc (
                            const std::string& wordsValue);

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeMxsr2msrDisplayOptions ();

#ifdef MF_TRACE_IS_ENABLED
    void                  initializeMxsr2msrTraceOptions ();
#endif // MF_TRACE_IS_ENABLED

    void                  initializeMxsr2msrEventsOptions (); // (temporary from 0.9.72)

    void                  initializeMxsr2msrPartsOptions ();

    void                  initializeMxsr2msrStavesOptions ();

    void                  initializeMxsr2msrVoiceOptions ();

    void                  initializeMxsr2msrClefsKeysTimesOptions ();

    void                  initializeMxsr2msrPageAndLineBreaksOptions ();

    void                  initializeMxsr2msrMeasuresOptions ();

    void                  initializeMxsr2msrNotesOptions ();

    void                  initializeMxsr2msrDynamicsAndWedgesOptions ();

    void                  initializeMxsr2msrWordsOptions ();

    void                  initializeMxsr2msrCombinedOptionsOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMxsrOahHelp ();

    void                  displayMxsrOahValues (int fieldWidth);

    void                  printMxsr2msrHelp ();

    void                  printMxsr2msrValues (int valueFieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // use MXSR events
    // --------------------------------------

    Bool                  fUseMxsrEvents;

    // prefix options
    // --------------------------------------

    S_oahPrefix           fShortIgnoreRedundantPrefix;
    S_oahPrefix           fLongIgnoreRedundantPrefix;

    S_oahPrefix           fShortDelayRestsPrefix;
    S_oahPrefix           fLongDelayRestsPrefix;

    // header
    // --------------------------------------

    Bool                  fUseLyricistsAsPoets; // lyricist is unknown to LilyPond

    // parts
    // --------------------------------------

    std::set <std::string>
                          fPartsIgnoreIDSet;
    std::set <std::string>
                          fMusicXMLPartsKeepIDSet;

    std::set <std::string>
                          fMusicXMLPartsIgnoreNameSet;
    std::set <std::string>
                          fMusicXMLPartsKeepNameSet;

     // for checkOptionsConsistency()
    S_oahStringSetElementAtom
                          fIgnoreMusicXMLPartIDAtom;
    S_oahStringSetElementAtom
                          fKeepMusicXMLPartIDAtom;

    S_oahStringSetElementAtom
                          fIgnoreMusicXMLPartNameAtom;
    S_oahStringSetElementAtom
                          fKeepMusicXMLPartNameAtom;

    // staves
    // --------------------------------------

    std::set <int>        fMusicXMLStavesIgnoreNumberSet;
    std::set <int>        fMusicXMLStavesKeepNumberSet;

     // for checkOptionsConsistency()
    S_oahIntSetElementAtom
                          fIgnoreStaffNumberAtom;
    S_oahIntSetElementAtom
                          fKeepStaffNumberAtom;

    // voices
    // --------------------------------------

    std::set <int>        fMusicXMLVoicesIgnoreNumberSet;
    std::set <int>        fMusicXMLVoicesKeepNumberSet;

     // for checkOptionsConsistency()
    S_oahIntSetElementAtom
                          fIgnoreVoiceNumberAtom;
    S_oahIntSetElementAtom
                          fKeepVoiceNumberAtom;

    // clefs, keys, time signatures
    // --------------------------------------

    std::map <msrClefKind, msrClefKind>
                          fReplaceClefKindToClefKindMapVariable;

    Bool                  fIgnoreRedundantClefs;
    Bool                  fIgnoreRedundantKeys;
    Bool                  fIgnoreRedundantTimes;

    S_oahCommonPrefixBooleansAtom
                          fIgnoreRedundantMultiplexBooleansAtom;

    // these atoms are used by the '-ignore-redundant*' multiplex options
    S_oahBooleanAtom      fIgnoreRedundantClefsAtom;
    S_oahBooleanAtom      fIgnoreRedundantKeysAtom;
    S_oahBooleanAtom      fIgnoreRedundantTimesAtom;

    // page and line breaks
    // --------------------------------------

    Bool                  fIgnoreMusicXMLPageBreaks;

    // line breaks
    // --------------------------------------

    Bool                  fIgnoreMusicXMLLineBreaks;

    // measures
    // --------------------------------------

    std::map <std::string,int>
                          fMeasuresToBeReplicatedStringToIntMap;

    std::map <std::string,int>
                          fAddEmptyMeasuresStringToIntMap;

    // notes
    // --------------------------------------

    Bool                  fDelayRestsDynamics;
    Bool                  fDelayRestsWords;
    Bool                  fDelayRestsBeams;
    Bool                  fDelayRestsSlurs;
    Bool                  fDelayRestsLigatures;
    Bool                  fDelayRestsPedals;
    Bool                  fDelayRestsSlashes;
    Bool                  fDelayRestsWedges;

    Bool                  fSlashAllGraceNotes;
    Bool                  fSlurAllGraceNotes;
    Bool                  fBeamAllGraceNotes;

    // articulations
    // --------------------------------------

    Bool                  fIgnoreArticulations;

    // ornaments
    // --------------------------------------

    Bool                  fIgnoreOrnaments;

    // words
    // --------------------------------------

    Bool                  fIgnoreMusicXMLWords;

    std::set <std::string>
                          fBoldWordsSet;
    S_oahStringSetElementAtom
                          fBoldWordsAtom;

    std::set <std::string>
                          fItalicWordsSet;
    S_oahStringSetElementAtom
                          fItalicWordsAtom;

    std::set <std::string>
                          fWordsToBePlacedAboveSet;
    S_oahStringSetElementAtom
                          fWordsToBePlacedAboveAtom;

    std::set <std::string> fWordsToBePlacedBelowSet;
    S_oahStringSetElementAtom
                          fWordsToBePlacedBelowAtom;

    Bool                  fAddMsrWordsFromTheMusicXMLLyrics;

    // words conversions
    std::set <std::string>
                          fWordsToBeConvertedSet;

    std::set <std::string>
                          fWordsToTemposSet;
    S_oahStringSetElementAtom
                          fWordsToTempoAtom;

    std::set <std::string>
                          fWordsToRehearsalMarkSet;
    S_oahStringSetElementAtom
                          fWordsToRehearsalMarkAtom;

    std::set <std::string> fWordsToSegnoSet;
    S_oahStringSetElementAtom
                          fWordsToSegnoAtom;

    std::set <std::string> fWordsToDalSegnoSet;
    S_oahStringSetElementAtom
                          fWordsToDalSegnoAtom;

    std::set <std::string> fWordsToDalSegnoAlFineSet;
    S_oahStringSetElementAtom
                          fWordsToDalSegnoAlFineAtom;

    std::set <std::string> fWordsToDalSegnoAlCodaSet;
    S_oahStringSetElementAtom
                          fWordsToDalSegnoAlCodaAtom;

    std::set <std::string> fWordsToCodaFirstSet;
    S_oahStringSetElementAtom
                          fWordsToCodaFirstAtom;

    std::set <std::string> fWordsToCodaSecondSet;
    S_oahStringSetElementAtom
                          fWordsToCodaSecondAtom;

    std::set <std::string> fWordsToCrescSet;
    S_oahStringSetElementAtom
                          fWordsToCrescAtom;

    std::set <std::string> fWordsToDecrescSet;
    S_oahStringSetElementAtom
                          fWordsToDecrescAtom;

    // ties
    // --------------------------------------

    Bool                  fIgnoreTies;

    // dynamics
    // --------------------------------------

    Bool                  fIgnoreDynamics;

    // slurs
    // --------------------------------------

    Bool                  fIgnoreSlurs;

    // wedges
    // --------------------------------------

    Bool                  fIgnoreWedges;

    // lyrics
    // --------------------------------------

    Bool                  fIgnoreLyrics;

    // harmonies
    // --------------------------------------

    Bool                  fIgnoreHarmonies;

    // figured bass
    // --------------------------------------

    Bool                  fIgnoreFiguredBasses;

    // dynamics and wedges
    // --------------------------------------

    Bool                  fAllDynamicsBelow;
    Bool                  fAllWedgesBelow;

    // combined options, cubase, needed in case it occurs in MusicXML <software/>
    // --------------------------------------

    Bool                  fCubase;

    // display MXSR
    // --------------------------------------

    Bool                  fDisplayMxsr;
    S_oahBooleanAtom      fDisplayMxsrAtom;

#ifdef MF_TRACE_IS_ENABLED
    // specific trace
    // --------------------------------------
    // encoding
    Bool                  fTraceEncoding;

    // divisions
    Bool                  fTraceDivisions;

    // backup & forward
    Bool                  fTraceBackup;
    Bool                  fTraceForward;

    // specific trace JMI move to traceOah JMI ??? 0.9.70
    // --------------------------------------

    Bool                  fTraceMxsr;

    // visitors
    Bool                  fTraceMxsrVisitors;
#endif // MF_TRACE_IS_ENABLED
};
typedef SMARTP<mxsr2msrOahGroup> S_mxsr2msrOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_mxsr2msrOahGroup& elt);

EXP extern S_mxsr2msrOahGroup gGlobalMxsr2msrOahGroup;

//______________________________________________________________________________
EXP S_mxsr2msrOahGroup createGlobalMxsr2msrOahGroup (
  const S_oahHandler& handler);


}


#endif // ___mxsr2msrOah___
