/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrScores___
#define ___lpsrScores___

#include "msr.h"

#include "lpsrComments.h"

#include "lpsrScheme.h"

#include "lpsrHeaders.h"
#include "lpsrPapers.h"
#include "lpsrLayouts.h"

#include "lpsrParallelMusic.h"

#include "lpsrBookBlockElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP lpsrScore : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrScore> create (
                            int                 inputLineNumber,
                            S_msrScore          theMsrScore,
                            S_mfcMultiComponent multiComponent);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrScore (
                            int                 inputLineNumber,
                            S_msrScore          theMsrScore,
                            S_mfcMultiComponent multiComponent);

    virtual               ~lpsrScore ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrScore            getMsrScore () const
                              { return fMsrScore; }

    S_lpsrHeader          getScoreHeader () const
                              { return fScoreHeader; }

    S_lpsrPaper           getScorePaper () const
                              { return fScorePaper; }

    S_lpsrLayout          getScoreLayout () const
                              { return fScoreLayout; }

    const list<S_msrElement>&
                          getScoreElementsList () const
                              { return fScoreElementsList; }

    const list<S_lpsrBookBlock>&
                          getScoreBookBlocksList () const
                              { return fScoreBookBlocksList; }

    S_lpsrScoreBlock      getScoreScoreBlock () const
                              { return fScoreScoreBlock; }

    // files includes
    // ---------------------

    void                  setJianpuFileIncludeIsNeeded ();

    Bool                  getJianpuFileIncludeIsNeeded () const
                              { return fJianpuFileIncludeIsNeeded; }

    // Scheme modules
    // ---------------------

    void                  setScmAndAccregSchemeModulesAreNeeded ();

    Bool                  getScmAndAccregSchemeModulesAreNeeded () const
                              { return fScmAndAccregSchemeModulesAreNeeded; }

    // Scheme functions
    // ---------------------

    // custom short bar lines
    void                  setCustomShortBarLineSchemeFunctionIsNeeded ();

    Bool                  getCustomShortBarLineSchemeFunctionIsNeeded () const
                              { return fCustomShortBarLineSchemeFunctionIsNeeded; }

    // tongue
    void                  setTongueSchemeFunctionIsNeeded ();

    Bool                  getTongueSchemeFunctionIsNeeded () const
                              { return fTongueSchemeFunctionIsNeeded; }


    // editorial accidentals
    void                  setEditorialAccidentalSchemeFunctionIsNeeded ();

    Bool                  getEditorialAccidentalSchemeFunctionIsNeeded () const
                              {
                                return
                                  fEditorialAccidentalSchemeFunctionIsNeeded;
                              }

    // dynamics
    void                  setDynamicsSchemeFunctionIsNeeded ();

    Bool                  getDynamicsSchemeFunctionIsNeeded () const
                              {
                                return
                                  fDynamicsSchemeFunctionIsNeeded;
                              }

    // tuplets curved brackets
    void                  setTupletsCurvedBracketsSchemeFunctionIsNeeded ();

    Bool                  getTupletsCurvedBracketsSchemeFunctionIsNeeded () const
                              {
                                return
                                  fTupletsCurvedBracketsSchemeFunctionIsNeeded;
                              }

    // after
    void                  setAfterSchemeFunctionIsNeeded ();

    Bool                  getAfterSchemeFunctionIsNeeded () const
                              {
                                return
                                  fAfterSchemeFunctionIsNeeded;
                              }

    // tempo relationships
    void                  setTempoNotesRelationshipshipSchemeFunctionIsNeeded ();

    Bool                  getTempoNotesRelationshipshipSchemeFunctionIsNeeded () const
                              {
                                return
                                  fTempoNotesRelationshipshipSchemeFunctionIsNeeded;
                              }

    // glissando with text
    void                  setGlissandoWithTextSchemeFunctionsIsNeeded ();

    Bool                  getGlissandoWithTextSchemeFunctionsIsNeeded () const
                              {
                                return
                                  fGlissandoWithTextSchemeFunctionsIsNeeded;
                              }

    // auto-voices
    void                  setAutoVoicesSchemeFunctionIsNeeded ();

    Bool                  getAutoVoicesSchemeFunctionIsNeeded () const
                              {
                                return
                                  fAutoVoicesSchemeFunctionIsNeeded;
                              }

    // schleifer
    void                  setSchleiferSchemeFunctionIsNeeded ();

    Bool                  getSchleiferSchemeFunctionIsNeeded () const
                              {
                                return
                                  fSchleiferSchemeFunctionIsNeeded;
                              }

    // other dynamic

    void                  setOtherDynamicSchemeFunctionIsNeeded ();

    Bool                  getOtherDynamicSchemeFunctionIsNeeded () const
                              {
                                return
                                  fOtherDynamicSchemeFunctionIsNeeded;
                              }
    // damps
    void                  setDampMarkupIsNeeded ();

    Bool                  getDampMarkupIsNeeded () const
                              {
                                return
                                  fDampMarkupIsNeeded;
                              }

    void                  setDampAllMarkupIsNeeded ();

    Bool                  getDampAllMarkupIsNeeded () const
                              {
                                return
                                  fDampAllMarkupIsNeeded;
                              }

    // white note heads
    void                  setWhiteNoteHeadsIsNeeded ();

    Bool                  getWhiteNoteHeadsIsNeeded () const
                              {
                                return
                                  fWhiteNoteHeadsIsNeeded;
                              }

    // bar numbers
    void                  setBoxAroundNextBarNumberIsNeeded ();

    Bool                  getBoxAroundNextBarNumberIsNeeded () const
                              {
                                return
                                  fBoxAroundNextBarNumberIsNeeded;
                              }

    // jazz chords display
    void                  setJazzChordsDisplayIsNeeded ();

    Bool                  getJazzChordsDisplayIsNeeded () const
                              {
                                return
                                  fJazzChordsDisplayIsNeeded;
                              }

    // colored ledger lines
    void                  setColoredLedgerLinesIsNeeded ();

    Bool                  getColoredLedgerLinesIsNeeded () const
                              {
                                return
                                  fColoredLedgerLinesIsNeeded;
                              }

    // hidden measure and barLine
    void                  setHiddenMeasureAndBarLineIsNeeded ();

    Bool                  getHiddenMeasureAndBarLineIsNeeded () const
                              {
                                return
                                  fHiddenMeasureAndBarLineIsNeeded;
                              }

    // merge rests
    void                  setMergeRestsIsNeeded ();

    Bool                  getMergeRestsIsNeeded () const
                              {
                                return
                                  fMergeRestsIsNeeded;
                              }

    // spanners with centered text

    void                  setTextSpannerWithCenteredTextIsNeeded ();

    Bool                  getTextSpannerWithCenteredTextIsNeeded () const
                              {
                                return
                                  fTextSpannerWithCenteredTextIsNeeded;
                              }


  public:

    // public services
    // ------------------------------------------------------

    void                  appendVoiceToLpsrScoreElementsList (
                            S_msrVoice voice)
                              { fScoreElementsList.push_back (voice); }

    void                  appendStanzaToLpsrScoreElementsList (
                            S_msrStanza stanza)
                              { fScoreElementsList.push_back (stanza); }

    void                  appendBookBlockToBookBlocksList (
                            S_lpsrBookBlock bookBlock)
                              { fScoreBookBlocksList.push_back (bookBlock); }

    void                  addGlissandoWithTextSchemeFunctionsToScore ();

  private:

    // private services
    // ------------------------------------------------------

    // files includes

    void                  addJianpuFileIncludeToScore ();

    // Scheme modules

    void                  addAccordionRegistrationSchemeModulesToScore ();

    // Scheme functions

    void                  addDateAndTimeSchemeFunctionsToScore ();

    void                  addPointAndClickOffSchemeFunctionsToScore ();

    void                  addCustomShortBarLineSchemeFunctionToScore ();

    void                  addTongueSchemeFunctionToScore ();

    void                  addEditorialAccidentalSchemeFunctionToScore ();

    void                  addDynamicsSchemeFunctionToScore ();

    void                  addTupletsCurvedBracketsSchemeFunctionToScore ();

    void                  addAfterSchemeFunctionToScore ();

    void                  addTempoNotesRelationshipshipSchemeFunctionToScore ();

    void                  addOtherDynamicSchemeFunctionToScore ();

    void                  addAutoVoicesSchemeFunctionToScore ();

    void                  addSchleiferSchemeFunctionToScore ();

    // markups

    void                  addDampMarkupToScore ();
    void                  addDampAllMarkupToScore ();

    // white note heads

    void                  addWhiteNoteHeadsToScore ();

    // bar numbers
    void                  addBoxAroundNextBarNumberToScore ();

    // jazz chords display

    void                  addJazzChordsDisplayToScore ();

    // colored ledger lines

    void                  addColoredLedgerLinesToScore ();

    // hidden measure and barLine

    void                  addHiddenMeasureAndBarLineToScore ();

    // merge rests

    void                  addMergeRestsToScore ();

    // spanners with centered text

    void                  addTextSpannerWithCenteredTextToScore ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printShort (ostream& os) const override;

    void                  printFull (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // MSR data
    S_msrScore            fMsrScore;

    // the multi-component
    // ------------------------------------------------------
    S_mfcMultiComponent   fMultiComponent;

    // command line
    S_lpsrComment         fInputSourceNameComment;

    S_lpsrComment         fCommandLineAsSuppliedComment;
    S_lpsrComment         fCommandLineLongOptionsComment;
    S_lpsrComment         fCommandLineShortOptionsComment;

    // conversion date
    S_lpsrComment         fRunDateComment;

    // LilyPond stuff
    S_lpsrHeader          fScoreHeader;
    S_lpsrPaper           fScorePaper;
    S_lpsrLayout          fScoreLayout;

    // variables, voices and stanzas
    list<S_msrElement>    fScoreElementsList;

    // score LPSR book blocks list
    list<S_lpsrBookBlock> fScoreBookBlocksList;
    S_lpsrScoreBlock      fScoreScoreBlock; // JMI ???

    // files includes
    Bool                  fJianpuFileIncludeIsNeeded;

    // Scheme modules
    Bool                  fScmAndAccregSchemeModulesAreNeeded;

    // Scheme functions
    Bool                  fTongueSchemeFunctionIsNeeded;

    Bool                  fCustomShortBarLineSchemeFunctionIsNeeded;

    Bool                  fEditorialAccidentalSchemeFunctionIsNeeded;

    Bool                  fDynamicsSchemeFunctionIsNeeded;

    Bool                  fTupletsCurvedBracketsSchemeFunctionIsNeeded;

    Bool                  fAfterSchemeFunctionIsNeeded;

    Bool                  fTempoNotesRelationshipshipSchemeFunctionIsNeeded;

    Bool                  fGlissandoWithTextSchemeFunctionsIsNeeded;

    Bool                  fOtherDynamicSchemeFunctionIsNeeded;

    Bool                  fAutoVoicesSchemeFunctionIsNeeded;

    Bool                  fSchleiferSchemeFunctionIsNeeded;

    // markups
    Bool                  fDampMarkupIsNeeded;
    Bool                  fDampAllMarkupIsNeeded;

    // white note heads
    Bool                  fWhiteNoteHeadsIsNeeded;

    // bar numbers
    Bool                  fBoxAroundNextBarNumberIsNeeded;

    // jazz chords display
    Bool                  fJazzChordsDisplayIsNeeded;

    // colored ledger lines
    Bool                  fColoredLedgerLinesIsNeeded;

    // hidden measure and barLine
    Bool                  fHiddenMeasureAndBarLineIsNeeded;

    // merge rests
    Bool                  fMergeRestsIsNeeded;

    // spanners with centered text

    Bool                  fTextSpannerWithCenteredTextIsNeeded;

  private:

    // private work fields
    // ------------------------------------------------------

    // the Scheme functions map
    map<string, S_lpsrSchemeFunction>
                          fScoreSchemeFunctionsMap;
};
typedef SMARTP<lpsrScore> S_lpsrScore;
EXP ostream& operator<< (ostream& os, const S_lpsrScore& elt);


}


#endif
