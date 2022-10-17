#ifndef ___msrRepeats___
#define ___msrRepeats___

#include "msrElements.h"
#include "msrMeasureRepeats.h"
#include "msrNotes.h"
#include "msrMultipleFullBarRests.h"
#include "msrVoiceElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
// pre-declaration
class msrRepeat;
typedef SMARTP<msrRepeat> S_msrRepeat;

class msrMultipleFullBarRests;
typedef SMARTP<msrMultipleFullBarRests> S_msrMultipleFullBarRests;

class msrMeasureRepeat;
typedef SMARTP<msrMeasureRepeat> S_msrMeasureRepeat;

class msrNote;
typedef SMARTP<msrNote> S_msrNote;

//______________________________________________________________________________
class EXP msrRepeatCommonPart : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatCommonPart> create (
                            int         inputLineNumber,
                            S_msrRepeat upLinkToRepeat);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrRepeatCommonPart (
                            int         inputLineNumber,
                            S_msrRepeat upLinkToRepeat);

    virtual               ~msrRepeatCommonPart ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrRepeat           getRepeatCommonPartUpLinkToRepeat () const
                              { return fRepeatCommonPartUpLinkToRepeat; }

    // elements
    const list<S_msrVoiceElement>&
                          getRepeatCommonPartElementsList () const
                              { return fRepeatCommonPartElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendVoiceElementToRepeatCommonPart (
                            int               inputLineNumber,
                            S_msrVoiceElement voiceElement,
                            const string&     context);

    void                  appendSegmentToRepeatCommonPart (
                            int          inputLineNumber,
                            S_msrSegment segment,
                            const string&context);

    void                  appendRepeatToRepeatCommonPart (
                            int           inputLineNumber,
                            S_msrRepeat   repeat,
                            const string& context);

    void                  appendMeasureRepeatToRepeatCommonPart (
                            int                inputLineNumber,
                            S_msrMeasureRepeat measureRepeat,
                            const string&      context);

    void                  appendMultipleFullBarRestsToRepeatCommonPart (
                            int                   inputLineNumber,
                            S_msrMultipleFullBarRests multipleFullBarRests,
                            const string&         context);

    S_msrNote             fetchRepeatCommonPartFirstNonGraceNote () const;

    void                  collectRepeatCommonPartMeasuresIntoFlatList (
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

    void                  printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrRepeat           fRepeatCommonPartUpLinkToRepeat;

    // elements list
    list<S_msrVoiceElement>
                          fRepeatCommonPartElementsList;
};
typedef SMARTP<msrRepeatCommonPart> S_msrRepeatCommonPart;
EXP ostream& operator<< (ostream& os, const S_msrRepeatCommonPart& elt);

//______________________________________________________________________________
class EXP msrRepeatEnding : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatEnding> create (
                            int                 inputLineNumber,
                            const string&       repeatEndingNumber, // may be "1, 2"
                            msrRepeatEndingKind repeatEndingKind,
                            S_msrRepeat         upLinkToRepeat);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrRepeatEnding (
                            int                 inputLineNumber,
                            const string&       repeatEndingNumber, // may be "1, 2"
                            msrRepeatEndingKind repeatEndingKind,
                            S_msrRepeat         upLinkToRepeat);

    virtual               ~msrRepeatEnding ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrRepeat           getRepeatEndingUpLinkToRepeat () const
                              { return fRepeatEndingUpLinkToRepeat; }

    // numbers
    string                getRepeatEndingNumber () const
                              { return fRepeatEndingNumber; }

    void                  setRepeatEndingNumber (int repeatEndingNumber)
                              { fRepeatEndingNumber = repeatEndingNumber; }

    void                  setRepeatEndingInternalNumber (
                            int repeatEndingInternalNumber)
                              {
                                fRepeatEndingInternalNumber =
                                  repeatEndingInternalNumber;
                              }

    int                   getRepeatEndingInternalNumber () const
                              { return fRepeatEndingInternalNumber; }

    // kind
    msrRepeatEndingKind   getRepeatEndingKind () const
                              { return fRepeatEndingKind; }

    // elements
    const list<S_msrVoiceElement>&
                          getRepeatEndingElementsList () const
                              { return fRepeatEndingElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendVoiceElementToRepeatEnding ( // JMI
                            int               inputLineNumber,
                            S_msrVoiceElement voiceElement,
                            const string&     context);

    void                  appendSegmentToRepeatEnding (
                            int           inputLineNumber,
                            S_msrSegment  segment,
                            const string& context);

    void                  appendRepeatToRepeatEnding (
                            int           inputLineNumber,
                            S_msrRepeat   repeat,
                            const string& context);

    void                  appendMeasureRepeatToRepeatEnding (
                            int                inputLineNumber,
                            S_msrMeasureRepeat measureRepeat,
                            const string&      context);

    void                  appendMultipleFullBarRestsToRepeatEnding (
                            int                   inputLineNumber,
                            S_msrMultipleFullBarRests multipleFullBarRests,
                            const string&         context);

    void                  collectRepeatEndingMeasuresIntoFlatList (
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

    void                  printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrRepeat           fRepeatEndingUpLinkToRepeat;

    // numbers
    string                fRepeatEndingNumber; // may be "1, 2"
    int                   fRepeatEndingInternalNumber; // internally assigned

    // kind
    msrRepeatEndingKind   fRepeatEndingKind;

    // elements list
    list<S_msrVoiceElement>
                          fRepeatEndingElementsList;
};
typedef SMARTP<msrRepeatEnding> S_msrRepeatEnding;
EXP ostream& operator<< (ostream& os, const S_msrRepeatEnding& elt);

//______________________________________________________________________________
class EXP msrRepeat : public msrVoiceElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrRepeatExplicitStartKind {
      kRepeatExplicitStartNo,
      kRepeatExplicitStartYes
    };

    static string repeatExplicitStartKindAsString (
      msrRepeatExplicitStartKind repeatExplicitStartKind);

    enum msrRepeatBuildPhaseKind {
      kRepeatBuildPhaseJustCreated,
      kRepeatBuildPhaseInCommonPart,
      kRepeatBuildPhaseInEndings,
      kRepeatBuildPhaseCompleted
    };

    static string repeatBuildPhaseKindAsString (
      msrRepeatBuildPhaseKind repeatBuildPhaseKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeat> create (
                            int        inputLineNumber,
                            int        repeatTimes,
                            S_msrVoice upLinkToVoice);

    SMARTP<msrRepeat> createRepeatNewbornClone (
                            S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrRepeat (
                            int        inputLineNumber,
                            int        repeatTimes,
                            S_msrVoice upLinkToVoice);

    virtual               ~msrRepeat ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            getRepeatUpLinkToVoice () const
                              { return fRepeatUpLinkToVoice; }
    // time signatures
    int                   getRepeatTimes () const
                              { return fRepeatTimes; }

    void                  setRepeatTimes (int repeatTimes) // JMI
                              { fRepeatTimes = repeatTimes; }

    // explicit start?
    void                  setRepeatExplicitStartKind (
                            msrRepeatExplicitStartKind repeatExplicitStartKind)
                              {
                                fRepeatExplicitStartKind =
                                  repeatExplicitStartKind;
                              }

    msrRepeatExplicitStartKind
                          getRepeatExplicitStartKind () const
                              { return fRepeatExplicitStartKind; }

    // common part
    void                  setRepeatCommonPart (
                            S_msrRepeatCommonPart repeatCommonPart);

    S_msrRepeatCommonPart getRepeatCommonPart () const
                              { return fRepeatCommonPart; }

    // endings
    const vector<S_msrRepeatEnding>&
                          getRepeatEndings () const
                              { return fRepeatEndings; }

    // immediately preceding and following repeats
    void                  setImmediatelyPrecedingRepeat (
                            S_msrRepeat precedingRepeat)
                              { fImmediatelyPrecedingRepeat = precedingRepeat; }

    S_msrRepeat           getImmediatelyPrecedingRepeat () const
                              { return fImmediatelyPrecedingRepeat; }

    void                  setImmediatelyFollowingRepeat (
                            S_msrRepeat followingRepeat)
                              { fImmediatelyFollowingRepeat = followingRepeat; }

    S_msrRepeat           getImmediatelyFollowingRepeat () const
                              { return fImmediatelyFollowingRepeat; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addRepeatEndingToRepeat (
                            int               inputLineNumber,
                            S_msrRepeatEnding repeatEnding);

    void                  appendSegmentToRepeat (
                            int           inputLineNumber,
                            S_msrSegment  segment,
                            const string& context);

    void                  appendRepeatToRepeat (
                            int          inputLineNumber,
                            S_msrRepeat  repeat,
                            const string& context);

    void                  appendMeasureRepeatToRepeat (
                            int                inputLineNumber,
                            S_msrMeasureRepeat measureRepeat,
                            const string&      context);

    void                  appendMultipleFullBarRestsToRepeat (
                            int                   inputLineNumber,
                            S_msrMultipleFullBarRests multipleFullBarRests,
                            const string&         context);

    S_msrNote             fetchRepeatFirstNonGraceNote () const;

    void                  collectRepeatMeasuresIntoFlatList (
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

    string                asShortString () const override;
    string                asString () const override;

    void                  displayRepeat (
                            int           inputLineNumber,
                            const string& context);

    void                  print (ostream& os) const override;

    void                  printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fRepeatUpLinkToVoice;

    // number of repetitions
    int                   fRepeatTimes;

    // explicit start?
    msrRepeatExplicitStartKind
                          fRepeatExplicitStartKind;

    // common part
    S_msrRepeatCommonPart fRepeatCommonPart;

    // repeat endings
    vector<S_msrRepeatEnding>
                          fRepeatEndings;
    int                   fRepeatEndingsInternalCounter;

    // immediately preceding and following repeats
    // detecting several repeats in a row helps LilyPond code generation
    // depending on the options JMI
    S_msrRepeat           fImmediatelyPrecedingRepeat;
    S_msrRepeat           fImmediatelyFollowingRepeat;

  public:

    // public work services
    // ------------------------------------------------------

    // repeat build phase
    void                  setCurrentRepeatBuildPhaseKind (
                            msrRepeatBuildPhaseKind repeatBuildPhaseKind)
                              {
                                fCurrentRepeatBuildPhaseKind =
                                  repeatBuildPhaseKind;
                              }

    msrRepeatBuildPhaseKind
                          getCurrentRepeatBuildPhaseKind () const
                            { return fCurrentRepeatBuildPhaseKind; }

  private:

    // private work services
    // ------------------------------------------------------

  private:

    // work fields
    // ------------------------------------------------------

    // repeat build phase, used when building the repeat JMI ???
    msrRepeatBuildPhaseKind
                          fCurrentRepeatBuildPhaseKind;
};
typedef SMARTP<msrRepeat> S_msrRepeat;
EXP ostream& operator<< (ostream& os, const S_msrRepeat& elt);


}


#endif
