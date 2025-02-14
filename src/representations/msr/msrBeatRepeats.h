/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrBeatRepeats___
#define ___msrBeatRepeats___

#include "msrElements.h"
#include "msrSegmentElements.h"

#include "msrSegments.h"
#include "msrVoices.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrBeatRepeatPattern : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBeatRepeatPattern> create (
                            int              inputLineNumber,
                            const S_msrBeatRepeat& upLinkToBeatRepeat);

    SMARTP<msrBeatRepeatPattern> createABeatRepeatPatternNewbornClone (
                            const S_msrVoice& containingVoice);

    SMARTP<msrBeatRepeatPattern> createABeatRepeatPatternDeepClone (
                            const S_msrVoice& containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrBeatRepeatPattern (
                            int              inputLineNumber,
                            const S_msrBeatRepeat& upLinkToBeatRepeat);

    virtual               ~msrBeatRepeatPattern ();

  public:

    // set and get
    // ------------------------------------------------------

    // segment
    void                  setBeatRepeatPatternSegment (
                            const S_msrSegment& beatRepeatPatternSegment);

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

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrBeatRepeat       fUpLinkToBeatRepeat;

    // segment
    S_msrSegment          fBeatRepeatPatternSegment;
};
typedef SMARTP<msrBeatRepeatPattern> S_msrBeatRepeatPattern;
EXP std::ostream& operator << (std::ostream& os, const S_msrBeatRepeatPattern& elt);

//______________________________________________________________________________
class EXP msrBeatRepeatReplicas : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBeatRepeatReplicas> create (
                            int              inputLineNumber,
                            const S_msrBeatRepeat& upLinkToBeatRepeat);

    SMARTP<msrBeatRepeatReplicas> createABeatRepeatReplicasNewbornClone (
                            const S_msrVoice& containingVoice);

    SMARTP<msrBeatRepeatReplicas> createABeatRepeatReplicasDeepClone (
                            const S_msrVoice& containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrBeatRepeatReplicas (
                            int              inputLineNumber,
                            const S_msrBeatRepeat& upLinkToBeatRepeat);

    virtual               ~msrBeatRepeatReplicas ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    S_msrBeatRepeat       getUpLinkToBeatRepeat () const
                              { return fUpLinkToBeatRepeat; }
    // segment
    void                  setBeatRepeatReplicasSegment (
                            const S_msrSegment& beatRepeatReplicasSegment);

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

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrBeatRepeat       fUpLinkToBeatRepeat;

    // segment
    S_msrSegment          fBeatRepeatReplicasSegment;
};
typedef SMARTP<msrBeatRepeatReplicas> S_msrBeatRepeatReplicas;
EXP std::ostream& operator << (std::ostream& os, const S_msrBeatRepeatReplicas& elt);

//______________________________________________________________________________
// data types

enum class msrBeatRepeatKind {
  kBeatRepeat_UNKNOWN_,
  kBeatRepeatStart, kBeatRepeatStop
};

std::string msrBeatRepeatKindAsString (
  msrBeatRepeatKind beatRepeatKind);

std::ostream& operator << (std::ostream& os, const msrBeatRepeatKind& elt);

enum class msrBeatRepeatBuildPhaseKind {
  kBeatRepeatBuildPhaseJustCreated,
  kBeatRepeatBuildPhaseInPattern,
  kBeatRepeatBuildPhaseInReplicas,
  kBeatRepeatBuildPhaseCompleted
};

std::string msrBeatRepeatBuildPhaseKindAsString (
  msrBeatRepeatBuildPhaseKind beatRepeatBuildPhaseKind);

std::ostream& operator << (std::ostream& os, const msrBeatRepeatBuildPhaseKind& elt);

class EXP msrBeatRepeat : public msrSegmentElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBeatRepeat> create (
                            int        inputLineNumber,
                            int        beatRepeatMeasuresNumber,
                            int        beatRepeatSlashesNumber,
                            const S_msrVoice& upLinkToVoice);

    SMARTP<msrBeatRepeat> createABeatRepeatNewbornClone (
                            const S_msrVoice& containingVoice);

    SMARTP<msrBeatRepeat> createABeatRepeatDeepClone (
                            const S_msrVoice& containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrBeatRepeat (
                            int        inputLineNumber,
                            int        beatRepeatMeasuresNumber,
                            int        beatRepeatSlashesNumber,
                            const S_msrVoice& upLinkToVoice);

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
                            const S_msrBeatRepeatPattern&
                              beatRepeatPattern);

    S_msrBeatRepeatPattern
                          getBeatRepeatPattern () const
                              { return fBeatRepeatPattern; }

    // measures repeat replicas
    void                  setBeatRepeatReplicas (
                            const S_msrBeatRepeatReplicas&
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
                            const S_msrMeasureElement& elem) override;

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

    std::string           asString () const override;

    void                  displayBeatRepeat (
                            int                inputLineNumber,
                            const std::string& context);

    void                  print (std::ostream& os) const override;

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
EXP std::ostream& operator << (std::ostream& os, const S_msrBeatRepeat& elt);


}


#endif // ___msrBeatRepeats___
