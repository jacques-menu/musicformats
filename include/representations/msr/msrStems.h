/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrStems___
#define ___msrStems___

#include "msrElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class msrStemKind {
  kStemNeutral,
  kStemUp, kStemDown, kStemDouble
};

string msrStemKindAsString (
  msrStemKind stemKind);

ostream& operator << (ostream& os, const msrStemKind& elt);

class EXP msrStem : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStem> create (
      int         inputLineNumber,
      msrStemKind stemKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrStem (
                            int         inputLineNumber,
                            msrStemKind stemKind);

    virtual               ~msrStem ();

  public:

    // set and get
    // ------------------------------------------------------

    msrStemKind           getStemKind () const
                              { return fStemKind; }

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

    msrStemKind           fStemKind;
};
typedef SMARTP<msrStem> S_msrStem;
EXP ostream& operator << (ostream& os, const S_msrStem& elt);


}


#endif
