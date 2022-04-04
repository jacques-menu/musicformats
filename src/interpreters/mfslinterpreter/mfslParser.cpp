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
      case symbol_kind::S_SingleString: // SingleString
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
      case symbol_kind::S_SingleString: // SingleString
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
      case symbol_kind::S_SingleString: // SingleString
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
      case symbol_kind::S_SingleString: // SingleString
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
#line 384 "mfslParser.cpp"
        break;

      case symbol_kind::S_DOUBLE: // "double number"
#line 62 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 390 "mfslParser.cpp"
        break;

      case symbol_kind::S_SINGLE_QUOTED_STRING: // "single quoted_string"
#line 62 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 396 "mfslParser.cpp"
        break;

      case symbol_kind::S_DOUBLE_QUOTED_STRING: // "double quoted_string"
#line 62 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 402 "mfslParser.cpp"
        break;

      case symbol_kind::S_NAME: // "name"
#line 62 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 408 "mfslParser.cpp"
        break;

      case symbol_kind::S_OPTION: // "option"
#line 62 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 414 "mfslParser.cpp"
        break;

      case symbol_kind::S_Number: // Number
#line 62 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 420 "mfslParser.cpp"
        break;

      case symbol_kind::S_SingleString: // SingleString
#line 62 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 426 "mfslParser.cpp"
        break;

      case symbol_kind::S_String: // String
#line 62 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 432 "mfslParser.cpp"
        break;

      case symbol_kind::S_OptionValue: // OptionValue
#line 62 "mfslParser.yy"
                 { yyo << yysym.value.template as < string > (); }
#line 438 "mfslParser.cpp"
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
      case symbol_kind::S_SingleString: // SingleString
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
#line 149 "mfslParser.yy"
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
#line 729 "mfslParser.cpp"
    break;

  case 3: // $@2: %empty
#line 168 "mfslParser.yy"
      {
        ++gIndenter;

        // DON'T pop the current current options block from the stack, JMI
        // it contains the consolidated options and values

        --gIndenter;
      }
#line 742 "mfslParser.cpp"
    break;

  case 5: // Number: "integer number"
#line 185 "mfslParser.yy"
    { yylhs.value.as < string > () = yystack_[0].value.as < string > (); }
#line 748 "mfslParser.cpp"
    break;

  case 6: // Number: "double number"
#line 186 "mfslParser.yy"
    { yylhs.value.as < string > () = yystack_[0].value.as < string > (); }
#line 754 "mfslParser.cpp"
    break;

  case 7: // SingleString: "single quoted_string"
#line 194 "mfslParser.yy"
    { yylhs.value.as < string > () = yystack_[0].value.as < string > (); }
#line 760 "mfslParser.cpp"
    break;

  case 8: // SingleString: "double quoted_string"
#line 195 "mfslParser.yy"
    { yylhs.value.as < string > () = yystack_[0].value.as < string > (); }
#line 766 "mfslParser.cpp"
    break;

  case 9: // String: SingleString
#line 199 "mfslParser.yy"
    { yylhs.value.as < string > () = yystack_[0].value.as < string > (); }
#line 772 "mfslParser.cpp"
    break;

  case 10: // String: String SingleString
#line 201 "mfslParser.yy"
      {
        yylhs.value.as < string > () = yystack_[1].value.as < string > () + yystack_[0].value.as < string > ();
      }
#line 780 "mfslParser.cpp"
    break;

  case 11: // Tool: "tool" ":" "name"
#line 212 "mfslParser.yy"
      {
        drv.setToolName (yystack_[0].value.as < string > ());
      }
#line 788 "mfslParser.cpp"
    break;

  case 12: // Input: "input" ":" "name"
#line 223 "mfslParser.yy"
      {
        drv.setInputSouceName (yystack_[0].value.as < string > ());
      }
#line 796 "mfslParser.cpp"
    break;

  case 13: // Input: "input" ":" String
#line 228 "mfslParser.yy"
      {
        drv.setInputSouceName (yystack_[0].value.as < string > ());
      }
#line 804 "mfslParser.cpp"
    break;

  case 21: // Option: "option"
#line 260 "mfslParser.yy"
      {
        ++gIndenter;

        if (drv.getDisplayOptions ()) {
          gLogStream <<
            "====> option " << yystack_[0].value.as < string > () <<
            ", line " << drv.getScannerLocation () <<
            endl;
        }

        drv.registerOptionInCurrentOptionsBlock (
          oahOption::create (yystack_[0].value.as < string > (), ""),
          drv);

        --gIndenter;
      }
#line 825 "mfslParser.cpp"
    break;

  case 22: // Option: "option" OptionValue
#line 278 "mfslParser.yy"
      {
        ++gIndenter;

        if (drv.getDisplayOptions ()) {
          gLogStream <<
            "====> option " << yystack_[1].value.as < string > () << ' ' << yystack_[0].value.as < string > () <<
            ", line " << drv.getScannerLocation () <<
            endl;
        }

        drv.registerOptionInCurrentOptionsBlock (
          oahOption::create (yystack_[1].value.as < string > (), yystack_[0].value.as < string > ()),
          drv);

        --gIndenter;
      }
#line 846 "mfslParser.cpp"
    break;

  case 23: // OptionValue: "name"
#line 297 "mfslParser.yy"
    { yylhs.value.as < string > () = yystack_[0].value.as < string > (); }
#line 852 "mfslParser.cpp"
    break;

  case 24: // OptionValue: Number
#line 298 "mfslParser.yy"
    { yylhs.value.as < string > () = yystack_[0].value.as < string > (); }
#line 858 "mfslParser.cpp"
    break;

  case 25: // OptionValue: String
#line 299 "mfslParser.yy"
    { yylhs.value.as < string > () = yystack_[0].value.as < string > (); }
#line 864 "mfslParser.cpp"
    break;

  case 26: // OptionValue: "name" "=" "name"
#line 301 "mfslParser.yy"
      {
        yylhs.value.as < string > () = yystack_[2].value.as < string > () + "=" + yystack_[0].value.as < string > ();
      }
#line 872 "mfslParser.cpp"
    break;

  case 27: // OptionValue: "name" ":" "name"
#line 305 "mfslParser.yy"
      {
        yylhs.value.as < string > () = yystack_[2].value.as < string > () + ":" + yystack_[0].value.as < string > ();
      }
#line 880 "mfslParser.cpp"
    break;

  case 28: // OptionValue: "name" "=" Number
#line 309 "mfslParser.yy"
      {
        yylhs.value.as < string > () = yystack_[2].value.as < string > () + "=" + yystack_[0].value.as < string > ();
      }
#line 888 "mfslParser.cpp"
    break;

  case 29: // OptionValue: "name" ":" Number
#line 313 "mfslParser.yy"
      {
        yylhs.value.as < string > () = yystack_[2].value.as < string > () + ":" + yystack_[0].value.as < string > ();
      }
#line 896 "mfslParser.cpp"
    break;

  case 30: // $@3: %empty
#line 324 "mfslParser.yy"
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
#line 927 "mfslParser.cpp"
    break;

  case 31: // $@4: %empty
#line 356 "mfslParser.yy"
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
#line 951 "mfslParser.cpp"
    break;

  case 32: // ChoiceDeclaration: "choice" "name" ":" $@3 ChoiceLabels "," "default" ":" "name" $@4 ";"
#line 377 "mfslParser.yy"
      {
        if (drv.getTraceChoiceStatements ()) {
          gLogStream <<
            "------------------------------------------------------------" <<
            endl;
        }

        --gIndenter;
      }
#line 965 "mfslParser.cpp"
    break;

  case 33: // ChoiceLabels: "name"
#line 390 "mfslParser.yy"
      {
        ++gIndenter;

        string label = yystack_[0].value.as < string > ();

        pCurrentChoiceChoice->
          addLabel (
            label,
            drv);

        --gIndenter;
      }
#line 982 "mfslParser.cpp"
    break;

  case 34: // ChoiceLabels: ChoiceLabels "|" "name"
#line 404 "mfslParser.yy"
      {
        ++gIndenter;

        string label = yystack_[0].value.as < string > ();

        pCurrentChoiceChoice->
          addLabel (
            label,
            drv);

        --gIndenter;
      }
#line 999 "mfslParser.cpp"
    break;

  case 35: // CaseLabel: "name"
#line 424 "mfslParser.yy"
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
#line 1022 "mfslParser.cpp"
    break;

  case 38: // $@5: %empty
#line 451 "mfslParser.yy"
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
              choice,
              drv);

        // push it onto the case statements stack
        drv.caseStatementsStackPush (
          caseStatement);

        // mark the choice as used
        choice->
          setChoiceIsUsedInCaseStatements ();
      }
#line 1079 "mfslParser.cpp"
    break;

  case 39: // CaseStatement: "case" "name" ":" $@5 OptionalCaseAlternativesSeq ";"
#line 505 "mfslParser.yy"
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
#line 1100 "mfslParser.cpp"
    break;

  case 44: // $@6: %empty
#line 536 "mfslParser.yy"
      {
        ++gIndenter;

        S_mfslCaseStatement
          currentCaseStatement =
            drv.caseStatementsStackTop ();

        // forget about previous case alternative if any
        currentCaseStatement->
          clearCaseCurrentLabelsList ();
      }
#line 1116 "mfslParser.cpp"
    break;

  case 45: // $@7: %empty
#line 549 "mfslParser.yy"
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

        S_mfslOptionsBlock
          caseAlternativeOptionsBlock =
            mfslOptionsBlock::create (
              caseAlternativeDescription);

        drv.optionsBlocksStackPush (
          caseAlternativeOptionsBlock,
          caseAlternativeDescription);
      }
#line 1148 "mfslParser.cpp"
    break;

  case 46: // CaseAlternative: $@6 CaseLabelsSeq ":" $@7 OptionalScriptElementsSeq ";"
#line 578 "mfslParser.yy"
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
              drv.optionsBlocksStackTop (),
              drv);
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
#line 1190 "mfslParser.cpp"
    break;

  case 50: // SelectStatement: "select" "name" ":" "name" ";"
#line 629 "mfslParser.yy"
      {
        string
          choiceName = yystack_[3].value.as < string > (),
          label = yystack_[1].value.as < string > ();

        drv.setSelectLabelForToolLaunching (
          choiceName,
          label);
      }
#line 1204 "mfslParser.cpp"
    break;

  case 51: // EveryStatement: "every" "name" ";"
#line 641 "mfslParser.yy"
      {
        string
          choiceName = yystack_[1].value.as < string > ();

        drv.setEveryChoiceForToolLaunching (
          choiceName);
      }
#line 1216 "mfslParser.cpp"
    break;


#line 1220 "mfslParser.cpp"

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
  "tool", "input", "choice", "default", "case", "select", "every",
  "integer number", "double number", "single quoted_string",
  "double quoted_string", "name", "option", "$accept", "Script", "$@1",
  "$@2", "Number", "SingleString", "String", "Tool", "Input",
  "OptionalScriptElementsSeq", "ScriptElementsSeq", "ScriptElement",
  "Option", "OptionValue", "ChoiceDeclaration", "$@3", "$@4",
  "ChoiceLabels", "CaseLabel", "CaseLabelsSeq", "CaseStatement", "$@5",
  "OptionalCaseAlternativesSeq", "CaseAlternativesSeq", "CaseAlternative",
  "$@6", "$@7", "OptionalSelectOrEveryStatement", "SelectStatement",
  "EveryStatement", YY_NULLPTR
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


  const signed char parser::yypact_ninf_ = -19;

  const signed char parser::yytable_ninf_ = -45;

  const signed char
  parser::yypact_[] =
  {
      15,    -4,    33,    25,    16,   -19,    30,   -19,   -19,     2,
      -9,   -19,   -19,   -19,   -19,     8,    19,    20,   -11,   -19,
      -9,   -19,   -19,   -19,   -19,   -19,    31,    34,   -19,   -19,
      18,   -19,     8,   -19,    14,   -19,   -19,   -19,    -6,    -1,
      22,    24,   -19,   -19,   -19,    26,    27,   -19,   -19,   -19,
     -19,    36,    39,   -19,     9,    42,    27,   -19,    29,    32,
     -19,    35,    38,   -19,   -19,   -19,   -19,    23,    45,   -19,
      46,   -19,    29,   -19,    37,    -9,   -19,   -19,    48,    50,
     -19,   -19
  };

  const signed char
  parser::yydefact_[] =
  {
       0,     0,     0,     0,     0,     1,     0,     2,    11,     0,
      15,     7,     8,    12,     9,    13,     0,     0,    21,     3,
      14,    16,    18,    19,    20,    10,     0,     0,     5,     6,
      23,    24,    25,    22,    49,    17,    30,    38,     0,     0,
       0,     0,     4,    47,    48,     0,    41,    26,    28,    27,
      29,     0,     0,    33,     0,     0,    40,    42,     0,     0,
      51,     0,     0,    39,    43,    35,    36,     0,     0,    34,
       0,    45,     0,    50,     0,    15,    37,    31,     0,     0,
      46,    32
  };

  const signed char
  parser::yypgoto_[] =
  {
     -19,   -19,   -19,   -19,    -7,   -15,    40,   -19,   -19,   -18,
     -19,    41,   -19,   -19,   -19,   -19,   -19,   -19,   -13,   -19,
     -19,   -19,   -19,   -19,     4,   -19,   -19,   -19,   -19,   -19
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,     2,    10,    34,    31,    14,    15,     3,     7,    19,
      20,    21,    22,    33,    23,    45,    79,    54,    66,    67,
      24,    46,    55,    56,    57,    58,    75,    42,    43,    44
  };

  const signed char
  parser::yytable_[] =
  {
      25,    16,     4,    17,    28,    29,    11,    12,    30,    28,
      29,    18,    61,    47,    28,    29,    62,    25,    49,    11,
      12,    13,    38,     1,    39,    11,    12,    40,    41,    71,
      72,    48,    50,     5,     6,     8,     9,    36,    26,    27,
      37,    51,    59,    52,    60,    53,   -44,    63,    65,    70,
      73,    68,    74,    80,    69,    81,    77,    78,    32,    76,
      64,    35
  };

  const signed char
  parser::yycheck_[] =
  {
      15,    10,     6,    12,    15,    16,    17,    18,    19,    15,
      16,    20,     3,    19,    15,    16,     7,    32,    19,    17,
      18,    19,     4,     8,     6,    17,    18,    13,    14,     6,
       7,    38,    39,     0,     9,    19,     6,     6,    19,    19,
       6,    19,     6,    19,     5,    19,    19,     5,    19,    11,
       5,    19,     6,     5,    19,     5,    19,    75,    18,    72,
      56,    20
  };

  const signed char
  parser::yystos_[] =
  {
       0,     8,    22,    28,     6,     0,     9,    29,    19,     6,
      23,    17,    18,    19,    26,    27,    10,    12,    20,    30,
      31,    32,    33,    35,    41,    26,    19,    19,    15,    16,
      19,    25,    27,    34,    24,    32,     6,     6,     4,     6,
      13,    14,    48,    49,    50,    36,    42,    19,    25,    19,
      25,    19,    19,    19,    38,    43,    44,    45,    46,     6,
       5,     3,     7,     5,    45,    19,    39,    40,    19,    19,
      11,     6,     7,     5,     6,    47,    39,    19,    30,    37,
       5,     5
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    21,    23,    24,    22,    25,    25,    26,    26,    27,
      27,    28,    29,    29,    30,    30,    31,    31,    32,    32,
      32,    33,    33,    34,    34,    34,    34,    34,    34,    34,
      36,    37,    35,    38,    38,    39,    40,    40,    42,    41,
      43,    43,    44,    44,    46,    47,    45,    48,    48,    48,
      49,    50
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     0,     0,     6,     1,     1,     1,     1,     1,
       2,     3,     3,     3,     1,     0,     1,     2,     1,     1,
       1,     1,     2,     1,     1,     1,     3,     3,     3,     3,
       0,     0,    11,     1,     3,     1,     1,     3,     0,     6,
       1,     0,     1,     2,     0,     0,     6,     1,     1,     0,
       5,     3
  };




#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   149,   149,   168,   145,   185,   186,   194,   195,   199,
     200,   211,   222,   227,   238,   239,   243,   245,   249,   250,
     251,   259,   277,   297,   298,   299,   300,   304,   308,   312,
     324,   356,   323,   389,   403,   423,   445,   446,   451,   450,
     524,   525,   529,   531,   536,   549,   536,   622,   623,   624,
     628,   640
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
#line 1835 "mfslParser.cpp"

#line 651 "mfslParser.yy"

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
