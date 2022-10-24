/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrBeams___
#define ___msrBeams___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrBeam : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBeam> create (
                            int         inputLineNumber,
                            int         number,
                            msrBeamKind beamKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrBeam (
                            int         inputLineNumber,
                            int         number,
                            msrBeamKind beamKind);

    virtual               ~msrBeam ();

  public:

    // set and get
    // ------------------------------------------------------

    msrBeamKind           getBeamKind   () const
                              { return fBeamKind; }

    int                   getBeamNumber () const
                              { return fBeamNumber; }

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

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fBeamNumber;
    msrBeamKind           fBeamKind;
};
typedef SMARTP<msrBeam> S_msrBeam;
EXP ostream& operator << (ostream& os, const S_msrBeam& elt);


} // namespace MusicFormats


#endif
