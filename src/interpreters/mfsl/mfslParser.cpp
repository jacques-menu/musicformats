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



// First part of user prologue.
#line 1 "mfslParser.yy"


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

#include "oahBasicTypes.h"

#include "mfslWae.h"


using namespace MusicFormats;


#line 70 "mfslParser.cpp"


#include "mfslParser.h"


// Unqualified %code blocks.
#line 64 "mfslParser.yy"


#include "mfslBasicTypes.h"

S_mfslVariable pCurrentChoiceVariable;

#line 100 "mfslParser.yy"

# include "mfslDriver.h"

#line 88 "mfslParser.cpp"


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
#if YYDEBUG

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

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 180 "mfslParser.cpp"

  /// Build a parser object.
  parser::parser (mfslDriver& drv_yyarg)
#if YYDEBUG
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
      case symbol_kind::S_String: // String
      case symbol_kind::S_OptionValue: // OptionValue
        value.YY_MOVE_OR_COPY< string > (YY_MOVE (that.value));
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
      case symbol_kind::S_String: // String
      case symbol_kind::S_OptionValue: // OptionValue
        value.move< string > (YY_MOVE (that.value));
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
      case symbol_kind::S_String: // String
      case symbol_kind::S_OptionValue: // OptionValue
        value.copy< string > (that.value);
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
      case symbol_kind::S_String: // String
      case symbol_kind::S_OptionValue: // OptionValue
        value.move< string > (that.value);
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

#if YYDEBUG
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
#line 61 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 380 "mfslParser.cpp"
        break;

      case symbol_kind::S_DOUBLE: // "double number"
#line 61 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 386 "mfslParser.cpp"
        break;

      case symbol_kind::S_SINGLE_QUOTED_STRING: // "single quoted_string"
#line 61 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 392 "mfslParser.cpp"
        break;

      case symbol_kind::S_DOUBLE_QUOTED_STRING: // "double quoted_string"
#line 61 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 398 "mfslParser.cpp"
        break;

      case symbol_kind::S_NAME: // "name"
#line 61 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 404 "mfslParser.cpp"
        break;

      case symbol_kind::S_OPTION: // "option"
#line 61 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 410 "mfslParser.cpp"
        break;

      case symbol_kind::S_Number: // Number
#line 61 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 416 "mfslParser.cpp"
        break;

      case symbol_kind::S_String: // String
#line 61 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 422 "mfslParser.cpp"
        break;

      case symbol_kind::S_OptionValue: // OptionValue
#line 61 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 428 "mfslParser.cpp"
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

#if YYDEBUG
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
#endif // YYDEBUG

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
    int yynerrs_ = 0;
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
      case symbol_kind::S_String: // String
      case symbol_kind::S_OptionValue: // OptionValue
        yylhs.value.emplace< string > ();
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
#line 145 "mfslParser.yy"
      {
        // push a new current scope onto the stack
        drv.pushScopeOntoStack (
          mfslScope::create ("Main scope"));
      }
#line 707 "mfslParser.cpp"
    break;

  case 3: // Script: $@1 Tool Input ScriptElementsSequence
#line 156 "mfslParser.yy"
      {
        // DON'T pop the current current scope from the stack,
        // it contains the consolidated options and values
      }
#line 716 "mfslParser.cpp"
    break;

  case 4: // Number: "integer number"
#line 167 "mfslParser.yy"
    { yylhs.value.as < string > () = yystack_[0].value.as < string > (); }
#line 722 "mfslParser.cpp"
    break;

  case 5: // Number: "double number"
#line 168 "mfslParser.yy"
    { yylhs.value.as < string > () = yystack_[0].value.as < string > (); }
#line 728 "mfslParser.cpp"
    break;

  case 6: // String: "single quoted_string"
#line 176 "mfslParser.yy"
    { yylhs.value.as < string > () = yystack_[0].value.as < string > (); }
#line 734 "mfslParser.cpp"
    break;

  case 7: // String: "double quoted_string"
#line 177 "mfslParser.yy"
    { yylhs.value.as < string > () = yystack_[0].value.as < string > (); }
#line 740 "mfslParser.cpp"
    break;

  case 8: // Tool: "tool" ":" "name"
#line 186 "mfslParser.yy"
      {
        drv.setToolName (yystack_[0].value.as < string > ());
      }
#line 748 "mfslParser.cpp"
    break;

  case 9: // Input: "input" ":" "name"
#line 197 "mfslParser.yy"
      {
        drv.setInputFileName (yystack_[0].value.as < string > ());
      }
#line 756 "mfslParser.cpp"
    break;

  case 10: // Input: "input" ":" String
#line 202 "mfslParser.yy"
      {
        drv.setInputFileName (yystack_[0].value.as < string > ());
     }
#line 764 "mfslParser.cpp"
    break;

  case 18: // Option: "option"
#line 238 "mfslParser.yy"
      {
        if (drv.getDisplayNonTerminals ()) {
          gLogStream <<
            "====> option " << yystack_[0].value.as < string > () <<
            endl;
        }

        drv.registerOptionInCurrentScope (
          oahOption::create (yystack_[0].value.as < string > (), ""));
      }
#line 779 "mfslParser.cpp"
    break;

  case 19: // Option: "option" OptionValue
#line 250 "mfslParser.yy"
      {
        if (drv.getDisplayNonTerminals ()) {
          gLogStream <<
            "====> option " << yystack_[1].value.as < string > () << ' ' << yystack_[0].value.as < string > () <<
            endl;
        }

        drv.registerOptionInCurrentScope (
          oahOption::create (yystack_[1].value.as < string > (), yystack_[0].value.as < string > ()));
      }
#line 794 "mfslParser.cpp"
    break;

  case 20: // OptionValue: "name"
#line 263 "mfslParser.yy"
    { yylhs.value.as < string > () = yystack_[0].value.as < string > (); }
#line 800 "mfslParser.cpp"
    break;

  case 21: // OptionValue: Number
#line 264 "mfslParser.yy"
    { yylhs.value.as < string > () = yystack_[0].value.as < string > (); }
#line 806 "mfslParser.cpp"
    break;

  case 22: // OptionValue: String
#line 265 "mfslParser.yy"
    { yylhs.value.as < string > () = yystack_[0].value.as < string > (); }
#line 812 "mfslParser.cpp"
    break;

  case 23: // $@2: %empty
#line 274 "mfslParser.yy"
      {
        if (drv.getTraceSemantics ()) {
          gLogStream <<
            "====> ChoiceDeclaration " << yystack_[1].value.as < string > () << " : " << "..." <<
            endl;
        }

        pCurrentChoiceVariable =
          drv.getVariablesTable ()->
            createAndInsertVariable (yystack_[1].value.as < string > ());
      }
#line 828 "mfslParser.cpp"
    break;

  case 24: // ChoiceDeclaration: "choice" "name" ":" $@2 ChoicesPossibleValues ";"
#line 287 "mfslParser.yy"
      {
      }
#line 835 "mfslParser.cpp"
    break;

  case 25: // ChoicesPossibleValues: "name"
#line 293 "mfslParser.yy"
      {
        pCurrentChoiceVariable->
          addPossibleValue (yystack_[0].value.as < string > ());
      }
#line 844 "mfslParser.cpp"
    break;

  case 26: // ChoicesPossibleValues: ChoicesPossibleValues "|" "name"
#line 299 "mfslParser.yy"
      {
        pCurrentChoiceVariable->
          addPossibleValue (yystack_[0].value.as < string > ());
      }
#line 853 "mfslParser.cpp"
    break;

  case 27: // $@3: %empty
#line 308 "mfslParser.yy"
      {
        if (drv.getTraceSemantics ()) {
          gLogStream <<
            "====> ChoiceSetting, set " << yystack_[1].value.as < string > () << " = " << "..." <<
            endl;
        }

        pCurrentChoiceVariable =
          drv.getVariablesTable () ->
            lookupVariableByName (yystack_[1].value.as < string > ());
      }
#line 869 "mfslParser.cpp"
    break;

  case 28: // ChoiceSetting: "set" "name" "=" $@3 "name" ";"
#line 321 "mfslParser.yy"
      {
        pCurrentChoiceVariable->
          setVariableValue (yystack_[1].value.as < string > ());
      }
#line 878 "mfslParser.cpp"
    break;

  case 29: // $@4: %empty
#line 333 "mfslParser.yy"
      {
        if (drv.getTraceSemantics ()) {
          gLogStream <<
            "====> CaseStatement: " << yystack_[1].value.as < string > () << ":" << " ..." <<
            endl;
        }

        // push a new current case statement
        drv.pushCaseStatementOntoStack (
          mfslCaseStatement::create (
            drv.getVariablesTable () ->
              checkCaseVariableByName (yystack_[1].value.as < string > ())));
//
//         // push a new current scope onto the stack
//         string caseScopeName = "Case " + $2;
//
//         drv.pushScopeOntoStack (
//           mfslScope::create (caseScopeName));
      }
#line 902 "mfslParser.cpp"
    break;

  case 30: // CaseStatement: "case" "name" ":" $@4 CaseAlternativesSequence ";"
#line 354 "mfslParser.yy"
      {
        // have all the possible value been used as labels?
        drv.topOfCaseStatementStack ()->
          checkThatAllPossibleValuesHaveBeenUsed ();

        // pop the current scorpe from the stack
        drv.popScopeFromStack ();

        // pop the current case statement from the stack
        drv.popCaseStatementFromStack ();
      }
#line 918 "mfslParser.cpp"
    break;

  case 33: // $@5: %empty
#line 375 "mfslParser.yy"
      {
        if (drv.getTraceSemantics ()) {
          gLogStream <<
            "====> CaseAlternative \"" <<
            yystack_[1].value.as < string > () <<
            endl;
        }

        // push a new current options names and values
        // for this case alternative
//         drv.registerOptionInCurrentScope (
//           oahOptionsVector::create ());
      }
#line 936 "mfslParser.cpp"
    break;

  case 34: // CaseAlternative: "name" ":" $@5 ScriptElementsSequence ";"
#line 390 "mfslParser.yy"
      {
        string label = yystack_[4].value.as < string > ();

        S_mfslCaseStatement
          currentCaseStatement =
            drv.topOfCaseStatementStack ();

        S_mfslVariable
          currentCaseVariable =
            currentCaseStatement->
              getCaseVariable ();

        // register this case label value
        currentCaseStatement->
          registerCaseLabelValue (label);

        // is label equal to case variable value?
        if (currentCaseVariable->getVariableValue () == label) {
          // this is the case alternative to keep
          if (drv.getTraceSemantics ()) {
            gLogStream <<
              "====> Keeping caseAlternative \"" <<
              label <<
              "\", optionsVector: " <<
              endl << endl;
          }

//           drv.registerOptionInCurrentScope (
//             optionsVector);
        }

        else {
          // discard this case alternative
          if (drv.getTraceSemantics ()) {
            gLogStream <<
              "====> Discarding caseAlternative \"" <<
              label <<
              "\", optionsVector: " <<
              endl << endl;
          }
        }

        // pop the current options names and values from the stack
//         drv.popOptionsFromStack ();
      }
#line 986 "mfslParser.cpp"
    break;

  case 35: // $@6: %empty
#line 443 "mfslParser.yy"
      {
        if (drv.getTraceSemantics ()) {
          gLogStream <<
            "====> AllStatement: " << yystack_[1].value.as < string > () << ":" << " ..." <<
            endl;
        }

        // push a new current case statement
//         drv.appendAllStatementToList (
//           mfslAllStatement::create (
//             drv.getVariablesTable () ->
//               checkAllVariableByName ($2)));

//         // push a new current scope onto the stack
//         string caseScopeName = "All " + $2;
//
//         drv.pushScopeOntoStack (
//           mfslScope::create (caseScopeName));
      }
#line 1010 "mfslParser.cpp"
    break;

  case 36: // AllStatement: "all" "name" ":" $@6 AllAlternativesSequence ";"
#line 464 "mfslParser.yy"
      {
        // have all the possible value been used as labels?
//         drv.topOfAllStatementsList ()->
//           checkThatAllPossibleValuesHaveBeenUsed ();

        // pop the current scorpe from the stack
        drv.popScopeFromStack ();

//         // pop the current case statement from the stack
//         drv.popAllStatementFromList ();
      }
#line 1026 "mfslParser.cpp"
    break;

  case 39: // $@7: %empty
#line 485 "mfslParser.yy"
      {
        if (drv.getTraceSemantics ()) {
          gLogStream <<
            "====> AllAlternative \"" <<
            yystack_[1].value.as < string > () <<
            endl;
        }

        // push a new current options names and values
        // for this case alternative
//         drv.registerOptionInCurrentScope (
//           oahOptionsVector::create ());
      }
#line 1044 "mfslParser.cpp"
    break;

  case 40: // AllAlternative: "name" ":" $@7 ScriptElementsSequence ";"
#line 500 "mfslParser.yy"
      {
        string label = yystack_[4].value.as < string > ();

        S_mfslAllStatement
          currentAllStatement = nullptr; // JMI
//             drv.topOfAllStatementsList ();

        S_mfslVariable
          currentAllVariable =
            currentAllStatement->
              getAllVariable ();

        // register this case label value
        currentAllStatement->
          registerAllLabelValue (label);

        // is label equal to case variable value?
        if (currentAllVariable->getVariableValue () == label) {
          // this is the case alternative to keep
          if (drv.getTraceSemantics ()) {
            gLogStream <<
              "====> Keeping caseAlternative \"" <<
              label <<
              "\", optionsVector: " <<
              endl << endl;
          }

//           drv.registerOptionInCurrentScope (
//             optionsVector);
        }

        else {
          // discard this case alternative
          if (drv.getTraceSemantics ()) {
            gLogStream <<
              "====> Discarding caseAlternative \"" <<
              label <<
              "\", optionsVector: " <<
              endl << endl;
          }
        }

        // pop the current options names and values from the stack
//         drv.popOptionsFromStack ();
      }
#line 1094 "mfslParser.cpp"
    break;


#line 1098 "mfslParser.cpp"

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
        ++yynerrs_;
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
    "end of file", "error", "invalid token", "|", ";", ":", "=", "/", ",",
  "*", "(", ")", "tool", "input", "choice", "set", "case", "all",
  "integer number", "double number", "single quoted_string",
  "double quoted_string", "name", "option", "$accept", "Script", "$@1",
  "Number", "String", "Tool", "Input", "ScriptElementsSequence",
  "ScriptElement", "Option", "OptionValue", "ChoiceDeclaration", "$@2",
  "ChoicesPossibleValues", "ChoiceSetting", "$@3", "CaseStatement", "$@4",
  "CaseAlternativesSequence", "CaseAlternative", "$@5", "AllStatement",
  "$@6", "AllAlternativesSequence", "AllAlternative", "$@7", YY_NULLPTR
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

#if YYDEBUG
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
#if YYDEBUG
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
#if YYDEBUG
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


  const signed char parser::yypact_ninf_ = -31;

  const signed char parser::yytable_ninf_ = -1;

  const signed char
  parser::yypact_[] =
  {
     -31,     2,     7,   -31,    18,    24,    16,    34,    11,   -31,
       9,    19,    20,    21,    22,   -12,    11,   -31,   -31,   -31,
     -31,   -31,   -31,   -31,   -31,   -31,   -31,    35,    39,    41,
      42,   -31,   -31,   -31,   -31,   -31,   -31,   -31,   -31,   -31,
     -31,   -31,    26,    27,    28,    31,   -31,    29,    50,    51,
      -1,   -31,    52,     0,   -31,    33,   -31,   -31,   -31,   -31,
     -31,   -31,   -31,   -31,   -31,    11,    11,    -3,     1,   -31,
     -31
  };

  const signed char
  parser::yydefact_[] =
  {
       2,     0,     0,     1,     0,     0,     0,     0,     0,     8,
       0,     0,     0,     0,     0,    18,     3,    11,    13,    14,
      15,    16,    17,     6,     7,     9,    10,     0,     0,     0,
       0,     4,     5,    20,    21,    22,    19,    12,    23,    27,
      29,    35,     0,     0,     0,     0,    25,     0,     0,     0,
       0,    31,     0,     0,    37,     0,    24,    28,    33,    30,
      32,    39,    36,    38,    26,     0,     0,     0,     0,    34,
      40
  };

  const signed char
  parser::yypgoto_[] =
  {
     -31,   -31,   -31,   -31,    43,   -31,   -31,   -30,   -16,   -31,
     -31,   -31,   -31,   -31,   -31,   -31,   -31,   -31,   -31,    10,
     -31,   -31,   -31,   -31,     6,   -31
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,     1,     2,    34,    26,     5,     8,    16,    17,    18,
      36,    19,    42,    47,    20,    43,    21,    44,    50,    51,
      65,    22,    45,    53,    54,    66
  };

  const signed char
  parser::yytable_[] =
  {
      37,    69,     3,    59,    62,    70,    31,    32,    23,    24,
      33,    11,    12,    13,    14,    11,    12,    13,    14,     4,
      15,    49,    52,     6,    15,    11,    12,    13,    14,    23,
      24,    25,    55,    56,    15,    67,    68,     7,     9,    10,
      38,    27,    28,    29,    30,    39,    40,    41,    46,    48,
      49,    37,    37,    52,    57,    64,    58,    61,    35,    63,
      60
  };

  const signed char
  parser::yycheck_[] =
  {
      16,     4,     0,     4,     4,     4,    18,    19,    20,    21,
      22,    14,    15,    16,    17,    14,    15,    16,    17,    12,
      23,    22,    22,     5,    23,    14,    15,    16,    17,    20,
      21,    22,     3,     4,    23,    65,    66,    13,    22,     5,
       5,    22,    22,    22,    22,     6,     5,     5,    22,    22,
      22,    67,    68,    22,     4,    22,     5,     5,    15,    53,
      50
  };

  const signed char
  parser::yystos_[] =
  {
       0,    25,    26,     0,    12,    29,     5,    13,    30,    22,
       5,    14,    15,    16,    17,    23,    31,    32,    33,    35,
      38,    40,    45,    20,    21,    22,    28,    22,    22,    22,
      22,    18,    19,    22,    27,    28,    34,    32,     5,     6,
       5,     5,    36,    39,    41,    46,    22,    37,    22,    22,
      42,    43,    22,    47,    48,     3,     4,     4,     5,     4,
      43,     5,     4,    48,    22,    44,    49,    31,    31,     4,
       4
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    24,    26,    25,    27,    27,    28,    28,    29,    30,
      30,    31,    31,    32,    32,    32,    32,    32,    33,    33,
      34,    34,    34,    36,    35,    37,    37,    39,    38,    41,
      40,    42,    42,    44,    43,    46,    45,    47,    47,    49,
      48
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     0,     4,     1,     1,     1,     1,     3,     3,
       3,     1,     2,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     0,     6,     1,     3,     0,     6,     0,
       6,     1,     2,     0,     5,     0,     6,     1,     2,     0,
       5
  };




#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   145,   145,   145,   167,   168,   176,   177,   185,   196,
     201,   219,   221,   225,   226,   227,   228,   229,   237,   249,
     263,   264,   265,   274,   273,   292,   298,   308,   307,   333,
     332,   368,   370,   375,   374,   443,   442,   478,   480,   485,
     484
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
#endif // YYDEBUG


} // yy
#line 1705 "mfslParser.cpp"

#line 550 "mfslParser.yy"

//_______________________________________________________________________________


// other service code
//_______________________________________________________________________________


void
yy::parser::error (const location_type& loc, const string& message)
{
  mfslError (
    message,
    loc);
}
