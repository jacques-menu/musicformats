/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___stringMatcherInterpreterOah___
#define ___stringMatcherInterpreterOah___

#include "msrGenerationBasicTypes.h"

#include "brailleGenerationOah.h"
#include "brailleGeneration.h"


namespace MusicFormats
{

//______________________________________________________________________________
#define MSDR_STANDARD_INPUT_NAME ""
//#define MSDR_STANDARD_INPUT_NAME "-"

//______________________________________________________________________________
class EXP stringMatcherInterpreterOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<stringMatcherInterpreterOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializestringMatcherInterpreterOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          stringMatcherInterpreterOahGroup ();

    virtual               ~stringMatcherInterpreterOahGroup ();

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

    // display
    void                  setDisplayServiceAndInput ()
                              { fDisplayServiceAndInput = true; }

    Bool                  getDisplayServiceAndInput () const
                              { return fDisplayServiceAndInput; }

    void                  setDisplayOptions ()
                              { fDisplayOptions = true; }

    Bool                  getDisplayOptions () const
                              { return fDisplayOptions; }

    // inputs
    void                  setTraceInputs ()
                              { fTraceInputs = true; }

    Bool                  getTraceInputs () const
                              { return fTraceInputs; }

    void                  setTraceCaseInputStatements ()
                              { fTraceCaseInputStatements = true; }

    Bool                  getTraceCaseInputStatements () const
                              { return fTraceCaseInputStatements; }

    // choices
    void                  setTraceChoices ()
                              { fTraceChoices = true; }

    Bool                  getTraceChoices () const
                              { return fTraceChoices; }

    void                  setTraceCaseChoiceStatements ()
                              { fTraceCaseChoiceStatements = true; }

    Bool                  getTraceCaseChoiceStatements () const
                              { return fTraceCaseChoiceStatements; }

    void                  setTraceOptionsBlocks ()
                              { fTraceOptionsBlocks = true; }

    // options
    Bool                  getTraceOptionsBlocks () const
                              { return fTraceOptionsBlocks; }

    void                  setNoLaunch ()
                              { fNoLaunch = true; }

    Bool                  getNoLaunch () const
                              { return fNoLaunch; }

    const std::set <std::string>&
                          getInputSourcesSet () const
                              { return fInputSourcesSet; }

    S_oahStringSetAtom
                          getInputSourcesSetAtom () const
                              { return fInputSourcesSetAtom; }

    const std::multimap <std::string, std::string>&
                          getSelectChoiceToLabelsMultiMap () const
                              { return fSelectChoiceToLabelsMultiMap; }

    S_oahStringToStringMultiMapAtom
                          getSelectChoiceToLabelsMultiMapAtom () const
                              { return fSelectChoiceToLabelsMultiMapAtom; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef MF_TRACE_IS_ENABLED
// JMI    void                  initializestringMatcherInterpretertracingOah ();
#endif // MF_TRACE_IS_ENABLED

    void                  initializestringMatcherOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printstringMatcherInterpreterOahHelp ();

    void                  printstringMatcherInterpreterOahValues (
                            int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // user options
    Bool                  fDisplayTokens;

    Bool                  fDisplayServiceAndInput;
    S_oahBooleanAtom      fDisplayServiceAndInputAtom;

    Bool                  fDisplayOptions;

    // nolaunch
    Bool                  fNoLaunch;

    // maintainance options
    Bool                  fTraceScanning;

    Bool                  fTraceParsing;

    // inputs
    Bool                  fTraceInputs;
    Bool                  fTraceCaseInputStatements;

    // choices
    Bool                  fTraceChoices;
    Bool                  fTraceCaseChoiceStatements;

    // options blocks
    Bool                  fTraceOptionsBlocks;

    // input sources
    std::set <std::string> fInputSourcesSet;
    S_oahStringSetAtom
                          fInputSourcesSetAtom;

    // select statement
    std::multimap <std::string, std::string>
                          fSelectChoiceToLabelsMultiMap;
    S_oahStringToStringMultiMapAtom
                          fSelectChoiceToLabelsMultiMapAtom;
};
typedef SMARTP<stringMatcherInterpreterOahGroup> S_stringMatcherInterpreterOahGroup;
EXP std::ostream& operator<< (std::ostream& os, const S_stringMatcherInterpreterOahGroup& elt);

EXP extern S_stringMatcherInterpreterOahGroup gGlobalstringMatcherInterpreterOahGroup;

//______________________________________________________________________________
EXP S_stringMatcherInterpreterOahGroup createGlobalstringMatcherInterpreterOahGroup ();


}


#endif // ___stringMatcherInterpreterOah___
