/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrClefsKeysTimeSignatures___
#define ___msrClefsKeysTimeSignatures___

#include "msrClefs.h"
#include "msrKeys.h"
#include "msrTimeSignatures.h"


namespace MusicFormats
{

//______________________________________________________________________________

/*
  The order for clefs, keys and time signatures may vary:
  this a musician's order, but MusicXML uses <key/>, <time/>, <clef/>,
  so we must provide use-specific browsing order for them
*/

enum class msrClefKeyTimeSignatureOrderKind {
  kClefKeyTimeSignatureOrder_UNKNOWN_,

  kClefKeyTimeSignatureOrderClefKeyTimeSignature, // musician's order
  kClefKeyTimeSignatureOrderKeyTimeSignatureClef  // MusicXML order
};

std::string msrClefKeyTimeSignatureOrderKindAsString (
  msrClefKeyTimeSignatureOrderKind clefKeyTimeSignatureOrderKind);

std::ostream& operator << (std::ostream& os, const msrClefKeyTimeSignatureOrderKind& elt);

EXP msrClefKeyTimeSignatureOrderKind msrClefKeyTimeSignatureOrderKindFromString (
  const mfInputLineNumber& inputLineNumber,
  const std::string& clefKeyTimeSignatureString);

std::string availableClefKeyTimeSignatureOrderKinds (size_t namesListMaxLength);
std::string availableClefKeyTimeSignatureOrderKindsNames (size_t namesListMaxLength);

extern std::map <std::string, msrClefKeyTimeSignatureOrderKind>
  gGlobalClefKeyTimeSignatureOrderKindsMap;

extern std::list <std::string>
  gClefKeyTimeSignatureOrderKindsNamesList;

void initializeClefKeyTimeSignatureOrderKinds ();

//______________________________________________________________________________
class EXP msrClefKeyTimeSignatureGroup : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrClefKeyTimeSignatureGroup> create (
                            const mfInputLineNumber& inputLineNumber,
                            int groupInputLineNumber);

    SMARTP<msrClefKeyTimeSignatureGroup> createClefKeyTimeSignatureGroupNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrClefKeyTimeSignatureGroup (
                            const mfInputLineNumber& inputLineNumber,
                            int groupInputLineNumber);

    virtual               ~msrClefKeyTimeSignatureGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getGroupInputLineNumber () const
                              { return fGroupInputLineNumber; }

    void                  setClef (const S_msrClef& clef);

    S_msrClef             getClef () const;

    void                  setKey (const S_msrKey& key);

    S_msrKey              getKey () const;

    void                  setTimeSignature (
                            const S_msrTimeSignature& timeSignature);

    S_msrTimeSignature    getTimeSignature () const;

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

    // public services
    // ------------------------------------------------------

    Bool                  fetchGroupIsEmpty () const
                              { return fGroupItemsNumber == 0; }


  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;

//     std::string           asStringForMeasuresSlices () const override;

    void                  print (std::ostream& os) const override;

  private:

  private:

    // private fields
    // ------------------------------------------------------

    int                   fGroupInputLineNumber; // start/end line numbers??? JMI 0.9.70

    // clef
    S_msrClef             fClef;

    // key
    S_msrKey              fKey;

    // time signature
    S_msrTimeSignature    fTimeSignature;

    int                   fGroupItemsNumber;
};
typedef SMARTP<msrClefKeyTimeSignatureGroup> S_msrClefKeyTimeSignatureGroup;
EXP std::ostream& operator << (std::ostream& os, const S_msrClefKeyTimeSignatureGroup& elt);

//______________________________________________________________________________
EXP void setClefKeyTimeSignatureOrderKind (
  msrClefKeyTimeSignatureOrderKind clefKeyTimeSignatureOrderKind);

EXP msrClefKeyTimeSignatureOrderKind getClefKeyTimeSignatureOrderKind ();


}


#endif // ___msrClefsKeysTimeSignatures___
