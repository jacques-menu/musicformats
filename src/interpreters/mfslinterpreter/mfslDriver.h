/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfslDriver___
#define ___mfslDriver___

#include <string>

#include "mfBool.h"
#include "mfMusicformatsErrors.h" // for mfMusicformatsErrorKind

#include "oahAtomsCollection.h"

#include "mfslBasicTypes.h"

#include "location.hh"

#include "mfslParser.h"

using namespace std;

using namespace MusicFormats;


//______________________________________________________________________________
// Conducting the whole scanning and parsing of MFSL
class mfslDriver
{
  public:

    // constants
    // ------------------------------------------------------

    static const std::string   K_ALL_PSEUDO_LABEL_NAME;

    // // constructor/destructor
    // ------------------------------------------------------

                          mfslDriver ();

    virtual               ~mfslDriver ();

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

    const mfsl::location& getScannerLocation () const
                              { return fScannerLocation; }

    mfsl::location&       getScannerLocationNonConst ()
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

    S_mfslChoicesTable    getChoicesTable () const
                              { return fChoicesTable; }

    void                  setCurrentChoiceChoice ( S_mfslChoice choice)
                              { fCurrentChoiceChoice = choice; }

    S_mfslChoice          getCurrentChoiceChoice () const
                              { return fCurrentChoiceChoice; }

    // inputs
    Bool                  getTraceInputs () const
                              { return fTraceInputs; }

    Bool                  getTraceCaseInputStatements () const
                              { return fTraceCaseInputStatements; }

    S_mfslInputsTable     getInputsTable () const
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
                            S_mfslOptionsBlock optionsBlock,
                            const std::string& context);

    S_mfslOptionsBlock    optionsBlocksStackTop () const;

    void                  optionsBlocksStackPop (
                            const std::string& context);

     void                 displayOptionsBlocksStack (
                            const std::string& context) const;

   // options
    void                  registerOptionInCurrentOptionsBlock (
                            S_oahOption option,
                            mfslDriver& drv);

    void                  registerOptionsSuppliedChoicesAsUsed (
                            const std::string& choiceName);
    void                  registerOptionsSuppliedChoicesAsUnused (
                            const std::string& choiceName);

    // case choice statements
    void                  caseChoiceStatementsStackPush (
                            S_mfslCaseChoiceStatement caseChoiceStatement);

    S_mfslCaseChoiceStatement
                          caseChoiceStatementsStackTop () const;

    void                  caseChoiceStatementsStackPop ();

    void                  displayCaseChoiceStatementsStack (
                            const std::string& context) const;

    // case input statements
    void                  caseInputStatementsStackPush (
                            S_mfslCaseInputStatement caseInputStatement);

    S_mfslCaseInputStatement
                          caseInputStatementsStackTop () const;

    void                  caseInputStatementsStackPop ();

    void                  displayCaseInputStatementsStack (
                            const std::string& context) const;

    // launching the MFSL service
    void                  handleSelectLabel (
                            const std::string& choiceName,
                            const std::string& label);

    void                  appendSelectLabelForServiceLaunching (
                            const S_mfslChoice choice,
                            const std::string& label,
                            Bool               allLabelSelected);

    // launch the service
    mfMusicformatsErrorKind   launchMfslService_Pass2 ();

  private:

    // private methods
    // ------------------------------------------------------

    // 'select' statements
//     Bool                  applySelectOptionIfPresent (
//                             const S_mfslChoice choice,
//                             const std::string&      label);

    Bool                  applySelectOptionsFinally ();

    Bool                  applySelectOption (
                            const S_mfslChoice choice,
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
    mfsl::location        fScannerLocation;

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
    S_mfslChoicesTable    fChoicesTable;

    std::multimap <std::string, std::string>
                          fOptionsSuppliedChoicesLabelsMultiMap;
    std::set <std::string> fUnusedOptionsSuppliedChoicesSet;

    // case choice statements
    int                   fCaseChoiceStatementsNumber;
    list<S_mfslCaseChoiceStatement>
                          fCaseChoiceStatementsStack;

    S_mfslChoice          fCurrentChoiceChoice;

    // inputs handling
    S_mfslInputsTable     fInputsTable;

    std::multimap <std::string, std::string>
                          fOptionsSuppliedInputsLabelsMultiMap;
    std::set <std::string> fUnusedOptionsSuppliedInputsSet;

    // case input statements
    int                   fCaseInputStatementsNumber;
    list<S_mfslCaseInputStatement>
                          fCaseInputStatementsStack;

    // options blocks
    list<S_mfslOptionsBlock>
                          fOptionsBlocksStack;

    S_mfslOptionsBlock    fMainOptionsBlock;

    // service launching
    list<S_mfslOptionsBlock>
                          fSelectedOptionsBlocksList;

    // commands list
    list<std::string>     fCommandsList;
};

//______________________________________________________________________________
// Give Flex the prototype of yylex() we want ...
# define YY_DECL \
  mfsl::parser::symbol_type yylex (mfslDriver& drv)
// ... and declare it for the parser's sake
YY_DECL;


#endif // ___mfslDriver___
