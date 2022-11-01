/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrArticulations___
#define ___msrArticulations___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrArticulation : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrArticulationKind {
      k_NoArticulation,

      kAccent, kBreathMark, kCaesura,
      kSpiccato,
      kStaccato, kStaccatissimo,
      kStress, kUnstress,
      kDetachedLegato,
      kStrongAccent, kTenuto,
      kFermata, // barLine ??? JMI
      kArpeggiato, kNonArpeggiato,
      kDoit, kFalloff, kPlop, kScoop
    };

    static string articulationKindAsString (
      msrArticulationKind articulationKind);

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

    virtual string        articulationKindAsString () const;

    virtual string        articulationPlacementKindAsString () const;

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

  protected:

    // protected fields
    // ------------------------------------------------------

    msrArticulationKind   fArticulationKind;

    msrPlacementKind      fArticulationPlacementKind;
};
typedef SMARTP<msrArticulation> S_msrArticulation;
EXP ostream& operator << (ostream& os, const S_msrArticulation& elt);

//______________________________________________________________________________
class EXP msrFermata : public msrArticulation
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrFermataKind {
        kNormalFermataKind, kAngledFermataKind, kSquareFermataKind
      };

    static string fermataKindAsString (
      msrFermataKind fermataKind);

    enum msrFermataTypeKind {
      kFermataTypeNone,
      kFermataTypeUpright, kFermataTypeInverted
    };

    static string fermataTypeKindAsString (
      msrFermataTypeKind fermataTypeKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFermata> create (
                            int                inputLineNumber,
                            msrFermataKind     fermataKind,
                            msrFermataTypeKind fermataTypeKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrFermata (
                            int                inputLineNumber,
                            msrFermataKind     fermataKind,
                            msrFermataTypeKind fermataTypeKind);

    virtual               ~msrFermata ();

  public:

    // set and get
    // ------------------------------------------------------

    msrFermataKind        getFermataKind () const
                              { return fFermataKind; }

    msrFermataTypeKind    getFermataTypeKind () const
                              { return fFermataTypeKind; }

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

    msrFermataKind        fFermataKind;

    msrFermataTypeKind    fFermataTypeKind;
};
typedef SMARTP<msrFermata> S_msrFermata;
EXP ostream& operator << (ostream& os, const S_msrFermata& elt);

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

    virtual string        arpeggiatoDirectionKindAsString () const;

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

    msrDirectionKind      fArpeggiatoDirectionKind;

    int                   fArpeggiatoNumber;
};
typedef SMARTP<msrArpeggiato> S_msrArpeggiato;
EXP ostream& operator << (ostream& os, const S_msrArpeggiato& elt);

//______________________________________________________________________________
class EXP msrNonArpeggiato : public msrArticulation
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrNonArpeggiatoTypeKind {
      kNonArpeggiatoTypeNone,
      kNonArpeggiatoTypeTop, kNonArpeggiatoTypeBottom
    };

    static string nonArpeggiatoTypeKindAsString (
      msrNonArpeggiatoTypeKind nonArpeggiatoTypeKind);

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

    virtual string        nonArpeggiatoTypeKindAsString () const;

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

    msrNonArpeggiatoTypeKind
                          fNonArpeggiatoTypeKind;

    int                   fNonArpeggiatoNumber;
};
typedef SMARTP<msrNonArpeggiato> S_msrNonArpeggiato;
EXP ostream& operator << (ostream& os, const S_msrNonArpeggiato& elt);


} // namespace MusicFormats


#endif
