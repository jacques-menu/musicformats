%{ /* -*- C++ -*- */

/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

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

#include "stringMatcherDriver.h"
#include "stringMatcherParser.h"

#include "stringMatcherWae.h"

#include "stringMatcherInterpreterOah.h"

#include "stringMatcherInterpreterInterface.h"

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

%option prefix="smi"

%option yylineno

%option noyywrap

%option nounput noinput debug interactive


%{
//   smi::parser::symbol_type JMI
//   make_NAME (const std::string &s, const smi::parser::location_type& loc);
%}



/* ---------------------------------------------------------------------- */
/* regular expressions */
/* ---------------------------------------------------------------------- */

blank                     [ \t\r]
endOfLine                 [\n]
character                 .

letter                    [A-Za-zéèêàâòôùûî]
digit                     [0-9]

word                      {letter}(_|-|\.|{letter}|{digit})*
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



%% /* ------------------------------------------------------------------- */



%{

/* ---------------------------------------------------------------------- */
/* choices local to yylex() */
/* ---------------------------------------------------------------------- */


/* strings */

#define                   STRING_BUFFER_SIZE 1024
char                      pStringBuffer [STRING_BUFFER_SIZE];

// A handy shortcut to the location held by the stringMatcherDriver
smi::location& loc = drv.getScannerLocationNonConst ();

// Code run each time yylex() is called
loc.step ();

%}


{blank} {
  loc.step ();
}

{word} {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": word [" << yytext << ']' <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    smi::parser::make_WORD (yytext, loc);
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
    smi::parser::make_SINGLE_QUOTED_STRING (pStringBuffer,loc);
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
      ": double quoted string [" << pStringBuffer << ']' <<
      endl;
  }

  loc.step ();
  BEGIN INITIAL;
  return
    smi::parser::make_DOUBLE_QUOTED_STRING (pStringBuffer, loc);
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


"|" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    smi::parser::make_OR (loc);
}


"^" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    smi::parser::make_XOR (loc);
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
    smi::parser::make_AND (loc);
}


"!" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    smi::parser::make_NOT (loc);
}


"(" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    smi::parser::make_LEFT_PARENTHESIS (loc);
}

")" {
  if (drv.getDisplayTokens ()) {
    gLog << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl;
  }

  loc.begin.column += yyleng;
  loc.step ();

  return
    smi::parser::make_RIGHT_PARENTHESIS (loc);
}

. {
   throw smi::parser::syntax_error (
     loc,
     "### invalid character: " + std::string (yytext));
}



<<EOF>> {
  return
    smi::parser::make_YYEOF (loc);
}



%% /* ------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- */
/* service code */
/* ---------------------------------------------------------------------- */

void stringMatcherDriver::scanBegin ()
{
  yy_flex_debug = fTraceScanning;

  if (fScriptName.size () == 0 || fScriptName == "-") {
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

        stringMatcherFileError (
          fScriptName,
          ss.str ());
      }
    }
}

void stringMatcherDriver::scanEnd ()
{
  fclose (yyin);
}
