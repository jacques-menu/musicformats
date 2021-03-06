/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfslDriver___
#define ___mfslDriver___

#include <string>

#include "mfBool.h"
#include "mfMusicformatsError.h" // for mfMusicformatsError

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

    static const string   K_ALL_PSEUDO_LABEL_NAME;

    // // constructor/destructor
    // ------------------------------------------------------

                          mfslDriver ();

    virtual               ~mfslDriver ();

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

    const mfsl::location& getScannerLocation () const
                              { return fScannerLocation; }

    mfsl::location&       getScannerLocationNonConst ()
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

    S_mfslChoicesTable    getChoicesTable () const
                              { return fChoicesTable; }

    void                  setCurrentChoiceChoice ( S_mfslChoice choice)
                              { fCurrentChoiceChoice = choice; }

    S_mfslChoice          getCurrentChoiceChoice () const
                              { return fCurrentChoiceChoice; }

    // inputs
    bool				          getTraceInputs () const
                              { return fTraceInputs; }

    bool				          getTraceCaseInputStatements () const
                              { return fTraceCaseInputStatements; }

    S_mfslInputsTable     getInputsTable () const
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
                            S_mfslOptionsBlock optionsBlock,
                            const string&      context);

    S_mfslOptionsBlock    optionsBlocksStackTop () const;

    void                  optionsBlocksStackPop (
                            const string& context);

    // options
    void                  registerOptionInCurrentOptionsBlock (
                            S_oahOption option,
                            mfslDriver& drv);

    void                  registerOptionsSuppliedChoicesAsUsed (
                            const string& choiceName);
    void                  registerOptionsSuppliedChoicesAsUnused (
                            const string& choiceName);

    void                  displayOptionsBlocksStack (
                            const string& context) const;

    // case choice statements
    void                  caseChoiceStatementsStackPush (
                            S_mfslCaseChoiceStatement caseChoiceStatement);

    S_mfslCaseChoiceStatement
                          caseChoiceStatementsStackTop () const;

    void                  caseChoiceStatementsStackPop ();

    void                  displayCaseChoiceStatementsStack (
                            const string& context) const;

    // case input statements
    void                  caseInputStatementsStackPush (
                            S_mfslCaseInputStatement caseInputStatement);

    S_mfslCaseInputStatement
                          caseInputStatementsStackTop () const;

    void                  caseInputStatementsStackPop ();

    void                  displayCaseInputStatementsStack (
                            const string& context) const;

    // launching the MFSL tool
    void                  handleSelectLabel (
                            const string& choiceName,
                            const string& label);

    void                  appendSelectLabelForToolLaunching (
                            const S_mfslChoice choice,
                            const string&      label,
                            Bool               allLabelSelected);

    mfMusicformatsError   launchMfslTool_Pass2 ();

  private:

    // private methods
    // ------------------------------------------------------

    // 'select' statements
//     Bool                  applySelectOptionIfPresent (
//                             const S_mfslChoice choice,
//                             const string&      label);

    Bool                  applySelectOptionsFinally ();

    Bool                  applySelectOption (
                            const S_mfslChoice choice,
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
    mfsl::location        fScannerLocation;

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
    S_mfslChoicesTable    fChoicesTable;

    multimap<string, string>
                          fOptionsSuppliedChoicesLabelsMultiMap;
    set<string>           fUnusedOptionsSuppliedChoicesSet;

    // case choice statements
    int                   fCaseChoiceStatementsNumber;
    list<S_mfslCaseChoiceStatement>
                          fCaseChoiceStatementsStack;

    S_mfslChoice          fCurrentChoiceChoice;

    // inputs handling
    S_mfslInputsTable     fInputsTable;

    multimap<string, string>
                          fOptionsSuppliedInputsLabelsMultiMap;
    set<string>           fUnusedOptionsSuppliedInputsSet;

    // case input statements
    int                   fCaseInputStatementsNumber;
    list<S_mfslCaseInputStatement>
                          fCaseInputStatementsStack;

    // options blocks
    list<S_mfslOptionsBlock>
                          fOptionsBlocksStack;

    S_mfslOptionsBlock    fMainOptionsBlock;

    // tool launching
    list<S_mfslOptionsBlock>
                          fSelectedOptionsBlocksList;

    // commands list
    list<string>          fCommandsList;
};

//______________________________________________________________________________
// Give Flex the prototype of yylex() we want ...
# define YY_DECL \
  mfsl::parser::symbol_type yylex (mfslDriver& drv)
// ... and declare it for the parser's sake
YY_DECL;


#endif
