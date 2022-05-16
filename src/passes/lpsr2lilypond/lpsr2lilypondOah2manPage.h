/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsr2lilypondOah2manPage___
#define ___lpsr2lilypondOah2manPage___

#include "lpsr2lilypondOah.h"

#include "oah2manPage.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP lpsr2lilypondOah2manPage : virtual public oah2manPage,

  public visitor<S_lilypondScoreOutputKindAtom>,

  public visitor<S_lilypondTransposePartNameAtom>,

  public visitor<S_lilypondAbsoluteOctaveEntryAtom>,

  public visitor<S_lilypondRelativeOctaveEntryAtom>,

  public visitor<S_lilypondFixedOctaveEntryAtom>,

  public visitor<S_lilypondAccidentalStyleKindAtom>,

  public visitor<S_lilypondChordsDisplayAtom>,

  public visitor<S_lilypondLyricsDurationsKindAtom>,

  public visitor<S_oahMidiTempoAtom>

{
  public:

                          lpsr2lilypondOah2manPage (
                            const S_oahHandler handler,
                            ostream&           manPageOutputStream);

    virtual               ~lpsr2lilypondOah2manPage ();

  protected:

    virtual void          visitStart (S_lilypondScoreOutputKindAtom& elt);
    virtual void          visitEnd   (S_lilypondScoreOutputKindAtom& elt);

    virtual void          visitStart (S_lilypondTransposePartNameAtom& elt);
    virtual void          visitEnd   (S_lilypondTransposePartNameAtom& elt);

    virtual void          visitStart (S_lilypondAbsoluteOctaveEntryAtom& elt);
    virtual void          visitEnd   (S_lilypondAbsoluteOctaveEntryAtom& elt);

    virtual void          visitStart (S_lilypondRelativeOctaveEntryAtom& elt);
    virtual void          visitEnd   (S_lilypondRelativeOctaveEntryAtom& elt);

    virtual void          visitStart (S_lilypondFixedOctaveEntryAtom& elt);
    virtual void          visitEnd   (S_lilypondFixedOctaveEntryAtom& elt);

    virtual void          visitStart (S_lilypondAccidentalStyleKindAtom& elt);
    virtual void          visitEnd   (S_lilypondAccidentalStyleKindAtom& elt);

    virtual void          visitStart (S_lilypondChordsDisplayAtom& elt);
    virtual void          visitEnd   (S_lilypondChordsDisplayAtom& elt);

    virtual void          visitStart (S_lilypondLyricsDurationsKindAtom& elt);
    virtual void          visitEnd   (S_lilypondLyricsDurationsKindAtom& elt);

    virtual void          visitStart (S_oahMidiTempoAtom& elt);
    virtual void          visitEnd   (S_oahMidiTempoAtom& elt);
};
typedef SMARTP<lpsr2lilypondOah2manPage> S_lpsr2lilypondOah2manPage;


}


#endif
