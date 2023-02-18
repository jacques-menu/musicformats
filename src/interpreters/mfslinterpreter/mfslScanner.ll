%{ /* -*- C++ -*- */

/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

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

#include "mfslDriver.h"
#include "mfslParser.h"

#include "mfslWae.h"

#include "mfslInterpreterOah.h"

#include "mfslInterpreterInterface.h"

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

%option prefix="mfsl"

%option yylineno

%option noyywrap

%option nounput noinput debug interactive


%{
//   mfsl::parser::symbol_type JMI
//   make_NAME (const std::string &s, const mfsl::parser::location_type& loc);
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

// A handy shortcut to the location held by the mfslDriver
mfsl::location& loc = drv.getScannerLocationNonConst ();

// Code run each time yylex() is called
loc.step ();

%}


{blank} {
  loc.step ();
}

{endOfLine} {
  loc.lines (yyleng); loc.step ();
}



"#" { // comment
  loc.step ();
  BEGIN COMMENT_TO_END_OF_LINE_MODE;
}

<COMMENT_TO_END_OF_LINE_MODE>{endOfLine} {
  loc.lines (yyleng); loc.step ();
  BEGIN INITIAL;
}

<COMMENT_TO_END_OF_LINE_MODE>. {
  /* accepting any character other than {endOfLine} */
  loc.step ();
}


"###" {
  loc.step ();
  BEGIN PARENTHESIZED_COMMENT_MODE;
}

<PARENTHESIZED_COMMENT_MODE>{endOfLine} {
  loc.lines (yyleng); loc.step ();
}

<PARENTHESIZED_COMMENT_MODE>"###" {
  BEGIN INITIAL;
}

<PARENTHESIZED_COMMENT_MODE>. {
  /* accepting any character other than {endOfLine} */
  loc.step ();
}



{singleleQuote} {
  pStringBuffer [0] = '\0';
  BEGIN SINGLE_QUOTED_STRING_MODE;
}

<SINGLE_QUOTED_STRING_MODE>{singleleQuote} {
  if (drv.getDisplayTokens ()) {
    gLog <<
      "--> " << drv.getScannerLocation () <<
      ": single quoted std::string [" << pStringBuffer << "]" <<
      endl;
  }

  BEGIN INITIAL;

  return
    mfsl::parser::make_SINGLE_QUOTED_STRING (pStringBuffer,loc);
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
      ": double quoted std::string [" << pStringBuffer << "]" <<
      endl;
  }

  loc.step ();
  BEGIN INITIAL;
  return
    mfsl::parser::make_DOUBLE_QUOTED_STRING (pStringBuffer, loc);
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
    mfsl::parser::make_DOUBLE (yytext, loc);
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
    mfsl::parser::make_INTEGER (yytext, loc);
}




"service" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mfsl::parser::make_SERVICE (loc);
}

"input" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mfsl::parser::make_INPUT (loc);
}

"choice" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mfsl::parser::make_CHOICE (loc);
}

"default" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mfsl::parser::make_DEFAULT (loc);
}

"case" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mfsl::parser::make_CASE (loc);
}

"select" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mfsl::parser::make_SELECT (loc);
}

"all" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mfsl::parser::make_ALL (loc);
}



{name} {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": name [" << yytext << "]" <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mfsl::parser::make_NAME (yytext, loc);
}



"--"{name} |
"-"{name} {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": option [" << yytext << "]" <<
    endl;
  }

  loc.begin.column += yyleng;

  return
    mfsl::parser::make_OPTION (yytext, loc);
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
    mfsl::parser::make_COMMA (loc);
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
    mfsl::parser::make_COLON (loc);
}

";" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mfsl::parser::make_SEMICOLON (loc);
}

"|" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mfsl::parser::make_BAR (loc);
}

"&" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mfsl::parser::make_AMPERSAND (loc);
}

"=" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    mfsl::parser::make_EQUAL (loc);
}


. {
   throw mfsl::parser::syntax_error (
     loc,
     "### invalid character: " + std::string (yytext));
}



<<EOF>> {
  return
    mfsl::parser::make_YYEOF (loc);
}



%% /* ------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- */
/* service code */
/* ---------------------------------------------------------------------- */

void mfslDriver::scanBegin ()
{
  yy_flex_debug = fTraceScanning;

  if (fScriptName.empty () || fScriptName == "-") {
    yyin = stdin;
  }

  else if (!(yyin = fopen (fScriptName.c_str (), "r")))
    {
      stringstream s;

      char*
        errorString =
          mfStrErrorCString ();

      if (errorString != nullptr) {
        s <<
          "cannot open " <<
          fScriptName << ": " <<
          errorString <<
          endl;

        mfslFileError (
          fScriptName,
          s.str ());
      }
    }
}

void mfslDriver::scanEnd ()
{
  fclose (yyin);
}

//_______________________________________________________________________________
mfMusicformatsErrorKind launchMfslInterpreter ()
{
  mfMusicformatsErrorKind
    result =
      mfMusicformatsErrorKind::kMusicformatsError_NONE;

  // the driver
  mfslDriver
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
      theDriver.launchMfslService_Pass2 ();
  }

	return result;
}
