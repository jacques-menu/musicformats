/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrSlurs___
#define ___msrSlurs___

#include "msrElements.h"

#include "msrLineTypes.h"
#include "msrPlacements.h"


namespace MusicFormats
{

// slurs
// ------------------------------------------------------

enum class msrSlurTypeKind {
  kSlurType_UNKNOWN_,

  kSlurTypeRegularStart, kSlurTypeRegularContinue, kSlurTypeRegularStop,

  kSlurTypePhrasingStart, kSlurTypePhrasingContinue, kSlurTypePhrasingStop
};

std::string msrSlurTypeKindAsString (
  msrSlurTypeKind slurTypeKind);

std::ostream& operator << (std::ostream& os, const msrSlurTypeKind& elt);

//______________________________________________________________________________
class EXP msrSlur : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSlur> create (
                            int              inputLineNumber,
                            int              slurNumber,
                            msrSlurTypeKind  slurTypeKind,
                            msrLineTypeKind  slurLineTypeKind,
                            msrPlacementKind slurPlacementKind);

    static SMARTP<msrSlur> create (
                            int              inputLineNumber,
                            int              slurNumber,
                            msrSlurTypeKind  slurTypeKind,
                            msrLineTypeKind  slurLineTypeKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSlur (
                            int              inputLineNumber,
                            int              slurNumber,
                            msrSlurTypeKind  slurTypeKind,
                            msrLineTypeKind  slurLineTypeKind,
                            msrPlacementKind slurPlacementKind);

    virtual               ~msrSlur ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setSlurNumber (int slurNumber)
                              { fSlurNumber = slurNumber; }

    int                   getSlurNumber () const
                              { return fSlurNumber; }

    void                  setSlurTypeKind (msrSlurTypeKind slurTypeKind)
                              { fSlurTypeKind = slurTypeKind; }

    msrSlurTypeKind       getSlurTypeKind () const
                              { return fSlurTypeKind; }

    void                  setSlurLineTypeKind (msrLineTypeKind slurLineTypeKind)
                              { fSlurLineTypeKind = slurLineTypeKind; }

    msrLineTypeKind       getSlurLineTypeKind () const
                              { return fSlurLineTypeKind; }

    void                  setSlurPlacementKind (msrPlacementKind slurPlacementKind)
                              { fSlurPlacementKind = slurPlacementKind; }

    msrPlacementKind      getSlurPlacementKind () const
                              { return fSlurPlacementKind; }

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

    int                   fSlurNumber;

    msrSlurTypeKind       fSlurTypeKind;

    msrLineTypeKind       fSlurLineTypeKind;

    msrPlacementKind      fSlurPlacementKind;
};
typedef SMARTP<msrSlur> S_msrSlur;
EXP std::ostream& operator << (std::ostream& os, const S_msrSlur& elt);


}


#endif // ___msrSlurs___
