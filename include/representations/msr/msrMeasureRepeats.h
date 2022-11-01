#ifndef ___msrMeasureRepeats___
#define ___msrMeasureRepeats___

#include "msrElements.h"
#include "msrSegments.h"
#include "msrVoices.h"
#include "msrSegmentElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
// PRE-declarations for classes mutual dependencies
class msrMeasureRepeat;
typedef SMARTP<msrMeasureRepeat> S_msrMeasureRepeat;

class msrVoice;
typedef SMARTP<msrVoice> S_msrVoice;

class msrSegment;
typedef SMARTP<msrSegment> S_msrSegment;

//______________________________________________________________________________
class EXP msrMeasureRepeatPattern : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasureRepeatPattern> create (
                            int                 inputLineNumber,
                            S_msrMeasureRepeat upLinkToMeasureRepeat);

    SMARTP<msrMeasureRepeatPattern> createMeasureRepeatPatternNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrMeasureRepeatPattern> createMeasureRepeatPatternDeepClone (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMeasureRepeatPattern (
                            int                 inputLineNumber,
                            S_msrMeasureRepeat upLinkToMeasureRepeat);

    virtual               ~msrMeasureRepeatPattern ();

  public:

    // set and get
    // ------------------------------------------------------

    // segment
    void                  setMeasureRepeatPatternSegment (
                            S_msrSegment measureRepeatPatternSegment);

    S_msrSegment          getMeasureRepeatPatternSegment () const
                              { return fMeasureRepeatPatternSegment; }

    // upLinks
    S_msrMeasureRepeat    getUpLinkToMeasureRepeat () const
                            { return fUpLinkToMeasureRepeat; }

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
    S_msrMeasureRepeat    fUpLinkToMeasureRepeat;

    // segment
    S_msrSegment          fMeasureRepeatPatternSegment;
};
typedef SMARTP<msrMeasureRepeatPattern> S_msrMeasureRepeatPattern;
EXP ostream& operator << (ostream& os, const S_msrMeasureRepeatPattern& elt);

//______________________________________________________________________________
class EXP msrMeasureRepeatReplicas : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasureRepeatReplicas> create (
                            int                 inputLineNumber,
                            S_msrMeasureRepeat upLinkToMeasureRepeat);

    SMARTP<msrMeasureRepeatReplicas> createMeasureRepeatReplicasNewbornClone (
                            S_msrVoice containingVoice);

    SMARTP<msrMeasureRepeatReplicas> createMeasureRepeatReplicasDeepClone (
                            S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMeasureRepeatReplicas (
                            int                 inputLineNumber,
                            S_msrMeasureRepeat upLinkToMeasureRepeat);

    virtual               ~msrMeasureRepeatReplicas ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrMeasureRepeat    getUpLinkToMeasureRepeat () const
                            { return fUpLinkToMeasureRepeat; }
    // segment
    void                  setMeasureRepeatReplicasSegment (
                            S_msrSegment measureRepeatReplicasSegment);

    S_msrSegment          getMeasureRepeatReplicasSegment () const
                              { return fMeasureRepeatReplicasSegment; }

  public:

    // public services
    // ------------------------------------------------------

    int                   fetchMeasuresNumber () const;

    int                   fetchMeasureRepeatReplicasNumber () const;

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
    S_msrMeasureRepeat    fUpLinkToMeasureRepeat;

    // segment
    S_msrSegment          fMeasureRepeatReplicasSegment;
};
typedef SMARTP<msrMeasureRepeatReplicas> S_msrMeasureRepeatReplicas;
EXP ostream& operator << (ostream& os, const S_msrMeasureRepeatReplicas& elt);

//______________________________________________________________________________
class EXP msrMeasureRepeat : public msrSegmentElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrMeasureRepeatKind {
      k_NoMeasureRepeat,
      kStartMeasureRepeat, kStopMeasureRepeat
    };

    static string measureRepeatKindAsString (
      msrMeasureRepeatKind measureRepeatKind);

    enum msrMeasureRepeatBuildPhaseKind {
      kMeasureRepeatBuildPhaseJustCreated,
      kMeasureRepeatBuildPhaseInPattern,
      kMeasureRepeatBuildPhaseInReplicas,
      kMeasureRepeatBuildPhaseCompleted
    };

    static string measureRepeatBuildPhaseKindAsString (
      msrMeasureRepeatBuildPhaseKind measureRepeatBuildPhaseKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasureRepeat> create (
                            int        inputLineNumber,
                            int        measureRepeatMeasuresNumber,
                            int        measureRepeatSlashesNumber,
                            S_msrVoice upLinkToVoice);

    SMARTP<msrMeasureRepeat> createMeasureRepeatNewbornClone (
                            S_msrVoice containingVoice);

    SMARTP<msrMeasureRepeat> createMeasureRepeatDeepClone (
                            S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMeasureRepeat (
                            int        inputLineNumber,
                            int        measureRepeatMeasuresNumber,
                            int        measureRepeatSlashesNumber,
                            S_msrVoice upLinkToVoice);

    virtual               ~msrMeasureRepeat ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            getUpLinkToMeasureRepeatToVoice () const
                            { return fUpLinkToMeasureRepeatToVoice; }

    // numbers
    int                   getMeasureRepeatMeasuresNumber () const
                              { return fMeasureRepeatMeasuresNumber; }

    int                   getMeasureRepeatSlashesNumber () const
                              { return fMeasureRepeatSlashesNumber; }

    // measureRepeat pattern
    void                  setMeasureRepeatPattern (
                            S_msrMeasureRepeatPattern
                              measureRepeatPattern);

    S_msrMeasureRepeatPattern
                          getMeasureRepeatPattern () const
                              { return fMeasureRepeatPattern; }

    // measures repeat replicas
    void                  setMeasureRepeatReplicas (
                            S_msrMeasureRepeatReplicas
                              measureRepeatReplicas);

    S_msrMeasureRepeatReplicas
                          getMeasureRepeatReplicas () const
                              { return fMeasureRepeatReplicas; }

    // measures repeat build phase
    void                  setCurrentMeasureRepeatBuildPhaseKind (
                            msrMeasureRepeatBuildPhaseKind
                              measureRepeatBuildPhaseKind)
                              {
                                fCurrentMeasureRepeatBuildPhaseKind =
                                  measureRepeatBuildPhaseKind;
                              }

    msrMeasureRepeatBuildPhaseKind
                          getCurrentMeasureRepeatBuildPhaseKind () const
                            { return fCurrentMeasureRepeatBuildPhaseKind; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendMeasureElementToSegmentElement (
                            S_msrMeasureElement elem) override;

    int                   fetchMeasureRepeatReplicasNumber () const;

    int                   fetchMeasureRepeatPatternMeasuresNumber () const;
    int                   fetchMeasureRepeatReplicasMeasuresNumber () const;

    void                  collectMeasureRepeatMeasuresIntoFlatList (
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

    void                  displayMeasureRepeat (
                            int           inputLineNumber,
                            const string& context);

    void                  print (ostream& os) const override;
    void                  printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fUpLinkToMeasureRepeatToVoice;

    // numbers
    int                   fMeasureRepeatMeasuresNumber;
    int                   fMeasureRepeatSlashesNumber;

    // measures repeat pattern
    S_msrMeasureRepeatPattern
                          fMeasureRepeatPattern;

    // measures repeat replicas
    S_msrMeasureRepeatReplicas
                          fMeasureRepeatReplicas;

    // measures repeat build phase, used when building the measures repeat
    msrMeasureRepeatBuildPhaseKind
                          fCurrentMeasureRepeatBuildPhaseKind;
};
typedef SMARTP<msrMeasureRepeat> S_msrMeasureRepeat;
EXP ostream& operator << (ostream& os, const S_msrMeasureRepeat& elt);


}


#endif


//______________________________________________________________________________
/* JMI
class EXP msrMeasureRepeatElement : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasureRepeatElement> create (
                            int                 inputLineNumber,
                            S_msrMeasureRepeat upLinkToMeasureRepeat);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMeasureRepeatElement (
                            int                 inputLineNumber,
                            S_msrMeasureRepeat upLinkToMeasureRepeat);

    virtual               ~msrMeasureRepeatElement ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrMeasureRepeat   getMeasureRepeatElementUpLinkToMeasureRepeat () const
                              { return fMeasureRepeatElementUpLinkToMeasureRepeat; }

    // elements
    const list<S_msrVoiceElement>&
                          getMeasureRepeatElementsList () const
                              { return fMeasureRepeatElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendSegmentToMeasureRepeatElementsList (
                            int          inputLineNumber,
                            S_msrSegment segment,
                            const string&       context);

    void                  appendMeasureRepeatToMeasureRepeatElementsList (
                            int                  inputLineNumber,
                            S_msrMeasureRepeat  measureRepeat,
                            const string&               context);


    void                  appendMultipleFullBarRestsToMeasureRepeatElementsList (
                            int                         inputLineNumber,
                            S_msrMeasureRepeat measureRepeat,
                            const string&                      context);

    S_msrNote             fetchMeasureRepeatElementFirstNonGraceNote () const;

    void                  collectMeasureRepeatElementMeasuresIntoFlatList (
                            int inputLineNumber);

  private:

    // private services
    // ------------------------------------------------------

    void                  appendVoiceElementToMeasureRepeatElementsList ( // JMI
                            int               inputLineNumber,
                            S_msrVoiceElement voiceElement,
                            const string&            context);

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

    void                  printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrMeasureRepeat   fMeasureRepeatElementUpLinkToMeasureRepeat;

    // elements list
    list<S_msrVoiceElement>
                          fMeasureRepeatElementsList;
};
typedef SMARTP<msrMeasureRepeatElement> S_msrMeasureRepeatElement;
EXP ostream& operator << (ostream& os, const S_msrMeasureRepeatElement& elt);
*/

