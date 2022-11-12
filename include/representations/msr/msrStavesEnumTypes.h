/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrStavesEnumTypes___
#define ___msrStavesEnumTypes___

#include <string>
#include <ostream>


namespace MusicFormats
{

// staves
// ------------------------------------------------------

enum class msrStaffKind {
  kStaffKindRegular,
  kStaffKindTablature,
  kStaffKindHarmonies,
  kStaffKindFiguredBass,
  kStaffKindDrum,
  kStaffKindRythmic
};

std::string msrStaffKindAsString (
  msrStaffKind staffKind);

std::ostream& operator << (std::ostream& os, const msrStaffKind& elt);
std::string msrStaffKindAsStringForPrint (
  msrStaffKind staffKind);

// data types

enum class msrStaffChangeKind {
	kStaffChange_NO_,
	kStaffChangeChordMemberNote,
	kStaffChangeOtherNote
};

std::string msrStaffChangeKindAsString (
  msrStaffChangeKind staffChangeKind);

std::ostream& operator << (std::ostream& os, const msrStaffChangeKind& elt);

}


#endif
