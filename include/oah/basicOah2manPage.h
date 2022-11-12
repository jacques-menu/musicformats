/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___basicOah2manPage___
#define ___basicOah2manPage___

#include "oahAtomsCollection.h"

#include "oah2manPage.h"


namespace MusicFormats
{

//________________________________________________________________________
class EXP basicOah2manPage : virtual public oah2manPage,

  public visitor<S_oahPrefix>,

  public visitor<S_oahGroup>,

  public visitor<S_oahSubGroup>,

  public visitor<S_oahAtom>,

  public visitor<S_oahAtomAlias>,

  public visitor<S_oahOptionsUsageAtom>,

  public visitor<S_oahHelpSummaryAtom>,

  public visitor<S_oahAtomStoringAValue>,

  public visitor<S_oahOptionNameHelpAtom>,

  public visitor<S_oahBooleanAtom>,
  public visitor<S_oahTwoBooleansAtom>,
  public visitor<S_oahThreeBooleansAtom>,

  public visitor<S_oahCombinedBooleansAtom>,

  public visitor<S_oahCommonPrefixBooleansAtom>,

// JMI  public visitor<S_oahAtomStoringAValue>,

  public visitor<S_oahIntegerAtom>,
  public visitor<S_oahTwoIntegersAtom>,

  public visitor<S_oahFloatAtom>,

  public visitor<S_oahStringAtom>,

  public visitor<S_oahRationalAtom>,

  public visitor<S_oahFactorizedStringAtom>,

  public visitor<S_oahStringWithDefaultValueAtom>,

  public visitor<oahNaturalNumbersSetElementAtom>,

  public visitor<S_oahStringSetElementAtom>,

  public visitor<S_oahColorRGBAtom>,

  public visitor<S_oahStringAndIntegerAtom>,
  public visitor<S_oahStringAndTwoIntegersAtom>

{
  public:

                          basicOah2manPage (
                            const S_oahHandler handler,
                            std::ostream&           manPageOutputStream);

    virtual               ~basicOah2manPage ();

  protected:

    virtual void          visitStart (S_oahHandler& elt);
    virtual void          visitEnd   (S_oahHandler& elt);

    virtual void          visitStart (S_oahGroup& elt);
    virtual void          visitEnd   (S_oahGroup& elt);

    virtual void          visitStart (S_oahPrefix& elt);
    virtual void          visitEnd   (S_oahPrefix& elt);

    virtual void          visitStart (S_oahSubGroup& elt);
    virtual void          visitEnd   (S_oahSubGroup& elt);

    virtual void          visitStart (S_oahAtom& elt);
    virtual void          visitEnd   (S_oahAtom& elt);

    virtual void          visitStart (S_oahAtomAlias& elt);
    virtual void          visitEnd   (S_oahAtomAlias& elt);

    virtual void          visitStart (S_oahOptionsUsageAtom& elt);
    virtual void          visitEnd   (S_oahOptionsUsageAtom& elt);

    virtual void          visitStart (S_oahHelpSummaryAtom& elt);
    virtual void          visitEnd   (S_oahHelpSummaryAtom& elt);

    virtual void          visitStart (S_oahAtomStoringAValue& elt);
    virtual void          visitEnd   (S_oahAtomStoringAValue& elt);

    virtual void          visitStart (S_oahOptionNameHelpAtom& elt);
    virtual void          visitEnd   (S_oahOptionNameHelpAtom& elt);

    virtual void          visitStart (S_oahBooleanAtom& elt);
    virtual void          visitEnd   (S_oahBooleanAtom& elt);
    virtual void          visitStart (S_oahTwoBooleansAtom& elt);
    virtual void          visitEnd   (S_oahTwoBooleansAtom& elt);
    virtual void          visitStart (S_oahThreeBooleansAtom& elt);
    virtual void          visitEnd   (S_oahThreeBooleansAtom& elt);

    virtual void          visitStart (S_oahCombinedBooleansAtom& elt);
    virtual void          visitEnd   (S_oahCombinedBooleansAtom& elt);

    virtual void          visitStart (S_oahCommonPrefixBooleansAtom& elt);
    virtual void          visitEnd   (S_oahCommonPrefixBooleansAtom& elt);

/*
    virtual void          visitStart (S_oahAtomStoringAValue& elt);
    virtual void          visitEnd   (S_oahAtomStoringAValue& elt);
*/
    virtual void          visitStart (S_oahIntegerAtom& elt);
    virtual void          visitEnd   (S_oahIntegerAtom& elt);
    virtual void          visitStart (S_oahTwoIntegersAtom& elt);
    virtual void          visitEnd   (S_oahTwoIntegersAtom& elt);

    virtual void          visitStart (S_oahFloatAtom& elt);
    virtual void          visitEnd   (S_oahFloatAtom& elt);

    virtual void          visitStart (S_oahStringAtom& elt);
    virtual void          visitEnd   (S_oahStringAtom& elt);

    virtual void          visitStart (S_oahRationalAtom& elt);
    virtual void          visitEnd   (S_oahRationalAtom& elt);

    virtual void          visitStart (S_oahFactorizedStringAtom& elt);
    virtual void          visitEnd   (S_oahFactorizedStringAtom& elt);

    virtual void          visitStart (S_oahStringWithDefaultValueAtom& elt);
    virtual void          visitEnd   (S_oahStringWithDefaultValueAtom& elt);

    virtual void          visitStart (S_oahNaturalNumbersSetElementAtom& elt);
    virtual void          visitEnd   (S_oahNaturalNumbersSetElementAtom& elt);

    virtual void          visitStart (S_oahStringSetElementAtom& elt);
    virtual void          visitEnd   (S_oahStringSetElementAtom& elt);

    virtual void          visitStart (S_oahColorRGBAtom& elt);
    virtual void          visitEnd   (S_oahColorRGBAtom& elt);

    virtual void          visitStart (S_oahStringAndIntegerAtom& elt);
    virtual void          visitEnd   (S_oahStringAndIntegerAtom& elt);

    virtual void          visitStart (S_oahStringAndTwoIntegersAtom& elt);
    virtual void          visitEnd   (S_oahStringAndTwoIntegersAtom& elt);
};
typedef SMARTP<basicOah2manPage> S_basicOah2manPage;


}


#endif
