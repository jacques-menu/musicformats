/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrBreaks___
#define ___msrBreaks___

#include "msrMeasureElements.h"

#include "msrTypesForwardDeclarations.h"


namespace MusicFormats
{

//______________________________________________________________________________
enum class msrUserChosenLineBreakKind {
  kUserChosenLineBreakYes,
  kUserChosenLineBreakNo
};

std::string msrUserChosenLineBreakKindAsString (
  msrUserChosenLineBreakKind userChosenLineBreakKind);

std::ostream& operator << (std::ostream& os, const msrUserChosenLineBreakKind& elt);

class EXP msrLineBreak : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrLineBreak> create (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            const std::string&  nextBarNumber,
                            msrUserChosenLineBreakKind
                                                userChosenLineBreakKind);

    static SMARTP<msrLineBreak> create (
                            int                 inputLineNumber,
                            const std::string&  nextBarNumber,
                            msrUserChosenLineBreakKind
                                                userChosenLineBreakKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrLineBreak (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            const std::string&  nextBarNumber,
                            msrUserChosenLineBreakKind
                                                userChosenLineBreakKind);

    virtual               ~msrLineBreak ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMeasureElementUpLinkToMeasure (
                            const S_msrMeasure& measure) override
                              { setLineBreakUpLinkToMeasure (measure); }

    void                  getMeasureElementUpLinkToMeasure (
                            S_msrMeasure& upLinkToMeasure) const override
                            { upLinkToMeasure = getLineBreakUpLinkToMeasure (); }

    void                  setLineBreakUpLinkToMeasure (
                            const S_msrMeasure& measure);

    S_msrMeasure          getLineBreakUpLinkToMeasure () const
                            { return fLineBreakUpLinkToMeasure; }

    std::string           getNextBarNumber () const
                              { return fNextBarNumber; }

    msrUserChosenLineBreakKind
                          getUserChosenLineBreakKind () const
                              { return fUserChosenLineBreakKind; }

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

    S_msrMeasure          fLineBreakUpLinkToMeasure;

    std::string           fNextBarNumber;

    msrUserChosenLineBreakKind
                          fUserChosenLineBreakKind;
};
typedef SMARTP<msrLineBreak> S_msrLineBreak;
EXP std::ostream& operator << (std::ostream& os, const S_msrLineBreak& elt);

//______________________________________________________________________________
enum class msrUserChosenPageBreakKind {
  kUserChosenPageBreakYes,
  kUserChosenPageBreakNo
};

std::string msrUserChosenPageBreakKindAsString (
  msrUserChosenPageBreakKind userChosenPageBreakKind);

std::ostream& operator << (std::ostream& os, const msrUserChosenPageBreakKind& elt);

class EXP msrPageBreak : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPageBreak> create (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            msrUserChosenPageBreakKind
                                                userChosenPageBreakKind);

    static SMARTP<msrPageBreak> create (
                            int                 inputLineNumber,
                            msrUserChosenPageBreakKind
                                                userChosenPageBreakKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrPageBreak (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            msrUserChosenPageBreakKind
                                                userChosenPageBreakKind);

    virtual               ~msrPageBreak ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMeasureElementUpLinkToMeasure (
                            const S_msrMeasure& measure) override
                              { setPageBreakUpLinkToMeasure (measure); }

    void                  getMeasureElementUpLinkToMeasure (
                            S_msrMeasure& upLinkToMeasure) const override
                            { upLinkToMeasure = getPageBreakUpLinkToMeasure (); }

    void                  setPageBreakUpLinkToMeasure (
                            const S_msrMeasure& measure);

    S_msrMeasure          getPageBreakUpLinkToMeasure () const
                            { return fPageBreakUpLinkToMeasure; }

    msrUserChosenPageBreakKind
                          getUserChosenPageBreakKind () const
                              { return fUserChosenPageBreakKind; }

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

    S_msrMeasure          fPageBreakUpLinkToMeasure;

    msrUserChosenPageBreakKind
                          fUserChosenPageBreakKind;
};
typedef SMARTP<msrPageBreak> S_msrPageBreak;
EXP std::ostream& operator << (std::ostream& os, const S_msrPageBreak& elt);


}


#endif
