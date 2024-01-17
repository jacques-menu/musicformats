/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___harmoniesExtraOah2manPage___
#define ___harmoniesExtraOah2manPage___

#include "mfPreprocessorSettings.h"


#include "oahHarmoniesExtraOah.h"

#include "oah2manPage.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP harmoniesExtraOah2manPage : virtual public oah2manPage,

  public                      visitor<S_extraDisplayAllHarmoniesStructuresAtom>,

  public                      visitor<S_extraDisplayAllHarmoniesContentsAtom>,

  public                      visitor<S_extraDisplayHarmonyDetailsAtom>,

  public                      visitor<S_extraDisplayHarmonyAnalysisAtom>

{
  public:

                          harmoniesExtraOah2manPage (
                            const S_oahHandler& handler,
                            std::ostream&       manPageOutputStream);

    virtual               ~harmoniesExtraOah2manPage ();

  protected:

    virtual void              visitStart (S_extraDisplayAllHarmoniesStructuresAtom& elt);
    virtual void              visitEnd   (S_extraDisplayAllHarmoniesStructuresAtom& elt);

    virtual void              visitStart (S_extraDisplayAllHarmoniesContentsAtom& elt);
    virtual void              visitEnd   (S_extraDisplayAllHarmoniesContentsAtom& elt);

    virtual void              visitStart (S_extraDisplayHarmonyDetailsAtom& elt);
    virtual void              visitEnd   (S_extraDisplayHarmonyDetailsAtom& elt);

    virtual void              visitStart (S_extraDisplayHarmonyAnalysisAtom& elt);
    virtual void              visitEnd   (S_extraDisplayHarmonyAnalysisAtom& elt);
};
typedef SMARTP<harmoniesExtraOah2manPage> S_harmoniesExtraOah2manPage;


}




#endif // ___harmoniesExtraOah2manPage___
