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

#include <sstream>

#include "mfslTokens.h"

#include "mfslParser.h"

#include "mfslWae.h"


// using namespace std;

using namespace MusicFormats;


/* ---------------------------------------------------------------------- */
/* variables global to yylex() */
/* ---------------------------------------------------------------------- */

// bool                gMontrerLesTerminaux;
//
// mfslTokenKind      gmfslTokenKindCourant;


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

%x                        STRING_ACCEPTATION

%x                        COMMENT_TO_END_OF_LINE
%x                        PARENTHESIZED_COMMENT


%% /* the language tokens and the corresponding actions */


%{

/* ---------------------------------------------------------------------- */
/* variables local to yylex() */
/* ---------------------------------------------------------------------- */

%}


"#" {
  /*
  cout << "==> mode COMMENT_TO_END_OF_LINE" << endl;
  */
  BEGIN COMMENT_TO_END_OF_LINE;
}

<COMMENT_TO_END_OF_LINE>{endOfLine} {
  /* cout << "==> mode INITIAL" << endl; */
  BEGIN INITIAL;
}

<COMMENT_TO_END_OF_LINE>. {
  /* accepting any character other than {endOfLine} */
}



"/*" {
  /* cout << "==> mode PARENTHESIZED_COMMENT" << endl; */
  BEGIN PARENTHESIZED_COMMENT;
}

<PARENTHESIZED_COMMENT>[^*{endOfLine}]* {
  /* accepting any character other than '*' */
}

<PARENTHESIZED_COMMENT>"*"+[^*/{endOfLine}]* {
  /* accepting all lthe '*' not followed by a '/' */
}

<PARENTHESIZED_COMMENT>"*"+"/" {
  /* cout << "==> mode INITIAL" << endl; */
  BEGIN INITIAL;
}

<PARENTHESIZED_COMMENT>. {
}



{doubleQuote} {
  yylval.fString = new string ("");
  /* cout << "==> mode STRING_ACCEPTATION" << endl; */
  BEGIN STRING_ACCEPTATION;
}

<STRING_ACCEPTATION>{doubleQuote} {
  /* cout << "==> mode INITIAL" << endl; */
  BEGIN INITIAL;
  /* yylval.fString is already right */
  return kMfslString;
}

<STRING_ACCEPTATION>{backSlash}{doubleQuote} {
  *(yylval.fString) += "\"";
}

<STRING_ACCEPTATION>{backSlash}n {
  *(yylval.fString) += "\n";
}

<STRING_ACCEPTATION>{backSlash}t {
  *(yylval.fString) += "\t";
}

<STRING_ACCEPTATION>{backSlash}{backSlash} {
  *(yylval.fString) += "\\";
}

<STRING_ACCEPTATION>{endOfLine} {
  *(yylval.fString) += yytext;
}

<STRING_ACCEPTATION>. {
  *(yylval.fString) += yytext;
}



{spaces} {
  /* ignoring this separator */
}

{endOfLine} {
  /* ignoring this separator */
}



{integer}"."{integer}({exponent})? |
{integer}{exponent}? {
  yylval.fDoubleNumber = atof (yytext);
  return kMfslDoubleNumber;
}

{integer} {
  yylval.fIntegerNumber = atoi (yytext);
  return kMfslIntegerNumber;
}



"tool" {
  return kMfslTool;
}

"input" {
  return kMfslInput;
}

"case" {
  return kMfslCase;
}



{name} {
  *(yylval.fName) = yytext;
  return kMfslName;
}



"(" {
  return kMfslLeftParenthesis;
}

")" {
  return kMfslRightParenthesis;
}

"=" {
  return kMfslEquals;
}

"," {
  return kMfslComma;
}

"+" {
  return kMfslPlus;
}

"-" {
  return kMfslMinus;
}

"*" {
  return kMfslTimes;
}

"/" {
  return kMfslSlash;
}

":" {
  return kMfslColon;
}

";" {
  return kMfslSemicolon;
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
  // only one script file is to be analyzed
  return 1;
}

//______________________________________________________________________________
int yyerror (char* message)
{
  mfslError (
    "gGlobalServiceRunData->getInputSourceName ()",
    yylineno,
    __FILE__, __LINE__,
    message);

 return 0;
}


