/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrInstruments___
#define ___msrInstruments___

#include <list>

#include "msrElements.h"
#include "msrMeasureElements.h"


#include "msrBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrStringTuning : public msrElement
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

    string                stringTuningKindAsString () const;

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

    int                   fStringTuningNumber;

    msrDiatonicPitchKind  fStringTuningDiatonicPitchKind;
    msrAlterationKind     fStringTuningAlterationKind;
    msrOctaveKind         fStringTuningOctaveKind;
};
typedef SMARTP<msrStringTuning> S_msrStringTuning;
EXP ostream& operator << (ostream& os, const S_msrStringTuning& elt);

//______________________________________________________________________________
class EXP msrScordatura : public msrMeasureElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrScordaturaKind {
      kScordaturaNone,
      kScordaturaUp, kScordaturaDown,
      kScordaturaStop, kScordaturaContinue
    };

    static string scordaturaKindAsString (
      msrScordaturaKind scordaturaKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrScordatura> create (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrScordatura (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure);

    virtual               ~msrScordatura ();

  public:

    // set and get
    // ------------------------------------------------------

    const list<S_msrStringTuning>&
                          getScordaturaStringTuningsList () const
                              { return fScordaturaStringTuningsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addStringTuningToScordatura (
                            S_msrStringTuning stringTuning);

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

    list<S_msrStringTuning>
                          fScordaturaStringTuningsList;
};
typedef SMARTP<msrScordatura> S_msrScordatura;
EXP ostream& operator << (ostream& os, const S_msrScordatura& elt);

//______________________________________________________________________________
class EXP msrAccordionRegistration : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrAccordionRegistration> create (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure,
                            int          highDotsNumber,
                            int          middleDotsNumber,
                            int          lowDotsNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrAccordionRegistration (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure,
                            int          highDotsNumber,
                            int          middleDotsNumber,
                            int          lowDotsNumber);

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

    string                asString () const override;

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

    // see https://de.wikipedia.org/wiki/Register_%28Akkordeon%29
    // for the meaning of the dots numbers

    int                   fHighDotsNumber;
    int                   fMiddleDotsNumber;
    int                   fLowDotsNumber;
};
typedef SMARTP<msrAccordionRegistration> S_msrAccordionRegistration;
EXP ostream& operator << (ostream& os, const S_msrAccordionRegistration& elt);

//______________________________________________________________________________
class EXP msrHarpPedalsTuning : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHarpPedalsTuning> create (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure);

    SMARTP<msrHarpPedalsTuning> createHarpPedalsTuningNewbornClone ();

    SMARTP<msrHarpPedalsTuning> createHarpPedalsTuningDeepClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrHarpPedalsTuning (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure);

    ~ msrHarpPedalsTuning ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<msrDiatonicPitchKind, msrAlterationKind>&
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

    string                asString () const override;

    void                  print (ostream& os) const override;

  public:

    // fields
    // ------------------------------------------------------

    map<msrDiatonicPitchKind, msrAlterationKind>
                          fHarpPedalsAlterationKindsMap;
};
typedef SMARTP<msrHarpPedalsTuning> S_msrHarpPedalsTuning;
EXP ostream& operator << (ostream& os, const S_msrHarpPedalsTuning& elt);

//______________________________________________________________________________
class EXP msrPedal : public msrMeasureElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrPedalTypeKind {
      k_NoPedalType,
      kPedalStart, kPedalContinue, kPedalChange, kPedalStop
    };

    static string pedalTypeKindAsString (
      msrPedalTypeKind pedalTypeKind);

    enum msrPedalLineKind {
      kPedalLineYes, kPedalLineNo
    };

    static string pedalLineKindAsString (
      msrPedalLineKind pedalLineKind);

    enum msrPedalSignKind {
      kPedalSignYes, kPedalSignNo
    };

    static string pedalSignKindAsString (
      msrPedalSignKind pedalSignKind);

    // creation from MusicXML
    // ------------------------------------------------------

                          static SMARTP<msrPedal> create (
                            int              inputLineNumber,
                            S_msrMeasure     upLinkToMeasure,
                            msrPedalTypeKind pedalTypeKind,
                            msrPedalLineKind pedalLineKind,
                            msrPedalSignKind pedalSignKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrPedal (
                            int              inputLineNumber,
                            S_msrMeasure     upLinkToMeasure,
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

    string                pedalTypeAsString () const;

    string                pedalLineAsString () const;

    string                pedalSignAsString () const;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrPedalTypeKind      fPedalTypeKind;
    msrPedalLineKind      fPedalLineKind;
    msrPedalSignKind      fPedalSignKind;
};
typedef SMARTP<msrPedal> S_msrPedal;
EXP ostream& operator << (ostream& os, const S_msrPedal& elt);

//______________________________________________________________________________
class EXP msrDamp : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDamp> create (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrDamp (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure);

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

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<msrDamp> S_msrDamp;
EXP ostream& operator << (ostream& os, const S_msrDamp& elt);

//______________________________________________________________________________
class EXP msrDampAll : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDampAll> create (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrDampAll (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure);

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

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<msrDampAll> S_msrDampAll;
EXP ostream& operator << (ostream& os, const S_msrDampAll& elt);


} // namespace MusicFormats


#endif
