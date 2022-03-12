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
#include <map>

//#include "mfBool.h"

#include "mfslParser.h"

using namespace std;


//______________________________________________________________________________
// Give Flex the prototype of yylex we want ...
# define YY_DECL \
  yy::parser::symbol_type yylex (mfslDriver& drv)
// ... and declare it for the parser's sake.
YY_DECL;

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
                              bool displayNonTerminals);

    virtual               ~mfslDriver ();

  public:

    // set and get
    // ------------------------------------------------------

    void  			          setInputFileName (string value)
                              { fInputFileName = value; }

    string			          getInputFileName ()
                              { return fInputFileName; }

    yy::location&         getScannerLocation ()
                              { return fScannerLocation; }

    bool									getDisplayTokens ()
                              { return fDisplayTokens; }

    bool				          getDisplayNonTerminals ()
                              { return fDisplayNonTerminals; }

    void  			          setToolName (string value)
                              { fToolName = value; }

    string			          getToolName ()
                              { return fToolName; }

  public:

    // public services
    // ------------------------------------------------------

    // run the parser on file inputFileName,
    // return 0 on success

    int                   parseFile (const string& inputFileName);

    // handling the scanner

    void                  scanBegin ();
    void                  scanEnd ();

  protected:

    // public fields
    // ------------------------------------------------------

    // The name of the file being parsed.
    string                fInputFileName;

    // scanning
    bool                  fTraceScanning;
    yy::location          fScannerLocation;

    // parsing
    bool                  fTraceParsing;

    // display
    bool                  fDisplayTokens;
    bool                  fDisplayNonTerminals;

    // variables handling
    map<string, int>      fScannerVariables;

		string                fToolName;

    int                   fDriverResult;
};


#endif
