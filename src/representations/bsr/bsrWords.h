#ifndef ___bsrWords___
#define ___bsrWords___

#include "bsrLineContentsElements.h"

#include "bsrCellsLists.h"


namespace MusicFormats
{

//______________________________________________________________________________
// brailling characters and words
S_bsrCellsList brailleCharacter (
  int  inputLineNumber,
  char ch);

S_bsrCellsList brailleWord (
  int           inputLineNumber,
  const std::string& str);

//______________________________________________________________________________
class EXP bsrWords : public bsrLineContentsElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrWords> create (
                            int           inputLineNumber,
                            const std::string& wordContents);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrWords (
                            int           inputLineNumber,
                            const std::string& wordContents);

    virtual               ~bsrWords ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getWordContents () const
                              { return fWordContents; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const override
                              { return fWordCellsList; }

    int                   fetchCellsNumber () const override;

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

    std::string           asDebugString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::string           fWordContents;

    S_bsrCellsList        fWordCellsList;
};
typedef SMARTP<bsrWords> S_bsrWords;
EXP std::ostream& operator << (std::ostream& os, const S_bsrWords& elt);


}


#endif
