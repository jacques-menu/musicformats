/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrTupletFactors___
#define ___msrTupletFactors___

#include "mfRational.h"


namespace MusicFormats
{

// tuplet factors
//______________________________________________________________________________
class EXP msrTupletFactor
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrTupletFactor ();

                          msrTupletFactor (
                            int tupletActualNotes,
                            int tupletNormalNotes);

                          msrTupletFactor (
                            const Rational& rationalTupletFactor);

    virtual               ~msrTupletFactor ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setTupletActualNotes (int actualNotes)
                              { fTupletActualNotes = actualNotes; }

    int                   getTupletActualNotes () const
                              { return fTupletActualNotes; }

    void                  setTupletNormalNotes (int normalNotes)
                              { fTupletNormalNotes = normalNotes; }

    int                   getTupletNormalNotes () const
                              { return fTupletNormalNotes; }

  public:

    // public services
    // ------------------------------------------------------

    Bool                  isEqualToOne () const
                              {
                                return
                                  fTupletActualNotes == fTupletNormalNotes;
                              }

    msrTupletFactor       inverse () const;

    Rational              asRational () const
                            {
                              return
                                Rational (
                                  fTupletActualNotes,
                                  fTupletNormalNotes);
                            }

    string                asFractionString () const;

  public:

    // visitors
    // ------------------------------------------------------

/* JMI
    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;
*/

  public:

    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fTupletActualNotes;
    int                   fTupletNormalNotes;
};
EXP ostream& operator << (ostream& os, const msrTupletFactor& elt);


}


#endif