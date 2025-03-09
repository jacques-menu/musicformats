/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___ischemeDriver___
#define ___ischemeDriver___

#include <string>

#include "mfBool.h"
#include "mfMusicformatsErrors.h" // for mfMusicformatsErrorKind

#include "oahAtomsCollection.h"

#include "ischemeBasicTypes.h"

#include "location.hh"

#include "ischemeParser.h"

using namespace std;

using namespace MusicFormats;


//______________________________________________________________________________
// Conducting the whole scanning and parsing of iScheme
class ischemeDriver
{
  public:

    // constants
    // ------------------------------------------------------

    static const std::string   K_ALL_PSEUDO_LABEL_NAME;

    // // constructor/destructor
    // ------------------------------------------------------

                          ischemeDriver ();

    virtual               ~ischemeDriver ();

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

    const stringMatcher::location& getScannerLocation () const
                              { return fScannerLocation; }

    stringMatcher::location&       getScannerLocationNonConst ()
                            // no const here
                            // due to constraints in the Flex-generated code
                              { return fScannerLocation; }

    bool                  getDisplayServiceAndInput () const
                              { return fDisplayServiceAndInput; }

    bool                  getTraceParsing () const
                              { return fTraceParsing; }

    bool                  getDisplayOptions () const
                              { return fDisplayOptions; }

    // choices
    bool                  getTraceChoices () const
                              { return fTraceChoices; }

    bool                  getTraceCaseChoiceStatements () const
                              { return fTraceCaseChoiceStatements; }

    S_ischemeChoicesTable getChoicesTable () const
                              { return fChoicesTable; }

    void                  setCurrentChoiceChoice ( S_ischemeChoice choice)
                              { fCurrentChoiceChoice = choice; }

    S_ischemeChoice       getCurrentChoiceChoice () const
                              { return fCurrentChoiceChoice; }

    // inputs
    bool                  getTraceInputs () const
                              { return fTraceInputs; }

    bool                  getTraceCaseInputStatements () const
                              { return fTraceCaseInputStatements; }

    S_ischemeInputsTable  getInputsTable () const
                              { return fInputsTable; }

    void                  setTraceOptionsBlocks () // TEMP JMI
                              { fTraceOptionsBlocks = true; }
    bool                  getTraceOptionsBlocks () const
                              { return fTraceOptionsBlocks; }

    bool                  getDisplayTokens () const
                              { return fDisplayTokens; }

    bool                  getNoLaunch () const
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
                            S_ischemeOptionsBlock optionsBlock,
                            const std::string&    context);

    S_ischemeOptionsBlock optionsBlocksStackTop () const;

    void                  optionsBlocksStackPop (
                            const std::string& context);

    // options
    void                  registerOptionInCurrentOptionsBlock (
                            S_oahOption    option,
                            ischemeDriver& drv);

    void                  registerOptionsSuppliedChoicesAsUsed (
                            const std::string& choiceName);
    void                  registerOptionsSuppliedChoicesAsUnused (
                            const std::string& choiceName);

    void                  displayOptionsBlocksStack (
                            const std::string& context) const;

    // case choice statements
    void                  caseChoiceStatementsStackPush (
                            S_ischemeCaseChoiceStatement caseChoiceStatement);

    S_ischemeCaseChoiceStatement
                          caseChoiceStatementsStackTop () const;

    void                  caseChoiceStatementsStackPop ();

    void                  displayCaseChoiceStatementsStack (
                            const std::string& context) const;

    // case input statements
    void                  caseInputStatementsStackPush (
                            S_ischemeCaseInputStatement caseInputStatement);

    S_ischemeCaseInputStatement
                          caseInputStatementsStackTop () const;

    void                  caseInputStatementsStackPop ();

    void                  displayCaseInputStatementsStack (
                            const std::string& context) const;

    // launching the iScheme service
    void                  handleSelectLabel (
                            const std::string& choiceName,
                            const std::string& label);

    void                  appendSelectLabelForServiceLaunching (
                            const S_ischemeChoice choice,
                            const std::string&    label,
                            Bool                  allLabelSelected);

    mfMusicformatsErrorKind
                          launchIschemeService_Pass2 ();

  private:

    // private methods
    // ------------------------------------------------------

    // 'select' statements
//     Bool                  applySelectOptionIfPresent (
//                             const S_ischemeChoice choice,
//                             const std::string&      label);

    Bool                  applySelectOptionsFinally ();

    Bool                  applySelectOption (
                            const S_ischemeChoice choice,
                            const std::string&    label);

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
    bool                  fTraceScanning;
    stringMatcher::location        fScannerLocation;

    // parsing
    bool                  fDisplayServiceAndInput;

    bool                  fTraceParsing;

    bool                  fDisplayOptions;

    // tokens
    bool                  fDisplayTokens;

    // choices
    bool                  fTraceChoices;

    bool                  fTraceCaseChoiceStatements;

    // inputs
    bool                  fTraceInputs;

    bool                  fTraceCaseInputStatements;

    // options
    bool                  fTraceOptionsBlocks;

    // launching
    bool                  fNoLaunch;

    // known service names
    std::set <std::string> fKnownNames;

    // choices handling
    S_ischemeChoicesTable fChoicesTable;

    std::multimap <std::string, std::string>
                          fOptionsSuppliedChoicesLabelsMultiMap;
    std::set <std::string> fUnusedOptionsSuppliedChoicesSet;

    // case choice statements
    int                   fCaseChoiceStatementsNumber;
    list<S_ischemeCaseChoiceStatement>
                          fCaseChoiceStatementsStack;

    S_ischemeChoice       fCurrentChoiceChoice;

    // inputs handling
    S_ischemeInputsTable  fInputsTable;

    std::multimap <std::string, std::string>
                          fOptionsSuppliedInputsLabelsMultiMap;
    std::set <std::string> fUnusedOptionsSuppliedInputsSet;

    // case input statements
    int                   fCaseInputStatementsNumber;
    list<S_ischemeCaseInputStatement>
                          fCaseInputStatementsStack;

    // options blocks
    list<S_ischemeOptionsBlock>
                          fOptionsBlocksStack;

    S_ischemeOptionsBlock fMainOptionsBlock;

    // service launching
    list<S_ischemeOptionsBlock>
                          fSelectedOptionsBlocksList;

    // commands list
    list<std::string>     fCommandsList;
};

//______________________________________________________________________________
// Give Flex the prototype of yylex() we want ...
# define YY_DECL \
  stringMatcher::parser::symbol_type yylex (ischemeDriver& drv)
// ... and declare it for the parser's sake
YY_DECL;


#endif // ___ischemeDriver___
