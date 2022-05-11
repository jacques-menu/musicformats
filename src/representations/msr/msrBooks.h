/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrBooks___
#define ___msrBooks___

#include <iostream>

#include <list>
#include <set>

#include "msrElements.h"

#include "msrCredits.h"
#include "msrIdentification.h"
#include "msrScaling.h"

#include "msrBookElements.h"
#include "msrVoices.h"


namespace MusicFormats
{

//______________________________________________________________________________
class msrPart;
typedef SMARTP<msrPart> S_msrPart;

class msrBook;
typedef SMARTP<msrBook> S_msrBook;

class msrVoice;
typedef SMARTP<msrVoice> S_msrVoice;

//______________________________________________________________________________
class EXP msrBook : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBook> create (
                            int           inputLineNumber,
                            const string& bookName);

    SMARTP<msrBook> createBookNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrBook (
                            int           inputLineNumber,
                            const string& bookName);

    virtual               ~msrBook ();

  public:

    // set and get
    // ------------------------------------------------------

    // name
    string                getBookName () const
                              { return fBookName; }

    // identification
    void                  setIdentification (
                            S_msrIdentification identification)
                              { fIdentification = identification; }

    S_msrIdentification   getIdentification () const
                              { return fIdentification; }

    // scaling
    void                  setScaling (
                            S_msrScaling scaling)
                              { fScaling = scaling; }

    S_msrScaling          getScaling () const
                              { return fScaling; }

    // page layout
    void                  setPageLayout (
                            S_msrPageLayout pageLayout)
                              { fPageLayout = pageLayout; }

    S_msrPageLayout       getPageLayout () const
                              { return fPageLayout; }

    // system layout
    void                  setSystemLayout (
                            S_msrSystemLayout systemLayout)
                              { fSystemLayout = systemLayout; }

    S_msrSystemLayout     getSystemLayout () const
                              { return fSystemLayout; }

    // staff layout

    void                  setStaffLayout (
                            S_msrStaffLayout staffLayout)
                              { fStaffLayout = staffLayout; }

    S_msrStaffLayout      getStaffLayout () const
                              { return fStaffLayout; }

    // appearance

    void                  setAppearance (
                            S_msrAppearance appearance)
                              { fAppearance = appearance; }

    S_msrAppearance       getAppearance () const
                              { return fAppearance; }

    // book elements
    const list<S_msrBookElement>&
                          getBookElementsList () const
                              { return fBookElementsList; }

//* JMI
    // master voice
    void                  setBookMasterVoice (
                            S_msrVoice masterVoice);

    S_msrVoice            getBookMasterVoice () const;

/* JMI
    void                  setBookMasterVoice (
                            S_msrVoice masterVoice)
                              { fBookMasterVoice = masterVoice; }

    S_msrVoice            getBookMasterVoice () const
                              { return fBookMasterVoice; }
*/

    // credits
    const list<S_msrCredit>&
                          getCreditsList () const
                              { return fCreditsList; }

    // measures
    void                  setBookNumberOfMeasures (
                            int scoreNumberOfMeasures)
                              {
                                fBookNumberOfMeasures =
                                  scoreNumberOfMeasures;
                              }

    int                   getBookNumberOfMeasures () const
                              { return fBookNumberOfMeasures; }

    void                  setStaffContainsMultipleFullBarRests (
                            Bool staffContainsMultipleFullBarRests)
                              {
                                fStaffContainsMultipleFullBarRests =
                                  staffContainsMultipleFullBarRests;
                              }

    Bool                  getStaffContainsMultipleFullBarRests () const
                              { return fStaffContainsMultipleFullBarRests; }

    // book element names max length
    void                  setBookElementNamesMaxLength (int value)
                              { fBookElementNamesMaxLength = value; }

    int                   getBookElementNamesMaxLength () const
                              { return fBookElementNamesMaxLength; }

    // instrument names max lengthes
    void                  setBookInstrumentNamesMaxLength (int value)
                              { fBookInstrumentNamesMaxLength = value; }

    int                   getBookInstrumentNamesMaxLength () const
                              { return fBookInstrumentNamesMaxLength; }

    void                  setBookInstrumentAbbreviationsMaxLength (int value)
                              { fBookInstrumentAbbreviationsMaxLength = value; }

    int                   getBookInstrumentAbbreviationsMaxLength () const
                              { return fBookInstrumentAbbreviationsMaxLength; }

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

    void                  setInhibitMultipleFullBarRestsBrowsing ()
                              {
                                fInhibitMultipleFullBarRestsBrowsing = true;
                              }

    Bool                  getInhibitMultipleFullBarRestsBrowsing () const
                              {
                                return
                                  fInhibitMultipleFullBarRestsBrowsing;
                              };

    // voices

    void                  registerVoiceInBookAllVoicesList (
                            S_msrVoice voice);

  public:

    // public services
    // ------------------------------------------------------

    void                  addBookElementToBook (
                            S_msrBookElement bookElement);

    void                  appendCreditToBook (
                            S_msrCredit credit);

    void                  fetchIdentificationFromCreditsIfAny (
                            int inputLineNumber);

/* JMI
    void                  setHeaderFromOptionsIfAny (
                            int inputLineNumber);
*/

    void                  collectBookElementsList (
                            int    inputLineNumber,
                            list<S_msrPart>& partsList);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printShort (ostream& os) const override;

    void                  printSummary (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // name
    string                fBookName;

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
    list<S_msrCredit>     fCreditsList;

    // book elements
    set<S_msrBookElement> fBookElementsSet;

    list<S_msrBookElement>fBookElementsList;

    // master voice

// JMI
    S_msrVoice            fBookMasterVoice;

    // number of measures

    int                   fBookNumberOfMeasures;

    // multiple full-bar rests

    Bool                  fStaffContainsMultipleFullBarRests;

    // book element names max length

    int                   fBookElementNamesMaxLength;

    // instrument names max lengthes

    int                   fBookInstrumentNamesMaxLength;
    int                   fBookInstrumentAbbreviationsMaxLength;

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

    // in <multiple-rest/>, the multiple full-bar rests are explicit,
    // whereas LilyPond only needs the number of multiple full-bar rests
    Bool                  fInhibitMultipleFullBarRestsBrowsing;

    // voices

    list<S_msrVoice>      fBookAllVoicesList;
};
typedef SMARTP<msrBook> S_msrBook;
EXP ostream& operator<< (ostream& os, const S_msrBook& elt);


}

#endif
