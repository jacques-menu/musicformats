/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfExceptions___
#define ___mfExceptions___

#include <string>
#include <ostream>

#include <exception>

#include "exports.h"
#include "smartpointer.h"


using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
EXP void mfDisplayException (std::exception& e, std::ostream& os);

//______________________________________________________________________________
class EXP mfException: public std::exception, public smartable
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfException (
                            std::string const& exceptionDescription = "",
                            int           exceptionNumber = 0,
                            int           exceptionLevel = 0
                          ) throw ();

    virtual               ~mfException () throw ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getExceptionNumber () const throw ()
                              { return fExceptionNumber; }

    int                   getExceptionLevel () const throw ()
                              { return fExceptionLevel; }

  public:

    // public services
    // ------------------------------------------------------

    virtual const char*   what () const throw ()
                              { return fExceptionDescription.c_str (); }

  private:

    std::string           fExceptionDescription;

    int                   fExceptionNumber;
    int                   fExceptionLevel;
};
typedef SMARTP<mfException> S_mfException;

//______________________________________________________________________________
class EXP mfAssertException: public mfException
{
  public:

    static SMARTP<mfAssertException> create (
                          std::string const& exceptionDescription);

                          mfAssertException (
                            std::string const& exceptionDescription) throw ();
};
typedef SMARTP<mfAssertException> S_mfAssertException;


}

#endif // ___mfExceptions___
