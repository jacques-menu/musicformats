/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsrChordEvents___
#define ___mxsrChordEvents___

// #include <iomanip>
// #include <string>

#include "exports.h"
#include "smartpointer.h"

// #include "mfBasicTypes.h"
// #include "mfBool.h"
// #include "mfIndentedTextOutput.h"
// #include "mfWrappedValueForArithmetic.h"

// #include "mxsrBasicTypes.h"
#include "mxsrNoteEvents.h"

// #include "msrTupletFactors.h"


using namespace MusicXML2;

namespace MusicFormats
{

//________________________________________________________________________
enum class mxsrChordEventKind {
  kChordEvent_NONE,
  kChordEventBegin, kChordEventEnd
};

std::string mxsrChordEventKindAsString (
  mxsrChordEventKind noteChordEventKind);

std::ostream& operator << (std::ostream& os, const mxsrChordEventKind& elt);

//------------------------------------------------------------------------
class EXP mxsrChordEvent : public mxsrNoteEvent
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrChordEvent>
                          create (
                            mxsrChordEventKind               chordEventKind,
                            const mxsrNoteSequentialNumber&  noteSequentialNumber,
                            const mfStaffNumber&             noteStaffNumber,
                            const mfVoiceNumber&             noteVoiceNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrChordEvent (
                            mxsrChordEventKind               chordEventKind,
                            const mxsrNoteSequentialNumber&  noteSequentialNumber,
                            const mfStaffNumber&             noteStaffNumber,
                            const mfVoiceNumber&             noteVoiceNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

    virtual               ~mxsrChordEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    mxsrChordEventKind    getChordEventKind () const
                              { return fChordEventKind; }

  public:

    // public services
    // ------------------------------------------------------


  private:

    // private services
    // ------------------------------------------------------


  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;
    std::string           asShortString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    mxsrChordEventKind    fChordEventKind;
};

using S_mxsrChordEvent = SMARTP<mxsrChordEvent>;

EXP std::ostream& operator << (std::ostream& os, const S_mxsrChordEvent& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrChordEvent& elt);


}


#endif // ___mxsrChordEvents___
