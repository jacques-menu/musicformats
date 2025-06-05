/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrHeaders___
#define ___lpsrHeaders___

#include "mfIndentedTextOutput.h"

#include "lpsrElements.h"

#include "msrIdentification.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP lpsrHeader : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrHeader> create (
                            const mfInputLineNumber& inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrHeader (
                            const mfInputLineNumber& inputLineNumber);

    virtual               ~lpsrHeader ();

  public:

    // set and get
    // ------------------------------------------------------

    // identification

    void                  setHeaderIdentification (S_msrIdentification value)
                              { fHeaderIdentification = value; }

    S_msrIdentification   getHeaderIdentification () const
                              { return fHeaderIdentification; }

    // LilyPond informations

    // centered

    void                  setLilypondDedication (std::string value)
                              { fLilypondDedication = value; }

    std::string           getLilypondDedication () const
                              { return fLilypondDedication; }

    void                  setLilypondPiece (std::string value)
                              { fLilypondPiece = value; }

    std::string           getLilypondPiece () const
                              { return fLilypondPiece; }

    void                  setLilypondOpus (std::string value)
                              { fLilypondOpus = value; }

    std::string           getLilypondOpus () const
                              { return fLilypondOpus; }

    void                  setLilypondTitle (std::string value)
                              { fLilypondTitle = value; }

    std::string           getLilypondTitle () const
                              { return fLilypondTitle; }

    void                  setLilypondSubTitle (std::string value)
                              { fLilypondSubTitle = value; }

    std::string           getLilypondSubTitle () const
                              { return fLilypondSubTitle; }

    void                  setLilypondSubSubTitle (std::string value)
                              { fLilypondSubSubTitle = value; }

    std::string           getLilypondSubSubTitle () const
                              { return fLilypondSubSubTitle; }

    // evenly spread on one line
    // "instrument" also appears on following pages

    void                  setLilypondInstrument (std::string value)
                              { fLilypondInstrument = value; }

    std::string           getLilypondInstrument () const
                              { return fLilypondInstrument; }

    // at opposite ends of the same line

    void                  setLilypondMeter (std::string value)
                              { fLilypondMeter = value; }

    std::string           getLilypondMeter () const
                              { return fLilypondMeter; }

    // centered at the bottom of the first page

    void                  setLilypondCopyright (std::string value)
                              { fLilypondCopyright = value; }

    std::string           getLilypondCopyright () const
                              { return fLilypondCopyright; }

    // centered at the bottom of the last page

    void                  setLilypondTagline (std::string value)
                              { fLilypondTagline = value; }

    std::string           getLilypondTagline () const
                              { return fLilypondTagline; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    size_t                maxLilypondVariablesNamesLength ();

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

  private:

    // private fields
    // ------------------------------------------------------

    // identification

    S_msrIdentification   fHeaderIdentification;

    // Lilypond informations

    // centered
    std::string           fLilypondDedication;

    std::string           fLilypondPiece;
    std::string           fLilypondOpus;

    std::string           fLilypondTitle;
    std::string           fLilypondSubTitle;
    std::string           fLilypondSubSubTitle;

    // evenly spread on one line
    // "instrument" also appears on following pages
    std::string           fLilypondInstrument;

    // at opposite ends of the same line
    std::string           fLilypondMeter;

    // centered at the bottom of the first page
    std::string           fLilypondCopyright;

    // centered at the bottom of the last page
    std::string           fLilypondTagline;
};
typedef SMARTP<lpsrHeader> S_lpsrHeader;
EXP std::ostream& operator << (std::ostream& os, const S_lpsrHeader& elt);
EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& os, const S_lpsrHeader& elt);


}


#endif // ___lpsrHeaders___
