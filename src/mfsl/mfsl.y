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

#include "mfIndentedTextOutput.h"

#include "mfslTokens.h"


using namespace std;

using namespace MusicFormats;


/* necessary declarations */
/* ---------------------- */

extern int                yylex ();
extern int                yyerror (char const* message);


// options */
/* ------- */

%}


/* MFSL tokens description */
/* ----------------------- */

%union { // with a basic C description of strings
 int                      fIntegerNumber;
 double                   fDoubleNumber;

 char*                    fString;

 char*                    fName;
};


/* MFSL tokens */
/* ----------- */

%token kTOOL

%token kINPUT

%token kBOOK

%token kCASE

%token kSINGLE_QUOTED_STRING
%token kDOUBLE_QUOTED_STRING

%token kINTEGER_NUMBER
%token kDOUBLE_NUMBER

%token kNAME

%token kLEFT_PARENTHESIS
%token kRIGHT_PARENTHESIS

%token kEQUALS
%token kCOMMA
%token kPLUS
%token kMINUS
%token kSTAR
%token kSLASH
%token kCOLON
%token kSEMI_COLON

%token kBAR

%token kDASH
%token kDASH_DASH


/* the MFSL axiom */
/* -------------- */

%start MfslScript


%%


/* the MFSL non-terminals */
/* ---------------------- */

MfslScript
 : MfTool
 /*
   Input
   Contents
 | error
    {
    cerr <<
      endl <<
      "MESSAGE" <<
      endl;
    }
*/
 ;


STRING
  : kSINGLE_QUOTED_STRING
  | kDOUBLE_QUOTED_STRING
  | kNAME
  ;


MfTool
 : kTOOL kNAME
    {
      gLogStream <<
        "tool: " << yylval.fName <<
        endl;
    }
 | error
    {
      cerr <<
        endl <<
        "'tool name' expected" <<
        endl;
    }
 ;


Input
  : kNAME
  | kINPUT kDOUBLE_QUOTED_STRING
  | kINPUT kSINGLE_QUOTED_STRING
    {
      gLogStream <<
        "input: " << yylval.fName <<
        endl;
    }
 ;


Contents
  : Option
  | ChoiceDeclaration
  | ChoiceSetting
  | CaseStatement
  | Contents Option
  ;


Option
  : kDASH kNAME
  | kDASH_DASH kNAME

  | kDASH kNAME kSINGLE_QUOTED_STRING
  | kDASH_DASH kNAME kSINGLE_QUOTED_STRING

  | kDASH kNAME kDOUBLE_QUOTED_STRING
  | kDASH_DASH kNAME kDOUBLE_QUOTED_STRING


ChoiceDeclaration
  : kNAME kCOLON Choices kSEMI_COLON
  ;

ChoiceSetting
  : kNAME kEQUALS kNAME kSEMI_COLON
  ;

Choices
  : kNAME
  | Choices kBAR kNAME
;


CaseStatement
  : kCASE kNAME Cases
;

Cases
  : Case
  | Cases Case
;

Case
  : kNAME kCOLON Options
;

Options
  : Option
  | Options Option
;


%%


/* other service code */
// ----------------------------------- */


