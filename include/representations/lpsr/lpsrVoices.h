/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrVoices___
#define ___lpsrVoices___

#include <iostream>

#include "lpsrElements.h"

#include "msrVoices.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP lpsrUseVoiceCommand : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrUseVoiceCommand> create (
                            int        inputLineNumber,
                            const S_msrVoice& voice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrUseVoiceCommand (
                            int        inputLineNumber,
                            const S_msrVoice& voice);

    virtual               ~lpsrUseVoiceCommand ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrVoice            getVoice () const { return fVoice; }

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

    S_msrVoice            fVoice;
};
typedef SMARTP<lpsrUseVoiceCommand> S_lpsrUseVoiceCommand;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrUseVoiceCommand& elt);


}


#endif // ___lpsrVoices___
