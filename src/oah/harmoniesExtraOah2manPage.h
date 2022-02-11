/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___harmoniesExtraOah2manPage___
#define ___harmoniesExtraOah2manPage___

#include "enableHarmoniesExtraOahIfDesired.h"

#ifdef EXTRA_OAH_IS_ENABLED

#include "harmoniesExtraOah.h"

#include "oah2manPage.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP harmoniesExtraOah2manPage : virtual public oah2manPage,

  public visitor<S_extraShowAllHarmoniesStructuresAtom>,

  public visitor<S_extraShowAllHarmoniesContentsAtom>,

  public visitor<S_extraShowHarmonyDetailsAtom>,

  public visitor<S_extraShowHarmonyAnalysisAtom>

{
  public:

    harmoniesExtraOah2manPage (
                            const S_oahHandler handler,
                            ostream&           manPageOutputStream);

    virtual               ~harmoniesExtraOah2manPage ();

  protected:

    virtual void visitStart (S_extraShowAllHarmoniesStructuresAtom& elt);
    virtual void visitEnd   (S_extraShowAllHarmoniesStructuresAtom& elt);

    virtual void visitStart (S_extraShowAllHarmoniesContentsAtom& elt);
    virtual void visitEnd   (S_extraShowAllHarmoniesContentsAtom& elt);

    virtual void visitStart (S_extraShowHarmonyDetailsAtom& elt);
    virtual void visitEnd   (S_extraShowHarmonyDetailsAtom& elt);

    virtual void visitStart (S_extraShowHarmonyAnalysisAtom& elt);
    virtual void visitEnd   (S_extraShowHarmonyAnalysisAtom& elt);
};
typedef SMARTP<harmoniesExtraOah2manPage> S_harmoniesExtraOah2manPage;


}


#endif

#endif
