/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMeasureConstants___
#define ___msrMeasureConstants___

#include <string>

#include "mfRational.h"

#include "msrMeasures.h"


namespace MusicFormats
{

// ------------------------------------------------------
extern const std::string K_MEASURE_NUMBER_UNKNOWN;

extern const Rational    K_WHOLE_NOTES_UNKNOWN;

//______________________________________________________________________________
extern S_msrMeasure gGlobalNullMeasureSmartPointer;


}


#endif
