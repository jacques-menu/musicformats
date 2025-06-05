/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrVoiceElements___
#define ___msrVoiceElements___

#include "msrElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
/*
  Various elements can found in voices,
  hence class   msrVoiceElement
*/

class EXP msrVoiceElement : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    // cloning
    // ------------------------------------------------------

  protected:

                          msrVoiceElement (
                            const mfInputLineNumber& inputLineNumber);

    virtual               ~msrVoiceElement ();

  /*
    The voice uplink is declared in the sub-classes,
    to allow for separate *.h files, C++ constraint
  */
};
typedef SMARTP<msrVoiceElement> S_msrVoiceElement;
EXP std::ostream& operator << (std::ostream& os, const S_msrVoiceElement& elt);


}


#endif // ___msrVoiceElements___
