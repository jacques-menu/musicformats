#ifndef ___msrBeatRepeats___
#define ___msrBeatRepeats___

#include "msrElements.h"
#include "msrSegmentElements.h"

#include "msrSegments.h"
#include "msrVoices.h"


namespace MusicFormats
{

//______________________________________________________________________________
// PRE-declarations for classes mutual dependencies
class   msrBeatRepeat;
typedef SMARTP<msrBeatRepeat> S_msrBeatRepeat;

class   msrVoice;
typedef SMARTP<msrVoice> S_msrVoice;

class   msrSegment;
typedef SMARTP<msrSegment> S_msrSegment;

//______________________________________________________________________________
class EXP msrBeatRepeatPattern : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBeatRepeatPattern> create (
                            int              inputLineNumber,
                            S_msrBeatRepeat upLinkToBeatRepeat);

    SMARTP<msrBeatRepeatPattern> createBeatRepeatPatternNewbornClone (
                            S_msrVoice containingVoice);

    SMARTP<msrBeatRepeatPattern> createBeatRepeatPatternDeepClone (
                            S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrBeatRepeatPattern (
                            int              inputLineNumber,
                            S_msrBeatRepeat upLinkToBeatRepeat);

    virtual               ~msrBeatRepeatPattern ();

  public:

    // set and get
    // ------------------------------------------------------

    // segment
    void                  setBeatRepeatPatternSegment (
                            S_msrSegment beatRepeatPatternSegment);

    S_msrSegment          getBeatRepeatPatternSegment () const
                              { return fBeatRepeatPatternSegment; }

    // upLinks
    S_msrBeatRepeat       getUpLinkToBeatRepeat () const
                            { return fUpLinkToBeatRepeat; }

  public:

    // public services
    // ------------------------------------------------------

    int                   fetchMeasuresNumber () const;

    void                  collectRepeatPatternMeasuresIntoFlatList (
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

    // upLinks
    S_msrBeatRepeat       fUpLinkToBeatRepeat;

    // segment
    S_msrSegment          fBeatRepeatPatternSegment;
};
typedef SMARTP<msrBeatRepeatPattern> S_msrBeatRepeatPattern;
EXP ostream& operator << (ostream& os, const S_msrBeatRepeatPattern& elt);

//______________________________________________________________________________
class EXP msrBeatRepeatReplicas : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBeatRepeatReplicas> create (
                            int              inputLineNumber,
                            S_msrBeatRepeat upLinkToBeatRepeat);

    SMARTP<msrBeatRepeatReplicas> createBeatRepeatReplicasNewbornClone (
                            S_msrVoice containingVoice);

    SMARTP<msrBeatRepeatReplicas> createBeatRepeatReplicasDeepClone (
                            S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrBeatRepeatReplicas (
                            int              inputLineNumber,
                            S_msrBeatRepeat upLinkToBeatRepeat);

    virtual               ~msrBeatRepeatReplicas ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrBeatRepeat       getUpLinkToBeatRepeat () const
                            { return fUpLinkToBeatRepeat; }
    // segment
    void                  setBeatRepeatReplicasSegment (
                            S_msrSegment beatRepeatReplicasSegment);

    S_msrSegment          getBeatRepeatReplicasSegment () const
                              { return fBeatRepeatReplicasSegment; }

  public:

    // public services
    // ------------------------------------------------------

    int                   fetchMeasuresNumber () const;

    int                   fetchBeatRepeatReplicasNumber () const;

    void                  collectRepeatReplicasMeasuresIntoFlatList (
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

    // upLinks
    S_msrBeatRepeat       fUpLinkToBeatRepeat;

    // segment
    S_msrSegment          fBeatRepeatReplicasSegment;
};
typedef SMARTP<msrBeatRepeatReplicas> S_msrBeatRepeatReplicas;
EXP ostream& operator << (ostream& os, const S_msrBeatRepeatReplicas& elt);

//______________________________________________________________________________
// data types

enum class msrBeatRepeatKind {
  kBeatRepeat_NO_,
  kBeatRepeatStart, kBeatRepeatStop
};

string msrBeatRepeatKindAsString (
  msrBeatRepeatKind beatRepeatKind);

ostream& operator << (ostream& os, const msrBeatRepeatKind& elt);

enum class msrBeatRepeatBuildPhaseKind {
  kBeatRepeatBuildPhaseJustCreated,
  kBeatRepeatBuildPhaseInPattern,
  kBeatRepeatBuildPhaseInReplicas,
  kBeatRepeatBuildPhaseCompleted
};

string msrBeatRepeatBuildPhaseKindAsString (
  msrBeatRepeatBuildPhaseKind beatRepeatBuildPhaseKind);

ostream& operator << (ostream& os, const msrBeatRepeatBuildPhaseKind& elt);

class EXP msrBeatRepeat : public msrSegmentElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBeatRepeat> create (
                            int        inputLineNumber,
                            int        beatRepeatMeasuresNumber,
                            int        beatRepeatSlashesNumber,
                            S_msrVoice upLinkToVoice);

    SMARTP<msrBeatRepeat> createBeatRepeatNewbornClone (
                            S_msrVoice containingVoice);

    SMARTP<msrBeatRepeat> createBeatRepeatDeepClone (
                            S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrBeatRepeat (
                            int        inputLineNumber,
                            int        beatRepeatMeasuresNumber,
                            int        beatRepeatSlashesNumber,
                            S_msrVoice upLinkToVoice);

    virtual               ~msrBeatRepeat ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            getUpLinkToBeatRepeatToVoice () const
                            { return fUpLinkToBeatRepeatToVoice; }

    // numbers
    int                   getBeatRepeatMeasuresNumber () const
                              { return fBeatRepeatMeasuresNumber; }

    int                   getBeatRepeatSlashesNumber () const
                              { return fBeatRepeatSlashesNumber; }

    // beatRepeat pattern
    void                  setBeatRepeatPattern (
                            S_msrBeatRepeatPattern
                              beatRepeatPattern);

    S_msrBeatRepeatPattern
                          getBeatRepeatPattern () const
                              { return fBeatRepeatPattern; }

    // measures repeat replicas
    void                  setBeatRepeatReplicas (
                            S_msrBeatRepeatReplicas
                              beatRepeatReplicas);

    S_msrBeatRepeatReplicas
                          getBeatRepeatReplicas () const
                              { return fBeatRepeatReplicas; }

    // measures repeat build phase
    void                  setCurrentBeatRepeatBuildPhaseKind (
                            msrBeatRepeatBuildPhaseKind
                              beatRepeatBuildPhaseKind)
                              {
                                fCurrentBeatRepeatBuildPhaseKind =
                                  beatRepeatBuildPhaseKind;
                              }

    msrBeatRepeatBuildPhaseKind
                          getCurrentBeatRepeatBuildPhaseKind () const
                            { return fCurrentBeatRepeatBuildPhaseKind; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendMeasureElementToSegmentElement (
                            S_msrMeasureElement elem) override;

    int                   fetchBeatRepeatReplicasNumber () const;

    int                   fetchBeatRepeatPatternMeasuresNumber () const;
    int                   fetchBeatRepeatReplicasMeasuresNumber () const;

    void                  collectBeatRepeatMeasuresIntoFlatList (
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

    void                  displayBeatRepeat (
                            int           inputLineNumber,
                            const string& context);

    void                  print (ostream& os) const override;
    void                  printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fUpLinkToBeatRepeatToVoice;

    // numbers
    int                   fBeatRepeatMeasuresNumber;
    int                   fBeatRepeatSlashesNumber;

    // measures repeat pattern
    S_msrBeatRepeatPattern
                          fBeatRepeatPattern;

    // measures repeat replicas
    S_msrBeatRepeatReplicas
                          fBeatRepeatReplicas;

    // measures repeat build phase, used when building the measures repeat
    msrBeatRepeatBuildPhaseKind
                          fCurrentBeatRepeatBuildPhaseKind; // unused??? JMI
};
typedef SMARTP<msrBeatRepeat> S_msrBeatRepeat;
EXP ostream& operator << (ostream& os, const S_msrBeatRepeat& elt);


}


#endif
