/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrStaves___
#define ___lpsrStaves___

#include <iostream>

#include "lpsrElements.h"

#include "msrStaves.h"
#include "msrElements.h"
#include "msrStavesDetails.h"
#include "msrVoices.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP lpsrNewStaffGroupBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrNewStaffGroupBlock> create (
                            int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrNewStaffGroupBlock (
                            int inputLineNumber);

    virtual               ~lpsrNewStaffGroupBlock ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

  private:

    // fields
    // ------------------------------------------------------

    std::vector <S_msrElement> fNewStaffGroupElementsVector;
};
typedef SMARTP<lpsrNewStaffGroupBlock> S_lpsrNewStaffGroupBlock;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrNewStaffGroupBlock& elt);

//______________________________________________________________________________
class EXP lpsrNewStaffTuningBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrNewStaffTuningBlock> create (
                            int                     inputLineNumber,
                            const S_msrStaffTuning& staffTuning);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrNewStaffTuningBlock (
                            int                     inputLineNumber,
                            const S_msrStaffTuning& staffTuning);

    virtual               ~lpsrNewStaffTuningBlock ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrStaffTuning      getStaffTuning () const
                              { return fStaffTuning; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

  private:

    // fields
    // ------------------------------------------------------

    S_msrStaffTuning      fStaffTuning;
};
typedef SMARTP<lpsrNewStaffTuningBlock> S_lpsrNewStaffTuningBlock;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrNewStaffTuningBlock& elt);

//______________________________________________________________________________
class EXP lpsrNewStaffBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrNewStaffBlock> create (
                            int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrNewStaffBlock (
                            int inputLineNumber);

    virtual               ~lpsrNewStaffBlock ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

  private:

    // fields
    // ------------------------------------------------------

    std::vector <S_msrElement> fNewStaffElementsVector;
};
typedef SMARTP<lpsrNewStaffBlock> S_lpsrNewStaffBlock;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrNewStaffBlock& elt);

//______________________________________________________________________________
class EXP lpsrStaffBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrStaffBlock> create (
                            const S_msrStaff& staff);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrStaffBlock (
                            const S_msrStaff& staff);

    virtual               ~lpsrStaffBlock ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrStaff            getStaff () const
                              { return fStaff; }

    const std::list <S_msrElement>&
                          getStaffBlockElements () const
                              { return fStaffBlockElements; }

    void                  setStaffBlockInstrumentName (
                            const std::string& instrumentName)
                              {
                                fStaffBlockInstrumentName =
                                  instrumentName;
                              }

    std::string           getStaffBlockInstrumentName () const
                              { return fStaffBlockInstrumentName; }

    void                  setStaffBlockShortInstrumentName (
                            const std::string& shortInstrumentName)
                              {
                                fStaffBlockShortInstrumentName =
                                  shortInstrumentName;
                              }

    std::string           getStaffBlockShortInstrumentName () const
                              { return fStaffBlockShortInstrumentName; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendVoiceUseToStaffBlock (
                            const S_msrVoice& voice);

    void                  appendLyricsUseToStaffBlock (
                            const S_msrStanza& stanza);

    void                  appendElementToStaffBlock (
                            const S_msrElement& elem)
                              { fStaffBlockElements.push_back (elem); }

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrStaff            fStaff;

    std::list <S_msrElement>
                          fStaffBlockElements;

    std::string           fStaffBlockInstrumentName;
    std::string           fStaffBlockShortInstrumentName;
};
typedef SMARTP<lpsrStaffBlock> S_lpsrStaffBlock;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrStaffBlock& elt);


}


#endif // ___lpsrStaves___
