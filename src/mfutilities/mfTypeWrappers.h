/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfWrappedRange___
#define ___mfWrappedRange___

#include <string>
#include <ostream>

#include "exports.h"

#include "mfBool.h"


namespace MusicFormats
{

/*
  Built-in types such a int and std::string
  don't bear any application-specific semantics by themselve.

  The lack of type distinction between an int staff number and an int voice number
  easily lead to potential assignment and parameter passing errors.

  As a self-defensive measure, wrapping bare built-in types such as int and std::string
  in distinct classes without implicit conversions
  helps trapping such errors at compile time.

  Doing so with template classes is also a nice way to:
    -  provide a default bare values for variables of these types,
       such as 0 for an input line number;
    -  print such values with a prefix and postfix,
       to avoid taking care of that everywhere in the code.
       Incidentally, these values specified in the template definition
       help disambiguate these template class types in C++
*/


}


#endif // ___mfWrappedRange___
