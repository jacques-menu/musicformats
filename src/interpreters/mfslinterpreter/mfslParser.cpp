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

// As a special std::exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special std::exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special std::exception.

// This special std::exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.


// Take the name prefix into account.
#define yylex   mfsllex

// First part of user prologue.
#line 1 "mfslParser.yy"


/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/


#include <iostream>

#include <string>

#include "mfIndentedTextOutput.h"

#include "oahBasicTypes.h"

#include "mfslWae.h"


using namespace MusicFormats;


#line 72 "mfslParser.cpp"


#include "mfslParser.h"


// Unqualified %code blocks.
#line 68 "mfslParser.yy"

  #include "mfslBasicTypes.h"
#line 98 "mfslParser.yy"

  #include "mfslDriver.h"

#line 86 "mfslParser.cpp"


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


// Whether we are compiled with std::exception support.
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
#if MFSLDEBUG

// A pseudo std::ostream that takes yydebug_ into account.
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

#else // !MFSLDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !MFSLDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace mfsl {
#line 178 "mfslParser.cpp"

  /// Build a parser object.
  parser::parser (mfslDriver& drv_yyarg)
#if MFSLDEBUG
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
      case symbol_kind::S_String: // std::string
      case symbol_kind::S_OptionValue: // OptionValue
      case symbol_kind::S_LabelName: // LabelName
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
      case symbol_kind::S_String: // std::string
      case symbol_kind::S_OptionValue: // OptionValue
      case symbol_kind::S_LabelName: // LabelName
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
      case symbol_kind::S_String: // std::string
      case symbol_kind::S_OptionValue: // OptionValue
      case symbol_kind::S_LabelName: // LabelName
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
      case symbol_kind::S_String: // std::string
      case symbol_kind::S_OptionValue: // OptionValue
      case symbol_kind::S_LabelName: // LabelName
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

#if MFSLDEBUG
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
        yyo << (yykind < YYNTOKENS ? "token": "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        switch (yykind)
    {
      case symbol_kind::S_INTEGER: // "integer number"
#line 65 "mfslParser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 386 "mfslParser.cpp"
        break;

      case symbol_kind::S_DOUBLE: // "double number"
#line 65 "mfslParser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 392 "mfslParser.cpp"
        break;

      case symbol_kind::S_SINGLE_QUOTED_STRING: // "single quoted_string"
#line 65 "mfslParser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 398 "mfslParser.cpp"
        break;

      case symbol_kind::S_DOUBLE_QUOTED_STRING: // "double quoted_string"
#line 65 "mfslParser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 404 "mfslParser.cpp"
        break;

      case symbol_kind::S_NAME: // "name"
#line 65 "mfslParser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 410 "mfslParser.cpp"
        break;

      case symbol_kind::S_OPTION: // "option"
#line 65 "mfslParser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 416 "mfslParser.cpp"
        break;

      case symbol_kind::S_Number: // Number
#line 65 "mfslParser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 422 "mfslParser.cpp"
        break;

      case symbol_kind::S_SingleString: // SingleString
#line 65 "mfslParser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 428 "mfslParser.cpp"
        break;

      case symbol_kind::S_String: // std::string
#line 65 "mfslParser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 434 "mfslParser.cpp"
        break;

      case symbol_kind::S_OptionValue: // OptionValue
#line 65 "mfslParser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 440 "mfslParser.cpp"
        break;

      case symbol_kind::S_LabelName: // LabelName
#line 65 "mfslParser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 446 "mfslParser.cpp"
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

#if MFSLDEBUG
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
#endif // MFSLDEBUG

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
            YYCDEBUG << "Caught std::exception: " << yyexc.what() << '\n';
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
      case symbol_kind::S_String: // std::string
      case symbol_kind::S_OptionValue: // OptionValue
      case symbol_kind::S_LabelName: // LabelName
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
#line 152 "mfslParser.yy"
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
#line 738 "mfslParser.cpp"
    break;

  case 3: // $@2: %empty
#line 171 "mfslParser.yy"
      {
        ++gIndenter;

        // DON'T pop the current current options block from the stack, JMI
        // it contains the consolidated options and values

        --gIndenter;
      }
#line 751 "mfslParser.cpp"
    break;

  case 5: // Number: "integer number"
#line 188 "mfslParser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 757 "mfslParser.cpp"
    break;

  case 6: // Number: "double number"
#line 189 "mfslParser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 763 "mfslParser.cpp"
    break;

  case 7: // SingleString: "single quoted_string"
#line 197 "mfslParser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 769 "mfslParser.cpp"
    break;

  case 8: // SingleString: "double quoted_string"
#line 198 "mfslParser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 775 "mfslParser.cpp"
    break;

  case 9: // std::string: SingleString
#line 202 "mfslParser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 781 "mfslParser.cpp"
    break;

  case 10: // std::string: std::string SingleString
#line 204 "mfslParser.yy"
      {
        yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + yystack_[0].value.as < std::string > ();
      }
#line 789 "mfslParser.cpp"
    break;

  case 11: // Tool: "tool" ":" "name" ";"
#line 215 "mfslParser.yy"
      {
        drv.setTool (yystack_[1].value.as < std::string > ());
      }
#line 797 "mfslParser.cpp"
    break;

  case 15: // InputSource: "name"
#line 235 "mfslParser.yy"
      {
        drv.appendInputSouce (yystack_[0].value.as < std::string > ());
      }
#line 805 "mfslParser.cpp"
    break;

  case 16: // InputSource: std::string
#line 240 "mfslParser.yy"
      {
        drv.appendInputSouce (yystack_[0].value.as < std::string > ());
      }
#line 813 "mfslParser.cpp"
    break;

  case 25: // Option: "option"
#line 273 "mfslParser.yy"
      {
        ++gIndenter;

        if (drv.getDisplayOptions ()) {
          gLogStream <<
            "====> option " << yystack_[0].value.as < std::string > () <<
            ", line " << drv.getScannerLocation () <<
            std::endl;
        }

        drv.registerOptionInCurrentOptionsBlock (
          oahOption::create (yystack_[0].value.as < std::string > (), ""),
          drv);

        --gIndenter;
      }
#line 834 "mfslParser.cpp"
    break;

  case 26: // Option: "option" OptionValue
#line 291 "mfslParser.yy"
      {
        ++gIndenter;

        if (drv.getDisplayOptions ()) {
          gLogStream <<
            "====> option " << yystack_[1].value.as < std::string > () << ' ' << yystack_[0].value.as < std::string > () <<
            ", line " << drv.getScannerLocation () <<
            std::endl;
        }

        drv.registerOptionInCurrentOptionsBlock (
          oahOption::create (yystack_[1].value.as < std::string > (), yystack_[0].value.as < std::string > ()),
          drv);

        --gIndenter;
      }
#line 855 "mfslParser.cpp"
    break;

  case 27: // OptionValue: "name"
#line 310 "mfslParser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 861 "mfslParser.cpp"
    break;

  case 28: // OptionValue: Number
#line 311 "mfslParser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 867 "mfslParser.cpp"
    break;

  case 29: // OptionValue: std::string
#line 312 "mfslParser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 873 "mfslParser.cpp"
    break;

  case 30: // OptionValue: "name" "=" "name"
#line 314 "mfslParser.yy"
      {
        yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + "=" + yystack_[0].value.as < std::string > ();
      }
#line 881 "mfslParser.cpp"
    break;

  case 31: // OptionValue: "name" ":" "name"
#line 318 "mfslParser.yy"
      {
        yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + ":" + yystack_[0].value.as < std::string > ();
      }
#line 889 "mfslParser.cpp"
    break;

  case 32: // OptionValue: "name" "=" Number
#line 322 "mfslParser.yy"
      {
        yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + "=" + yystack_[0].value.as < std::string > ();
      }
#line 897 "mfslParser.cpp"
    break;

  case 33: // OptionValue: "name" ":" Number
#line 326 "mfslParser.yy"
      {
        yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + ":" + yystack_[0].value.as < std::string > ();
      }
#line 905 "mfslParser.cpp"
    break;

  case 34: // $@3: %empty
#line 337 "mfslParser.yy"
      {
        ++gIndenter;

        std::string choiceName = yystack_[1].value.as < std::string > ();

        if (drv.getTraceCaseChoiceStatements ()) {
          gLogStream <<
            "====> choice " << choiceName << ": " << "..." <<
            ", line " << drv.getScannerLocation () <<
            std::endl;
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

        drv.setCurrentChoiceChoice (choice);
      }
#line 936 "mfslParser.cpp"
    break;

  case 35: // $@4: %empty
#line 369 "mfslParser.yy"
      {
        std::string
          choiceName = yystack_[7].value.as < std::string > (),
          label      = yystack_[0].value.as < std::string > ();

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
#line 960 "mfslParser.cpp"
    break;

  case 36: // ChoiceDeclaration: "choice" "name" ":" $@3 ChoiceLabels "," "default" ":" "name" $@4 ";"
#line 390 "mfslParser.yy"
      {
        if (drv.getTraceCaseChoiceStatements ()) {
          gLogStream <<
            "------------------------------------------------------------" <<
            std::endl;
        }

        --gIndenter;
      }
#line 974 "mfslParser.cpp"
    break;

  case 37: // ChoiceLabels: "name"
#line 403 "mfslParser.yy"
      {
        ++gIndenter;

        std::string label = yystack_[0].value.as < std::string > ();

        drv.getCurrentChoiceChoice ()->
          addLabel (
            label,
            drv);

        --gIndenter;
      }
#line 991 "mfslParser.cpp"
    break;

  case 38: // ChoiceLabels: ChoiceLabels "|" "name"
#line 417 "mfslParser.yy"
      {
        ++gIndenter;

        std::string label = yystack_[0].value.as < std::string > ();

        drv.getCurrentChoiceChoice ()->
          addLabel (
            label,
            drv);

        --gIndenter;
      }
#line 1008 "mfslParser.cpp"
    break;

  case 39: // CaseChoiceLabel: "name"
#line 437 "mfslParser.yy"
      {
        ++gIndenter;

        std::string label = yystack_[0].value.as < std::string > ();

        // fetch case statement stack top
        S_mfslCaseChoiceStatement
          currentCaseChoiceStatement =
            drv.caseChoiceStatementsStackTop ();

        // register this case label
        currentCaseChoiceStatement->
          registerCaseChoiceLabel (
            label,
            drv);

        --gIndenter;
      }
#line 1031 "mfslParser.cpp"
    break;

  case 42: // $@5: %empty
#line 464 "mfslParser.yy"
      {
        ++gIndenter;

        std::string choiceName = yystack_[1].value.as < std::string > ();

        if (drv.getTraceCaseChoiceStatements ()) {
          gLogStream <<
            "====> case " << choiceName << ": ..." <<
            ", line " << drv.getScannerLocation () <<
            std::endl;
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
          std::stringstream s;

          s <<
            "name \"" << choiceName <<
            "\" is no choice name, cannot be used in a 'select' statement" <<
            ", line " << drv.getScannerLocation () <<
            std::endl;

          mfslError (
            s.str (),
            drv.getScannerLocation ());
        }

        S_mfslCaseChoiceStatement
          caseChoiceStatement =
            mfslCaseChoiceStatement::create (
              choice,
              drv);

        // push it onto the case statements stack
        drv.caseChoiceStatementsStackPush (
          caseChoiceStatement);

        // mark the choice as used
        choice->
          setChoiceIsUsedInCaseChoiceStatements ();
      }
#line 1088 "mfslParser.cpp"
    break;

  case 43: // CaseChoiceStatement: "case" "name" ":" $@5 OptionalCaseChoiceAlternativesSeq ";"
#line 518 "mfslParser.yy"
      {
        // have all the label been used as labels?
        drv.caseChoiceStatementsStackTop ()->
          checkThatAllLabelsHaveBeenUsed (drv);

        // pop the current case statement from the case statements stack
        drv.caseChoiceStatementsStackPop ();

        if (drv.getTraceCaseChoiceStatements ()) {
          gLogStream <<
            "------------------------------------------------------------" <<
            std::endl;
        }

        --gIndenter;
      }
#line 1109 "mfslParser.cpp"
    break;

  case 48: // $@6: %empty
#line 549 "mfslParser.yy"
      {
        ++gIndenter;

        S_mfslCaseChoiceStatement
          currentCaseChoiceStatement =
            drv.caseChoiceStatementsStackTop ();

        // forget about previous case alternative if any
        currentCaseChoiceStatement->
          clearCaseCurrentLabelsList ();
      }
#line 1125 "mfslParser.cpp"
    break;

  case 49: // $@7: %empty
#line 562 "mfslParser.yy"
      {
        S_mfslCaseChoiceStatement
          currentCaseChoiceStatement =
            drv.caseChoiceStatementsStackTop ();

        // push a new current options block onto the stack
        std::stringstream s;

        s <<
          "Case alternative for " <<
          currentCaseChoiceStatement->
            currentLabelsListAsString () <<
          ", line " << drv.getScannerLocation ();

        std::string
          CaseChoiceAlternativeDescription =
            s.str ();

        S_mfslOptionsBlock
          CaseChoiceAlternativeOptionsBlock =
            mfslOptionsBlock::create (
              CaseChoiceAlternativeDescription);

        drv.optionsBlocksStackPush (
          CaseChoiceAlternativeOptionsBlock,
          CaseChoiceAlternativeDescription);
      }
#line 1157 "mfslParser.cpp"
    break;

  case 50: // CaseChoiceAlternative: $@6 CaseChoiceLabelsSeq ":" $@7 OptionalScriptElementsSeq ";"
#line 591 "mfslParser.yy"
      {
        S_mfslCaseChoiceStatement
          currentCaseChoiceStatement =
            drv.caseChoiceStatementsStackTop ();

        S_mfslChoice
          currentCaseChoice =
            currentCaseChoiceStatement->
              getCaseChoice ();

        // handle the labels
        for (std::string label : currentCaseChoiceStatement->getCaseCurrentLabelsList ()) {
          // enrich the options block for label
          currentCaseChoice->
            enrichLabelOptionsBlock (
              label,
              drv.optionsBlocksStackTop (),
              drv);
        } // for

        // discard this case alternative
        std::stringstream s;

        s <<
          "Discarding case alternative options block for " <<
          currentCaseChoiceStatement->
            currentLabelsListAsString () <<
          ", line " << drv.getScannerLocation () <<
          std::endl;

        std::string context = s.str ();

        drv.optionsBlocksStackPop (
          context);

        --gIndenter;
      }
#line 1199 "mfslParser.cpp"
    break;

  case 51: // CaseInputName: "name"
#line 636 "mfslParser.yy"
      {
        ++gIndenter;

        std::string label = yystack_[0].value.as < std::string > ();

        // fetch case input statement stack top
        S_mfslCaseInputStatement
          currentCaseInputStatement =
            drv.caseInputStatementsStackTop ();

        // register this case input label
        currentCaseInputStatement->
          registerCaseInputName (
            label,
            drv);

        --gIndenter;
      }
#line 1222 "mfslParser.cpp"
    break;

  case 54: // $@8: %empty
#line 663 "mfslParser.yy"
      {
        ++gIndenter;

        std::string inputName = "$2 INPUT";

        if (drv.getTraceCaseInputStatements ()) {
          gLogStream <<
            "====> case input " << inputName << ": ..." <<
            ", line " << drv.getScannerLocation () <<
            std::endl;
        }

        // create a new current case input statement
        S_mfslInputsTable
          inputsTable =
            drv.getInputsTable ();

        S_mfslInput
          input =
            inputsTable->
              fetchInputByName (
                inputName,
                drv);

        if (! input) {
          std::stringstream s;

          s <<
            "name \"" << inputName <<
            "\" is no input name, cannot be used in a 'select' statement" <<
            ", line " << drv.getScannerLocation () <<
            std::endl;

          mfslError (
            s.str (),
            drv.getScannerLocation ());
        }

        S_mfslCaseInputStatement
          caseInputStatement =
            mfslCaseInputStatement::create (
              input,
              drv);

        // push it onto the case input statements stack
        drv.caseInputStatementsStackPush (
          caseInputStatement);

        // mark the input as used
        input->
          setInputIsUsedInCaseInputStatements ();
      }
#line 1279 "mfslParser.cpp"
    break;

  case 55: // CaseInputStatement: "case" "input" ":" $@8 OptionalCaseInputAlternativesSeq ";"
#line 717 "mfslParser.yy"
      {
        // have all the name been used as names?
        drv.caseInputStatementsStackTop ()->
          clearCaseInputCurrentNamesList (); // drv ??? JMI

        // pop the current case input statement from the case input statements stack
        drv.caseInputStatementsStackPop ();

        if (drv.getTraceCaseInputStatements ()) {
          gLogStream <<
            "------------------------------------------------------------" <<
            std::endl;
        }

        --gIndenter;
      }
#line 1300 "mfslParser.cpp"
    break;

  case 60: // $@9: %empty
#line 748 "mfslParser.yy"
      {
        ++gIndenter;

        S_mfslCaseInputStatement
          currentCaseInputStatement =
            drv.caseInputStatementsStackTop ();

        // forget about previous case input alternative if any
        currentCaseInputStatement->
          clearCaseInputCurrentNamesList ();
      }
#line 1316 "mfslParser.cpp"
    break;

  case 61: // $@10: %empty
#line 761 "mfslParser.yy"
      {
        S_mfslCaseInputStatement
          currentCaseInputStatement =
            drv.caseInputStatementsStackTop ();

        // push a new current options block onto the stack
        std::stringstream s;

        s <<
          "CaseInput alternative for " <<
          currentCaseInputStatement->
            currentNamesListAsString () <<
          ", line " << drv.getScannerLocation ();

        std::string
          caseInputAlternativeDescription =
            s.str ();

        S_mfslOptionsBlock
          caseInputAlternativeOptionsBlock =
            mfslOptionsBlock::create (
              caseInputAlternativeDescription);

        drv.optionsBlocksStackPush (
          caseInputAlternativeOptionsBlock,
          caseInputAlternativeDescription);
      }
#line 1348 "mfslParser.cpp"
    break;

  case 62: // CaseInputAlternative: $@9 CaseInputNamesSeq ":" $@10 OptionalScriptElementsSeq ";"
#line 790 "mfslParser.yy"
      {
        S_mfslCaseInputStatement
          currentCaseInputStatement =
            drv.caseInputStatementsStackTop ();

        S_mfslInput
          currentCaseInputInput =
            currentCaseInputStatement->
              getCaseInputInput ();

        // handle the names
        for (std::string name : currentCaseInputStatement->getCaseInputCurrentNamesList ()) {
          // enrich the options block for name
          currentCaseInputInput->
            enrichNameOptionsBlock (
              name,
              drv.optionsBlocksStackTop (),
              drv);
        } // for

        // discard this case input alternative
        std::stringstream s;

        s <<
          "Discarding case input alternative options block for " <<
          currentCaseInputStatement->
            currentNamesListAsString () <<
          ", line " << drv.getScannerLocation () <<
          std::endl;

        std::string context = s.str ();

        drv.optionsBlocksStackPop (
          context);

        --gIndenter;
      }
#line 1390 "mfslParser.cpp"
    break;

  case 67: // LabelName: "name"
#line 844 "mfslParser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1396 "mfslParser.cpp"
    break;

  case 68: // LabelName: "all"
#line 846 "mfslParser.yy"
      { yylhs.value.as < std::string > () = mfslDriver::K_ALL_PSEUDO_LABEL_NAME; }
#line 1402 "mfslParser.cpp"
    break;

  case 69: // SelectStatement: "select" "name" ":" LabelName ";"
#line 851 "mfslParser.yy"
      {
        std::string
          choiceName = yystack_[3].value.as < std::string > (),
          label = yystack_[1].value.as < std::string > ();

        drv.handleSelectLabel (
          choiceName,
          label);
      }
#line 1416 "mfslParser.cpp"
    break;


#line 1420 "mfslParser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught std::exception: " << yyexc.what() << '\n';
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
        YYCDEBUG << "std::exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an std::exception.
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
    "end of file", "error", "invalid token", "|", "&", "=", ";", ":", ",",
  "tool", "input", "choice", "default", "case", "select", "all",
  "integer number", "double number", "single quoted_string",
  "double quoted_string", "name", "option", "$accept", "Script", "$@1",
  "$@2", "Number", "SingleString", "std::string", "Tool", "Input",
  "InputSourcesSeq", "InputSource", "OptionalScriptElementsSeq",
  "ScriptElementsSeq", "ScriptElement", "Option", "OptionValue",
  "ChoiceDeclaration", "$@3", "$@4", "ChoiceLabels", "CaseChoiceLabel",
  "CaseChoiceLabelsSeq", "CaseChoiceStatement", "$@5",
  "OptionalCaseChoiceAlternativesSeq", "CaseChoiceAlternativesSeq",
  "CaseChoiceAlternative", "$@6", "$@7", "CaseInputName",
  "CaseInputNamesSeq", "CaseInputStatement", "$@8",
  "OptionalCaseInputAlternativesSeq", "CaseInputAlternativesSeq",
  "CaseInputAlternative", "$@9", "$@10", "OptionalSelectStatements",
  "SelectStatementSeq", "LabelName", "SelectStatement", YY_NULLPTR
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

#if MFSLDEBUG
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
#if MFSLDEBUG
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
#if MFSLDEBUG
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


  const signed char parser::yypact_ninf_ = -65;

  const signed char parser::yytable_ninf_ = -61;

  const signed char
  parser::yypact_[] =
  {
      10,     3,    22,    33,     7,   -65,    37,   -65,    39,   -13,
     -10,   -65,   -65,   -65,   -65,   -65,    17,    24,   -65,    27,
      -8,    -2,   -65,   -10,   -65,   -65,   -65,   -65,   -65,   -65,
     -13,   -65,    41,    42,    43,   -65,   -65,    26,   -65,    17,
     -65,    38,   -65,   -65,   -65,   -65,   -65,     4,     9,    31,
     -65,    38,   -65,    34,    35,    36,   -65,   -65,   -65,   -65,
      46,   -65,   -65,     1,    40,    35,   -65,    44,    51,    36,
     -65,    45,    -7,    47,    48,   -65,   -65,   -65,   -65,    30,
     -65,   -65,   -65,   -65,    32,   -65,   -65,    52,   -65,    54,
     -65,    44,   -65,    45,   -65,    49,   -10,   -65,   -10,   -65,
     -65,    53,    56,    57,   -65,   -65,   -65
  };

  const signed char
  parser::yydefact_[] =
  {
       0,     0,     0,     0,     0,     1,     0,     2,     0,     0,
      18,    11,     7,     8,    15,     9,    16,     0,    13,     0,
       0,    25,     3,    17,    19,    21,    22,    23,    24,    10,
       0,    12,     0,     0,     0,     5,     6,    27,    28,    29,
      26,    64,    20,    14,    34,    54,    42,     0,     0,     0,
       4,    63,    65,     0,    57,    45,    30,    32,    31,    33,
       0,    66,    37,     0,     0,    56,    58,     0,     0,    44,
      46,     0,     0,     0,     0,    55,    59,    51,    52,     0,
      43,    47,    39,    40,     0,    68,    67,     0,    38,     0,
      61,     0,    49,     0,    69,     0,    18,    53,    18,    41,
      35,     0,     0,     0,    62,    50,    36
  };

  const signed char
  parser::yypgoto_[] =
  {
     -65,   -65,   -65,   -65,    -6,   -16,    50,   -65,   -65,   -65,
      55,   -64,   -65,    58,   -65,   -65,   -65,   -65,   -65,   -65,
     -27,   -65,   -65,   -65,   -65,   -65,    -1,   -65,   -65,   -21,
     -65,   -65,   -65,   -65,   -65,     8,   -65,   -65,   -65,   -65,
     -65,    21
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,     2,    10,    41,    38,    15,    16,     3,     7,    17,
      18,    22,    23,    24,    25,    40,    26,    53,   103,    63,
      83,    84,    27,    55,    68,    69,    70,    71,    98,    78,
      79,    28,    54,    64,    65,    66,    67,    96,    50,    51,
      87,    52
  };

  const signed char
  parser::yytable_[] =
  {
      29,    19,    33,    20,    73,    12,    13,    14,    85,    74,
       4,    21,    34,    86,    35,    36,    12,    13,    37,     1,
      35,    36,     5,    29,    56,    35,    36,     8,    30,    58,
      31,    47,   101,    48,   102,    12,    13,    90,    91,    92,
      93,    57,    59,     6,     9,    11,    75,    32,    44,    45,
      46,    60,    49,    72,    62,   -60,   -48,    80,    94,   104,
      89,    95,   105,   106,    77,    82,    99,    88,    81,   100,
      97,    39,    61,    76,     0,     0,     0,     0,     0,     0,
       0,    42,     0,     0,     0,    43
  };

  const signed char
  parser::yycheck_[] =
  {
      16,    11,    10,    13,     3,    18,    19,    20,    15,     8,
       7,    21,    20,    20,    16,    17,    18,    19,    20,     9,
      16,    17,     0,    39,    20,    16,    17,    20,     4,    20,
       6,     5,    96,     7,    98,    18,    19,     7,     8,     7,
       8,    47,    48,    10,     7,     6,     6,    20,     7,     7,
       7,    20,    14,     7,    20,    20,    20,     6,     6,     6,
      12,     7,     6,     6,    20,    20,    93,    20,    69,    20,
      91,    21,    51,    65,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    30
  };

  const signed char
  parser::yystos_[] =
  {
       0,     9,    23,    29,     7,     0,    10,    30,    20,     7,
      24,     6,    18,    19,    20,    27,    28,    31,    32,    11,
      13,    21,    33,    34,    35,    36,    38,    44,    53,    27,
       4,     6,    20,    10,    20,    16,    17,    20,    26,    28,
      37,    25,    35,    32,     7,     7,     7,     5,     7,    14,
      60,    61,    63,    39,    54,    45,    20,    26,    20,    26,
      20,    63,    20,    41,    55,    56,    57,    58,    46,    47,
      48,    49,     7,     3,     8,     6,    57,    20,    51,    52,
       6,    48,    20,    42,    43,    15,    20,    62,    20,    12,
       7,     8,     7,     8,     6,     7,    59,    51,    50,    42,
      20,    33,    33,    40,     6,     6,     6
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    22,    24,    25,    23,    26,    26,    27,    27,    28,
      28,    29,    30,    31,    31,    32,    32,    33,    33,    34,
      34,    35,    35,    35,    35,    36,    36,    37,    37,    37,
      37,    37,    37,    37,    39,    40,    38,    41,    41,    42,
      43,    43,    45,    44,    46,    46,    47,    47,    49,    50,
      48,    51,    52,    52,    54,    53,    55,    55,    56,    56,
      58,    59,    57,    60,    60,    61,    61,    62,    62,    63
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     0,     0,     6,     1,     1,     1,     1,     1,
       2,     4,     4,     1,     3,     1,     1,     1,     0,     1,
       2,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       3,     3,     3,     3,     0,     0,    11,     1,     3,     1,
       1,     3,     0,     6,     1,     0,     1,     2,     0,     0,
       6,     1,     1,     3,     0,     6,     1,     0,     1,     2,
       0,     0,     6,     1,     0,     1,     2,     1,     1,     5
  };




#if MFSLDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   152,   152,   171,   148,   188,   189,   197,   198,   202,
     203,   214,   225,   229,   230,   234,   239,   250,   251,   255,
     257,   261,   262,   263,   264,   272,   290,   310,   311,   312,
     313,   317,   321,   325,   337,   369,   336,   402,   416,   436,
     458,   459,   464,   463,   537,   538,   542,   544,   549,   562,
     549,   635,   657,   658,   663,   662,   736,   737,   741,   743,
     748,   761,   748,   834,   835,   839,   840,   844,   845,   850
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "stack now";
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
#endif // MFSLDEBUG


} // mfsl
#line 2056 "mfslParser.cpp"

#line 863 "mfslParser.yy"

//_______________________________________________________________________________


// other service code
//_______________________________________________________________________________


void
mfsl::parser::error (const location_type& loc, const std::string& message)
{
  mfslError (
    message,
    loc);
}
