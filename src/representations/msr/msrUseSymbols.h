/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrUseSymbols___
#define ___msrUseSymbols___

#include <string>
#include <ostream>


namespace MusicFormats
{

// uue symbolss
//______________________________________________________________________________
enum class msrUseSymbolsKind {
  kUseSymbols_UNKNOWN_,
  kUseSymbolsNo, kUseSymbolsYes
};

std::string msrUseSymbolsKindAsString (
  msrUseSymbolsKind UseSymbolsKind);

std::ostream& operator << (std::ostream& os, const msrUseSymbolsKind& elt);

msrUseSymbolsKind msrUseSymbolsKindFromString (
  int                inputLineNumber,
  const std::string& useSymbolsString);


}

#endif // ___msrUseSymbols___
