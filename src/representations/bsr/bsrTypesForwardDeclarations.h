/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrTypesForwardDeclarations___
#define ___bsrTypesForwardDeclarations___

#include "smartpointer.h"


using namespace MusicXML2;

namespace MusicFormats
{

/*
#include "bsrTypesForwardDeclarations.h"
*/

//______________________________________________________________________________
class   bsrBarLine;
typedef SMARTP<bsrBarLine> S_bsrBarLine;

class   bsrClef;
typedef SMARTP<bsrClef> S_bsrClef;

class   bsrFootNotes;
typedef SMARTP<bsrFootNotes> S_bsrFootNotes;

class   bsrKey;
typedef SMARTP<bsrKey> S_bsrKey;

class   bsrLine;
typedef SMARTP<bsrLine> S_bsrLine;

class   bsrLineContents;
typedef SMARTP<bsrLineContents> S_bsrLineContents;

class   bsrMeasure;
typedef SMARTP<bsrMeasure> S_bsrMeasure;

class   bsrMusicHeading;
typedef SMARTP<bsrMusicHeading> S_bsrMusicHeading;

class   bsrNote;
typedef SMARTP<bsrNote> S_bsrNote;

class   bsrNumber;
typedef SMARTP<bsrNumber> S_bsrNumber;

class   bsrPage;
typedef SMARTP<bsrPage> S_bsrPage;

class   bsrPageHeading;
typedef SMARTP<bsrPageHeading> S_bsrPageHeading;

class   bsrScore;
typedef SMARTP<bsrScore> S_bsrScore;

class   bsrSpaces;
typedef SMARTP<bsrSpaces> S_bsrSpaces;

class   bsrTempo;
typedef SMARTP<bsrTempo> S_bsrTempo;

class   bsrTimeSignature;
typedef SMARTP<bsrTimeSignature> S_bsrTimeSignature;

class   bsrTranscriptionNotes;
typedef SMARTP<bsrTranscriptionNotes> S_bsrTranscriptionNotes;

class   bsrTranscriptionNotesElement;
typedef SMARTP<bsrTranscriptionNotesElement> S_bsrTranscriptionNotesElement;

class   bsrWords;
typedef SMARTP<bsrWords> S_bsrWords;


}


#endif
