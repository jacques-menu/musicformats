/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrOah2manPage___
#define ___msrOah2manPage___

#include "msrOah.h"
#include "msr2msrOah.h"

#include "oah2manPage.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP msrOah2manPage : virtual public oah2manPage,

  public visitor<S_msrRenamePartAtom>,

  public visitor<S_msrIgnorePartAtom>,

  public visitor<S_msrKeepPartAtom>,

  public visitor<S_msrPitchesLanguageAtom>

{
  public:

                          msrOah2manPage (
                            const S_oahHandler& handler,
                            std::ostream&           manPageOutputStream);

    virtual               ~msrOah2manPage ();

  protected:

    virtual void          visitStart (S_msrRenamePartAtom& elt);
    virtual void          visitEnd   (S_msrRenamePartAtom& elt);

    virtual void          visitStart (S_msrIgnorePartAtom& elt);
    virtual void          visitEnd   (S_msrIgnorePartAtom& elt);

    virtual void          visitStart (S_msrKeepPartAtom& elt);
    virtual void          visitEnd   (S_msrKeepPartAtom& elt);

    virtual void          visitStart (S_msrPitchesLanguageAtom& elt);
    virtual void          visitEnd   (S_msrPitchesLanguageAtom& elt);
};
typedef SMARTP<msrOah2manPage> S_msrOah2manPage;


}


#endif
