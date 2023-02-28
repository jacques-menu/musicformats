/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfStaticSettings.h"

#include "mfStringsHandling.h"

#include "oahOah.h"

#include "msdl2msrOah.h"

#include "msrOah.h"
#include "msr2lpsrOah.h"

#include "msdlWae.h"

#include "msdlKeywords.h"
#include "msdlTokens.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
std::string msdlTokenKindAsString (
  msdlTokenKind tokenKind)
{
  std::stringstream ss;

  switch (tokenKind) {
    case msdlTokenKind::kToken_UNKNOWN:
      ss << "*noToken*";
      break;

    case msdlTokenKind::k_TokenEOF:
      ss << "tokenEOF";
      break;

    case msdlTokenKind::k_TokenMalformed:
      ss << "*malformedToken*";
      break;

    // separators, for use by MSDL whole input translation
    // ------------------------------------

    case msdlTokenKind::kTokenSpace:
      ss << "tokenSpace";
      break;
    case msdlTokenKind::kTokenTabSeparator:
      ss << "tokenTab";
      break;
    case msdlTokenKind::kTokenCarriageReturn:
      ss << "tokenCarriageReturn";
      break;

    case msdlTokenKind::kTokenEndOfLine:
      ss << "tokenEndOfLine";
      break;

    case msdlTokenKind::kTokenParenthesizedComment:
      ss << "tokenParenthesizedComment";
      break;

    case msdlTokenKind::kTokenCommentToEndOfLine:
      ss << "tokenCommentToEndOfLine";
      break;

    // language-independent tokens
    // ------------------------------------

    case msdlTokenKind::kTokenEqualSign:
      ss << "tokenEqualSign";
      break;

    case msdlTokenKind::kTokenComma:
      ss << "tokenComma";
      break;
    case msdlTokenKind::kTokenColon:
      ss << "tokenColon";
      break;
    case msdlTokenKind::kTokenSemiColon:
      ss << "tokenSemiColon";
      break;

    case msdlTokenKind::kTokenPlus:
      ss << "tokenPlus";
      break;
    case msdlTokenKind::kTokenMinus:
      ss << "tokenMinus";
      break;
    case msdlTokenKind::kTokenStar:
      ss << "tokenStar";
      break;
    case msdlTokenKind::kTokenSlash:
      ss << "tokenSlash";
      break;
    case msdlTokenKind::kTokenPercent:
      ss << "tokenPercent";
      break;

    case msdlTokenKind::kTokenConcat:
      ss << "tokenConcat";
      break;

    case msdlTokenKind::kTokenQuestionMark:
      ss << "tokenQuestionMark";
      break;

    case msdlTokenKind::kTokenTilda:
      ss << "tokenTilda";
      break;
    case msdlTokenKind::kTokenLeftAngle:
      ss << "tokenLeftAngle";
      break;
    case msdlTokenKind::kTokenRightAngle:
      ss << "tokenRightAngle";
      break;

    case msdlTokenKind::kTokenLeftParenthesis:
      ss << "tokenLeftParenthesis";
      break;
    case msdlTokenKind::kTokenRightParenthesis:
      ss << "tokenRightParenthesis";
      break;

    case msdlTokenKind::kTokenLeftBracket:
      ss << "tokenLeftBracket";
      break;
    case msdlTokenKind::kTokenRightBracket:
      ss << "tokenRightBracket";
      break;

    case msdlTokenKind::kTokenQuote:
      ss << "tokenQuote";
      break;
    case msdlTokenKind::kTokenDot:
      ss << "tokenDot";
      break;

    case msdlTokenKind::kTokenMeasure:
      ss << "tokenMeasure";
      break;
    case msdlTokenKind::kTokenDoubleBar:
      ss << "tokenDoubleBar";
      break;
    case msdlTokenKind::kTokenFinalBar:
      ss << "tokenFinalBar";
      break;

    case msdlTokenKind::kTokenRepeatStart:
      ss << "tokenRepeatStart";
      break;
    case msdlTokenKind::kTokenRepeatEnd:
      ss << "tokenRepeatEnd";
      break;

    case msdlTokenKind::kTokenInteger:
      ss << "tokenInteger";
      break;
//    case msdlTokenKind::kTokenDouble:
//      ss << "tokenDouble";
//      break;

    case msdlTokenKind::kTokenString:
      ss << "tokenString";
      break;

    case msdlTokenKind::kTokenName:
      ss << "tokenName";
      break;
    case msdlTokenKind::kTokenIdentifier:
      ss << "tokeIdentifier";
      break;

    // language-dependent keywords
    // ------------------------------------

    case msdlTokenKind::kTokenTitle:
      ss << "tokenTitle";
      break;
    case msdlTokenKind::kTokenComposer:
      ss << "tokenComposer";
      break;
    case msdlTokenKind::kTokenOpus:
      ss << "tokenOpus";
      break;

    case msdlTokenKind::kTokenPitches:
      ss << "tokenPitches";
      break;
    case msdlTokenKind::kTokenOctaves:
      ss << "tokenOctaves";
      break;

    case msdlTokenKind::kTokenAnacrusis:
      ss << "tokenAnacrusis";
      break;

    case msdlTokenKind::kTokenBook:
      ss << "tokenBook";
      break;
    case msdlTokenKind::kTokenScore:
      ss << "tokenScore";
      break;
    case msdlTokenKind::kTokenPartGroup:
      ss << "tokenPartGroup";
      break;
    case msdlTokenKind::kTokenPart:
      ss << "tokenPart";
      break;
    case msdlTokenKind::kTokenMusic:
      ss << "tokenMusic";
      break;
    case msdlTokenKind::kTokenFragment:
      ss << "tokenFragment";
      break;

    case msdlTokenKind::kTokenClef:
      ss << "tokenClef";
      break;

    case msdlTokenKind::kTokenKey:
      ss << "tokenKey";
      break;
    case msdlTokenKind::kTokenTreble:
      ss << "tokenTreble";
      break;
    case msdlTokenKind::kTokenSoprano:
      ss << "tokenSoprano";
      break;
    case msdlTokenKind::kTokenAlto:
      ss << "tokenAlto";
      break;
    case msdlTokenKind::kTokenTenor:
      ss << "tokenTenor";
      break;
    case msdlTokenKind::kTokenBaryton:
      ss << "tokenBaryton";
      break;
    case msdlTokenKind::kTokenBass:
      ss << "tokenBass";
      break;
    case msdlTokenKind::kTokenTab:
      ss << "tokenTab";
      break;

    case msdlTokenKind::kTokenTime:
      ss << "tokenTime";
      break;
  } // switch

  return ss.str ();
}

std::string msdlTokenDescriptionKindAsString (
  msdlTokenDescriptionKind tokenDescriptionKind)
{
  std::string result;

  switch (tokenDescriptionKind) {
    case msdlTokenDescriptionKind::kTokenKeyword:
      result = "tokenDescriptionKeyword";
      break;
    case msdlTokenDescriptionKind::kTokenInteger:
      result = "tokenDescriptionInteger";
      break;
    case msdlTokenDescriptionKind::kTokenDouble:
      result = "tokenDescriptionDouble";
      break;
    case msdlTokenDescriptionKind::kTokenCharacter:
      result = "tokenDescriptionCharacter";
    case msdlTokenDescriptionKind::kTokenString:
      result = "tokenDescriptionString";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msdlTokenDescriptionKind& elt)
{
  os << msdlTokenDescriptionKindAsString (elt);
  return os;
}

std::string msdlTokenKindAsMsdlString (
  msdlTokenKind            tokenKind,
  msdlKeywordsLanguageKind languageKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsdl2msrOahGroup->getTraceTokens ()) {
    std::stringstream ss;

    ss <<
      "==> msdlTokenKindAsMsdlString()" <<
      ", tokenKind" << ": " << msdlTokenKindAsString (tokenKind) <<
      ", languageKind: " << msdlKeywordsLanguageKindAsString (languageKind) <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string result;

  switch (tokenKind) {
    case msdlTokenKind::kToken_UNKNOWN:
      result = "*NoToken*";
      break;

    case msdlTokenKind::k_TokenEOF:
      result = "EOF";
      break;

    case msdlTokenKind::k_TokenMalformed:
      result = "*TokenMalformed*";
      break;

    // separators, for use by MSDL whole input translation
    // ------------------------------------

    case msdlTokenKind::kTokenSpace:
      result = "SPACE";
      break;
    case msdlTokenKind::kTokenTabSeparator:
      result = "TAB";
      break;
    case msdlTokenKind::kTokenCarriageReturn:
      result = "CarriageReturn";
      break;

    case msdlTokenKind::kTokenEndOfLine:
      result = "EndOfLine";
      break;

    case msdlTokenKind::kTokenParenthesizedComment:
      result =
        "aParenthesizedPercentComment";
      result += "aParenthesizedStartComment";
      break;

    case msdlTokenKind::kTokenCommentToEndOfLine:
      result =
        "aPercentCommentToEndOfLine";
      result += "aStartCommentToEndOfLine";
      break;

    // language-independent tokens
    // ------------------------------------

    case msdlTokenKind::kTokenEqualSign:
      result = "=";
      break;

    case msdlTokenKind::kTokenComma:
      result = ",";
      break;
    case msdlTokenKind::kTokenColon:
      result = ":";
      break;
    case msdlTokenKind::kTokenSemiColon:
      result = ";";
      break;

    case msdlTokenKind::kTokenPlus:
      result = "+";
      break;
    case msdlTokenKind::kTokenMinus:
      result = "-";
      break;
    case msdlTokenKind::kTokenStar:
      result = "*";
      break;
    case msdlTokenKind::kTokenSlash:
      result = '/';
      break;
    case msdlTokenKind::kTokenPercent:
      result = "%";
      break;

    case msdlTokenKind::kTokenConcat:
      result = "!!";
      break;

    case msdlTokenKind::kTokenQuestionMark:
      result = "?";
      break;

    case msdlTokenKind::kTokenTilda:
      result = "~";
      break;

    case msdlTokenKind::kTokenLeftAngle:
      result = "<";
      break;
    case msdlTokenKind::kTokenRightAngle:
      result = ">";
      break;

    case msdlTokenKind::kTokenLeftParenthesis:
      result = "(";
      break;
    case msdlTokenKind::kTokenRightParenthesis:
      result = ")";
      break;

    case msdlTokenKind::kTokenLeftBracket:
      result = "{";
      break;
    case msdlTokenKind::kTokenRightBracket:
      result = "}";
      break;

    case msdlTokenKind::kTokenQuote:
      result = "'";
      break;
    case msdlTokenKind::kTokenDot:
      result = ".";
      break;

    case msdlTokenKind::kTokenMeasure:
      result = "|";
      break;
    case msdlTokenKind::kTokenDoubleBar:
      result = "||";
      break;
    case msdlTokenKind::kTokenFinalBar:
      result = "|||";
      break;

    case msdlTokenKind::kTokenRepeatStart:
      result = "|||:";
      break;
    case msdlTokenKind::kTokenRepeatEnd:
      result = ":|||";
      break;

    case msdlTokenKind::kTokenInteger:
      result = "<integer>";
      break;
//    case msdlTokenKind::kTokenDouble:
//      result = "<double>";
//      break;

    case msdlTokenKind::kTokenString:
      result = "<std::string>";
      break;

    case msdlTokenKind::kTokenName:
      result = "<name>";
      break;
    case msdlTokenKind::kTokenIdentifier:
      result = "<identifier>";
      break;

    // language-dependent keywords
    // ------------------------------------

    default:
      {
        msdlKeywordKind
          keywordKind =
            msdlKeywordKindFromTokenKind (
              tokenKind);

        result =
          msdlKeywordKindAsMsdlString (
            languageKind,
            keywordKind);
      }
  } // switch

  return result;
}

msdlTokenKind msdlTokenKindFromKeywordKind (
  msdlKeywordKind keywordKind)
{
  msdlTokenKind result = msdlTokenKind::kToken_UNKNOWN;

  switch (keywordKind) {
    case msdlKeywordKind::kKeyword_UNKNOWN:
      result = msdlTokenKind::kToken_UNKNOWN;
      break;

    case msdlKeywordKind::kKeywordTitle:
      result = msdlTokenKind::kTokenTitle;
      break;
    case msdlKeywordKind::kKeywordComposer:
      result = msdlTokenKind::kTokenComposer;
      break;
    case msdlKeywordKind::kKeywordOpus:
      result = msdlTokenKind::kTokenOpus;
      break;

    case msdlKeywordKind::kKeywordPitches:
      result = msdlTokenKind::kTokenPitches;
      break;
    case msdlKeywordKind::kKeywordOctaves:
      result = msdlTokenKind::kTokenOctaves;
      break;

    case msdlKeywordKind::kKeywordBook:
      result = msdlTokenKind::kTokenBook;
      break;
    case msdlKeywordKind::kKeywordScore:
      result = msdlTokenKind::kTokenScore;
      break;
    case msdlKeywordKind::kKeywordPartGroup:
      result = msdlTokenKind::kTokenPartGroup;
      break;
    case msdlKeywordKind::kKeywordPart:
      result = msdlTokenKind::kTokenPart;
      break;
    case msdlKeywordKind::kKeywordMusic:
      result = msdlTokenKind::kTokenMusic;
      break;
    case msdlKeywordKind::kKeywordFragment:
      result = msdlTokenKind::kTokenFragment;
      break;

    case msdlKeywordKind::kKeywordAnacrusis:
      result = msdlTokenKind::kTokenAnacrusis;
      break;

    case msdlKeywordKind::kKeywordClef:
      result = msdlTokenKind::kTokenClef;
      break;
    case msdlKeywordKind::kKeywordTreble:
      result = msdlTokenKind::kTokenTreble;
      break;
    case msdlKeywordKind::kKeywordSoprano:
      result = msdlTokenKind::kTokenSoprano;
      break;
    case msdlKeywordKind::kKeywordAlto:
      result = msdlTokenKind::kTokenAlto;
      break;
    case msdlKeywordKind::kKeywordTenor:
      result = msdlTokenKind::kTokenTenor;
      break;
    case msdlKeywordKind::kKeywordBaryton:
      result = msdlTokenKind::kTokenBaryton;
      break;
    case msdlKeywordKind::kKeywordBass:
      result = msdlTokenKind::kTokenBass;
      break;
    case msdlKeywordKind::kKeywordTab:
      result = msdlTokenKind::kTokenTab;
      break;

    case msdlKeywordKind::kKeywordKey:
      result = msdlTokenKind::kTokenKey;
      break;

    case msdlKeywordKind::kKeywordTime:
      result = msdlTokenKind::kTokenTime;
      break;
  } // switch

  return result;
}

std::string existingTokensInLanguage (
  msdlKeywordsLanguageKind keywordsLanguageKind,
  size_t                   namesListMaxLength)
{
  std::string result;

  std::map<msdlKeywordKind, std::string> *keywordsNamesMapPTR = nullptr;

  // select the relevant keywords names map
  switch (keywordsLanguageKind) {
    case msdlKeywordsLanguageKind::kKeywordsLanguage_UNKNOWN:
      // should not occur
      break;

    case msdlKeywordsLanguageKind::kKeywordsLanguageEnglish:
      keywordsNamesMapPTR = &gGlobalEnglishKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsLanguageFrench:
      keywordsNamesMapPTR = &gGlobalFrenchKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsLanguageItalian:
      keywordsNamesMapPTR = &gGlobalItalianKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsLanguageGerman:
      keywordsNamesMapPTR = &gGlobalGermanKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsLanguageSpanish:
      keywordsNamesMapPTR = &gGlobalSpanishKeywordsNamesMap;
      break;
    case msdlKeywordsLanguageKind::kKeywordsLanguageNederlands:
      keywordsNamesMapPTR = &gGlobalDutchKeywordsNamesMap;
      break;
  } // switch

  if (keywordsNamesMapPTR) {
    size_t
      nextToLast =
        (int) msdlTokenKind::NonSeparatorsLast - 2;

    size_t count = 0;
    size_t cumulatedLength = 0;

    std::stringstream ss;

    for (auto e : EnumNonSeparators<msdlTokenKind> ()) {
      std::string
        nonSeparatorTokenAsMsdlString  =
          msdlTokenKindAsMsdlString (
            e,
            keywordsLanguageKind);

      ++count;

      cumulatedLength += nonSeparatorTokenAsMsdlString.size ();
      if (
        cumulatedLength >= namesListMaxLength
          ||
        e == msdlTokenKind::TranslatableFirst
          ||
        e == msdlTokenKind::LanguageDependentFirst
      ) {
        ss << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        ss << gIndenter.getSpacer ();
      }
      ss << nonSeparatorTokenAsMsdlString;

      if (count <= nextToLast) {
        ss << ' ';
      }
/*
      if ((int) i < (int) msdlTokenKind::LastIterable - 2) { JMI KEEP
        ss << ", ";
      }
      else {
        ss << " and ";
      }
*/
    } // for

    result = ss.str ();
  }

  return result;
}

//________________________________________________________________________
S_msdlTokenKindsSet msdlTokenKindsSet::create ()
{
  // create the set
  msdlTokenKindsSet* o = new
    msdlTokenKindsSet ();
  assert (o != nullptr);
  return o;
}

S_msdlTokenKindsSet msdlTokenKindsSet::create (
  std::initializer_list<msdlTokenKind> elements)
{
  // create the set
  msdlTokenKindsSet* o = new
    msdlTokenKindsSet (
      elements);
  assert (o != nullptr);
  return o;
}

S_msdlTokenKindsSet msdlTokenKindsSet::create (
  msdlTokenKindsSet &tokenKindsSet)
{
  // create the set
  msdlTokenKindsSet* o = new
    msdlTokenKindsSet (
      tokenKindsSet);
  assert (o != nullptr);
  return o;
}

S_msdlTokenKindsSet msdlTokenKindsSet::createClone ()
{
  S_msdlTokenKindsSet
    result =
      msdlTokenKindsSet::create (*this);

  if (fTokenKindsSet.size ()) {
    for (
      std::set<msdlTokenKind>::const_iterator i =
        fTokenKindsSet.begin ();
      i != fTokenKindsSet.end ();
      ++i
    ) {
      result->fTokenKindsSet.insert ((*i));
    } // for
  }

#ifdef MF_TRACE_IS_ENABLED
  if (false && gGlobalMsdl2msrOahGroup->getTraceTokens ()) {
    std::stringstream ss;

    ss <<
      "<== createClone()" <<
      ", result:" << result->asString () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return result;
}

msdlTokenKindsSet::msdlTokenKindsSet ()
{}

msdlTokenKindsSet::msdlTokenKindsSet (
  std::initializer_list<msdlTokenKind> elements)
{
  // populate the set
  std::initializer_list<msdlTokenKind>::const_iterator it;

  for (it = elements.begin (); it != elements.end (); ++it) {
    fTokenKindsSet.insert ((*it));
  } // for
}

msdlTokenKindsSet::msdlTokenKindsSet (
  msdlTokenKindsSet& tokenKindsSet)
  : fTokenKindsSet (
    tokenKindsSet.fTokenKindsSet)
{}

msdlTokenKindsSet::~msdlTokenKindsSet ()
{}

void msdlTokenKindsSet::addElementsFrom (S_msdlTokenKindsSet tokenKindsSet)
{
  if (tokenKindsSet->fTokenKindsSet.size ()) {
    for (
      std::set<msdlTokenKind>::const_iterator i =
        tokenKindsSet->fTokenKindsSet.begin ();
      i != tokenKindsSet->fTokenKindsSet.end ();
      ++i
    ) {
      fTokenKindsSet.insert ((*i));
    } // for
  }
}

void msdlTokenKindsSet::removeElementsFrom (S_msdlTokenKindsSet tokenKindsSet)
{
  if (tokenKindsSet->fTokenKindsSet.size ()) {
    for (
      std::set<msdlTokenKind>::const_iterator i =
        tokenKindsSet->fTokenKindsSet.begin ();
      i != tokenKindsSet->fTokenKindsSet.end ();
      ++i
    ) {
      fTokenKindsSet.erase ((*i));
    } // for
  }
}

std::string msdlTokenKindsSet::asString () const
{
  std::stringstream ss;

  ss << '[';

  if (fTokenKindsSet.size ()) {
    std::set<msdlTokenKind>::const_iterator
      iBegin = fTokenKindsSet.begin (),
      iEnd   = fTokenKindsSet.end (),
      i      = iBegin;

    for ( ; ; ) {
      ss <<
        msdlTokenKindAsString ((*i));
        /*
        .asMsdlString (
          fUserLanguageKind,
          msdlCommentsTypeKind::kCommentsTypePercent);
*/

      if (++i == iEnd) break;

      ss << ", ";
    } // for
  }

  ss << ']';

  return ss.str ();
}

void msdlTokenKindsSet::print (std::ostream& os) const
{
  os <<
    "msdlTokenKindsSet:";

  if (fTokenKindsSet.size ()) {
    std::set<msdlTokenKind>::const_iterator
      iBegin = fTokenKindsSet.begin (),
      iEnd   = fTokenKindsSet.end (),
      i      = iBegin;

    for ( ; ; ) {
      os <<
        msdlTokenKindAsString ((*i));
        /*
        .asMsdlString (
          fUserLanguageKind,
          msdlCommentsTypeKind::kCommentsTypePercent);
*/

      if (++i == iEnd) break;

      os << ", ";
    } // for
  }
  else {
    os << " empty" << std::endl;
  }
}

std::ostream& operator << (std::ostream& os, const S_msdlTokenKindsSet& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

EXP S_msdlTokenKindsSet operator+= (
  S_msdlTokenKindsSet aTokenKindsSet,
  msdlTokenKind       aTokenKind)
{
  aTokenKindsSet->
    addTokenKind (
      aTokenKind);

  return aTokenKindsSet;
}

EXP S_msdlTokenKindsSet operator+= (
  S_msdlTokenKindsSet aTokenKindsSet,
  S_msdlTokenKindsSet anotherTokenKindsSet)
{
  aTokenKindsSet->
    addElementsFrom (
      anotherTokenKindsSet);

  return aTokenKindsSet;
}

EXP S_msdlTokenKindsSet operator-= (
  S_msdlTokenKindsSet aTokenKindsSet,
  msdlTokenKind       aTokenKind)
{
  aTokenKindsSet->
    removeTokenKind (
      aTokenKind);

  return aTokenKindsSet;
}

EXP S_msdlTokenKindsSet operator-= (
  S_msdlTokenKindsSet aTokenKindsSet,
  S_msdlTokenKindsSet anotherTokenKindsSet)
{
  aTokenKindsSet->
    removeElementsFrom (
      anotherTokenKindsSet);

  return aTokenKindsSet;
}

EXP S_msdlTokenKindsSet operator+ (
  S_msdlTokenKindsSet aTokenKindsSet,
  msdlTokenKind       aTokenKind)
{
  S_msdlTokenKindsSet
    result =
      aTokenKindsSet->createClone ();

  result->
    addTokenKind (
      aTokenKind);

#ifdef MF_TRACE_IS_ENABLED
  if (false && gGlobalMsdl2msrOahGroup->getTraceTokens ()) {
    std::stringstream ss;

    ss <<
      "<== S_msdlTokenKindsSet operator+()" <<
      ", result:" << result->asString () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return result;
}

EXP S_msdlTokenKindsSet operator+ (
  S_msdlTokenKindsSet aTokenKindsSet,
  S_msdlTokenKindsSet anotherTokenKindsSet)
{
  S_msdlTokenKindsSet
    result =
      aTokenKindsSet->createClone ();

  result += anotherTokenKindsSet;

#ifdef MF_TRACE_IS_ENABLED
  if (false && gGlobalMsdl2msrOahGroup->getTraceTokens ()) {
    std::stringstream ss;

    ss <<
      "<== S_msdlTokenKindsSet operator+()" <<
      ", result:" << result->asString () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return result;
}

EXP S_msdlTokenKindsSet operator- (
  S_msdlTokenKindsSet aTokenKindsSet,
  msdlTokenKind       aTokenKind)
{
  S_msdlTokenKindsSet
    result =
      aTokenKindsSet->createClone ();

  result->
    removeTokenKind (
      aTokenKind);

  return result;
}

EXP S_msdlTokenKindsSet operator- (
  S_msdlTokenKindsSet aTokenKindsSet,
  S_msdlTokenKindsSet anotherTokenKindsSet)
{
  S_msdlTokenKindsSet
    result =
      aTokenKindsSet->createClone ();

  result -= anotherTokenKindsSet;

  return result;
}

//_______________________________________________________________________________
S_msdlTokenDescription msdlTokenDescription::create ()
{
  msdlTokenDescription* o =
    new msdlTokenDescription ();
  assert (o != nullptr);
  return o;
}

msdlTokenDescription::msdlTokenDescription ()
{}

msdlTokenDescription::~msdlTokenDescription ()
{}

msdlKeywordKind msdlTokenDescription::getKeywordKind () const
{
  msdlKeywordKind result = msdlKeywordKind::kKeyword_UNKNOWN;

  switch (fTokenDescriptionKind) {
    case msdlTokenDescriptionKind::kTokenKeyword:
      result = fKeywordKind;
      break;

    case msdlTokenDescriptionKind::kTokenInteger:
      {
        std::string message =
          "MSDL token desr contains an integer, not a keyword";

        gLog <<
          message <<
          std::endl;

        throw msdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenDouble:
      {
        std::string message =
          "MSDL token desr contains a double, not an keyword";

        gLog <<
          message <<
          std::endl;

        throw msdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenCharacter:
      {
        std::string message =
          "MSDL token desr contains a character, not an keyword";

        gLog <<
          message <<
          std::endl;

        throw msdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenString:
      {
        std::string message =
          "MSDL token desr contains a std::string, not an keyword";

        gLog <<
          message <<
          std::endl;

        throw msdlException (message);
      }
      break;
  } // switch

  return result;
}

int msdlTokenDescription::getInteger () const
{
  int result = 0;

  switch (fTokenDescriptionKind) {
    case msdlTokenDescriptionKind::kTokenKeyword:
      {
        std::string message =
          "MSDL token desr contains a keyword, not an integer";

        gLog <<
          message <<
          std::endl;

        throw msdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenInteger:
      result = fInteger;
      break;

    case msdlTokenDescriptionKind::kTokenDouble:
      {
        std::string message =
          "MSDL token desr contains a double, not an integer";

        gLog <<
          message <<
          std::endl;

        throw msdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenCharacter:
      {
        std::string message =
          "MSDL token desr contains a character, not an integer";

        gLog <<
          message <<
          std::endl;

        throw msdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenString:
      {
        std::string message =
          "MSDL token desr contains a std::string, not an integer";

        gLog <<
          message <<
          std::endl;

        throw msdlException (message);
      }
      break;
  } // switch

  return result;
}

double msdlTokenDescription::getDouble () const
{
  double result = 0.0;

  switch (fTokenDescriptionKind) {
    case msdlTokenDescriptionKind::kTokenKeyword:
      {
        std::string message =
          "MSDL token desr contains a keyword, not a double";

        gLog <<
          message <<
          std::endl;

        throw msdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenInteger:
      {
        std::string message =
          "MSDL token desr contains an integer, not a double";

        gLog <<
          message <<
          std::endl;

        throw msdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenDouble:
      result = fDouble;
      break;

    case msdlTokenDescriptionKind::kTokenCharacter:
      {
        std::string message =
          "MSDL token desr contains a character, not a double";

        gLog <<
          message <<
          std::endl;

        throw msdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenString:
      {
        std::string message =
          "MSDL token desr contains a std::string, not a double";

        gLog <<
          message <<
          std::endl;

        throw msdlException (message);
      }
      break;
  } // switch


  return result;
}

char msdlTokenDescription::getCharacter () const
{
  char result = '@';

  switch (fTokenDescriptionKind) {
    case msdlTokenDescriptionKind::kTokenKeyword:
      {
        std::string message =
          "MSDL token desr contains a keyword, not a character";

        gLog <<
          message <<
          std::endl;

        throw msdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenInteger:
      {
        std::string message =
          "MSDL token desr contains an integer, not a character";

        gLog <<
          message <<
          std::endl;

        throw msdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenDouble:
      {
        std::string message =
          "MSDL token desr contains a double, not a character";

        gLog <<
          message <<
          std::endl;

        throw msdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenCharacter:
      result = fCharacter;
      break;

    case msdlTokenDescriptionKind::kTokenString:
      {
        std::string message =
          "MSDL token desr contains a double, not a character";

        gLog <<
          message <<
          std::endl;

        throw msdlException (message);
      }
  } // switch

  return result;
}

std::string msdlTokenDescription::getString () const
{
  std::string result;

  switch (fTokenDescriptionKind) {
    case msdlTokenDescriptionKind::kTokenKeyword:
      {
        std::string message =
          "MSDL token desr contains a keyword, not a std::string";

        gLog <<
          message <<
          std::endl;

        throw msdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenInteger:
      {
        std::string message =
          "MSDL token desr contains an integer, not a std::string";

        gLog <<
          message <<
          std::endl;

        throw msdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenDouble:
      {
        std::string message =
          "MSDL token desr contains a double, not a std::string";

        gLog <<
          message <<
          std::endl;

        throw msdlException (message);
      }
      break;

    case msdlTokenDescriptionKind::kTokenCharacter:
      result = fCharacter;
      break;

    case msdlTokenDescriptionKind::kTokenString:
      result = fString;
      break;
  } // switch

  return result;
}

std::string msdlTokenDescription::asString () const
{
  std::stringstream ss;

  ss <<
    "[MSDL tokenDescription" <<
    ", tokenDescriptionKind: " <<
    fTokenDescriptionKind <<
    ": ";

  switch (fTokenDescriptionKind) {
    case msdlTokenDescriptionKind::kTokenKeyword:
      ss << msdlKeywordKindAsString (fKeywordKind);
      break;

    case msdlTokenDescriptionKind::kTokenInteger:
      ss << fInteger;
      break;

    case msdlTokenDescriptionKind::kTokenDouble:
      ss << fDouble;
      break;

    case msdlTokenDescriptionKind::kTokenCharacter:
      ss << "'" << fString << "'";
      break;

    case msdlTokenDescriptionKind::kTokenString:
      ss << "\"" << fString << "\"";
      break;
  } // switch

  ss << ']';

  return ss.str ();
}

void msdlTokenDescription::print (std::ostream& os) const
{
  os <<
    fTokenDescriptionKind <<
    ": ";

  switch (fTokenDescriptionKind) {
    case msdlTokenDescriptionKind::kTokenKeyword:
      os << msdlKeywordKindAsString (fKeywordKind);
      break;

    case msdlTokenDescriptionKind::kTokenInteger:
      os << fInteger;
      break;

    case msdlTokenDescriptionKind::kTokenDouble:
      os << fDouble;
      break;

    case msdlTokenDescriptionKind::kTokenCharacter:
      os << "'" << fString << "'";
      break;

    case msdlTokenDescriptionKind::kTokenString:
      os << "\"" << fString << "\"";
      break;
  } // switch

  os << ']';
}

std::ostream& operator << (std::ostream& os, const msdlTokenDescription& elt)
{
  elt.print (os);
  return os;
}

//_______________________________________________________________________________
msdlToken::msdlToken ()
    : fTokenKind (msdlTokenKind::kToken_UNKNOWN)
{
  initializeToken ();
}

msdlToken::msdlToken (
  msdlTokenKind tokenKind)
    : fTokenKind (tokenKind)
{
  initializeToken ();
}

msdlToken::msdlToken (
  msdlTokenKind   tokenKind,
  msdlKeywordKind value)
    : fTokenKind (tokenKind)
{
  initializeToken ();

  fTokenDescription.setKeywordKind (value);
}

msdlToken::msdlToken (
  msdlTokenKind tokenKind,
  int           value)
    : fTokenKind (tokenKind)
{
  initializeToken ();

  fTokenDescription.setInteger (value);
}

msdlToken::msdlToken (
  msdlTokenKind tokenKind,
  const std::string& value)
    : fTokenKind (tokenKind)
{
  initializeToken ();

  fTokenDescription.setString (value);
}

msdlToken::msdlToken (
  msdlTokenKind tokenKind,
  double        value)
    : fTokenKind (tokenKind)
{
  initializeToken ();

  fTokenDescription.setDouble (value);
}

msdlToken::~msdlToken ()
{}

void msdlToken::initializeToken ()
{
  fTokenLineNumber     = 0;
  fTokenPositionInLine = 0;
}

std::string msdlToken::asString () const
{
  std::stringstream ss;

  ss << std::left <<
    "[ " <<
    "@" << fTokenLineNumber <<
    ":" << fTokenPositionInLine <<
    ' ' <<
    msdlTokenKindAsString (fTokenKind) <<
    ' ';

  switch (fTokenKind) {
    case msdlTokenKind::kToken_UNKNOWN:
      // should not occur
      break;

    case msdlTokenKind::k_TokenEOF:
      ss << "END_OF_FILE";
      break;

    case msdlTokenKind::k_TokenMalformed:
      ss << "\"" << fTokenDescription.getString () << "\"";
      break;

    // separators, for use by MSDL whole input translation
    // ------------------------------------

    case msdlTokenKind::kTokenSpace:
      ss << "SPACE";
      break;
    case msdlTokenKind::kTokenTabSeparator:
      ss << "TAB";
      break;
    case msdlTokenKind::kTokenCarriageReturn:
      ss << "CARRIAGE_RETURN";
      break;

    case msdlTokenKind::kTokenEndOfLine:
      ss << "END_OF_LINE";
      break;

    case msdlTokenKind::kTokenParenthesizedComment:
      ss << "\"" << fTokenDescription.getString () << "\"";
      break;

    case msdlTokenKind::kTokenCommentToEndOfLine:
      ss << "\"" << fTokenDescription.getString () << "\"";
      break;

    // language-independent tokens
    // ------------------------------------

    case msdlTokenKind::kTokenEqualSign:
      ss << "=";
      break;

    case msdlTokenKind::kTokenComma:
      ss << ",";
      break;
    case msdlTokenKind::kTokenColon:
      ss << ":";
      break;
    case msdlTokenKind::kTokenSemiColon:
      ss << ";";
      break;

    case msdlTokenKind::kTokenPlus:
      ss << "+";
      break;
    case msdlTokenKind::kTokenMinus:
      ss << "-";
      break;
    case msdlTokenKind::kTokenStar:
      ss << "*";
      break;
    case msdlTokenKind::kTokenSlash:
      ss << '/';
      break;
    case msdlTokenKind::kTokenPercent:
      ss << "%";
      break;

    case msdlTokenKind::kTokenConcat:
      ss << "!!";
      break;

    case msdlTokenKind::kTokenQuestionMark:
      ss << "?";
      break;

    case msdlTokenKind::kTokenTilda:
      ss << "~";
      break;

    case msdlTokenKind::kTokenLeftAngle:
      ss << "<";
      break;
    case msdlTokenKind::kTokenRightAngle:
      ss << ">";
      break;

    case msdlTokenKind::kTokenLeftParenthesis:
      ss << "(";
      break;
    case msdlTokenKind::kTokenRightParenthesis:
      ss << ")";
      break;

    case msdlTokenKind::kTokenLeftBracket:
      ss << '{';
      break;
    case msdlTokenKind::kTokenRightBracket:
      ss << '}';
      break;

    case msdlTokenKind::kTokenQuote:
      ss << "'";
      break;
    case msdlTokenKind::kTokenDot:
      ss << ".";
      break;

    case msdlTokenKind::kTokenMeasure:
      ss << "|";
      break;
    case msdlTokenKind::kTokenDoubleBar:
      ss << "||";
      break;
    case msdlTokenKind::kTokenFinalBar:
      ss << "|||";
      break;

    case msdlTokenKind::kTokenRepeatStart:
      ss << "|||:";
      break;
    case msdlTokenKind::kTokenRepeatEnd:
      ss << ":|||";
      break;

    case msdlTokenKind::kTokenInteger:
      ss << fTokenDescription.getInteger ();
      break;
//    case msdlTokenKind::kTokenDouble:
//      ss << fTokenDescription.getDouble ();
//      break;

    case msdlTokenKind::kTokenString:
      ss << "\"" << fTokenDescription.getString () << "\"";
      break;

    case msdlTokenKind::kTokenName:
      ss << "\"" << fTokenDescription.getString () << "\"";
      break;
    case msdlTokenKind::kTokenIdentifier:
      ss << "\"" << fTokenDescription.getString () << "\"";
      break;

    // language-dependent keywords
    // ------------------------------------

    default:
      ss <<
        msdlKeywordKindAsString (fTokenDescription.getKeywordKind ());
  } // switch

  ss << " ]";

  return ss.str ();
}

std::string msdlToken::asMsdlString (
  msdlKeywordsLanguageKind languageKind,
  msdlCommentsTypeKind     commentsTypeKind) const
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsdl2msrOahGroup->getTraceTokens ()) {
    std::stringstream ss;

    ss <<
      "==> asMsdlString()" <<
      ", tokenKind" << ": " << msdlTokenKindAsString (fTokenKind) <<
      ", languageKind: " << msdlKeywordsLanguageKindAsString (languageKind) <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::stringstream ss;

  switch (fTokenKind) {
    case msdlTokenKind::kToken_UNKNOWN:
      // should not occur
      break;

    case msdlTokenKind::k_TokenEOF:
      ss << "TokenEOF";
      break;

    case msdlTokenKind::k_TokenMalformed:
      ss << "*TokenMalformed*";
      break;

    // separators, for use by MSDL whole input translation
    // ------------------------------------

    case msdlTokenKind::kTokenSpace:
      ss << ' ';
      break;
    case msdlTokenKind::kTokenTabSeparator:
      ss << "TokenTab";
      break;
    case msdlTokenKind::kTokenCarriageReturn:
      ss << "TokenCarriageReturn";
      break;

    case msdlTokenKind::kTokenEndOfLine:
      ss << std::endl;
      break;

    case msdlTokenKind::kTokenParenthesizedComment:
      switch (commentsTypeKind) {
        case msdlCommentsTypeKind::kCommentsTypePercent:
          ss << "%{" << fTokenDescription.getString () << "%}";
          break;
        case msdlCommentsTypeKind::kCommentsTypeStar:
          ss << "/*" << fTokenDescription.getString () << "*/";
          break;
      } // switch
      break;

    case msdlTokenKind::kTokenCommentToEndOfLine:
      switch (commentsTypeKind) {
        case msdlCommentsTypeKind::kCommentsTypePercent:
          ss << "%" << fTokenDescription.getString ();
          break;
        case msdlCommentsTypeKind::kCommentsTypeStar:
          ss << "//" << fTokenDescription.getString ();
          break;
      } // switch
      break;

    // language-independent tokens
    // ------------------------------------

    case msdlTokenKind::kTokenEqualSign:
      ss << "=";
      break;

    case msdlTokenKind::kTokenComma:
      ss << ",";
      break;
    case msdlTokenKind::kTokenColon:
      ss << ":";
      break;
    case msdlTokenKind::kTokenSemiColon:
      ss << ";";
      break;

    case msdlTokenKind::kTokenPlus:
      ss << "+";
      break;
    case msdlTokenKind::kTokenMinus:
      ss << "-";
      break;
    case msdlTokenKind::kTokenStar:
      ss << "*";
      break;
    case msdlTokenKind::kTokenSlash:
      ss << '/';
      break;
    case msdlTokenKind::kTokenPercent:
      ss << "%";
      break;

    case msdlTokenKind::kTokenConcat:
      ss << "!!";
      break;

    case msdlTokenKind::kTokenQuestionMark:
      ss << "?";
      break;

    case msdlTokenKind::kTokenTilda:
      ss << "~";
      break;

    case msdlTokenKind::kTokenLeftAngle:
      ss << "<";
      break;
    case msdlTokenKind::kTokenRightAngle:
      ss << ">";
      break;

    case msdlTokenKind::kTokenLeftParenthesis:
      ss << "(";
      break;
    case msdlTokenKind::kTokenRightParenthesis:
      ss << ")";
      break;

    case msdlTokenKind::kTokenLeftBracket:
      ss << "{";
      break;
    case msdlTokenKind::kTokenRightBracket:
      ss << "}";
      break;

    case msdlTokenKind::kTokenQuote:
      ss << "'";
      break;
    case msdlTokenKind::kTokenDot:
      ss << ".";
      break;

    case msdlTokenKind::kTokenMeasure:
      ss << "|";
      break;
    case msdlTokenKind::kTokenDoubleBar:
      ss << "||";
      break;
    case msdlTokenKind::kTokenFinalBar:
      ss << "|||";
      break;

    case msdlTokenKind::kTokenRepeatStart:
      ss << "|||:";
      break;
    case msdlTokenKind::kTokenRepeatEnd:
      ss << ":|||";
      break;

    case msdlTokenKind::kTokenInteger:
      ss << fTokenDescription.getInteger ();
      break;
//    case msdlTokenKind::kTokenDouble:
//      ss << fTokenDescription.getDouble ();
//      break;

    case msdlTokenKind::kTokenString:
      ss << "\"" << fTokenDescription.getString () << "\"";
      break;

    case msdlTokenKind::kTokenName:
      ss << fTokenDescription.getString ();
      break;
    case msdlTokenKind::kTokenIdentifier:
      ss << fTokenDescription.getString ();
      break;

    // language-dependent keywords
    // ------------------------------------

    default:
      ss <<
        msdlKeywordKindAsMsdlString (
          languageKind,
          fTokenDescription.getKeywordKind ());
  } // switch

  return ss.str ();
}

void msdlToken::print (std::ostream& os) const
{
  os << std::left <<
    msdlTokenKindAsString (fTokenKind) <<
    " @" << fTokenLineNumber <<
    ":" << fTokenPositionInLine <<
    std::endl;

  switch (fTokenKind) {
    case msdlTokenKind::kToken_UNKNOWN:
      // should not occur
      break;

    case msdlTokenKind::k_TokenEOF:
      os << "END_OF_FILE";
      break;

    case msdlTokenKind::k_TokenMalformed:
      os << "\"" << fTokenDescription.getString () << "\"";
      break;

    // separators, for use by MSDL whole input translation
    // ------------------------------------

    case msdlTokenKind::kTokenSpace:
      os << "SPACE";
      break;
    case msdlTokenKind::kTokenTabSeparator:
      os << "TAB";
      break;
    case msdlTokenKind::kTokenCarriageReturn:
      os << "CARRIAGE_RETURN";
      break;

    case msdlTokenKind::kTokenEndOfLine:
      os << "END_OF_LINE";
      break;

    case msdlTokenKind::kTokenParenthesizedComment:
      os << "\"" << fTokenDescription.getString () << "\"";
      break;

    case msdlTokenKind::kTokenCommentToEndOfLine:
      os << "\"" << fTokenDescription.getString () << "\"";
      break;

    // language-independent tokens
    // ------------------------------------

    case msdlTokenKind::kTokenEqualSign:
      os << "=";
      break;

    case msdlTokenKind::kTokenComma:
      os << ",";
      break;
    case msdlTokenKind::kTokenColon:
      os << ":";
      break;
    case msdlTokenKind::kTokenSemiColon:
      os << ";";
      break;

    case msdlTokenKind::kTokenPlus:
      os << "+";
      break;
    case msdlTokenKind::kTokenMinus:
      os << "-";
      break;
    case msdlTokenKind::kTokenStar:
      os << "*";
      break;
    case msdlTokenKind::kTokenSlash:
      os << '/';
      break;
    case msdlTokenKind::kTokenPercent:
      os << "%";
      break;

    case msdlTokenKind::kTokenConcat:
      os << "!!";
      break;

    case msdlTokenKind::kTokenQuestionMark:
      os << "?";
      break;

    case msdlTokenKind::kTokenTilda:
      os << "~";
      break;

    case msdlTokenKind::kTokenLeftAngle:
      os << "<";
      break;
    case msdlTokenKind::kTokenRightAngle:
      os << ">";
      break;

    case msdlTokenKind::kTokenLeftParenthesis:
      os << "(";
      break;
    case msdlTokenKind::kTokenRightParenthesis:
      os << ")";
      break;

    case msdlTokenKind::kTokenLeftBracket:
      os << "{";
      break;
    case msdlTokenKind::kTokenRightBracket:
      os << "}";
      break;

    case msdlTokenKind::kTokenQuote:
      os << "'";
      break;
    case msdlTokenKind::kTokenDot:
      os << ".";
      break;

    case msdlTokenKind::kTokenMeasure:
      os << "|";
      break;
    case msdlTokenKind::kTokenDoubleBar:
      os << "||";
      break;
    case msdlTokenKind::kTokenFinalBar:
      os << "|||";
      break;

    case msdlTokenKind::kTokenRepeatStart:
      os << "|||:";
      break;
    case msdlTokenKind::kTokenRepeatEnd:
      os << ":|||";
      break;

    case msdlTokenKind::kTokenInteger:
      os << fTokenDescription.getInteger ();
      break;
//    case msdlTokenKind::kTokenDouble:
//      os << fTokenDescription.getDouble ();
//      break;

    case msdlTokenKind::kTokenString:
      os << "\"" << fTokenDescription.getString () << "\"";
      break;

    case msdlTokenKind::kTokenName:
      os << "\"" << fTokenDescription.getString () << "\"";
      break;
    case msdlTokenKind::kTokenIdentifier:
      os << "\"" << fTokenDescription.getString () << "\"";
      break;

    // language-dependent keywords
    // ------------------------------------

    default:
      os <<
        msdlKeywordKindAsString (fTokenDescription.getKeywordKind ());
  } // switch

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const msdlToken& elt)
{
  elt.print (os);
  return os;
}

//_______________________________________________________________________________
S_msdlTokensList msdlTokensList::create ()
{
  msdlTokensList* o =
    new msdlTokensList ();
  assert (o != nullptr);
  return o;
}

msdlTokensList::msdlTokensList ()
{}

msdlTokensList::~msdlTokensList ()
{}

void msdlTokensList::appendTokenToTokensList (
  const msdlToken& token)
{
  fTokensList.push_back (token);
}

std::string msdlTokensList::asString () const
{
  std::stringstream ss;

  ss <<
    "MSDL tokens list" <<
    mfSingularOrPluralWithoutNumber (
      fTokensList.size (), "element", "elements");

  return ss.str ();
}

void msdlTokensList::print (std::ostream& os) const
{
  os <<
    "MsdlTokensList" <<
    std::endl;

  ++gIndenter;

  unsigned
    tokensListSize =
      fTokensList.size ();

  if (tokensListSize) {
    ++gIndenter;

    std::list<msdlToken>::const_iterator
      iBegin = fTokensList.begin (),
      iEnd   = fTokensList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << ((*i));
      if (++i == iEnd) break;
 // JMI      os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os << "[EMPTY]" << std::endl;
  }

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msdlTokensList& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}


} // namespace
