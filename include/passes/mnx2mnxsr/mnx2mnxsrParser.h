// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton interface for Bison LALR(1) parsers in C++

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


/**
 ** \file mnx2mnxsrParser.h
 ** Define the mnx2mnxsr::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

#ifndef YY_MNX2MNXSR_MNX2MNXSRPARSER_H_INCLUDED
# define YY_MNX2MNXSR_MNX2MNXSRPARSER_H_INCLUDED
// "%code requires" blocks.
#line 46 "mnx2mnxsrParser.yy"

  #include <string>

  class mnx2mnxsrDriver;

#line 55 "mnx2mnxsrParser.h"

# include <cassert>
# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif
# include "location.hh"
#include <typeinfo>
#ifndef MNX2MNXSR_ASSERT
# include <cassert>
# define MNX2MNXSR_ASSERT assert
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Debug traces.  */
#ifndef MNX2MNXSRDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define MNX2MNXSRDEBUG 1
#  else
#   define MNX2MNXSRDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define MNX2MNXSRDEBUG 1
# endif /* ! defined YYDEBUG */
#endif  /* ! defined MNX2MNXSRDEBUG */

namespace mnx2mnxsr {
#line 203 "mnx2mnxsrParser.h"




  /// A Bison parser.
  class parser
  {
  public:
#ifdef MNX2MNXSRSTYPE
# ifdef __GNUC__
#  pragma GCC message "bison: do not #define MNX2MNXSRSTYPE in C++, use %define api.value.type"
# endif
    typedef MNX2MNXSRSTYPE value_type;
#else
  /// A buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current parser state.
  class value_type
  {
  public:
    /// Type of *this.
    typedef value_type self_type;

    /// Empty construction.
    value_type () YY_NOEXCEPT
      : yyraw_ ()
      , yytypeid_ (YY_NULLPTR)
    {}

    /// Construct and fill.
    template <typename T>
    value_type (YY_RVREF (T) t)
      : yytypeid_ (&typeid (T))
    {
      MNX2MNXSR_ASSERT (sizeof (T) <= size);
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    value_type (const self_type&) = delete;
    /// Non copyable.
    self_type& operator= (const self_type&) = delete;
#endif

    /// Destruction, allowed only if empty.
    ~value_type () YY_NOEXCEPT
    {
      MNX2MNXSR_ASSERT (!yytypeid_);
    }

# if 201103L <= YY_CPLUSPLUS
    /// Instantiate a \a T in here from \a t.
    template <typename T, typename... U>
    T&
    emplace (U&&... u)
    {
      MNX2MNXSR_ASSERT (!yytypeid_);
      MNX2MNXSR_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (std::forward <U>(u)...);
    }
# else
    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    emplace ()
    {
      MNX2MNXSR_ASSERT (!yytypeid_);
      MNX2MNXSR_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T ();
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    emplace (const T& t)
    {
      MNX2MNXSR_ASSERT (!yytypeid_);
      MNX2MNXSR_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (t);
    }
# endif

    /// Instantiate an empty \a T in here.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build ()
    {
      return emplace<T> ();
    }

    /// Instantiate a \a T in here from \a t.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build (const T& t)
    {
      return emplace<T> (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as () YY_NOEXCEPT
    {
      MNX2MNXSR_ASSERT (yytypeid_);
      MNX2MNXSR_ASSERT (*yytypeid_ == typeid (T));
      MNX2MNXSR_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const YY_NOEXCEPT
    {
      MNX2MNXSR_ASSERT (yytypeid_);
      MNX2MNXSR_ASSERT (*yytypeid_ == typeid (T));
      MNX2MNXSR_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Swap the content with \a that, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsibility.
    /// Swapping between built and (possibly) non-built is done with
    /// self_type::move ().
    template <typename T>
    void
    swap (self_type& that) YY_NOEXCEPT
    {
      MNX2MNXSR_ASSERT (yytypeid_);
      MNX2MNXSR_ASSERT (*yytypeid_ == *that.yytypeid_);
      std::swap (as<T> (), that.as<T> ());
    }

    /// Move the content of \a that to this.
    ///
    /// Destroys \a that.
    template <typename T>
    void
    move (self_type& that)
    {
# if 201103L <= YY_CPLUSPLUS
      emplace<T> (std::move (that.as<T> ()));
# else
      emplace<T> ();
      swap<T> (that);
# endif
      that.destroy<T> ();
    }

# if 201103L <= YY_CPLUSPLUS
    /// Move the content of \a that to this.
    template <typename T>
    void
    move (self_type&& that)
    {
      emplace<T> (std::move (that.as<T> ()));
      that.destroy<T> ();
    }
#endif

    /// Copy the content of \a that to this.
    template <typename T>
    void
    copy (const self_type& that)
    {
      emplace<T> (that.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
      yytypeid_ = YY_NULLPTR;
    }

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    value_type (const self_type&);
    /// Non copyable.
    self_type& operator= (const self_type&);
#endif

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ () YY_NOEXCEPT
    {
      void *yyp = yyraw_;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const YY_NOEXCEPT
    {
      const void *yyp = yyraw_;
      return static_cast<const T*> (yyp);
     }

    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // "integer number"
      // "double number"
      // "single quoted_string"
      // "double quoted_string"
      // "name"
      // "option"
      // Number
      // SingleString
      // String
      char dummy1[sizeof (std::string)];
    };

    /// The size of the largest semantic type.
    enum { size = sizeof (union_type) };

    /// A buffer to store semantic values.
    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me_;
      /// A buffer large enough to store any of the semantic values.
      char yyraw_[size];
    };

    /// Whether the content is built: if defined, the name of the stored type.
    const std::type_info *yytypeid_;
  };

#endif
    /// Backward compatibility (Bison 3.8).
    typedef value_type semantic_type;

    /// Symbol locations.
    typedef smi::location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m)
        : std::runtime_error (m)
        , location (l)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
        , location (s.location)
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;

      location_type location;
    };

    /// Token kinds.
    struct token
    {
      enum token_kind_type
      {
        MNX2MNXSR_TOK_MNX2MNXSREMPTY = -2,
    MNX2MNXSR_TOK_YYEOF = 0,       // "end of file"
    MNX2MNXSR_TOK_MNX2MNXSRerror = 1, // error
    MNX2MNXSR_TOK_MNX2MNXSRUNDEF = 2, // "invalid token"
    MNX2MNXSR_TOK_COLON = 3,       // ":"
    MNX2MNXSR_TOK_CURLY_LEFT_BRACKET = 4, // "{"
    MNX2MNXSR_TOK_CURLY_RIGHT_BRACKET = 5, // "}"
    MNX2MNXSR_TOK_LEFT_BRACKET = 6, // "["
    MNX2MNXSR_TOK_RIGHT_BRACKET = 7, // "]"
    MNX2MNXSR_TOK_COMMA = 8,       // ","
    MNX2MNXSR_TOK_MNX = 9,         // "mnx"
    MNX2MNXSR_TOK_VERSION = 10,    // "version"
    MNX2MNXSR_TOK_GLOBAL = 11,     // "global"
    MNX2MNXSR_TOK_PARTS = 12,      // "parts"
    MNX2MNXSR_TOK_MEASURES = 13,   // "measures"
    MNX2MNXSR_TOK_SEQUENCES = 14,  // "sequences"
    MNX2MNXSR_TOK_CLEFS = 15,      // "clefs"
    MNX2MNXSR_TOK_CLEF = 16,       // "clef"
    MNX2MNXSR_TOK_SIGN = 17,       // "sign"
    MNX2MNXSR_TOK_STAFF_POSITION = 18, // "staffPosition"
    MNX2MNXSR_TOK_CONTENT = 19,    // "content"
    MNX2MNXSR_TOK_TYPE = 20,       // "type"
    MNX2MNXSR_TOK_DURATION = 21,   // "duration"
    MNX2MNXSR_TOK_BASE = 22,       // "base"
    MNX2MNXSR_TOK_NOTES = 23,      // "notes"
    MNX2MNXSR_TOK_PITCH = 24,      // "pitch"
    MNX2MNXSR_TOK_OCTAVE = 25,     // "octave"
    MNX2MNXSR_TOK_STEP = 26,       // "step"
    MNX2MNXSR_TOK_INTEGER = 27,    // "integer number"
    MNX2MNXSR_TOK_DOUBLE = 28,     // "double number"
    MNX2MNXSR_TOK_SINGLE_QUOTED_STRING = 29, // "single quoted_string"
    MNX2MNXSR_TOK_DOUBLE_QUOTED_STRING = 30, // "double quoted_string"
    MNX2MNXSR_TOK_NAME = 31,       // "name"
    MNX2MNXSR_TOK_OPTION = 32      // "option"
      };
      /// Backward compatibility alias (Bison 3.6).
      typedef token_kind_type yytokentype;
    };

    /// Token kind, as returned by yylex.
    typedef token::token_kind_type token_kind_type;

    /// Backward compatibility alias (Bison 3.6).
    typedef token_kind_type token_type;

    /// Symbol kinds.
    struct symbol_kind
    {
      enum symbol_kind_type
      {
        YYNTOKENS = 33, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // "end of file"
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_COLON = 3,                             // ":"
        S_CURLY_LEFT_BRACKET = 4,                // "{"
        S_CURLY_RIGHT_BRACKET = 5,               // "}"
        S_LEFT_BRACKET = 6,                      // "["
        S_RIGHT_BRACKET = 7,                     // "]"
        S_COMMA = 8,                             // ","
        S_MNX = 9,                               // "mnx"
        S_VERSION = 10,                          // "version"
        S_GLOBAL = 11,                           // "global"
        S_PARTS = 12,                            // "parts"
        S_MEASURES = 13,                         // "measures"
        S_SEQUENCES = 14,                        // "sequences"
        S_CLEFS = 15,                            // "clefs"
        S_CLEF = 16,                             // "clef"
        S_SIGN = 17,                             // "sign"
        S_STAFF_POSITION = 18,                   // "staffPosition"
        S_CONTENT = 19,                          // "content"
        S_TYPE = 20,                             // "type"
        S_DURATION = 21,                         // "duration"
        S_BASE = 22,                             // "base"
        S_NOTES = 23,                            // "notes"
        S_PITCH = 24,                            // "pitch"
        S_OCTAVE = 25,                           // "octave"
        S_STEP = 26,                             // "step"
        S_INTEGER = 27,                          // "integer number"
        S_DOUBLE = 28,                           // "double number"
        S_SINGLE_QUOTED_STRING = 29,             // "single quoted_string"
        S_DOUBLE_QUOTED_STRING = 30,             // "double quoted_string"
        S_NAME = 31,                             // "name"
        S_OPTION = 32,                           // "option"
        S_YYACCEPT = 33,                         // $accept
        S_MnxSource = 34,                        // MnxSource
        S_35_1 = 35,                             // $@1
        S_36_2 = 36,                             // $@2
        S_37_3 = 37,                             // $@3
        S_Number = 38,                           // Number
        S_SingleString = 39,                     // SingleString
        S_String = 40,                           // String
        S_Mnx = 41,                              // Mnx
        S_Version = 42,                          // Version
        S_Global = 43,                           // Global
        S_Parts = 44,                            // Parts
        S_Part = 45,                             // Part
        S_Measures = 46,                         // Measures
        S_Measure = 47,                          // Measure
        S_MeasureElements = 48,                  // MeasureElements
        S_MeasureElement = 49,                   // MeasureElement
        S_Clefs = 50,                            // Clefs
        S_Clef = 51,                             // Clef
        S_Sequences = 52,                        // Sequences
        S_SequencesElements = 53,                // SequencesElements
        S_SequencesElement = 54,                 // SequencesElement
        S_Content = 55,                          // Content
        S_ContentElements = 56,                  // ContentElements
        S_ContentElement = 57,                   // ContentElement
        S_Type = 58,                             // Type
        S_Duration = 59,                         // Duration
        S_DurationElements = 60,                 // DurationElements
        S_DurationElement = 61,                  // DurationElement
        S_Notes = 62,                            // Notes
        S_NotesElements = 63,                    // NotesElements
        S_NotesElement = 64,                     // NotesElement
        S_Pitch = 65                             // Pitch
      };
    };

    /// (Internal) symbol kind.
    typedef symbol_kind::symbol_kind_type symbol_kind_type;

    /// The number of tokens.
    static const symbol_kind_type YYNTOKENS = symbol_kind::YYNTOKENS;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol kind
    /// via kind ().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol () YY_NOEXCEPT
        : value ()
        , location ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that)
        : Base (std::move (that))
        , value ()
        , location (std::move (that.location))
      {
        switch (this->kind ())
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
        value.move< std::string > (std::move (that.value));
        break;

      default:
        break;
    }

      }
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructors for typed symbols.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, location_type&& l)
        : Base (t)
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const location_type& l)
        : Base (t)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::string&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::string& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }



      /// Destroy contents, and record that is empty.
      void clear () YY_NOEXCEPT
      {
        // User destructor.
        symbol_kind_type yykind = this->kind ();
        basic_symbol<Base>& yysym = *this;
        (void) yysym;
        switch (yykind)
        {
       default:
          break;
        }

        // Value type destructor.
switch (yykind)
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
        value.template destroy< std::string > ();
        break;

      default:
        break;
    }

        Base::clear ();
      }

      /// The user-facing name of this symbol.
      const char *name () const YY_NOEXCEPT
      {
        return parser::symbol_name (this->kind ());
      }

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      value_type value;

      /// The location.
      location_type location;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_kind
    {
      /// The symbol kind as needed by the constructor.
      typedef token_kind_type kind_type;

      /// Default constructor.
      by_kind () YY_NOEXCEPT;

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_kind (by_kind&& that) YY_NOEXCEPT;
#endif

      /// Copy constructor.
      by_kind (const by_kind& that) YY_NOEXCEPT;

      /// Constructor from (external) token numbers.
      by_kind (kind_type t) YY_NOEXCEPT;



      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_kind& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// The symbol kind.
      /// \a S_YYEMPTY when empty.
      symbol_kind_type kind_;
    };

    /// Backward compatibility for a private implementation detail (Bison 3.6).
    typedef by_kind by_type;

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_kind>
    {
      /// Superclass.
      typedef basic_symbol<by_kind> super_type;

      /// Empty symbol.
      symbol_type () YY_NOEXCEPT {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, location_type l)
        : super_type (token_kind_type (tok), std::move (l))
#else
      symbol_type (int tok, const location_type& l)
        : super_type (token_kind_type (tok), l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        MNX2MNXSR_ASSERT (tok == token::MNX2MNXSR_TOK_YYEOF
                   || (token::MNX2MNXSR_TOK_MNX2MNXSRerror <= tok && tok <= token::MNX2MNXSR_TOK_STEP));
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, std::string v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const std::string& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        MNX2MNXSR_ASSERT ((token::MNX2MNXSR_TOK_INTEGER <= tok && tok <= token::MNX2MNXSR_TOK_OPTION));
#endif
      }
    };

    /// Build a parser object.
    parser (mnx2mnxsrDriver& drv_yyarg);
    virtual ~parser ();

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    parser (const parser&) = delete;
    /// Non copyable.
    parser& operator= (const parser&) = delete;
#endif

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if MNX2MNXSRDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

    /// The user-facing name of the symbol whose (internal) number is
    /// YYSYMBOL.  No bounds checking.
    static const char *symbol_name (symbol_kind_type yysymbol);

    // Implementation of make_symbol for each token kind.
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYEOF (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_YYEOF, std::move (l));
      }
#else
      static
      symbol_type
      make_YYEOF (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_YYEOF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MNX2MNXSRerror (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_MNX2MNXSRerror, std::move (l));
      }
#else
      static
      symbol_type
      make_MNX2MNXSRerror (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_MNX2MNXSRerror, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MNX2MNXSRUNDEF (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_MNX2MNXSRUNDEF, std::move (l));
      }
#else
      static
      symbol_type
      make_MNX2MNXSRUNDEF (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_MNX2MNXSRUNDEF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COLON (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_COLON, std::move (l));
      }
#else
      static
      symbol_type
      make_COLON (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_COLON, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CURLY_LEFT_BRACKET (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_CURLY_LEFT_BRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_CURLY_LEFT_BRACKET (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_CURLY_LEFT_BRACKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CURLY_RIGHT_BRACKET (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_CURLY_RIGHT_BRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_CURLY_RIGHT_BRACKET (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_CURLY_RIGHT_BRACKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LEFT_BRACKET (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_LEFT_BRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_LEFT_BRACKET (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_LEFT_BRACKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RIGHT_BRACKET (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_RIGHT_BRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_RIGHT_BRACKET (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_RIGHT_BRACKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COMMA (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_COMMA, std::move (l));
      }
#else
      static
      symbol_type
      make_COMMA (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_COMMA, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MNX (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_MNX, std::move (l));
      }
#else
      static
      symbol_type
      make_MNX (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_MNX, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_VERSION (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_VERSION, std::move (l));
      }
#else
      static
      symbol_type
      make_VERSION (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_VERSION, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GLOBAL (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_GLOBAL, std::move (l));
      }
#else
      static
      symbol_type
      make_GLOBAL (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_GLOBAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PARTS (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_PARTS, std::move (l));
      }
#else
      static
      symbol_type
      make_PARTS (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_PARTS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MEASURES (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_MEASURES, std::move (l));
      }
#else
      static
      symbol_type
      make_MEASURES (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_MEASURES, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SEQUENCES (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_SEQUENCES, std::move (l));
      }
#else
      static
      symbol_type
      make_SEQUENCES (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_SEQUENCES, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CLEFS (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_CLEFS, std::move (l));
      }
#else
      static
      symbol_type
      make_CLEFS (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_CLEFS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CLEF (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_CLEF, std::move (l));
      }
#else
      static
      symbol_type
      make_CLEF (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_CLEF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SIGN (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_SIGN, std::move (l));
      }
#else
      static
      symbol_type
      make_SIGN (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_SIGN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_STAFF_POSITION (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_STAFF_POSITION, std::move (l));
      }
#else
      static
      symbol_type
      make_STAFF_POSITION (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_STAFF_POSITION, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CONTENT (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_CONTENT, std::move (l));
      }
#else
      static
      symbol_type
      make_CONTENT (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_CONTENT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TYPE (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_TYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_TYPE (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_TYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DURATION (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_DURATION, std::move (l));
      }
#else
      static
      symbol_type
      make_DURATION (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_DURATION, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BASE (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_BASE, std::move (l));
      }
#else
      static
      symbol_type
      make_BASE (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_BASE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NOTES (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_NOTES, std::move (l));
      }
#else
      static
      symbol_type
      make_NOTES (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_NOTES, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PITCH (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_PITCH, std::move (l));
      }
#else
      static
      symbol_type
      make_PITCH (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_PITCH, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OCTAVE (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_OCTAVE, std::move (l));
      }
#else
      static
      symbol_type
      make_OCTAVE (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_OCTAVE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_STEP (location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_STEP, std::move (l));
      }
#else
      static
      symbol_type
      make_STEP (const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_STEP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INTEGER (std::string v, location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_INTEGER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_INTEGER (const std::string& v, const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_INTEGER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DOUBLE (std::string v, location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_DOUBLE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_DOUBLE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_DOUBLE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SINGLE_QUOTED_STRING (std::string v, location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_SINGLE_QUOTED_STRING, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_SINGLE_QUOTED_STRING (const std::string& v, const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_SINGLE_QUOTED_STRING, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DOUBLE_QUOTED_STRING (std::string v, location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_DOUBLE_QUOTED_STRING, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_DOUBLE_QUOTED_STRING (const std::string& v, const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_DOUBLE_QUOTED_STRING, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NAME (std::string v, location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_NAME, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_NAME (const std::string& v, const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_NAME, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OPTION (std::string v, location_type l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_OPTION, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_OPTION (const std::string& v, const location_type& l)
      {
        return symbol_type (token::MNX2MNXSR_TOK_OPTION, v, l);
      }
#endif


    class context
    {
    public:
      context (const parser& yyparser, const symbol_type& yyla);
      const symbol_type& lookahead () const YY_NOEXCEPT { return yyla_; }
      symbol_kind_type token () const YY_NOEXCEPT { return yyla_.kind (); }
      const location_type& location () const YY_NOEXCEPT { return yyla_.location; }

      /// Put in YYARG at most YYARGN of the expected tokens, and return the
      /// number of tokens stored in YYARG.  If YYARG is null, return the
      /// number of expected tokens (guaranteed to be less than YYNTOKENS).
      int expected_tokens (symbol_kind_type yyarg[], int yyargn) const;

    private:
      const parser& yyparser_;
      const symbol_type& yyla_;
    };

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    parser (const parser&);
    /// Non copyable.
    parser& operator= (const parser&);
#endif

    /// Check the lookahead yytoken.
    /// \returns  true iff the token will be eventually shifted.
    bool yy_lac_check_ (symbol_kind_type yytoken) const;
    /// Establish the initial context if no initial context currently exists.
    /// \returns  true iff the token will be eventually shifted.
    bool yy_lac_establish_ (symbol_kind_type yytoken);
    /// Discard any previous initial lookahead context because of event.
    /// \param event  the event which caused the lookahead to be discarded.
    ///               Only used for debbuging output.
    void yy_lac_discard_ (const char* event);

    /// Stored state numbers (used for stacks).
    typedef signed char state_type;

    /// The arguments of the error message.
    int yy_syntax_error_arguments_ (const context& yyctx,
                                    symbol_kind_type yyarg[], int yyargn) const;

    /// Generate an error message.
    /// \param yyctx     the context in which the error occurred.
    virtual std::string yysyntax_error_ (const context& yyctx) const;
    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    static state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT;

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT;

    static const signed char yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token kind \a t to a symbol kind.
    /// In theory \a t should be a token_kind_type, but character literals
    /// are valid, yet not members of the token_kind_type enum.
    static symbol_kind_type yytranslate_ (int t) YY_NOEXCEPT;



    // Tables.
    // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
    // STATE-NUM.
    static const signed char yypact_[];

    // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
    // Performed when YYTABLE does not specify something else to do.  Zero
    // means the default is an error.
    static const signed char yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const signed char yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const signed char yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const signed char yytable_[];

    static const signed char yycheck_[];

    // YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
    // state STATE-NUM.
    static const signed char yystos_[];

    // YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.
    static const signed char yyr1_[];

    // YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.
    static const signed char yyr2_[];


#if MNX2MNXSRDEBUG
    // YYRLINE[YYN] -- Source line where rule number YYN was defined.
    static const short yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r) const;
    /// Print the state stack on the debug stream.
    virtual void yy_stack_print_ () const;

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol kind, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol kind as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_state& that);

      /// The symbol kind (corresponding to \a state).
      /// \a symbol_kind::S_YYEMPTY when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      /// We use the initial state, as it does not have a value.
      enum { empty_state = 0 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);

      /// Assignment, needed by push_back by other implementations.
      /// Needed by some other old implementations.
      stack_symbol_type& operator= (const stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::iterator iterator;
      typedef typename S::const_iterator const_iterator;
      typedef typename S::size_type size_type;
      typedef typename std::ptrdiff_t index_type;

      stack (size_type n = 200) YY_NOEXCEPT
        : seq_ (n)
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Non copyable.
      stack (const stack&) = delete;
      /// Non copyable.
      stack& operator= (const stack&) = delete;
#endif

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (index_type i) const
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (index_type i)
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (std::ptrdiff_t n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      index_type
      size () const YY_NOEXCEPT
      {
        return index_type (seq_.size ());
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.begin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.end ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, index_type range) YY_NOEXCEPT
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (index_type i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        index_type range_;
      };

    private:
#if YY_CPLUSPLUS < 201103L
      /// Non copyable.
      stack (const stack&);
      /// Non copyable.
      stack& operator= (const stack&);
#endif
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;
    /// The stack for LAC.
    /// Logically, the yy_lac_stack's lifetime is confined to the function
    /// yy_lac_check_. We just store it as a member of this class to hold
    /// on to the memory and to avoid frequent reallocations.
    /// Since yy_lac_check_ is const, this member must be mutable.
    mutable std::vector<state_type> yylac_stack_;
    /// Whether an initial LAC context was established.
    bool yy_lac_established_;


    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1) YY_NOEXCEPT;

    /// Constants.
    enum
    {
      yylast_ = 124,     ///< Last index in yytable_.
      yynnts_ = 33,  ///< Number of nonterminal symbols.
      yyfinal_ = 5 ///< Termination state number.
    };


    // User arguments.
    mnx2mnxsrDriver& drv;

  };

  inline
  parser::symbol_kind_type
  parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    return static_cast<symbol_kind_type> (t);
  }

  // basic_symbol.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
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
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  parser::symbol_kind_type
  parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
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
        value.move< std::string > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  inline
  parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  inline
  parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  inline
  parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  inline
  parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  inline
  void
  parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  inline
  void
  parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  inline
  parser::symbol_kind_type
  parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  inline
  parser::symbol_kind_type
  parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


} // mnx2mnxsr
#line 1867 "mnx2mnxsrParser.h"




#endif // !YY_MNX2MNXSR_MNX2MNXSRPARSER_H_INCLUDED
