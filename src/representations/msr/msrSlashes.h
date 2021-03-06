/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrSlashes___
#define ___msrSlashes___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrSlash : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSlash> create (
                            int                  inputLineNumber,
                            msrSlashTypeKind     slashTypeKind,
                            msrUseDotsKind  useDotsKind,
                            msrSlashUseStemsKind slashUseStemsKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSlash (
                            int                  inputLineNumber,
                            msrSlashTypeKind     slashTypeKind,
                            msrUseDotsKind  useDotsKind,
                            msrSlashUseStemsKind slashUseStemsKind);

    virtual               ~msrSlash ();

  public:

    // set and get
    // ------------------------------------------------------

    msrSlashTypeKind      getSlashTypeKind () const
                              { return fSlashTypeKind; }

    msrUseDotsKind   getUseDotsKind () const
                              { return fUseDotsKind; }

    msrSlashUseStemsKind  getSlashUseStemsKind () const
                              { return fSlashUseStemsKind; }

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

    msrSlashTypeKind      fSlashTypeKind;
    msrUseDotsKind        fUseDotsKind;
    msrSlashUseStemsKind  fSlashUseStemsKind;
};
typedef SMARTP<msrSlash> S_msrSlash;
EXP ostream& operator<< (ostream& os, const S_msrSlash& elt);


} // namespace MusicFormats


#endif
