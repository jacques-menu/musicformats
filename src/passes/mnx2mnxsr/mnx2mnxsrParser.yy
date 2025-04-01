%{

/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/


#include <iostream>

#include <string>

#include "mfIndentedTextOutput.h"

#include "oahBasicTypes.h"

#include "mnx2mnxsrWae.h"


using namespace MusicFormats;

%}


/// the Bison options
//_______________________________________________________________________________

%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.1"
%defines

%define api.prefix {mnx2mnxsr}

%define api.token.raw

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  #include <string>

  class mnx2mnxsrDriver;
}

// the parsing context
%param { mnx2mnxsrDriver& drv } // declaration, any parameter name is fine

%verbose // to produce mnx2mnxsrParser.output

%locations

// other Bison options
%define parse.trace
%define parse.error detailed
%define parse.lac full
// %define api.pure full

%printer { yyo << $$; } <*>;


%code {
  #include "mnx2mnxsrBasicTypes.h"
}


// the MNX2MNXSR tokens
//_______________________________________________________________________________

%define api.token.prefix {MNX2MNXSR_TOK_}

%token
  COLON                   ":"

  CURLY_LEFT_BRACKET      "{"
  CURLY_RIGHT_BRACKET     "}"

  LEFT_BRACKET            "["
  RIGHT_BRACKET           "]"

  COMMA                   ","

  MNX                     "mnx"
  VERSION                 "version"

  GLOBAL                  "global"

  PARTS                   "parts"

  MEASURES                "measures"

  SEQUENCES               "sequences"

  CLEFS                   "clefs"
  CLEF                    "clef"
  SIGN                    "sign"
  STAFF_POSITION          "staffPosition"

  CONTENT                 "content"
  TYPE                    "type"
  DURATION                "duration"
  BASE                    "base"

  NOTES                   "notes"
  PITCH                   "pitch"
  OCTAVE                  "octave"
  STEP                    "step"
;

%code {
  #include "mnx2mnxsrDriver.h"
}

%token <std::string> INTEGER "integer number"
%token <std::string> DOUBLE  "double number"

%token <std::string> SINGLE_QUOTED_STRING "single quoted_string"
%token <std::string> DOUBLE_QUOTED_STRING "double quoted_string"

%token <std::string> NAME "name"

%token <std::string> OPTION "option"


// the MNX2MNXSR non-terminals
//_______________________________________________________________________________

%nterm <std::string> Number

%nterm <std::string> SingleString
%nterm <std::string> String


// the MNX2MNXSR axiom
//_______________________________________________________________________________

%start MnxSource



//_______________________________________________________________________________
%%
//_______________________________________________________________________________



//_______________________________________________________________________________
// the MNX2MNXSR non-terminals
//_______________________________________________________________________________


// the MNX2MNXSR axiom
//_______________________________________________________________________________

MnxSource :
  CURLY_LEFT_BRACKET

  Mnx

      {
        ++gIndenter;

        gLog << "Mnx accepted" << std::endl;

        --gIndenter;
      }

  COMMA

  Global

      {
        ++gIndenter;

        gLog << "Global accepted" << std::endl;

        --gIndenter;
      }

  COMMA

  Parts

      {
        ++gIndenter;

        gLog << "Parts accepted" << std::endl;

        --gIndenter;
      }

  CURLY_RIGHT_BRACKET
;


// numbers
//_______________________________________________________________________________

Number
  : INTEGER
  | DOUBLE
;


// strings
//_______________________________________________________________________________

SingleString
  : SINGLE_QUOTED_STRING
  | DOUBLE_QUOTED_STRING
;

String
  : SingleString
  | String SingleString
      {
        $$ = $1 + $2;
      }
;


// Mnx
//_______________________________________________________________________________

Mnx
  : MNX COLON CURLY_LEFT_BRACKET
    Version
    CURLY_RIGHT_BRACKET

      {
        // drv.setService ($3);
      }
 ;

Version
  : VERSION COLON
    Number

      {
        // drv.setService ($3);
      }
 ;


// Global
//_______________________________________________________________________________

Global
  : GLOBAL COLON CURLY_LEFT_BRACKET
    Measures
    CURLY_RIGHT_BRACKET

      {
        // drv.setService ($3);
      }
 ;


// Parts
//_______________________________________________________________________________

Parts
  : PARTS COLON LEFT_BRACKET
    Part
    RIGHT_BRACKET

      {
        // drv.setService ($3);
      }
 ;

Part
  : CURLY_LEFT_BRACKET
    Measures
    CURLY_RIGHT_BRACKET

      {
        // drv.setService ($3);
      }
 ;


// Measures
//_______________________________________________________________________________

Measures
  : PARTS COLON LEFT_BRACKET
    Measure
    RIGHT_BRACKET

      {
        // drv.setService ($3);
      }
 ;

Measure
  : CURLY_LEFT_BRACKET
    MeasureElements
    CURLY_RIGHT_BRACKET

      {
        // drv.setService ($3);
      }
 ;

MeasureElements
  : MeasureElement
  | MeasureElements MeasureElement
;

MeasureElement
  : Clefs
  | Sequences
;


// Clefs
//_______________________________________________________________________________

Clefs
  : CLEFS COLON LEFT_BRACKET
    Clef
    RIGHT_BRACKET

      {
        // drv.setService ($3);
      }
 ;

Clef
  : COLON CURLY_LEFT_BRACKET
    SIGN  COLON String
    COMMA
    STAFF_POSITION COLON Number
    CURLY_RIGHT_BRACKET

      {
        // drv.setService ($3);
      }
 ;


// Sequences
//_______________________________________________________________________________

Sequences
  : SEQUENCES COLON LEFT_BRACKET
    SequencesElements
    RIGHT_BRACKET

      {
        // drv.setService ($3);
      }
 ;

SequencesElements
  : SequencesElement
  | SequencesElements SequencesElement
;

SequencesElement
  : Content
;


// Content
//_______________________________________________________________________________

Content
  : CONTENT COLON LEFT_BRACKET
    ContentElements
    RIGHT_BRACKET

      {
        // drv.setService ($3);
      }
;

ContentElements
  : ContentElement
  | ContentElements ContentElement
;

ContentElement
  : Type
    Duration
    Notes
;


// Type
//_______________________________________________________________________________

Type
  : TYPE COLON String
;


// Duration
//_______________________________________________________________________________

Duration
  : DURATION COLON CURLY_LEFT_BRACKET
    DurationElements
    CURLY_RIGHT_BRACKET

      {
        // drv.setService ($3);
      }
;

DurationElements
  : DurationElement
  | DurationElements DurationElement
;

DurationElement
  : BASE COLON String
;


// Notes
//_______________________________________________________________________________

Notes
  : DURATION COLON LEFT_BRACKET
    NotesElements
    RIGHT_BRACKET

      {
        // drv.setService ($3);
      }
;

NotesElements
  : NotesElement
  | NotesElements NotesElement
;

NotesElement
  : Pitch
;

Pitch
  : PITCH COLON CURLY_LEFT_BRACKET
    OCTAVE COLON Number
    COMMA
    STEP COLON String
    CURLY_RIGHT_BRACKET
;



//_______________________________________________________________________________
%%
//_______________________________________________________________________________


// other service code
//_______________________________________________________________________________


void
mnx2mnxsr::parser::error (const location_type& loc, const std::string& message)
{
  mnx2mnxsrError (
    message,
    loc);
}
