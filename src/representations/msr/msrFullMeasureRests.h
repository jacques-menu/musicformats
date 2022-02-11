#ifndef ___msrFullMeasureRests___
#define ___msrFullMeasureRests___

#include "msrElements.h"
#include "msrMeasures.h"
#include "msrSegments.h"
#include "msrVoices.h"
#include "msrVoiceElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
// pre-declaration
class msrFullMeasureRests;
typedef SMARTP<msrFullMeasureRests> S_msrFullMeasureRests;

class msrVoice;
typedef SMARTP<msrVoice> S_msrVoice;

class msrSegment;
typedef SMARTP<msrSegment> S_msrSegment;

class msrMeasure;
typedef SMARTP<msrMeasure> S_msrMeasure;

//______________________________________________________________________________
class EXP msrFullMeasureRestsContents : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFullMeasureRestsContents> create (
                            int               inputLineNumber,
                            S_msrFullMeasureRests fullMeasureRests);

    SMARTP<msrFullMeasureRestsContents> createFullMeasureRestsContentsNewbornClone (
                            S_msrFullMeasureRests fullMeasureRests);

    SMARTP<msrFullMeasureRestsContents> createFullMeasureRestsContentsDeepClone (
                            S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrFullMeasureRestsContents (
                            int               inputLineNumber,
                            S_msrFullMeasureRests fullMeasureRests);

    virtual               ~msrFullMeasureRestsContents ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLink
    S_msrFullMeasureRests
                          getFullMeasureRestsContentsFullMeasureRestsUpLink () const
                            {
                              return fFullMeasureRestsContentsFullMeasureRestsUpLink;
                            }

    void                  setFullMeasureRestsContentsSegment (
                            int          inputLineNumber,
                            S_msrSegment fullMeasureRestsContentsSegment);

    S_msrSegment          getFullMeasureRestsContentsSegment () const
                              { return fFullMeasureRestsContentsSegment; }

  public:

    // public services
    // ------------------------------------------------------

    int                   fullMeasureRestsContentsMeasuresNumber () const;

    void                  collectFullMeasureRestsContentsMeasuresIntoFlatList (
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

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLink
    S_msrFullMeasureRests fFullMeasureRestsContentsFullMeasureRestsUpLink;

    S_msrSegment          fFullMeasureRestsContentsSegment;
};
typedef SMARTP<msrFullMeasureRestsContents> S_msrFullMeasureRestsContents;
EXP ostream& operator<< (ostream& os, const S_msrFullMeasureRestsContents& elt);

//______________________________________________________________________________
class EXP msrFullMeasureRests : public msrVoiceElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrFullMeasureRestsKind { // unused ??? JMI
      k_NoFullMeasureRests,
      kStartFullMeasureRests, kStopFullMeasureRests
    };

    static string fullMeasureRestsKindAsString (
      msrFullMeasureRestsKind fullMeasureRestsKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFullMeasureRests> create (
                            int             inputLineNumber,
                            const rational& fullMeasureRestsMeasureSoundingNotes,
                            int             fullMeasureRestsNumber,
                            S_msrVoice      voiceUpLink);

    static SMARTP<msrFullMeasureRests> create (
                            int          inputLineNumber,
                            S_msrMeasure restMeasureClone,
                            S_msrVoice   voiceUpLink);

    SMARTP<msrFullMeasureRests> createFullMeasureRestsNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrFullMeasureRests> createFullMeasureRestsDeepClone (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrFullMeasureRests (
                            int             inputLineNumber,
                            const rational& fullMeasureRestsMeasureSoundingNotes,
                            int             fullMeasureRestsNumber,
                            S_msrVoice      voiceUpLink);

                          msrFullMeasureRests (
                            int          inputLineNumber,
                            S_msrMeasure restMeasureClone,
                            S_msrVoice   voiceUpLink);

    virtual               ~msrFullMeasureRests ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrVoice            getFullMeasureRestsVoiceUpLink () const
                            { return fFullMeasureRestsVoiceUpLink; }

    int                   getFullMeasureRestsNumber () const
                              { return fFullMeasureRestsNumber; }

    void                  setFullMeasureRestsContents (
                            S_msrFullMeasureRestsContents
                              fullMeasureRestsContents);

    S_msrFullMeasureRestsContents
                          getFullMeasureRestsContents () const
                              { return fFullMeasureRestsContents; }

    void                  setFullMeasureRestsNextMeasureNumber (
                            const string& nextMeasureNumber);

    string                getFullMeasureRestsNextMeasureNumber () const
                              { return fFullMeasureRestsNextMeasureNumber; }

    void                  setFullMeasureRestsLastMeasurePuristMeasureNumber (
                            int inputLineNumber);

    int                   getFullMeasureRestsLastMeasurePuristMeasureNumber () const
                              { return fFullMeasureRestsLastMeasurePuristNumber; }

    rational              getFullMeasureRestsMeasureSoundingNotes () const
                              { return fFullMeasureRestsMeasureSoundingNotes; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendMeasureCloneToFullMeasureRests (
                            S_msrMeasure measureClone);

    int                   fullMeasureRestsContentsMeasuresNumber () const
                            {
                              return
                                fFullMeasureRestsContents->
                                  fullMeasureRestsContentsMeasuresNumber ();
                            }

    void                  collectFullMeasureRestsIntoFlatList (
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

    void                  displayFullMeasureRests (
                            int           inputLineNumber,
                            const string& context);

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrVoice            fFullMeasureRestsVoiceUpLink;

    int                   fFullMeasureRestsNumber;

    S_msrFullMeasureRestsContents
                          fFullMeasureRestsContents;

    string                fFullMeasureRestsNextMeasureNumber;
    int                   fFullMeasureRestsLastMeasurePuristNumber;

    // shortcut for efficiency
    rational              fFullMeasureRestsMeasureSoundingNotes;
};
typedef SMARTP<msrFullMeasureRests> S_msrFullMeasureRests;
EXP ostream& operator<< (ostream& os, const S_msrFullMeasureRests& elt);


}


#endif
