/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrDynamic___
#define ___msrDynamic___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrDynamic : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDynamic> create (
                            int              inputLineNumber,
                            msrDynamicKind  dynamicKind,
                            msrPlacementKind dynamicPlacementKind);

    // creation from the applications
    // ------------------------------------------------------

    static SMARTP<msrDynamic> createDynamicFromString (
                            int              inputLineNumber,
                            const string&    dynamicsString,
                            msrPlacementKind dynamicPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrDynamic (
                            int              inputLineNumber,
                            msrDynamicKind  dynamicKind,
                            msrPlacementKind dynamicPlacementKind);

    virtual               ~msrDynamic ();

  public:

    // set and get
    // ------------------------------------------------------

    msrDynamicKind       getDynamicKind () const
                              { return fDynamicKind; }

    msrPlacementKind      getDynamicPlacementKind () const
                              { return fDynamicPlacementKind; }

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

    msrDynamicKind       fDynamicKind;

    msrPlacementKind      fDynamicPlacementKind;
};
typedef SMARTP<msrDynamic> S_msrDynamic;
EXP ostream& operator<< (ostream& os, const S_msrDynamic& elt);

//______________________________________________________________________________
class EXP msrOtherDynamic : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrOtherDynamic> create (
                            int              inputLineNumber,
                            const string&    otherDynamicString,
                            msrPlacementKind otherDynamicPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrOtherDynamic (
                            int              inputLineNumber,
                            const string&    otherDynamicString,
                            msrPlacementKind otherDynamicPlacementKind);

    virtual               ~msrOtherDynamic ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getOtherDynamicsString () const
                              { return fOtherDynamicsString; }

    msrPlacementKind      getOtherDynamicPlacementKind () const
                              { return fOtherDynamicPlacementKind; }

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

  private:

    // private fields
    // ------------------------------------------------------

    string                fOtherDynamicsString;

    msrPlacementKind      fOtherDynamicPlacementKind;
};
typedef SMARTP<msrOtherDynamic> S_msrOtherDynamic;
EXP ostream& operator<< (ostream& os, const S_msrOtherDynamic& elt);

//______________________________________________________________________________
enum class msrCrescDecrescKind {
  kCrescDecrescCrescendo, kCrescDecrescDecrescendo
};

string crescDecrescKindAsString (
  msrCrescDecrescKind crescDecrescKind);

ostream& operator<< (ostream& os,const msrCrescDecrescKind& elt);

//______________________________________________________________________________
class EXP msrCrescDecresc : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrCrescDecresc> create (
                            int                 inputLineNumber,
                            msrCrescDecrescKind crescDecrescKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrCrescDecresc (
                            int                 inputLineNumber,
                            msrCrescDecrescKind crescDecrescKind);

    virtual               ~msrCrescDecresc ();

  public:

    // set and get
    // ------------------------------------------------------

    msrCrescDecrescKind   getCrescDecrescKind () const
                              { return fCrescDecrescKind; }

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

    msrCrescDecrescKind   fCrescDecrescKind;
};
typedef SMARTP<msrCrescDecresc> S_msrCrescDecresc;
EXP ostream& operator<< (ostream& os, const S_msrCrescDecresc& elt);

//______________________________________________________________________________
enum class msrWedgeKind {
  kWedgeKindNone,
  kWedgeCrescendo, kWedgeDecrescendo, kWedgeStop
};

string wedgeKindAsString (
  msrWedgeKind wedgeKind);

ostream& operator<< (ostream& os,const msrWedgeKind& elt);

enum class msrWedgeNienteKind {
  kWedgeNienteYes, kWedgeNienteNo
};

string wedgeNienteKindAsString (
  msrWedgeNienteKind wedgeNienteKind);

ostream& operator<< (ostream& os,const msrWedgeNienteKind& elt);

//______________________________________________________________________________
class EXP msrWedge : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrWedge> create (
                            int                inputLineNumber,
                            msrWedgeKind       wedgeKind,
                            msrWedgeNienteKind wedgeNienteKind,
                            msrLineTypeKind    wedgeLineTypeKind,
                            msrPlacementKind   wedgePlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrWedge (
                            int                inputLineNumber,
                            msrWedgeKind       wedgeKind,
                            msrWedgeNienteKind wedgeNienteKind,
                            msrLineTypeKind    wedgeLineTypeKind,
                            msrPlacementKind   wedgePlacementKind);

    virtual               ~msrWedge ();

  public:

    // set and get
    // ------------------------------------------------------

    msrWedgeKind          getWedgeKind () const
                              { return fWedgeKind; }

    msrWedgeNienteKind    getWedgeNienteKind () const
                              { return fWedgeNienteKind; }

    msrLineTypeKind       getWedgeLineTypeKind () const
                              { return fWedgeLineTypeKind; }

    msrPlacementKind      getWedgePlacementKind () const
                              { return fWedgePlacementKind; }

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

    msrWedgeKind          fWedgeKind;

    msrWedgeNienteKind    fWedgeNienteKind;

    msrLineTypeKind       fWedgeLineTypeKind;

    msrPlacementKind      fWedgePlacementKind;
};
typedef SMARTP<msrWedge> S_msrWedge;
EXP ostream& operator<< (ostream& os, const S_msrWedge& elt);


} // namespace MusicFormats


#endif
