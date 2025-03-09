/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrRepeatsEnumTypes___
#define ___msrRepeatsEnumTypes___

#include <string>
#include <ostream>

namespace MusicFormats
{

// repeats
//______________________________________________________________________________
enum class msrRepeatExplicitStartKind {
  kRepeatExplicitStartUnknown,
  kRepeatExplicitStartNo,
  kRepeatExplicitStartYes
};

std::string msrRepeatExplicitStartKindAsString (
  msrRepeatExplicitStartKind repeatExplicitStartKind);

std::ostream& operator << (std::ostream& os, const msrRepeatExplicitStartKind& elt);

enum class msrRepeatBuildPhaseKind {
  kRepeatBuildPhaseJustCreated,
  kRepeatBuildPhaseInCommonPart,
  kRepeatBuildPhaseInEndings,
  kRepeatBuildPhaseCompleted
};

std::string repeatBuildPhaseKindAsString (
  msrRepeatBuildPhaseKind repeatBuildPhaseKind);

std::ostream& operator << (std::ostream& os, const msrRepeatBuildPhaseKind& elt);

// repeat endings
//______________________________________________________________________________
enum class msrRepeatEndingKind {
  kRepeatEndingHooked,
  kRepeatEndingHookless
};

std::string msrRepeatEndingKindAsString (
  msrRepeatEndingKind repeatEndingKind);

std::ostream& operator << (std::ostream& os, const msrRepeatEndingKind& elt);


}


#endif // ___msrRepeatsEnumTypes___
