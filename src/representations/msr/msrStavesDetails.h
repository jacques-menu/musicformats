/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrStavesDetails___
#define ___msrStavesDetails___

#include <list>

#include "msrBasicTypes.h"
#include "msrMeasureElements.h"


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

    string                asString () const override;

    void                  print (ostream& os) const override;

  public:

    // fields
    // ------------------------------------------------------

    int                   fStaffTuningLineNumber;

    msrQuarterTonesPitchKind
                          fStaffTuningQuarterTonesPitchKind;
    msrOctaveKind         fStaffTuningOctaveKind;
};
typedef SMARTP<msrStaffTuning> S_msrStaffTuning;
EXP ostream& operator << (ostream& os, const S_msrStaffTuning& elt);

//______________________________________________________________________________
class EXP msrStaffDetails : public msrMeasureElement
{
  public:


    // data types
    // ------------------------------------------------------

    enum msrStaffTypeKind {
      kRegularStaffType,
      kOssiaStaffType, kCueStaffType, kEditorialStaffType,
      kAlternateStaffType
    };

    static string staffTypeKindAsString (
      msrStaffTypeKind staffTypeKind);

    enum msrShowFretsKind {
      kShowFretsNumbers, kShowFretsLetters
    };

    static string showFretsKindAsString (
      msrShowFretsKind showFretsKind);

    enum msrPrintSpacingKind {
      kPrintSpacingYes, kPrintSpacingNo
    };

    static string printSpacingKindAsString (
      msrPrintSpacingKind printSpacingKind);

    // creation from MusicXML
    // ------------------------------------------------------

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
                            msrStaffTypeKind    staffTypeKind,
                            msrShowFretsKind    showFretsKind,
                            msrPrintObjectKind  printObjectKind,
                            msrPrintSpacingKind printSpacingKin);

    virtual               ~msrStaffDetails ();

  public:

    // set and get
    // ------------------------------------------------------

    msrStaffTypeKind      getStaffTypeKind () const
                              { return fStaffTypeKind; }

    void                  setStaffLinesNumber (
                            int staffLinesNumber)
                              { fStaffLinesNumber = staffLinesNumber; }

    int                   getStaffLinesNumber () const
                              { return fStaffLinesNumber; }

    const list<S_msrStaffTuning>&
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
                            S_msrStaffTuning staffTuning)
                              {
                                fStaffTuningsList.push_back (staffTuning);
                              }

    string                asShortString () const override;

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

  private:

    // private fields
    // ------------------------------------------------------

    msrStaffTypeKind      fStaffTypeKind;

    int                   fStaffLinesNumber;
    list<S_msrStaffTuning>
                          fStaffTuningsList;

    msrShowFretsKind      fShowFretsKind;

    msrPrintObjectKind    fPrintObjectKind;
    msrPrintSpacingKind   fPrintSpacingKind;
};
typedef SMARTP<msrStaffDetails> S_msrStaffDetails;
EXP ostream& operator << (ostream& os, const S_msrStaffDetails& elt);


} // namespace MusicFormats


#endif
