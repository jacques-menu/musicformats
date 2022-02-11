/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfIndentedTextOutput___
#define ___mfIndentedTextOutput___

#include <sstream>   // for stringbuf

#include "exports.h"
#include "smartpointer.h"

#include "mfBool.h"


using namespace std;
using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP mfOutputIndenter
{
  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfOutputIndenter (string spacer = "  ");

    virtual               ~mfOutputIndenter ();

  public:

    // set and get
    // ------------------------------------------------------

    // set the indent
    void                  setIndent (int indent)
                              { fIndent = indent; }

    // get the indent
    int                   getIndent () const
                              { return fIndent; }

  public:

    // public services
    // ------------------------------------------------------

/*
    // Declare prefix and postfix decrement operators.
    Point& operator--();       // Prefix decrement operator.
    Point operator--(int);     // Postfix decrement operator.
*/

    // increase the indentation by 1, prefix operator
    mfOutputIndenter&       operator++ ();

    // decrease the indentation by 1, prefix operator
    mfOutputIndenter&       operator-- ();

    // increase the indentation by 1, postfix operator
    mfOutputIndenter        operator++ (int);
    // decrease the indentation by 1, postfix operator
    mfOutputIndenter        operator-- (int);

    mfOutputIndenter&       increment (int value);
    mfOutputIndenter&       decrement (int value);

    // reset the indentation
    void                  resetToZero ()
                              { fIndent = 0; }

    // compare indentation value
    Bool                  operator == (const int &value) const
                              { return fIndent == value; }
    Bool                  operator != (const int &value) const
                              { return fIndent != value; }

    // output as much space as specified
    void                  print (ostream& os) const;

    // get a spacer for adhoc uses, without increasing the indentation
    string                getSpacer () const
                              { return fSpacer; }

    // indent a multiline 'R"(...)"' string
    string                indentMultiLineString (
                            const string& theString);
    string                indentInitialSpacerIfNeededAndMultiLineString (
                            const string& theString);

    // global variables for general use
    static mfOutputIndenter gGlobalOStreamIndenter;

  private:
    int                   fIndent;
    string                fSpacer;
};

EXP ostream& operator<< (ostream& os, const mfOutputIndenter& theIndenter);

// useful shortcut macros
#define gIndenter mfOutputIndenter::gGlobalOStreamIndenter
#define gTab      mfOutputIndenter::gGlobalOStreamIndenter.getSpacer ()

//______________________________________________________________________________
// a stream buffer that prefixes each line
// with the current indentation, i.e. spaces

/*
endl declaration:

  endl for ostream
  ostream& endl (ostream& os);

  basic template
  template <class charT, class traits>
  basic_ostream<charT,traits>& endl (basic_ostream<charT,traits>& os);

  Insert newline and flush
  Inserts a new-line character and flushes the stream.

  Its behavior is equivalent to calling os.put('\n') (or os.put(os.widen('\n')) for character types other than char), and then os.flush().

--

Reference for this class:
  https://stackoverflow.com/questions/2212776/overload-handling-of-stdendl
*/

class EXP mfIndentedStreamBuf: public stringbuf
{
  private:

    ostream&              fOutputSteam;
    mfOutputIndenter&       fOutputIndenter;

  public:

    // constructor
                          mfIndentedStreamBuf (
                            ostream&   outputStream,
                            mfOutputIndenter& theIndenter)
                            : fOutputSteam (
                                outputStream),
                              fOutputIndenter (
                                theIndenter)
                              {}

    // indentation
    mfOutputIndenter&       getOutputIndenter () const
                              { return fOutputIndenter; }

    // flush
    void                  flush ()
                              { fOutputSteam.flush (); }

    virtual int           sync ();
};

//______________________________________________________________________________
class EXP mfIndentedOstream: public ostream, public smartable
{
/*
Reference for this class:
  https://stackoverflow.com/questions/2212776/overload-handling-of-stdendl

Usage:
  mfIndentedOstream myStream (cout);

  myStream <<
    1 << 2 << 3 << endl <<
    5 << 6 << endl <<
    7 << 8 << endl;
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfIndentedOstream> create (
                            ostream&        theOStream,
                            mfOutputIndenter& theIndenter)
                              {
                                mfIndentedOstream* o = new mfIndentedOstream (
                                  theOStream,
                                  theIndenter);
                                assert (o != nullptr);

                                return o;
                              }

    // constructors/destructor
    // ------------------------------------------------------

                          mfIndentedOstream (
                            ostream&        theOStream,
                            mfOutputIndenter& theIndenter)
                            : ostream (
                                & fIndentedStreamBuf),
                              fIndentedStreamBuf (
                                theOStream,
                                theIndenter)
                              {}

    virtual               ~mfIndentedOstream () {};

  public:

    // public services
    // ------------------------------------------------------

    // flush
    void                  flush ()
                              { fIndentedStreamBuf.flush (); }

    // indentation
    mfOutputIndenter&       getIndenter () const
                              { return fIndentedStreamBuf.getOutputIndenter (); }

    void                  incrIdentation ()
                              { ++ (fIndentedStreamBuf.getOutputIndenter ()); }

    void                  decrIdentation ()
                              { -- (fIndentedStreamBuf.getOutputIndenter ()); }

  private:

    // private fields
    // ------------------------------------------------------

    // mfIndentedOstream just uses an mfIndentedStreamBuf
    mfIndentedStreamBuf     fIndentedStreamBuf;

};
typedef SMARTP<mfIndentedOstream> S_indentedOstream;

//______________________________________________________________________________
// the global log indented stream
EXP extern S_indentedOstream gGlobalOutputIndentedOstream;
EXP extern S_indentedOstream gGlobalLogIndentedOstream;

#define gOutputStream *gGlobalOutputIndentedOstream
#define gLogStream    *gGlobalLogIndentedOstream

EXP extern void createTheGlobalIndentedOstreams (
  ostream& theOutputStream,
  ostream& theLogStream);


}


#endif
