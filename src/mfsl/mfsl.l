%{

/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/


#define DEBUG_MFSL_SCANNER


/* ---------------------------------------------------------------------- */
/* inclusions */
/* ---------------------------------------------------------------------- */

#include <iostream> // for cout, cerr, TEMP JMI
#include <sstream>

#include "mfBool.h"

#include "mfMusicformatsError.h" // for mfMusicformatsError

#include "oahBasicTypes.h"

#include "mfslTokens.h"
#include "mfslParser.h"

#include "mfslWae.h"

#include "mfslInterpreterInterface.h"


using namespace MusicFormats;


/* ---------------------------------------------------------------------- */
/* variables global to yylex() */
/* ---------------------------------------------------------------------- */

// Bool                   gMontrerLesTerminaux;
//
// mfslTokenKind          gmfslTokenKindCourant;

string                    pInputFileName;

Bool                      pVerboseMode; // JMI


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

name                      {letter}(_|{letter}|{digit})*
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
#ifdef DEBUG_MFSL_SCANNER
  cerr << "==> mode COMMENT_TO_END_OF_LINE_MODE" << endl;
#endif

  BEGIN COMMENT_TO_END_OF_LINE_MODE;
}

<COMMENT_TO_END_OF_LINE_MODE>{endOfLine} {
#ifdef DEBUG_MFSL_SCANNER
  cerr << "==> back to mode INITIAL" << endl;
#endif

  BEGIN INITIAL;
}

<COMMENT_TO_END_OF_LINE_MODE>. {
  /* accepting any character other than {endOfLine} */
}



"/*" {
#ifdef DEBUG_MFSL_SCANNER
  cerr << "==> mode PARENTHESIZED_COMMENT_MODE" << endl;
#endif

  BEGIN PARENTHESIZED_COMMENT_MODE;
}

<PARENTHESIZED_COMMENT_MODE>[^*{endOfLine}]* {
  /* accepting any character other than '*' */
}

<PARENTHESIZED_COMMENT_MODE>"*"+[^*/{endOfLine}]* {
  /* accepting all lthe '*' not followed by a '/' */
}

<PARENTHESIZED_COMMENT_MODE>"*"+"/" {
#ifdef DEBUG_MFSL_SCANNER
  cerr << "==> back to mode INITIAL" << endl;
#endif

  BEGIN INITIAL;
}

<PARENTHESIZED_COMMENT_MODE>. {
}



{singleleQuote} {
  pStringBuffer [0] = '\0';

#ifdef DEBUG_MFSL_SCANNER
  cerr << "==> mode SINGLE_QUOTED_STRING_MODE" << endl;
#endif

  BEGIN SINGLE_QUOTED_STRING_MODE;
}

<SINGLE_QUOTED_STRING_MODE>{singleleQuote} {
#ifdef DEBUG_MFSL_SCANNER
  cerr << "--> single quoted string: [" << yytext << "]" << endl;

  cerr << "==> back to mode INITIAL" << endl;
#endif

  BEGIN INITIAL;

  yylval.fString = pStringBuffer;
  return MusicFormats::kSINGLE_QUOTED_STRING;
}

<SINGLE_QUOTED_STRING_MODE>{backSlash}{singleleQuote} {
  strlcat (pStringBuffer, "'", STRING_BUFFER_SIZE);
}

<SINGLE_QUOTED_STRING_MODE>{backSlash}n {
  strlcat (pStringBuffer, "\n", STRING_BUFFER_SIZE);
}

<SINGLE_QUOTED_STRING_MODE>{backSlash}t {
  strlcat (pStringBuffer, "\t", STRING_BUFFER_SIZE);
}

<SINGLE_QUOTED_STRING_MODE>{backSlash}{backSlash} {
  strlcat (pStringBuffer, "\\", STRING_BUFFER_SIZE);
}

<SINGLE_QUOTED_STRING_MODE>{endOfLine} {
  strlcat (pStringBuffer, yytext, STRING_BUFFER_SIZE);
}

<SINGLE_QUOTED_STRING_MODE>. {
  strlcat (pStringBuffer, yytext, STRING_BUFFER_SIZE);
}



{doubleQuote} {
  pStringBuffer [0] = '\0';

#ifdef DEBUG_MFSL_SCANNER
  cerr << "==> mode kDOUBLE_QUOTED_STRING" << endl;
#endif

  BEGIN DOUBLE_QUOTED_STRING_MODE;
}

<DOUBLE_QUOTED_STRING_MODE>{doubleQuote} {
#ifdef DEBUG_MFSL_SCANNER
  cerr << "--> double quoted string: [" << yytext << "]" << endl;

  cerr << "==> back to mode INITIAL" << endl;
#endif

  BEGIN INITIAL;

  yylval.fString = pStringBuffer;
  return MusicFormats::kSINGLE_QUOTED_STRING;
}

<DOUBLE_QUOTED_STRING_MODE>{backSlash}{doubleQuote} {
  strlcat (pStringBuffer, "\"", STRING_BUFFER_SIZE);
}

<DOUBLE_QUOTED_STRING_MODE>{backSlash}n {
  strlcat (pStringBuffer, "\n", STRING_BUFFER_SIZE);
}

<DOUBLE_QUOTED_STRING_MODE>{backSlash}t {
  strlcat (pStringBuffer, "\t", STRING_BUFFER_SIZE);
}

<DOUBLE_QUOTED_STRING_MODE>{backSlash}{backSlash} {
  strlcat (pStringBuffer, "\\", STRING_BUFFER_SIZE);
}

<DOUBLE_QUOTED_STRING_MODE>{endOfLine} {
  strlcat (pStringBuffer, yytext, STRING_BUFFER_SIZE);
}

<DOUBLE_QUOTED_STRING_MODE>. {
  strlcat (pStringBuffer, yytext, STRING_BUFFER_SIZE);
}



{spaces} {
  /* ignoring this separator */
}

{endOfLine} {
  /* ignoring this separator */
}



{integer}"."{integer}({exponent})? |
{integer}{exponent} {
 #ifdef DEBUG_MFSL_SCANNER
  cerr << "--> float: " << yytext << endl;
#endif

 yylval.fDoubleNumber = atof (yytext);
  return MusicFormats::kDOUBLE_NUMBER;
}

{integer} {
#ifdef DEBUG_MFSL_SCANNER
  cerr << "--> integer: " << yytext << endl;
#endif

  yylval.fIntegerNumber = atoi (yytext);
  return MusicFormats::kINTEGER_NUMBER;
}



"tool" {
#ifdef DEBUG_MFSL_SCANNER
  cerr << "--> tool" << endl;
#endif

  return MusicFormats::kTOOL;
}

"input" {
#ifdef DEBUG_MFSL_SCANNER
  cerr << "--> input" << endl;
#endif

  return MusicFormats::kINPUT;
}

"case" {
#ifdef DEBUG_MFSL_SCANNER
  cerr << "--> case" << endl;
#endif

  return MusicFormats::kCASE;
}



{name} {
#ifdef DEBUG_MFSL_SCANNER
  cerr << "--> name: " << yytext << endl;
#endif

  yylval.fName = strdup (yytext);
  return MusicFormats::kNAME;
}



"(" {
#ifdef DEBUG_MFSL_SCANNER
  cerr << "--> (" << endl;
#endif

  return MusicFormats::kLEFT_PARENTHESIS;
}

")" {
#ifdef DEBUG_MFSL_SCANNER
  cerr << "--> )" << endl;
#endif

  return MusicFormats::kRIGHT_PARENTHESIS;
}

"=" {
#ifdef DEBUG_MFSL_SCANNER
  cerr << "--> =" << endl;
#endif

  return MusicFormats::kEQUALS;
}

"," {
#ifdef DEBUG_MFSL_SCANNER
  cerr << "--> ," << endl;
#endif

  return MusicFormats::kCOMMA;
}

"+" {
#ifdef DEBUG_MFSL_SCANNER
  cerr << "--> +" << endl;
#endif

  return MusicFormats::kPLUS;
}

"-" {
#ifdef DEBUG_MFSL_SCANNER
  cerr << "--> -" << endl;
#endif

  return MusicFormats::kMINUS;
}

"*" {
#ifdef DEBUG_MFSL_SCANNER
  cerr << "--> *" << endl;
#endif

  return MusicFormats::kSTAR;
}

"/" {
#ifdef DEBUG_MFSL_SCANNER
  cerr << "--> /" << endl;
#endif

  return MusicFormats::kSLASH;
}

":" {
#ifdef DEBUG_MFSL_SCANNER
  cerr << "--> :" << endl;
#endif

  return MusicFormats::kCOLON;
}

";" {
#ifdef DEBUG_MFSL_SCANNER
  cerr << "--> ;" << endl;
#endif

  return MusicFormats::kSEMI_COLON;
}



. {
  stringstream s;

  s <<
    "illegal character [" <<
    yytext [0] <<
    "] at line " <<
    yylineno <<
    " in MFSL script";

  mfslError (
    "gGlobalServiceRunData->getInputSourceName ()",
    44, // inputLineNumber,
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
  if (pInputFileName == "-") {
    // read from standard input
  }

  else {
    const char*
      pInputFileNameCString =
        pInputFileName.c_str ();

    if ( (yyin = fopen (pInputFileNameCString, "r")) == NULL ) {
      perror (pInputFileNameCString);
      return 1;
    }
  }

  // only one script file is to be analyzed
  return 1;
}

//______________________________________________________________________________
int yyerror (char const* message)
{
  cerr <<
    "### MFSL error: " << message <<
    endl;

  abort ();

//   mfslError (
//     "gGlobalServiceRunData->getInputSourceName ()",
//     yylineno,
//     __FILE__, __LINE__,
//     message);

  return 0;
}

//______________________________________________________________________________
EXP void performMfslLexicalAnalysisOnly (
	int		argc,
	char* argv[],
	Bool	verboseMode)
{
  pInputFileName = argv [1];
	pVerboseMode   = verboseMode;

	yywrap ();	// open the first (and only) script file

	while (yylex () != 0)	//	till the end of file
		;	//	nothing
}

//_______________________________________________________________________________
mfMusicformatsError launchMfslInterpreter (
  int                     argc,
  char*                   argv[],
  string&                 theMfTool,
  oahOptionsAndArguments& optionsAndArguments,
  Bool                    verboseMode)
{
  mfMusicformatsError
    res =
      mfMusicformatsError::k_NoError;

  pInputFileName = argv [1];
	pVerboseMode   = verboseMode;

//  	yywrap ();	// open the first (and only) script file

	if (yyparse ()) {
	  // the interpretation failed
	  res = mfMusicformatsError::kErrorInvalidFile;
	}

  theMfTool = "theMfTool";

	return res;
}
