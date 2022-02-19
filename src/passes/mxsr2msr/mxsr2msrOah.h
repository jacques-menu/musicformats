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

#include "msrBasicTypes.h"

#include "oahAtomsCollection.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrDalSegnoAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrDalSegnoAtom> create (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            map<string, msrDalSegno::msrDalSegnoKind>&
                                              stringToDalSegnoKindMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrDalSegnoAtom (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            map<string, msrDalSegno::msrDalSegnoKind>&
                                              stringToDalSegnoKindMapVariable);

    virtual               ~msrDalSegnoAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<string, msrDalSegno::msrDalSegnoKind>&
                          getStringToDalSegnoKindMapVariable () const
                              { return fStringToDalSegnoKindMapVariable; }

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

    map<string, msrDalSegno::msrDalSegnoKind>&
                          fStringToDalSegnoKindMapVariable;
};
typedef SMARTP<msrDalSegnoAtom> S_msrDalSegnoAtom;
EXP ostream& operator<< (ostream& os, const S_msrDalSegnoAtom& elt);

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
EXP ostream& operator<< (ostream& os, const S_msrReplaceClefAtom& elt);

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
    const set<string>&    getPartsKeepIDSet () const
                              { return fPartsKeepIDSet; }

    const set<string>&    getPartsIgnoreNameSet () const
                              { return fPartsIgnoreNameSet; }
    const set<string>&    getPartsKeepNameSet () const
                              { return fPartsKeepNameSet; }

    // for checkOptionsConsistency()
    S_oahStringSetAtom    getIgnorePartIDAtom () const
                              { return fIgnorePartIDAtom; }
    S_oahStringSetAtom    getKeepPartIDAtom () const
                              { return fKeepPartIDAtom; }

    S_oahStringSetAtom    getIgnorePartNameAtom () const
                              { return fIgnorePartNameAtom; }
    S_oahStringSetAtom    getKeepPartNameAtom () const
                              { return fKeepPartNameAtom; }

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

    // page breaks
    // --------------------------------------

    Bool                  getIgnorePageBreaksInMusicXML () const
                              { return fIgnorePageBreaksInMusicXML; }

    // measures
    // --------------------------------------

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

    // words
    // --------------------------------------

    Bool                  getIgnoreMusicXMLWords () const
                              { return fIgnoreMusicXMLWords; }

    Bool                  getConvertMusicXMLWordsToMSRTempos () const
                              { return fConvertMusicXMLWordsToMSRTempos; }

    Bool                  getAddMsrWordsFromTheMusicXMLLyrics () const
                              { return fAddMsrWordsFromTheMusicXMLLyrics; }

    Bool                  getConvertMusicXMLWordsToMSRRehearsalMarks () const
                              { return fConvertMusicXMLWordsToMSRRehearsalMarks; }

    Bool                  getConvertMusicXMLTemposToMSRRehearsalMarks () const
                              { return fConvertMusicXMLTemposToMSRRehearsalMarks; }

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

    Bool                  getIgnoreFiguredBassElements () const
                              { return fIgnoreFiguredBassElements; }

    // dynamics and wedges
    // --------------------------------------

    Bool                  getAllDynamicsBelow () const
                              { return fAllDynamicsBelow; }
    Bool                  getAllWedgesBelow () const
                              { return fAllWedgesBelow; }

    // convert words to dal segno
    // --------------------------------------

    const map<string, msrDalSegno::msrDalSegnoKind>&
                          getConverStringToMsrDalSegnoMap () const
                              { return fConverStringToMsrDalSegnoMap; }

    // combined options, cubase
    // --------------------------------------

    void                  setCubase ()
                              { fCubase = true; }
    Bool                  getCubase () const
                              { return fCubase; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  initializeMxsr2msrTraceOptions ();
#endif

    void                  initializePartsOptions ();

    void                  initializeClefsKeysTimesOptions ();

    void                  initializePageBreaksOptions ();

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
    set<string>           fPartsKeepIDSet;

    set<string>           fPartsIgnoreNameSet;
    set<string>           fPartsKeepNameSet;

    // for checkOptionsConsistency()
    S_oahStringSetAtom    fIgnorePartIDAtom;
    S_oahStringSetAtom    fKeepPartIDAtom;

    S_oahStringSetAtom    fIgnorePartNameAtom;
    S_oahStringSetAtom    fKeepPartNameAtom;

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

    // page breaks
    // --------------------------------------

    Bool                  fIgnorePageBreaksInMusicXML;

    // measures
    // --------------------------------------

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

    Bool                  fConvertMusicXMLWordsToMSRTempos;
    Bool                  fConvertMusicXMLWordsToMSRRehearsalMarks;

    Bool                  fConvertMusicXMLTemposToMSRRehearsalMarks; // JMI???

    Bool                  fAddMsrWordsFromTheMusicXMLLyrics;

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

    Bool                  fIgnoreFiguredBassElements;

    // dynamics and wedges
    // --------------------------------------

    Bool                  fAllDynamicsBelow;
    Bool                  fAllWedgesBelow;

    // convert words to dal segno
    // --------------------------------------

    map<string, msrDalSegno::msrDalSegnoKind>
                          fConverStringToMsrDalSegnoMap;

    // combined options, cubase, needed in case it occurs in MusicXML <software/>
    // --------------------------------------

    Bool                  fCubase;


#ifdef TRACING_IS_ENABLED
    // specific trace
    // --------------------------------------
#endif
};
typedef SMARTP<mxsr2msrOahGroup> S_mxsr2msrOahGroup;
EXP ostream& operator<< (ostream& os, const S_mxsr2msrOahGroup& elt);

EXP extern S_mxsr2msrOahGroup gGlobalMxsr2msrOahGroup;

//______________________________________________________________________________
EXP S_mxsr2msrOahGroup createGlobalMxsr2msrOahGroup (
  S_oahHandler handler);


}


#endif
