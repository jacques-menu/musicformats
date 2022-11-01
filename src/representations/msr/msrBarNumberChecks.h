/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrBarNumberChecks___
#define ___msrBarNumberChecks___

#include <list>

#include "msrBasicTypes.h"
#include "msrMeasureElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrBarNumberCheck : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBarNumberCheck> create (
                            int           inputLineNumber,
                            S_msrMeasure  upLinkToMeasure,
                            const string& nextBarOriginalNumber,
                            int           nextBarPuristNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrBarNumberCheck (
                            int           inputLineNumber,
                            S_msrMeasure  upLinkToMeasure,
                            const string& nextBarOriginalNumber,
                            int           nextBarPuristNumber);

    virtual               ~msrBarNumberCheck ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getNextBarOriginalNumber () const
                              { return fNextBarOriginalNumber; }

    int                   getNextBarPuristNumber () const
                              { return fNextBarPuristNumber; }

  public:

    // public services
    // ------------------------------------------------------

    string                asString () const override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    string                fNextBarOriginalNumber;
    int                   fNextBarPuristNumber;
};
typedef SMARTP<msrBarNumberCheck> S_msrBarNumberCheck;
EXP ostream& operator << (ostream& os, const S_msrBarNumberCheck& elt);


} // namespace MusicFormats


#endif
