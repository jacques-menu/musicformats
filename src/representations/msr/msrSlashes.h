/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrSlashes___
#define ___msrSlashes___

#include "msrElements.h"


namespace MusicFormats
{

// measure styles
//______________________________________________________________________________
enum class msrSlashTypeKind {
  kSlashType_UNKNOWN_,
  kSlashTypeStart, kSlashTypeStop
};

std::string msrSlashTypeKindAsString (
  msrSlashTypeKind slashTypeKind);

std::ostream& operator << (std::ostream& os, const msrSlashTypeKind& elt);

enum class msrUseDotsKind {
  kUseDots_UNKNOWN_,
  kUseDotsYes, kUseDotsNo
};

std::string msrUseDotsKindAsString (
  msrUseDotsKind useDotsKind);

std::ostream& operator << (std::ostream& os, const msrUseDotsKind& elt);

msrUseDotsKind msrUseDotsFromString (
  const mfInputLineNumber& inputLineNumber,
  const std::string& useDotsString);

enum class msrSlashUseStemsKind {
  kSlashUseStems_UNKNOWN_,
  kSlashUseStemsYes, kSlashUseStemsNo
};

std::string msrSlashUseStemsKindAsString (
  msrSlashUseStemsKind slashUseStemsKind);

std::ostream& operator << (std::ostream& os, const msrSlashUseStemsKind& elt);

//______________________________________________________________________________
class EXP msrSlash : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSlash> create (
                            const mfInputLineNumber& inputLineNumber,
                            msrSlashTypeKind     slashTypeKind,
                            msrUseDotsKind  useDotsKind,
                            msrSlashUseStemsKind slashUseStemsKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSlash (
                            const mfInputLineNumber& inputLineNumber,
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

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrSlashTypeKind      fSlashTypeKind;
    msrUseDotsKind        fUseDotsKind;
    msrSlashUseStemsKind  fSlashUseStemsKind;
};
typedef SMARTP<msrSlash> S_msrSlash;
EXP std::ostream& operator << (std::ostream& os, const S_msrSlash& elt);


}


#endif // ___msrSlashes___
