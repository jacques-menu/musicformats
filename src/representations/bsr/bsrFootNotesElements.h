/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrFootNotesElements___
#define ___bsrFootNotesElements___

#include "bsrElements.h"

#include "bsrCellsLists.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP bsrFootNotesElement : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrFootNotesElement> create (
                            int           inputLineNumber,
                            const std::string& footNoteText);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrFootNotesElement (
                            int           inputLineNumber,
                            const std::string& footNoteText);

    virtual               ~bsrFootNotesElement ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getFootNoteText () const
                              { return fFootNoteText; }

    S_bsrCellsList        getFootNotesElementCellsList () const
                              { return fFootNotesElementCellsList ;}

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

    std::string           fFootNoteText;

    S_bsrCellsList        fFootNotesElementCellsList; // UNUSED JMI 0.9.66
};
typedef SMARTP<bsrFootNotesElement> S_bsrFootNotesElement;
EXP std::ostream& operator << (std::ostream& os, const S_bsrFootNotesElement& elt);


}


#endif // ___bsrFootNotesElements___
