%{

/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>

#include <string>

#include "mfslTokens.h"


using namespace std;


/* necessary declarations */
/* ---------------------- */

extern int                yylex ();
extern int                yyerror (char* message);


// options */
/* ------- */

bool                      gVerboseMode;
bool                      gDisplayTheTokens; // for yylex ()

%}


/* MFSL tokens description */
/* ----------------------- */

%union {
 int                      fIntegerNumber;
 double                   fDoubleNumber;

 string*                  fString;

 string*                  fName;

//   // avoid default constructor deletion
//   YYSTYPE () {}
//
//   // avoid default constructor deletion
//    string* operator= (const string&) { return fString; }
};


/* MFSL tokens */
/* ----------- */

%token MFTOOL

%token INPUT
%token NAME

%token COLON
%token SEMICOLON
%token EQUALS

%token BAR
%token CASE

%token QUOTED_STRING
%token DOUBLE_QUOTED_STRING

%token DASH
%token DASH_DASH


/* the MFSL axiom */
/* -------------- */

%start MfslScript


%%


/* the MFSL non-terminals */
/* ---------------------- */

MfslScript
 : MfTool
   Input
   Contents
/*
 | error
  {
  cout <<
   endl <<
   "MESSAGE" <<
   endl << endl;
  }
*/
 ;


STRING
  : QUOTED_STRING
  | DOUBLE_QUOTED_STRING
  | NAME
  ;


MfTool
 : MFTOOL NAME
 ;


Input
 : INPUT STRING
 ;


Contents
  : Option
  | ChoiceDeclaration
  | ChoiceSetting
  | CaseStatement
  | Contents Option
  ;


Option
  : DASH NAME
  | DASH NAME STRING
  | DASH_DASH NAME
  | DASH_DASH NAME STRING


ChoiceDeclaration
  : NAME COLON Choices SEMICOLON
  ;

ChoiceSetting
  : NAME EQUALS NAME SEMICOLON
  ;

Choices
  : NAME
  | Choices BAR NAME
;


CaseStatement
  : CASE NAME Cases
;

Cases
  : Case
  | Cases Case
;

Case
  : NAME COLON Options
;

Options
  : Option
  | Options Option
;


%%


// On doit fournir l'analyseur lexical */
// ----------------------------------- */
//
// #include "mfslParser.cpp"


/* other service code */
// ----------------------------------- */


