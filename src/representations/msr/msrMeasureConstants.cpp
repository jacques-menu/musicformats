/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msrMeasureConstants.h"

namespace MusicFormats
{

//______________________________________________________________________________
const std::string
  msrMeasureElement::K_NO_MEASURE_NUMBER = "K_NO_MEASURE_NUMBER";

const Rational
  msrMeasureElement::K_NO_WHOLE_NOTES (-444444, 1);

//______________________________________________________________________________
S_msrMeasure gNullMeasureSmartPointer;


}
