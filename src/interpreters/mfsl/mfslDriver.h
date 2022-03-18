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

                          mfslDriver (
                            bool traceScanning,
                            bool tTraceParsing,
                            bool displayTokens,
                            bool displayNonTerminals,
                            bool traceSemantics);

    virtual               ~mfslDriver ();

  public:

    // set and get
    // ------------------------------------------------------

    // internal
    void  			          setToolName (string value);

    string			          getToolName ()
                              { return fToolName; }

    void  			          setInputFileName (string value);

    string			          getInputFileName ()
                              { return fInputFileName; }

    yy::location&         getScannerLocation ()
                              { return fScannerLocation; }

    bool									getDisplayTokens ()
                              { return fDisplayTokens; }

    bool				          getDisplayNonTerminals ()
                              { return fDisplayNonTerminals; }

    bool				          getTraceSemantics ()
                              { return fTraceSemantics; }

    S_mfslVariablesTable  getVariablesTable () const
                              { return fVariablesTable; }

  public:

    // public services
    // ------------------------------------------------------

    // run the parser on file inputFileName,
    // return 0 on success

    int                   parseFile (const string& inputFileName);

    // handling the scanner

    void                  scanBegin ();
    void                  scanEnd ();

    // scopes
    void                  pushScopeOntoStack (
                            S_mfslScope scope);

    S_mfslScope
                          topOfScopesStack () const
                              { return fScopesStack.front (); }

    void                  popScopeFromStack ();

    // options
    void                  registerOptionInCurrentScope (
                            S_oahOption option);

    // case statements
    void                  pushCaseStatementOntoStack (
                            S_mfslCaseStatement caseStatement);

    S_mfslCaseStatement
                          topOfCaseStatementStack () const
                              { return fCaseStatementsStack.front (); }

    void                  popCaseStatementFromStack ();

    // all statements
    void                  appendAllStatementToList (
                            S_mfslAllStatement allStatement);

//     S_mfslAllStatement
//                           topOfAllStatementsList () const
//                               { return fAllStatementsList.front (); }
//
//     void                  popAllStatementFromList ();

   // launching the MFSL tool
    mfMusicformatsError   launchMfslTool ();

  private:

    // private fields
    // ------------------------------------------------------

    // the name of the MusicFormsts tool
		string                fToolName;

    // the name of the file being parsed
    string                fInputFileName;

    // scanning
    bool                  fTraceScanning;
    yy::location          fScannerLocation;

    // parsing
    bool                  fTraceParsing;

    // display
    bool                  fDisplayTokens;
    bool                  fDisplayNonTerminals;

    bool                  fTraceSemantics;

    // known tool names
    set<string>           fKnownToolNames;

    // variables handling
    S_mfslVariablesTable  fVariablesTable;

    // scopes
    list<S_mfslScope>     fScopesStack;

    // case statements
    list<S_mfslCaseStatement>
                          fCaseStatementsStack;

    // all statements
    list<S_mfslAllStatement>
                          fAllStatementsList;
};

//______________________________________________________________________________
// Give Flex the prototype of yylex we want ...
# define YY_DECL \
  yy::parser::symbol_type yylex (mfslDriver& drv)
// ... and declare it for the parser's sake
YY_DECL;


#endif
