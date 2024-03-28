/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrOah2manPage___
#define ___lpsrOah2manPage___

#include "lpsrOah.h"

#include "oah2manPage.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP lpsrOah2manPage : virtual public oah2manPage,

// JMI  public                      visitor<S_lpsrScoreOutputKindAtom>,

  public                      visitor<S_lpsrPitchesLanguageAtom>,

  public                      visitor<S_lpsrChordsLanguageAtom>,

  public                      visitor<S_lpsrTransposeAtom>

{
  public:

                          lpsrOah2manPage (
                            const S_oahHandler& handler,
                            std::ostream&       manPageOutputStream);

    virtual               ~lpsrOah2manPage ();

  protected:

/* JMI
    virtual void              visitStart (S_lpsrScoreOutputKindAtom& elt);
    virtual void              visitEnd   (S_lpsrScoreOutputKindAtom& elt);
*/
    virtual void              visitStart (S_lpsrPitchesLanguageAtom& elt);
    virtual void              visitEnd   (S_lpsrPitchesLanguageAtom& elt);

    virtual void              visitStart (S_lpsrChordsLanguageAtom& elt);
    virtual void              visitEnd   (S_lpsrChordsLanguageAtom& elt);

    virtual void              visitStart (S_lpsrTransposeAtom& elt);
    virtual void              visitEnd   (S_lpsrTransposeAtom& elt);
};
typedef SMARTP<lpsrOah2manPage> S_lpsrOah2manPage;


}


#endif // ___lpsrOah2manPage___
