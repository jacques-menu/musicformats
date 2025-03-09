/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrBarNumbers___
#define ___lpsrBarNumbers___

#include <iostream>

#include "lpsrElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP lpsrBarNumberCheck : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrBarNumberCheck> create (
                            int inputLineNumber,
                            int nextBarNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrBarNumberCheck(
                            int inputLineNumber,
                            int nextBarNumber);

    virtual               ~lpsrBarNumberCheck ();

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

    // private fields
    // ------------------------------------------------------

    int                   fNextBarNumber;
};
typedef SMARTP<lpsrBarNumberCheck> S_lpsrBarNumberCheck;

//______________________________________________________________________________
class EXP lpsrBarCommand : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrBarCommand> create (
                            int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrBarCommand (
                            int inputLineNumber);

    virtual               ~lpsrBarCommand ();

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

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<lpsrBarCommand> S_lpsrBarCommand;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrBarCommand& elt);


}


#endif // ___lpsrBarNumbers___
