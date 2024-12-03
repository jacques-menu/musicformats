/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfList___
#define ___mfList___

#include <list>

#include "exports.h"
#include "smartpointer.h"


using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
template <typename ELEMENT_TYPE>
class EXP mfList : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfList> create ();

    static SMARTP<mfList> create (const ELEMENT_TYPE& element);

    // constructors/destructor
    // ------------------------------------------------------

                          mfList ();

                          mfList (const ELEMENT_TYPE& element);

    virtual               ~mfList ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::list <ELEMENT_TYPE>&
                          getElementsStdList () const
                              { return fElementsStdList;}

  public:

    // public services
    // ------------------------------------------------------

    // basic list stuff
    size_t                size () const
                              { return fElementsStdList.size (); }

    ELEMENT_TYPE&         front ()
                              { return fElementsStdList.front (); }

    ELEMENT_TYPE&         back ()
                              { return fElementsStdList.back (); }

    void                  pop_front ()
                              { fElementsStdList.pop_front (); }

    void                  pop_back ()
                              { fElementsStdList.pop_back (); }

    void                  push_front (
                            const ELEMENT_TYPE& element)
                              { fElementsStdList.push_front (element); }

    void                  push_back (
                            const ELEMENT_TYPE& element)
                              { fElementsStdList.push_back (element); }

    // specific list stuff
    Bool                  contains (
                            const ELEMENT_TYPE& value) const;

    // sort
    void                  sortByDecreasingIdentity();

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const;

    void                  printWithContext (
                            const std::string& context,
                            char               evidencer,
                            std::ostream&      os) const;

  private:

    // private fields
    // ------------------------------------------------------

    std::string           fElementListName;

    std::list <ELEMENT_TYPE>
                          fElementsStdList;
};

template <typename ELEMENT_TYPE>
EXP std::ostream& operator << (std::ostream& os, const mfList<ELEMENT_TYPE>& elt);

template <typename ELEMENT_TYPE>
EXP std::ostream& operator << (std::ostream& os, const SMARTP<mfList<ELEMENT_TYPE>>& elt);


}


#endif // ___mfList___
