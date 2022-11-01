/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMusicXMLSpecifics___
#define ___msrMusicXMLSpecifics___

#include "msrElements.h"
#include "msrMeasureElements.h"

#include "msrBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrPageLayout : public msrElement
{
/*
<!--
  Page layout can be defined both in score-wide defaults
  and in the print element. Page margins are specified either
  for both even and odd pages, or via separate odd and even
  page number values. The type is not needed when used as
  part of a print element. If ignored when used in the
  defaults element, "both" is the default.
-->
<!ELEMENT page-layout ((page-height, page-width)?,
  (page-margins, page-margins?)?)>
<!ELEMENT page-height %layout-tenths;>
<!ELEMENT page-width %layout-tenths;>
<!ELEMENT page-margins (left-margin, right-margin,
  top-margin, bottom-margin)>
<!ATTLIST page-margins
    type (odd | even | both) #IMPLIED
>
*/

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPageLayout> create (
                            int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrPageLayout (
                            int inputLineNumber);

    virtual               ~msrPageLayout ();

  public:

    // set and get
    // ------------------------------------------------------

    // page size
    void                  setPageHeight (S_msrLength val)
                              { fPageHeight = val; }
    S_msrLength           getPageHeight () const
                              { return fPageHeight; }

    void                  setPageWidth (S_msrLength val)
                              { fPageWidth = val; }
    S_msrLength           getPageWidth () const
                              { return fPageWidth; }

    // margins groups
    void                  setOddMarginsGroup (
                            int               inputLineNumber,
                            S_msrMarginsGroup val);
    S_msrMarginsGroup     getOddMarginsGroup () const
                              { return fOddMarginsGroup; }

    void                  setEvenMarginsGroup (
                            int               inputLineNumber,
                            S_msrMarginsGroup val);
    S_msrMarginsGroup     getEvenMarginsGroup () const
                              { return fEvenMarginsGroup; }

    void                  setBothMarginsGroup (
                            int               inputLineNumber,
                            S_msrMarginsGroup val);
    S_msrMarginsGroup     getBothMarginsGroup () const
                              { return fBothMarginsGroup; }

  public:

    // public services
    // ------------------------------------------------------

    // LilyPond knows inner/outer margins and binding offset,
    // but no odd, even nor both margins settings,
    // so we need methods to obtain a single margin from
    // fOddMarginsGroup, fEvenMarginsGroup and fBothMarginsGroup
    S_msrMargin           getSingleLeftMargin () const;
    S_msrMargin           getSingleRightMargin () const;
    S_msrMargin           getSingleTopMargin () const;
    S_msrMargin           getSingleBottomMargin () const;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // page size
    S_msrLength           fPageHeight;
    S_msrLength           fPageWidth;

    // margins groups
    S_msrMarginsGroup     fOddMarginsGroup;
    S_msrMarginsGroup     fEvenMarginsGroup;
    S_msrMarginsGroup     fBothMarginsGroup;
};
typedef SMARTP<msrPageLayout> S_msrPageLayout;
EXP ostream& operator << (ostream& os, const S_msrPageLayout& elt);

//______________________________________________________________________________
class EXP msrSystemLayout : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSystemLayout> create (
                            int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSystemLayout (
                            int inputLineNumber);

    virtual               ~msrSystemLayout ();

  public:

    // set and get
    // ------------------------------------------------------

    // margins
    void                  setLeftMargin (S_msrMargin val)
                              { fLeftMargin = val; }
    S_msrMargin           getLeftMargin () const
                              { return fLeftMargin; }

    void                  setRightMargin (S_msrMargin val)
                              { fRightMargin = val; }
    S_msrMargin           getRightMargin () const
                              { return fRightMargin; }

    // distances
    void                  setSystemDistance (S_msrLength val)
                              { fSystemDistance = val; }
    S_msrLength           getSystemDistance () const
                              { return fSystemDistance; }

    void                  setTopSystemDistance (S_msrLength val)
                              { fTopSystemDistance = val; }
    S_msrLength           getTopSystemDistance () const
                              { return fTopSystemDistance; }

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

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // margins
    S_msrMargin           fLeftMargin;
    S_msrMargin           fRightMargin;

    // distances
    S_msrLength           fSystemDistance;
    S_msrLength           fTopSystemDistance;
};
typedef SMARTP<msrSystemLayout> S_msrSystemLayout;
EXP ostream& operator << (ostream& os, const S_msrSystemLayout& elt);

//______________________________________________________________________________
class EXP msrSystemDividers : public msrElement
{
/*
<!--
  The system-dividers element indicates the presence or
  absence of system dividers (also known as system separation
  marks) between systems displayed on the same page. Dividers
  on the left and right side of the page are controlled by
  the left-divider and right-divider elements respectively.
  The default vertical position is half the system-distance
  value from the top of the system that is below the divider.
  The default horizontal position is the left and right
  system margin, respectively.

  When used in the print element, the system-dividers element
  affects the dividers that would appear between the current
  system and the previous system.
-->
<!ELEMENT system-dividers (left-divider, right-divider)>
<!ELEMENT left-divider EMPTY>
<!ATTLIST left-divider
    %print-object;
    %print-style-align;
>
<!ELEMENT right-divider EMPTY>
<!ATTLIST right-divider
    %print-object;
    %print-style-align;
>

    <system-layout>
      <system-margins>
        <left-margin>0</left-margin>
        <right-margin>0</right-margin>
      </system-margins>
      <system-distance>39</system-distance>
      <top-system-distance>39</top-system-distance>
      <system-dividers>
        <left-divider print-object="yes"/>
        <right-divider print-object="no"/>
      </system-dividers>
    </system-layout>
*/

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSystemDividers> create (
                            int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSystemDividers (
                            int inputLineNumber);

    virtual               ~msrSystemDividers ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setLeftDivider (Bool val)
                              { fLeftDivider = val; }
    Bool                  getLeftDivider () const
                              { return fLeftDivider; }

    void                  setRightDivider (Bool val)
                              { fRightDivider = val; }
    Bool                  getRightDivider () const
                              { return fRightDivider; }

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

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    Bool                  fLeftDivider;
    Bool                  fRightDivider;
};
typedef SMARTP<msrSystemDividers> S_msrSystemDividers;
EXP ostream& operator << (ostream& os, const S_msrSystemDividers& elt);

//______________________________________________________________________________
class EXP msrStaffLayout : public msrElement
{
/*
<!--
  Staff layout includes the vertical distance from the bottom
  line of the previous staff in this system to the top line
  of the staff specified by the number attribute. The
  optional number attribute refers to staff numbers within
  the part, from top to bottom on the system. A value of 1
  is assumed if not present. When used in the defaults
  element, the values apply to all parts. This value is
  ignored for the first staff in a system.
-->
<!ELEMENT staff-layout (staff-distance?)>
<!ELEMENT staff-distance %layout-tenths;>
<!ATTLIST staff-layout
    number CDATA #IMPLIED
>
*/

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStaffLayout> create (
                            int inputLineNumber,
                            int staffNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrStaffLayout (
                            int inputLineNumber,
                            int staffNumber);

    virtual               ~msrStaffLayout ();

  public:

    // set and get
    // ------------------------------------------------------

    // staff number
    void                  setStaffNumber (int val)
                              { fStaffNumber = val; }
    int                   getStaffNumber () const
                              { return fStaffNumber; }

    // staff distance
    void                  setStaffDistance (S_msrLength val)
                              { fStaffDistance = val; }
    S_msrLength           getStaffDistance () const
                              { return fStaffDistance; }

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

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // staff number
    int                   fStaffNumber;

    // staff distance
    S_msrLength           fStaffDistance;
};
typedef SMARTP<msrStaffLayout> S_msrStaffLayout;
EXP ostream& operator << (ostream& os, const S_msrStaffLayout& elt);

//______________________________________________________________________________
class EXP msrMeasureLayout : public msrElement
{
/*
<!--
  Measure layout includes the horizontal distance from the
  previous measure. This value is only used for systems
  where there is horizontal whitespace in the middle of a
  system, as in systems with codas. To specify the measure
  width, use the width attribute of the measure element.
-->
<!ELEMENT measure-layout (measure-distance?)>
<!ELEMENT measure-distance %layout-tenths;>
*/

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasureLayout> create (
                            int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMeasureLayout (
                            int inputLineNumber);

    virtual               ~msrMeasureLayout ();

  public:

    // set and get
    // ------------------------------------------------------

    // measure distance
    void                  setMeasureDistance (S_msrLength val)
                              { fMeasureDistance = val; }
    S_msrLength           getMeasureDistance () const
                              { return fMeasureDistance; }

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

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // measure distance
    S_msrLength           fMeasureDistance;
};
typedef SMARTP<msrMeasureLayout> S_msrMeasureLayout;
EXP ostream& operator << (ostream& os, const S_msrMeasureLayout& elt);

//______________________________________________________________________________
class EXP msrPrintLayout : public msrMeasureElement
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

    static SMARTP<msrPrintLayout> create (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrPrintLayout (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure);

    virtual               ~msrPrintLayout ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setPageLayout (S_msrPageLayout pageLayout)
                              { fPageLayout = pageLayout; }

    S_msrPageLayout       getPageLayout () const
                              { return fPageLayout; }

    void                  setSystemLayout (S_msrSystemLayout systemLayout)
                              { fSystemLayout = systemLayout; }

    S_msrSystemLayout     getSystemLayout () const
                              { return fSystemLayout; }

    const list<S_msrStaffLayout>&
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
                            S_msrStaffLayout staffLayout)
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

    string                asString () const override;

    void                  print (ostream& os) const override;

    void                  printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_msrPageLayout       fPageLayout;

    S_msrSystemLayout     fSystemLayout;

    list<S_msrStaffLayout>
                          fStaffLayoutsList;

    S_msrSystemDividers   fSystemDividers; // JMI ???

    float                 fStaffSpacing;

    Bool                  fNewSystem;
    Bool                  fNewPage;

    int                   fBlankPage;

    int                   fPageNumber;
};
typedef SMARTP<msrPrintLayout> S_msrPrintLayout;
EXP ostream& operator << (ostream& os, const S_msrPrintLayout& elt);

//______________________________________________________________________________
class EXP msrLineWidth : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrLineWidthTypeKind {
      k_NoLineWidthTypeKind,
      kBeamLineWidth, kBracketLineWidth, kDashesLineWidth,
      kEnclosureLineWidth,
      kEndingLineWidth,
      kExtendLineWidth,
      kHeavyBarLineLineWidth, kLegerLineWidth, kLightBarLineLineWidthLineWidth,
      kOctaveShiftLineWidth,
      kPedalLineWidth,
      kSlurMiddleLineWidth, kSlurTipLineWidth,
      kStaffLineWidth, kStemLineWidthLineWidth,
      kTieMiddleLineWidth, kTieTipLineWidth,
      kTupletBracketLineWidth,
      kWedgeLineWidth
    };

    static string lineWidthTypeKindAsString (
      msrLineWidthTypeKind lineWidthTypeKind);

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrLineWidth> create (
                            int                  inputLineNumber,
                            msrLineWidthTypeKind lineWidthTypeKind,
                            S_msrLength          lineWidthValue);

  protected:

                          msrLineWidth (
                            int                  inputLineNumber,
                            msrLineWidthTypeKind lineWidthTypeKind,
                            S_msrLength          lineWidthValue);

    virtual               ~msrLineWidth ();

  public:

    // set and get
    // ------------------------------------------------------

    msrLineWidthTypeKind  getLineWidthTypeKind () const
                              { return fLineWidthTypeKind; }

    S_msrLength           getLineWidthValue () const
                              { return fLineWidthValue; }

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrLineWidthTypeKind  fLineWidthTypeKind;
    S_msrLength           fLineWidthValue;
};
typedef SMARTP<msrLineWidth> S_msrLineWidth;
EXP ostream& operator << (ostream& os, const S_msrLineWidth& elt);

//______________________________________________________________________________
class EXP msrNoteSize : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrNoteSizeTypeKind {
      k_NoNoteSizeTypeKind,
      kCueNoteSize, kGraceNoteSize, kLargeNoteSize
    };

    static string noteSizeTypeKindAsString (
      msrNoteSizeTypeKind noteSizeTypeKind);

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrNoteSize> create (
                            int                 inputLineNumber,
                            msrNoteSizeTypeKind noteSizeTypeKind,
                            float               noteSizeValue);


  protected:

                          msrNoteSize (
                            int                 inputLineNumber,
                            msrNoteSizeTypeKind noteSizeTypeKind,
                            float               noteSizeValue);

    virtual               ~msrNoteSize ();

  public:

    // set and get
    // ------------------------------------------------------

    msrNoteSizeTypeKind   getNoteSizeTypeKind () const
                              { return fNoteSizeTypeKind; }

    float                 getNoteSizeValue () const
                              { return fNoteSizeValue; }

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrNoteSizeTypeKind   fNoteSizeTypeKind;
    float                 fNoteSizeValue;
};
typedef SMARTP<msrNoteSize> S_msrNoteSize;
EXP ostream& operator << (ostream& os, const S_msrNoteSize& elt);

//______________________________________________________________________________
class EXP msrDistance : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrDistanceTypeKind {
      k_NoDistanceTypeKind,
      kHyphenDistance, kBeamDistance
    };

    static string distanceTypeKindAsString (
      msrDistanceTypeKind distanceTypeKind);

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDistance> create (
      int                 inputLineNumber,
      msrDistanceTypeKind distanceTypeKind,
      S_msrLength         distanceValue);

  protected:

    msrDistance (
      int                  inputLineNumber,
      msrDistanceTypeKind  distanceTypeKind,
      S_msrLength          distanceValue);

    virtual               ~msrDistance ();

  public:

    // set and get
    // ------------------------------------------------------

    msrDistanceTypeKind   getDistanceTypeKind () const
                              { return fDistanceTypeKind; }

    S_msrLength           getDistanceValue () const
                              { return fDistanceValue; }

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrDistanceTypeKind   fDistanceTypeKind;
    S_msrLength           fDistanceValue;
};
typedef SMARTP<msrDistance> S_msrDistance;
EXP ostream& operator << (ostream& os, const S_msrDistance& elt);

//______________________________________________________________________________
class EXP msrGlyph : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrGlyphTypeKind {
      k_NoGlyphTypeKind,
      kQuarterRestGlyph,
      kGClefOttavaBassaGlyph,
      kCClefGlyph, kFClefGlyph,
      kClefPercussionGlyph,
      kOctaveShiftUp8Glyph, kOctaveShiftDown8Glyph,
      kOctaveShiftContinue8Glyph, kOctaveShiftDown15Glyph,
      kOctaveShiftUp15Glyph, kOctaveShiftContinue15Glyph,
      kOctaveShiftDown22Glyph, kOctaveShiftUp22Glyph,
      kOctaveShiftContinue22Glyph
    };

    static string glyphTypeKindAsString (
      msrGlyphTypeKind GlyphTypeKind);

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrGlyph> create (
                            int              inputLineNumber,
                            msrGlyphTypeKind glyphTypeKind,
                            const string&    glyphValue);

  protected:

                          msrGlyph (
                            int              inputLineNumber,
                            msrGlyphTypeKind glyphTypeKind,
                            const string&    glyphValue);

    virtual               ~msrGlyph ();

  public:

    // set and get
    // ------------------------------------------------------

    msrGlyphTypeKind      getGlyphTypeKind () const
                              { return fGlyphTypeKind; }

    string                getGlyphValue () const
                              { return fGlyphValue; }

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrGlyphTypeKind      fGlyphTypeKind;
    string                fGlyphValue;
};
typedef SMARTP<msrGlyph> S_msrGlyph;
EXP ostream& operator << (ostream& os, const S_msrGlyph& elt);

//______________________________________________________________________________
class EXP msrOtherAppearance : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrOtherAppearanceTypeKind { // JMI which values???
      k_NoOtherAppearanceTypeKind
    };

    static string otherAppearanceTypeKindAsString (
      msrOtherAppearanceTypeKind otherAppearanceTypeKind);

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrOtherAppearance> create (
                            int                        inputLineNumber,
                            msrOtherAppearanceTypeKind otherAppearanceTypeKind,
                            const string&              otherAppearanceValue);

  protected:

                          msrOtherAppearance (
                            int                        inputLineNumber,
                            msrOtherAppearanceTypeKind otherAppearanceTypeKind,
                            const string&              otherAppearanceValue);

    virtual               ~msrOtherAppearance ();

  public:

    // set and get
    // ------------------------------------------------------

    msrOtherAppearanceTypeKind
                          getOtherAppearanceTypeKind () const
                              { return fOtherAppearanceTypeKind; }

    string                getOtherAppearanceValue () const
                              { return fOtherAppearanceValue; }

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrOtherAppearanceTypeKind
                          fOtherAppearanceTypeKind;
    string                fOtherAppearanceValue;
};
typedef SMARTP<msrOtherAppearance> S_msrOtherAppearance;
EXP ostream& operator << (ostream& os, const S_msrOtherAppearance& elt);

//______________________________________________________________________________
class EXP msrAppearance : public msrElement
{
/*
<!--
  The appearance element controls general graphical
  settings for the music's final form appearance on a
  printed page of display. This includes support
  for line widths, definitions for note sizes, and standard
  distances between notation elements, plus an extension
  element for other aspects of appearance.

  The line-width element indicates the width of a line type
  in tenths. The type attribute defines what type of line is
  being defined. Values include beam, bracket, dashes,
  enclosure, ending, extend, heavy barLine, leger,
  light barLine, octave shift, pedal, slur middle, slur tip,
  staff, stem, tie middle, tie tip, tuplet bracket, and
  wedge. The text content is expressed in tenths.

  The note-size element indicates the percentage of the
  regular note size to use for notes with a cue and large
  size as defined in the type element. The grace-cue type
  is used for notes of grace-cue size. The grace type is
  used for notes of cue size that include a grace element.
  The cue type is used for all other notes with cue size,
  whether defined explicitly or implicitly via a cue element.
  The large type is used for notes of large size. The text
  content represent the numeric percentage. A value of 100
  would be identical to the size of a regular note as defined
  by the music font.

  The distance element represents standard distances between
  notation elements in tenths. The type attribute defines what
  type of distance is being defined. Values include hyphen
  (for hyphens in lyrics) and beam.

  The glyph element represents what SMuFL glyph should be used
  for different variations of symbols that are semantically
  identical. The type attribute specifies what type of glyph
  is being defined. The element value specifies what
  SMuFL glyph to use, including recommended stylistic
  regulars.

  Glyph type attribute values include quarter-rest,
  g-clef-ottava-bassa, c-clef, f-clef, percussion-clef,
  octave-shift-up-8, octave-shift-down-8,
  octave-shift-continue-8, octave-shift-down-15,
  octave-shift-up-15, octave-shift-continue-15,
  octave-shift-down-22, octave-shift-up-22, and
  octave-shift-continue-22. A quarter-rest type specifies the
  glyph to use when a note has a rest element and a type value
  of quarter. The c-clef, f-clef, and percussion-clef types
  specify the glyph to use when a clef sign element value is C,
  F, or percussion respectively. The g-clef-ottava-bassa type
  specifies the glyph to use when a clef sign element value is
  G and the clef-octave-change element value is -1. The
  octave-shift types specify the glyph to use when an
  octave-shift type attribute value is up, down, or continue
  and the octave-shift size attribute value is 8, 15, or 22.

  The SMuFL glyph name should match the type. For instance,
  a type of quarter-rest would use values restQuarter,
  restQuarterOld, or restQuarterZ. A type of g-clef-ottava-bassa
  would use values gClef8vb, gClef8vbOld, or gClef8vbCClef. A
  type of octave-shift-up-8 would use values ottava, ottavaBassa,
  ottavaBassaBa, ottavaBassaVb, or octaveBassa.

  The other-appearance element is used to define any
  graphical settings not yet in the current version of the
  MusicXML format. This allows extended representation,
  though without application interoperability.
-->
<!ELEMENT appearance
  (line-width*, note-size*, distance*, glyph*,
   other-appearance*)>
<!ELEMENT line-width %layout-tenths;>
<!ATTLIST line-width
    type CDATA #REQUIRED
>
<!ELEMENT note-size (#PCDATA)>
<!ATTLIST note-size
    type (cue | grace | grace-cue | large) #REQUIRED
>
<!ELEMENT distance %layout-tenths;>
<!ATTLIST distance
    type CDATA #REQUIRED
>
<!ELEMENT glyph (#PCDATA)>
<!ATTLIST glyph
    type CDATA #REQUIRED
>
<!ELEMENT other-appearance (#PCDATA)>
<!ATTLIST other-appearance
    type CDATA #REQUIRED
>
*/

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrAppearance> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrAppearance (
      int inputLineNumber);

    virtual               ~msrAppearance ();

  public:

    // set and get
    // ------------------------------------------------------

    const list<S_msrLineWidth>&
                          getLineWidthsList () const
                              { return fLineWidthsList; }

    const list<S_msrNoteSize>&
                          getNoteSizesList () const
                              { return fNoteSizesList; }

    const list<S_msrDistance>&
                          getDistancesList () const
                              { return fDistancesList; }

    const list<S_msrGlyph>&
                          getGlyphsList () const
                              { return fGlyphsList; }

    const list<S_msrOtherAppearance>&
                          getOtherAppearancesList () const
                              { return fOtherAppearancesList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendLineWidth (S_msrLineWidth lineWidth)
                              { fLineWidthsList.push_back (lineWidth); }

    void                  appendNoteSize (S_msrNoteSize noteSize)
                              { fNoteSizesList.push_back (noteSize); }

    void                  appendDistance (S_msrDistance distance)
                              { fDistancesList.push_back (distance); }

    void                  appendGlyph (S_msrGlyph glyph)
                              { fGlyphsList.push_back (glyph); }

    void                  appendOtherAppearance (
                            S_msrOtherAppearance otherAppearance)
                              {
                                fOtherAppearancesList.push_back (otherAppearance);
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

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    list<S_msrLineWidth>  fLineWidthsList;

    list<S_msrNoteSize>   fNoteSizesList;

    list<S_msrDistance>   fDistancesList;

    list<S_msrGlyph>      fGlyphsList;

    list<S_msrOtherAppearance>
                          fOtherAppearancesList;
};
typedef SMARTP<msrAppearance> S_msrAppearance;
EXP ostream& operator << (ostream& os, const S_msrAppearance& elt);


} // namespace MusicFormats


#endif
