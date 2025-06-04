/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsrGraceEvents___
#define ___mxsrGraceEvents___

#include "exports.h"
#include "smartpointer.h"

#include "mxsrNoteEvents.h"


using namespace MusicXML2;

namespace MusicFormats
{

//________________________________________________________________________
enum class mxsrGraceEventKind {
  kGraceEvent_NONE,
  kGraceEventBegin, kGraceEventEnd
};

std::string mxsrGraceEventKindAsString (
  mxsrGraceEventKind noteGraceEventKind);

std::ostream& operator << (std::ostream& os, const mxsrGraceEventKind& elt);

//------------------------------------------------------------------------
class EXP mxsrGraceEvent : public mxsrNoteEvent
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<mxsrGraceEvent>
                          create (
                            mxsrGraceEventKind               graceEventKind,
                            const mxsrNoteSequentialNumber&  noteSequentialNumber,
                            const mfStaffNumber&             noteStaffNumber,
                            const mfVoiceNumber&             noteVoiceNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrGraceEvent (
                            mxsrGraceEventKind               graceEventKind,
                            const mxsrNoteSequentialNumber&  noteSequentialNumber,
                            const mfStaffNumber&             noteStaffNumber,
                            const mfVoiceNumber&             noteVoiceNumber,
                            const mxsrEventSequentialNumber& eventSequentialNumber,
                            const mfInputLineNumber&         eventInputLineNumber);

    virtual               ~mxsrGraceEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    mxsrGraceEventKind
                          getGraceEventKind () const
                              { return fGraceEventKind; }

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

    mxsrGraceEventKind    fGraceEventKind;
};

using S_mxsrGraceEvent = SMARTP<mxsrGraceEvent>;

EXP std::ostream& operator << (std::ostream& os, const S_mxsrGraceEvent& elt);
EXP std::ostream& operator << (std::ostream& os, const mxsrGraceEvent& elt);


}


#endif // ___mxsrGraceEvents___
