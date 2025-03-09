/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrInstruments___
#define ___msrInstruments___

#include <list>

#include "msrMeasureElements.h"

#include "msrMeasures.h"

#include "msrOctaves.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrStringTuning : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStringTuning> create (
                            int                  inputLineNumber,
                            int                  stringTuningNumber,
                            msrDiatonicPitchKind stringTuningDiatonicPitchKind,
                            msrAlterationKind    stringTuningAlterationKind,
                            msrOctaveKind        stringTuningOctaveKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrStringTuning (
                            int                  inputLineNumber,
                            int                  stringTuningNumber,
                            msrDiatonicPitchKind stringTuningDiatonicPitchKind,
                            msrAlterationKind    stringTuningAlterationKind,
                            msrOctaveKind        stringTuningOctaveKind);

    virtual               ~msrStringTuning ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getStringTuningNumber () const
                              { return fStringTuningNumber; }

    msrDiatonicPitchKind  getStringTuningDiatonicPitchKind () const
                              { return fStringTuningDiatonicPitchKind; }

    msrAlterationKind     getStringTuningAlterationKind () const
                              { return fStringTuningAlterationKind; }

    msrOctaveKind         getStringTuningOctaveKind () const
                              { return fStringTuningOctaveKind; }

  public:

    // public services
    // ------------------------------------------------------

    std::string           stringTuningKindAsString () const;

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

    int                   fStringTuningNumber;

    msrDiatonicPitchKind  fStringTuningDiatonicPitchKind;
    msrAlterationKind     fStringTuningAlterationKind;
    msrOctaveKind         fStringTuningOctaveKind;
};
typedef SMARTP<msrStringTuning> S_msrStringTuning;
EXP std::ostream& operator << (std::ostream& os, const S_msrStringTuning& elt);

//______________________________________________________________________________
// data types

enum class msrScordaturaKind {
  kScordaturaNone,
  kScordaturaUp, kScordaturaDown,
  kScordaturaStop, kScordaturaContinue
};

std::string msrScordaturaKindAsString (
  msrScordaturaKind scordaturaKind);

std::ostream& operator << (std::ostream& os, const msrScordaturaKind& elt);

//______________________________________________________________________________
class EXP msrScordatura : public msrMeasureElement
{
  public:

    // ------------------------------------------------------

    static SMARTP<msrScordatura> create (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

    static SMARTP<msrScordatura> create (
                            int                 inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrScordatura (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

    virtual               ~msrScordatura ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::list <S_msrStringTuning>&
                          getScordaturaStringTuningsList () const
                              { return fScordaturaStringTuningsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addStringTuningToScordatura (
                            const S_msrStringTuning& stringTuning);

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

    std::list <S_msrStringTuning>
                          fScordaturaStringTuningsList;
};
typedef SMARTP<msrScordatura> S_msrScordatura;
EXP std::ostream& operator << (std::ostream& os, const S_msrScordatura& elt);

//______________________________________________________________________________
class EXP msrAccordionRegistration : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrAccordionRegistration> create (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            int           highDotsNumber,
                            int           middleDotsNumber,
                            int           lowDotsNumber);

    static SMARTP<msrAccordionRegistration> create (
                            int           inputLineNumber,
                            int           highDotsNumber,
                            int           middleDotsNumber,
                            int           lowDotsNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrAccordionRegistration (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure,
                            int           highDotsNumber,
                            int           middleDotsNumber,
                            int           lowDotsNumber);

    virtual               ~msrAccordionRegistration ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getHighDotsNumber () const
                              { return fHighDotsNumber; }

    int                   getMiddleDotsNumber () const
                              { return fMiddleDotsNumber; }

    int                   getLowDotsNumber () const
                              { return fLowDotsNumber; }

  public:

    // public services
    // ------------------------------------------------------

    std::string           asString () const override;

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

    // see https://de.wikipedia.org/wiki/Register_%28Akkordeon%29
    // for the meaning of the dots numbers

    int                   fHighDotsNumber;
    int                   fMiddleDotsNumber;
    int                   fLowDotsNumber;
};
typedef SMARTP<msrAccordionRegistration> S_msrAccordionRegistration;
EXP std::ostream& operator << (std::ostream& os, const S_msrAccordionRegistration& elt);

//______________________________________________________________________________
class EXP msrHarpPedalsTuning : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHarpPedalsTuning> create (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

    static SMARTP<msrHarpPedalsTuning> create (
                            int                 inputLineNumber);

    SMARTP<msrHarpPedalsTuning> createHarpPedalsTuningNewbornClone ();

    SMARTP<msrHarpPedalsTuning> createHarpPedalsTuningDeepClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrHarpPedalsTuning (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

                          msrHarpPedalsTuning (
                            int                 inputLineNumber);

    ~ msrHarpPedalsTuning ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::map <msrDiatonicPitchKind, msrAlterationKind>&
                          getHarpPedalsAlterationKindsMap () const
                              { return fHarpPedalsAlterationKindsMap; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addPedalTuning (
                            int                  intputLineNumber,
                            msrDiatonicPitchKind diatonicPitchKind,
                            msrAlterationKind    alterationKind);

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

    std::map <msrDiatonicPitchKind, msrAlterationKind>
                          fHarpPedalsAlterationKindsMap;
};
typedef SMARTP<msrHarpPedalsTuning> S_msrHarpPedalsTuning;
EXP std::ostream& operator << (std::ostream& os, const S_msrHarpPedalsTuning& elt);

//______________________________________________________________________________
// data types

enum class msrPedalTypeKind {
  kPedalType_UNKNOWN_,
  kPedalTypeStart, kPedalTypeContinue, kPedalTypeChange, kPedalTypeStop
};

std::string msrPedalTypeKindAsString (
  msrPedalTypeKind pedalTypeKind);

std::ostream& operator << (std::ostream& os, const msrPedalTypeKind& elt);

enum class msrPedalLineKind {
  kPedalLineYes, kPedalLineNo
};

std::string msrPedalLineKindAsString (
  msrPedalLineKind pedalLineKind);

std::ostream& operator << (std::ostream& os, const msrPedalLineKind& elt);

enum class msrPedalSignKind {
  kPedalSignYes, kPedalSignNo
};

std::string msrPedalSignKindAsString (
  msrPedalSignKind pedalSignKind);

std::ostream& operator << (std::ostream& os, const msrPedalSignKind& elt);

class EXP msrPedal : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

                          static SMARTP<msrPedal> create (
                            int              inputLineNumber,
                            const S_msrMeasure&     upLinkToMeasure,
                            msrPedalTypeKind pedalTypeKind,
                            msrPedalLineKind pedalLineKind,
                            msrPedalSignKind pedalSignKind);

                          static SMARTP<msrPedal> create (
                            int              inputLineNumber,
                            msrPedalTypeKind pedalTypeKind,
                            msrPedalLineKind pedalLineKind,
                            msrPedalSignKind pedalSignKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrPedal (
                            int              inputLineNumber,
                            const S_msrMeasure&     upLinkToMeasure,
                            msrPedalTypeKind pedalTypeKind,
                            msrPedalLineKind pedalLineKind,
                            msrPedalSignKind pedalSignKind);

    virtual               ~msrPedal ();

  public:

    // set and get
    // ------------------------------------------------------

    msrPedalTypeKind      getPedalTypeKind () const
                              { return fPedalTypeKind; }

    msrPedalLineKind      getPedalLineKind () const
                              { return fPedalLineKind; }

    msrPedalSignKind      getPedalSignKind () const
                              { return fPedalSignKind; }

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

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrPedalTypeKind      fPedalTypeKind;
    msrPedalLineKind      fPedalLineKind;
    msrPedalSignKind      fPedalSignKind;
};
typedef SMARTP<msrPedal> S_msrPedal;
EXP std::ostream& operator << (std::ostream& os, const S_msrPedal& elt);

//______________________________________________________________________________
class EXP msrDamp : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDamp> create (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

    static SMARTP<msrDamp> create (
                            int           inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrDamp (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

    virtual               ~msrDamp ();

  public:

    // set and get
    // ------------------------------------------------------

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

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

};
typedef SMARTP<msrDamp> S_msrDamp;
EXP std::ostream& operator << (std::ostream& os, const S_msrDamp& elt);

//______________________________________________________________________________
class EXP msrDampAll : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDampAll> create (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

    static SMARTP<msrDampAll> create (
                            int           inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrDampAll (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

    virtual               ~msrDampAll ();

  public:

    // set and get
    // ------------------------------------------------------

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

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

};
typedef SMARTP<msrDampAll> S_msrDampAll;
EXP std::ostream& operator << (std::ostream& os, const S_msrDampAll& elt);


}


#endif // ___msrInstruments___
