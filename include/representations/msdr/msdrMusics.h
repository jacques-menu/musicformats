/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdrMusics___
#define ___msdrMusics___

#include <vector>
#include <list> // JMI ???

#include "smartpointer.h"

#include "msdrMeasures.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msdrMusic : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdrMusic> create (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& musicName);

    SMARTP<msdrMusic> createMusicNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdrMusic (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& musicName);

                          msdrMusic ();

    virtual               ~msdrMusic ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMusic ();

  public:

    // set and get
    // ------------------------------------------------------

    // music name
    std::string           getMusicName () const
                              { return fMusicName; }

    // input line number
    mfInputLineNumber     getInputLineNumber () const
                              { return fInputLineNumber; }

    // music measures vector
    const std::vector <S_msdrMeasure>&
                          getMusicMeasuresVector () const
                              { return fMusicMeasuresVector; }
    // measures list
    const std::list <S_msdrMeasure>&
                          getMusicMeasuresList () const
                              { return fMusicMeasuresList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addMeasureToMusic (
                            const mfInputLineNumber& inputLineNumber,
                            int           measureNumber,
                            const S_msdrMeasure& measure);

  private:

    // private services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v);
    void                  acceptOut (basevisitor* v);

    void                  browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortString () const;
    std::string           asString () const;

    void                  displayMusic (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& context) const;

    void                  print (std::ostream& os) const;
    void                  printFull (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // music name
    std::string           fMusicName;

    // input line number
    mfInputLineNumber     fInputLineNumber;

    // measures vector
    std::vector <S_msdrMeasure>
                          fMusicMeasuresVector;

  public:

    // public work services
    // ------------------------------------------------------

  private:

    // work fields
    // ------------------------------------------------------

    // measures list
    std::list <S_msdrMeasure>
                          fMusicMeasuresList; // JMI

    // voice finalization
    Bool                  fMusicHasBeenFinalized;
};
typedef SMARTP<msdrMusic> S_msdrMusic;
EXP std::ostream& operator << (std::ostream& os, const S_msdrMusic& elt);


}


#endif // ___msdrMusics___
