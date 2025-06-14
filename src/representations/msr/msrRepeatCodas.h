/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrRepeatCodas___
#define ___msrRepeatCodas___

#include "msrElements.h"

#include "msrSegments.h"
#include "msrRepeats.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrRepeatCoda : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatCoda> create (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrSegment& segment,
                            const S_msrRepeat&  upLinkToRepeat);

    SMARTP<msrRepeatCoda> createRepeatCodaNewbornClone (
                            const S_msrRepeat& containingRepeat);

    SMARTP<msrRepeatCoda> createRepeatCodaDeepClone (
                            const S_msrRepeat& containingRepeat);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrRepeatCoda (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrSegment& segment,
                            const S_msrRepeat&  upLinkToRepeat);

    virtual               ~msrRepeatCoda ();

  public:

    // set and get
    // ------------------------------------------------------

    // segment
    S_msrSegment          getRepeatCodaSegment () const
                              { return fRepeatCodaSegment; }

    // upLinks
    S_msrRepeat           getRepeatCodaUpLinkToRepeat () const
                              { return fRepeatCodaUpLinkToRepeat; }

  public:

    // public services
    // ------------------------------------------------------

 //  JMI void                  appendElementToRepeatCoda (S_msrElement elem);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // segment
    S_msrSegment          fRepeatCodaSegment;

    // upLinks
    S_msrRepeat           fRepeatCodaUpLinkToRepeat;
};
typedef SMARTP<msrRepeatCoda> S_msrRepeatCoda;
EXP std::ostream& operator << (std::ostream& os, const S_msrRepeatCoda& elt);


}


#endif // ___msrRepeatCodas___
