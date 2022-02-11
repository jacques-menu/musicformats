/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>

#include "mfExceptions.h"

#include "enebleCaughtExceptionsDisplay.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
EXP void mfDisplayException (exception& e, ostream& os)
{
#ifdef CAUGHT_EXCEPTIONS_DISPLAY_IS_ENABLED
  os <<
    "Exception caught: " <<
    e.what () <<
    endl;
#endif
}

//______________________________________________________________________________
mfException::mfException (
  string const& exceptionDescription,
  int           exceptionNumber,
  int           exceptionLevel) throw ()
  : fExceptionDescription (
      "mfException: " + exceptionDescription),
    fExceptionNumber (exceptionNumber),
    fExceptionLevel (exceptionLevel)
{}

mfException::~mfException () throw ()
{}


//______________________________________________________________________________
mfAssertException::mfAssertException (
  string const& exceptionDescription) throw ()
  : mfException (exceptionDescription)
{}

S_mfAssertException mfAssertException::create (
  string const& exceptionDescription)
{
  mfAssertException* o = new
    mfAssertException (
      exceptionDescription);
  assert (o!=0);

  return o;
}


}
