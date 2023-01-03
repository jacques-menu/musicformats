/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsr2musicxmlWae___
#define ___mxsr2musicxmlWae___

#include "exports.h"

#include "mfExceptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP mxsr2musicxmlException : public mfException
{
  public:
    mxsr2musicxmlException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mxsr2musicxmlException> S_mxsr2musicxmlException;


}


#endif
