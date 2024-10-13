/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___stringMatcherDriver___
#define ___stringMatcherDriver___

#include <string>

#include "mfBool.h"
#include "mfMusicformatsErrors.h" // for mfMusicformatsErrorKind

#include "stringMatcherBasicTypes.h"

#include "location.hh"

#include "stringMatcherParser.h"


using namespace std;

using namespace MusicFormats;

//______________________________________________________________________________
// Conducting the whole scanning and parsing of stringMatcher
class stringMatcherDriver : public smartable
{
  public:

    // // constructor/destructor
    // ------------------------------------------------------

                          stringMatcherDriver ();

    virtual               ~stringMatcherDriver ();

  public:

    // set and get
    // ------------------------------------------------------

    // internal
    void                  setScriptName (std::string scriptName);

    std::string           getScriptName () const
                              { return fScriptName; }

    bool                  getTraceScanning () const
                              { return fTraceScanning; }

    const smi::location&
                          getScannerLocation () const
                              { return fScannerLocation; }

    smi::location&
                          getScannerLocationNonConst ()
                            // no const here
                            // due to constraints in the Flex-generated code
                              { return fScannerLocation; }

    bool                  getTraceParsing () const
                              { return fTraceParsing; }

    Bool                  getDisplayTokens () const
                              { return fDisplayTokens; }

    Bool                  getTraceExpressions () const
                              { return fTraceExpressions; }

  public:

    // public services
    // ------------------------------------------------------

    // run the parser, return 0 on success
    int                   parseInput_Pass1 ();

//     // options blocks
//     void                  expressionsStackPush (
//                             const S_stringMatcherExpression& expression,
//                             const std::string&        context);

//     S_stringMatcherExpression    expressionsStackTop ();

//     void                  expressionsStackPop (
//                             const std::string& context);
//
//     void                  displayExpressionsStack (
//                             const std::string& context) const;

    // do the job
    mfMusicformatsErrorKind
                          launchstringMatcherService_Pass2 ();

  private:

    // private methods
    // ------------------------------------------------------

    void                  scanBegin ();
    void                  scanEnd ();

  private:

    // private fields
    // ------------------------------------------------------

    // the name of the MusicFormats script
    std::string           fScriptName;

//     // expressions
//     mfStack<S_stringMatcherExpression>
//                           fExpressionsStack;

//     S_stringMatcherExpression    fMainExpression;

    // scanning
    bool                  fTraceScanning; // this interacts with the scanner
    smi::location      fScannerLocation;

    // parsing
    Bool                  fDisplayServiceAndInput;

    bool                  fTraceParsing; // this interacts with the scanner

    Bool                  fDisplayOptions;

    // tokens
    Bool                  fDisplayTokens;

    // trace
    Bool                  fTraceExpressions;
};

//______________________________________________________________________________
// Give Flex the prototype of yylex() we want ...
# define YY_DECL \
  smi::parser::symbol_type yylex (stringMatcherDriver& drv)
// ... and declare it for the parser's sake
YY_DECL;


#endif // ___stringMatcherDriver___
