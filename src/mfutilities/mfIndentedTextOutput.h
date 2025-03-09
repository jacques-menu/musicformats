/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfIndentedTextOutput___
#define ___mfIndentedTextOutput___

#include <cassert>
#include <sstream>   // for std::stringbuf

#include "exports.h"
#include "smartpointer.h"

#include "mfBool.h"
#include "mfRational.h"


using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP mfOutputIndenter
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfOutputIndenter (std::string spacer = "  ");

    virtual               ~mfOutputIndenter ();

  public:

    // set and get
    // ------------------------------------------------------

    // spacer
    std::string           getSpacer () const
                              { return fSpacer; }

    // indentation
    void                  setIndentation (int indentation)
                              { fIndentation = indentation; }
    int                   getIndentation () const
                              { return fIndentation; }

  public:

    // public services
    // ------------------------------------------------------

/*
    // Declare prefix and postfix decrement operators.
    Point& operator--();       // Prefix decrement operator.
    Point operator--(int);     // Postfix decrement operator.
*/

    // increase the indentation by 1, prefix operator
    mfOutputIndenter&     operator++ ();

    // decrease the indentation by 1, prefix operator
    mfOutputIndenter&     operator-- ();

    // increase the indentation by 'n', postfix operator
    mfOutputIndenter      operator++ (int n);
    // decrease the indentation by 'n', postfix operator
    mfOutputIndenter      operator-- (int n);

    mfOutputIndenter&     increment (int value = 1);
    mfOutputIndenter&     decrement (int value = 1);

    // reset the indentation
    void                  resetToZero ()
                              { fIndentation = 0; }

    // compare indentation value
    Bool                  operator == (const int &value) const
                              { return fIndentation == value; }
    Bool                  operator != (const int &value) const
                              { return fIndentation != value; }

    // output as much space as specified
    void                  printSpacers (std::ostream& os) const;

    // current offset
    std::string           fetchCurrentOffset ();

    // indent a multi-line 'R"(...)"' std::string
    void                  indentMultiLineString (
                            const std::string& theString,
                            std::ostream&      os);

    std::string           indentMultiLineStringWithCurrentOffset (
                            const std::string& theString);

    // print a multi-line string in a table with leading spaces
    // from the second line on
    void                  printMultiLineStringInATable (
                            const std::string& theString,
                            const int          columnStart,
                            const int          columnWidth,
                            std::ostream&      os);

  private:

    // private fields
    // ------------------------------------------------------

    int                   fIndentation;
    std::string           fSpacer;
};

EXP std::ostream& operator << (std::ostream& os, const mfOutputIndenter& theIndenter);

//______________________________________________________________________________
// a stream buffer that prefixes each line
// with the current indentation, i.e. spaces

/*
std::endl declaration:

  std::endl for std::ostream
  std::ostream& std::endl (std::ostream& os);

  basic template
  template <class   charT, class   traits>
  basic_ostream<charT,traits>& std::endl (basic_ostream<charT,traits>& os);

  Insert newline and flush
  Inserts a new-line character and flushes the stream.

  Its behavior is equivalent to calling os.put('\n') (or os.put(os.widen('\n')) for character types other than char), and then os.flush().

--

Reference for this class:
  https://stackoverflow.com/questions/2212776/overload-handling-of-stdendl
*/

class EXP mfIndentedOStreamBuf: public std::stringbuf
{
  public:

    // constructor
                          mfIndentedOStreamBuf (
                            std::ostream&     outputStream,
                            mfOutputIndenter& theIndenter);

    // indentation
    mfOutputIndenter&     getOutputIndenter () const
                              { return fOutputIndenter; }

    // flush
    void                  flush ()
                              { fOutputOStream.flush (); }

    virtual int           sync ();

  private:

    // private fields
    // ------------------------------------------------------

    std::ostream&         fOutputOStream;
    mfOutputIndenter&     fOutputIndenter;

};

//______________________________________________________________________________
class EXP mfIndentedOstream: public std::ostream, public smartable
{
/*
Reference for this class:
  https://stackoverflow.com/questions/2212776/overload-handling-of-stdendl

Usage:
  mfIndentedOstream myStream (std::cout);

  myStream <<
    1 << 2 << 3 << std::endl <<
    5 << 6 << std::endl <<
    7 << 8 << std::endl;
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfIndentedOstream> create (
                            std::ostream&     theOStream,
                            mfOutputIndenter& theIndenter);

    // constructors/destructor
    // ------------------------------------------------------

                          mfIndentedOstream (
                            std::ostream&     theOStream,
                            mfOutputIndenter& theIndenter);

    virtual               ~mfIndentedOstream () {};

  public:

    // public services
    // ------------------------------------------------------

    // flush
    void                  flush ()
                              { fIndentedOStreamBuf.flush (); }

    // indentation
    mfOutputIndenter&     getIndenter () const
                              { return fIndentedOStreamBuf.getOutputIndenter (); }

    void                  incrIndentation ()
                              { ++ (fIndentedOStreamBuf.getOutputIndenter ()); }

    void                  decrIndentation ()
                              { -- (fIndentedOStreamBuf.getOutputIndenter ()); }

  private:

    // private fields
    // ------------------------------------------------------

    // mfIndentedOstream just uses an mfIndentedOStreamBuf
    mfIndentedOStreamBuf  fIndentedOStreamBuf;

};
typedef SMARTP<mfIndentedOstream> S_mfIndentedOstream;

//______________________________________________________________________________
class EXP mfIndentedStringStream: public smartable
{
/*
Reference for this class:
  https://cplusplus.com/reference/sstream/stringstream/

Usage:
  mfIndentedStringStream iss;

  iss <<
    1 << 2 << 3 << std::endl << // no std::endl here!
    5 << 6 << std::endl <<
    7 << 8 << std::endl;
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfIndentedStringStream> create ();

    // constructors/destructor
    // ------------------------------------------------------

                          mfIndentedStringStream () {}

    virtual               ~mfIndentedStringStream () {};

  public:

    // set and get
    // ------------------------------------------------------

    std::stringstream&    getStringstream ()
                              { return fStringstream; }

  public:

    // public services
    // ------------------------------------------------------

    std::string           str () const
                              { return fStringstream.str (); }

  private:

    // private fields
    // ------------------------------------------------------

    std::stringstream     fStringstream;
};
typedef SMARTP<mfIndentedStringStream> S_mfIndentedStringStream;

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const char theChar);

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const std::string& theString);

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const char* theCString);

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const int& elt);

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const unsigned int& elt);

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const long int& elt);

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const unsigned long int& elt);

#ifdef WIN32
EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const size_t& elt);
#endif // WIN32

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const float& elt);

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const mfRational& elt);

//______________________________________________________________________________
// the hidden global output indenter for general use
EXP mfOutputIndenter& getGlobalOutputIndenter ();

// useful shortcut macros
#define gIndenter getGlobalOutputIndenter ()
#define gTab      getGlobalOutputIndenter ().getSpacer ()

// the hidden global log and output indented streams
EXP S_mfIndentedOstream& getGlobalOutputIndentedOstream ();
EXP S_mfIndentedOstream& getGlobalLogIndentedOstream ();

// useful shortcut macros
#define gOutput *(getGlobalOutputIndentedOstream ())
#define gLog    *(getGlobalLogIndentedOstream ())

//______________________________________________________________________________
EXP extern void createTheGlobalIndentedOstreams (
  std::ostream& theOutputStream,
  std::ostream& theLogStream);


}


#endif // ___mfIndentedTextOutput___
