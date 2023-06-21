/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___stringFilterInterpreterOah___
#define ___stringFilterInterpreterOah___

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
class EXP stringFilterInterpreterOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<stringFilterInterpreterOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializestringFilterInterpreterOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          stringFilterInterpreterOahGroup ();

    virtual               ~stringFilterInterpreterOahGroup ();

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

    const std::set<std::string>&
                          getInputSourcesSet () const
                              { return fInputSourcesSet; }

    S_oahStringSetElementAtom
                          getInputSourcesSetAtom () const
                              { return fInputSourcesSetAtom; }

    const std::multimap<std::string, std::string>&
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

#ifdef MF_TRACE_IS_ENABLED
// JMI    void                  initializestringFilterInterpretertracingOah ();
#endif // MF_TRACE_IS_ENABLED

    void                  initializestringFilterOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printstringFilterInterpreterOahHelp ();

    void                  printstringFilterInterpreterOahValues (
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
    std::set<std::string> fInputSourcesSet;
    S_oahStringSetElementAtom
                          fInputSourcesSetAtom;

    // select statement
    std::multimap<std::string, std::string>
                          fSelectChoiceToLabelsMultiMap;
    S_oahStringToStringMultiMapElementAtom
                          fSelectChoiceToLabelsMultiMapAtom;
};
typedef SMARTP<stringFilterInterpreterOahGroup> S_stringFilterInterpreterOahGroup;
EXP std::ostream& operator<< (std::ostream& os, const S_stringFilterInterpreterOahGroup& elt);

EXP extern S_stringFilterInterpreterOahGroup gGlobalstringFilterInterpreterOahGroup;

//______________________________________________________________________________
EXP S_stringFilterInterpreterOahGroup createGlobalstringFilterInterpreterOahGroup ();


}


#endif // ___stringFilterInterpreterOah___
