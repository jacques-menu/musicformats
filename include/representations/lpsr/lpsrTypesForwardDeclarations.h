/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrTypesForwardDeclarations___
#define ___lpsrTypesForwardDeclarations___

#include "smartpointer.h"


using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
class   lpsrBarCommand;
typedef SMARTP<lpsrBarCommand> S_lpsrBarCommand;

class   msrBarre;
typedef SMARTP<msrBarre> S_msrBarre;

class   lpsrBookBlock;
typedef SMARTP<lpsrBookBlock> S_lpsrBookBlock;

class   lpsrComment;
typedef SMARTP<lpsrComment> S_lpsrComment;

class   lpsrBookPartBlock;
typedef SMARTP<lpsrBookPartBlock> S_lpsrBookPartBlock;

class   lpsrChordNamesContext;
typedef SMARTP<lpsrChordNamesContext> S_lpsrChordNamesContext;

class   lpsrElement;
typedef SMARTP<lpsrElement> S_lpsrElement;

class   lpsrFiguredBassContext;
typedef SMARTP<lpsrFiguredBassContext> S_lpsrFiguredBassContext;

class   lpsrHeader;
typedef SMARTP<lpsrHeader> S_lpsrHeader;

class   lpsrLayout;
typedef SMARTP<lpsrLayout> S_lpsrLayout;

class   lpsrNewLyricsBlock;
typedef SMARTP<lpsrNewLyricsBlock> S_lpsrNewLyricsBlock;

class   lpsrPaper;
typedef SMARTP<lpsrPaper> S_lpsrPaper;

class   lpsrParallelMusicBLock;
typedef SMARTP<lpsrParallelMusicBLock> S_lpsrParallelMusicBLock;

class   lpsrPartBlock;
typedef SMARTP<lpsrPartBlock> S_lpsrPartBlock;

class   lpsrPartGroupBlock;
typedef SMARTP<lpsrPartGroupBlock> S_lpsrPartGroupBlock;

class   lpsrSchemeFunction;
typedef SMARTP<lpsrSchemeFunction> S_lpsrSchemeFunction;

class   lpsrSchemeVariable;
typedef SMARTP<lpsrSchemeVariable> S_lpsrSchemeVariable;

class   lpsrScore;
typedef SMARTP<lpsrScore> S_lpsrScore;

class   lpsrScoreBlock;
typedef SMARTP<lpsrScoreBlock> S_lpsrScoreBlock;

class   lpsrStaffBlock;
typedef SMARTP<lpsrStaffBlock> S_lpsrStaffBlock;

class   lpsrUseVoiceCommand;
typedef SMARTP<lpsrUseVoiceCommand> S_lpsrUseVoiceCommand;

class   lpsrVariableUseCommand;
typedef SMARTP<lpsrVariableUseCommand> S_lpsrVariableUseCommand;


}


#endif
