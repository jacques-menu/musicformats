/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrTupletsForwardDeclarations___
#define ___msrTupletsForwardDeclarations___

#include "smartpointer.h"


using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
class   msrTupletElement;
typedef SMARTP<msrTupletElement> S_msrTupletElement;

class   msrTuplet;
typedef SMARTP<msrTuplet> S_msrTuplet;

class   msrNote;
typedef SMARTP<msrNote> S_msrNote;

class   msrChord;
typedef SMARTP<msrChord> S_msrChord;


}


#endif // ___msrTupletsForwardDeclarations___
