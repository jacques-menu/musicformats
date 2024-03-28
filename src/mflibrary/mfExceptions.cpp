/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>

#include "mfPreprocessorSettings.h"

#include "mfExceptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
EXP void mfDisplayException (std::exception& e, std::ostream& os)
{
#ifdef MF_CAUGHT_EXCEPTIONS_DISPLAY_IS_ENABLED
  os <<
    "std::exception caught: " <<
    e.what () <<
    std::endl;
#endif // MF_CAUGHT_EXCEPTIONS_DISPLAY_IS_ENABLED
}

//______________________________________________________________________________
mfException::mfException (
  std::string const& exceptionDescription,
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
  std::string const& exceptionDescription) throw ()
  : mfException (exceptionDescription)
{}

S_mfAssertException mfAssertException::create (
  std::string const& exceptionDescription)
{
  mfAssertException* obj = new
    mfAssertException (
      exceptionDescription);
  assert (obj!=0);

  return obj;
}


}
