#include <format>
#include <iostream>

using namespace std;

// the MusicFormats languages
//______________________________________________________________________________
enum class mfLanguageKind {
  kMusicFormatsLanguage_UNKNOWN,

  kMusicFormatsLanguageEnglish, // MusicFormats default
  kMusicFormatsLanguageFrench,
  kMusicFormatsLanguageItalian,
  kMusicFormatsLanguageGerman,
  kMusicFormatsLanguageSpanish,
  kMusicFormatsLanguageDutch
};

std::string mfLanguageKindAsString (
  mfLanguageKind languageKind);

std::ostream& operator << (std::ostream& os, const mfLanguageKind& elt);

mfLanguageKind mfLanguageKindFromString (
  const std::string& theString);


std::string mfLanguageKindAsString (
  mfLanguageKind languageKind)
{
  std::string result;

  switch (languageKind) {
    case mfLanguageKind::kMusicFormatsLanguage_UNKNOWN:
      result = "*kMusicFormatsLanguage_UNKNOWN*";
      break;
    case mfLanguageKind::kMusicFormatsLanguageEnglish: // default value
      result = "english";
      break;
    case mfLanguageKind::kMusicFormatsLanguageFrench:
      result = "francais";
      break;
    case mfLanguageKind::kMusicFormatsLanguageItalian:
      result = "italiano";
      break;
    case mfLanguageKind::kMusicFormatsLanguageGerman:
      result = "deutsch";
      break;
    case mfLanguageKind::kMusicFormatsLanguageSpanish:
      result = "espanol";
      break;
    case mfLanguageKind::kMusicFormatsLanguageDutch:
      result = "dutch";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const mfLanguageKind& elt)
{
  os << mfLanguageKindAsString (elt);
  return os;
}


//______________________________________________________________________________

// https://en.cppreference.com/w/cpp/utility/variadic

// https://en.cppreference.com/w/cpp/utility/variadic/va_arg

#include <iostream>
#include <cstdarg>

void simple_printf(const char* fmt...) // C-style "const char* fmt, ..." is also valid
{
    va_list args;
    va_start(args, fmt);

    while (*fmt != '\0') {
        if (*fmt == 'd') {
            int i = va_arg(args, int);
            std::cout << i << '\n';
        } else if (*fmt == 'c') {
            // note automatic conversion to integral type
            int c = va_arg(args, int);
            std::cout << static_cast<char>(c) << '\n';
        } else if (*fmt == 'f') {
            double d = va_arg(args, double);
            std::cout << d << '\n';
        }
        ++fmt;
    }

    va_end(args);
}

int main()
{
    simple_printf("dcff", 3, 'a', 1.999, 42.5);
}


// #include <format>
// #include <iostream>
// #include <chrono>
//
// int main() {
//     auto ym = std::chrono::year { 2022 } / std::chrono::July;
//     std::string msg = std::format("{:*^10}\n{:*>10}\nin{}!", "hello", "world", ym);
//     std::cout << msg;
// }

// int main (int argc, char* argv[])
// {
//   mfLanguageKind languageKind =
//     mfLanguageKind::kMusicFormatsLanguageFrench;
//
//   std::cout <<
//     "languageKind: " << languageKind <<
//     std::endl;
//
//   std::cout <<
//     std::format ("languageKind: {}", languageKind) <<
//     std::endl;
//
// }


// operator std::string (const mfLanguageKind elt) { return mfLanguageKindAsString (elt); }
// std::string std::string (const mfLanguageKind elt) { return mfLanguageKindAsString (elt); }
// operator std::string (const mfLanguageKind elt)
// {
//   return mfLanguageKindAsString (elt);
// }

// void checkFormat (std::string theString)
// {
//   gLogStream <<
//     "checkConvertion, theString = " << theString <<
//     std::endl;
// }

//   checkFormat (mfLanguageKind::kMusicFormatsLanguageFrench);
