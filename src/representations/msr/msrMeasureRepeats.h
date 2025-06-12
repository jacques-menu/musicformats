/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMeasureRepeats___
#define ___msrMeasureRepeats___

#include "msrElements.h"
#include "msrSegmentElements.h"

#include "msrSegments.h"
#include "msrVoices.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrMeasureRepeatPattern : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasureRepeatPattern> create (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasureRepeat& upLinkToMeasureRepeat);

    SMARTP<msrMeasureRepeatPattern> createMeasureRepeatPatternNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrMeasureRepeatPattern> createMeasureRepeatPatternDeepClone (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMeasureRepeatPattern (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasureRepeat& upLinkToMeasureRepeat);

    virtual               ~msrMeasureRepeatPattern ();

  public:

    // set and get
    // ------------------------------------------------------

    // segment
    void                  setMeasureRepeatPatternSegment (
                            const S_msrSegment& measureRepeatPatternSegment);

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

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrMeasureRepeat    fUpLinkToMeasureRepeat;

    // segment
    S_msrSegment          fMeasureRepeatPatternSegment;
};

using S_msrMeasureRepeatPattern = SMARTP<msrMeasureRepeatPattern>;

EXP std::ostream& operator << (std::ostream& os, const S_msrMeasureRepeatPattern& elt);

//______________________________________________________________________________
class EXP msrMeasureRepeatReplicas : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasureRepeatReplicas> create (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasureRepeat& upLinkToMeasureRepeat);

    SMARTP<msrMeasureRepeatReplicas> createMeasureRepeatReplicasNewbornClone (
                            const S_msrVoice& containingVoice);

    SMARTP<msrMeasureRepeatReplicas> createMeasureRepeatReplicasDeepClone (
                            const S_msrVoice& containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMeasureRepeatReplicas (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasureRepeat& upLinkToMeasureRepeat);

    virtual               ~msrMeasureRepeatReplicas ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrMeasureRepeat    getUpLinkToMeasureRepeat () const
                              { return fUpLinkToMeasureRepeat; }
    // segment
    void                  setMeasureRepeatReplicasSegment (
                            const S_msrSegment& measureRepeatReplicasSegment);

    S_msrSegment          getMeasureRepeatReplicasSegment () const
                              { return fMeasureRepeatReplicasSegment; }

  public:

    // public services
    // ------------------------------------------------------

    int                   fetchMeasuresNumber () const;

    int                   fetchMeasureRepeatReplicasNumber () const;

    void                  collectRepeatReplicasMeasuresIntoFlatList (
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

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrMeasureRepeat    fUpLinkToMeasureRepeat;

    // segment
    S_msrSegment          fMeasureRepeatReplicasSegment;
};

using S_msrMeasureRepeatReplicas = SMARTP<msrMeasureRepeatReplicas>;

EXP std::ostream& operator << (std::ostream& os, const S_msrMeasureRepeatReplicas& elt);

//______________________________________________________________________________
// data types

enum class msrMeasureRepeatKind {
  kMeasureRepeat_UNKNOWN_,
  kMeasureRepeatStart, kMeasureRepeatStop
};

std::string msrMeasureRepeatKindAsString ( // JMI 0.9.66
  msrMeasureRepeatKind measureRepeatKind);

std::ostream& operator << (std::ostream& os, const msrMeasureRepeatKind& elt);

enum class msrMeasureRepeatBuildPhaseKind {
  kMeasureRepeatBuildPhaseJustCreated,
  kMeasureRepeatBuildPhaseInPattern,
  kMeasureRepeatBuildPhaseInReplicas,
  kMeasureRepeatBuildPhaseCompleted
};

std::ostream& operator << (std::ostream& os, const msrMeasureRepeatBuildPhaseKind& elt);

// std::string msrMeasureRepeatBuildPhaseKindAsString ( // JMI 0.9.66
//   msrMeasureRepeatBuildPhaseKind measureRepeatBuildPhaseKind);
//

class EXP msrMeasureRepeat : public msrSegmentElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasureRepeat> create (
                            const mfInputLineNumber& inputLineNumber,
                            int measureRepeatMeasuresNumber,
                            int measureRepeatSlashesNumber);

    SMARTP<msrMeasureRepeat>
                          createMeasureRepeatNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMeasureRepeat (
                            const mfInputLineNumber& inputLineNumber,
                            int measureRepeatMeasuresNumber,
                            int measureRepeatSlashesNumber);

    virtual               ~msrMeasureRepeat ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            getUpLinkToMeasureRepeatToVoice () const
                              { return fUpLinkToMeasureRepeatToVoice; }

    void                  setUpLinkToMeasureRepeatToVoice (
                            S_msrVoice containingVoice)
                              { fUpLinkToMeasureRepeatToVoice = containingVoice; }

    // slashes
    int                   getMeasureRepeatSlashesNumber () const
                              { return fMeasureRepeatSlashesNumber; }

    // measureRepeat pattern
    void                  setMeasureRepeatPattern (
                            const S_msrMeasureRepeatPattern&
                              measureRepeatPattern);

    S_msrMeasureRepeatPattern
                          getMeasureRepeatPattern () const
                              { return fMeasureRepeatPattern; }

    // measures repeat replicas
    void                  setMeasureRepeatReplicas (
                            const S_msrMeasureRepeatReplicas&
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
                            const S_msrMeasureElement& elem) override;

    int                   fetchMeasureRepeatReplicasNumber () const;

    int                   fetchMeasureRepeatPatternMeasuresNumber () const;
    int                   fetchMeasureRepeatReplicasMeasuresNumber () const;

    void                  collectMeasureRepeatMeasuresIntoFlatList (
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

    void                  displayMeasureRepeat (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& context);

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fUpLinkToMeasureRepeatToVoice;

    // measures
    int                   fMeasureRepeatMeasuresNumber;

    // slashes
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

using S_msrMeasureRepeat = SMARTP<msrMeasureRepeat>;

EXP std::ostream& operator << (std::ostream& os, const S_msrMeasureRepeat& elt);


}


#endif // ___msrMeasureRepeats___


//______________________________________________________________________________
/* JMI
class EXP msrMeasureRepeatElement : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasureRepeatElement> create (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasureRepeat& upLinkToMeasureRepeat);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMeasureRepeatElement (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasureRepeat& upLinkToMeasureRepeat);

    virtual               ~msrMeasureRepeatElement ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrMeasureRepeat   getMeasureRepeatElementUpLinkToMeasureRepeat () const
                              { return fMeasureRepeatElementUpLinkToMeasureRepeat; }

    // elements
    const std::list <S_msrVoiceElement>&
                          getMeasureRepeatElementsList () const
                              { return fMeasureRepeatElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendSegmentToMeasureRepeatElementsList (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrSegment& segment,
                            const std::string&       context);

    void                  appendMeasureRepeatToMeasureRepeatElementsList (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasureRepeat&  measureRepeat,
                            const std::string&          context);


    void                  cascadeAppendMultipleMeasureRestToMeasureRepeatElementsList (
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrMeasureRepeat& measureRepeat,
                            const std::string&       context);

    S_msrNote             fetchMeasureRepeatElementFirstNonGraceNote () const;

    void                  collectMeasureRepeatElementMeasuresIntoFlatList (
                            const mfInputLineNumber& inputLineNumber);

  private:

    // private services
    // ------------------------------------------------------

    void                  appendVoiceElementToMeasureRepeatElementsList ( // JMI
                            const mfInputLineNumber& inputLineNumber,
                            const S_msrVoiceElement& voiceElement,
                            const std::string&       context);

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

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrMeasureRepeat   fMeasureRepeatElementUpLinkToMeasureRepeat;

    // elements list
    std::list <S_msrVoiceElement>
                          fMeasureRepeatElementsList;
};

using S_msrMeasureRepeatElement = SMARTP<msrMeasureRepeatElement>;

EXP std::ostream& operator << (std::ostream& os, const S_msrMeasureRepeatElement& elt);
*/

