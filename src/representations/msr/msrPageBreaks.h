/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrPageBreaks___
#define ___msrPageBreaks___

#include "msrMeasureElements.h"

#include "msrTypesForwardDeclarations.h"


namespace MusicFormats
{

//______________________________________________________________________________
enum class msrUserSelectedPageBreakKind {
  kUserSelectedPageBreakYes,
  kUserSelectedPageBreakNo
};

std::string msrUserSelectedPageBreakKindAsString (
  msrUserSelectedPageBreakKind userSelectedPageBreakKind);

std::ostream& operator << (std::ostream& os, const msrUserSelectedPageBreakKind& elt);

class EXP msrPageBreak : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPageBreak> create (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            int                 nextBarPuristNumber,
                            msrUserSelectedPageBreakKind
                                                userSelectedPageBreakKind);

    static SMARTP<msrPageBreak> create (
                            int                 inputLineNumber,
                            int                 nextBarPuristNumber,
                            msrUserSelectedPageBreakKind
                                                userSelectedPageBreakKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrPageBreak (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            int                 nextBarPuristNumber,
                            msrUserSelectedPageBreakKind
                                                userSelectedPageBreakKind);

    virtual               ~msrPageBreak ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getNextBarPuristNumber () const
                              { return fNextBarPuristNumber; }

    msrUserSelectedPageBreakKind
                          getUserSelectedPageBreakKind () const
                              { return fUserSelectedPageBreakKind; }

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

    int                   fNextBarPuristNumber;

    msrUserSelectedPageBreakKind
                          fUserSelectedPageBreakKind;
};

using S_msrPageBreak = SMARTP<msrPageBreak>;

EXP std::ostream& operator << (std::ostream& os, const S_msrPageBreak& elt);


}


#endif // ___msrPageBreaks___
