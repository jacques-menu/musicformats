/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrLyrics___
#define ___lpsrLyrics___

#include "msrVoices.h"

#include "lpsrElements.h"

#include "lpsrVariables.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP lpsrNewLyricsBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrNewLyricsBlock> create (
                            int         inputLineNumber,
                            const S_msrStanza& stanza,
                            const S_msrVoice&  voice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrNewLyricsBlock (
                            int         inputLineNumber,
                            const S_msrStanza& stanza,
                            const S_msrVoice&  voice);

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

    void                  print (std::ostream& os) const override;

  private:

    // fields
    // ------------------------------------------------------

    S_msrStanza           fStanza;
    S_msrVoice            fVoice;
};
typedef SMARTP<lpsrNewLyricsBlock> S_lpsrNewLyricsBlock;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrNewLyricsBlock& elt);


}


#endif // ___lpsrLyrics___
