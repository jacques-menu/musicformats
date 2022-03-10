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

#define YYDEBUG 1


#include <iostream>

#include <string>

#include "mfIndentedTextOutput.h"

#include "mfslTokens.h"


// using namespace std;

using namespace MusicFormats;

#define gLogStream        *MusicFormats::gGlobalLogIndentedOstream


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
  int                     fIntegerNumber;
  double                  fDoubleNumber;

  char*                   fString;

  char*                   fName;

  char*                   fOption;
};


/* the MFSL tokens */
/* --------------- */

%token kTOOL

%token kINPUT

%token kBOOK
%token kSCORE

%token kCASE

%token <fString> kSINGLE_QUOTED_STRING
%token <fString> kDOUBLE_QUOTED_STRING

%token <fIntegerNumber> kINTEGER_NUMBER
%token <fDoubleNumber> kDOUBLE_NUMBER

%token <fName> kNAME

%token kLEFT_PARENTHESIS
%token kRIGHT_PARENTHESIS

%token kEQUALS
%token kCOMMA
%token kPLUS
/* %token kMINUS JMI */
%token kSTAR
%token kSLASH
%token kCOLON
%token kSEMI_COLON

%token kBAR

%token <fOption> kOPTION


// the MFSL non-terminals types

%type <fString> String


/* the MFSL axiom */
/* -------------- */

%start MfslScript


%%


/* ---------------------- */
/* the MFSL non-terminals */
/* ---------------------- */

// the MFSL axiom
//_______________________________________________________________________________

MfslScript :
  Tool

  Input

  Contents

 | error
    {
      gLogStream <<
        endl <<
        "### Ill-formed MDSL input" <<
        endl;
    }
 ;


// strings
//_______________________________________________________________________________

String
  : kSINGLE_QUOTED_STRING
  | kDOUBLE_QUOTED_STRING
  ;


// options
//_______________________________________________________________________________

Options
  : Option
  | Options Option
;

Option
  :  kOPTION
    {
#ifdef DEBUG_MFSL_SCANNER
      gLogStream <<
        "==> option" <<
        endl;
#endif
    }

  |  kOPTION kNAME
    {
#ifdef DEBUG_MFSL_SCANNER
      gLogStream <<
        "$1" << ' ' << $2 <<
        endl;
#endif
    }

  | kOPTION String
    {
#ifdef DEBUG_MFSL_SCANNER
      gLogStream <<
        "==> option" << $2 <<
        endl;
#endif
    }


// tool
//_______________________________________________________________________________

Tool
 : kTOOL kNAME
    {
#ifdef DEBUG_MFSL_SCANNER
      gLogStream <<
        "==> tool " << $2 <<
        endl;
#endif
    }

 | error
    {
      gLogStream <<
        endl <<
        "'tool <NAME>' expected" <<
        endl;
    }
 ;


// input
//_______________________________________________________________________________

Input
  : kINPUT kNAME
    {
#ifdef DEBUG_MFSL_SCANNER
      gLogStream <<
        "==> input" << $2 <<
        endl;
#endif
    }

  | kINPUT String
    {
#ifdef DEBUG_MFSL_SCANNER
      gLogStream <<
        "==> input" << $2 <<
        endl;
#endif
    }
 ;


// contents
//_______________________________________________________________________________

Contents
  : Option
  | ChoiceDeclaration
  | ChoiceSetting
  | CaseStatement
  | Contents Option
  ;


// choices
//_______________________________________________________________________________

ChoiceDeclaration
  : kNAME kCOLON Choices kSEMI_COLON
    {
#ifdef DEBUG_MFSL_SCANNER
      gLogStream <<
        "==> ChoiceDeclaration" << $1 <<
        endl;
#endif
    }
  ;

ChoiceSetting
  : kNAME kEQUALS kNAME kSEMI_COLON
    {
#ifdef DEBUG_MFSL_SCANNER
      gLogStream <<
        "==> ChoiceSetting" << $1 << ' ' << $3 <<
        endl;
#endif
    }
  ;

Choices
  : kNAME
  | Choices kBAR kNAME
;


CaseStatement
  : kCASE kNAME Cases
    {
#ifdef DEBUG_MFSL_SCANNER
      gLogStream <<
        "==> CaseStatement" << $2 <<
        endl;
#endif
    }
;

Cases
  : Case
  | Cases Case
;

Case
  : kNAME kCOLON Options
    {
#ifdef DEBUG_MFSL_SCANNER
      gLogStream <<
        "==> Case" <<
        endl;
#endif
    }
;


%%


/* other service code */
// ----------------------------------- */


