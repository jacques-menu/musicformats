/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdrLayers___
#define ___msdrLayers___

#include <vector>

#include "smartpointer.h"

#include "msrClefKeyTimeSignatureGroups.h"
#include "msrClefKeyTimeSignatureGroups.h"
#include "msrClefKeyTimeSignatureGroups.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msdrLayer : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdrLayer> create (
                            int                inputLineNumber,
                            const std::string& layerNumber);

    SMARTP<msdrLayer> createMusicNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    // for regular voices
                          msdrLayer (
                            int                inputLineNumber,
                            const std::string& layerNumber);

                          msdrLayer ();

    // destructor
    virtual               ~msdrLayer ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeLayer ();

  public:

    // set and get
    // ------------------------------------------------------

    // measure number
    std::string           getLayerNumber () const
                              { return fLayerNumber; }

    // input line number
    int                   getInputLineNumber () const
                              { return fInputLineNumber; }

    // music measures vector
    const std::vector <S_msrMeasureElement>&
                          getMeasureElementsVector () const
                              { return fLayerElementsVector; }
    // measures flat list
    const std::list <S_msrMeasureElement>&
                          getMeasureElementsList () const
                              { return fLayerElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addClefToLayer (
                            int              inputLineNumber,
                            int              measureNumber,
                            const S_msrClef& clef);

    void                  addKeyToLayer (
                            int   inputLineNumber,
                            int   measureNumber,
                            const S_msrKey& key);

    void                  addTimeSignatureToLayer (
                            int                       inputLineNumber,
                            int                       measureNumber,
                            const S_msrTimeSignature& timeSignature);

    void                  addNoteToLayer (
                            int   inputLineNumber,
                            int   measureNumber,
                            const S_msrNote& note);

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

    void                  displayLayer (
                            int                inputLineNumber,
                            const std::string& context) const;

    void                  print (std::ostream& os) const;
    void                  printFull (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // measure number
    std::string           fLayerNumber;

    // input line number
    int                   fInputLineNumber;

    // measures elements list
    std::list <S_msrMeasureElement>
                          fLayerElementsList;

  public:

    // public work services
    // ------------------------------------------------------

  private:

    // work fields
    // ------------------------------------------------------

    // measures elements vector
    std::vector <S_msrMeasureElement>
                          fLayerElementsVector;

    // voice finalization
    Bool                  fLayerHasBeenFinalized;
};
typedef SMARTP<msdrLayer> S_msdrLayer;
EXP std::ostream& operator << (std::ostream& os, const S_msdrLayer& elt);


}


#endif // ___msdrLayers___
