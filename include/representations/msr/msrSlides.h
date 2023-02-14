/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrSlides___
#define ___msrSlides___

#include "msrElements.h"

#include "msrLineTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class msrSlideTypeKind {
  kSlideTypeNone,
  kSlideTypeStart, kSlideTypeStop
};

std::string msrSlideTypeKindAsString (
  msrSlideTypeKind slideTypeKind);

std::ostream& operator << (std::ostream& os, const msrSlideTypeKind& elt);

class EXP msrSlide : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSlide> create (
                            int              inputLineNumber,
                            int              slideNumber,
                            msrSlideTypeKind slideTypeKind,
                            msrLineTypeKind  slideLineTypeKind,
                            const std::string&         slideTextValue);

    SMARTP<msrSlide> createSlideNewbornClone ();

    SMARTP<msrSlide> createSlideDeepClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSlide (
                            int              inputLineNumber,
                            int              slideNumber,
                            msrSlideTypeKind slideTypeKind,
                            msrLineTypeKind  slideLineTypeKind,
                            const std::string&         slideTextValue);

    virtual               ~msrSlide ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getSlideNumber () const
                              { return fSlideNumber; }

    msrSlideTypeKind      getSlideTypeKind () const
                              { return fSlideTypeKind; }

    msrLineTypeKind       getSlideLineTypeKind () const
                              { return fSlideLineTypeKind; }

    std::string           getSlideTextValue () const
                              { return fSlideTextValue; }

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

    int                   fSlideNumber;

    msrSlideTypeKind      fSlideTypeKind;

    msrLineTypeKind       fSlideLineTypeKind;

    std::string           fSlideTextValue;
};
typedef SMARTP<msrSlide> S_msrSlide;
EXP std::ostream& operator << (std::ostream& os, const S_msrSlide& elt);


}


#endif // ___msrSlides___
