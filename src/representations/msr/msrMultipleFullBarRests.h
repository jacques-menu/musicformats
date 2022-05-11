#ifndef ___msrMultipleFullBarRests___
#define ___msrMultipleFullBarRests___

#include "msrElements.h"
#include "msrMeasures.h"
#include "msrSegmentElements.h"
#include "msrVoices.h"


namespace MusicFormats
{

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
                            const rational& multipleFullBarRestsMeasureSoundingNotes,
                            int             multipleFullBarRestsNumber,
                            S_msrVoice      voiceUpLink);

    static SMARTP<msrMultipleFullBarRests> create (
                            int          inputLineNumber,
                            S_msrMeasure restMeasureClone,
                            S_msrVoice   voiceUpLink);

    SMARTP<msrMultipleFullBarRests> createMultipleFullBarRestsNewbornClone (
                              S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMultipleFullBarRests (
                            int             inputLineNumber,
                            const rational& multipleFullBarRestsMeasureSoundingNotes,
                            int             multipleFullBarRestsNumber,
                            S_msrVoice      voiceUpLink);

                          msrMultipleFullBarRests (
                            int          inputLineNumber,
                            S_msrMeasure restMeasureClone,
                            S_msrVoice   voiceUpLink);

    virtual               ~msrMultipleFullBarRests ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrVoice            getMultipleFullBarRestsVoiceUpLink () const
                            { return fMultipleFullBarRestsVoiceUpLink; }

    int                   getMultipleFullBarRestsNumber () const
                              { return fMultipleFullBarRestsNumber; }

//     void                  setMultipleFullBarRestsContents (
//                             S_msrMultipleFullBarRestsContents
//                               multipleFullBarRestsContents);

//     S_msrMultipleFullBarRestsContents
//                           getMultipleFullBarRestsContents () const
//                               { return fMultipleFullBarRestsContents; }

    void                  setMultipleFullBarRestsNextMeasureNumber (
                            const string& nextMeasureNumber);

    string                getMultipleFullBarRestsNextMeasureNumber () const
                              { return fMultipleFullBarRestsNextMeasureNumber; }

    void                  setMultipleFullBarRestsLastMeasurePuristMeasureNumber (
                            int inputLineNumber);

    int                   getMultipleFullBarRestsLastMeasurePuristMeasureNumber () const
                              { return fMultipleFullBarRestsLastMeasurePuristNumber; }

    rational              getMultipleFullBarRestsMeasureSoundingNotes () const
                              { return fMultipleFullBarRestsMeasureSoundingNotes; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendSegmentElement (
                            S_msrSegmentElement elem) override;

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

  private:

    // private fields
    // ------------------------------------------------------

    S_msrVoice            fMultipleFullBarRestsVoiceUpLink;

    int                   fMultipleFullBarRestsNumber; // supplied by MusicXML
    list<S_msrMeasure>    fFullBarRestsMeasuresList;

//     S_msrMultipleFullBarRestsContents
//                           fMultipleFullBarRestsContents;

    string                fMultipleFullBarRestsNextMeasureNumber;
    int                   fMultipleFullBarRestsLastMeasurePuristNumber;

    // shortcut for efficiency JMI ???
    rational              fMultipleFullBarRestsMeasureSoundingNotes;
};
typedef SMARTP<msrMultipleFullBarRests> S_msrMultipleFullBarRests;
EXP ostream& operator<< (ostream& os, const S_msrMultipleFullBarRests& elt);


}


#endif
