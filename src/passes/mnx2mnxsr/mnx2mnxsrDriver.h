/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mnx2mnxsrDriver___
#define ___mnx2mnxsrDriver___

#include <string>

#include "mfBool.h"
#include "mfMusicformatsErrors.h" // for mfMusicformatsErrorKind

#include "oahAtomsCollection.h"

#include "mnx2mnxsrBasicTypes.h"

#include "location.hh"

#include "mnx2mnxsrParser.h"

using namespace std;

using namespace MusicFormats;


//______________________________________________________________________________
// Conducting the whole scanning and parsing of MNX2MNXSR
class mnx2mnxsrDriver
{
  public:

    // constants
    // ------------------------------------------------------

    static const std::string   K_ALL_PSEUDO_LABEL_NAME;

    // // constructor/destructor
    // ------------------------------------------------------

                          mnx2mnxsrDriver ();

    virtual               ~mnx2mnxsrDriver ();

  public:

    // set and get
    // ------------------------------------------------------

    // internal
    void                  setScriptName (std::string scriptName);

    std::string            getScriptName () const
                              { return fScriptName; }

    void                  setService (std::string service);

    std::string            getService () const
                              { return fService; }

    void                  appendInputSouce (std::string inputSouce);

    const list<std::string>&
                          getInputSoucesList () const
                              { return fInputSoucesList; }

    bool                  getTraceScanning () const
                              { return fTraceScanning; }

    const mnx2mnxsr::location& getScannerLocation () const
                              { return fScannerLocation; }

    mnx2mnxsr::location&       getScannerLocationNonConst ()
                            // no const here
                            // due to constraints in the Flex-generated code
                              { return fScannerLocation; }

    Bool                  getDisplayServiceAndInput () const
                              { return fDisplayServiceAndInput; }

    bool                  getTraceParsing () const
                              { return fTraceParsing; }

    Bool                  getDisplayOptions () const
                              { return fDisplayOptions; }

    // choices
    Bool                  getTraceChoices () const
                              { return fTraceChoices; }

    Bool                  getTraceCaseChoiceStatements () const
                              { return fTraceCaseChoiceStatements; }

    S_mnx2mnxsrChoicesTable    getChoicesTable () const
                              { return fChoicesTable; }

    void                  setCurrentChoiceChoice ( S_mnx2mnxsrChoice choice)
                              { fCurrentChoiceChoice = choice; }

    S_mnx2mnxsrChoice          getCurrentChoiceChoice () const
                              { return fCurrentChoiceChoice; }

    // inputs
    Bool                  getTraceInputs () const
                              { return fTraceInputs; }

    Bool                  getTraceCaseInputStatements () const
                              { return fTraceCaseInputStatements; }

    S_mnx2mnxsrInputsTable     getInputsTable () const
                              { return fInputsTable; }

    void                  setTraceOptionsBlocks () // TEMP JMI
                              { fTraceOptionsBlocks = true; }
    Bool                  getTraceOptionsBlocks () const
                              { return fTraceOptionsBlocks; }

    Bool                  getDisplayTokens () const
                              { return fDisplayTokens; }

    Bool                  getNoLaunch () const
                              { return fNoLaunch; }

  public:

    // public services
    // ------------------------------------------------------

    // run the parser, return 0 on success
    int                   parseInput_Pass1 ();

    // handling the scanner
    void                  scanBegin ();
    void                  scanEnd ();

    // options blocks
    void                  optionsBlocksStackPush (
                            S_mnx2mnxsrOptionsBlock optionsBlock,
                            const std::string& context);

    S_mnx2mnxsrOptionsBlock    optionsBlocksStackTop () const;

    void                  optionsBlocksStackPop (
                            const std::string& context);

     void                 displayOptionsBlocksStack (
                            const std::string& context) const;

   // options
    void                  registerOptionInCurrentOptionsBlock (
                            S_oahOption option,
                            mnx2mnxsrDriver& drv);

    void                  registerOptionsSuppliedChoicesAsUsed (
                            const std::string& choiceName);
    void                  registerOptionsSuppliedChoicesAsUnused (
                            const std::string& choiceName);

    // case choice statements
    void                  caseChoiceStatementsStackPush (
                            S_mnx2mnxsrCaseChoiceStatement caseChoiceStatement);

    S_mnx2mnxsrCaseChoiceStatement
                          caseChoiceStatementsStackTop () const;

    void                  caseChoiceStatementsStackPop ();

    void                  displayCaseChoiceStatementsStack (
                            const std::string& context) const;

    // case input statements
    void                  caseInputStatementsStackPush (
                            S_mnx2mnxsrCaseInputStatement caseInputStatement);

    S_mnx2mnxsrCaseInputStatement
                          caseInputStatementsStackTop () const;

    void                  caseInputStatementsStackPop ();

    void                  displayCaseInputStatementsStack (
                            const std::string& context) const;

    // launching the MNX2MNXSR service
    void                  handleSelectLabel (
                            const std::string& choiceName,
                            const std::string& label);

    void                  appendSelectLabelForServiceLaunching (
                            const S_mnx2mnxsrChoice choice,
                            const std::string& label,
                            Bool               allLabelSelected);

    // launch the service
    mfMusicformatsErrorKind   launchMnx2mnxsrService_Pass2 ();

  private:

    // private methods
    // ------------------------------------------------------

    // 'select' statements
//     Bool                  applySelectOptionIfPresent (
//                             const S_mnx2mnxsrChoice choice,
//                             const std::string&      label);

    Bool                  applySelectOptionsFinally ();

    Bool                  applySelectOption (
                            const S_mnx2mnxsrChoice choice,
                            const std::string& label);

    // final semantics check
    void                  finalSemanticsCheck ();

    // populate commands list
    void                  populateTheCommandsList ();

  private:

    // private fields
    // ------------------------------------------------------

    // the name of the MusicFormats service
    std::string           fService;

    // the name of the MusicFormats script
    std::string           fScriptName;

    // the names of the input sources
    list<std::string>     fInputSoucesList;

    // scanning
    bool                  fTraceScanning; // this interacts with the scanner
    mnx2mnxsr::location        fScannerLocation;

    // parsing
    Bool                  fDisplayServiceAndInput;

    bool                  fTraceParsing; // this interacts with the scanner

    Bool                  fDisplayOptions;

    // tokens
    Bool                  fDisplayTokens;

    // choices
    Bool                  fTraceChoices;

    Bool                  fTraceCaseChoiceStatements;

    // inputs
    Bool                  fTraceInputs;

    Bool                  fTraceCaseInputStatements;

    // options
    Bool                  fTraceOptionsBlocks;

    // launching
    Bool                  fNoLaunch;

    // known service names
    std::set <std::string> fKnownNames;

    // choices handling
    S_mnx2mnxsrChoicesTable    fChoicesTable;

    std::multimap <std::string, std::string>
                          fOptionsSuppliedChoicesLabelsMultiMap;
    std::set <std::string> fUnusedOptionsSuppliedChoicesSet;

    // case choice statements
    int                   fCaseChoiceStatementsNumber;
    list<S_mnx2mnxsrCaseChoiceStatement>
                          fCaseChoiceStatementsStack;

    S_mnx2mnxsrChoice          fCurrentChoiceChoice;

    // inputs handling
    S_mnx2mnxsrInputsTable     fInputsTable;

    std::multimap <std::string, std::string>
                          fOptionsSuppliedInputsLabelsMultiMap;
    std::set <std::string> fUnusedOptionsSuppliedInputsSet;

    // case input statements
    int                   fCaseInputStatementsNumber;
    list<S_mnx2mnxsrCaseInputStatement>
                          fCaseInputStatementsStack;

    // options blocks
    list<S_mnx2mnxsrOptionsBlock>
                          fOptionsBlocksStack;

    S_mnx2mnxsrOptionsBlock    fMainOptionsBlock;

    // service launching
    list<S_mnx2mnxsrOptionsBlock>
                          fSelectedOptionsBlocksList;

    // commands list
    list<std::string>     fCommandsList;
};

//______________________________________________________________________________
// Give Flex the prototype of yylex() we want ...
# define YY_DECL \
  mnx2mnxsr::parser::symbol_type yylex (mnx2mnxsrDriver& drv)
// ... and declare it for the parser's sake
YY_DECL;


#endif // ___mnx2mnxsrDriver___
