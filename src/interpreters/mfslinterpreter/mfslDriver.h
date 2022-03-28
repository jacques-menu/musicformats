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

    // // constructor/destructor
    // ------------------------------------------------------

                          mfslDriver ();

    virtual               ~mfslDriver ();

  public:

    // set and get
    // ------------------------------------------------------

    // internal
    void  			          setScriptName (string scriptName);

    string			          getScriptSourceName () const
                              { return fScriptSourceName; }

    void  			          setToolName (string toolName);

    string			          getToolName () const
                              { return fToolName; }

    void  			          setInputSouceName (string inputSouceName);

    string			          getInputSouceName () const
                              { return fInputSouceName; }

    bool				          getTraceScanning () const
                              { return fTraceScanning; }

    const yy::location&   getScannerLocation () const
                            // no const here
                            // due to Flex-generated code constraints
                              { return fScannerLocation; }
    yy::location&         getScannerLocationToModify ()
                            // no const here
                            // due to Flex-generated code constraints
                              { return fScannerLocation; }

    bool				          getDisplayToolAndInput () const
                              { return fDisplayToolAndInput; }

    bool				          getTraceParsing () const
                              { return fTraceParsing; }

    bool				          getDisplayOptions () const
                              { return fDisplayOptions; }

    bool				          getTraceChoices () const
                              { return fTraceChoices; }

    bool				          getTraceChoiceStatements () const
                              { return fTraceChoiceStatements; }

    bool				          getTraceCaseStatements () const
                              { return fTraceCaseStatements; }

    void				          setTraceOptionsBlocks () // TEMP JMI
                              { fTraceOptionsBlocks = true; }
    bool				          getTraceOptionsBlocks () const
                              { return fTraceOptionsBlocks; }

    bool									getDisplayTokens () const
                              { return fDisplayTokens; }

    bool				          getNoLaunch () const
                              { return fNoLaunch; }

    S_mfslChoicesTable    getChoicesTable () const
                              { return fChoicesTable; }

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
                            const string& context);

    S_mfslOptionsBlock         optionsBlocksStackTop () const;

    void                  optionsBlocksStackPop (
                            const string& context);

    // options
    void                  registerOptionInCurrentOptionsBlock (
                            S_oahOption option);

    void                  registerOptionsSuppliedChoicesAsUsed (
                            const string& choiceName)
                              {
                                fUnusedOptionsSuppliedChoicesSet.erase (
                                  choiceName);
                              }

    void                  displayOptionsBlocksStack (
                            const string& context) const;

    // case statements
    void                  caseStatementsStackPush (
                            S_mfslCaseStatement caseStatement);

    S_mfslCaseStatement   caseStatementsStackTop () const;

    void                  caseStatementsStackPop ();

    void                  displayCaseStatementsStack (
                            const string& context) const;

    // launching the MFSL tool
    void                  setSelectLabelForToolLaunching (
                            const string& choiceName,
                            const string& label);

    void                  setAllChoicesOptionsBlockForToolLaunching (
                            const string& choiceName);

    mfMusicformatsError   launchMfslTool_Pass2 ();

  private:

    // private methods
    // ------------------------------------------------------

    void                  populateTheCommandsList ();

  private:

    // private fields
    // ------------------------------------------------------

    // the name of the MusicFormats script source
		string                fScriptSourceName;

    // the name of the MusicFormats tool
		string                fToolName;

    // the name of the input being parsed
    string                fInputSouceName;

    // scanning
    bool                  fTraceScanning;
    yy::location          fScannerLocation;

    // parsing
    bool                  fDisplayToolAndInput;

    bool                  fTraceParsing;

    bool                  fDisplayOptions;

    bool                  fTraceChoices;

    bool                  fTraceChoiceStatements;
    bool                  fTraceCaseStatements;

    bool                  fTraceOptionsBlocks;

    bool                  fDisplayTokens;

    // launching
    bool                  fNoLaunch;

    // known tool names
    set<string>           fKnownNames;

    // choices handling
    S_mfslChoicesTable    fChoicesTable;

    map<string, string>   fOptionsSuppliedChoicesLabelsMap;
    set<string>           fUnusedOptionsSuppliedChoicesSet;

    // case statements
    int                   fCaseStatementsNumber;
    list<S_mfslCaseStatement>
                          fCaseStatementsStack;

    // options blocks
    list<S_mfslOptionsBlock>
                          fOptionsBlocksStack;

    S_mfslOptionsBlock    fMainOptionsBlock;

    // tool launching
    S_mfslOptionsBlock    fOptionsBlockToUseForSelectLaunching;

    S_mfslChoice          fChoiceToUseForAllLaunching;

    list<string>          fCommandsList;
};

//______________________________________________________________________________
// Give Flex the prototype of yylex we want ...
# define YY_DECL \
  yy::parser::symbol_type yylex (mfslDriver& drv)
// ... and declare it for the parser's sake
YY_DECL;


#endif
