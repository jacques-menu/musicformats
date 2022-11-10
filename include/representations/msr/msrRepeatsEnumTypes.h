/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrRepeatsEnumTypes___
#define ___msrRepeatsEnumTypes___


namespace MusicFormats
{

// repeats
//______________________________________________________________________________
enum class msrRepeatExplicitStartKind {
  kRepeatExplicitStartNo,
  kRepeatExplicitStartYes
};

string msrRepeatExplicitStartKindAsString (
  msrRepeatExplicitStartKind repeatExplicitStartKind);

ostream& operator << (ostream& os, const msrRepeatExplicitStartKind& elt);

enum class msrRepeatBuildPhaseKind {
  kRepeatBuildPhaseJustCreated,
  kRepeatBuildPhaseInCommonPart,
  kRepeatBuildPhaseInEndings,
  kRepeatBuildPhaseCompleted
};

string repeatBuildPhaseKindAsString (
  msrRepeatBuildPhaseKind repeatBuildPhaseKind);

ostream& operator << (ostream& os, const msrRepeatBuildPhaseKind& elt);

// repeat endings
//______________________________________________________________________________
enum class msrRepeatEndingKind {
  kRepeatEndingHooked,
  kRepeatEndingHookless
};

string msrRepeatEndingKindAsString (
  msrRepeatEndingKind repeatEndingKind);

ostream& operator << (ostream& os, const msrRepeatEndingKind& elt);


}


#endif
