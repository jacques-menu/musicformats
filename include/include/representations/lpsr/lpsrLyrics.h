/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrLyrics___
#define ___lpsrLyrics___

#include "lpsrElements.h"

#include "msr.h"

#include "lpsrVariables.h"


namespace MusicFormats
{

//______________________________________________________________________________
class msrStanza;
typedef SMARTP<msrStanza> S_msrStanza;

//______________________________________________________________________________
class EXP lpsrNewLyricsBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrNewLyricsBlock> create (
                            int         inputLineNumber,
                            S_msrStanza stanza,
                            S_msrVoice  voice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrNewLyricsBlock (
                            int         inputLineNumber,
                            S_msrStanza stanza,
                            S_msrVoice  voice);

    virtual               ~lpsrNewLyricsBlock ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrStanza           getStanza () const { return fStanza; }
    S_msrVoice            getVoice  () const { return fVoice; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

  private:

    // fields
    // ------------------------------------------------------

    S_msrStanza           fStanza;
    S_msrVoice            fVoice;
};
typedef SMARTP<lpsrNewLyricsBlock> S_lpsrNewLyricsBlock;
EXP ostream& operator << (ostream& os, const S_lpsrNewLyricsBlock& elt);


}


#endif
