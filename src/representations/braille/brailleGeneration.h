/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___brailleGeneration___
#define ___brailleGeneration___

#include "bsrCellsLists.h"

#include "bsrMusicHeadings.h"


namespace MusicFormats
{

//______________________________________________________________________________
enum class bsrUTFKind {
  kUTF8, kUTF16
};

std::string bsrUTFKindAsString (
  bsrUTFKind UTFKind);

std::ostream& operator << (std::ostream& os, const bsrUTFKind& elt);

enum class bsrByteOrderingKind {
  kByteOrderingNone,
  kByteOrderingBigEndian, kByteOrderingSmallEndian
};

std::string bsrByteOrderingKindAsString (
  bsrByteOrderingKind byteOrderingKind);

std::ostream& operator << (std::ostream& os, const bsrByteOrderingKind& elt);

//______________________________________________________________________________
// Bytes Encoding Marks (BOM)
const std::string
  kBOM_UTF_32_BigEndian =
    "\x00\x00\xFE\xFF", // UTF-32, big-endian
  kBOM_UTF_32_LittleEndian =
    "\xFF\xFE\x00\x00", // UTF-32, little-endian
  kBOM_UTF_16_BigEndian =
    "\xFE\xFF",         // UTF-16, big-endian
  kBOM_UTF_16_LittleEndian =
    "\xFF\xFE",         // UTF-16, little-endian
  kBOM_UTF_8 =
    "\xEF\xBB\xBF";     // UTF-8

//______________________________________________________________________________
class EXP bsrBrailleGenerator : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

/* this class is purely virtual
    static SMARTP<bsrBrailleGenerator> create (
                            std::ostream& brailleOutputStream);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrBrailleGenerator (
                            std::ostream& brailleOutputStream);

    virtual               ~bsrBrailleGenerator ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    virtual void          generateCodeForBrailleCell (
                            bsrCellKind cellKind) = 0;

    void                  generateCodeForCellsList (
                            const S_bsrCellsList& cellsList);

    virtual void          generateCodeForMusicHeading (
                            const S_bsrMusicHeading& musicHeading);

    virtual void          generateCodeForLineContents (
                            const S_bsrLineContents& lineContents);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    virtual void          print (std::ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    std::ostream&              fBrailleOutputStream;
};
typedef SMARTP<bsrBrailleGenerator> S_bsrBrailleGenerator;
EXP std::ostream& operator << (std::ostream& os, const S_bsrBrailleGenerator& elt);

//______________________________________________________________________________
class EXP bsrAsciiBrailleGenerator : public bsrBrailleGenerator
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrAsciiBrailleGenerator> create (
                            std::ostream& brailleOutputStream);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrAsciiBrailleGenerator (
                            std::ostream& brailleOutputStream);

    virtual               ~bsrAsciiBrailleGenerator ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    virtual void          generateCodeForBrailleCell (
                            bsrCellKind cellKind) override;

    static void           writeTestData (std::ostream& os);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<bsrAsciiBrailleGenerator> S_bsrAsciiBrailleGenerator;
EXP std::ostream& operator << (std::ostream& os, const S_bsrAsciiBrailleGenerator& elt);

//______________________________________________________________________________
class EXP bsrUTF8BrailleGenerator : public bsrBrailleGenerator
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrUTF8BrailleGenerator> create (
                            bsrByteOrderingKind byteOrderingKind,
                            std::ostream&            brailleOutputStream);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrUTF8BrailleGenerator (
                            bsrByteOrderingKind byteOrderingKind,
                            std::ostream&            brailleOutputStream);

    virtual               ~bsrUTF8BrailleGenerator ();

  public:

    // set and get
    // ------------------------------------------------------

    bsrByteOrderingKind   getByteOrderingKind () const
                              { return fByteOrderingKind; }

  public:

    // public services
    // ------------------------------------------------------

    virtual void          generateCodeForBrailleCell (
                            bsrCellKind cellKind) override;

    static void           writeTestData (std::ostream& os);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    bsrByteOrderingKind   fByteOrderingKind;
};
typedef SMARTP<bsrUTF8BrailleGenerator> S_bsrUTF8BrailleGenerator;
EXP std::ostream& operator << (std::ostream& os, const S_bsrUTF8BrailleGenerator& elt);

//______________________________________________________________________________
class EXP bsrUTF8DebugBrailleGenerator : public bsrUTF8BrailleGenerator
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrUTF8DebugBrailleGenerator> create (
                            bsrByteOrderingKind byteOrderingKind,
                            std::ostream&            brailleOutputStream);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrUTF8DebugBrailleGenerator (
                            bsrByteOrderingKind byteOrderingKind,
                            std::ostream&            brailleOutputStream);

    virtual               ~bsrUTF8DebugBrailleGenerator ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  generateCodeForMusicHeading (
                            const S_bsrMusicHeading& musicHeading) override;

    void                  generateCodeForLineContents (
                            const S_bsrLineContents& lineContents) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<bsrUTF8DebugBrailleGenerator> S_bsrUTF8DebugBrailleGenerator;
EXP std::ostream& operator << (std::ostream& os, const S_bsrUTF8DebugBrailleGenerator& elt);

//______________________________________________________________________________
class EXP bsrUTF16BigEndianBrailleGenerator : public bsrBrailleGenerator
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrUTF16BigEndianBrailleGenerator> create (
                            bsrByteOrderingKind byteOrderingKind,
                            std::ostream&            brailleOutputStream);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrUTF16BigEndianBrailleGenerator (
                            bsrByteOrderingKind byteOrderingKind,
                            std::ostream&            brailleOutputStream);

    virtual               ~bsrUTF16BigEndianBrailleGenerator ();

  public:

    // set and get
    // ------------------------------------------------------

    bsrByteOrderingKind   getByteOrderingKind () const
                              { return fByteOrderingKind; }

  public:

    // public services
    // ------------------------------------------------------

    virtual void          generateCodeForBrailleCell (
                            bsrCellKind cellKind) override;

    static void           writeTestData (std::ostream& os);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    bsrByteOrderingKind   fByteOrderingKind;
};
typedef SMARTP<bsrUTF16BigEndianBrailleGenerator> S_bsrUTF16BigEndianBrailleGenerator;
EXP std::ostream& operator << (std::ostream& os, const S_bsrUTF16BigEndianBrailleGenerator& elt);

//______________________________________________________________________________
class EXP bsrUTF16SmallEndianBrailleGenerator : public bsrBrailleGenerator
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrUTF16SmallEndianBrailleGenerator> create (
                            bsrByteOrderingKind byteOrderingKind,
                            std::ostream&            brailleOutputStream);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrUTF16SmallEndianBrailleGenerator (
                            bsrByteOrderingKind byteOrderingKind,
                            std::ostream&            brailleOutputStream);

    virtual               ~bsrUTF16SmallEndianBrailleGenerator ();

  public:

    // set and get
    // ------------------------------------------------------

    bsrByteOrderingKind   getByteOrderingKind () const
                              { return fByteOrderingKind; }

  public:

    // public services
    // ------------------------------------------------------

    virtual void          generateCodeForBrailleCell (
                            bsrCellKind cellKind) override;

    static void           writeTestData (std::ostream& os);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    bsrByteOrderingKind   fByteOrderingKind;
};
typedef SMARTP<bsrUTF16SmallEndianBrailleGenerator> S_bsrUTF16SmallEndianBrailleGenerator;
EXP std::ostream& operator << (std::ostream& os, const S_bsrUTF16SmallEndianBrailleGenerator& elt);


}


#endif // ___brailleGeneration___
