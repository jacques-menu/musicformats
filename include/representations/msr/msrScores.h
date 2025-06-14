/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrScores___
#define ___msrScores___

#include <iostream>

#include <list>
#include <set>

#include "msrBookElements.h"

#include "msrBooks.h"
#include "msrCredits.h"
#include "msrIdentification.h"
#include "msrMusicXMLBasicTypes.h"
#include "msrPartGroups.h"
#include "msrScaling.h"
#include "msrMeasuresSlices.h"
#include "msrVoices.h"


namespace MusicFormats
{

//______________________________________________________________________________
enum class msrStavesBrowingOrderKind {
  kStavesBrowingOrder_UNKNOWN_,

  kStavesBrowingOrderHarmoniesRegularsFiguredBasses,
  kStavesBrowingOrderRegularsHarmoniesFiguredBasses,
  kStavesBrowingOrderHarmoniesFiguredBassesRegulars
};

std::string msrStavesBrowingOrderKindAsString (
  msrStavesBrowingOrderKind stavesBrowingOrderKind);

std::string msrStavesBrowingOrderKindAsShortString (
  msrStavesBrowingOrderKind stavesBrowingOrderKind);

std::ostream& operator << (std::ostream& os, const msrStavesBrowingOrderKind& elt);

std::string msrStavesBrowingOrderKindShortName (
  msrStavesBrowingOrderKind stavesBrowingOrderKind);

EXP msrStavesBrowingOrderKind msrStavesBrowingOrderKindFromString (
  const std::string& theString);

std::string availableStavesBrowingOrderKinds (size_t namesListMaxLength);
std::string availableStavesBrowingOrderKindsNames (size_t namesListMaxLength);

//______________________________________________________________________________
class EXP msrScore : public msrBookElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrScore> create (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& scoreName);

    SMARTP<msrScore> createScoreNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrScore (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& scoreName);

    virtual               ~msrScore ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink
    void                  setScoreUpLinkToBook (S_msrBook book)
                              { fScoreUpLinkToBook = book; }

    S_msrBook             getScoreUpLinkToBook () const
                              { return fScoreUpLinkToBook; }

    // name
    const std::string&    getScoreName () const
                              { return fScoreName; }

    // identification
    void                  setIdentification (
                            const S_msrIdentification& identification)
                              { fIdentification = identification; }

    S_msrIdentification   getIdentification () const
                              { return fIdentification; }

    // scaling
    void                  setScaling (
                            const S_msrScaling& scaling)
                              { fScaling = scaling; }

    S_msrScaling          getScaling () const
                              { return fScaling; }

    // page layout
    void                  setPageLayout (
                            const S_msrPageLayout& pageLayout)
                              { fPageLayout = pageLayout; }

    S_msrPageLayout       getPageLayout () const
                              { return fPageLayout; }

    // system layout
    void                  setSystemLayout (
                            const S_msrSystemLayout& systemLayout)
                              { fSystemLayout = systemLayout; }

    S_msrSystemLayout     getSystemLayout () const
                              { return fSystemLayout; }

    // staff layout

    void                  setStaffLayout (
                            const S_msrStaffLayout& staffLayout)
                              { fStaffLayout = staffLayout; }

    S_msrStaffLayout      getStaffLayout () const
                              { return fStaffLayout; }

    // appearance

    void                  setAppearance (
                            const S_msrAppearance& appearance)
                              { fAppearance = appearance; }

    S_msrAppearance       getAppearance () const
                              { return fAppearance; }

    // part groups
    const std::list <S_msrPartGroup>&
                          getPartGroupsList () const
                              { return fPartGroupsList; }

//* JMI
    // master voice
    void                  setScoreMasterVoice (
                            const S_msrVoice& masterVoice);

    S_msrVoice            getScoreMasterVoice () const;

/* JMI
    void                  setScoreMasterVoice (
                            const S_msrVoice& masterVoice)
                              { fScoreMasterVoice = masterVoice; }

    S_msrVoice            getScoreMasterVoice () const
                              { return fScoreMasterVoice; }
*/

    // credits
    const std::list <S_msrCredit>&
                          getCreditsList () const
                              { return fCreditsList; }

    // measures
    void                  setScoreFirstMeasureNumber (
                            const mfMeasureNumber& measureNumber)
                              { fScoreFirstMeasureNumber = measureNumber; }

    const mfMeasureNumber&
                          getScoreFirstMeasureNumber () const
                              { return fScoreFirstMeasureNumber; }

    void                  setScoreLastMeasureNumber (
                             const mfMeasureNumber& measureNumber)
                              { fScoreLastMeasureNumber = measureNumber; }

    const mfMeasureNumber&
                          getScoreLastMeasureNumber () const
                              { return fScoreLastMeasureNumber; }

    void                  setScoreMeasuresNumber (
                            int scoreMeasuresNumber);

    int                   getScoreMeasuresNumber () const
                              { return fScoreMeasuresNumber; }

    void                  setStaffContainsMultipleMeasureRests (
                            Bool staffContainsMultipleMeasureRests)
                              {
                                fStaffContainsMultipleMeasureRests =
                                  staffContainsMultipleMeasureRests;
                              }

    Bool                  getStaffContainsMultipleMeasureRests () const
                              { return fStaffContainsMultipleMeasureRests; }

    // part group names max length
    void                  setScorePartGroupNamesMaxLength (size_t value)
                              { fScorePartGroupNamesMaxLength = value; }

    size_t                getScorePartGroupNamesMaxLength () const
                              { return fScorePartGroupNamesMaxLength; }

    // part names max length
    void                  setScorePartNamesMaxLength (size_t value)
                              { fScorePartNamesMaxLength = value; }

    size_t                getScorePartNamesMaxLength () const
                              { return fScorePartNamesMaxLength; }

    // instrument names max lengths
    void                  setIdentificationScoreInstrumentNamesMaxLength (
                            size_t value)
                              { fScoreInstrumentNamesMaxLength = value; }

    size_t                getScoreInstrumentNamesMaxLength () const
                              { return fScoreInstrumentNamesMaxLength; }

    void                  setIdentificationScoreInstrumentAbbreviationsMaxLength (
                            size_t value)
                              { fScoreInstrumentAbbreviationsMaxLength = value; }

    size_t                getScoreInstrumentAbbreviationsMaxLength () const
                              { return fScoreInstrumentAbbreviationsMaxLength; }

    // inhibiting browsing
    void                  setInhibitGraceNotesGroupsBeforeBrowsing ()
                              {
                                fInhibitGraceNotesGroupsBeforeBrowsing = true;
                              }

    Bool                  getInhibitGraceNotesGroupsBeforeBrowsing () const
                              {
                                return
                                  fInhibitGraceNotesGroupsBeforeBrowsing;
                              };

    void                  setInhibitGraceNotesGroupsAfterBrowsing ()
                              {
                                fInhibitGraceNotesGroupsAfterBrowsing = true;
                              }

    Bool                  getInhibitGraceNotesGroupsAfterBrowsing () const
                              {
                                return
                                  fInhibitGraceNotesGroupsAfterBrowsing;
                              };

    void                  setInhibitChordsInGraceNotesGroupsBrowsing ()
                              {
                                fInhibitChordsInGraceNotesGroupsBrowsing = true;
                              }

    Bool                  getInhibitChordsInGraceNotesGroupsBrowsing () const
                              {
                                return
                                  fInhibitChordsInGraceNotesGroupsBrowsing;
                              };

    void                  setInhibitTupletsInGraceNotesGroupsBrowsing ()
                              {
                                fInhibitTupletsInGraceNotesGroupsBrowsing = true;
                              }

    Bool                  getInhibitTupletsInGraceNotesGroupsBrowsing () const
                              {
                                return
                                  fInhibitTupletsInGraceNotesGroupsBrowsing;
                              };

    void                  setInhibitMeasureRepeatReplicasBrowsing ()
                              {
                                fInhibitMeasureRepeatReplicasBrowsing = true;
                              }

    Bool                  getInhibitMeasureRepeatReplicasBrowsing () const
                              {
                                return
                                  fInhibitMeasureRepeatReplicasBrowsing;
                              };

    void                  setInhibitMultipleMeasureRestsBrowsing ()
                              {
                                fInhibitMultipleMeasureRestsBrowsing = true;
                              }

    Bool                  getInhibitMultipleMeasureRestsBrowsing () const
                              {
                                return
                                  fInhibitMultipleMeasureRestsBrowsing;
                              };

    // staves browsing order

    void                  setStavesBrowingOrderKind (
                            msrStavesBrowingOrderKind stavesBrowingOrderKind)
                              { fStavesBrowingOrderKind = stavesBrowingOrderKind; }

    msrStavesBrowingOrderKind
                          getStavesBrowingOrderKind () const
                              { return fStavesBrowingOrderKind; }

    // score voices

    const std::list <S_msrVoice>&
                          getScoreAllVoicesList () const
                              { return fScoreAllVoicesList; }

    // measures slices sequence

    S_msrMeasuresSlicesSequence
                          getScoreMeasuresSlicesSequence () const
                              { return fScoreMeasuresSlicesSequence; }

  public:

    // public services
    // ------------------------------------------------------

    // voices

    void                  registerVoiceInScoreAllVoicesList (
                            const S_msrVoice& voice);

    void                  addPartGroupToScore (
                            const S_msrPartGroup& partGroup);
    void                  removePartGroupFromScore (
                            const S_msrPartGroup& partGroup);

    void                  appendCreditToScore (
                            const S_msrCredit& credit);

    S_msrPart             fetchPartFromScoreByItsPartID (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& partMusicXMLID);

    void                  collectScorePartsInList (
                            const mfInputLineNumber& inputLineNumber,
                            std::list <S_msrPart>& partsList);

    void                  collectScorePartsInMap (
                            std::map <std::string, S_msrPart>& partsMap);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  displayPartGroupsList (
                            const std::string& context);

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

    void                  printSummary (std::ostream& os) const override;

    void                  printSlices (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // uplink
    S_msrBook             fScoreUpLinkToBook;

    // name
    std::string           fScoreName;

    // identification
    S_msrIdentification   fIdentification;

    // scaling
    S_msrScaling          fScaling;

    // layout
    S_msrPageLayout       fPageLayout;

    S_msrSystemLayout     fSystemLayout;

    S_msrStaffLayout      fStaffLayout;

    // appearance
    S_msrAppearance       fAppearance;

    // credits
    std::list <S_msrCredit>
                          fCreditsList;

    // part groups
    std::set <S_msrPartGroup>
                          fScorePartGroupsSet;

    std::list <S_msrPartGroup>
                          fPartGroupsList;

    // master voice

// JMI
    S_msrVoice            fScoreMasterVoice;

    // measures

    mfMeasureNumber       fScoreFirstMeasureNumber;
    mfMeasureNumber       fScoreLastMeasureNumber;

    int                   fScoreMeasuresNumber;

    // multiple measure rests

    Bool                  fStaffContainsMultipleMeasureRests;

    // part group names max length

    size_t                fScorePartGroupNamesMaxLength;

    // part names max length

    size_t                fScorePartNamesMaxLength;

    // instrument names max lengths

    size_t                fScoreInstrumentNamesMaxLength;
    size_t                fScoreInstrumentAbbreviationsMaxLength;

    // inhibiting browsing

    // grace notes groups before are handled at the note level,
    // and should be not be browsed afterwards
    Bool                  fInhibitGraceNotesGroupsBeforeBrowsing;
    Bool                  fInhibitGraceNotesGroupsAfterBrowsing;

    Bool                  fInhibitChordsInGraceNotesGroupsBrowsing;

    // not relevant, since there are not tuplets in grace notes groups ??? JMI
    Bool                  fInhibitTupletsInGraceNotesGroupsBrowsing;

    // in <measure-repeat/>, the measure replicas are explicit,
    // whereas LilyPond only needs the repeated measure
    Bool                  fInhibitMeasureRepeatReplicasBrowsing;

    // in <multiple-rest/>, the multiple measure rests are explicit,
    // whereas LilyPond only needs the number of multiple measure rests
    Bool                  fInhibitMultipleMeasureRestsBrowsing;

    // staves browsing order

    msrStavesBrowingOrderKind
                          fStavesBrowingOrderKind;

    // score voices

    std::list <S_msrVoice> fScoreAllVoicesList;

    // measures slices sequence

    S_msrMeasuresSlicesSequence
                          fScoreMeasuresSlicesSequence;
};
typedef SMARTP<msrScore> S_msrScore;
EXP std::ostream& operator << (std::ostream& os, const S_msrScore& elt);


}

#endif // ___msrScores___
