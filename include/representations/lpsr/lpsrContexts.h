/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrContexts___
#define ___lpsrContexts___

#include <iostream>
#include <string>
#include <list>

#include "visitor.h"

#include "msrParts.h"
#include "msrStaves.h"
#include "msrVoices.h"

#include "lpsrElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class lpsrContextTypeKind {
  kContextChordNames, kContextFiguredBass
};

std::string lpsrContextTypeKindAsString (
  lpsrContextTypeKind contextTypeKind);

std::ostream& operator << (std::ostream& os, const lpsrContextTypeKind& elt);

enum class lpsrContextUseExistingKind {
  kUseExistingContextYes, kUseExistingContextNo
};

std::string lpsrContextUseExistingKindAsString (
  lpsrContextUseExistingKind contextUseExistingKind);

std::ostream& operator << (std::ostream& os, const lpsrContextUseExistingKind& elt);

//______________________________________________________________________________
class EXP lpsrContext : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrContext> create (
                            int                        inputLineNumber,
                            lpsrContextUseExistingKind contextUseExistingKind,
                            lpsrContextTypeKind        contextTypeKind,
                            const std::string&         contextPathLikeName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrContext (
                            int                        inputLineNumber,
                            lpsrContextUseExistingKind contextUseExistingKind,
                            lpsrContextTypeKind        contextTypeKind,
                            const std::string&         contextPathLikeName);

    virtual               ~lpsrContext ();

  public:

    // set and get
    // ------------------------------------------------------

    lpsrContextUseExistingKind
                          getContextUseExistingKind () const
                              { return fContextUseExistingKind; }

    lpsrContextTypeKind   getContextTypeKind () const
                              { return fContextTypeKind; }

    std::string           getContextPathLikeName () const
                              { return fContextPathLikeName; }

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

    std::string           getLpsrContextTypeKindAsString () const
                              {
                                return
                                  lpsrContextTypeKindAsString (
                                    fContextTypeKind);
                              }

    std::string           getLpsrContextUseExistingKindAsString () const
                              {
                                return
                                  lpsrContextUseExistingKindAsString (
                                    fContextUseExistingKind);
                              }

    void                  print (std::ostream& os) const override;

  protected:

    // fields
    // ------------------------------------------------------

    lpsrContextUseExistingKind
                          fContextUseExistingKind;
    lpsrContextTypeKind   fContextTypeKind;

    std::string           fContextPathLikeName;

    std::list <S_msrElement>
                          fContextElementsList;
};
typedef SMARTP<lpsrContext> S_lpsrContext;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrContext& elt);

//______________________________________________________________________________
class EXP lpsrChordNamesContext : public lpsrContext
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrChordNamesContext> create (
                            int                        inputLineNumber,
                            lpsrContextUseExistingKind contextUseExistingKind,
                            const std::string&         contextPathLikeName,
                            const S_msrVoice&          contextVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrChordNamesContext (
                            int                        inputLineNumber,
                            lpsrContextUseExistingKind contextUseExistingKind,
                            const std::string&         contextPathLikeName,
                            const S_msrVoice&          contextVoice);

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

    void                  print (std::ostream& os) const override;

  private:

    // fields
    // ------------------------------------------------------

    S_msrVoice            fContextVoice;
};
typedef SMARTP<lpsrChordNamesContext> S_lpsrChordNamesContext;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrChordNamesContext& elt);

//______________________________________________________________________________
class EXP lpsrFiguredBassContext : public lpsrContext
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrFiguredBassContext> create (
                            int                        inputLineNumber,
                            lpsrContextUseExistingKind contextUseExistingKind,
                            const std::string&        contextPathLikeName,
                            const S_msrStaff&         contextStaff);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrFiguredBassContext (
                            int                        inputLineNumber,
                            lpsrContextUseExistingKind contextUseExistingKind,
                            const std::string&         contextPathLikeName,
                            const S_msrStaff&          contextStaff);

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

    void                  print (std::ostream& os) const override;

  private:

    // fields
    // ------------------------------------------------------

    S_msrStaff            fContextStaff;
};
typedef SMARTP<lpsrFiguredBassContext> S_lpsrFiguredBassContext;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrFiguredBassContext& elt);


}


#endif // ___lpsrContexts___
