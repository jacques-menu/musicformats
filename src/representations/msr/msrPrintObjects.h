/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrPrintObjects___
#define ___msrPrintObjects___

#include <string>
#include <ostream>


namespace MusicFormats
{

// print object
//______________________________________________________________________________
enum class msrPrintObjectKind {
  kPrintObjectNone,
  kPrintObjectYes,
  kPrintObjectNo
};

std::string msrPrintObjectKindAsString (
  msrPrintObjectKind printObjectKind);

std::ostream& operator << (std::ostream& os, const msrPrintObjectKind& elt);

msrPrintObjectKind msrPrintObjectKindFromString (
  int           inputLineNumber,
  const std::string& printObjectString);


}

#endif
