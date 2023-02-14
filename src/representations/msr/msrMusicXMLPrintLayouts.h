/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMusicXMLPrintLayouts___
#define ___msrMusicXMLPrintLayouts___

#include <list>

#include "msrMeasureElements.h"
#include "msrMusicXMLBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrMusicXMLPrintLayout : public msrMeasureElement
  // include it for MusicXML generation from MSR
{
/*
<!--
  The print element contains general printing parameters,
  including the layout elements defined in the layout.mod
  file. The part-name-display and part-abbreviation-display
  elements used in the score.mod file may also be used here
  to change how a part name or abbreviation is displayed over
  the course of a piece. They take effect when the current
  measure or a succeeding measure starts a new system.

  The new-system and new-page attributes indicate whether
  to force a system or page break, or to force the current
  music onto the same system or page as the preceding music.
  Normally this is the first music data within a measure.
  If used in multi-part music, they should be placed in the
  same positions within each part, or the results are
  undefined. The page-number attribute sets the number of a
  new page; it is ignored if new-page is not "yes". Version
  2.0 adds a blank-page attribute. This is a positive integer
  value that specifies the number of blank pages to insert
  before the current measure. It is ignored if new-page is
  not "yes". These blank pages have no music, but may have
  text or images specified by the credit element. This is
  used to allow a combination of pages that are all text,
  or all text and images, together with pages of music.

  Staff spacing between multiple staves is measured in
  tenths of staff lines (e.g. 100 = 10 staff lines). This is
  deprecated as of Version 1.1; the staff-layout element
  should be used instead. If both are present, the
  staff-layout values take priority.

  Layout elements in a print statement only apply to the
  current page, system, staff, or measure. Music that
  follows continues to take the default values from the
  layout included in the defaults element.
-->
<!ELEMENT print (page-layout?, system-layout?, staff-layout*,
    measure-layout?, measure-numbering?, part-name-display?,
    part-abbreviation-display?)>
<!ATTLIST print
    staff-spacing %tenths; #IMPLIED
    new-system %yes-no; #IMPLIED
    new-page %yes-no; #IMPLIED
    blank-page NMTOKEN #IMPLIED
    page-number CDATA #IMPLIED
    %optional-unique-id;
>
*/

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMusicXMLPrintLayout> create (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

    static SMARTP<msrMusicXMLPrintLayout> create (
                            int           inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMusicXMLPrintLayout (
                            int                 inputLineNumber,
                            const S_msrMeasure& upLinkToMeasure);

    virtual               ~msrMusicXMLPrintLayout ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink to measure
    void                  setMeasureElementUpLinkToMeasure (
                            const S_msrMeasure& measure) override;

    void                  getMeasureElementUpLinkToMeasure (
                            S_msrMeasure& upLinkToMeasure) const override;

    void                  setMusicXMLPrintLayoutUpLinkToMeasure (
                            const S_msrMeasure& measure);

    S_msrMeasure          getMusicXMLPrintLayoutUpLinkToMeasure () const;

    void                  setPageLayout (S_msrPageLayout pageLayout)
                              { fPageLayout = pageLayout; }

    S_msrPageLayout       getPageLayout () const
                              { return fPageLayout; }

    void                  setSystemLayout (S_msrSystemLayout systemLayout)
                              { fSystemLayout = systemLayout; }

    S_msrSystemLayout     getSystemLayout () const
                              { return fSystemLayout; }

    const std::list<S_msrStaffLayout>&
                          getStaffLayoutsList () const
                              { return fStaffLayoutsList; }

    void                  setSystemDividers (S_msrSystemDividers systemDividers)
                              { fSystemDividers = systemDividers; }

    S_msrSystemDividers   getSystemDividers () const
                              { return fSystemDividers; }

    void                  setStaffSpacing (float val)
                              { fStaffSpacing = val; }

    float                 getStaffSpacing () const
                              { return fStaffSpacing; }

    void                  setNewSystem ()
                              { fNewSystem = true; }

    Bool                  getNewSystem () const
                              { return fNewSystem; }

    void                  setNewPage ()
                              { fNewPage = true; }

    Bool                  getNewPage () const
                              { return fNewPage; }

    void                  setBlankPage (int val)
                              { fBlankPage = val; }

    int                   getBlankPage () const
                              { return fBlankPage; }

    void                  setPageNumber (int val)
                              { fPageNumber = val; }

    int                   getPageNumber () const
                              { return fPageNumber; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendStaffLayout (
                            const S_msrStaffLayout& staffLayout)
                              {
                                fStaffLayoutsList.push_back (staffLayout);
                              }

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
    void                  printFull (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrMeasure          fMusicXMLPrintLayoutUpLinkToMeasure;

    S_msrPageLayout       fPageLayout;

    S_msrSystemLayout     fSystemLayout;

    std::list<S_msrStaffLayout>
                          fStaffLayoutsList;

    S_msrSystemDividers   fSystemDividers; // JMI ??? v0.9.66

    float                 fStaffSpacing;

    Bool                  fNewSystem;
    Bool                  fNewPage;

    int                   fBlankPage;

    int                   fPageNumber;
};
typedef SMARTP<msrMusicXMLPrintLayout> S_msrMusicXMLPrintLayout;
EXP std::ostream& operator << (std::ostream& os, const S_msrMusicXMLPrintLayout& elt);


}


#endif // ___msrMusicXMLPrintLayouts___
