#ifndef ___msrBeatRepeats___
#define ___msrBeatRepeats___

#include "msrElements.h"
#include "msrSegments.h"
#include "msrVoices.h"
#include "msrSegmentElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
// pre-declaration
class msrBeatRepeat;
typedef SMARTP<msrBeatRepeat> S_msrBeatRepeat;

class msrVoice;
typedef SMARTP<msrVoice> S_msrVoice;

class msrSegment;
typedef SMARTP<msrSegment> S_msrSegment;

//______________________________________________________________________________
class EXP msrBeatRepeatPattern : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBeatRepeatPattern> create (
                            int              inputLineNumber,
                            S_msrBeatRepeat beatRepeatUpLink);

    SMARTP<msrBeatRepeatPattern> createBeatRepeatPatternNewbornClone (
                            S_msrVoice containingVoice);

    SMARTP<msrBeatRepeatPattern> createBeatRepeatPatternDeepClone (
                            S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrBeatRepeatPattern (
                            int              inputLineNumber,
                            S_msrBeatRepeat beatRepeatUpLink);

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
    S_msrBeatRepeat      getBeatRepeatUpLink () const
                            { return fBeatRepeatUpLink; }

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
    S_msrBeatRepeat      fBeatRepeatUpLink;

    // segment
    S_msrSegment          fBeatRepeatPatternSegment;
};
typedef SMARTP<msrBeatRepeatPattern> S_msrBeatRepeatPattern;
EXP ostream& operator<< (ostream& os, const S_msrBeatRepeatPattern& elt);

//______________________________________________________________________________
class EXP msrBeatRepeatReplicas : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBeatRepeatReplicas> create (
                            int              inputLineNumber,
                            S_msrBeatRepeat beatRepeatUpLink);

    SMARTP<msrBeatRepeatReplicas> createBeatRepeatReplicasNewbornClone (
                            S_msrVoice containingVoice);

    SMARTP<msrBeatRepeatReplicas> createBeatRepeatReplicasDeepClone (
                            S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrBeatRepeatReplicas (
                            int              inputLineNumber,
                            S_msrBeatRepeat beatRepeatUpLink);

    virtual               ~msrBeatRepeatReplicas ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrBeatRepeat      getBeatRepeatUpLink () const
                            { return fBeatRepeatUpLink; }
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
    S_msrBeatRepeat      fBeatRepeatUpLink;

    // segment
    S_msrSegment          fBeatRepeatReplicasSegment;
};
typedef SMARTP<msrBeatRepeatReplicas> S_msrBeatRepeatReplicas;
EXP ostream& operator<< (ostream& os, const S_msrBeatRepeatReplicas& elt);

//______________________________________________________________________________
class EXP msrBeatRepeat : public msrSegmentElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrBeatRepeatKind {
      k_NoBeatRepeat,
      kStartBeatRepeat, kStopBeatRepeat
    };

    static string beatRepeatKindAsString (
      msrBeatRepeatKind beatRepeatKind);

    enum msrBeatRepeatBuildPhaseKind {
      kBeatRepeatBuildPhaseJustCreated,
      kBeatRepeatBuildPhaseInPattern,
      kBeatRepeatBuildPhaseInReplicas,
      kBeatRepeatBuildPhaseCompleted
    };

    static string beatRepeatBuildPhaseKindAsString (
      msrBeatRepeatBuildPhaseKind beatRepeatBuildPhaseKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBeatRepeat> create (
                            int        inputLineNumber,
                            int        beatRepeatMeasuresNumber,
                            int        beatRepeatSlashesNumber,
                            S_msrVoice voiceUpLink);

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
                            S_msrVoice voiceUpLink);

    virtual               ~msrBeatRepeat ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            getBeatRepeatVoiceUpLink () const
                            { return fBeatRepeatVoiceUpLink; }

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

    void                  appendSegmentElement (
                            S_msrSegmentElement elem) override;

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

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fBeatRepeatVoiceUpLink;

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
EXP ostream& operator<< (ostream& os, const S_msrBeatRepeat& elt);


}


#endif
