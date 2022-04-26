#ifndef ___msrFullBarRests___
#define ___msrFullBarRests___

#include "msrElements.h"
#include "msrMeasures.h"
#include "msrSegments.h"
#include "msrVoices.h"
#include "msrVoiceElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
// pre-declaration
class msrFullBarRests;
typedef SMARTP<msrFullBarRests> S_msrFullBarRests;

class msrVoice;
typedef SMARTP<msrVoice> S_msrVoice;

class msrSegment;
typedef SMARTP<msrSegment> S_msrSegment;

class msrMeasure;
typedef SMARTP<msrMeasure> S_msrMeasure;

//______________________________________________________________________________
class EXP msrFullBarRestsContents : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFullBarRestsContents> create (
                            int                   inputLineNumber,
                            S_msrFullBarRests fullBarRests);

    SMARTP<msrFullBarRestsContents> createFullBarRestsContentsNewbornClone (
                            S_msrFullBarRests fullBarRests);

    SMARTP<msrFullBarRestsContents> createFullBarRestsContentsDeepClone (
                            S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrFullBarRestsContents (
                            int                   inputLineNumber,
                            S_msrFullBarRests fullBarRests);

    virtual               ~msrFullBarRestsContents ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLink
    S_msrFullBarRests
                          getFullBarRestsContentsFullBarRestsUpLink () const
                            {
                              return fFullBarRestsContentsFullBarRestsUpLink;
                            }

    void                  setFullBarRestsContentsSegment (
                            int          inputLineNumber,
                            S_msrSegment fullBarRestsContentsSegment);

    S_msrSegment          getFullBarRestsContentsSegment () const
                              { return fFullBarRestsContentsSegment; }

  public:

    // public services
    // ------------------------------------------------------

    int                   fullBarRestsContentsMeasuresNumber () const;

    void                  collectFullBarRestsContentsMeasuresIntoFlatList (
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
    S_msrFullBarRests fFullBarRestsContentsFullBarRestsUpLink;

    S_msrSegment          fFullBarRestsContentsSegment;
};
typedef SMARTP<msrFullBarRestsContents> S_msrFullBarRestsContents;
EXP ostream& operator<< (ostream& os, const S_msrFullBarRestsContents& elt);

//______________________________________________________________________________
class EXP msrFullBarRests : public msrVoiceElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrFullBarRestsKind { // unused ??? JMI
      k_NoFullBarRests,
      kStartFullBarRests, kStopFullBarRests
    };

    static string fullBarRestsKindAsString (
      msrFullBarRestsKind fullBarRestsKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFullBarRests> create (
                            int             inputLineNumber,
                            const rational& fullBarRestsMeasureSoundingNotes,
                            int             fullBarRestsNumber,
                            S_msrVoice      voiceUpLink);

    static SMARTP<msrFullBarRests> create (
                            int          inputLineNumber,
                            S_msrMeasure restMeasureClone,
                            S_msrVoice   voiceUpLink);

    SMARTP<msrFullBarRests> createFullBarRestsNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrFullBarRests> createFullBarRestsDeepClone (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrFullBarRests (
                            int             inputLineNumber,
                            const rational& fullBarRestsMeasureSoundingNotes,
                            int             fullBarRestsNumber,
                            S_msrVoice      voiceUpLink);

                          msrFullBarRests (
                            int          inputLineNumber,
                            S_msrMeasure restMeasureClone,
                            S_msrVoice   voiceUpLink);

    virtual               ~msrFullBarRests ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrVoice            getFullBarRestsVoiceUpLink () const
                            { return fFullBarRestsVoiceUpLink; }

    int                   getFullBarRestsNumber () const
                              { return fFullBarRestsNumber; }

    void                  setFullBarRestsContents (
                            S_msrFullBarRestsContents
                              fullBarRestsContents);

    S_msrFullBarRestsContents
                          getFullBarRestsContents () const
                              { return fFullBarRestsContents; }

    void                  setFullBarRestsNextMeasureNumber (
                            const string& nextMeasureNumber);

    string                getFullBarRestsNextMeasureNumber () const
                              { return fFullBarRestsNextMeasureNumber; }

    void                  setFullBarRestsLastMeasurePuristMeasureNumber (
                            int inputLineNumber);

    int                   getFullBarRestsLastMeasurePuristMeasureNumber () const
                              { return fFullBarRestsLastMeasurePuristNumber; }

    rational              getFullBarRestsMeasureSoundingNotes () const
                              { return fFullBarRestsMeasureSoundingNotes; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendMeasureCloneToFullBarRests (
                            S_msrMeasure measureClone);

    int                   fullBarRestsContentsMeasuresNumber () const
                            {
                              return
                                fFullBarRestsContents->
                                  fullBarRestsContentsMeasuresNumber ();
                            }

    void                  collectFullBarRestsIntoFlatList (
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

    void                  displayFullBarRests (
                            int           inputLineNumber,
                            const string& context);

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrVoice            fFullBarRestsVoiceUpLink;

    int                   fFullBarRestsNumber;

    S_msrFullBarRestsContents
                          fFullBarRestsContents;

    string                fFullBarRestsNextMeasureNumber;
    int                   fFullBarRestsLastMeasurePuristNumber;

    // shortcut for efficiency
    rational              fFullBarRestsMeasureSoundingNotes;
};
typedef SMARTP<msrFullBarRests> S_msrFullBarRests;
EXP ostream& operator<< (ostream& os, const S_msrFullBarRests& elt);


}


#endif
