/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrTechnicals___
#define ___msrTechnicals___

#include "msrElements.h"

#include "msrPlacements.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class msrTechnicalKind {
  kTechnicalArrow, // rich JMI
  kTechnicalDoubleTongue,
  kTechnicalDownBow,
  kTechnicalFingernails,
  kTechnicalHarmonic,
  kTechnicalHeel,
  kTechnicalHole, // rich JMI ??? v0.9.66
  kTechnicalOpenString,
  kTechnicalSnapPizzicato,
  kTechnicalStopped,
  kTechnicalTap,
  kTechnicalThumbPosition,
  kTechnicalToe,
  kTechnicalTripleTongue,
  kTechnicalUpBow
};

std::string msrTechnicalKindAsString (
  msrTechnicalKind technicalKind);

std::ostream& operator << (std::ostream& os, const msrTechnicalKind& elt);

/* JMI v0.9.66
enum class msrTechnicalAccidentalMarkKind {
  msrAlterationKind::kAlterationNatural, msrAlterationKind::kAlterationSharp, msrAlterationKind::kAlterationFlat
};

std::string technicalAccidentalMarkKindAsString (
  msrTechnicalAccidentalMarkKind technicalAccidentalMarkKind);

std::ostream& operator << (std::ostream& os, const msrTechnicalAccidentalMarkKind& elt);

*/

enum class msrTechnicalTypeKind { // JMI ???
  kTechnicalType_UNKNOWN_,
  kTechnicalTypeStart, kTechnicalTypeStop
};

std::string msrTechnicalTypeKindAsString (
  msrTechnicalTypeKind technicalTypeKind);

std::ostream& operator << (std::ostream& os, const msrTechnicalTypeKind& elt);

// data types

enum class msrTechnicalWithFloatKind {
  kTechnicalWithFloatBend
};

std::string msrTechnicalWithFloatKindAsString (
  msrTechnicalWithFloatKind technicalWithFloatKind);

std::ostream& operator << (std::ostream& os, const msrTechnicalWithFloatKind& elt);

enum class msrTechnicalWithIntegerKind {
  kFingering,
  kFret,
  kString
};

std::string msrTechnicalWithIntegerKindAsString (
  msrTechnicalWithIntegerKind technicalWithIntegerKind);

std::ostream& operator << (std::ostream& os, const msrTechnicalWithIntegerKind& elt);

enum class msrTechnicalWithStringKind {
  kHammerOn,
  kHandbell,
  kOtherTechnical,
  kPluck,
  kPullOff
};

std::string msrTechnicalWithStringKindAsString (
  msrTechnicalWithStringKind technicalWithStringKind);

std::ostream& operator << (std::ostream& os, const msrTechnicalWithStringKind& elt);

//______________________________________________________________________________
class EXP msrTechnical : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTechnical> create (
                            int              inputLineNumber,
                            msrTechnicalKind technicalKind,
                            msrPlacementKind technicalPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTechnical (
                            int              inputLineNumber,
                            msrTechnicalKind technicalKind,
                            msrPlacementKind technicalPlacementKind);

    virtual               ~msrTechnical ();

  public:

    // set and get
    // ------------------------------------------------------

    msrTechnicalKind      getTechnicalKind () const
                              { return fTechnicalKind; }

    void                  setTechnicalPlacementKind (
                            msrPlacementKind technicalPlacementKind)
                              {
                                fTechnicalPlacementKind =
                                  technicalPlacementKind;
                              }

    msrPlacementKind      getTechnicalPlacementKind () const
                              { return fTechnicalPlacementKind; }

/* JMI
    void                  setTechnicalAccidentalMarkKind (
                            msrTechnicalAccidentalMarkKind
                              technicalAccidentalMarkKind)
                              {
                                fTechnicalAccidentalMarkKind =
                                  technicalAccidentalMarkKind;
                              }

    msrTechnicalAccidentalMarkKind
                          getTechnicalAccidentalMarkKind () const
                              { return fTechnicalAccidentalMarkKind; }
*/

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

    msrTechnicalKind      fTechnicalKind;

    msrPlacementKind      fTechnicalPlacementKind;

// JMI    msrTechnicalAccidentalMarkKind  fTechnicalAccidentalMarkKind;
};
typedef SMARTP<msrTechnical> S_msrTechnical;
EXP std::ostream& operator << (std::ostream& os, const S_msrTechnical& elt);

//______________________________________________________________________________
class EXP msrTechnicalWithInteger : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTechnicalWithInteger> create (
                            int                         inputLineNumber,
                            msrTechnicalWithIntegerKind technicalWithIntegerKind,
                            int                         technicalWithIntegerValue,
                            msrPlacementKind            technicalWithIntegerPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTechnicalWithInteger (
                            int                         inputLineNumber,
                            msrTechnicalWithIntegerKind technicalWithIntegerKind,
                            int                         technicalWithIntegerValue,
                            msrPlacementKind            technicalWithIntegerPlacementKind);

    virtual               ~msrTechnicalWithInteger ();

  public:

    // set and get
    // ------------------------------------------------------

    msrTechnicalWithIntegerKind
                          getTechnicalWithIntegerKind () const
                              { return fTechnicalWithIntegerKind; }

    int                   getTechnicalWithIntegerValue () const
                              { return fTechnicalWithIntegerValue; }

    void                  setTechnicalWithIntegerPlacementKind (
                            msrPlacementKind
                              technicalWithIntegerPlacementKind)
                              {
                                fTechnicalWithIntegerPlacementKind =
                                  technicalWithIntegerPlacementKind;
                              }

    msrPlacementKind      getTechnicalWithIntegerPlacementKind () const
                              { return fTechnicalWithIntegerPlacementKind; }

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

    msrTechnicalWithIntegerKind
                          fTechnicalWithIntegerKind;

    int                   fTechnicalWithIntegerValue;

    msrPlacementKind      fTechnicalWithIntegerPlacementKind;
};
typedef SMARTP<msrTechnicalWithInteger> S_msrTechnicalWithInteger;
EXP std::ostream& operator << (std::ostream& os, const S_msrTechnicalWithInteger& elt);

//______________________________________________________________________________
class EXP msrTechnicalWithFloat : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTechnicalWithFloat> create (
                            int                       inputLineNumber,
                            msrTechnicalWithFloatKind technicalWithFloatKind,
                            float                     technicalWithFloatValue,
                            msrPlacementKind          technicalWithFloatPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTechnicalWithFloat (
                            int                       inputLineNumber,
                            msrTechnicalWithFloatKind technicalWithFloatKind,
                            float                     technicalWithFloatValue,
                            msrPlacementKind          technicalWithFloatPlacementKind);

    virtual               ~msrTechnicalWithFloat ();

  public:

    // set and get
    // ------------------------------------------------------

    msrTechnicalWithFloatKind
                          getTechnicalWithFloatKind () const
                              { return fTechnicalWithFloatKind; }

    float                 getTechnicalWithFloatValue () const
                              { return fTechnicalWithFloatValue; }

    void                  setTechnicalWithFloatPlacementKind (
                            msrPlacementKind
                              technicalWithFloatPlacementKind)
                              {
                                fTechnicalWithFloatPlacementKind =
                                  technicalWithFloatPlacementKind;
                              }

    msrPlacementKind      getTechnicalWithFloatPlacementKind () const
                              { return fTechnicalWithFloatPlacementKind; }

  public:

    // public services
    // ------------------------------------------------------

    std::string           technicalWithFloatAccidentalMarkKindAsString () const;

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

    msrTechnicalWithFloatKind
                          fTechnicalWithFloatKind;

    float                 fTechnicalWithFloatValue;

    msrPlacementKind      fTechnicalWithFloatPlacementKind;
};
typedef SMARTP<msrTechnicalWithFloat> S_msrTechnicalWithFloat;
EXP std::ostream& operator << (std::ostream& os, const S_msrTechnicalWithFloat& elt);

//______________________________________________________________________________
class EXP msrTechnicalWithString : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTechnicalWithString> create (
                            int                        inputLineNumber,
                            msrTechnicalWithStringKind technicalWithStringKind,
                            msrTechnicalTypeKind       technicalWithStringTypeKind,
                            const std::string&         technicalWithStringValue,
                            msrPlacementKind           technicalWithStringPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTechnicalWithString (
                            int                        inputLineNumber,
                            msrTechnicalWithStringKind technicalWithStringKind,
                            msrTechnicalTypeKind       technicalWithStringTypeKind,
                            const std::string&         technicalWithStringValue,
                            msrPlacementKind           technicalWithStringPlacementKind);

    virtual               ~msrTechnicalWithString ();

  public:

    // set and get
    // ------------------------------------------------------

    msrTechnicalWithStringKind
                          getTechnicalWithStringKind () const
                              { return fTechnicalWithStringKind; }

    msrTechnicalTypeKind  getTechnicalWithStringTypeKind () const
                              { return fTechnicalWithStringTypeKind; }

    std::string           getTechnicalWithStringValue () const
                              { return fTechnicalWithStringValue; }

    void                  setTechnicalWithStringPlacementKind (
                            msrPlacementKind
                              technicalWithStringPlacementKind)
                              {
                                fTechnicalWithStringPlacementKind =
                                  technicalWithStringPlacementKind;
                              }

    msrPlacementKind      getTechnicalWithStringPlacementKind () const
                              { return fTechnicalWithStringPlacementKind; }

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

    msrTechnicalWithStringKind
                          fTechnicalWithStringKind;

    msrTechnicalTypeKind  fTechnicalWithStringTypeKind;

    std::string           fTechnicalWithStringValue;

    msrPlacementKind      fTechnicalWithStringPlacementKind;
};
typedef SMARTP<msrTechnicalWithString> S_msrTechnicalWithString;
EXP std::ostream& operator << (std::ostream& os, const S_msrTechnicalWithString& elt);


}


#endif // ___msrTechnicals___
