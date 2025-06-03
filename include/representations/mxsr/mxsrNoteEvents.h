/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsrNoteEvents___
#define ___mxsrNoteEvents___

// #include <iomanip>
// #include <string>

#include "exports.h"
#include "smartpointer.h"

#include "mfBasicTypes.h"
// #include "mfBool.h"
// #include "mfIndentedTextOutput.h"
// #include "mfWrappedValueForArithmetic.h"

#include "mxsrBasicTypes.h"
#include "mxsrEvents.h"

// #include "msrTupletFactors.h"


using namespace MusicXML2;

namespace MusicFormats
{

//------------------------------------------------------------------------
class EXP mxsrNoteEvent : public mxsrEvent
{
/* this class is purely virtual
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrNoteEvent>
                          create (
                            const mxsrNoteSequentialNumber&  noteSequentialNumber,
                            const mfStaffNumber&             noteStaffNumber,
                            const mfVoiceNumber&             noteVoiceNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);
*/

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrNoteEvent (
                            const mxsrNoteSequentialNumber&  noteSequentialNumber,
                            const mfStaffNumber&             noteStaffNumber,
                            const mfVoiceNumber&             noteVoiceNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

    virtual               ~mxsrNoteEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    const mxsrNoteSequentialNumber&
                          getNoteSequentialNumber () const
                              { return fNoteSequentialNumber; }

    const mfStaffNumber&  getNoteStaffNumber () const
                              { return fNoteStaffNumber; }

    const mfVoiceNumber&  getNoteVoiceNumber () const
                              { return fNoteVoiceNumber; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

//     std::string           asString () const override;
//     std::string           asShortString () const override;
//
//     void                  print (std::ostream& os) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    mxsrNoteSequentialNumber
                          fNoteSequentialNumber;

    // the staff and voice numbers of the note upon which the event occurs
    mfStaffNumber         fNoteStaffNumber;
    mfVoiceNumber         fNoteVoiceNumber;
};

using S_mxsrNoteEvent = SMARTP<mxsrNoteEvent>;

// EXP std::ostream& operator << (std::ostream& os, const S_mxsrNoteEvent& elt);
// EXP std::ostream& operator << (std::ostream& os, const mxsrNoteEvent& elt);


}


#endif // ___mxsrNoteEvents___
