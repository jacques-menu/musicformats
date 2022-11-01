#ifndef ___msrMultipleFullBarRests___
#define ___msrMultipleFullBarRests___

#include "msrElements.h"
#include "msrMeasures.h"
#include "msrSegmentElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class msrSegment;
typedef SMARTP<msrSegment> S_msrSegment;

//______________________________________________________________________________
class EXP msrMultipleFullBarRests : public msrSegmentElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrMultipleFullBarRestsKind { // unused ??? JMI
      k_NoMultipleFullBarRests,
      kStartMultipleFullBarRests, kStopMultipleFullBarRests
    };

    static string multipleFullBarRestsKindAsString (
      msrMultipleFullBarRestsKind multipleFullBarRestsKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMultipleFullBarRests> create (
                            int             inputLineNumber,
                            int             multipleFullBarRestsNumber,
                            S_msrSegment    upLinkToSegment);

    static SMARTP<msrMultipleFullBarRests> create (
                            int          inputLineNumber,
                            S_msrMeasure restMeasureClone,
                            S_msrSegment upLinkToSegment);

    SMARTP<msrMultipleFullBarRests> createMultipleFullBarRestsNewbornClone (
                              S_msrSegment containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMultipleFullBarRests (
                            int             inputLineNumber,
                            int             multipleFullBarRestsNumber,
                            S_msrSegment    upLinkToSegment);

                          msrMultipleFullBarRests (
                            int          inputLineNumber,
                            S_msrMeasure restMeasureClone,
                            S_msrSegment upLinkToSegment);

    virtual               ~msrMultipleFullBarRests ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrSegment          getMultipleFullBarRestsUpLinkToSegment () const
                            { return fMultipleFullBarRestsUpLinkToSegment; }

    int                   getMultipleFullBarRestsNumber () const
                              { return fMultipleFullBarRestsNumber; }

    const list<S_msrMeasure>&
                          getFullBarRestsMeasuresList () const
                              { return fFullBarRestsMeasuresList; }

    void                  setMultipleFullBarRestsNextMeasureNumber (
                            const string& nextMeasureNumber);

    string                getMultipleFullBarRestsNextMeasureNumber () const
                              { return fMultipleFullBarRestsNextMeasureNumber; }

    void                  setMultipleFullBarRestsLastMeasurePuristMeasureNumber (
                            int inputLineNumber);

    int                   getMultipleFullBarRestsLastMeasurePuristMeasureNumber () const
                              { return fMultipleFullBarRestsLastMeasurePuristNumber; }

    Rational              fetchMultipleFullBarRestsMeasureSoundingNotes () const;
                            // don't simply multiply duration by number,
                            // may help pin-point errors in representation
  public:

    // public services
    // ------------------------------------------------------

    void                  appendMeasureElementToSegmentElement (
                            S_msrMeasureElement elem) override;

    void                  appendMeasureToMultipleFullBarRests (
                            S_msrMeasure measure);

    int                   multipleFullBarRestsContentsMeasuresNumber () const
                            {
                              return 0; // JMI
//                                 fMultipleFullBarRestsContents->
//                                   multipleFullBarRestsContentsMeasuresNumber ();
                            }

    void                  collectMultipleFullBarRestsIntoFlatList (
                            int inputLineNumber);

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

    void                  displayMultipleFullBarRests (
                            int           inputLineNumber,
                            const string& context);

    void                  print (ostream& os) const override;
    void                  printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrSegment          fMultipleFullBarRestsUpLinkToSegment;

    int                   fMultipleFullBarRestsNumber; // supplied by MusicXML
    list<S_msrMeasure>    fFullBarRestsMeasuresList;

    int                   fMultipleFullBarRestsLastMeasurePuristNumber;

    string                fMultipleFullBarRestsNextMeasureNumber;
};
typedef SMARTP<msrMultipleFullBarRests> S_msrMultipleFullBarRests;
EXP ostream& operator << (ostream& os, const S_msrMultipleFullBarRests& elt);


}


#endif
