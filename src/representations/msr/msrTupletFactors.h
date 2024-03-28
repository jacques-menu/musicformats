/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

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
                            const mfRational& rationalTupletFactor);

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

    mfRational            asRational () const
                              {
                              return
                                mfRational (
                                  fTupletActualNotes,
                                  fTupletNormalNotes);
                            }

    std::string           asFractionString () const;

  public:

    // visitors
    // ------------------------------------------------------

  public:

    // ------------------------------------------------------

    std::string           asShortString () const;
    std::string           asString () const
                              { return asShortString (); };

    virtual void          print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fTupletActualNotes;
    int                   fTupletNormalNotes;
};
EXP std::ostream& operator << (std::ostream& os, const msrTupletFactor& elt);


}


#endif // ___msrTupletFactors___
