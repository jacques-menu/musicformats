/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrArticulations___
#define ___msrArticulations___

#include "msrElements.h"

#include "msrDirections.h"
#include "msrPlacements.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class msrArticulationKind {
  kArticulation_UNKNOWN_,
  kArticulationAccent, kArticulationBreathMark, kArticulationCaesura,
  kArticulationSpiccato,
  kArticulationStaccato, kArticulationStaccatissimo,
  kArticulationStress, kArticulationUnstress,
  kArticulationDetachedLegato,
  kArticulationStrongAccent, kArticulationTenuto,
  kArticulationFermata, // barLine ??? JMI v0.9.66
  kArticulationArpeggiato, kArticulationNonArpeggiato,
  kArticulationDoit, kArticulationFalloff, kArticulationPlop, kArticulationScoop
};

std::string msrArticulationKindAsString (
  msrArticulationKind articulationKind);

std::ostream& operator << (std::ostream& os, const msrArticulationKind& elt);

//______________________________________________________________________________
class EXP msrArticulation : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrArticulation> create (
                            int                 inputLineNumber,
                            msrArticulationKind articulationKind,
                            msrPlacementKind    articulationPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrArticulation (
                            int                 inputLineNumber,
                            msrArticulationKind articulationKind,
                            msrPlacementKind    articulationPlacementKind);

    virtual               ~msrArticulation ();

  public:

    // set and get
    // ------------------------------------------------------

    msrArticulationKind   getArticulationKind () const
                              { return fArticulationKind; }

    msrPlacementKind      getArticulationPlacementKind () const
                              { return fArticulationPlacementKind; }

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

  protected:

    // protected fields
    // ------------------------------------------------------

    msrArticulationKind   fArticulationKind;

    msrPlacementKind      fArticulationPlacementKind;
};
typedef SMARTP<msrArticulation> S_msrArticulation;
EXP std::ostream& operator << (std::ostream& os, const S_msrArticulation& elt);

//______________________________________________________________________________
// data types

enum class msrFermataShapeKind {
  kArticulationFermataNormal, kArticulationFermataAngled, kArticulationFermataSquare
};

std::string msrFermataShapeKindAsString (
  msrFermataShapeKind fermataShapeKind);

std::ostream& operator << (std::ostream& os, const msrFermataShapeKind& elt);

enum class msrArticulationFermataType {
  kArticulationFermataTypeNone,
  kArticulationFermataTypeUpright, kArticulationFermataTypeInverted
};

std::string msrArticulationFermataTypeAsString (
  msrArticulationFermataType fermataTypeKind);

std::ostream& operator << (std::ostream& os, const msrArticulationFermataType& elt);

class EXP msrFermata : public msrArticulation
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFermata> create (
                            int                        inputLineNumber,
                            msrFermataShapeKind        fermataShapeKind,
                            msrArticulationFermataType fermataTypeKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrFermata (
                            int                        inputLineNumber,
                            msrFermataShapeKind        fermataShapeKind,
                            msrArticulationFermataType fermataTypeKind);

    virtual               ~msrFermata ();

  public:

    // set and get
    // ------------------------------------------------------

    msrFermataShapeKind
                          getFermataShapeKind () const
                              { return fFermataShapeKind; }

    msrArticulationFermataType
                          getFermataTypeKind () const
                              { return fFermataTypeKind; }

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

    msrFermataShapeKind   fFermataShapeKind;

    msrArticulationFermataType
                          fFermataTypeKind;
};
typedef SMARTP<msrFermata> S_msrFermata;
EXP std::ostream& operator << (std::ostream& os, const S_msrFermata& elt);

//______________________________________________________________________________
class EXP msrArpeggiato : public msrArticulation
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrArpeggiato> create (
                            int              inputLineNumber,
                            msrPlacementKind arpeggiatoPlacementKind,
                            msrDirectionKind arpeggiatoDirectionKind,
                            int              arpeggiatoNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrArpeggiato (
                            int              inputLineNumber,
                            msrPlacementKind arpeggiatoPlacementKind,
                            msrDirectionKind arpeggiatoDirectionKind,
                            int              arpeggiatoNumber);

    virtual               ~msrArpeggiato ();

  public:

    // set and get
    // ------------------------------------------------------

    msrDirectionKind      getArpeggiatoDirectionKind () const
                              { return fArpeggiatoDirectionKind; }

    int                   getArpeggiatoNumber () const
                              { return fArpeggiatoNumber; }

  public:

    // public services
    // ------------------------------------------------------

    virtual std::string   arpeggiatoDirectionKindAsString () const;

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

    msrDirectionKind      fArpeggiatoDirectionKind;

    int                   fArpeggiatoNumber;
};
typedef SMARTP<msrArpeggiato> S_msrArpeggiato;
EXP std::ostream& operator << (std::ostream& os, const S_msrArpeggiato& elt);

//______________________________________________________________________________
// data types

enum class msrNonArpeggiatoTypeKind {
  kArticulationNonArpeggiatoTypeNone,
  kArticulationNonArpeggiatoTypeTop, kArticulationNonArpeggiatoTypeBottom
};

std::string msrNonArpeggiatoTypeKindAsString (
  msrNonArpeggiatoTypeKind nonArpeggiatoTypeKind);

std::ostream& operator << (std::ostream& os, const msrNonArpeggiatoTypeKind& elt);

class EXP msrNonArpeggiato : public msrArticulation
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrNonArpeggiato> create (
                            int                      inputLineNumber,
                            msrPlacementKind         nonArpeggiatoPlacementKind,
                            msrNonArpeggiatoTypeKind nonArpeggiatoTypeKind,
                            int                      nonArpeggiatoNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrNonArpeggiato (
                            int                      inputLineNumber,
                            msrPlacementKind         nonArpeggiatoPlacementKind,
                            msrNonArpeggiatoTypeKind nonArpeggiatoTypeKind,
                            int                      nonArpeggiatoNumber);

    virtual               ~msrNonArpeggiato ();

  public:

    // set and get
    // ------------------------------------------------------

    msrNonArpeggiatoTypeKind
                          getNonArpeggiatoTypeKind () const
                              { return fNonArpeggiatoTypeKind; }

    int                   getNonArpeggiatoNumber () const
                              { return fNonArpeggiatoNumber; }

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

    msrNonArpeggiatoTypeKind
                          fNonArpeggiatoTypeKind;

    int                   fNonArpeggiatoNumber;
};
typedef SMARTP<msrNonArpeggiato> S_msrNonArpeggiato;
EXP std::ostream& operator << (std::ostream& os, const S_msrNonArpeggiato& elt);


}


#endif // ___msrArticulations___
