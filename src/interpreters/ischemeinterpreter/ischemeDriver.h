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
#include "mfMusicformatsError.h" // for mfMusicformatsError

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

    static const string   K_ALL_PSEUDO_LABEL_NAME;

    // // constructor/destructor
    // ------------------------------------------------------

                          ischemeDriver ();

    virtual               ~ischemeDriver ();

  public:

    // set and get
    // ------------------------------------------------------

    // internal
    void  			          setScriptName (string scriptName);

    string			          getScriptName () const
                              { return fScriptName; }

    void  			          setTool (string tool);

    string			          getTool () const
                              { return fTool; }

    void  			          appendInputSouce (string inputSouce);

    const list<string>&   getInputSoucesList () const
                              { return fInputSoucesList; }

    bool				          getTraceScanning () const
                              { return fTraceScanning; }

    const iscm::location& getScannerLocation () const
                              { return fScannerLocation; }

    iscm::location&       getScannerLocationNonConst ()
                            // no const here
                            // due to constraints in the Flex-generated code
                              { return fScannerLocation; }

    bool				          getDisplayToolAndInput () const
                              { return fDisplayToolAndInput; }

    bool				          getTraceParsing () const
                              { return fTraceParsing; }

    bool				          getDisplayOptions () const
                              { return fDisplayOptions; }

    // choices
    bool				          getTraceChoices () const
                              { return fTraceChoices; }

    bool				          getTraceCaseChoiceStatements () const
                              { return fTraceCaseChoiceStatements; }

    S_ischemeChoicesTable getChoicesTable () const
                              { return fChoicesTable; }

    void                  setCurrentChoiceChoice ( S_ischemeChoice choice)
                              { fCurrentChoiceChoice = choice; }

    S_ischemeChoice       getCurrentChoiceChoice () const
                              { return fCurrentChoiceChoice; }

    // inputs
    bool				          getTraceInputs () const
                              { return fTraceInputs; }

    bool				          getTraceCaseInputStatements () const
                              { return fTraceCaseInputStatements; }

    S_ischemeInputsTable  getInputsTable () const
                              { return fInputsTable; }

    void				          setTraceOptionsBlocks () // TEMP JMI
                              { fTraceOptionsBlocks = true; }
    bool				          getTraceOptionsBlocks () const
                              { return fTraceOptionsBlocks; }

    bool									getDisplayTokens () const
                              { return fDisplayTokens; }

    bool				          getNoLaunch () const
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
                            const string&      context);

    S_ischemeOptionsBlock         optionsBlocksStackTop () const;

    void                  optionsBlocksStackPop (
                            const string& context);

    // options
    void                  registerOptionInCurrentOptionsBlock (
                            S_oahOption option,
                            ischemeDriver& drv);

    void                  registerOptionsSuppliedChoicesAsUsed (
                            const string& choiceName);
    void                  registerOptionsSuppliedChoicesAsUnused (
                            const string& choiceName);

    void                  displayOptionsBlocksStack (
                            const string& context) const;

    // case choice statements
    void                  caseChoiceStatementsStackPush (
                            S_ischemeCaseChoiceStatement caseChoiceStatement);

    S_ischemeCaseChoiceStatement
                          caseChoiceStatementsStackTop () const;

    void                  caseChoiceStatementsStackPop ();

    void                  displayCaseChoiceStatementsStack (
                            const string& context) const;

    // case input statements
    void                  caseInputStatementsStackPush (
                            S_ischemeCaseInputStatement caseInputStatement);

    S_ischemeCaseInputStatement
                          caseInputStatementsStackTop () const;

    void                  caseInputStatementsStackPop ();

    void                  displayCaseInputStatementsStack (
                            const string& context) const;

    // launching the iScheme tool
    void                  handleSelectLabel (
                            const string& choiceName,
                            const string& label);

    void                  appendSelectLabelForToolLaunching (
                            const S_ischemeChoice choice,
                            const string&      label,
                            Bool               allLabelSelected);

    mfMusicformatsError   launchIschemeTool_Pass2 ();

  private:

    // private methods
    // ------------------------------------------------------

    // 'select' statements
//     Bool                  applySelectOptionIfPresent (
//                             const S_ischemeChoice choice,
//                             const string&      label);

    Bool                  applySelectOptionsFinally ();

    Bool                  applySelectOption (
                            const S_ischemeChoice choice,
                            const string&      label);

    // final semantics check
    void                  finalSemanticsCheck ();

    // populate commands list
    void                  populateTheCommandsList ();

  private:

    // private fields
    // ------------------------------------------------------

    // the name of the MusicFormats tool
		string                fTool;

    // the name of the MusicFormats script
		string                fScriptName;

    // the names of the input sources
    list<string>          fInputSoucesList;

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
    set<string>           fKnownNames;

    // choices handling
    S_ischemeChoicesTable    fChoicesTable;

    multimap<string, string>
                          fOptionsSuppliedChoicesLabelsMultiMap;
    set<string>           fUnusedOptionsSuppliedChoicesSet;

    // case choice statements
    int                   fCaseChoiceStatementsNumber;
    list<S_ischemeCaseChoiceStatement>
                          fCaseChoiceStatementsStack;

    S_ischemeChoice       fCurrentChoiceChoice;

    // inputs handling
    S_ischemeInputsTable  fInputsTable;

    multimap<string, string>
                          fOptionsSuppliedInputsLabelsMultiMap;
    set<string>           fUnusedOptionsSuppliedInputsSet;

    // case input statements
    int                   fCaseInputStatementsNumber;
    list<S_ischemeCaseInputStatement>
                          fCaseInputStatementsStack;

    // options blocks
    list<S_ischemeOptionsBlock>
                          fOptionsBlocksStack;

    S_ischemeOptionsBlock    fMainOptionsBlock;

    // tool launching
    list<S_ischemeOptionsBlock>
                          fSelectedOptionsBlocksList;

    // commands list
    list<string>          fCommandsList;
};

//______________________________________________________________________________
// Give Flex the prototype of yylex() we want ...
# define YY_DECL \
  iscm::parser::symbol_type yylex (ischemeDriver& drv)
// ... and declare it for the parser's sake
YY_DECL;


#endif
