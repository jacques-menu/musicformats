/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrVariables___
#define ___lpsrVariables___

#include <string>

#include "lpsrElements.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP lpsrVariableUseCommand : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrVariableUseCommand> create (
                            int           inputLineNumber,
                            const string& variableName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrVariableUseCommand (
                            int           inputLineNumber,
                            const string& variableName);

    virtual               ~lpsrVariableUseCommand ();

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

    string                fVariableName;
};
typedef SMARTP<lpsrVariableUseCommand> S_lpsrVariableUseCommand;
EXP ostream& operator << (ostream& os, const S_lpsrVariableUseCommand& elt);


}


#endif
