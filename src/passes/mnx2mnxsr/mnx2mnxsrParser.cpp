// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.


// Take the name prefix into account.
#define yylex   mnx2mnxsrlex

// First part of user prologue.
#line 1 "mnx2mnxsrParser.yy"


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


#line 72 "mnx2mnxsrParser.cpp"


#include "mnx2mnxsrParser.h"


// Unqualified %code blocks.
#line 68 "mnx2mnxsrParser.yy"

  #include "mnx2mnxsrBasicTypes.h"
#line 116 "mnx2mnxsrParser.yy"

  #include "mnx2mnxsrDriver.h"

#line 86 "mnx2mnxsrParser.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if MNX2MNXSRDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !MNX2MNXSRDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !MNX2MNXSRDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace mnx2mnxsr {
#line 178 "mnx2mnxsrParser.cpp"

  /// Build a parser object.
  parser::parser (mnx2mnxsrDriver& drv_yyarg)
#if MNX2MNXSRDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      yy_lac_established_ (false),
      drv (drv_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_INTEGER: // "integer number"
      case symbol_kind::S_DOUBLE: // "double number"
      case symbol_kind::S_SINGLE_QUOTED_STRING: // "single quoted_string"
      case symbol_kind::S_DOUBLE_QUOTED_STRING: // "double quoted_string"
      case symbol_kind::S_NAME: // "name"
      case symbol_kind::S_OPTION: // "option"
      case symbol_kind::S_Number: // Number
      case symbol_kind::S_SingleString: // SingleString
      case symbol_kind::S_String: // String
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_INTEGER: // "integer number"
      case symbol_kind::S_DOUBLE: // "double number"
      case symbol_kind::S_SINGLE_QUOTED_STRING: // "single quoted_string"
      case symbol_kind::S_DOUBLE_QUOTED_STRING: // "double quoted_string"
      case symbol_kind::S_NAME: // "name"
      case symbol_kind::S_OPTION: // "option"
      case symbol_kind::S_Number: // Number
      case symbol_kind::S_SingleString: // SingleString
      case symbol_kind::S_String: // String
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_INTEGER: // "integer number"
      case symbol_kind::S_DOUBLE: // "double number"
      case symbol_kind::S_SINGLE_QUOTED_STRING: // "single quoted_string"
      case symbol_kind::S_DOUBLE_QUOTED_STRING: // "double quoted_string"
      case symbol_kind::S_NAME: // "name"
      case symbol_kind::S_OPTION: // "option"
      case symbol_kind::S_Number: // Number
      case symbol_kind::S_SingleString: // SingleString
      case symbol_kind::S_String: // String
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_INTEGER: // "integer number"
      case symbol_kind::S_DOUBLE: // "double number"
      case symbol_kind::S_SINGLE_QUOTED_STRING: // "single quoted_string"
      case symbol_kind::S_DOUBLE_QUOTED_STRING: // "double quoted_string"
      case symbol_kind::S_NAME: // "name"
      case symbol_kind::S_OPTION: // "option"
      case symbol_kind::S_Number: // Number
      case symbol_kind::S_SingleString: // SingleString
      case symbol_kind::S_String: // String
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if MNX2MNXSRDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        switch (yykind)
    {
      case symbol_kind::S_INTEGER: // "integer number"
#line 65 "mnx2mnxsrParser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 378 "mnx2mnxsrParser.cpp"
        break;

      case symbol_kind::S_DOUBLE: // "double number"
#line 65 "mnx2mnxsrParser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 384 "mnx2mnxsrParser.cpp"
        break;

      case symbol_kind::S_SINGLE_QUOTED_STRING: // "single quoted_string"
#line 65 "mnx2mnxsrParser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 390 "mnx2mnxsrParser.cpp"
        break;

      case symbol_kind::S_DOUBLE_QUOTED_STRING: // "double quoted_string"
#line 65 "mnx2mnxsrParser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 396 "mnx2mnxsrParser.cpp"
        break;

      case symbol_kind::S_NAME: // "name"
#line 65 "mnx2mnxsrParser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 402 "mnx2mnxsrParser.cpp"
        break;

      case symbol_kind::S_OPTION: // "option"
#line 65 "mnx2mnxsrParser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 408 "mnx2mnxsrParser.cpp"
        break;

      case symbol_kind::S_Number: // Number
#line 65 "mnx2mnxsrParser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 414 "mnx2mnxsrParser.cpp"
        break;

      case symbol_kind::S_SingleString: // SingleString
#line 65 "mnx2mnxsrParser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 420 "mnx2mnxsrParser.cpp"
        break;

      case symbol_kind::S_String: // String
#line 65 "mnx2mnxsrParser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 426 "mnx2mnxsrParser.cpp"
        break;

      default:
        break;
    }
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if MNX2MNXSRDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // MNX2MNXSRDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    // int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // Discard the LAC context in case there still is one left from a
    // previous invocation.
    yy_lac_discard_ ("init");

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (drv));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        if (!yy_lac_establish_ (yyla.kind ()))
          goto yyerrlab;
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        if (!yy_lac_establish_ (yyla.kind ()))
          goto yyerrlab;

        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    yy_lac_discard_ ("shift");
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_INTEGER: // "integer number"
      case symbol_kind::S_DOUBLE: // "double number"
      case symbol_kind::S_SINGLE_QUOTED_STRING: // "single quoted_string"
      case symbol_kind::S_DOUBLE_QUOTED_STRING: // "double quoted_string"
      case symbol_kind::S_NAME: // "name"
      case symbol_kind::S_OPTION: // "option"
      case symbol_kind::S_Number: // Number
      case symbol_kind::S_SingleString: // SingleString
      case symbol_kind::S_String: // String
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // $@1: %empty
#line 166 "mnx2mnxsrParser.yy"
      {
        ++gIndenter;

        gLog << "Mnx accepted" << std::endl;

        --gIndenter;
      }
#line 707 "mnx2mnxsrParser.cpp"
    break;

  case 3: // $@2: %empty
#line 178 "mnx2mnxsrParser.yy"
      {
        ++gIndenter;

        gLog << "Global accepted" << std::endl;

        --gIndenter;
      }
#line 719 "mnx2mnxsrParser.cpp"
    break;

  case 4: // $@3: %empty
#line 190 "mnx2mnxsrParser.yy"
      {
        ++gIndenter;

        gLog << "Parts accepted" << std::endl;

        --gIndenter;
      }
#line 731 "mnx2mnxsrParser.cpp"
    break;

  case 6: // Number: "integer number"
#line 206 "mnx2mnxsrParser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 737 "mnx2mnxsrParser.cpp"
    break;

  case 7: // Number: "double number"
#line 207 "mnx2mnxsrParser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 743 "mnx2mnxsrParser.cpp"
    break;

  case 8: // SingleString: "single quoted_string"
#line 215 "mnx2mnxsrParser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 749 "mnx2mnxsrParser.cpp"
    break;

  case 9: // SingleString: "double quoted_string"
#line 216 "mnx2mnxsrParser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 755 "mnx2mnxsrParser.cpp"
    break;

  case 10: // String: SingleString
#line 220 "mnx2mnxsrParser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 761 "mnx2mnxsrParser.cpp"
    break;

  case 11: // String: String SingleString
#line 222 "mnx2mnxsrParser.yy"
      {
        yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + yystack_[0].value.as < std::string > ();
      }
#line 769 "mnx2mnxsrParser.cpp"
    break;

  case 12: // Mnx: "mnx" ":" "{" Version "}"
#line 236 "mnx2mnxsrParser.yy"
      {
        // drv.setService ($3);
      }
#line 777 "mnx2mnxsrParser.cpp"
    break;

  case 13: // Version: "version" ":" Number
#line 245 "mnx2mnxsrParser.yy"
      {
        // drv.setService ($3);
      }
#line 785 "mnx2mnxsrParser.cpp"
    break;

  case 14: // Global: "global" ":" "{" Measures "}"
#line 259 "mnx2mnxsrParser.yy"
      {
        // drv.setService ($3);
      }
#line 793 "mnx2mnxsrParser.cpp"
    break;

  case 15: // Parts: "parts" ":" "[" Part "]"
#line 273 "mnx2mnxsrParser.yy"
      {
        // drv.setService ($3);
      }
#line 801 "mnx2mnxsrParser.cpp"
    break;

  case 16: // Part: "{" Measures "}"
#line 283 "mnx2mnxsrParser.yy"
      {
        // drv.setService ($3);
      }
#line 809 "mnx2mnxsrParser.cpp"
    break;

  case 17: // Measures: "parts" ":" "[" Measure "]"
#line 297 "mnx2mnxsrParser.yy"
      {
        // drv.setService ($3);
      }
#line 817 "mnx2mnxsrParser.cpp"
    break;

  case 18: // Measure: "{" MeasureElements "}"
#line 307 "mnx2mnxsrParser.yy"
      {
        // drv.setService ($3);
      }
#line 825 "mnx2mnxsrParser.cpp"
    break;

  case 23: // Clefs: "clefs" ":" "[" Clef "]"
#line 331 "mnx2mnxsrParser.yy"
      {
        // drv.setService ($3);
      }
#line 833 "mnx2mnxsrParser.cpp"
    break;

  case 24: // Clef: ":" "{" "sign" ":" String "," "staffPosition" ":" Number "}"
#line 343 "mnx2mnxsrParser.yy"
      {
        // drv.setService ($3);
      }
#line 841 "mnx2mnxsrParser.cpp"
    break;

  case 25: // Sequences: "sequences" ":" "[" SequencesElements "]"
#line 357 "mnx2mnxsrParser.yy"
      {
        // drv.setService ($3);
      }
#line 849 "mnx2mnxsrParser.cpp"
    break;

  case 29: // Content: "content" ":" "[" ContentElements "]"
#line 380 "mnx2mnxsrParser.yy"
      {
        // drv.setService ($3);
      }
#line 857 "mnx2mnxsrParser.cpp"
    break;

  case 34: // Duration: "duration" ":" "{" DurationElements "}"
#line 413 "mnx2mnxsrParser.yy"
      {
        // drv.setService ($3);
      }
#line 865 "mnx2mnxsrParser.cpp"
    break;

  case 38: // Notes: "duration" ":" "[" NotesElements "]"
#line 436 "mnx2mnxsrParser.yy"
      {
        // drv.setService ($3);
      }
#line 873 "mnx2mnxsrParser.cpp"
    break;


#line 877 "mnx2mnxsrParser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        // ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      yy_lac_discard_ ("error recovery");
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  const char *
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    static const char *const yy_sname[] =
    {
    "end of file", "error", "invalid token", ":", "{", "}", "[", "]", ",",
  "mnx", "version", "global", "parts", "measures", "sequences", "clefs",
  "clef", "sign", "staffPosition", "content", "type", "duration", "base",
  "notes", "pitch", "octave", "step", "integer number", "double number",
  "single quoted_string", "double quoted_string", "name", "option",
  "$accept", "MnxSource", "$@1", "$@2", "$@3", "Number", "SingleString",
  "String", "Mnx", "Version", "Global", "Parts", "Part", "Measures",
  "Measure", "MeasureElements", "MeasureElement", "Clefs", "Clef",
  "Sequences", "SequencesElements", "SequencesElement", "Content",
  "ContentElements", "ContentElement", "Type", "Duration",
  "DurationElements", "DurationElement", "Notes", "NotesElements",
  "NotesElement", "Pitch", YY_NULLPTR
    };
    return yy_sname[yysymbol];
  }



  // parser::context.
  parser::context::context (const parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

#if MNX2MNXSRDEBUG
    // Execute LAC once. We don't care if it is successful, we
    // only do it for the sake of debugging output.
    if (!yyparser_.yy_lac_established_)
      yyparser_.yy_lac_check_ (yyla_.kind ());
#endif

    for (int yyx = 0; yyx < YYNTOKENS; ++yyx)
      {
        symbol_kind_type yysym = YY_CAST (symbol_kind_type, yyx);
        if (yysym != symbol_kind::S_YYerror
            && yysym != symbol_kind::S_YYUNDEF
            && yyparser_.yy_lac_check_ (yysym))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = yysym;
          }
      }
    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }




  bool
  parser::yy_lac_check_ (symbol_kind_type yytoken) const
  {
    // Logically, the yylac_stack's lifetime is confined to this function.
    // Clear it, to get rid of potential left-overs from previous call.
    yylac_stack_.clear ();
    // Reduce until we encounter a shift and thereby accept the token.
#if MNX2MNXSRDEBUG
    YYCDEBUG << "LAC: checking lookahead " << symbol_name (yytoken) << ':';
#endif
    std::ptrdiff_t lac_top = 0;
    while (true)
      {
        state_type top_state = (yylac_stack_.empty ()
                                ? yystack_[lac_top].state
                                : yylac_stack_.back ());
        int yyrule = yypact_[+top_state];
        if (yy_pact_value_is_default_ (yyrule)
            || (yyrule += yytoken) < 0 || yylast_ < yyrule
            || yycheck_[yyrule] != yytoken)
          {
            // Use the default action.
            yyrule = yydefact_[+top_state];
            if (yyrule == 0)
              {
                YYCDEBUG << " Err\n";
                return false;
              }
          }
        else
          {
            // Use the action from yytable.
            yyrule = yytable_[yyrule];
            if (yy_table_value_is_error_ (yyrule))
              {
                YYCDEBUG << " Err\n";
                return false;
              }
            if (0 < yyrule)
              {
                YYCDEBUG << " S" << yyrule << '\n';
                return true;
              }
            yyrule = -yyrule;
          }
        // By now we know we have to simulate a reduce.
        YYCDEBUG << " R" << yyrule - 1;
        // Pop the corresponding number of values from the stack.
        {
          std::ptrdiff_t yylen = yyr2_[yyrule];
          // First pop from the LAC stack as many tokens as possible.
          std::ptrdiff_t lac_size = std::ptrdiff_t (yylac_stack_.size ());
          if (yylen < lac_size)
            {
              yylac_stack_.resize (std::size_t (lac_size - yylen));
              yylen = 0;
            }
          else if (lac_size)
            {
              yylac_stack_.clear ();
              yylen -= lac_size;
            }
          // Only afterwards look at the main stack.
          // We simulate popping elements by incrementing lac_top.
          lac_top += yylen;
        }
        // Keep top_state in sync with the updated stack.
        top_state = (yylac_stack_.empty ()
                     ? yystack_[lac_top].state
                     : yylac_stack_.back ());
        // Push the resulting state of the reduction.
        state_type state = yy_lr_goto_state_ (top_state, yyr1_[yyrule]);
        YYCDEBUG << " G" << int (state);
        yylac_stack_.push_back (state);
      }
  }

  // Establish the initial context if no initial context currently exists.
  bool
  parser::yy_lac_establish_ (symbol_kind_type yytoken)
  {
    /* Establish the initial context for the current lookahead if no initial
       context is currently established.

       We define a context as a snapshot of the parser stacks.  We define
       the initial context for a lookahead as the context in which the
       parser initially examines that lookahead in order to select a
       syntactic action.  Thus, if the lookahead eventually proves
       syntactically unacceptable (possibly in a later context reached via a
       series of reductions), the initial context can be used to determine
       the exact set of tokens that would be syntactically acceptable in the
       lookahead's place.  Moreover, it is the context after which any
       further semantic actions would be erroneous because they would be
       determined by a syntactically unacceptable token.

       yy_lac_establish_ should be invoked when a reduction is about to be
       performed in an inconsistent state (which, for the purposes of LAC,
       includes consistent states that don't know they're consistent because
       their default reductions have been disabled).

       For parse.lac=full, the implementation of yy_lac_establish_ is as
       follows.  If no initial context is currently established for the
       current lookahead, then check if that lookahead can eventually be
       shifted if syntactic actions continue from the current context.  */
    if (yy_lac_established_)
      return true;
    else
      {
#if MNX2MNXSRDEBUG
        YYCDEBUG << "LAC: initial context established for "
                 << symbol_name (yytoken) << '\n';
#endif
        yy_lac_established_ = true;
        return yy_lac_check_ (yytoken);
      }
  }

  // Discard any previous initial lookahead context.
  void
  parser::yy_lac_discard_ (const char* event)
  {
   /* Discard any previous initial lookahead context because of Event,
      which may be a lookahead change or an invalidation of the currently
      established initial context for the current lookahead.

      The most common example of a lookahead change is a shift.  An example
      of both cases is syntax error recovery.  That is, a syntax error
      occurs when the lookahead is syntactically erroneous for the
      currently established initial context, so error recovery manipulates
      the parser stacks to try to find a new initial context in which the
      current lookahead is syntactically acceptable.  If it fails to find
      such a context, it discards the lookahead.  */
    if (yy_lac_established_)
      {
        YYCDEBUG << "LAC: initial context discarded due to "
                 << event << '\n';
        yy_lac_established_ = false;
      }
  }


  int
  parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
         In the first two cases, it might appear that the current syntax
         error should have been detected in the previous state when
         yy_lac_check was invoked.  However, at that time, there might
         have been a different syntax error that discarded a different
         initial context during error recovery, leaving behind the
         current lookahead.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -81;

  const signed char parser::yytable_ninf_ = -1;

  const signed char
  parser::yypact_[] =
  {
       7,     6,    13,    15,   -81,   -81,    31,    29,    11,    27,
      36,    35,    38,   -81,   -18,   -81,    39,    37,   -81,   -81,
     -81,    32,    34,    44,    43,    46,   -81,    45,   -81,    47,
      49,    48,    51,   -81,    17,    50,    32,    52,    53,    55,
       2,   -81,   -81,   -81,   -81,    56,   -81,    54,    57,   -81,
     -81,   -81,    58,    59,    61,     1,   -81,   -81,    62,    60,
      63,   -81,   -81,    33,   -81,    64,    65,    67,    -1,   -81,
      66,     4,     4,   -81,   -81,    68,    69,   -81,   -81,   -81,
      -4,     4,    70,    72,   -81,    71,   -81,    73,    74,    75,
      76,     0,   -81,    41,   -18,     4,   -81,   -81,    78,     5,
     -81,   -81,    77,     4,    79,   -81,   -81,   -81,    80,    82,
     -18,    83,    81,    85,     4,    -2,   -81
  };

  const signed char
  parser::yydefact_[] =
  {
       0,     0,     0,     0,     2,     1,     0,     0,     0,     0,
       0,     0,     0,     3,     0,    12,     0,     0,     6,     7,
      13,     0,     0,     0,     0,     0,     4,     0,    14,     0,
       0,     0,     0,     5,     0,     0,     0,     0,     0,     0,
       0,    19,    21,    22,    17,     0,    15,     0,     0,    18,
      20,    16,     0,     0,     0,     0,    26,    28,     0,     0,
       0,    25,    27,     0,    23,     0,     0,     0,     0,    30,
       0,     0,     0,    29,    31,     0,     0,     8,     9,    10,
       0,    33,     0,     0,    32,     0,    11,     0,     0,     0,
       0,     0,    35,     0,     0,     0,    34,    36,     0,     0,
      39,    41,     0,    37,     0,    38,    40,    24,     0,     0,
       0,     0,     0,     0,     0,     0,    42
  };

  const signed char
  parser::yypgoto_[] =
  {
     -81,   -81,   -81,   -81,   -81,   -80,   -79,   -72,   -81,   -81,
     -81,   -81,   -81,    40,   -81,   -81,    84,   -81,   -81,   -81,
     -81,    18,   -81,   -81,    24,   -81,   -81,   -81,   -19,   -81,
     -81,   -13,   -81
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,     2,     7,    17,    30,    20,    79,    80,     4,    11,
      13,    26,    37,    24,    35,    40,    41,    42,    59,    43,
      55,    56,    57,    68,    69,    70,    76,    91,    92,    84,
      99,   100,   101
  };

  const signed char
  parser::yytable_[] =
  {
      81,    86,    86,   116,    85,    96,    73,    49,    61,    18,
      19,     1,   105,     5,   102,     3,    38,    39,     6,    67,
      54,    10,    90,   103,    86,    77,    78,    77,    78,    98,
     111,    38,    39,    77,    78,     8,    86,     9,    12,    14,
      15,    16,   115,    21,    23,    22,    25,    27,    28,    29,
      66,    31,    34,    32,    33,    36,    47,    44,    48,    46,
      52,    51,    58,    53,    60,    98,    63,    64,    71,    65,
      72,    82,    97,    62,    87,    88,    45,    54,    94,    95,
      93,   104,   107,   108,    67,   110,   106,    75,   114,    89,
      83,   112,    74,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   109,     0,   113,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50
  };

  const signed char
  parser::yycheck_[] =
  {
      72,    80,    81,     5,     8,     5,     7,     5,     7,    27,
      28,     4,     7,     0,    94,     9,    14,    15,     3,    20,
      19,    10,    22,    95,   103,    29,    30,    29,    30,    24,
     110,    14,    15,    29,    30,     4,   115,     8,    11,     3,
       5,     3,   114,     4,    12,     8,    12,     3,     5,     3,
      17,     6,     4,     6,     5,     4,     3,     7,     3,     7,
       6,     5,     3,     6,     3,    24,     4,     7,     3,     6,
       3,     3,    91,    55,     4,     3,    36,    19,     3,     3,
       6,     3,     5,     4,    20,     3,    99,    21,     3,    18,
      21,     8,    68,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    40
  };

  const signed char
  parser::yystos_[] =
  {
       0,     4,    34,     9,    41,     0,     3,    35,     4,     8,
      10,    42,    11,    43,     3,     5,     3,    36,    27,    28,
      38,     4,     8,    12,    46,    12,    44,     3,     5,     3,
      37,     6,     6,     5,     4,    47,     4,    45,    14,    15,
      48,    49,    50,    52,     7,    46,     7,     3,     3,     5,
      49,     5,     6,     6,    19,    53,    54,    55,     3,    51,
       3,     7,    54,     4,     7,     6,    17,    20,    56,    57,
      58,     3,     3,     7,    57,    21,    59,    29,    30,    39,
      40,    40,     3,    21,    62,     8,    39,     4,     3,    18,
      22,    60,    61,     6,     3,     3,     5,    61,    24,    63,
      64,    65,    38,    40,     3,     7,    64,     5,     4,    25,
       3,    38,     8,    26,     3,    40,     5
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    33,    35,    36,    37,    34,    38,    38,    39,    39,
      40,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      48,    49,    49,    50,    51,    52,    53,    53,    54,    55,
      56,    56,    57,    58,    59,    60,    60,    61,    62,    63,
      63,    64,    65
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     0,     0,     0,    10,     1,     1,     1,     1,
       1,     2,     5,     3,     5,     5,     3,     5,     3,     1,
       2,     1,     1,     5,    10,     5,     1,     2,     1,     5,
       1,     2,     3,     3,     5,     1,     2,     3,     5,     1,
       2,     1,    11
  };




#if MNX2MNXSRDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   166,   166,   178,   190,   162,   206,   207,   215,   216,
     220,   221,   232,   242,   255,   269,   279,   293,   303,   313,
     314,   318,   319,   327,   337,   353,   363,   364,   368,   376,
     386,   387,   391,   401,   409,   419,   420,   424,   432,   442,
     443,   447,   451
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // MNX2MNXSRDEBUG


} // mnx2mnxsr
#line 1513 "mnx2mnxsrParser.cpp"

#line 461 "mnx2mnxsrParser.yy"

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
