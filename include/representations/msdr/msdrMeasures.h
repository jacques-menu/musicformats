/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdrMeasures___
#define ___msdrMeasures___

#include <vector>

#include "smartpointer.h"

#include "msdrLayers.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msdrMeasure : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdrMeasure> create (
                            int           inputLineNumber,
                            const std::string& measureNumber);

    SMARTP<msdrMeasure> createMusicNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    // for regular voices
                          msdrMeasure (
                            int           inputLineNumber,
                            const std::string& measureNumber);

                          msdrMeasure ();

    // destructor
    virtual               ~msdrMeasure ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMeasure ();

  public:

    // set and get
    // ------------------------------------------------------

    // measure number
    std::string           getMeasureNumber () const
                              { return fMeasureNumber; }

    // input line number
    int                   getInputStartLineNumber () const
                              { return fInputStartLineNumber; }

    // music measures vector
    const std::vector<S_msdrLayer>&
                          getMeasureElementsVector () const
                              { return fMeasureElementsVector; }
    // measures flat list
    const std::list<S_msdrLayer>&
                          getMeasureLayersList () const
                              { return fMeasureLayersList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addLayerToMeasure (
                            int         inputLineNumber,
                            int         layerNumber,
                            const S_msdrLayer& layer);

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

    void                  displayMeasure (
                            int                inputLineNumber,
                            const std::string& context) const;

    void                  print (std::ostream& os) const;
    void                  printFull (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // measure number
    std::string           fMeasureNumber;

    // input line number
    int                   fInputStartLineNumber;

    // measures elements list
    std::list<S_msdrLayer>
                          fMeasureLayersList;

  public:

    // public work services
    // ------------------------------------------------------

  private:

    // work fields
    // ------------------------------------------------------

    // measures elements vector
    std::vector<S_msdrLayer>
                          fMeasureElementsVector;

    // voice finalization
    Bool                  fMeasureHasBeenFinalized;
};
typedef SMARTP<msdrMeasure> S_msdrMeasure;
EXP std::ostream& operator << (std::ostream& os, const S_msdrMeasure& elt);


}


#endif // ___msdrMeasures___
