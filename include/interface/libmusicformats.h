/*
  MusicXML Library
  Copyright (C) Grame 2006-2020

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __libmusicformats__
#define __libmusicformats__

#include <string>
#include <vector>

#include "exports.h"


namespace MusicXML2
{

enum class musicFormatsError {
  k_NoError, kErrorInvalidFile, kErrorInvalidOption, kErrorUnsupported
};

EXP std::string musicFormatsErrorAsString (musicFormatsError err);

#ifdef __cplusplus
extern "C" {
#endif

/*!
  \brief Gives the library version number.
  \return the library version number as a float value
*/
EXP float         libmusicformatsVersion();
/*!
  \brief Gives the library version as a string.
  \return a string
*/
EXP const char*   libmusicformatsVersionStr();


//_______________________________________________________________________________
/*
  A type to pass options and their values if any to the functions in the interfaces

  In the pair, first is the name of the option, such as '-x',
  and second is the value of the option
*/

typedef std::vector<std::pair<std::string, std::string> > optionsVector;

//_______________________________________________________________________________


#ifdef __cplusplus
}
#endif

}


#endif
