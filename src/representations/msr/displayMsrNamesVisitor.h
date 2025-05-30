/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___displayMsrNamesVisitor___
#define ___displayMsrNamesVisitor___

#include "visitor.h"

#include "msrOah.h"


using namespace MusicXML2;

namespace MusicFormats
{

//________________________________________________________________________
class EXP displayMsrNamesVisitor :

  public                      visitor<S_msrScore>,

  // parts & part groups

  public                      visitor<S_msrPartGroup>,

  public                      visitor<S_msrPart>,

  // staves

  public                      visitor<S_msrStaff>,

  // voices

  public                      visitor<S_msrVoice>

{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          displayMsrNamesVisitor (
                            const S_msrOahGroup& msrOpts);

    virtual               ~displayMsrNamesVisitor ();

  public:

    // public services
    // ------------------------------------------------------


    void                  printMsrScoreNames (
                            const S_msrScore& score);

  protected:

    // visitors
    // ------------------------------------------------------

    virtual void              visitStart (S_msrScore& elt);
    virtual void              visitEnd   (S_msrScore& elt);

    virtual void              visitStart (S_msrPartGroup& elt);
    virtual void              visitEnd   (S_msrPartGroup& elt);

    virtual void              visitStart (S_msrPart& elt);
    virtual void              visitEnd   (S_msrPart& elt);

    virtual void              visitStart (S_msrStaff& elt);
    virtual void              visitEnd   (S_msrStaff& elt);

    virtual void              visitStart (S_msrVoice& elt);
    virtual void              visitEnd   (S_msrVoice& elt);

  private:

    // private fields
    // ------------------------------------------------------

    S_msrOahGroup         fMsrOahGroup;

    // score

    // part groups

    int                   fPartGroupsCounter;

    // parts

    int                   fPartsCounter;

    // staves

    int                   fStavesCounter;

    // prevent clef, key and time signature from being handled twice
    Bool                  fOnGoingStaff;

    // voices

    int                   fVoicesCounter;
};


}


#endif // ___displayMsrNamesVisitor___
