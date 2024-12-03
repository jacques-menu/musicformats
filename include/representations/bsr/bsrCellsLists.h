/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrCellsLists___
#define ___bsrCellsLists___

#include <list>
#include <vector>

#include "exports.h"
#include "smartpointer.h"
#include "basevisitor.h"

#include "bsrEnumTypes.h"
#include "bsrCellsLists.h"


using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
class   bsrCellsList;
typedef SMARTP<bsrCellsList> S_bsrCellsList;

//______________________________________________________________________________
class EXP bsrCellsList : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrCellsList> create (
                            int         inputLineNumber);

    static SMARTP<bsrCellsList> create (
                            int         inputLineNumber,
                            bsrCellKind cellKind1);

    static SMARTP<bsrCellsList> create (
                            int         inputLineNumber,
                            bsrCellKind cellKind1,
      bsrCellKind cellKind2);

    static SMARTP<bsrCellsList> create (
                            int         inputLineNumber,
                            bsrCellKind cellKind1,
                            bsrCellKind cellKind2,
                            bsrCellKind cellKind3);

    static SMARTP<bsrCellsList> create (
                            int         inputLineNumber,
                            bsrCellKind cellKind1,
                            bsrCellKind cellKind2,
                            bsrCellKind cellKind3,
                            bsrCellKind cellKind4);

    static SMARTP<bsrCellsList> create (
                            int         inputLineNumber,
                            bsrCellKind cellKind1,
                            bsrCellKind cellKind2,
                            bsrCellKind cellKind3,
                            bsrCellKind cellKind4,
                            bsrCellKind cellKind5);

    static SMARTP<bsrCellsList> create (
                            int         inputLineNumber,
                            bsrCellKind cellKind1,
                            bsrCellKind cellKind2,
                            bsrCellKind cellKind3,
                            bsrCellKind cellKind4,
                            bsrCellKind cellKind5,
                            bsrCellKind cellKind6);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrCellsList (
                            int         inputLineNumber);

                          bsrCellsList (
                            int         inputLineNumber,
                            bsrCellKind cellKind1);

                          bsrCellsList (
                            int         inputLineNumber,
                            bsrCellKind cellKind1,
                            bsrCellKind cellKind2);

                          bsrCellsList (
                            int         inputLineNumber,
                            bsrCellKind cellKind1,
                            bsrCellKind cellKind2,
                            bsrCellKind cellKind3);

                          bsrCellsList (
                            int         inputLineNumber,
                            bsrCellKind cellKind1,
                            bsrCellKind cellKind2,
                            bsrCellKind cellKind3,
                            bsrCellKind cellKind4);

                          bsrCellsList (
                            int         inputLineNumber,
                            bsrCellKind cellKind1,
                            bsrCellKind cellKind2,
                            bsrCellKind cellKind3,
                            bsrCellKind cellKind4,
                            bsrCellKind cellKind5);

                          bsrCellsList (
                            int         inputLineNumber,
                            bsrCellKind cellKind1,
                            bsrCellKind cellKind2,
                            bsrCellKind cellKind3,
                            bsrCellKind cellKind4,
                            bsrCellKind cellKind5,
                            bsrCellKind cellKind6);

    virtual               ~bsrCellsList ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::list <bsrCellKind>&
                          getCellsListElements () const
                              { return fCellsListElements; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendCellKindToCellsList (bsrCellKind cellKind)
                              { fCellsListElements.push_back (cellKind); }

    void                  appendCellsListToCellsList (
                            const S_bsrCellsList& otherCellsList);

    void                  prependCellsListToCellsList (
                            const S_bsrCellsList& otherCellsList);

    int                   fetchCellsNumber () const
                              { return fCellsListElements.size (); }

// JMI    void                  generateBrailleCode (std::ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual std::string   asString () const;

    virtual std::string   asShortString () const;

    virtual void          print (std::ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    std::list <bsrCellKind>     fCellsListElements;
};
typedef SMARTP<bsrCellsList> S_bsrCellsList;
EXP std::ostream& operator << (std::ostream& os, const S_bsrCellsList& elt);


}


#endif // ___bsrCellsLists___
