/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrRepeats___
#define ___msrRepeats___

#include <vector>

#include "msrElements.h"
#include "msrBeatRepeats.h"
#include "msrMeasureRepeats.h"
#include "msrMultipleMeasureRests.h"
#include "msrRepeatsEnumTypes.h"
#include "msrVoiceElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrRepeatElement: public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatElement> create (
                            const mfInputLineNumber& inputLineNumber);

    static SMARTP<msrRepeatElement> create (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrRepeat&       upLinkToRepeat);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrRepeatElement (
                            const mfInputLineNumber& inputLineNumber);

                          msrRepeatElement (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrRepeat&       upLinkToRepeat);

    virtual               ~msrRepeatElement ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrRepeat           getRepeatElementUpLinkToRepeat () const
                              { return fRepeatElementUpLinkToRepeat; }

    // segment
    S_msrSegment          getRepeatElementSegment () const
                              { return fRepeatElementSegment; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendMeasureToRepeatElement (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasure&      measure,
                            const std::string&       context);

    void                  appendRepeatToRepeatElement (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrRepeat&       repeat,
                            const std::string&       context);

    void                  appendBeatRepeatToRepeatElement (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrBeatRepeat&   beatRepeat,
                            const std::string&       context);

    void                  appendMeasureRepeatToRepeatElement (
                            const mfInputLineNumber&  inputLineNumber,
                            const S_msrMeasureRepeat& measureRepeat,
                            const std::string&        context);

    void                  appendMultipleMeasureRestToRepeatElement (
                            const mfInputLineNumber&        inputLineNumber,
                            const S_msrMultipleMeasureRest& multipleMeasureRest,
                            const std::string&              context);

    S_msrNote             fetchRepeatElementFirstNonGraceNote () const;

    void                  collectRepeatElementMeasuresIntoFlatList (
                            const mfInputLineNumber& inputLineNumber);

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeRepeatElement (
                            const mfInputLineNumber& inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;
//
    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    // upLinks
    S_msrRepeat           fRepeatElementUpLinkToRepeat;

    // elements list
    S_msrSegment          fRepeatElementSegment;
};

using S_msrRepeatElement = SMARTP<msrRepeatElement>;

EXP std::ostream& operator << (std::ostream& os, const S_msrRepeatElement& elt);

//______________________________________________________________________________
class EXP msrRepeatCommonPart : public msrRepeatElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatCommonPart> create (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrRepeat&       upLinkToRepeat);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrRepeatCommonPart (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrRepeat&       upLinkToRepeat);

    virtual               ~msrRepeatCommonPart ();

  public:

    // set and get
    // ------------------------------------------------------

//     // upLinks
//     S_msrRepeat           getRepeatCommonPartUpLinkToRepeat () const
//                               { return fRepeatCommonPartUpLinkToRepeat; }

//     // elements
//     const std::list <S_msrVoiceElement>&
//                           getRepeatCommonPartElementsList () const
//                               { return fRepeatCommonPartElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendVoiceElementToRepeatCommonPart (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrVoiceElement& voiceElement,
                            const std::string&       context);

//     void                  appendSegmentToRepeatCommonPart (
//                             const mfInputLineNumber& inputLineNumber,
//                             const S_msrSegment&      segment,
//                             const std::string&       context);

    void                  appendRepeatToRepeatCommonPart (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrRepeat&       repeat,
                            const std::string&       context);

    void                  appendBeatRepeatToRepeatCommonPart (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrBeatRepeat&   beatRepeat,
                            const std::string&       context);

    void                  appendMeasureRepeatToRepeatCommonPart (
                            const mfInputLineNumber&  inputLineNumber,
                            const S_msrMeasureRepeat& measureRepeat,
                            const std::string&        context);

    void                  appendMultipleMeasureRestToRepeatCommonPart (
                            const mfInputLineNumber&        inputLineNumber,
                            const S_msrMultipleMeasureRest& multipleMeasureRests,
                            const std::string&              context);

    S_msrNote             fetchRepeatCommonPartFirstNonGraceNote () const;

    void                  collectRepeatCommonPartMeasuresIntoFlatList (
                            const mfInputLineNumber& inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

//     // upLinks
//     S_msrRepeat           fRepeatCommonPartUpLinkToRepeat;

//     // elements list
//     std::list <S_msrVoiceElement>
//                           fRepeatCommonPartElementsList;
};

using S_msrRepeatCommonPart = SMARTP<msrRepeatCommonPart>;

EXP std::ostream& operator << (std::ostream& os, const S_msrRepeatCommonPart& elt);

//______________________________________________________________________________
class EXP msrRepeatEnding : public msrRepeatElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatEnding> create (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string&       repeatEndingNumber, // a string, because if may be "1, 2" for example
                            msrRepeatEndingKind      repeatEndingKind,
                            const S_msrRepeat&       upLinkToRepeat);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrRepeatEnding (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string&       repeatEndingNumber, // a string, because if may be "1, 2" for example
                            msrRepeatEndingKind      repeatEndingKind,
                            const S_msrRepeat&       upLinkToRepeat);

    virtual               ~msrRepeatEnding ();

  public:

    // set and get
    // ------------------------------------------------------

//     // upLinks
//     S_msrRepeat           getRepeatEndingUpLinkToRepeat () const
//                               { return fRepeatEndingUpLinkToRepeat; }

    // numbers
    std::string           getRepeatEndingNumber () const
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

//     // elements
//     const std::list <S_msrVoiceElement>&
//                           getRepeatEndingElementsList () const
//                               { return fRepeatEndingElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendVoiceElementToRepeatEnding ( // JMI
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrVoiceElement& voiceElement,
                            const std::string&       context);

//     void                  appendSegmentToRepeatEnding (
//                             const mfInputLineNumber& inputLineNumber,
//                             const S_msrSegment&      segment,
//                             const std::string&       context);

    void                  appendRepeatToRepeatEnding (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrRepeat&       repeat,
                            const std::string&        context);

    void                  appendMeasureRepeatToRepeatEnding (
                            const mfInputLineNumber&  inputLineNumber,
                            const S_msrMeasureRepeat& measureRepeat,
                            const std::string&        context);

    void                  appendMultipleMeasureRestToRepeatEnding (
                            const mfInputLineNumber&        inputLineNumber,
                            const S_msrMultipleMeasureRest& multipleMeasureRests,
                            const std::string&              context);

    void                  collectRepeatEndingMeasuresIntoFlatList (
                            const mfInputLineNumber& inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

//     // upLinks
//     S_msrRepeat           fRepeatEndingUpLinkToRepeat;

    // numbers
    std::string           fRepeatEndingNumber; // a string, because if may be "1, 2" for example
    int                   fRepeatEndingInternalNumber; // internally assigned

    // kind
    msrRepeatEndingKind   fRepeatEndingKind;

//     // elements list
//     std::list <S_msrVoiceElement>
//                           fRepeatEndingElementsList;
};

using S_msrRepeatEnding = SMARTP<msrRepeatEnding>;

EXP std::ostream& operator << (std::ostream& os, const S_msrRepeatEnding& elt);

//______________________________________________________________________________
// class EXP msrRepeat : public msrVoiceElement
class EXP msrRepeat : public msrSegmentElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeat> create (
                            const mfInputLineNumber& inputLineNumber,
                            int repeatTimes);

    static SMARTP<msrRepeat> create (
                            const mfInputLineNumber& inputLineNumber,
                            int                      repeatTimes,
                            const S_msrVoice&        upLinkToVoice);

    SMARTP<msrRepeat> createRepeatNewbornClone (
                            const S_msrVoice& containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrRepeat (
                            const mfInputLineNumber& inputLineNumber,
                            int                      repeatTimes,
                            const S_msrVoice&        upLinkToVoice);

    virtual               ~msrRepeat ();

  private:

    // private initialization
    // ------------------------------------------------------

    void                  initializeRepeat (
                            const mfInputLineNumber& inputLineNumber,
                            int                      repeatTimes,
                            const S_msrVoice&        upLinkToVoice);

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    void                  setRepeatUpLinkToVoice (S_msrVoice voice)
                              { fRepeatUpLinkToVoice = voice; }

    S_msrVoice            getRepeatUpLinkToVoice () const
                              { return fRepeatUpLinkToVoice; }

    // time signatures
    void                  setRepeatTimes (int repeatTimes) // JMI
                              { fRepeatTimes = repeatTimes; }

    int                   getRepeatTimes () const
                              { return fRepeatTimes; }

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
                            const S_msrRepeatCommonPart& repeatCommonPart);

    S_msrRepeatCommonPart getRepeatCommonPart () const
                              { return fRepeatCommonPart; }

    // endings
    const std::vector <S_msrRepeatEnding>&
                          getRepeatEndings () const
                              { return fRepeatEndings; }

    // immediately preceding and following repeats
    void                  setImmediatelyPrecedingRepeat (
                            const S_msrRepeat& precedingRepeat)
                              { fImmediatelyPrecedingRepeat = precedingRepeat; }

    S_msrRepeat           getImmediatelyPrecedingRepeat () const
                              { return fImmediatelyPrecedingRepeat; }

    void                  setImmediatelyFollowingRepeat (
                            const S_msrRepeat& followingRepeat)
                              { fImmediatelyFollowingRepeat = followingRepeat; }

    S_msrRepeat           getImmediatelyFollowingRepeat () const
                              { return fImmediatelyFollowingRepeat; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addRepeatEndingToRepeat (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrRepeatEnding& repeatEnding);

//     void                  appendSegmentToRepeat (
//                             const mfInputLineNumber& inputLineNumber,
//                             const S_msrSegment&      segment,
//                             const std::string&       context);

    void                  appendRepeatToRepeat (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrRepeat&       repeat,
                            const std::string&       context);

    void                  appendMeasureRepeatToRepeat (
                            const mfInputLineNumber&  inputLineNumber,
                            const S_msrMeasureRepeat& measureRepeat,
                            const std::string&        context);

    void                  appendMultipleMeasureRestToRepeat (
                            const mfInputLineNumber&        inputLineNumber,
                            const S_msrMultipleMeasureRest& multipleMeasureRests,
                            const std::string&              context);

    S_msrNote             fetchRepeatFirstNonGraceNote () const;

    void                  collectRepeatMeasuresIntoFlatList (
                            const mfInputLineNumber& inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortString () const override;
    std::string           asString () const override;

    void                  displayRepeat (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& context);

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

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
    std::vector <S_msrRepeatEnding>
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

using S_msrRepeat = SMARTP<msrRepeat>;

EXP std::ostream& operator << (std::ostream& os, const S_msrRepeat& elt);


}


#endif // ___msrRepeats___
