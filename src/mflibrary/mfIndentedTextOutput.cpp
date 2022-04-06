/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfAssert.h"

#include "mfIndentedTextOutput.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
// #define DEBUG_INDENTER

//______________________________________________________________________________
mfOutputIndenter mfOutputIndenter::gGlobalOStreamIndenter;

mfOutputIndenter::mfOutputIndenter (string spacer)
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
    endl;
#endif

  ++fIndentation;

#ifdef DEBUG_INDENTER
  gLogStream <<
    "% INDENTER: " << fIndentation <<
    endl;
#endif

  return *this;
}

// decrease the indentation by 1, prefix operator
mfOutputIndenter& mfOutputIndenter::operator-- ()
{
#ifdef DEBUG_INDENTER
  gLogStream <<
    "% Decrementing INDENTER: " << fIndentation <<
    endl;
#endif

  --fIndentation;

  if (fIndentation < 0) {
    gLogStream <<
      endl <<
      "% ### Indentation has become negative: " <<  fIndentation <<
      endl << endl;

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
      endl;
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
    endl;
#endif

  return *this;
}

// decrease the indentation by 1, postfix operator
mfOutputIndenter mfOutputIndenter::mfOutputIndenter::operator-- (int)
{
  --fIndentation;

  if (fIndentation < 0) {
    gLogStream <<
      endl <<
      "% ### Indentation has become negative: " <<  fIndentation <<
      endl << endl;

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
      endl;
  }
#endif

  return *this;
}

mfOutputIndenter& mfOutputIndenter::increment (int value)
{
  fIndentation += value;

  if (fIndentation < 0) {
    gLogStream <<
      endl <<
      "% ### Indentation has become negative: " <<  fIndentation <<
      endl << endl;

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
      endl;
  }
#endif

  return *this;
}

mfOutputIndenter& mfOutputIndenter::decrement (int value)
{
  fIndentation -= value;

  if (fIndentation < 0) {
    gLogStream <<
      endl <<
      "% ### Indentation has become negative: " <<  fIndentation <<
      endl << endl;

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
      endl;
  }
#endif

  return *this;
}

string mfOutputIndenter::indentMultiLineString (
  const string& theString)
{
  // add indentation ahead of all lines inside 'theString'
  istringstream inputStream (theString);
  string        line;
  stringstream  s;

  // print theString line by line
  while (getline (inputStream, line)) {
    if (s.str ().size ()) {
      this->print (s);
    }
    s << line;

    if (inputStream.eof ()) break;

    s << endl;
  } // while

  return s.str ();
}

string mfOutputIndenter::indentInitialSpacerIfNeededAndMultiLineString (
  const string& theString)
{
  // the initial spacer is necessary when nothing has been written
  // since the last endl

  // add indentation ahead of all lines inside 'theString'
  istringstream inputStream (theString);
  string        line;
  stringstream  s;

  // print the spacer if theStrint actually contains end of lines JMI ???
//   if (theString.find ("\n") != string::npos) {
//     s << fSpacer;
//   }

  // print theString line by line
  while (getline (inputStream, line)) {
    if (s.str ().size ()) {
      this->print (s);
    }
    s << line;

    if (inputStream.eof ()) break;

    s << endl;
  } // while

  return s.str ();
}

void mfOutputIndenter::print (ostream& os) const
{
  int i = fIndentation;

  while (i-- > 0) os << fSpacer;
}

ostream& operator<< (ostream& os, const mfOutputIndenter& theIndenter) {
  theIndenter.print(os);
  return os;
}

//______________________________________________________________________________
int mfIndentedStreamBuf::sync ()
{
  // When we sync the stream with fOutputSteam:
  // 1) output the indentation then the buffer
  // 2) reset the buffer
  // 3) flush the actual output stream we are using.

  size_t strSize = str ().size ();

  // fetch the last non-space character in the buffer
  // caution: the '\n' is present as the last character!
  size_t found = str ().find_last_not_of (' ', strSize - 2);

  // this can be uncommented to see low level informations
  // fOutputSteam << "% strSize: " << strSize << ", found: " << found << '\n';

  // output the indenter
  fOutputSteam << fOutputIndenter;

  // output the buffer
  if (found == strSize - 3) {
    // don't output the trailing spaces, but output the end of line
    fOutputSteam << str ().substr (0, found + 1) << '\n';
  }
  else {
    // output the whole buffer
    fOutputSteam << str ();
  }

  // reset the buffer
  str ("");

  // flush the output stream
  fOutputSteam.flush ();

  return 0;
}

//______________________________________________________________________________
// the global indented streams
S_indentedOstream gGlobalOutputIndentedOstream;
S_indentedOstream gGlobalLogIndentedOstream;

void createTheGlobalIndentedOstreams (
  ostream& theOutputStream,
  ostream& theLogStream)
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
template<typename Ch, typename Traits = char_traits<Ch> >
struct basic_nullbuf : basic_streambuf<Ch, Traits>
{
  typedef basic_streambuf<Ch, Traits> base_type;
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

ostream cnull  (& cnull_obj);
//wostream wcnull (& wcnull_obj);


}

