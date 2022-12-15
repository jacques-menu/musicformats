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
                            int           inputLineNumber,
                            const std::string& musicName);

    SMARTP<msdrMusic> createMusicNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdrMusic (
                            int           inputLineNumber,
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
    int                   getInputLineNumber () const
                              { return fInputLineNumber; }

    // music measures std::vector
    const std::vector<S_msdrMeasure>&
                          getMusicMeasuresVector () const
                              { return fMusicMeasuresVector; }
    // measures std::list
    const std::list<S_msdrMeasure>&
                          getMusicMeasuresList () const
                              { return fMusicMeasuresList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addMeasureToMusic (
                            int           inputLineNumber,
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
                            int                inputLineNumber,
                            const std::string& context) const;

    void                  print (std::ostream& os) const;
    void                  printFull (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // music name
    std::string           fMusicName;

    // input line number
    int                   fInputLineNumber;

    // measures std::vector
    std::vector<S_msdrMeasure> fMusicMeasuresVector;

  public:

    // public work services
    // ------------------------------------------------------

  private:

    // work fields
    // ------------------------------------------------------

    // measures std::list
    std::list<S_msdrMeasure>   fMusicMeasuresList; // JMI

    // voice finalization
    Bool                  fMusicHasBeenFinalized;
};
typedef SMARTP<msdrMusic> S_msdrMusic;
EXP std::ostream& operator << (std::ostream& os, const S_msdrMusic& elt);


}


#endif
