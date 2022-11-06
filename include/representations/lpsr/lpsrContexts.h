/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrContexts___
#define ___lpsrContexts___

#include <iostream>
#include <string>
#include <list>

#include "msr.h"

#include "lpsrElements.h"

using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class lpsrContextTypeKind {
  kChordNames, kFiguredBass
};

string lpsrContextTypeKindAsString (
  lpsrContextTypeKind contextTypeKind);

ostream& operator << (ostream& os, const lpsrContextTypeKind& elt);

enum class lpsrContextExistingKind {
  kExistingContextYes, kExistingContextNo
};

string lpsrContextExistingKindAsString (
  lpsrContextExistingKind contextExistingKind);

ostream& operator << (ostream& os, const lpsrContextExistingKind& elt);

class EXP lpsrContext : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrContext> create (
                            int                     inputLineNumber,
                            lpsrContextExistingKind contextExistingKind,
                            lpsrContextTypeKind     contextTypeKind,
                            const string&           contextName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrContext (
                            int                     inputLineNumber,
                            lpsrContextExistingKind contextExistingKind,
                            lpsrContextTypeKind     contextTypeKind,
                            const string&           contextName);

    virtual               ~lpsrContext ();

  public:

    // set and get
    // ------------------------------------------------------

    lpsrContextExistingKind
                          getContextExistingKind () const
                              { return fContextExistingKind; }

    lpsrContextTypeKind   getContextTypeKind () const
                              { return fContextTypeKind; }

    string                getContextName () const
                              { return fContextName; }

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

    string                getlpsrContextTypeKindAsString () const
                              {
                                return
                                  lpsrContextTypeKindAsString (
                                    fContextTypeKind);
                              }

    string                getlpsrContextExistingKindAsString () const
                              {
                                return
                                  lpsrContextExistingKindAsString (
                                    fContextExistingKind);
                              }

    void                  print (ostream& os) const override;

  protected:

    // fields
    // ------------------------------------------------------

    lpsrContextExistingKind
                          fContextExistingKind;
    lpsrContextTypeKind   fContextTypeKind;
    string                fContextName;

    list<S_msrElement>    fContextElementsList;
};
typedef SMARTP<lpsrContext> S_lpsrContext;
EXP ostream& operator << (ostream& os, const S_lpsrContext& elt);

//______________________________________________________________________________
class EXP lpsrChordNamesContext : public lpsrContext
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrChordNamesContext> create (
                            int                     inputLineNumber,
                            lpsrContextExistingKind contextExistingKind,
                            const string&           contextName,
                            S_msrVoice              contextVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrChordNamesContext (
                            int                     inputLineNumber,
                            lpsrContextExistingKind contextExistingKind,
                            const string&           contextName,
                            S_msrVoice              contextVoice);

    virtual               ~lpsrChordNamesContext ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrVoice            getContextVoice () const
                              { return fContextVoice; }

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

    S_msrVoice            fContextVoice;
};
typedef SMARTP<lpsrChordNamesContext> S_lpsrChordNamesContext;
EXP ostream& operator << (ostream& os, const S_lpsrChordNamesContext& elt);

//______________________________________________________________________________
class EXP lpsrFiguredBassContext : public lpsrContext
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrFiguredBassContext> create (
                            int                     inputLineNumber,
                            lpsrContextExistingKind contextExistingKind,
                            const string&           contextName,
                            S_msrStaff              contextStaff);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrFiguredBassContext (
                            int                     inputLineNumber,
                            lpsrContextExistingKind contextExistingKind,
                            const string&           contextName,
                            S_msrStaff              contextStaff);

    virtual               ~lpsrFiguredBassContext ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrStaff            getContextStaff () const
                              { return fContextStaff; }

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

    S_msrStaff            fContextStaff;
};
typedef SMARTP<lpsrFiguredBassContext> S_lpsrFiguredBassContext;
EXP ostream& operator << (ostream& os, const S_lpsrFiguredBassContext& elt);


}


#endif
