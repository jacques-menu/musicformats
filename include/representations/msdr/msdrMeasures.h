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
    int                   getInputLineNumber () const
                              { return fInputLineNumber; }

    // music measures std::vector
    const std::vector<S_msdrLayer>&
                          getMeasureElementsVector () const
                              { return fMeasureElementsVector; }
    // measures flat std::list
    const std::list<S_msdrLayer>&
                          getMeasureLayersList () const
                              { return fMeasureLayersList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addLayerToMeasure (
                            int         inputLineNumber,
                            int         layerNumber,
                            S_msdrLayer layer);

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
                            int           inputLineNumber,
                            const std::string& context) const;

    void                  print (std::ostream& os) const;

    void                  printShort (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // measure number
    std::string           fMeasureNumber;

    // input line number
    int                   fInputLineNumber;

    // measures elements list
    std::list<S_msdrLayer>
                          fMeasureLayersList;

  public:

    // public work services
    // ------------------------------------------------------

  private:

    // work fields
    // ------------------------------------------------------

    // measures elements std::vector
    std::vector<S_msdrLayer>
                          fMeasureElementsVector;

    // voice finalization
    Bool                  fMeasureHasBeenFinalized;
};
typedef SMARTP<msdrMeasure> S_msdrMeasure;
EXP std::ostream& operator << (std::ostream& os, const S_msdrMeasure& elt);


}


#endif
