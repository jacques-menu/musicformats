/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___oah2manPage___
#define ___oah2manPage___

#include "mfEnableHarmoniesExtraSetting.h"
#ifdef MF_TRACING_IS_ENABLED
  #include "oahHarmoniesExtraOah.h"
#endif

#include "oahVisitor.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP oah2manPage : public oahVisitor
{
  public:

    oah2manPage (
      const S_oahHandler& handler,
      std::ostream&           manPageOutputStream);

    virtual               ~oah2manPage ();

  protected:

    std::ostream&              fManPageOutputStream;
};
typedef SMARTP<oah2manPage> S_oah2manPage;


}


#endif
