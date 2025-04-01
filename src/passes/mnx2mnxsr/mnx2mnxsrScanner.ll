%{ /* -*- C++ -*- */

/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/


/* ---------------------------------------------------------------------- */
/* inclusions */
/* ---------------------------------------------------------------------- */

#include <sstream>
#include <string>

#include "mfStringsHandling.h"
#include "mfSystemInterface.h"

#include "oahBasicTypes.h"

#include "mnx2mnxsrDriver.h"
#include "mnx2mnxsrParser.h"

#include "mnx2mnxsrWae.h"

#include "mnx2mnxsrInterpreterOah.h"

#include "mnx2mnxsrInterpreterInterface.h"

#ifdef WIN32
  #include "mfSystemInterface.h" // for isatty()

  #define YY_NO_UNISTD_H
#endif // WIN32


/* ---------------------------------------------------------------------- */
/* choices global to yylex() */
/* ---------------------------------------------------------------------- */


/* ---------------------------------------------------------------------- */
/* functions */
/* ---------------------------------------------------------------------- */

%}


/* ---------------------------------------------------------------------- */
/* flex options */
/* ---------------------------------------------------------------------- */

%option prefix="mnx2mnxsr"

%option yylineno

%option noyywrap

%option nounput noinput debug interactive


%{
//   mnx2mnxsr::parser::symbol_type JMI
//   make_NAME (const std::string &s, const mnx2mnxsr::parser::location_type& loc);
%}



/* ---------------------------------------------------------------------- */
/* regular expressions */
/* ---------------------------------------------------------------------- */

blank                     [ \t\r]
endOfLine                 [\n]
character                 .

letter                    [A-Za-zéèêàâòôùûî]
digit                     [0-9]

name                      {letter}(_|-|\.|{letter}|{digit})*
integer                   {digit}+
exponent                  [eE][+-]?{integer}

singleleQuote             [']
doubleQuote               ["]
tabulator                 [\t]
backSlash                 [\\]


%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}


/* ---------------------------------------------------------------------- */
/* exclusive modes */
/* ---------------------------------------------------------------------- */

%x                        SINGLE_QUOTED_STRING_MODE
%x                        DOUBLE_QUOTED_STRING_MODE

%x                        COMMENT_TO_END_OF_LINE_MODE
%x                        PARENTHESIZED_COMMENT_MODE



%% /* ------------------------------------------------------------------- */



%{

/* ---------------------------------------------------------------------- */
/* choices local to yylex() */
/* ---------------------------------------------------------------------- */


/* strings */

#define                   STRING_BUFFER_SIZE 1024
char                      pStringBuffer [STRING_BUFFER_SIZE];

// A handy shortcut to the location held by the mnx2mnxsrDriver
mnx2mnxsr::location& loc = drv.getScannerLocationNonConst ();

// Code run each time yylex() is called
loc.step ();

%}


{blank} {
  loc.step ();
}

{endOfLine} {
  loc.lines (yyleng); loc.step ();
}



{singleleQuote} {
  pStringBuffer [0] = '\0';
  BEGIN SINGLE_QUOTED_STRING_MODE;
}

<SINGLE_QUOTED_STRING_MODE>{singleleQuote} {
  if (drv.getDisplayTokens ()) {
    gLog <<
      "--> " << drv.getScannerLocation () <<
      ": single quoted std::string [" << pStringBuffer << ']' <<
      endl;
  }

  BEGIN INITIAL;

  return
    mnx2mnxsr::parser::make_SINGLE_QUOTED_STRING (pStringBuffer,loc);
}

<SINGLE_QUOTED_STRING_MODE>{backSlash}{singleleQuote} {
  mfCharStarCat (pStringBuffer, "'", STRING_BUFFER_SIZE);
  loc.step ();
}

<SINGLE_QUOTED_STRING_MODE>{endOfLine} {
  mfCharStarCat (pStringBuffer, yytext, STRING_BUFFER_SIZE);
  loc.lines (yyleng); loc.step ();
}

<SINGLE_QUOTED_STRING_MODE>{backSlash}n {
  mfCharStarCat (pStringBuffer, "\n", STRING_BUFFER_SIZE);
}

<SINGLE_QUOTED_STRING_MODE>{backSlash}t {
  mfCharStarCat (pStringBuffer, "\t", STRING_BUFFER_SIZE);
}

<SINGLE_QUOTED_STRING_MODE>{backSlash}{backSlash} {
  mfCharStarCat (pStringBuffer, "\\", STRING_BUFFER_SIZE);
}

<SINGLE_QUOTED_STRING_MODE>. {
  mfCharStarCat (pStringBuffer, yytext, STRING_BUFFER_SIZE);
}



{doubleQuote} {
  pStringBuffer [0] = '\0';
  BEGIN DOUBLE_QUOTED_STRING_MODE;
}

<DOUBLE_QUOTED_STRING_MODE>{doubleQuote} {
  if (drv.getDisplayTokens ()) {
    gLog <<
      "--> " << drv.getScannerLocation () <<
      ": double quoted std::string [" << pStringBuffer << ']' <<
      endl;
  }

  loc.step ();
  BEGIN INITIAL;
  return
    mnx2mnxsr::parser::make_DOUBLE_QUOTED_STRING (pStringBuffer, loc);
}

<DOUBLE_QUOTED_STRING_MODE>{backSlash}{doubleQuote} {
  mfCharStarCat (pStringBuffer, "\"", STRING_BUFFER_SIZE);
  loc.step ();
}

<DOUBLE_QUOTED_STRING_MODE>{endOfLine} {
  mfCharStarCat (pStringBuffer, yytext, STRING_BUFFER_SIZE);
  loc.lines (yyleng); loc.step ();
}

<DOUBLE_QUOTED_STRING_MODE>{backSlash}n {
  mfCharStarCat (pStringBuffer, "\n", STRING_BUFFER_SIZE);
}

<DOUBLE_QUOTED_STRING_MODE>{backSlash}t {
  mfCharStarCat (pStringBuffer, "\t", STRING_BUFFER_SIZE);
}

<DOUBLE_QUOTED_STRING_MODE>{backSlash}{backSlash} {
  mfCharStarCat (pStringBuffer, "\\", STRING_BUFFER_SIZE);
}

<DOUBLE_QUOTED_STRING_MODE>. {
  mfCharStarCat (pStringBuffer, yytext, STRING_BUFFER_SIZE);
}



{integer}"."{integer}({exponent})? |
{integer}{exponent} {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    " double: " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_DOUBLE (yytext, loc);
}

{integer} {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    " integer: " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_INTEGER (yytext, loc);
}



"mnx" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_MNX (loc);
}

"version" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_VERSION (loc);
}



"global" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_GLOBAL (loc);
}



"parts" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_PARTS (loc);
}



"measures" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_MEASURES (loc);
}



"sequences" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_SEQUENCES (loc);
}



"clefs" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_CLEFS (loc);
}

"clef" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_CLEF (loc);
}

"sign" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_SIGN (loc);
}

"staffPosition" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_STAFF_POSITION (loc);
}



"content" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_CONTENT (loc);
}

"type" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_TYPE (loc);
}

"duration" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_DURATION (loc);
}

"base" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_BASE (loc);
}



"notes" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_NOTES (loc);
}

"pitch" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_PITCH (loc);
}

"octave" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_OCTAVE (loc);
}

"step" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_STEP (loc);
}




{name} {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": name [" << yytext << ']' <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_NAME (yytext, loc);
}



":" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_COLON (loc);
}

"{" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_LEFT_CURLY_BRACKET (loc);
}

"}" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_RIGHT_CURLY_BRACKET (loc);
}

"[" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_LEFT_BRACKET (loc);
}

"]" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_RIGHT_BRACKET (loc);
}

"," {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mnx2mnxsr::parser::make_COMMA (loc);
}



. {
   throw mnx2mnxsr::parser::syntax_error (
     loc,
     "### invalid character: " + std::string (yytext));
}



<<EOF>> {
  return
    mnx2mnxsr::parser::make_YYEOF (loc);
}



%% /* ------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- */
/* service code */
/* ---------------------------------------------------------------------- */

void mnx2mnxsrDriver::scanBegin ()
{
  yy_flex_debug = fTraceScanning;

  if (fScriptName.empty () || fScriptName == "-") {
    yyin = stdin;
  }

  else if (!(yyin = fopen (fScriptName.c_str (), "r")))
    {
      std::stringstream ss;

      char*
        errorString =
          mfStrErrorCString ();

      if (errorString != nullptr) {
        ss <<
          "cannot open " <<
          fScriptName << ": " <<
          errorString <<
          endl;

        mnx2mnxsrFileError (
          fScriptName,
          ss.str ());
      }
    }
}

void mnx2mnxsrDriver::scanEnd ()
{
  fclose (yyin);
}

//_______________________________________________________________________________
mfMusicformatsErrorKind launchMnx2mnxsrInterpreter ()
{
  mfMusicformatsErrorKind
    result =
      mfMusicformatsErrorKind::kMusicformatsError_NONE;

  // the driver
  mnx2mnxsrDriver
    theDriver;

  // parse the script
  int
    parseResult =
      theDriver.parseInput_Pass1 ();

  // launch the service
  if (parseResult != 0) {
    result =
      mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  else {
    result =
      theDriver.launchMnx2mnxsrService_Pass2 ();
  }

  return result;
}
