/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrKeys___
#define ___msrKeys___

#include "msrOctaves.h"


namespace MusicFormats
{

//______________________________________________________________________________
// PRE-declarations for class mutual dependencies

class   msrKey;
typedef SMARTP<msrKey> S_msrKey;

// keys
//______________________________________________________________________________

enum class msrKeyKind {
  kKeyTraditional, kKeyHumdrumScot
};

std::string msrKeyKindAsString (
  msrKeyKind keyKind);

std::ostream& operator << (std::ostream& os, const msrKeyKind& elt);

enum class msrModeKind {
  kMode_UNKNOWN_,

  kModeMajor, kModeMinor,
  kModeIonian, kModeDorian, kModePhrygian, kModeLydian,
  kModeMixolydian, kModeAeolian, kModeLocrian
};

std::string msrModeKindAsString (
  msrModeKind modeKind);

std::ostream& operator << (std::ostream& os,const msrModeKind& elt);

msrModeKind modeKindFromString (
  int           inputLineNumber,
  const std::string& modeString);

//______________________________________________________________________________
// PRE-declarations for mutual class dependencies

class   msrHumdrumScotKeyItem;
typedef SMARTP<msrHumdrumScotKeyItem> S_msrHumdrumScotKeyItem;

//______________________________________________________________________________
class EXP msrHumdrumScotKeyItem : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHumdrumScotKeyItem> create (
                            int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrHumdrumScotKeyItem (
                            int inputLineNumber);

    virtual               ~msrHumdrumScotKeyItem ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setKeyItemDiatonicPitchKind (
                            msrDiatonicPitchKind diatonicPitchKind);

    msrDiatonicPitchKind  getKeyItemDiatonicPitchKind () const
                              { return fKeyDiatonicPitchKind; }

    void                  setKeyItemAlterationKind (
                            msrAlterationKind alterationKind);

    msrAlterationKind     getKeyItemAlterationKind () const
                              { return fKeyAlterationKind; }

    void                  setKeyItemOctaveKind (msrOctaveKind keyOctaveKind);

    msrOctaveKind         getKeyItemOctaveKind () const
                              { return fKeyOctaveKind; }

  public:

    // public services
    // ------------------------------------------------------

    Bool                  isEqualTo (
                            const S_msrHumdrumScotKeyItem&
                              otherHumdrumScotKeyItem) const;

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

//     std::string           asStringForMeasuresSlices () const override;

    void                  print (std::ostream& os) const override;

  private:

    // fields
    // ------------------------------------------------------

    msrDiatonicPitchKind  fKeyDiatonicPitchKind;
    msrAlterationKind     fKeyAlterationKind;
    msrOctaveKind         fKeyOctaveKind;
};
typedef SMARTP<msrHumdrumScotKeyItem> S_msrHumdrumScotKeyItem;
EXP std::ostream& operator << (std::ostream& os, const S_msrHumdrumScotKeyItem& elt);

//______________________________________________________________________________
// PRE-declaration

class   msrKey;
typedef SMARTP<msrKey> S_msrKey;

//______________________________________________________________________________
class EXP msrKey : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrKey> createTraditional (
                            int                      inputLineNumber,
                            const S_msrMeasure&      upLinkToMeasure,
                            msrQuarterTonesPitchKind keyTonicQuarterTonesPitchKind,
                            msrModeKind              modeKind,
                            int                      keyCancel);

    static SMARTP<msrKey> createTraditional (
                            int                      inputLineNumber,
                            msrQuarterTonesPitchKind keyTonicQuarterTonesPitchKind,
                            msrModeKind              modeKind,
                            int                      keyCancel);

    static SMARTP<msrKey> createHumdrumScot ( // for Humdrum/Scot keys
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

    static SMARTP<msrKey> createHumdrumScot ( // for Humdrum/Scot keys
                            int inputLineNumber);

    // creation from the applications
    // ------------------------------------------------------

    static SMARTP<msrKey> createTraditionalKeyFromString (
                            int                inputLineNumber,
                            const std::string& keyString);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrKey ( // for traditional keys
                            int                      inputLineNumber,
                            const S_msrMeasure&      upLinkToMeasure,
                            msrQuarterTonesPitchKind keyTonicQuarterTonesPitchKind,
                            msrModeKind              modeKind,
                            int                      keyCancel);

                          msrKey ( // for Humdrum/Scot keys
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

    virtual               ~msrKey ();

  public:

    // set and get
    // ------------------------------------------------------

    msrKeyKind            getKeyKind () const
                              { return fKeyKind; }

    // traditional keys

    msrQuarterTonesPitchKind
                          getKeyTonicQuarterTonesPitchKind () const
                              { return fKeyTonicQuarterTonesPitchKind; }

    msrModeKind           getModeKind () const
                              { return fModeKind; }

    int                   getKeyCancel () const
                              { return fKeyCancel; }

    // Humdrum/Scot keys

 //   void                  setKeyItemsOctavesAreSpecified ()
 //                           { fKeyItemsOctavesAreSpecified = true; }

    Bool                  getKeyItemsOctavesAreSpecified () const
                              { return fKeyItemsOctavesAreSpecified; }

    const std::vector <S_msrHumdrumScotKeyItem>&
                          getHumdrumScotKeyItemsVector () const
                              { return fHumdrumScotKeyItemsVector; }

  public:

    // public services
    // ------------------------------------------------------

    Bool                  isEqualTo (S_msrKey otherKey) const;

    void                  appendHumdrumScotKeyItem (
                            const S_msrHumdrumScotKeyItem& item);

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

//     std::string           asStringForMeasuresSlices () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrKeyKind            fKeyKind;

    // traditional keys

    msrQuarterTonesPitchKind
                          fKeyTonicQuarterTonesPitchKind;
    msrModeKind           fModeKind;
    int                   fKeyCancel;

    // Humdrum/Scot keys

    std::vector <S_msrHumdrumScotKeyItem>
                          fHumdrumScotKeyItemsVector;
    Bool                  fKeyItemsOctavesAreSpecified;
};
typedef SMARTP<msrKey> S_msrKey;
EXP std::ostream& operator << (std::ostream& os, const S_msrKey& elt);


}


#endif // ___msrKeys___
