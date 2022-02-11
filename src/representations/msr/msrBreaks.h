/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrBreaks___
#define ___msrBreaks___

#include "msrBasicTypes.h"
#include "msrMeasureElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
enum class msrUserChosenLineBreakKind {
  kUserChosenLineBreakYes,
  kUserChosenLineBreakNo
};

string userChosenLineBreakKindAsString (
  msrUserChosenLineBreakKind userChosenLineBreakKind);

class EXP msrLineBreak : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrLineBreak> create (
                            int           inputLineNumber,
                            const string& nextBarNumber,
                            msrUserChosenLineBreakKind
                                          userChosenLineBreakKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrLineBreak (
                            int           inputLineNumber,
                            const string& nextBarNumber,
                            msrUserChosenLineBreakKind
                                          userChosenLineBreakKind);

    virtual               ~msrLineBreak ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getNextBarNumber () const
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

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    string                fNextBarNumber;

    msrUserChosenLineBreakKind
                          fUserChosenLineBreakKind;
};
typedef SMARTP<msrLineBreak> S_msrLineBreak;
EXP ostream& operator<< (ostream& os, const S_msrLineBreak& elt);

//______________________________________________________________________________
enum class msrUserChosenPageBreakKind {
  kUserChosenPageBreakYes,
  kUserChosenPageBreakNo
};

string userChosenPageBreakKindAsString (
  msrUserChosenPageBreakKind userChosenPageBreakKind);

class EXP msrPageBreak : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPageBreak> create (
                            int   inputLineNumber,
                            msrUserChosenPageBreakKind
                                  userChosenPageBreakKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrPageBreak (
                            int   inputLineNumber,
                            msrUserChosenPageBreakKind
                                  userChosenPageBreakKind);

    virtual               ~msrPageBreak ();

  public:

    // set and get
    // ------------------------------------------------------

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

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrUserChosenPageBreakKind
                          fUserChosenPageBreakKind;
};
typedef SMARTP<msrPageBreak> S_msrPageBreak;
EXP ostream& operator<< (ostream& os, const S_msrPageBreak& elt);


} // namespace MusicFormats


#endif
