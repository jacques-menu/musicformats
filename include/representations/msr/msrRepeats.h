/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrRepeats___
#define ___msrRepeats___

#include <vector>

#include "msrElements.h"
#include "msrMeasureRepeats.h"
#include "msrMultipleFullBarRests.h"
#include "msrRepeatsEnumTypes.h"
#include "msrVoiceElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrRepeatCommonPart : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatCommonPart> create (
                            int         inputLineNumber,
                            const S_msrRepeat& upLinkToRepeat);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrRepeatCommonPart (
                            int         inputLineNumber,
                            const S_msrRepeat& upLinkToRepeat);

    virtual               ~msrRepeatCommonPart ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrRepeat           getRepeatCommonPartUpLinkToRepeat () const
                              { return fRepeatCommonPartUpLinkToRepeat; }

    // elements
    const std::list<S_msrVoiceElement>&
                          getRepeatCommonPartElementsList () const
                              { return fRepeatCommonPartElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendVoiceElementToRepeatCommonPart (
                            int               inputLineNumber,
                            const S_msrVoiceElement& voiceElement,
                            const std::string&          context);

    void                  appendSegmentToRepeatCommonPart (
                            int          inputLineNumber,
                            const S_msrSegment& segment,
                            const std::string&context);

    void                  appendRepeatToRepeatCommonPart (
                            int           inputLineNumber,
                            const S_msrRepeat&   repeat,
                            const std::string& context);

    void                  appendMeasureRepeatToRepeatCommonPart (
                            int                inputLineNumber,
                            const S_msrMeasureRepeat& measureRepeat,
                            const std::string&           context);

    void                  appendMultipleFullBarRestsToRepeatCommonPart (
                            int                   inputLineNumber,
                            const S_msrMultipleFullBarRests& multipleFullBarRests,
                            const std::string&         context);

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

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrRepeat           fRepeatCommonPartUpLinkToRepeat;

    // elements list
    std::list<S_msrVoiceElement>
                          fRepeatCommonPartElementsList;
};
typedef SMARTP<msrRepeatCommonPart> S_msrRepeatCommonPart;
EXP std::ostream& operator << (std::ostream& os, const S_msrRepeatCommonPart& elt);

//______________________________________________________________________________
class EXP msrRepeatEnding : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatEnding> create (
                            int                 inputLineNumber,
                            const std::string&  repeatEndingNumber, // may be "1, 2"
                            msrRepeatEndingKind repeatEndingKind,
                            const S_msrRepeat&  upLinkToRepeat);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrRepeatEnding (
                            int                 inputLineNumber,
                            const std::string&  repeatEndingNumber, // may be "1, 2"
                            msrRepeatEndingKind repeatEndingKind,
                            const S_msrRepeat&  upLinkToRepeat);

    virtual               ~msrRepeatEnding ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrRepeat           getRepeatEndingUpLinkToRepeat () const
                              { return fRepeatEndingUpLinkToRepeat; }

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

    // elements
    const std::list<S_msrVoiceElement>&
                          getRepeatEndingElementsList () const
                              { return fRepeatEndingElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendVoiceElementToRepeatEnding ( // JMI
                            int               inputLineNumber,
                            const S_msrVoiceElement& voiceElement,
                            const std::string&          context);

    void                  appendSegmentToRepeatEnding (
                            int           inputLineNumber,
                            const S_msrSegment&  segment,
                            const std::string& context);

    void                  appendRepeatToRepeatEnding (
                            int           inputLineNumber,
                            const S_msrRepeat&   repeat,
                            const std::string& context);

    void                  appendMeasureRepeatToRepeatEnding (
                            int                inputLineNumber,
                            const S_msrMeasureRepeat& measureRepeat,
                            const std::string&           context);

    void                  appendMultipleFullBarRestsToRepeatEnding (
                            int                   inputLineNumber,
                            const S_msrMultipleFullBarRests& multipleFullBarRests,
                            const std::string&         context);

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

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrRepeat           fRepeatEndingUpLinkToRepeat;

    // numbers
    std::string           fRepeatEndingNumber; // may be "1, 2"
    int                   fRepeatEndingInternalNumber; // internally assigned

    // kind
    msrRepeatEndingKind   fRepeatEndingKind;

    // elements list
    std::list<S_msrVoiceElement>
                          fRepeatEndingElementsList;
};
typedef SMARTP<msrRepeatEnding> S_msrRepeatEnding;
EXP std::ostream& operator << (std::ostream& os, const S_msrRepeatEnding& elt);

//______________________________________________________________________________
class EXP msrRepeat : public msrVoiceElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeat> create (
                            int        inputLineNumber,
                            int        repeatTimes,
                            const S_msrVoice& upLinkToVoice);

    SMARTP<msrRepeat> createRepeatNewbornClone (
                            const S_msrVoice& containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrRepeat (
                            int        inputLineNumber,
                            int        repeatTimes,
                            const S_msrVoice& upLinkToVoice);

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
                            const S_msrRepeatCommonPart& repeatCommonPart);

    S_msrRepeatCommonPart getRepeatCommonPart () const
                              { return fRepeatCommonPart; }

    // endings
    const std::vector<S_msrRepeatEnding>&
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
                            int               inputLineNumber,
                            const S_msrRepeatEnding& repeatEnding);

    void                  appendSegmentToRepeat (
                            int           inputLineNumber,
                            const S_msrSegment&  segment,
                            const std::string& context);

    void                  appendRepeatToRepeat (
                            int          inputLineNumber,
                            const S_msrRepeat&  repeat,
                            const std::string& context);

    void                  appendMeasureRepeatToRepeat (
                            int                inputLineNumber,
                            const S_msrMeasureRepeat& measureRepeat,
                            const std::string&           context);

    void                  appendMultipleFullBarRestsToRepeat (
                            int                   inputLineNumber,
                            const S_msrMultipleFullBarRests& multipleFullBarRests,
                            const std::string&         context);

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

    std::string           asShortString () const override;
    std::string           asString () const override;

    void                  displayRepeat (
                            int                inputLineNumber,
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
    std::vector<S_msrRepeatEnding>
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
EXP std::ostream& operator << (std::ostream& os, const S_msrRepeat& elt);


}


#endif // ___msrRepeats___
