/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrFootNotes___
#define ___bsrFootNotes___

#include "bsrLines.h"

#include "bsrFootNotesElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP bsrFootNotes : public bsrLine
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrFootNotes> create (
                            int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrFootNotes (
                            int inputLineNumber);

    virtual               ~bsrFootNotes ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  appendElementToFootNotes (
                            const S_bsrFootNotesElement& footNotesElement)
                              {
                                fFootNotesElementsList.push_back (
                                  footNotesElement);
                              }

    int                   fetchLineContentsNumber () const override
                              { return fLineContentsList.size (); }

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

    std::list<S_bsrFootNotesElement>
                          fFootNotesElementsList;
};
typedef SMARTP<bsrFootNotes> S_bsrFootNotes;
EXP std::ostream& operator << (std::ostream& os, const S_bsrFootNotes& elt);


}


#endif // ___bsrFootNotes___
