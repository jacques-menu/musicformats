/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrBarChecks___
#define ___msrBarChecks___

#include <list>

#include "msrMeasureElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
// PRE-declarations for classes mutual dependencies
// class msrMeasure;
// typedef SMARTP<msrMeasure> S_msrMeasure;
//
// class msrMeasureElement;
// typedef SMARTP<msrMeasureElement> S_msrMeasureElement;

//______________________________________________________________________________
class EXP msrBarCheck : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBarCheck> create (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure);

    static SMARTP<msrBarCheck> createWithNextBarPuristNumber ( // JMI superflous??? v0.9.66
                            int           inputLineNumber,
                            S_msrMeasure  upLinkToMeasure,
                            const string& nextBarOriginalNumber,
                            int           nextBarPuristNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrBarCheck (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure);

                          msrBarCheck (
                            int           inputLineNumber,
                            S_msrMeasure  upLinkToMeasure,
                            const string& nextBarOriginalNumber,
                            int           nextBarPuristNumber);

    virtual               ~msrBarCheck ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getNextBarOriginalNumber () const
                              { return fNextBarOriginalNumber; }

    void                  setNextBarPuristNumber (int puristNumber);

    int                   getNextBarPuristNumber () const
                              { return fNextBarPuristNumber; }

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

    string                fNextBarOriginalNumber;
    int                   fNextBarPuristNumber;
};
typedef SMARTP<msrBarCheck> S_msrBarCheck;
EXP ostream& operator << (ostream& os, const S_msrBarCheck& elt);


} // namespace MusicFormats


#endif
