/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrPartGroups___
#define ___lpsrPartGroups___

#include "msrPartGroups.h"

#include "lpsrElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class   lpsrPartGroupBlock;
typedef SMARTP<lpsrPartGroupBlock> S_lpsrPartGroupBlock;

//______________________________________________________________________________
class EXP lpsrPartGroupBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrPartGroupBlock> create (
                            const S_msrPartGroup& partGroup);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrPartGroupBlock (
                            const S_msrPartGroup& partGroup);

    virtual               ~lpsrPartGroupBlock ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrPartGroup        getPartGroup () const
                              { return fPartGroup; }

    const std::list<S_msrElement>&
                          getPartGroupBlockElements () const
                              { return fPartGroupBlockElements; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendElementToPartGroupBlock (
                            const S_msrElement& elem)
                              { fPartGroupBlockElements.push_back (elem); }

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrPartGroup        fPartGroup;

    std::list<S_msrElement>
                          fPartGroupBlockElements;
};
EXP std::ostream& operator << (std::ostream& os, const S_lpsrPartGroupBlock& elt);


}


#endif
