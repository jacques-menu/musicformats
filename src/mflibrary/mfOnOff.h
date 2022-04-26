/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfOnOff___
#define ___mfOnOff___

#include <string>
#include <ostream>

#include "exports.h"

#include "mfBool.h"

using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
enum class mfOnOffKind {
  kMfOnOffUnknown,
  kMfOnOffOn, kMfOnOffOff
};

Bool mfOnOffKindAsBool (
  mfOnOffKind onOffKind);

string mfOnOffKindAsString (
  mfOnOffKind onOffKind);

ostream& operator<< (ostream& os, const mfOnOffKind elt);


}


#endif
