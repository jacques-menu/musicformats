/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

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

using namespace MusicFormats;


//______________________________________________________________________________
// Conducting the whole scanning and parsing of iScheme
class   ischemeDriver
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

    std::string           getScriptName () const
                              { return fScriptName; }

    void                  setTool (std::string tool);

    std::string           getTool () const
                              { return fTool; }

    void                  appendInputSouce (std::string inputSouce);

    const std::list<std::string>&   
                          getInputSoucesList () const
                              { return fInputSoucesList; }

    bool                  getTraceScanning () const
                              { return fTraceScanning; }

    const iscm::location& getScannerLocation () const
                              { return fScannerLocation; }

    iscm::location&       getScannerLocationNonConst ()
                            // no const here
                            // due to constraints in the Flex-generated code
                              { return fScannerLocation; }

    bool                  getDisplayToolAndInput () const
                              { return fDisplayToolAndInput; }

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

    void                  setCurrentChoiceChoice (S_ischemeChoice choice)
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
                            const std::string&      context);

    S_ischemeOptionsBlock         optionsBlocksStackTop () const;

    void                  optionsBlocksStackPop (
                            const std::string& context);

    // options
    void                  registerOptionInCurrentOptionsBlock (
                            const S_oahOption& option,
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

    // launching the iScheme tool
    void                  handleSelectLabel (
                            const std::string& choiceName,
                            const std::string& label);

    void                  appendSelectLabelForToolLaunching (
                            const S_ischemeChoice choice,
                            const std::string&      label,
                            Bool               allLabelSelected);

    mfMusicformatsErrorKind   launchIschemeTool_Pass2 ();

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
                            const std::string&      label);

    // final semantics check
    void                  finalSemanticsCheck ();

    // populate commands std::list
    void                  populateTheCommandsList ();

  private:

    // private fields
    // ------------------------------------------------------

    // the name of the MusicFormats tool
    std::string           fTool;

    // the name of the MusicFormats script
    std::string           fScriptName;

    // the names of the input sources
    std::list<std::string>          fInputSoucesList;

    // scanning
    bool                  fTraceScanning;
    iscm::location        fScannerLocation;

    // parsing
    bool                  fDisplayToolAndInput;

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

    // known tool names
    std::set<std::string>           fKnownNames;

    // choices handling
    S_ischemeChoicesTable    fChoicesTable;

    std::multimap<std::string, std::string>
                          fOptionsSuppliedChoicesLabelsMultiMap;
    std::set<std::string>           fUnusedOptionsSuppliedChoicesSet;

    // case choice statements
    int                   fCaseChoiceStatementsNumber;
    std::list<S_ischemeCaseChoiceStatement>
                          fCaseChoiceStatementsStack;

    S_ischemeChoice       fCurrentChoiceChoice;

    // inputs handling
    S_ischemeInputsTable  fInputsTable;

    std::multimap<std::string, std::string>
                          fOptionsSuppliedInputsLabelsMultiMap;
    std::set<std::string>           fUnusedOptionsSuppliedInputsSet;

    // case input statements
    int                   fCaseInputStatementsNumber;
    std::list<S_ischemeCaseInputStatement>
                          fCaseInputStatementsStack;

    // options blocks
    std::list<S_ischemeOptionsBlock>
                          fOptionsBlocksStack;

    S_ischemeOptionsBlock    fMainOptionsBlock;

    // tool launching
    std::list<S_ischemeOptionsBlock>
                          fSelectedOptionsBlocksList;

    // commands std::list
    std::list<std::string>          fCommandsList;
};

//______________________________________________________________________________
// Give Flex the prototype of yylex() we want ...
# define YY_DECL \
  iscm::parser::symbol_type yylex (ischemeDriver& drv)
// ... and declare it for the parser's sake
YY_DECL;


#endif
