/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfFindDriver___
#define ___mfFindDriver___

#include <string>

#include "mfBool.h"
#include "mfList.h"
#include "mfMusicformatsErrors.h" // for mfMusicformatsErrorKind
#include "mfStack.h"

#include "mfFindBasicTypes.h"

#include "location.hh"

#include "mfFindParser.h"


using namespace std;

using namespace MusicFormats;

//______________________________________________________________________________
// Conducting the whole scanning and parsing of mfFind
class mfFindDriver : public smartable
{
  public:

    // // constructor/destructor
    // ------------------------------------------------------

                          mfFindDriver ();

    virtual               ~mfFindDriver ();

  public:

    // set and get
    // ------------------------------------------------------

    // internal
    void                  setScriptName (std::string scriptName);

    std::string           getScriptName () const
                              { return fScriptName; }

    bool                  getTraceScanning () const
                              { return fTraceScanning; }

    const mfFind::location&
                          getScannerLocation () const
                              { return fScannerLocation; }

    mfFind::location&     getScannerLocationNonConst ()
                            // no const here
                            // due to constraints in the Flex-generated code
                              { return fScannerLocation; }

    bool                  getTraceParsing () const
                              { return fTraceParsing; }

    bool                  getDisplayTokens () const
                              { return fDisplayTokens; }

  public:

    // public services
    // ------------------------------------------------------

    // run the parser, return 0 on success
    int                   parseInput_Pass1 ();

    mfMusicformatsErrorKind
                          launchmfFindService_Pass2 ();

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

    mfStack<S_mfFindExpression>
                          fExpressionsStack;

    mfList<S_mfFindExpression>
                          fExpressionsList;

    // scanning
    bool                  fTraceScanning;
    mfFind::location      fScannerLocation;

    // parsing
    bool                  fDisplayServiceAndInput;

    bool                  fTraceParsing;

    bool                  fDisplayOptions;

    // tokens
    bool                  fDisplayTokens;

};

//______________________________________________________________________________
// Give Flex the prototype of yylex() we want ...
# define YY_DECL \
  mfFind::parser::symbol_type yylex (mfFindDriver& drv)
// ... and declare it for the parser's sake
YY_DECL;


#endif // ___mfFindDriver___
