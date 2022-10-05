#ifndef ___msdrLayers___
#define ___msdrLayers___

#include <vector>

#include "smartpointer.h"

#include "msrMeasureElements.h"

#include "msrClefs.h"
#include "msrKeys.h"
#include "msrTimeSignatures.h"
#include "msrNotes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msdrLayer : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdrLayer> create (
                            int           inputLineNumber,
                            const string& layerNumber);

    SMARTP<msdrLayer> createMusicNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    // for regular voices
                          msdrLayer (
                            int           inputLineNumber,
                            const string& layerNumber);

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
    string                getLayerNumber () const
                              { return fLayerNumber; }

    // input line number
    int                   getInputLineNumber () const
                              { return fInputLineNumber; }

    // music measures vector
    const vector<S_msrMeasureElement>&
                          getMeasureElementsVector () const
                              { return fLayerElementsVector; }
    // measures flat list
    const list<S_msrMeasureElement>&
                          getMeasureElementsList () const
                              { return fLayerElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addClefToLayer (
                            int       inputLineNumber,
                            int       measureNumber,
                            S_msrClef clef);

    void                  addKeyToLayer (
                            int      inputLineNumber,
                            int      measureNumber,
                            S_msrKey key);

    void                  addTimeSignatureToLayer (
                            int                inputLineNumber,
                            int                measureNumber,
                            S_msrTimeSignature timeSignature);

    void                  addNoteToLayer (
                            int       inputLineNumber,
                            int       measureNumber,
                            S_msrNote note);

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

    string                asShortString () const;
    string                asString () const;

    void                  displayLayer (
                            int           inputLineNumber,
                            const string& context) const;

    void                  print (ostream& os) const;

    void                  printShort (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // measure number
    string                fLayerNumber;

    // input line number
    int                   fInputLineNumber;

    // measures elements list
    list<S_msrMeasureElement>
                          fLayerElementsList;

  public:

    // public work services
    // ------------------------------------------------------

  private:

    // work fields
    // ------------------------------------------------------

    // measures elements vector
    vector<S_msrMeasureElement>
                          fLayerElementsVector;

    // voice finalization
    Bool                  fLayerHasBeenFinalized;
};
typedef SMARTP<msdrLayer> S_msdrLayer;
EXP ostream& operator<< (ostream& os, const S_msdrLayer& elt);


}


#endif
