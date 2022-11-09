/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsr2msrOah___
#define ___mxsr2msrOah___

#include "msrSegnos.h"
#include "msrDalSegnos.h"

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
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            map<msrClefKind, msrClefKind>&
                                              clefKindToClefKindMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrReplaceClefAtom (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            map<msrClefKind, msrClefKind>&
                                              clefKindToClefKindMapVariable);

    virtual               ~msrReplaceClefAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<msrClefKind, msrClefKind>&
                          getClefKindToClefKindMapVariable () const
                              { return fClefKindToClefKindMapVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    map<msrClefKind, msrClefKind>&
                          fClefKindToClefKindMapVariable;
};
typedef SMARTP<msrReplaceClefAtom> S_msrReplaceClefAtom;
EXP ostream& operator << (ostream& os, const S_msrReplaceClefAtom& elt);

//______________________________________________________________________________
class EXP mxsr2msrOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mxsr2msrOahGroup> create (
                            S_oahHandler handler);

  private:

    // initialisation
    // ------------------------------------------------------

    void                  createTheMxsr2msrPrefixes (
                            S_oahHandler handler);

    void                  initializeMxsr2msrOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsr2msrOahGroup (
                            S_oahHandler handler);

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

    // options and help trace
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

    const set<string>&    getPartsIgnoreIDSet () const
                              { return fPartsIgnoreIDSet; }
    const set<string>&    getMusicXMLPartsKeepIDSet () const
                              { return fMusicXMLPartsKeepIDSet; }

    const set<string>&    getMusicXMLMusicXMLPartsIgnoreNameSet () const
                              { return fMusicXMLMusicXMLPartsIgnoreNameSet; }
    const set<string>&    getMusicXMLPartsKeepNameSet () const
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

    const map<msrClefKind, msrClefKind>&
                          getReplaceClefKindToClefKindMapVariable () const
                              { return fReplaceClefKindToClefKindMapVariable; }

    Bool                  getIgnoreRedundantClefs () const
                              { return fIgnoreRedundantClefs; }
    Bool                  getIgnoreRedundantKeys () const
                              { return fIgnoreRedundantKeys; }
    Bool                  getIgnoreRedundantTimes () const
                              { return fIgnoreRedundantTimes; }

    S_oahBooleanAtom      getIgnoreRedundantClefsAtom () const
                              { return fIgnoreRedundantClefsAtom; }
    S_oahBooleanAtom      getIgnoreRedundantKeysAtom () const
                              { return fIgnoreRedundantKeysAtom; }
    S_oahBooleanAtom      getIgnoreRedundantTimesAtom () const
                              { return fIgnoreRedundantTimesAtom; }

    // page and line breaks
    // --------------------------------------

    Bool                  getIgnorePageBreaksInMusicXML () const
                              { return fIgnorePageBreaksInMusicXML; }

    // line breaks
    // --------------------------------------

    Bool                  getIgnoreLineBreaksInMusicXML () const
                              { return fIgnoreLineBreaksInMusicXML; }

    // measures
    // --------------------------------------

    const map<string,int>&
                          getMeasuresToBeReplicatedStringToIntMap () const
                              { return fMeasuresToBeReplicatedStringToIntMap; }

    const map<string,int>&
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

    const set<string>&    getBoldWordsSet () const
                              { return fBoldWordsSet; }
    S_oahStringSetElementAtom
                          getBoldWordsAtom () const
                              { return fBoldWordsAtom; }

    const set<string>&    getItalicWordsSet () const
                              { return fItalicWordsSet; }
    S_oahStringSetElementAtom
                          getItalicWordsAtom () const
                              { return fItalicWordsAtom; }

    const set<string>&    getWordsToBePlacedAboveSet () const
                              { return fWordsToBePlacedAboveSet; }
    S_oahStringSetElementAtom
                          getWordsToBePlacedAboveAtom () const
                              { return fWordsToBePlacedAboveAtom; }

    const set<string>&    getWordsToBePlacedBelowSet () const
                              { return fWordsToBePlacedBelowSet; }
    S_oahStringSetElementAtom
                          getWordsToBePlacedBelowAtom () const
                              { return fWordsToBePlacedBelowAtom; }

    Bool                  getAddMsrWordsFromTheMusicXMLLyrics () const
                              { return fAddMsrWordsFromTheMusicXMLLyrics; }

    const set<string>&    getWordsToRehearsalMarkSet () const
                              { return fWordsToRehearsalMarkSet; }
    S_oahStringSetElementAtom
                          getWordsToRehearsalMarkAtom () const
                              { return fWordsToRehearsalMarkAtom; }

    const set<string>&    getWordsToSegnoSet () const
                              { return fWordsToSegnoSet; }
    S_oahStringSetElementAtom
                          getWordsToSegnoAtom () const
                              { return fWordsToSegnoAtom; }

    const set<string>&    getWordsToDalSegnoSet () const
                              { return fWordsToDalSegnoSet; }
    S_oahStringSetElementAtom
                          getWordsToDalSegnoAtom () const
                              { return fWordsToDalSegnoAtom; }

    const set<string>&    getWordsToDalSegnoAlFineSet () const
                              { return fWordsToDalSegnoAlFineSet; }
    S_oahStringSetElementAtom
                          getWordsToDalSegnoAlFineAtom () const
                              { return fWordsToDalSegnoAlFineAtom; }

    const set<string>&    getWordsToDalSegnoAlCodaSet () const
                              { return fWordsToDalSegnoAlCodaSet; }
    S_oahStringSetElementAtom
                          getWordsToSegnoAlCodaAtom () const
                              { return fWordsToDalSegnoAlCodaAtom; }

    const set<string>&    getWordsToCodaFirstSet () const
                              { return fWordsToCodaFirstSet; }
    S_oahStringSetElementAtom
                          getWordsToCodFirstaAtom () const
                              { return fWordsToCodaFirstAtom; }

    const set<string>&    getWordsToCodaSecondSet () const
                              { return fWordsToCodaSecondSet; }
    S_oahStringSetElementAtom
                          getWordsToCodaSecondAtom () const
                              { return fWordsToCodaSecondAtom; }

    const set<string>&    getWordsToCrescSet () const
                              { return fWordsToCrescSet; }
    S_oahStringSetElementAtom
                          getWordsToCrescAtom () const
                              { return fWordsToCrescAtom; }

    const set<string>&    getWordsToDecrescSet () const
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

  public:

    // public services
    // ------------------------------------------------------

    Bool                  wordsIsToBeConvertedToBold (
                            const string& wordsValue);

    Bool                  wordsIsToBeConvertedToItalic (
                            const string& wordsValue);

    Bool                  wordsIsToBePlacedAbove (
                            const string& wordsValue);

    Bool                  wordsIsToBePlacedBelow (
                            const string& wordsValue);

    Bool                  wordsIsToBeConvertedToTempo (
                            const string& wordsValue);

    Bool                  wordsIsToBeConvertedToRehearsalMark (
                            const string& wordsValue);

    Bool                  wordsIsToBeConvertedToSegno (
                            const string& wordsValue);

    Bool                  wordsIsToBeConvertedToDalSegno (
                            const string& wordsValue);

    Bool                  wordsIsToBeConvertedToDalSegnoAlFine (
                            const string& wordsValue);

    Bool                  wordsIsToBeConvertedToDalSegnoAlCoda (
                            const string& wordsValue);

    Bool                  wordsIsToBeConvertedToCodaFirst (
                            const string& wordsValue);
    Bool                  wordsIsToBeConvertedToCodaSecond (
                            const string& wordsValue);

    Bool                  wordsIsToBeConvertedToCresc (
                            const string& wordsValue);

    Bool                  wordsIsToBeConvertedToDecresc (
                            const string& wordsValue);

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  initializeMxsr2msrTraceOptions ();
#endif

    void                  initializePartsOptions ();

    void                  initializeStavesOptions ();

    void                  initializeVoiceOptions ();

    void                  initializeClefsKeysTimesOptions ();

    void                  initializePageAndLineBreaksOptions ();

    void                  initializeMeasuresOptions ();

    void                  initializeNotesOptions ();

    void                  initializeDynamicsAndWedgesOptions ();

    void                  initializeWordsOptions ();

    void                  initializeCombinedOptionsOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMxsr2msrHelp ();

    void                  printMxsr2msrValues (int valueFieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // options and help trace
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

    set<string>           fPartsIgnoreIDSet;
    set<string>           fMusicXMLPartsKeepIDSet;

    set<string>           fMusicXMLMusicXMLPartsIgnoreNameSet;
    set<string>           fMusicXMLPartsKeepNameSet;

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

    set<int>              fMusicXMLMusicXMLStavesIgnoreNumberSet;
    set<int>              fMusicXMLStavesKeepNumberSet;

     // for checkOptionsConsistency()
    S_oahIntSetElementAtom
                          fIgnoreStaffNumberAtom;
    S_oahIntSetElementAtom
                          fKeepStaffNumberAtom;

    // voices
    // --------------------------------------

    set<int>              fMusicXMLMusicXMLVoicesIgnoreNumberSet;
    set<int>              fMusicXMLVoicesKeepNumberSet;

     // for checkOptionsConsistency()
    S_oahIntSetElementAtom
                          fIgnoreVoiceNumberAtom;
    S_oahIntSetElementAtom
                          fKeepVoiceNumberAtom;

    // clefs, keys, time signatures
    // --------------------------------------

    map<msrClefKind, msrClefKind>
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

    Bool                  fIgnorePageBreaksInMusicXML;

    // line breaks
    // --------------------------------------

    Bool                  fIgnoreLineBreaksInMusicXML;

    // measures
    // --------------------------------------

    map<string,int>       fMeasuresToBeReplicatedStringToIntMap;

    map<string,int>       fAddEmptyMeasuresStringToIntMap;

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

    set<string>           fBoldWordsSet;
    S_oahStringSetElementAtom
                          fBoldWordsAtom;

    set<string>           fItalicWordsSet;
    S_oahStringSetElementAtom
                          fItalicWordsAtom;

    set<string>           fWordsToBePlacedAboveSet;
    S_oahStringSetElementAtom
                          fWordsToBePlacedAboveAtom;

    set<string>           fWordsToBePlacedBelowSet;
    S_oahStringSetElementAtom
                          fWordsToBePlacedBelowAtom;

    Bool                  fAddMsrWordsFromTheMusicXMLLyrics;

    // words conversions
    set<string>           fWordsToBeConvertedSet;

    set<string>           fWordsToTemposSet;
    S_oahStringSetElementAtom
                          fWordsToTempoAtom;

    set<string>           fWordsToRehearsalMarkSet;
    S_oahStringSetElementAtom
                          fWordsToRehearsalMarkAtom;

    set<string>           fWordsToSegnoSet;
    S_oahStringSetElementAtom
                          fWordsToSegnoAtom;

    set<string>           fWordsToDalSegnoSet;
    S_oahStringSetElementAtom
                          fWordsToDalSegnoAtom;

    set<string>           fWordsToDalSegnoAlFineSet;
    S_oahStringSetElementAtom
                          fWordsToDalSegnoAlFineAtom;

    set<string>           fWordsToDalSegnoAlCodaSet;
    S_oahStringSetElementAtom
                          fWordsToDalSegnoAlCodaAtom;

    set<string>           fWordsToCodaFirstSet;
    S_oahStringSetElementAtom
                          fWordsToCodaFirstAtom;

    set<string>           fWordsToCodaSecondSet;
    S_oahStringSetElementAtom
                          fWordsToCodaSecondAtom;

    set<string>           fWordsToCrescSet;
    S_oahStringSetElementAtom
                          fWordsToCrescAtom;

    set<string>           fWordsToDecrescSet;
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


#ifdef TRACING_IS_ENABLED
    // specific trace
    // --------------------------------------
#endif
};
typedef SMARTP<mxsr2msrOahGroup> S_mxsr2msrOahGroup;
EXP ostream& operator << (ostream& os, const S_mxsr2msrOahGroup& elt);

EXP extern S_mxsr2msrOahGroup gGlobalMxsr2msrOahGroup;

//______________________________________________________________________________
EXP S_mxsr2msrOahGroup createGlobalMxsr2msrOahGroup (
  S_oahHandler handler);


}


#endif
