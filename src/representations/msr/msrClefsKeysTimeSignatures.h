/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrClefsKeysTimeSignatures___
#define ___msrClefsKeysTimeSignatures___

#include <list>
#include <map>
#include <vector>

#include "msrMeasureElements.h"

#include "msrMeasures.h"
#include "msrOctaves.h"


namespace MusicFormats
{

//______________________________________________________________________________
// PRE-declarations for class mutual dependencies

class   msrClef;
typedef SMARTP<msrClef> S_msrClef;

class   msrKey;
typedef SMARTP<msrKey> S_msrKey;

class   msrTimeSignature;
typedef SMARTP<msrTimeSignature> S_msrTimeSignature;

// clefs
//______________________________________________________________________________

enum class msrClefKind {
  kClef_UNKNOWN_,

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

std::string availableClefKinds (size_t namesListMaxLength);
std::string availableClefKindsNames (size_t namesListMaxLength);

extern std::map <std::string, msrClefKind>
  gGlobalClefKindsMap;

extern std::list <std::string>
  gClefKindsNamesList;

void initializeClefKinds ();

//______________________________________________________________________________
// PRE-declaration

class   msrClef;
typedef SMARTP<msrClef> S_msrClef;

//______________________________________________________________________________
class EXP msrClef : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrClef> create (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            msrClefKind         clefKind,
                            int                 clefStaffNumber);

    static SMARTP<msrClef> create (
                            int                 inputLineNumber,
                            msrClefKind         clefKind,
                            int                 clefStaffNumber);

    // creation from the applications
    // ------------------------------------------------------

    static SMARTP<msrClef> createClefFromString (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            const std::string& clefString,
                            int                 clefLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrClef (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            msrClefKind         clefKind,
                            int                 clefStaffNumber);

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

//     std::string           asStringForMeasuresSlices () const override;

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




// time signatures
//______________________________________________________________________________
enum class msrTimeSignatureSymbolKind {
  kTimeSignatureSymbolNone,
  kTimeSignatureSymbolCommon,
  kTimeSignatureSymbolCut,
  kTimeSignatureSymbolNote,
  kTimeSignatureSymbolDottedNote,
  kTimeSignatureSymbolSingleNumber,
  kTimeSignatureSymbolSenzaMisura
};

std::string msrTimeSignatureSymbolKindAsString (
  msrTimeSignatureSymbolKind timeSignatureSymbolKind);

std::ostream& operator << (std::ostream& os, const msrTimeSignatureSymbolKind& elt);

enum class msrTimeSignatureSeparatorKind {
  kTimeSignatureSeparatorNone,
  kTimeSignatureSeparatorHorizontal,
  kTimeSignatureSeparatorDiagonal,
  kTimeSignatureSeparatorVertical,
  kTimeSignatureSeparatorAdjacent
};

std::string msrTimeSignatureSeparatorKindAsString (
  msrTimeSignatureSeparatorKind timeSignatureSeparatorKind);

std::ostream& operator << (std::ostream& os, const msrTimeSignatureSeparatorKind& elt);

enum class msrTimeSignatureRelationKind {
  kTimeSignatureRelationNone,
  kTimeSignatureRelationParentheses,
  kTimeSignatureRelationBracket,
  kTimeSignatureRelationEquals,
  kTimeSignatureRelationSlash,
  kTimeSignatureRelationSpace,
  kTimeSignatureRelationHyphen
};

std::string msrTimeSignatureRelationKindAsString (
  msrTimeSignatureRelationKind timeSignatureRelationKind);

std::ostream& operator << (std::ostream& os, const msrTimeSignatureRelationKind& elt);

//______________________________________________________________________________
class EXP msrTimeSignatureItem : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTimeSignatureItem> create (
                            int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTimeSignatureItem (
                            int inputLineNumber);

    virtual               ~msrTimeSignatureItem ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::vector <int>&
                          getTimeSignatureBeatsNumbersVector () const
                              { return fTimeSignatureBeatsNumbersVector; }

    void                  setTimeSignatureBeatValue (int timeSignatureBeatValue);

    int                   getTimeSignatureBeatValue () const
                              { return fTimeSignatureBeatValue; }

  public:

    // public services
    // ------------------------------------------------------

    Bool                  isEqualTo (
                            const S_msrTimeSignatureItem& otherTimeSignatureItem) const;

    void                  appendBeatsNumber (int beatsNumber);

    int                   getTimeSignatureBeatsNumber () const;

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

    std::vector <int>      fTimeSignatureBeatsNumbersVector; // 5+3+1 is possible
    int                   fTimeSignatureBeatValue;
};
typedef SMARTP<msrTimeSignatureItem> S_msrTimeSignatureItem;
EXP std::ostream& operator << (std::ostream& os, const S_msrTimeSignatureItem& elt);

//______________________________________________________________________________
class EXP msrTimeSignature : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTimeSignature> create (
                            int                        inputLineNumber,
                            const S_msrMeasure&        upLinkToMeasure,
                            msrTimeSignatureSymbolKind timeSignatureSymbolKind);

    static SMARTP<msrTimeSignature> create (
                            int                        inputLineNumber,
                            msrTimeSignatureSymbolKind timeSignatureSymbolKind);

    // creation from the applications
    // ------------------------------------------------------

    static SMARTP<msrTimeSignature> createTwoEightsTime (
                            int inputLineNumber);

    static SMARTP<msrTimeSignature> createThreeEightsTime (
                            int inputLineNumber);

    static SMARTP<msrTimeSignature> createSixEightsTime (
                            int inputLineNumber);

    static SMARTP<msrTimeSignature> createTwoQuartersTime (
                            int inputLineNumber);

    static SMARTP<msrTimeSignature> createThreeQuartersTime (
                            int inputLineNumber);

    static SMARTP<msrTimeSignature> createFourQuartersTime (
                            int inputLineNumber);

    static SMARTP<msrTimeSignature> createFiveQuartersTime (
                            int inputLineNumber);

    static SMARTP<msrTimeSignature> createTwoHalvesTime (
                            int inputLineNumber);

    static SMARTP<msrTimeSignature> createThreeHalvesTime (
                            int inputLineNumber);

    static SMARTP<msrTimeSignature> createFourHalvesTime (
                            int inputLineNumber);

    // creation from the applications
    // ------------------------------------------------------

    static SMARTP<msrTimeSignature> createTimeFromString (
                            int                inputLineNumber,
                            const std::string& timeString);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTimeSignature (
                            int                        inputLineNumber,
                            const S_msrMeasure&        upLinkToMeasure,
                            msrTimeSignatureSymbolKind timeSignatureSymbolKind);

    virtual               ~msrTimeSignature ();

  public:

    // set and get
    // ------------------------------------------------------

    msrTimeSignatureSymbolKind
                          getTimeSignatureSymbolKind () const
                              { return fTimeSignatureSymbolKind; }

    Bool                  getTimeIsCompound () const
                              { return fTimeIsCompound; }

    const std::vector <S_msrTimeSignatureItem>&
                          getTimeSignatureItemsVector () const
                              { return fTimeSignatureItemsVector; }

  public:

    // public services
    // ------------------------------------------------------

    Bool                  isEqualTo (
                            S_msrTimeSignature otherTimeSignature) const;

    void                  appendTimeSignatureItem (
                            const S_msrTimeSignatureItem& timeSignatureItem);

    msrWholeNotes         timeSignatureWholeNotesPerMeasure () const;

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

    std::string           asShortString () const override;

//     std::string           asStringForMeasuresSlices () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrTimeSignatureSymbolKind
                          fTimeSignatureSymbolKind;

    std::vector <S_msrTimeSignatureItem>
                          fTimeSignatureItemsVector;

    // a time signature is compound if it contains several items
    // or if the only one has several beats numbers
    // i.e. 3/4 is not, (3+4)/8 is, and 2/4+3/4 is too
    Bool                  fTimeIsCompound;
};
typedef SMARTP<msrTimeSignature> S_msrTimeSignature;
EXP std::ostream& operator << (std::ostream& os, const S_msrTimeSignature& elt);
EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const S_msrTimeSignature& elt);

// clefs, keys, time signatures order kinds
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
  int                inputLineNumber,
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
                            int inputLineNumber,
                            int groupInputLineNumber);

    SMARTP<msrClefKeyTimeSignatureGroup> createClefKeyTimeSignatureGroupNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrClefKeyTimeSignatureGroup (
                            int inputLineNumber,
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

    int                   fGroupInputLineNumber; // start/end line numbers??? JMI v0.9.70

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
