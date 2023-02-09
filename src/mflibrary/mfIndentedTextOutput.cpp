/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "mfAssert.h"

#include "mfStringsHandling.h"

#include "mfIndentedTextOutput.h"


namespace MusicFormats
{

//______________________________________________________________________________
// #define DEBUG_INDENTER

//______________________________________________________________________________
mfOutputIndenter::mfOutputIndenter (std::string spacer)
{
  fIndentation = 0;
  fSpacer = spacer;
}

mfOutputIndenter::~mfOutputIndenter ()
{}

/*
  Declare prefix and postfix decrement operators.
    Point& operator--();       // Prefix decrement operator.
    Point operator--(int);     // Postfix decrement operator.
*/

// increase the indentation by 1, prefix operator
mfOutputIndenter& mfOutputIndenter::operator++ ()
{
#ifdef DEBUG_INDENTER
  gLog <<
    "% Incrementing INDENTER: " << fIndentation <<
    std::endl;
#endif

  ++fIndentation;

#ifdef DEBUG_INDENTER
  gLog <<
    "% INDENTER: " << fIndentation <<
    std::endl;
#endif

  return *this;
}

// decrease the indentation by 1, prefix operator
mfOutputIndenter& mfOutputIndenter::operator-- ()
{
#ifdef DEBUG_INDENTER
  gLog <<
    "% Decrementing INDENTER: " << fIndentation <<
    std::endl;
#endif

  --fIndentation;

  if (fIndentation < 0) {
    gLog <<
      std::endl <<
      "% ### Indentation has become negative: " <<  fIndentation <<
      std::endl << std::endl;

#ifdef DEBUG_INDENTER
    mfAssert (
      __FILE__, __LINE__,
      false,
      "indentation has become negative");
#endif
  }

#ifdef DEBUG_INDENTER
  else {
    gLog <<
      "% INDENTER: " << fIndentation <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  return *this;
}

// increase the indentation by 1, postfix operator
mfOutputIndenter mfOutputIndenter::mfOutputIndenter::operator++ (int)
{
  ++fIndentation;

#ifdef DEBUG_INDENTER
  gLog <<
    "% INDENTER: " << fIndentation <<
    std::endl;
#endif

  return *this;
}

// decrease the indentation by 1, postfix operator
mfOutputIndenter mfOutputIndenter::mfOutputIndenter::operator-- (int)
{
  --fIndentation;

  if (fIndentation < 0) {
    gLog <<
      std::endl <<
      "% ### Indentation has become negative: " <<  fIndentation <<
      std::endl << std::endl;

#ifdef DEBUG_INDENTER
    mfAssert (
      __FILE__, __LINE__,
      false,
      "indentation has become negative");
#endif
  }

#ifdef DEBUG_INDENTER
  else {
    gLog <<
      "% INDENTER: " << fIndentation <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  return *this;
}

mfOutputIndenter& mfOutputIndenter::increment (int value)
{
  fIndentation += value;

  if (fIndentation < 0) {
    gLog <<
      std::endl <<
      "% ### Indentation has become negative: " <<  fIndentation <<
      std::endl << std::endl;

#ifdef DEBUG_INDENTER
    mfAssert (
      __FILE__, __LINE__,
      false,
      "indentation has become negative");
#endif
  }

#ifdef DEBUG_INDENTER
  else {
    gLog <<
      "% INDENTER: " << fIndentation <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  return *this;
}

mfOutputIndenter& mfOutputIndenter::decrement (int value)
{
  fIndentation -= value;

  if (fIndentation < 0) {
    gLog <<
      std::endl <<
      "% ### Indentation has become negative: " <<  fIndentation <<
      std::endl << std::endl;

#ifdef DEBUG_INDENTER
    mfAssert (
      __FILE__, __LINE__,
      false,
      "indentation has become negative");
#endif
  }

#ifdef DEBUG_INDENTER
  else {
    gLog <<
      "% INDENTER: " << fIndentation <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  return *this;
}

std::string mfOutputIndenter::fetchCurrentOffset ()
{
  return
    mfReplicateString (
      fSpacer,
      fIndentation);
}

void mfOutputIndenter::indentMultiLineString (
  const std::string& theString,
  std::ostream&      os)
{
  // add indentation ahead of all lines of theString

  // the std::istringstream to read lines from theString
  std::istringstream inputStream (theString);
  std::string        line;

  // print theString line by line
  while (getline (inputStream, line)) {
    os << line << std::endl;
  } // while
}

std::string mfOutputIndenter::indentMultiLineStringWithCurrentOffset (
  const std::string& theString)
{
  // add indentation ahead of all lines of theString

  std::stringstream ss;

  // the std::istringstream to read lines from theString
  std::istringstream inputStream (theString);
  std::string        line;

  // append theString line by line
  while (getline (inputStream, line)) {
    ss << fetchCurrentOffset () << line;
  } // while

  return ss.str ();
}

void mfOutputIndenter::printMultiLineStringInATable (
  const std::string& theString,
  const int          columnStart,
  const int          columnWidth,
  std::ostream&      os)
{
  // the std::istringstream to read lines from theString
  std::istringstream inputStream (theString);
  std::string        line;

  // print theString line by line
  int counter = 0;

  while (getline (inputStream, line)) {
    ++ counter;
    if (counter > 1) {
      os << std::left <<
        std::setw (columnWidth) <<
        mfReplicateChar (' ', columnStart);
    }
    os << line;
  } // while
}

void mfOutputIndenter::printSpacers (std::ostream& os) const
{
  int i = fIndentation;

  while (i-- > 0) os << fSpacer;
}

std::ostream& operator << (std::ostream& os, const mfOutputIndenter& theIndenter)
{
  theIndenter.printSpacers (os);
  return os;
}

//______________________________________________________________________________
int mfIndentedOStreamBuf::sync ()
{
  // When we sync the stream with fOutputOStream:
  // 1) output the indentation then the buffer
  // 2) reset the buffer
  // 3) flush the actual output stream we are using.

  size_t strSize = str ().size ();

  // fetch the last non-space character in the buffer
  // caution: the std::endl is present as the last character!
  size_t found = str ().find_last_not_of (' ', strSize - 2);

  // this can be uncommented to see low level informations
  // fOutputOStream << "% strSize: " << strSize << ", found: " << found << std::endl;

  // output the spacers
  fOutputIndenter.printSpacers (fOutputOStream);

  // output the buffer
  if (found == strSize - 3) {
    // don't output the trailing spaces, but output the end of line
    fOutputOStream << str ().substr (0, found + 1) << std::endl;
  }
  else {
    // output the whole buffer
    fOutputOStream << str ();
  }

  // reset the buffer
  str ("");

  // flush the output stream
  fOutputOStream.flush ();

  return 0;
}

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const char theChar)
{
  iss.getStringstream () <<
    theChar;

  return iss;
}

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const std::string& theString)
{
  iss.getStringstream () <<
    gIndenter.indentMultiLineStringWithCurrentOffset (
      theString);

  return iss;
}

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const char* theCString)
{
  iss.getStringstream () <<
    gIndenter.indentMultiLineStringWithCurrentOffset (
      theCString);

  return iss;
}

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const int& elt)
{
  iss.getStringstream () <<
    elt;

  return iss;
}

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const unsigned int& elt)
{
  iss.getStringstream () <<
    elt;

  return iss;
}

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const long int& elt)
{
  iss.getStringstream () <<
    elt;

  return iss;
}

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const unsigned long int& elt)
{
  iss.getStringstream () <<
    elt;

  return iss;
}

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const float& elt)
{
  iss.getStringstream () <<
    elt;

  return iss;
}

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const Rational& rat)
{
  iss.getStringstream () <<
    gIndenter.indentMultiLineStringWithCurrentOffset (
      rat.asString ());

  return iss;
}

//______________________________________________________________________________
// code taken from:
// http://comp.lang.cpp.moderated.narkive.com/fylLGJgp/redirect-output-to-dev-null
template<typename Ch, typename Traits = std::char_traits<Ch> >
struct basic_nullbuf : std::basic_streambuf<Ch, Traits>
{
  typedef std::basic_streambuf<Ch, Traits> base_type;
  typedef typename base_type::int_type int_type;
  typedef typename base_type::traits_type traits_type;

  virtual int_type overflow (int_type c) {
    return traits_type::not_eof (c);
  }
};

// convenient typedefs
typedef basic_nullbuf <char>    nullbuf;
//typedef basic_nullbuf <wchar_t> wnullbuf;

// buffers and streams
nullbuf cnull_obj;
//wnullbuf wcnull_obj;

std::ostream cnull  (& cnull_obj);
//wostream wcnull (& wcnull_obj);

//______________________________________________________________________________
// the global hidden indenter for general use
EXP mfOutputIndenter pGlobalOutputIndenter;

EXP mfOutputIndenter& getGlobalOutputIndenter ()
{
  return pGlobalOutputIndenter;
}

// the hidden global log and output indented streams
S_indentedOstream pGlobalOutputIndentedOstream;
S_indentedOstream pGlobalLogIndentedOstream;

EXP S_indentedOstream& getGlobalOutputIndentedOstream ()
{
  return pGlobalOutputIndentedOstream;
}

EXP S_indentedOstream& getGlobalLogIndentedOstream ()
{
  return pGlobalLogIndentedOstream;
}

//______________________________________________________________________________
void createTheGlobalIndentedOstreams (
  std::ostream& theOutputStream,
  std::ostream& theLogStream)
{
  pGlobalOutputIndentedOstream =
    mfIndentedOstream::create (
      theOutputStream,
      theOutputStream);

  pGlobalLogIndentedOstream =
    mfIndentedOstream::create (
      theLogStream,
      theLogStream);
}


}


/*
//______________________________________________________________________________
  testMfIndentedStringstream (); // JMI v0.9.66


void testMfIndentedStringstream ()
{
  mfIndentedStringStream iss;

  // populate the mfIndentedStringStream
  iss <<
    "iss 1" <<
    std::endl;

  ++gIndenter;
  iss <<
    "iss 1.1" <<
    std::endl <<
    "iss 1.2" <<
    std::endl;
  --gIndenter;

  iss <<
    "iss 2" <<
    std::endl;

  ++gIndenter;
  iss <<
    "iss 2.1" <<
    std::endl;

  iss <<
    "iss 2.2" <<
    std::endl;
  {
    ++gIndenter;
    iss <<
      "iss 2.2.1" <<
      std::endl <<
      "iss 2.2.2" <<
      std::endl;
    --gIndenter;
  }

  iss <<
    "iss 2.3" <<
    std::endl;

  --gIndenter;

  iss <<
    "iss 3" <<
    std::endl;


  // display the mfIndentedStringStream contents
//   gLog <<
//     "iss.str ():" <<
//     std::endl;
//
//   ++gIndenter;
  gLog <<
    iss.str () <<
    std::endl;
//   --gIndenter;
}

*/
