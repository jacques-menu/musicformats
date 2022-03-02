/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrOah___
#define ___lpsrOah___

#include "msrPitchesNames.h"

#include "lpsrBasicTypes.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahAtomsCollection.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP lpsrPitchesLanguageAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lpsrPitchesLanguageAtom> create (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            msrQuarterTonesPitchesLanguageKind&
                                              lpsrPitchesLanguageKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrPitchesLanguageAtom (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            msrQuarterTonesPitchesLanguageKind&
                                              lpsrPitchesLanguageKindVariable);

    virtual               ~lpsrPitchesLanguageAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMsrQuarterTonesPitchesLanguageKindVariable (
                            msrQuarterTonesPitchesLanguageKind value)
                              {
                                fMsrQuarterTonesPitchesLanguageKindVariable = value;
                              }

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

    msrQuarterTonesPitchesLanguageKind&
                          fMsrQuarterTonesPitchesLanguageKindVariable;
};
typedef SMARTP<lpsrPitchesLanguageAtom> S_lpsrPitchesLanguageAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrPitchesLanguageAtom& elt);

//______________________________________________________________________________
class EXP lpsrChordsLanguageAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lpsrChordsLanguageAtom> create (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            lpsrChordsLanguageKind&
                                              lpsrChordsLanguageKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrChordsLanguageAtom (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            lpsrChordsLanguageKind&
                                               lpsrChordsLanguageKindVariable);

    virtual               ~lpsrChordsLanguageAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setLpsrChordsLanguageKindVariable (
                            lpsrChordsLanguageKind value)
                              {
                                fLpsrChordsLanguageKindVariable = value;
                              }

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

    lpsrChordsLanguageKind&
                          fLpsrChordsLanguageKindVariable;
};
typedef SMARTP<lpsrChordsLanguageAtom> S_lpsrChordsLanguageAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrChordsLanguageAtom& elt);

//______________________________________________________________________________
class EXP lpsrTransposeAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lpsrTransposeAtom> create (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            S_msrSemiTonesPitchAndOctave&
                                              semiTonesPitchAndOctaveVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrTransposeAtom (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            S_msrSemiTonesPitchAndOctave&
                                              semiTonesPitchAndOctaveVariable);

    virtual               ~lpsrTransposeAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setSemiTonesPitchAndOctaveVariable (
                            S_msrSemiTonesPitchAndOctave value)
                              {
                                fSemiTonesPitchAndOctaveVariable = value;
                              }

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

    S_msrSemiTonesPitchAndOctave&
                          fSemiTonesPitchAndOctaveVariable;
};
typedef SMARTP<lpsrTransposeAtom> S_lpsrTransposeAtom;
EXP ostream& operator<< (ostream& os, const S_lpsrTransposeAtom& elt);

//______________________________________________________________________________
class EXP lpsrOahGroup : public oahGroup
{
  public:

    static SMARTP<lpsrOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeLpsrOahGroup ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrOahGroup ();

    virtual               ~lpsrOahGroup ();

    // set and get
    // ------------------------------------------------------

    // trace
    // --------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  setTraceLpsr ()
                              { fTraceLpsr = true; }
    Bool                  getTraceLpsr () const
                              { return fTraceLpsr; }

    void                  setTraceLilypondVersion ()
                              { fTraceLilypondVersion = true; }
    Bool                  getTraceLilypondVersion () const
                              { return fTraceLilypondVersion; }

    void                  setTraceLpsrVisitors ()
                              { fTraceLpsrVisitors = true; }
    Bool                  getTraceLpsrVisitors () const
                              { return fTraceLpsrVisitors; }

    void                  setTraceLpsrBlocks ()
                              { fTraceLpsrBlocks = true; }
    Bool                  getTraceLpsrBlocks () const
                              { return fTraceLpsrBlocks; }

    void                  setTraceSchemeFunctions ()
                              { fTraceSchemeFunctions = true; }
    Bool                  getTraceSchemeFunctions () const
                              { return fTraceSchemeFunctions; }
#endif

    // display
    // --------------------------------------

    void                  setDisplayLpsr ()
                              { fDisplayLpsr = true; }
    Bool                  getDisplayLpsr () const
                              { return fDisplayLpsr; }

    void                  setDisplayLpsrFull ()
                              { fDisplayLpsrFull = true; }
    Bool                  getDisplayLpsrFull () const
                              { return fDisplayLpsrFull; }

    void                  setDisplayLpsrShort ()
                              { fDisplayLpsrShort = true; }
    Bool                  getDisplayLpsrShort () const
                              { return fDisplayLpsrShort; }

    // paper
    // --------------------------------------

    void                  setAllPaperVariables ()
                              { fAllPaperVariables = true; }
    Bool                  getAllPaperVariables () const
                              { return fAllPaperVariables; }

    msrLengthUnitKind     getLengthUnitKindDefaultValue () const
                              { return fLengthUnitKindDefaultValue; }

    // size
    msrLength             getPaperHeight () const
                              { return fPaperHeight; }
    S_oahLengthAtom       getPaperHeightAtom () const
                              { return fPaperHeightAtom; }

    msrLength             getPaperWidth () const
                              { return fPaperWidth; }
    S_oahLengthAtom       getPaperWidthAtom () const
                              { return fPaperWidthAtom; }

    // margins
    msrLength             getPaperLeftMargin () const
                              { return fPaperLeftMargin; }
    S_oahLengthAtom       getPaperLeftMarginAtom () const
                              { return fPaperLeftMarginAtom; }

    msrLength             getPaperRightMargin () const
                              { return fPaperRightMargin; }
    S_oahLengthAtom       getPaperRightMarginAtom () const
                              { return fPaperRightMarginAtom; }

    msrLength             getPaperTopMargin () const
                              { return fPaperTopMargin; }
    S_oahLengthAtom       getPaperTopMarginAtom () const
                              { return fPaperTopMarginAtom; }

    msrLength             getPaperBottomMargin () const
                              { return fPaperBottomMargin; }
    S_oahLengthAtom       getPaperBottomMarginAtom () const
                              { return fPaperBottomMarginAtom; }

    // indents
    msrLength             getPaperHorizontalShift () const
                              { return fPaperHorizontalShift; }
    msrLength             getPaperIndent () const
                              { return fPaperIndent; }
    msrLength             getPaperShortIndent () const
                              { return fPaperShortIndent; }

    // spaces
    msrLength             getMarkupSystemSpacingPadding () const
                              { return fMarkupSystemSpacingPadding; }
    msrLength             getBetweenSystemSpace () const
                              { return fBetweenSystemSpace; }
    msrLength             getPageTopSpace () const
                              { return fPageTopSpace; }

    // counts
    void                  setPageCount (int value)
                              { fPageCount = value; }
    int                   getPageCount () const
                              { return fPageCount; }

    void                  setSystemCount (int value)
                              { fSystemCount = value; }
    int                   getSystemCount () const
                              { return fSystemCount; }

    void                  setRaggedBottom ()
                              { fRaggedBottom = true; }
    Bool                  getRaggedBottom () const
                              { return fRaggedBottom; }

    void                  setRaggedLast ()
                              { fRaggedLast = true; }
    Bool                  getRaggedLast () const
                              { return fRaggedLast; }

    void                  setRaggedLastBottom ()
                              { fRaggedLastBottom = true; }
    Bool                  getRaggedLastBottom () const
                              { return fRaggedLastBottom; }

    void                  setRaggedRight ()
                              { fRaggedRight = true; }
    Bool                  getRaggedRight () const
                              { return fRaggedRight; }

    void                  setTagline ()
                              { fTagline = true; }
    Bool                  getTagline () const
                              { return fTagline; }

    // measures
    // --------------------------------------

    // replicate empty measure JMI ???
    string                getReplicateEmptyMeasureNumber () const
                              { return fReplicateEmptyMeasureNumber; }

    void                  setReplicateEmptyMeasureReplicas ()
                              { fReplicateEmptyMeasureReplicas = true; }
    int                   getReplicateEmptyMeasureReplicas () const
                              { return fReplicateEmptyMeasureReplicas; }

    // add empty measures
// JMI    map<string,int>       getAddEmptyMeasuresStringToIntMap () const;

    // tempos
    // --------------------------------------

    // convert tempos to rehearsal marks
    void                  setConvertMusicXMLTemposToMsrRehearsalMarks ()
                              { fConvertMusicXMLTemposToMsrRehearsalMarks = true; }
    Bool                  getConvertMusicXMLTemposToMsrRehearsalMarks () const
                              { return fConvertMusicXMLTemposToMsrRehearsalMarks; }

    // words
    // --------------------------------------

    // convert words to tempo
    void                  setConvertLpsrWordsToTempo ()
                              { fConvertLpsrWordsToTempo = true; }
    Bool                  getConvertLpsrWordsToTempo () const
                              { return fConvertLpsrWordsToTempo; }

    // add words from the lyrics
    void                  setAddLpsrWordsFromTheLyrics ()
                              { fAddLpsrWordsFromTheLyrics = true; }
    Bool                  getAddLpsrWordsFromTheLyrics () const
                              { return fAddLpsrWordsFromTheLyrics; }

    // convert LPSR words to rehearsal marks
    void                  setConvertLpsrWordsToRehearsalMarks ()
                              { fConvertLpsrWordsToRehearsalMarks = true; }
    Bool                  getConvertLpsrWordsToRehearsalMarks () const
                              { return fConvertLpsrWordsToRehearsalMarks; }

    // languages
    // --------------------------------------

    Bool                  setLpsrQuarterTonesPitchesLanguage (string language);

    msrQuarterTonesPitchesLanguageKind
                          getLpsrQuarterTonesPitchesLanguageKind () const
                              { return fLpsrQuarterTonesPitchesLanguageKind; }

    Bool                  setLpsrChordsLanguageKind (string language);

    lpsrChordsLanguageKind
                          getLpsrChordsLanguageKind () const
                              { return fLpsrChordsLanguageKind; }

    // transpose
    // --------------------------------------

    S_msrSemiTonesPitchAndOctave
                          getTransposeSemiTonesPitchAndOctave () const
                              { return fTransposeSemiTonesPitchAndOctave; }

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printLpsrOahHelp ();

    void                  printLpsrOahValues (int fieldWidth);

  private:

    // private methods
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  initializeLpsrtracingOah ();
#endif

    void                  initializeLpsrDisplayOptions ();

    void                  initializeLpsrPaperOptions ();

    void                  initializeLpsrVoicesOptions ();

    void                  initializeLpsrMeasuresOptions ();

    void                  initializeLpsrLanguagesOptions ();

    void                  initializeLpsrTransposeOptions ();

  private:

    // private fields
    // --------------------------------------

    // trace
    // --------------------------------------

#ifdef TRACING_IS_ENABLED
    Bool                  fTraceLpsr;

    Bool                  fTraceLilypondVersion;

    Bool                  fTraceLpsrVisitors;

    Bool                  fTraceLpsrBlocks;

    Bool                  fTraceSchemeFunctions;
#endif

    // display
    // --------------------------------------

    Bool                  fDisplayLpsr;
    Bool                  fDisplayLpsrFull;
    Bool                  fDisplayLpsrShort;

    // paper
    // --------------------------------------

    Bool                  fAllPaperVariables;

    msrLengthUnitKind     fLengthUnitKind;
    msrLengthUnitKind     fLengthUnitKindDefaultValue;

    // size
    msrLength             fPaperHeight;
    S_oahLengthAtom       fPaperHeightAtom;

    msrLength             fPaperWidth;
    S_oahLengthAtom       fPaperWidthAtom;

    // margins
    msrLength             fPaperLeftMargin;
    S_oahLengthAtom       fPaperLeftMarginAtom;

    msrLength             fPaperRightMargin;
    S_oahLengthAtom       fPaperRightMarginAtom;

    msrLength             fPaperTopMargin;
    S_oahLengthAtom       fPaperTopMarginAtom;

    msrLength             fPaperBottomMargin;
    S_oahLengthAtom       fPaperBottomMarginAtom;

    // indents
    msrLength             fPaperHorizontalShift;

    msrLength             fPaperIndent;
    msrLength             fPaperShortIndent;

    // spaces
    msrLength             fMarkupSystemSpacingPadding;

    msrLength             fBetweenSystemSpace;
    msrLength             fPageTopSpace;

    // counts
    int                   fPageCount;   // 0 if not specified
    int                   fSystemCount; // 0 if not specified

    // ragging
    Bool                  fRaggedBottom;
    Bool                  fRaggedLast;
    Bool                  fRaggedLastBottom;
    Bool                  fRaggedRight;

    // tag line
    Bool                  fTagline;

    // measures
    // --------------------------------------

    // replicate empty measure JMI ???
    string                fReplicateEmptyMeasureNumber;
    int                   fReplicateEmptyMeasureReplicas;

    // add empty measures
// JMI    map<string,int>       fAddEmptyMeasuresStringToIntMap;

    // tempos
    // --------------------------------------

    // convert tempos to rehearsal marks
    Bool                  fConvertMusicXMLTemposToMsrRehearsalMarks;

    // words
    // --------------------------------------

    // convert LPSR words to tempo
    Bool                  fConvertLpsrWordsToTempo;
    // add LPSR words from the lyrics
    Bool                  fAddLpsrWordsFromTheLyrics;

    // convert LPSR words to rehearsal marks
    Bool                  fConvertLpsrWordsToRehearsalMarks;

    // languages
    // --------------------------------------

    msrQuarterTonesPitchesLanguageKind
                          fLpsrQuarterTonesPitchesLanguageKind;

    lpsrChordsLanguageKind
                          fLpsrChordsLanguageKind;

    // transpose
    // --------------------------------------

    S_msrSemiTonesPitchAndOctave
                          fTransposeSemiTonesPitchAndOctave;
};
typedef SMARTP<lpsrOahGroup> S_lpsrOahGroup;
EXP ostream& operator<< (ostream& os, const S_lpsrOahGroup& elt);

EXP extern S_lpsrOahGroup gGlobalLpsrOahGroup;

//______________________________________________________________________________
EXP S_lpsrOahGroup createGlobalLpsrOahGroup ();


}


#endif

