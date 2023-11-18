/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrStavesDetails___
#define ___msrStavesDetails___

#include <list>

#include "msrElements.h"
#include "msrMeasureElements.h"

#include "msrOctaves.h"
#include "msrPrintObjects.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrStaffTuning : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStaffTuning> create (
                            int                      inputLineNumber,
                            int                      staffTuningLineNumber,
                            msrQuarterTonesPitchKind staffTuningQuarterTonesPitchKind,
                            msrOctaveKind            staffTuningOctaveKind);

    SMARTP<msrStaffTuning> createStaffTuningNewbornClone ();

    SMARTP<msrStaffTuning> createStaffTuningDeepClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrStaffTuning (
                            int                      inputLineNumber,
                            int                      staffTuningLineNumber,
                            msrQuarterTonesPitchKind staffTuningQuarterTonesPitchKind,
                            msrOctaveKind            staffTuningOctaveKind);

    ~ msrStaffTuning ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getStaffTuningLineNumber () const
                              { return fStaffTuningLineNumber; }

    msrQuarterTonesPitchKind
                          getStaffTuningQuarterTonesPitchKind () const
                              { return fStaffTuningQuarterTonesPitchKind; }

    msrOctaveKind         getStaffTuningOctaveKind () const
                              { return fStaffTuningOctaveKind; }

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

    int                   fStaffTuningLineNumber;

    msrQuarterTonesPitchKind
                          fStaffTuningQuarterTonesPitchKind;
    msrOctaveKind         fStaffTuningOctaveKind;
};
typedef SMARTP<msrStaffTuning> S_msrStaffTuning;
EXP std::ostream& operator << (std::ostream& os, const S_msrStaffTuning& elt);

//______________________________________________________________________________
// data types

enum class msrStaffTypeKind {
  kStaffTypeRegular,
  kStaffTypeOssia, kStaffTypeCue, kStaffTypeEditorial,
  kStaffTypeAlternate
};

std::string msrStaffTypeKindAsString (
  msrStaffTypeKind staffTypeKind);

std::ostream& operator << (std::ostream& os, const msrStaffTypeKind& elt);

enum class msrShowFretsKind {
  kShowFretsNumbers, kShowFretsLetters
};

std::string msrShowFretsKindAsString (
  msrShowFretsKind showFretsKind);

std::ostream& operator << (std::ostream& os, const msrShowFretsKind& elt);

enum class msrPrintSpacingKind {
  kPrintSpacingYes, kPrintSpacingNo
};

std::string msrPrintSpacingKindAsString (
  msrPrintSpacingKind printSpacingKind);

std::ostream& operator << (std::ostream& os, const msrPrintSpacingKind& elt);

//______________________________________________________________________________
class EXP msrStaffDetails : public msrMeasureElement
{
  public:


    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStaffDetails> create (
                            int                 inputLineNumber,
                            const S_msrMeasure&        upLinkToMeasure,
                            msrStaffTypeKind    staffTypeKind,
                            msrShowFretsKind    showFretsKind,
                            msrPrintObjectKind  printObjectKind,
                            msrPrintSpacingKind printSpacingKin);

    static SMARTP<msrStaffDetails> create (
                            int                 inputLineNumber,
                            msrStaffTypeKind    staffTypeKind,
                            msrShowFretsKind    showFretsKind,
                            msrPrintObjectKind  printObjectKind,
                            msrPrintSpacingKind printSpacingKin);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrStaffDetails (
                            int                 inputLineNumber,
                            const S_msrMeasure&        upLinkToMeasure,
                            msrStaffTypeKind    staffTypeKind,
                            msrShowFretsKind    showFretsKind,
                            msrPrintObjectKind  printObjectKind,
                            msrPrintSpacingKind printSpacingKin);

    virtual               ~msrStaffDetails ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink to measure
//     void                  setMeasureElementUpLinkToMeasure (
//                             const S_msrMeasure& measure) override
//                               { setStaffDetailsUpLinkToMeasure (measure); }
//
//     S_msrMeasure          getMeasureElementUpLinkToMeasure () const override
//                               { return getStaffDetailsUpLinkToMeasure (); }
//
//     void                  setStaffDetailsUpLinkToMeasure (
//                             const S_msrMeasure& measure);
//
//     S_msrMeasure          getStaffDetailsUpLinkToMeasure () const
//                               { return fStaffDetailsUpLinkToMeasure; }

    msrStaffTypeKind      getStaffTypeKind () const
                              { return fStaffTypeKind; }

    void                  setStaffLinesNumber (
                            int staffLinesNumber)
                              { fStaffLinesNumber = staffLinesNumber; }

    int                   getStaffLinesNumber () const
                              { return fStaffLinesNumber; }

    const std::list<S_msrStaffTuning>&
                          getStaffTuningsList () const
                              { return fStaffTuningsList; }

    msrShowFretsKind      getShowFretsKind () const
                              { return fShowFretsKind; }

    msrPrintObjectKind    getPrintObjectKind () const
                              { return fPrintObjectKind; }

    msrPrintSpacingKind   getPrintSpacingKind () const
                              { return fPrintSpacingKind; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addStaffTuningToStaffDetails (
                            const S_msrStaffTuning& staffTuning)
                              {
                                fStaffTuningsList.push_back (staffTuning);
                              }

    std::string           asShortString () const override;

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

//     S_msrMeasure          fStaffDetailsUpLinkToMeasure;

    msrStaffTypeKind      fStaffTypeKind;

    int                   fStaffLinesNumber;
    std::list<S_msrStaffTuning>
                          fStaffTuningsList;

    msrShowFretsKind      fShowFretsKind;

    msrPrintObjectKind    fPrintObjectKind;
    msrPrintSpacingKind   fPrintSpacingKind;
};
typedef SMARTP<msrStaffDetails> S_msrStaffDetails;
EXP std::ostream& operator << (std::ostream& os, const S_msrStaffDetails& elt);


}


#endif // ___msrStavesDetails___
