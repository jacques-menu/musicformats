/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrStaves___
#define ___lpsrStaves___

#include <iostream>

#include "lpsrElements.h"

#include "msrStaves.h"
#include "msrStavesDetails.h"
#include "msrVoices.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP lpsrNewStaffgroupBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrNewStaffgroupBlock> create (
                            int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrNewStaffgroupBlock (
                            int inputLineNumber);

    virtual               ~lpsrNewStaffgroupBlock ();

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

    void                  print (ostream& os) const override;

  private:

    // fields
    // ------------------------------------------------------

    vector<S_msrElement> fNewStaffgroupElements;
};
typedef SMARTP<lpsrNewStaffgroupBlock> S_lpsrNewStaffgroupBlock;
EXP ostream& operator << (ostream& os, const S_lpsrNewStaffgroupBlock& elt);

//______________________________________________________________________________
class EXP lpsrNewStaffTuningBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrNewStaffTuningBlock> create (
                            int              inputLineNumber,
                            S_msrStaffTuning staffTuning);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrNewStaffTuningBlock (
                            int              inputLineNumber,
                            S_msrStaffTuning staffTuning);

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

    void                  print (ostream& os) const override;

  private:

    // fields
    // ------------------------------------------------------

    S_msrStaffTuning fStaffTuning;
};
typedef SMARTP<lpsrNewStaffTuningBlock> S_lpsrNewStaffTuningBlock;
EXP ostream& operator << (ostream& os, const S_lpsrNewStaffTuningBlock& elt);

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

    void                  print (ostream& os) const override;

  private:

    // fields
    // ------------------------------------------------------

    vector<S_msrElement> fNewStaffElements;
};
typedef SMARTP<lpsrNewStaffBlock> S_lpsrNewStaffBlock;
EXP ostream& operator << (ostream& os, const S_lpsrNewStaffBlock& elt);

//______________________________________________________________________________
class EXP lpsrStaffBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrStaffBlock> create (
                            S_msrStaff staff);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrStaffBlock (
                            S_msrStaff staff);

    virtual               ~lpsrStaffBlock ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrStaff            getStaff () const
                              { return fStaff; }

    const list<S_msrElement>&
                          getStaffBlockElements () const
                              { return fStaffBlockElements; }

    void                  setStaffBlockInstrumentName (
                            const string& instrumentName)
                              {
                                fStaffBlockInstrumentName =
                                  instrumentName;
                              }

    string                getStaffBlockInstrumentName () const
                              { return fStaffBlockInstrumentName; }

    void                  setStaffBlockShortInstrumentName (
                            const string& shortInstrumentName)
                              {
                                fStaffBlockShortInstrumentName =
                                  shortInstrumentName;
                              }

    string                getStaffBlockShortInstrumentName () const
                              { return fStaffBlockShortInstrumentName; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendVoiceUseToStaffBlock (
                            S_msrVoice voice);

    void                  appendLyricsUseToStaffBlock (
                            S_msrStanza stanza);

    void                  appendElementToStaffBlock (
                            S_msrElement elem)
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

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrStaff            fStaff;

    list<S_msrElement>    fStaffBlockElements;

    string                fStaffBlockInstrumentName;
    string                fStaffBlockShortInstrumentName;
};
typedef SMARTP<lpsrStaffBlock> S_lpsrStaffBlock;
EXP ostream& operator << (ostream& os, const S_lpsrStaffBlock& elt);


}


#endif
