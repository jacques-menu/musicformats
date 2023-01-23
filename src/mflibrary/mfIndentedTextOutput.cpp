/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfAssert.h"

#include "mfStringsHandling.h"

#include "mfIndentedTextOutput.h"


namespace MusicFormats
{

//______________________________________________________________________________
// #define DEBUG_INDENTER

//______________________________________________________________________________
mfOutputIndenter mfOutputIndenter::gGlobalOStreamIndenter;

mfOutputIndenter::mfOutputIndenter (std::string spacer)
{
  fIndentation = 0;
  fSpacer = spacer;
}

mfOutputIndenter::~mfOutputIndenter ()
{}

/*
  // Declare prefix and postfix decrement operators.
  Point& operator--();       // Prefix decrement operator.
  Point operator--(int);     // Postfix decrement operator.
*/

// increase the indentation by 1, prefix operator
mfOutputIndenter& mfOutputIndenter::operator++ ()
{
#ifdef DEBUG_INDENTER
  gLogStream <<
    "% Incrementing INDENTER: " << fIndentation <<
    std::endl;
#endif

  ++fIndentation;

#ifdef DEBUG_INDENTER
  gLogStream <<
    "% INDENTER: " << fIndentation <<
    std::endl;
#endif

  return *this;
}

// decrease the indentation by 1, prefix operator
mfOutputIndenter& mfOutputIndenter::operator-- ()
{
#ifdef DEBUG_INDENTER
  gLogStream <<
    "% Decrementing INDENTER: " << fIndentation <<
    std::endl;
#endif

  --fIndentation;

  if (fIndentation < 0) {
    gLogStream <<
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
    gLogStream <<
      "% INDENTER: " << fIndentation <<
      std::endl;
  }
#endif

  return *this;
}

// increase the indentation by 1, postfix operator
mfOutputIndenter mfOutputIndenter::mfOutputIndenter::operator++ (int)
{
  ++fIndentation;

#ifdef DEBUG_INDENTER
  gLogStream <<
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
    gLogStream <<
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
    gLogStream <<
      "% INDENTER: " << fIndentation <<
      std::endl;
  }
#endif

  return *this;
}

mfOutputIndenter& mfOutputIndenter::increment (int value)
{
  fIndentation += value;

  if (fIndentation < 0) {
    gLogStream <<
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
    gLogStream <<
      "% INDENTER: " << fIndentation <<
      std::endl;
  }
#endif

  return *this;
}

mfOutputIndenter& mfOutputIndenter::decrement (int value)
{
  fIndentation -= value;

  if (fIndentation < 0) {
    gLogStream <<
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
    gLogStream <<
      "% INDENTER: " << fIndentation <<
      std::endl;
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

  std::stringstream s;

  // the std::istringstream to read lines from theString
  std::istringstream inputStream (theString);
  std::string        line;

  // append theString line by line
  while (getline (inputStream, line)) {
    s << fetchCurrentOffset () << line << std::endl;
  } // while

  return s.str ();
}

void mfOutputIndenter::print (std::ostream& os) const
{
  int i = fIndentation;

  while (i-- > 0) os << fSpacer;
}

std::ostream& operator << (std::ostream& os, const mfOutputIndenter& theIndenter) {
  theIndenter.print(os);
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
  // caution: the '\n' is present as the last character!
  size_t found = str ().find_last_not_of (' ', strSize - 2);

  // this can be uncommented to see low level informations
  // fOutputOStream << "% strSize: " << strSize << ", found: " << found << '\n';

  // output the indenter
  fOutputOStream << fOutputIndenter;

  // output the buffer
  if (found == strSize - 3) {
    // don't output the trailing spaces, but output the end of line
    fOutputOStream << str ().substr (0, found + 1) << '\n';
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

//______________________________________________________________________________
int mfIndentedStringStreamBuf::sync ()
{
  // When we sync the stream with fOutputStringStream:
  // 1) output the indentation then the buffer
  // 2) reset the buffer
  // 3) flush the actual output stream we are using.

  size_t strSize = str ().size ();

  // fetch the last non-space character in the buffer
  // caution: the '\n' is present as the last character!
  size_t found = str ().find_last_not_of (' ', strSize - 2);

  // this can be uncommented to see low level informations
  // fOutputStringStream << "% strSize: " << strSize << ", found: " << found << '\n';

  // output the indenter
  fOutputStringStream << fOutputIndenter;

  // output the buffer
  if (found == strSize - 3) {
    // don't output the trailing spaces, but output the end of line
    fOutputStringStream << str ().substr (0, found + 1) << '\n';
  }
  else {
    // output the whole buffer
    fOutputStringStream << str ();
  }

  // reset the buffer
  str ("");

  // flush the output stream
  fOutputStringStream.flush ();

  return 0;
}

//______________________________________________________________________________
// the global indented streams
S_indentedOstream gGlobalOutputIndentedOstream;
S_indentedOstream gGlobalLogIndentedOstream;

void createTheGlobalIndentedOstreams (
  std::ostream& theOutputStream,
  std::ostream& theLogStream)
{
  gGlobalOutputIndentedOstream =
    mfIndentedOstream::create (
      theOutputStream,
      mfOutputIndenter::gGlobalOStreamIndenter);

  gGlobalLogIndentedOstream =
    mfIndentedOstream::create (
      theLogStream,
      mfOutputIndenter::gGlobalOStreamIndenter);
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


}

