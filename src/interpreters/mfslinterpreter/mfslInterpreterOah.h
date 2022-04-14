/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfslInterpreterOah___
#define ___mfslInterpreterOah___

#include "msr.h"

#include "msrGenerationBasicTypes.h"

#include "brailleGenerationOah.h"
#include "brailleGeneration.h"


namespace MusicFormats
{

//______________________________________________________________________________
#define MSDR_STANDARD_INPUT_NAME ""
//#define MSDR_STANDARD_INPUT_NAME "-"

//______________________________________________________________________________
class EXP mfslInterpreterOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfslInterpreterOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMfslInterpreterOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfslInterpreterOahGroup ();

    virtual               ~mfslInterpreterOahGroup ();

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

    void                  setTraceScanning ()
                              { fTraceScanning = true; }

    Bool                  getTraceScanning () const
                              { return fTraceScanning; }

    void                  setDisplayTokens ()
                              { fDisplayTokens = true; }

    Bool                  getDisplayTokens () const
                              { return fDisplayTokens; }

    void                  setTraceParsing ()
                              { fTraceParsing = true; }

    Bool                  getTraceParsing () const
                              { return fTraceParsing; }

    void                  setDisplayToolAndInput ()
                              { fDisplayToolAndInput = true; }

    Bool                  getDisplayToolAndInput () const
                              { return fDisplayToolAndInput; }

    void                  setDisplayOptions ()
                              { fDisplayOptions = true; }

    Bool                  getDisplayOptions () const
                              { return fDisplayOptions; }

    void                  setTraceChoices ()
                              { fTraceChoices = true; }

    Bool                  getTraceChoices () const
                              { return fTraceChoices; }

    void                  setTraceChoiceStatements ()
                              { fTraceChoiceStatements = true; }

    Bool                  getTraceChoiceStatements () const
                              { return fTraceChoiceStatements; }

    void                  setTraceCaseStatements ()
                              { fTraceCaseStatements = true; }

    Bool                  getTraceCaseStatements () const
                              { return fTraceCaseStatements; }

    void                  setTraceOptionsBlocks ()
                              { fTraceOptionsBlocks = true; }

    Bool                  getTraceOptionsBlocks () const
                              { return fTraceOptionsBlocks; }

    void                  setNoLaunch ()
                              { fNoLaunch = true; }

    Bool                  getNoLaunch () const
                              { return fNoLaunch; }

    const set<string>&    getInputSourcesSet () const
                              { return fInputSourcesSet; }

    S_oahStringSetElementAtom
                          getInputSourcesSetAtom () const
                              { return fInputSourcesSetAtom; }

    const multimap<string, string>&
                          getSelectChoiceToLabelsMultiMap () const
                              { return fSelectChoiceToLabelsMultiMap; }

    S_oahStringToStringMultiMapElementAtom
                          getSelectChoiceToLabelsMultiMapAtom () const
                              { return fSelectChoiceToLabelsMultiMapAtom; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
// JMI    void                  initializemfslInterpretertracingOah ();
#endif

    void                  initializeMfslOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMfslInterpreterOahHelp ();

    void                  printMfslInterpreterOahValues (
                            int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // user options
    Bool                  fDisplayTokens;

    Bool                  fDisplayToolAndInput;
    S_oahBooleanAtom      fDisplayToolAndInputAtom;

    Bool                  fDisplayOptions;

    Bool                  fNoLaunch;

    // maintainance options
    Bool                  fTraceScanning;

    Bool                  fTraceParsing;

    Bool                  fTraceChoices;
    Bool                  fTraceChoiceStatements;

    Bool                  fTraceCaseStatements;

    Bool                  fTraceOptionsBlocks;

    // input sources
    set<string>           fInputSourcesSet;
    S_oahStringSetElementAtom
                          fInputSourcesSetAtom;

    // select statement
    multimap<string, string>
                          fSelectChoiceToLabelsMultiMap;
    S_oahStringToStringMultiMapElementAtom
                          fSelectChoiceToLabelsMultiMapAtom;
};
typedef SMARTP<mfslInterpreterOahGroup> S_mfslInterpreterOahGroup;
EXP ostream& operator<< (ostream& os, const S_mfslInterpreterOahGroup& elt);

EXP extern S_mfslInterpreterOahGroup gGlobalMfslInterpreterOahGroup;

//______________________________________________________________________________
EXP S_mfslInterpreterOahGroup createGlobalMfslInterpreterOahGroup ();


}


#endif
