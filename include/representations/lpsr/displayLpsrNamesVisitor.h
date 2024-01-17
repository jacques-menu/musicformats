/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___displayLpsrNamesVisitor___
#define ___displayLpsrNamesVisitor___

#include "visitor.h"

#include "lpsrScores.h"

#include "lpsrOah.h"


using namespace MusicXML2;

namespace MusicFormats
{

//________________________________________________________________________
class EXP displayLpsrNamesVisitor :

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

                          displayLpsrNamesVisitor (
                            const S_lpsrOahGroup& lpsrOpts);

    virtual               ~displayLpsrNamesVisitor ();

  public:

    // public services
    // ------------------------------------------------------


    void                  printNamesFromLpsrScore (
                            const S_lpsrScore& score);

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

    S_lpsrOahGroup        fLpsrOahGroup;

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


#endif // ___displayLpsrNamesVisitor___
