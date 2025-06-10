/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrOah___
#define ___lpsrOah___

#include "msrPitchesNames.h"

#include "lpsrEnumTypes.h"

#include "mfPreprocessorSettings.h"

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
                            const std::string&          longName,
                            const std::string&          shortName,
                            const std::string&          description,
                            const std::string&          valueSpecification,
                            const std::string&          variableName,
                            msrQuarterTonesPitchesLanguageKind&
                                              lpsrPitchesLanguageKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrPitchesLanguageAtom (
                            const std::string&          longName,
                            const std::string&          shortName,
                            const std::string&          description,
                            const std::string&          valueSpecification,
                            const std::string&          variableName,
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

    msrQuarterTonesPitchesLanguageKind&
                          fMsrQuarterTonesPitchesLanguageKindVariable;
};
typedef SMARTP<lpsrPitchesLanguageAtom> S_lpsrPitchesLanguageAtom;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrPitchesLanguageAtom& elt);

//______________________________________________________________________________
class EXP lpsrChordsLanguageAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lpsrChordsLanguageAtom> create (
                            const std::string&          longName,
                            const std::string&          shortName,
                            const std::string&          description,
                            const std::string&          valueSpecification,
                            const std::string&          variableName,
                            lpsrChordsLanguageKind&
                                              lpsrChordsLanguageKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrChordsLanguageAtom (
                            const std::string&          longName,
                            const std::string&          shortName,
                            const std::string&          description,
                            const std::string&          valueSpecification,
                            const std::string&          variableName,
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

    lpsrChordsLanguageKind&
                          fLpsrChordsLanguageKindVariable;
};
typedef SMARTP<lpsrChordsLanguageAtom> S_lpsrChordsLanguageAtom;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrChordsLanguageAtom& elt);

//______________________________________________________________________________
class EXP lpsrStaffInstrumentNameAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lpsrStaffInstrumentNameAtom> create (
                            const std::string&         longName,
                            const std::string&         shortName,
                            const std::string&         description,
                            const std::string&         valueSpecification,
                            const std::string&         variableName,
                            std::map <std::string, std::string>&  stringToStringMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrStaffInstrumentNameAtom (
                            const std::string&         longName,
                            const std::string&         shortName,
                            const std::string&         description,
                            const std::string&         valueSpecification,
                            const std::string&         variableName,
                            std::map <std::string, std::string>&  stringToStringMapVariable);

    virtual               ~lpsrStaffInstrumentNameAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::map <std::string, std::string>&
                          getStringToStringMapVariable () const
                              { return fStringToStringMapVariable; }

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

    std::map <std::string, std::string>&  fStringToStringMapVariable;
};
typedef SMARTP<lpsrStaffInstrumentNameAtom> S_lpsrStaffInstrumentNameAtom;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrStaffInstrumentNameAtom& elt);

//______________________________________________________________________________
class EXP lpsrTransposeAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lpsrTransposeAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            S_msrSemiTonesPitchAndOctave&
                                               semiTonesPitchAndOctaveVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrTransposeAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            S_msrSemiTonesPitchAndOctave&
                                               semiTonesPitchAndOctaveVariable);

    virtual               ~lpsrTransposeAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setSemiTonesPitchAndOctaveVariable (
                            const S_msrSemiTonesPitchAndOctave& value)
                              {
                                fSemiTonesPitchAndOctaveVariable = value;
                              }

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

    S_msrSemiTonesPitchAndOctave&
                          fSemiTonesPitchAndOctaveVariable;
};
typedef SMARTP<lpsrTransposeAtom> S_lpsrTransposeAtom;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrTransposeAtom& elt);

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

#ifdef MF_TRACE_IS_ENABLED
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
#endif // MF_TRACE_IS_ENABLED

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

    void                  setDisplayLpsrEmbeddedMsr ()
                              { fDisplayLpsrEmbeddedMsr = true; }
    Bool                  getDisplayLpsrEmbeddedMsr () const
                              { return fDisplayLpsrEmbeddedMsr; }

    void                  setDisplayLpsrNames ()
                              { fDisplayLpsrNames = true; }

    Bool                  getDisplayLpsrNames () const
                              { return fDisplayLpsrNames; }

    void                  setDisplayLpsrSummary ()
                              { fDisplayLpsrSummary = true; }
    Bool                  getDisplayLpsrSummary () const
                              { return fDisplayLpsrSummary; }

    void                  setDisplayLpsrFlatView ()
                              { fDisplayLpsrFlatView = true; }
    Bool                  getDisplayLpsrFlatView () const
                              { return fDisplayLpsrFlatView; }

    void                  setDisplayLpsrMeasuresSlices ()
                              { fDisplayLpsrMeasuresSlices = true; }
    Bool                  getDisplayLpsrMeasuresSlices () const
                              { return fDisplayLpsrMeasuresSlices; }

    void                  setDisplayLpsrMeasuresSlicesDetails ()
                              { fDisplayLpsrMeasuresSlicesDetails = true; }
    Bool                  getDisplayLpsrMeasuresSlicesDetails () const
                              { return fDisplayLpsrMeasuresSlicesDetails; }

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
    S_oahLengthAtom       getPaperHorizontalShiftAtom () const
                              { return fPaperHorizontalShiftAtom; }

    msrLength             getPaperIndent () const
                              { return fPaperIndent; }
    S_oahLengthAtom       getPaperIndentAtom () const
                              { return fPaperIndentAtom; }

    msrLength             getPaperShortIndent () const
                              { return fPaperShortIndent; }
    S_oahLengthAtom       getPaperShortIndentAtom () const
                              { return fPaperShortIndentAtom; }

    // spaces
    msrLength             getMarkupSystemSpacingPadding () const
                              { return fMarkupSystemSpacingPadding; }
    S_oahLengthAtom       getMarkupSystemSpacingPaddingAtom () const
                              { return fMarkupSystemSpacingPaddingAtom; }

    msrLength             getBetweenSystemSpace () const
                              { return fBetweenSystemSpace; }
    S_oahLengthAtom       getBetweenSystemSpaceAtom () const
                              { return fBetweenSystemSpaceAtom; }

    msrLength             getPageTopSpacing () const
                              { return fPageTopSpacing; }
    S_oahLengthAtom       getPageTopSpacingAtom () const
                              { return fPageTopSpacingAtom; }

    // counts
    void                  setPageCount (int value)
                              { fPageCount = value; }

    int                   getPageCount () const
                              { return fPageCount; }
    S_oahIntegerAtom      getPageCountAtom () const
                              { return fPageCountAtom; }

    void                  setSystemCount (int value)
                              { fSystemCount = value; }

    int                   getSystemCount () const
                              { return fSystemCount; }
    S_oahIntegerAtom      getSystemCountAtom () const
                              { return fSystemCountAtom; }

    void                  setRaggedBottom (mfOnOffKind value)
                              { fRaggedBottom = value; }

    mfOnOffKind           getRaggedBottom () const
                              { return fRaggedBottom; }
    S_oahOnOffAtom        getRaggedBottomAtom () const
                              { return fRaggedBottomAtom; }

    void                  setRaggedLast (mfOnOffKind value)
                              { fRaggedLast = value; }

    mfOnOffKind           getRaggedLast () const
                              { return fRaggedLast; }
    S_oahOnOffAtom        getRaggedLastAtom () const
                              { return fRaggedLastAtom; }

    void                  setRaggedLastBottom (mfOnOffKind value)
                              { fRaggedLastBottom = value; }

    mfOnOffKind           getRaggedLastBottom () const
                              { return fRaggedLastBottom; }
    S_oahOnOffAtom        getRaggedLastBottomAtom () const
                              { return fRaggedLastBottomAtom; }

    void                  setRaggedRight (mfOnOffKind value)
                              { fRaggedRight = value; }

    mfOnOffKind           getRaggedRight () const
                              { return fRaggedRight; }
    S_oahOnOffAtom        getRaggedRightAtom () const
                              { return fRaggedRightAtom; }

    void                  setTagline ()
                              { fTagline = true; }

    Bool                  getTagline () const
                              { return fTagline; }
    const S_oahBooleanAtom&
                          getTaglineAtom () const
                              { return fTaglineAtom; }

    // staves
    // --------------------------------------

    const std::map <std::string, std::string>&
                          getLpsrStavesInstrumentsNamesMap () const
                              { return fLpsrStavesInstrumentsNamesMap; }

    S_lpsrStaffInstrumentNameAtom
                          getLpsrStavesInstrumentsNamesMapAtom () const
                              { return fLpsrStavesInstrumentsNamesMapAtom; }

    // measures
    // --------------------------------------

    // replicate empty measure JMI ???
    std::string           getReplicateEmptyMeasureNumber () const
                              { return fReplicateEmptyMeasureNumber; }

    void                  setReplicateEmptyMeasureReplicas ()
                              { fReplicateEmptyMeasureReplicas = true; }
    int                   getReplicateEmptyMeasureReplicas () const
                              { return fReplicateEmptyMeasureReplicas; }

    // add empty measures
// JMI    std::map <std::string, int>       getAddEmptyMeasuresStringToIntMap () const;

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

    Bool                  setLpsrQuarterTonesPitchesLanguage (std::string language);

    msrQuarterTonesPitchesLanguageKind
                          getLpsrQuarterTonesPitchesLanguageKind () const
                              { return fLpsrQuarterTonesPitchesLanguageKind; }

    Bool                  setLpsrChordsLanguageKind (std::string language);

    lpsrChordsLanguageKind
                          getLpsrChordsLanguageKind () const
                              { return fLpsrChordsLanguageKind; }

    // transposition
    // --------------------------------------

    S_msrSemiTonesPitchAndOctave
                          getTranspositionSemiTonesPitchAndOctave () const
                              { return fTranspositionSemiTonesPitchAndOctave; }

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

    void                  displayLpsrOahValues (int fieldWidth);

  private:

    // private methods
    // ------------------------------------------------------

#ifdef MF_TRACE_IS_ENABLED
    void                  initializeLpsrTraceOah ();
#endif // MF_TRACE_IS_ENABLED

    void                  initializeLpsrDisplayOptions ();

    void                  initializeLpsrPaperOptions ();

    void                  initializeLpsrStavesOptions ();

    void                  initializeLpsrVoicesOptions ();

    void                  initializeLpsrMeasuresOptions ();

    void                  initializeLpsrLanguagesOptions ();

    void                  initializeLpsrTransposeOptions ();

  private:

    // private fields
    // --------------------------------------

    // trace
    // --------------------------------------

#ifdef MF_TRACE_IS_ENABLED
    Bool                  fTraceLpsr;

    Bool                  fTraceLilypondVersion;

    Bool                  fTraceLpsrVisitors;

    Bool                  fTraceLpsrBlocks;

    Bool                  fTraceSchemeFunctions;
#endif // MF_TRACE_IS_ENABLED

    // display
    // --------------------------------------

    Bool                  fDisplayLpsr;
    Bool                  fDisplayLpsrFull;

    Bool                  fDisplayLpsrEmbeddedMsr;

    Bool                  fDisplayLpsrNames;

    Bool                  fDisplayLpsrSummary;

    Bool                  fDisplayLpsrFlatView;
    Bool                  fDisplayLpsrFlatViewDetails;

    Bool                  fDisplayLpsrMeasuresSlices;
    Bool                  fDisplayLpsrMeasuresSlicesDetails;

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
    S_oahLengthAtom       fPaperHorizontalShiftAtom;

    msrLength             fPaperIndent;
    S_oahLengthAtom       fPaperIndentAtom;

    msrLength             fPaperShortIndent;
    S_oahLengthAtom       fPaperShortIndentAtom;

    // spaces
    msrLength             fMarkupSystemSpacingPadding;
    S_oahLengthAtom       fMarkupSystemSpacingPaddingAtom;

    msrLength             fBetweenSystemSpace;
    S_oahLengthAtom       fBetweenSystemSpaceAtom;

    msrLength             fPageTopSpacing;
    S_oahLengthAtom       fPageTopSpacingAtom;

    // counts
    int                   fPageCount;   // 0 by default
    S_oahIntegerAtom      fPageCountAtom;

    int                   fSystemCount; // 0 by default
    S_oahIntegerAtom      fSystemCountAtom;

    // ragged output
    mfOnOffKind           fRaggedBottom;
    S_oahOnOffAtom        fRaggedBottomAtom;

    mfOnOffKind           fRaggedLast;
    S_oahOnOffAtom        fRaggedLastAtom;

    mfOnOffKind           fRaggedLastBottom;
    S_oahOnOffAtom        fRaggedLastBottomAtom;

    mfOnOffKind           fRaggedRight;
    S_oahOnOffAtom        fRaggedRightAtom;

    // tag line
    Bool                  fTagline;
    S_oahBooleanAtom      fTaglineAtom;

    // measures
    // --------------------------------------

    // replicate empty measure JMI ???
    std::string           fReplicateEmptyMeasureNumber;
    int                   fReplicateEmptyMeasureReplicas;

    // add empty measures
// JMI    std::map <std::string, int>       fAddEmptyMeasuresStringToIntMap;

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

    // staves instrument names
    std::map <std::string, std::string>
                          fLpsrStavesInstrumentsNamesMap;
    S_lpsrStaffInstrumentNameAtom
                          fLpsrStavesInstrumentsNamesMapAtom;

    // transposition
    // --------------------------------------

    S_msrSemiTonesPitchAndOctave
                          fTranspositionSemiTonesPitchAndOctave;
};
typedef SMARTP<lpsrOahGroup> S_lpsrOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrOahGroup& elt);

//______________________________________________________________________________
// hidden global LPSR OAH group variable
EXP S_lpsrOahGroup getGlobalLpsrOahGroup ();

// a handy shortcut
#define gLpsrOahGroup getGlobalLpsrOahGroup ()

//______________________________________________________________________________
EXP S_lpsrOahGroup createGlobalLpsrOahGroup ();


}


#endif // ___lpsrOah___

