%{

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

#include <string.h> // strcat(), strlcat() is not available on Ubuntu nor Windows
#include <stdlib.h> // getenv()

#include "mfBool.h"

#include "mfMusicformatsError.h" // for mfMusicformatsError

#include "oahBasicTypes.h"

#include "mfslTokens.h"
#include "mfslParser.h"

#include "mfslWae.h"

#include "mfslInterpreterInterface.h"


// using namespace MusicFormats;


/* ---------------------------------------------------------------------- */
/* variables global to yylex() */
/* ---------------------------------------------------------------------- */

// Bool                   gMontrerLesTerminaux;
//
// mfslTokenKind          gmfslTokenKindCourant;

Bool                      pVerboseMode;

string                    pInputFileName;


/* ---------------------------------------------------------------------- */
/* functions */
/* ---------------------------------------------------------------------- */

%}


/* ---------------------------------------------------------------------- */
/* flex options */
/* ---------------------------------------------------------------------- */

%option yylineno


/* ---------------------------------------------------------------------- */
/* regular expressions */
/* ---------------------------------------------------------------------- */

character                 .

spaces                    [ \t]+
endOfLine                 [\n]

letter                    [A-Za-zéèêàâòôùûî]
digit                     [0-9]

name                      {letter}(_|-|\.|{letter}|{digit})*
integer                   {digit}+
exponent                  [eE][+-]?{integer}

singleleQuote             [']
doubleQuote               ["]
tabulator                 [\t]
backSlash                 [\\]


/* ---------------------------------------------------------------------- */
/* exclusive modes */
/* ---------------------------------------------------------------------- */

%x                        SINGLE_QUOTED_STRING_MODE
%x                        DOUBLE_QUOTED_STRING_MODE

%x                        COMMENT_TO_END_OF_LINE_MODE
%x                        PARENTHESIZED_COMMENT_MODE


%% /* the language tokens and the corresponding actions */


%{

/* ---------------------------------------------------------------------- */
/* variables local to yylex() */
/* ---------------------------------------------------------------------- */

#define                   STRING_BUFFER_SIZE 1024
char                      pStringBuffer [STRING_BUFFER_SIZE];

%}


"#" {
if (pVerboseMode) {
//   cerr << "// ==> mode COMMENT_TO_END_OF_LINE_MODE" << endl;
}

  BEGIN COMMENT_TO_END_OF_LINE_MODE;
}

<COMMENT_TO_END_OF_LINE_MODE>{endOfLine} {
if (pVerboseMode) {
//   cerr << "==> back to mode INITIAL" << endl;
}

  BEGIN INITIAL;
}

<COMMENT_TO_END_OF_LINE_MODE>. {
  /* accepting any character other than {endOfLine} */
}



"/*" {
if (pVerboseMode) {
//   cerr << "==> mode PARENTHESIZED_COMMENT_MODE" << endl;
}

  BEGIN PARENTHESIZED_COMMENT_MODE;
}

<PARENTHESIZED_COMMENT_MODE>[^*{endOfLine}]* {
  /* accepting any character other than '*' */
}

<PARENTHESIZED_COMMENT_MODE>"*"+[^*/{endOfLine}]* {
  /* accepting all lthe '*' not followed by a '/' */
}

<PARENTHESIZED_COMMENT_MODE>"*"+"/" {
if (pVerboseMode) {
//   cerr << "==> back to mode INITIAL" << endl;
}

  BEGIN INITIAL;
}

<PARENTHESIZED_COMMENT_MODE>. {
}



{singleleQuote} {
  pStringBuffer [0] = '\0';

if (pVerboseMode) {
//   cerr << "==> mode SINGLE_QUOTED_STRING_MODE" << endl;
}

  BEGIN SINGLE_QUOTED_STRING_MODE;
}

<SINGLE_QUOTED_STRING_MODE>{singleleQuote} {
  yylval.fString = strdup (pStringBuffer);

if (pVerboseMode) {
  cerr << "--> token: single quoted string [" << yylval.fString << "]" << endl;

//   cerr << "==> back to mode INITIAL" << endl;
}

  BEGIN INITIAL;

  return MfslToken::kSINGLE_QUOTED_STRING;
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

if (pVerboseMode) {
//   cerr << "==> mode kDOUBLE_QUOTED_STRING" << endl;
}

  BEGIN DOUBLE_QUOTED_STRING_MODE;
}

<DOUBLE_QUOTED_STRING_MODE>{doubleQuote} {
  yylval.fString = strdup (pStringBuffer);

if (pVerboseMode) {
  cerr << "--> token: double quoted string [" << yylval.fString << "]" << endl;

//   cerr << "==> back to mode INITIAL" << endl;
}

  BEGIN INITIAL;

  return MfslToken::kSINGLE_QUOTED_STRING;
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



{spaces} {
  /* ignoring this separator */
}

{endOfLine} {
  /* ignoring this separator */
}



{integer}"."{integer}({exponent})? |
{integer}{exponent} {
if (pVerboseMode) {
  cerr << "--> token: float " << yytext << endl;
}

 yylval.fDoubleNumber = atof (yytext);
  return MfslToken::kDOUBLE_NUMBER;
}

{integer} {
if (pVerboseMode) {
  cerr << "--> token: integer " << yytext << endl;
}

  yylval.fIntegerNumber = atoi (yytext);
  return MfslToken::kINTEGER_NUMBER;
}



"tool" {
if (pVerboseMode) {
  cerr << "--> token: tool" << endl;
}

  return MfslToken::kTOOL;
}

"input" {
if (pVerboseMode) {
  cerr << "--> token: input" << endl;
}

  return MfslToken::kINPUT;
}

"case" {
if (pVerboseMode) {
  cerr << "--> token: case" << endl;
}

  return MfslToken::kCASE;
}



{name} {
  yylval.fName = strdup (yytext);

if (pVerboseMode) {
  cerr << "--> token: name " << yylval.fName << endl;
}

  return MfslToken::kNAME;
}



"--"{name} |
"-"{name} {
  yylval.fOption = strdup (yytext);

if (pVerboseMode) {
  cerr << "--> token: option " << yylval.fOption << endl;
}

  return MfslToken::kOPTION;
}



"(" {
if (pVerboseMode) {
  cerr << "--> token: (" << endl;
}

  return MfslToken::kLEFT_PARENTHESIS;
}

")" {
if (pVerboseMode) {
  cerr << "--> token: )" << endl;
}

  return MfslToken::kRIGHT_PARENTHESIS;
}

"=" {
if (pVerboseMode) {
  cerr << "--> token: =" << endl;
}

  return MfslToken::kEQUALS;
}

"," {
if (pVerboseMode) {
  cerr << "--> token: ," << endl;
}

  return kCOMMA;
}

"+" {
if (pVerboseMode) {
  cerr << "--> token: +" << endl;
}

  return MfslToken::kPLUS;
}

"*" {
if (pVerboseMode) {
  cerr << "--> token: *" << endl;
}

  return MfslToken::kSTAR;
}

"/" {
if (pVerboseMode) {
  cerr << "--> token: /" << endl;
}

  return MfslToken::kSLASH;
}

":" {
if (pVerboseMode) {
  cerr << "--> token: :" << endl;
}

  return MfslToken::kCOLON;
}

";" {
if (pVerboseMode) {
  cerr << "--> token: ;" << endl;
}

  return MfslToken::kSEMI_COLON;
}

"|" {
if (pVerboseMode) {
  cerr << "--> token: |" << endl;
}

  return MfslToken::kBAR;
}



. {
  stringstream s;

  s <<
    "illegal character '" <<
    yytext [0] <<
    " in MFSL script" <<
    ", line " <<
    yylineno;

  mfslError (
    pInputFileName,
    yylineno,
    __FILE__, __LINE__,
    s.str ());
}


%%

/* ---------------------------------------------------------------------- */
/* service code */
/* ---------------------------------------------------------------------- */

//______________________________________________________________________________
int yywrap (void)
{
  static Bool pPrivateThisFunctionHasBeenRun (false);

  if (! pPrivateThisFunctionHasBeenRun) {
    if (pInputFileName == "-") {
      gLogStream <<
        "Reading from standard input" <<
        endl;
    }

    else {
      gLogStream <<
        "Opening \"" << pInputFileName << "\" for reading" <<
        endl;

      if ( (yyin = fopen (pInputFileName.c_str (), "r")) == NULL ) {
        stringstream s;

        s <<
          "Cannot open \"" << pInputFileName << "\"";

        mfslFileError (
          pInputFileName,
          s.str ());

        return 1;
      }
    }

    pPrivateThisFunctionHasBeenRun = true;
  }

  // only one script file is to be analyzed
  return 1;
}

//______________________________________________________________________________
int yyerror (char const* message)
{
  mfslError (
    pInputFileName,
    yylineno,
    __FILE__, __LINE__,
    message);

  return 0;
}

//______________________________________________________________________________
EXP void performMfslLexicalAnalysisOnly (
  const string& inputSourceName,
	Bool	        verboseMode)
{
	pVerboseMode   = verboseMode;

  pInputFileName = inputSourceName;

	yywrap ();	// open the first (and only) script file

	while (yylex () != 0)	//	till the end of file
		;	//	nothing
}

//_______________________________________________________________________________
mfMusicformatsError launchMfslInterpreter (
  const string&           inputSourceName,
  string&                 theMfTool,
  oahOptionsAndArguments& optionsAndArguments,
  Bool                    verboseMode)
{
  mfMusicformatsError
    result =
      mfMusicformatsError::k_NoError;

//   if (getenv ("YYDEBUG") != nullptr) {
//     gLogStream <<
//       "--> YYDEBUG is defined in the environment" <<
//       endl;
//
//     yydebug = 1;
//   }
//   else {
//     gLogStream <<
//       "--> YYDEBUG is NOT defined in the environment" <<
//       endl;
//   }
//
//   yydebug = 1;

	pVerboseMode   = verboseMode;

  pInputFileName = inputSourceName;

//   gLogStream <<
//     "--> pInputFileName: \"" << pInputFileName << "\"" <<
//     endl <<
//     "--> pVerboseMode: " << pVerboseMode <<
//     endl;

 	yywrap ();	// open the first (and only) script file

	if (yyparse ()) {
	  // the interpretation failed
	  result = mfMusicformatsError::kErrorInvalidFile;
	}

  fclose (yyin);

  theMfTool = "theMfTool";

	return result;
}
