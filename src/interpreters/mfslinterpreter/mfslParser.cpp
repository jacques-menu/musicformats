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
#line 65 "mfslParser.yy"


#include "mfslBasicTypes.h"

S_mfslChoice pCurrentChoiceChoice;

#line 97 "mfslParser.yy"

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
#line 62 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 380 "mfslParser.cpp"
        break;

      case symbol_kind::S_DOUBLE: // "double number"
#line 62 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 386 "mfslParser.cpp"
        break;

      case symbol_kind::S_SINGLE_QUOTED_STRING: // "single quoted_string"
#line 62 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 392 "mfslParser.cpp"
        break;

      case symbol_kind::S_DOUBLE_QUOTED_STRING: // "double quoted_string"
#line 62 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 398 "mfslParser.cpp"
        break;

      case symbol_kind::S_NAME: // "name"
#line 62 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 404 "mfslParser.cpp"
        break;

      case symbol_kind::S_OPTION: // "option"
#line 62 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 410 "mfslParser.cpp"
        break;

      case symbol_kind::S_Number: // Number
#line 62 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 416 "mfslParser.cpp"
        break;

      case symbol_kind::S_String: // String
#line 62 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 422 "mfslParser.cpp"
        break;

      case symbol_kind::S_OptionValue: // OptionValue
#line 62 "mfslParser.yy"
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
#line 147 "mfslParser.yy"
      {
        ++gIndenter;

        // create the main options block
        S_mfslOptionsBlock
          mainOptionsBlock =
            mfslOptionsBlock::create (
              "Main options block");

        // push it onto the stack
        drv.optionsBlocksStackPush (
          mainOptionsBlock,
          "Creation of the main options block");

        --gIndenter;
      }
#line 718 "mfslParser.cpp"
    break;

  case 3: // $@2: %empty
#line 166 "mfslParser.yy"
      {
        ++gIndenter;

        // DON'T pop the current current options block from the stack, JMI
        // it contains the consolidated options and values

        --gIndenter;
      }
#line 731 "mfslParser.cpp"
    break;

  case 5: // Number: "integer number"
#line 183 "mfslParser.yy"
    { yylhs.value.as < string > () = yystack_[0].value.as < string > (); }
#line 737 "mfslParser.cpp"
    break;

  case 6: // Number: "double number"
#line 184 "mfslParser.yy"
    { yylhs.value.as < string > () = yystack_[0].value.as < string > (); }
#line 743 "mfslParser.cpp"
    break;

  case 7: // String: "single quoted_string"
#line 192 "mfslParser.yy"
    { yylhs.value.as < string > () = yystack_[0].value.as < string > (); }
#line 749 "mfslParser.cpp"
    break;

  case 8: // String: "double quoted_string"
#line 193 "mfslParser.yy"
    { yylhs.value.as < string > () = yystack_[0].value.as < string > (); }
#line 755 "mfslParser.cpp"
    break;

  case 9: // Tool: "tool" ":" "name"
#line 202 "mfslParser.yy"
      {
        drv.setToolName (yystack_[0].value.as < string > ());
      }
#line 763 "mfslParser.cpp"
    break;

  case 10: // Input: "input" ":" "name"
#line 213 "mfslParser.yy"
      {
        drv.setInputSouceName (yystack_[0].value.as < string > ());
      }
#line 771 "mfslParser.cpp"
    break;

  case 11: // Input: "input" ":" String
#line 218 "mfslParser.yy"
      {
        drv.setInputSouceName (yystack_[0].value.as < string > ());
      }
#line 779 "mfslParser.cpp"
    break;

  case 19: // Option: "option"
#line 250 "mfslParser.yy"
      {
        ++gIndenter;

        if (drv.getDisplayOptions ()) {
          gLogStream <<
            "====> option " << yystack_[0].value.as < string > () <<
            ", line " << drv.getScannerLocation () <<
            endl;
        }

        drv.registerOptionInCurrentOptionsBlock (
          oahOption::create (yystack_[0].value.as < string > (), ""));

        --gIndenter;
      }
#line 799 "mfslParser.cpp"
    break;

  case 20: // Option: "option" OptionValue
#line 267 "mfslParser.yy"
      {
        ++gIndenter;

        if (drv.getDisplayOptions ()) {
          gLogStream <<
            "====> option " << yystack_[1].value.as < string > () << ' ' << yystack_[0].value.as < string > () <<
            ", line " << drv.getScannerLocation () <<
            endl;
        }

        drv.registerOptionInCurrentOptionsBlock (
          oahOption::create (yystack_[1].value.as < string > (), yystack_[0].value.as < string > ()));

        --gIndenter;
      }
#line 819 "mfslParser.cpp"
    break;

  case 21: // OptionValue: "name"
#line 285 "mfslParser.yy"
    { yylhs.value.as < string > () = yystack_[0].value.as < string > (); }
#line 825 "mfslParser.cpp"
    break;

  case 22: // OptionValue: Number
#line 286 "mfslParser.yy"
    { yylhs.value.as < string > () = yystack_[0].value.as < string > (); }
#line 831 "mfslParser.cpp"
    break;

  case 23: // OptionValue: String
#line 287 "mfslParser.yy"
    { yylhs.value.as < string > () = yystack_[0].value.as < string > (); }
#line 837 "mfslParser.cpp"
    break;

  case 24: // OptionValue: "name" "=" "name"
#line 289 "mfslParser.yy"
      {
        yylhs.value.as < string > () = yystack_[2].value.as < string > () + "=" + yystack_[0].value.as < string > ();
      }
#line 845 "mfslParser.cpp"
    break;

  case 25: // OptionValue: "name" ":" "name"
#line 293 "mfslParser.yy"
      {
        yylhs.value.as < string > () = yystack_[2].value.as < string > () + ":" + yystack_[0].value.as < string > ();
      }
#line 853 "mfslParser.cpp"
    break;

  case 26: // OptionValue: "name" "=" Number
#line 297 "mfslParser.yy"
      {
        yylhs.value.as < string > () = yystack_[2].value.as < string > () + "=" + yystack_[0].value.as < string > ();
      }
#line 861 "mfslParser.cpp"
    break;

  case 27: // OptionValue: "name" ":" Number
#line 301 "mfslParser.yy"
      {
        yylhs.value.as < string > () = yystack_[2].value.as < string > () + ":" + yystack_[0].value.as < string > ();
      }
#line 869 "mfslParser.cpp"
    break;

  case 28: // $@3: %empty
#line 312 "mfslParser.yy"
      {
        ++gIndenter;

        string choiceName = yystack_[1].value.as < string > ();

        if (drv.getTraceChoiceStatements ()) {
          gLogStream <<
            "====> choice " << choiceName << " : " << "..." <<
            ", line " << drv.getScannerLocation () <<
            endl;
        }

        // create a choice
        S_mfslChoice
          choice =
             mfslChoice::create (
               choiceName);

        // add it to the driver's choices table
        drv.getChoicesTable ()->
          addChoice (
            choice,
            drv);

        pCurrentChoiceChoice = choice;
      }
#line 900 "mfslParser.cpp"
    break;

  case 29: // $@4: %empty
#line 344 "mfslParser.yy"
      {
        string
          choiceName = yystack_[7].value.as < string > (),
          label      = yystack_[0].value.as < string > ();

        // fetch the voice in the choices table
        S_mfslChoice
          choice =
            drv.getChoicesTable ()->
              fetchChoiceByName (
                choiceName,
                drv);

        // register label as the default label in this choice
        choice->
          registerChoiceDefaultLabel (
            label,
            drv);
      }
#line 924 "mfslParser.cpp"
    break;

  case 30: // ChoiceDeclaration: "choice" "name" ":" $@3 ChoiceLabels "," "default" ":" "name" $@4 ";"
#line 365 "mfslParser.yy"
      {
        if (drv.getTraceChoiceStatements ()) {
          gLogStream <<
            "------------------------------------------------------------" <<
            endl;
        }

        --gIndenter;
      }
#line 938 "mfslParser.cpp"
    break;

  case 31: // ChoiceLabels: "name"
#line 378 "mfslParser.yy"
      {
        ++gIndenter;

        string label = yystack_[0].value.as < string > ();

        pCurrentChoiceChoice->
          addLabel (
            label,
            drv);

        --gIndenter;
      }
#line 955 "mfslParser.cpp"
    break;

  case 32: // ChoiceLabels: ChoiceLabels "|" "name"
#line 392 "mfslParser.yy"
      {
        ++gIndenter;

        string label = yystack_[0].value.as < string > ();

        pCurrentChoiceChoice->
          addLabel (
            label,
            drv);

        --gIndenter;
      }
#line 972 "mfslParser.cpp"
    break;

  case 33: // CaseLabel: "name"
#line 412 "mfslParser.yy"
      {
        ++gIndenter;

        string label = yystack_[0].value.as < string > ();

        // fetch case statement stack top
        S_mfslCaseStatement
          currentCaseStatement =
            drv.caseStatementsStackTop ();

        // register this case label
        currentCaseStatement->
          registerCaseLabel (
            label,
            drv);

        --gIndenter;
      }
#line 995 "mfslParser.cpp"
    break;

  case 36: // $@5: %empty
#line 439 "mfslParser.yy"
      {
        ++gIndenter;

        string choiceName = yystack_[1].value.as < string > ();

        if (drv.getTraceCaseStatements ()) {
          gLogStream <<
            "====> case " << choiceName << ": ..." <<
            ", line " << drv.getScannerLocation () <<
            endl;
        }

        // create a new current case statement
        S_mfslChoicesTable
          choicesTable =
            drv.getChoicesTable ();

        S_mfslChoice
          choice =
            choicesTable->
              fetchChoiceByName (
                choiceName,
                drv);

        if (! choice) {
          stringstream s;

          s <<
            "name \"" << choiceName <<
            "\" is no choice name, cannot be used in a 'select' statement" <<
            ", line " << drv.getScannerLocation () <<
            endl;

          mfslError (
            s.str (),
            drv.getScannerLocation ());
        }

        S_mfslCaseStatement
          caseStatement =
            mfslCaseStatement::create (
              choice);

        // push it onto the case statements stack
        drv.caseStatementsStackPush (
          caseStatement);

        // mark the choice as used
        choice->
          setChoiceIsUsedInCaseStatements ();
      }
#line 1051 "mfslParser.cpp"
    break;

  case 37: // CaseStatement: "case" "name" ":" $@5 OptionalCaseAlternativesSeq ";"
#line 492 "mfslParser.yy"
      {
        // have all the label been used as labels?
        drv.caseStatementsStackTop ()->
          checkThatAllLabelsHaveBeenUsed (drv);

        // pop the current case statement from the case statements stack
        drv.caseStatementsStackPop ();

        if (drv.getTraceCaseStatements ()) {
          gLogStream <<
            "------------------------------------------------------------" <<
            endl;
        }

        --gIndenter;
      }
#line 1072 "mfslParser.cpp"
    break;

  case 42: // $@6: %empty
#line 523 "mfslParser.yy"
      {
        ++gIndenter;

        S_mfslCaseStatement
          currentCaseStatement =
            drv.caseStatementsStackTop ();

        // forget about previous case alternative if any
        currentCaseStatement->
          clearCaseCurrentLabelsList ();
      }
#line 1088 "mfslParser.cpp"
    break;

  case 43: // $@7: %empty
#line 536 "mfslParser.yy"
      {
        S_mfslCaseStatement
          currentCaseStatement =
            drv.caseStatementsStackTop ();

        // push a new current options block onto the stack
        stringstream s;

        s <<
          "Case alternative for " <<
          currentCaseStatement->
            currentLabelsListAsString () <<
          ", line " << drv.getScannerLocation ();

        string
          caseAlternativeDescription =
            s.str ();

        drv.optionsBlocksStackPush (
          mfslOptionsBlock::create (
            caseAlternativeDescription),
          caseAlternativeDescription);
      }
#line 1116 "mfslParser.cpp"
    break;

  case 44: // CaseAlternative: $@6 CaseLabelsSeq ":" $@7 OptionalScriptElementsSeq ";"
#line 561 "mfslParser.yy"
      {
        S_mfslCaseStatement
          currentCaseStatement =
            drv.caseStatementsStackTop ();

        S_mfslChoice
          currentCaseChoice =
            currentCaseStatement->
              getCaseChoice ();

        // handle the labels
        for (string label : currentCaseStatement->getCaseCurrentLabelsList ()) {
          // enrich the options block for label
          currentCaseChoice->
            enrichLabelOptionsBlock (
              label,
              drv.optionsBlocksStackTop ());
        } // for

        // discard this case alternative
        stringstream s;

        s <<
          "Discarding case alternative options block for " <<
          currentCaseStatement->
            currentLabelsListAsString () <<
          ", line " << drv.getScannerLocation () <<
          endl;

        string context = s.str ();

        drv.optionsBlocksStackPop (
          context);

        --gIndenter;
      }
#line 1157 "mfslParser.cpp"
    break;

  case 48: // SelectStatement: "select" "name" ":" "name" ";"
#line 611 "mfslParser.yy"
      {
        string
          choiceName = yystack_[3].value.as < string > (),
          label = yystack_[1].value.as < string > ();

        drv.setSelectLabelForToolLaunching (
          choiceName,
          label);
      }
#line 1171 "mfslParser.cpp"
    break;

  case 49: // AllStatement: "all" "name" ";"
#line 623 "mfslParser.yy"
      {
        string
          choiceName = yystack_[1].value.as < string > ();

        drv.setAllChoicesOptionsBlockForToolLaunching (
          choiceName);
      }
#line 1183 "mfslParser.cpp"
    break;


#line 1187 "mfslParser.cpp"

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
    "end of file", "error", "invalid token", "|", "=", ";", ":", ",",
  "tool", "input", "choice", "default", "case", "select", "all",
  "integer number", "double number", "single quoted_string",
  "double quoted_string", "name", "option", "$accept", "Script", "$@1",
  "$@2", "Number", "String", "Tool", "Input", "OptionalScriptElementsSeq",
  "ScriptElementsSeq", "ScriptElement", "Option", "OptionValue",
  "ChoiceDeclaration", "$@3", "$@4", "ChoiceLabels", "CaseLabel",
  "CaseLabelsSeq", "CaseStatement", "$@5", "OptionalCaseAlternativesSeq",
  "CaseAlternativesSeq", "CaseAlternative", "$@6", "$@7",
  "OptionalSelectOrAllStatement", "SelectStatement", "AllStatement", YY_NULLPTR
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


  const signed char parser::yypact_ninf_ = -20;

  const signed char parser::yytable_ninf_ = -43;

  const signed char
  parser::yypact_[] =
  {
      14,    -5,    16,    21,    12,   -20,    26,   -20,   -20,     1,
     -10,   -20,   -20,   -20,   -20,    15,    18,   -12,   -20,   -10,
     -20,   -20,   -20,   -20,    27,    29,   -20,   -20,    17,   -20,
     -20,   -20,    11,   -20,   -20,   -20,    -7,    -2,    19,    22,
     -20,   -20,   -20,    23,    24,   -20,   -20,   -20,   -20,    30,
      34,   -20,     8,    35,    24,   -20,    25,    28,   -20,    31,
      37,   -20,   -20,   -20,   -20,    20,    40,   -20,    43,   -20,
      25,   -20,    32,   -10,   -20,   -20,    41,    47,   -20,   -20
  };

  const signed char
  parser::yydefact_[] =
  {
       0,     0,     0,     0,     0,     1,     0,     2,     9,     0,
      13,     7,     8,    10,    11,     0,     0,    19,     3,    12,
      14,    16,    17,    18,     0,     0,     5,     6,    21,    22,
      23,    20,    47,    15,    28,    36,     0,     0,     0,     0,
       4,    45,    46,     0,    39,    24,    26,    25,    27,     0,
       0,    31,     0,     0,    38,    40,     0,     0,    49,     0,
       0,    37,    41,    33,    34,     0,     0,    32,     0,    43,
       0,    48,     0,    13,    35,    29,     0,     0,    44,    30
  };

  const signed char
  parser::yypgoto_[] =
  {
     -20,   -20,   -20,   -20,    -8,    36,   -20,   -20,   -19,   -20,
      38,   -20,   -20,   -20,   -20,   -20,   -20,   -15,   -20,   -20,
     -20,   -20,   -20,     2,   -20,   -20,   -20,   -20,   -20
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,     2,    10,    32,    29,    14,     3,     7,    18,    19,
      20,    21,    31,    22,    43,    77,    52,    64,    65,    23,
      44,    53,    54,    55,    56,    73,    40,    41,    42
  };

  const signed char
  parser::yytable_[] =
  {
      15,     4,    16,    26,    27,    11,    12,    28,    26,    27,
      17,    59,    45,    26,    27,    60,     5,    47,    11,    12,
      13,    36,     1,    37,    38,    39,    69,    70,    46,    48,
       6,     8,     9,    34,    24,    35,    57,    25,    49,    58,
      61,    50,    51,   -42,    63,    71,    78,    66,    68,    72,
      67,    75,    79,    30,    76,    74,    62,    33
  };

  const signed char
  parser::yycheck_[] =
  {
      10,     6,    12,    15,    16,    17,    18,    19,    15,    16,
      20,     3,    19,    15,    16,     7,     0,    19,    17,    18,
      19,     4,     8,     6,    13,    14,     6,     7,    36,    37,
       9,    19,     6,     6,    19,     6,     6,    19,    19,     5,
       5,    19,    19,    19,    19,     5,     5,    19,    11,     6,
      19,    19,     5,    17,    73,    70,    54,    19
  };

  const signed char
  parser::yystos_[] =
  {
       0,     8,    22,    27,     6,     0,     9,    28,    19,     6,
      23,    17,    18,    19,    26,    10,    12,    20,    29,    30,
      31,    32,    34,    40,    19,    19,    15,    16,    19,    25,
      26,    33,    24,    31,     6,     6,     4,     6,    13,    14,
      47,    48,    49,    35,    41,    19,    25,    19,    25,    19,
      19,    19,    37,    42,    43,    44,    45,     6,     5,     3,
       7,     5,    44,    19,    38,    39,    19,    19,    11,     6,
       7,     5,     6,    46,    38,    19,    29,    36,     5,     5
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    21,    23,    24,    22,    25,    25,    26,    26,    27,
      28,    28,    29,    29,    30,    30,    31,    31,    31,    32,
      32,    33,    33,    33,    33,    33,    33,    33,    35,    36,
      34,    37,    37,    38,    39,    39,    41,    40,    42,    42,
      43,    43,    45,    46,    44,    47,    47,    47,    48,    49
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     0,     0,     6,     1,     1,     1,     1,     3,
       3,     3,     1,     0,     1,     2,     1,     1,     1,     1,
       2,     1,     1,     1,     3,     3,     3,     3,     0,     0,
      11,     1,     3,     1,     1,     3,     0,     6,     1,     0,
       1,     2,     0,     0,     6,     1,     1,     0,     5,     3
  };




#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   147,   147,   166,   143,   183,   184,   192,   193,   201,
     212,   217,   228,   229,   233,   235,   239,   240,   241,   249,
     266,   285,   286,   287,   288,   292,   296,   300,   312,   344,
     311,   377,   391,   411,   433,   434,   439,   438,   511,   512,
     516,   518,   523,   536,   523,   604,   605,   606,   610,   622
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
#line 1793 "mfslParser.cpp"

#line 633 "mfslParser.yy"

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
