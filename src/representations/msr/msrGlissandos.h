/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrGlissandos___
#define ___msrGlissandos___

#include "msrElements.h"

#include "msrLineTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class msrGlissandoTypeKind {
  kGlissandoTypeNone,
  kGlissandoTypeStart, kGlissandoTypeStop
};

std::string msrGlissandoTypeKindAsString (
  msrGlissandoTypeKind glissandoTypeKind);

std::ostream& operator << (std::ostream& os, const msrGlissandoTypeKind& elt);

class EXP msrGlissando : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrGlissando> create (
                            int                  inputLineNumber,
                            int                  glissandoNumber,
                            msrGlissandoTypeKind glissandoTypeKind,
                            msrLineTypeKind      glissandoLineTypeKind,
                            const std::string&   glissandoTextValue);

    SMARTP<msrGlissando> createGlissandoNewbornClone ();

    SMARTP<msrGlissando> createGlissandoDeepClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrGlissando (
                            int                  inputLineNumber,
                            int                  glissandoNumber,
                            msrGlissandoTypeKind glissandoTypeKind,
                            msrLineTypeKind      glissandoLineTypeKind,
                            const std::string&   glissandoTextValue);

    virtual               ~msrGlissando ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getGlissandoNumber () const
                              { return fGlissandoNumber; }

    msrGlissandoTypeKind  getGlissandoTypeKind () const
                              { return fGlissandoTypeKind; }

    msrLineTypeKind       getGlissandoLineTypeKind () const
                              { return fGlissandoLineTypeKind; }

    std::string           getGlissandoTextValue () const
                              { return fGlissandoTextValue; }

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

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fGlissandoNumber;

    msrGlissandoTypeKind  fGlissandoTypeKind;

    msrLineTypeKind       fGlissandoLineTypeKind;

    std::string           fGlissandoTextValue;
};
typedef SMARTP<msrGlissando> S_msrGlissando;
EXP std::ostream& operator << (std::ostream& os, const S_msrGlissando& elt);


}


#endif // ___msrGlissandos___
