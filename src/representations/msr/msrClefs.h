/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrClefs___
#define ___msrClefs___

#include <list>
#include <map>

#include "msrMeasureElementLambda.h"


namespace MusicFormats
{

// clefs
//______________________________________________________________________________

enum class msrClefKind {
  kClef_NO_,

  kClefTreble,
  kClefSoprano, kClefMezzoSoprano, kClefAlto, kClefTenor, kClefBaritone, kClefBass,
  kClefTrebleLine1,
  kClefTrebleMinus15, kClefTrebleMinus8, kClefTreblePlus8, kClefTreblePlus15,

  kClefBassMinus15, kClefBassMinus8, kClefBassPlus8, kClefBassPlus15,

  kClefVarbaritone,

  kClefTablature4, kClefTablature5, kClefTablature6, kClefTablature7,

  kClefPercussion,

  kClefJianpu
};

std::string msrClefKindAsString (
  msrClefKind clefKind);

std::ostream& operator << (std::ostream& os, const msrClefKind& elt);

EXP msrClefKind msrClefKindFromString (
  int           inputLineNumber,
  const std::string& clefString);

std::string existingClefKinds (size_t namesListMaxLength);
std::string existingClefKindsNames (size_t namesListMaxLength);

extern std::map<std::string, msrClefKind>
  gGlobalClefKindsMap;

extern std::list<std::string>
  gClefKindsNamesList;

void initializeClefKinds ();

//______________________________________________________________________________
// PRE-declaration

class   msrClef;
typedef SMARTP<msrClef> S_msrClef;

//______________________________________________________________________________
class EXP msrClef : public msrMeasureElementLambda
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrClef> create (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure,
                            msrClefKind  clefKind,
                            int          clefStaffNumber);

    // creation from the applications
    // ------------------------------------------------------

    static SMARTP<msrClef> createClefFromString (
                            int           inputLineNumber,
                            S_msrMeasure  upLinkToMeasure,
                            const std::string& clefString,
                            int           clefLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrClef (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure,
                            msrClefKind  clefKind,
                            int          clefStaffNumber);

    virtual               ~msrClef ();

  public:

    // set and get
    // ------------------------------------------------------

    msrClefKind           getClefKind () const
                              { return fClefKind; }

    int                   getClefStaffNumber () const
                              { return fClefStaffNumber; }

  public:

    // public services
    // ------------------------------------------------------

    Bool                  isEqualTo (S_msrClef otherClef) const;

    Bool                  clefIsATablatureClef () const;

    Bool                  clefIsAPercussionClef () const;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;
    std::string           asShortStringForMeasuresSlices () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrClefKind           fClefKind;
    int                   fClefStaffNumber; // 0 by default in MSR,
                                            // 1 by default in MusicXML
};
typedef SMARTP<msrClef> S_msrClef;
EXP std::ostream& operator << (std::ostream& os, const S_msrClef& elt);


}


#endif
