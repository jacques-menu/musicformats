/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrLineBreaks___
#define ___msrLineBreaks___

#include "msrMeasureElements.h"

#include "msrTypesForwardDeclarations.h"


namespace MusicFormats
{

//______________________________________________________________________________
enum class msrUserSelectedLineBreakKind {
  kUserSelectedLineBreakYes,
  kUserSelectedLineBreakNo
};

std::string msrUserSelectedLineBreakKindAsString (
  msrUserSelectedLineBreakKind userSelectedLineBreakKind);

std::ostream& operator << (std::ostream& os, const msrUserSelectedLineBreakKind& elt);

class EXP msrLineBreak : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrLineBreak> create (
                            const mfInputLineNumber& inputLineNumber,
                            msrUserSelectedLineBreakKind
                                                userSelectedLineBreakKind);

    static SMARTP<msrLineBreak> create (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            int                 nextBarPuristNumber,
                            msrUserSelectedLineBreakKind
                                                userSelectedLineBreakKind);

    static SMARTP<msrLineBreak> create (
                            const mfInputLineNumber& inputLineNumber,
                            int                 nextBarPuristNumber,
                            msrUserSelectedLineBreakKind
                                                userSelectedLineBreakKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrLineBreak (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            int                 nextBarPuristNumber,
                            msrUserSelectedLineBreakKind
                                                userSelectedLineBreakKind);

    virtual               ~msrLineBreak ();

  public:

    // set and get
    // ------------------------------------------------------

   void                   setNextBarPuristNumber (int puristNumber)
                              { fNextBarPuristNumber = puristNumber; }

   int                    getNextBarPuristNumber () const
                              { return fNextBarPuristNumber; }

    msrUserSelectedLineBreakKind
                          getUserSelectedLineBreakKind () const
                              { return fUserSelectedLineBreakKind; }

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

    msrUserSelectedLineBreakKind
                          fUserSelectedLineBreakKind;
};

using S_msrLineBreak = SMARTP<msrLineBreak>;

EXP std::ostream& operator << (std::ostream& os, const S_msrLineBreak& elt);


}


#endif // ___msrLineBreaks___
