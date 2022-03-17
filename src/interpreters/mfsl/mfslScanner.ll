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

#include <iostream> // for cout, cerr, TEMP JMI
#include <sstream>

// #include "mfMusicformatsError.h" // for mfMusicformatsError

#include "oahBasicTypes.h"

#include "mfslDriver.h"
#include "mfslParser.h"

#include "mfslWae.h"

#include "mfslInterpreterInterface.h"


/* ---------------------------------------------------------------------- */
/* variables global to yylex() */
/* ---------------------------------------------------------------------- */


/* ---------------------------------------------------------------------- */
/* functions */
/* ---------------------------------------------------------------------- */

%}


/* ---------------------------------------------------------------------- */
/* flex options */
/* ---------------------------------------------------------------------- */

%option yylineno

%option noyywrap

%option nounput noinput interactive debug


%{
//   yy::parser::symbol_type
//   make_NAME (const std::string &s, const yy::parser::location_type& loc);
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
/* variables local to yylex() */
/* ---------------------------------------------------------------------- */


/* strings */

#define                   STRING_BUFFER_SIZE 1024
char                      pStringBuffer [STRING_BUFFER_SIZE];

// A handy shortcut to the location held by the mfslDriver.
yy::location& loc = drv.getScannerLocation ();

// Code run each time yylex is called.
loc.step ();

%}


{blank}+   loc.step ();
\n+        loc.lines (yyleng); loc.step ();



"#" {
  BEGIN COMMENT_TO_END_OF_LINE_MODE;
}

<COMMENT_TO_END_OF_LINE_MODE>{endOfLine} {
  BEGIN INITIAL;
}

<COMMENT_TO_END_OF_LINE_MODE>. {
  /* accepting any character other than {endOfLine} */
}


"/*" {
  BEGIN PARENTHESIZED_COMMENT_MODE;
}

<PARENTHESIZED_COMMENT_MODE>[^*{endOfLine}]* {
  /* accepting any character other than '*' */
}

<PARENTHESIZED_COMMENT_MODE>"*"+[^*/{endOfLine}]* {
  /* accepting all lthe '*' not followed by a '/' */
}

<PARENTHESIZED_COMMENT_MODE>"*"+"/" {
  BEGIN INITIAL;
}

<PARENTHESIZED_COMMENT_MODE>. {
}



{singleleQuote} {
  pStringBuffer [0] = '\0';

  BEGIN SINGLE_QUOTED_STRING_MODE;
}

<SINGLE_QUOTED_STRING_MODE>{singleleQuote} {
  if (drv.getDisplayTokens ()) {
    gLogStream <<
      "--> " << drv.getScannerLocation () <<
      ": single quoted string [" << pStringBuffer << "]" <<
      endl << endl;
  }

  BEGIN INITIAL;

  return yy::parser::make_SINGLE_QUOTED_STRING (pStringBuffer,loc);
}

<SINGLE_QUOTED_STRING_MODE>{backSlash}{singleleQuote} {
  strcat (pStringBuffer, "'"); // , STRING_BUFFER_SIZE);
}

<SINGLE_QUOTED_STRING_MODE>{backSlash}n {
  strcat (pStringBuffer, "\n"); // , STRING_BUFFER_SIZE);
}

<SINGLE_QUOTED_STRING_MODE>{backSlash}t {
  strcat (pStringBuffer, "\t"); // , STRING_BUFFER_SIZE);
}

<SINGLE_QUOTED_STRING_MODE>{backSlash}{backSlash} {
  strcat (pStringBuffer, "\\"); // , STRING_BUFFER_SIZE);
}

<SINGLE_QUOTED_STRING_MODE>{endOfLine} {
  strcat (pStringBuffer, yytext); // , STRING_BUFFER_SIZE);
}

<SINGLE_QUOTED_STRING_MODE>. {
  strcat (pStringBuffer, yytext); // , STRING_BUFFER_SIZE);
}


{doubleQuote} {
  pStringBuffer [0] = '\0';

  BEGIN DOUBLE_QUOTED_STRING_MODE;
}

<DOUBLE_QUOTED_STRING_MODE>{doubleQuote} {
  if (drv.getDisplayTokens ()) {
    gLogStream <<
      "--> " << drv.getScannerLocation () <<
      ": double quoted string [" << pStringBuffer << "]" <<
      endl << endl;
  }

  BEGIN INITIAL;

  return yy::parser::make_DOUBLE_QUOTED_STRING (pStringBuffer, loc);
}

<DOUBLE_QUOTED_STRING_MODE>{backSlash}{doubleQuote} {
  strcat (pStringBuffer, "\""); // , STRING_BUFFER_SIZE);
}

<DOUBLE_QUOTED_STRING_MODE>{backSlash}n {
  strcat (pStringBuffer, "\n"); // , STRING_BUFFER_SIZE);
}

<DOUBLE_QUOTED_STRING_MODE>{backSlash}t {
  strcat (pStringBuffer, "\t"); // , STRING_BUFFER_SIZE);
}

<DOUBLE_QUOTED_STRING_MODE>{backSlash}{backSlash} {
  strcat (pStringBuffer, "\\"); // , STRING_BUFFER_SIZE);
}

<DOUBLE_QUOTED_STRING_MODE>{endOfLine} {
  strcat (pStringBuffer, yytext); // , STRING_BUFFER_SIZE);
}

<DOUBLE_QUOTED_STRING_MODE>. {
  strcat (pStringBuffer, yytext); // , STRING_BUFFER_SIZE);
}



{integer}"."{integer}({exponent})? |
{integer}{exponent} {
  if (drv.getDisplayTokens ()) {
    gLogStream << "--> " << drv.getScannerLocation () <<
    " double: " << yytext <<
    endl << endl;
  }
  return yy::parser::make_DOUBLE (yytext, loc);
}

{integer} {
  if (drv.getDisplayTokens ()) {
    gLogStream << "--> " << drv.getScannerLocation () <<
    " integer: " << yytext <<
    endl << endl;
  }
  return yy::parser::make_INTEGER (yytext, loc);
}




"tool" {
  if (drv.getDisplayTokens ()) {
    gLogStream << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl << endl;
  }
  return yy::parser::make_TOOL (loc);
}

"input" {
  if (drv.getDisplayTokens ()) {
    gLogStream << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl << endl;
  }
  return yy::parser::make_INPUT (loc);
}

"choice" {
  if (drv.getDisplayTokens ()) {
    gLogStream << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl << endl;
  }
  return yy::parser::make_CHOICE (loc);
}

"set" {
  if (drv.getDisplayTokens ()) {
    gLogStream << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl << endl;
  }
  return yy::parser::make_SET (loc);
}

"case" {
  if (drv.getDisplayTokens ()) {
    gLogStream << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl << endl;
  }
  return yy::parser::make_CASE (loc);
}

"all" {
  if (drv.getDisplayTokens ()) {
    gLogStream << "--> " << drv.getScannerLocation () <<
    ": " << yytext <<
    endl << endl;
  }
  return yy::parser::make_ALL (loc);
}



{name} {
  if (drv.getDisplayTokens ()) {
    gLogStream << "--> " << drv.getScannerLocation () <<
    ": name [" << yytext << "]" <<
    endl << endl;
  }
  return yy::parser::make_NAME (yytext, loc);
}



"--"{name} |
"-"{name} {
  if (drv.getDisplayTokens ()) {
    gLogStream << "--> " << drv.getScannerLocation () <<
    ": option [" << yytext << "]" <<
    endl << endl;
  }
  return yy::parser::make_OPTION (yytext, loc);
}



"(" {
  if (drv.getDisplayTokens ()) {
    gLogStream << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl << endl;
  }
  return yy::parser::make_LEFT_PARENTHESIS (loc);
}

")" {
  if (drv.getDisplayTokens ()) {
    gLogStream << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl << endl;
  }
  return yy::parser::make_RIGHT_PARENTHESIS (loc);
}

"=" {
  if (drv.getDisplayTokens ()) {
    gLogStream << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl << endl;
  }
  return yy::parser::make_EQUAL (loc);
}

"," {
  if (drv.getDisplayTokens ()) {
    gLogStream << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl << endl;
  }
  return yy::parser::make_COMMA (loc);
}

"*" {
  if (drv.getDisplayTokens ()) {
    gLogStream << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl << endl;
  }
  return yy::parser::make_STAR (loc);
}

"/" {
  if (drv.getDisplayTokens ()) {
    gLogStream << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl << endl;
  }
  return yy::parser::make_SLASH (loc);
}

":" {
  if (drv.getDisplayTokens ()) {
    gLogStream << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl << endl;
  }
  return yy::parser::make_COLON (loc);
}

";" {
  if (drv.getDisplayTokens ()) {
    gLogStream << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl << endl;
  }
  return yy::parser::make_SEMICOLON (loc);
}

"|" {
  if (drv.getDisplayTokens ()) {
    gLogStream << "--> " << drv.getScannerLocation () <<
    ": '" << yytext << '\'' <<
    endl << endl;
  }
  return yy::parser::make_BAR (loc);
}


. {
   throw yy::parser::syntax_error (
     loc,
     "### invalid character: " + std::string (yytext));
}



<<EOF>>    return yy::parser::make_YYEOF (loc);



%% /* ------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- */
/* service code */
/* ---------------------------------------------------------------------- */

void mfslDriver::scanBegin ()
{
  yy_flex_debug = fTraceScanning;

  if (fInputFileName.empty () || fInputFileName == "-") {
    yyin = stdin;
  }

  else if (!(yyin = fopen (fInputFileName.c_str (), "r")))
    {
      std::cerr <<
        "cannot open " << fInputFileName << ": " << strerror (errno) <<
        '\n';

      exit (EXIT_FAILURE);
    }
}

void mfslDriver::scanEnd ()
{
  fclose (yyin);
}


//_______________________________________________________________________________
mfMusicformatsError launchMfslInterpreter (
  const string& inputSourceName,
  bool          traceScanning,
  bool          traceParsing,
  bool          displayTokens,
  bool          displayNonTerminals,
  bool          traceSemantics)
{
  mfMusicformatsError
    result =
      mfMusicformatsError::k_NoError;

  mfslDriver
    theDriver (
      traceScanning,
      traceParsing,
      displayTokens,
      displayNonTerminals,
      traceSemantics);

  int
    parseResult =
  	  theDriver.parseFile (inputSourceName);

  string
    theMfToolName =
      theDriver.getToolName (),
    theInputFileName =
      theDriver.getInputFileName ();

  if (theDriver.getTraceSemantics ()) {
    gLogStream <<
      "--> parseResult:   " << parseResult <<
      endl;

    gLogStream <<
      "--> toolName:      " << theMfToolName <<
      endl <<
      "--> inputFileName: " << theInputFileName <<
      endl << endl;
  }

  // parse the script
  if (parseResult != 0) {
    result =
      mfMusicformatsError::kErrorInvalidFile;
  }

  else {
    result =
      theDriver.launchMfslTool ();
  }

	return result;
}
