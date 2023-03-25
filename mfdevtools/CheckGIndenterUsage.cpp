/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <string>


// #define TRACE_TOOL_IS_ENABLED


//_______________________________________________________________________________
void checkGIndenterUsageNesting ()
{
  std::string incrementString = "++gIndenter";
  std::string decrementString = "--gIndenter";

  std::string commentString   = "//";

  int currentLineNumber = 0;

  int currentIndentationLevel = 0;

  // decipher std::cin line by line
  while (true) {
    std::string currentLine;

    getline (std::cin, currentLine);
    ++currentLineNumber;

#ifdef TRACE_TOOL_IS_ENABLED
//     std::cout <<
//       std::endl <<
//       std::endl <<
//       "==> currentLine: " << currentLine <<
//       std::endl <<
//       "==> currentIndentationLevel: " << currentIndentationLevel <<
//       std::endl;
#endif // TRACE_TOOL_IS_ENABLED

    // find the first occurrences of commentString in currentLine
    size_t commentFirstPos = currentLine.find (commentString);

    // find the first occurrences of incrementString in currentLine
    size_t incrementFirstPos = currentLine.find (incrementString);

    if (incrementFirstPos != std::string::npos) {
#ifdef TRACE_TOOL_IS_ENABLED
        std::cout <<
          "==> currentIndentationLevel: " << currentIndentationLevel <<
          std::endl <<
          "--> line number " << currentLineNumber <<
          ':' << incrementFirstPos <<
          " contains \"" << currentLine << '\"' <<
          std::endl <<
          std::endl;
#endif // TRACE_TOOL_IS_ENABLED

      if (incrementFirstPos < commentFirstPos) {
        if ((incrementFirstPos % 2) != 0 ) {
          std::cout <<
            " -- incrementFirstPos " << incrementFirstPos << " is not even" <<
            std::endl <<
            "--> line number " << currentLineNumber <<
            ':' << incrementFirstPos <<
            std::endl <<
            std::endl;

          return;
        }

        ++currentIndentationLevel;

//         int
//           indentation      = incrementFirstPos / 2,
//           indentationDelta = indentation - currentIndentationLevel;
//
//         switch (indentationDelta) {
//           case 1:
//             ++currentIndentationLevel;
//             std::cout <<
//             "==> NEW currentIndentationLevel: " << currentIndentationLevel <<
//               std::endl;
//
//             break;
//           default:
//             std::cout <<
//               "--> line number " << currentLineNumber <<
//               ": \"" << currentLine << "\"" <<
//               std::endl <<
//               "--> indentation delta " <<
//               indentationDelta <<
//               " wrong " <<
//               " on \"" <<
//               incrementString <<
//               "\"" <<
//               ", missing \"" <<
//               decrementString <<
//               '\"' <<
//               std::endl <<
//               std::endl <<
//               std::endl;
//
//             return;
//         } // switch
      }
    }

    // find the first occurrences of decrementString in currentLine
    size_t decrementFirstPos = currentLine.find (decrementString);

    if (decrementFirstPos < commentFirstPos) {
      if (decrementFirstPos != std::string::npos) {
#ifdef TRACE_TOOL_IS_ENABLED
        std::cout <<
          "==> currentIndentationLevel: " << currentIndentationLevel <<
          std::endl <<
          "--> line number " << currentLineNumber <<
          ':' << decrementFirstPos <<
          " contains \"" << currentLine << '\"' <<
          std::endl <<
          std::endl;
#endif // TRACE_TOOL_IS_ENABLED

        if ((decrementFirstPos % 2) != 0 ) {
          std::cout <<
          "==> currentIndentationLevel: " << currentIndentationLevel <<
            std::endl <<
            "--> line number " << currentLineNumber <<
            ':' << decrementFirstPos <<
            " -- decrementFirstPos " << decrementFirstPos << " is not even" <<
            std::endl <<
            std::endl;

          return;
        }

        --currentIndentationLevel;

        if (currentIndentationLevel < 0) {
          std::cout <<
          "==> ERROR --- currentIndentationLevel: " << currentIndentationLevel <<
            std::endl <<
            "--> line number " << currentLineNumber <<
            ':' << decrementFirstPos <<
            " -- decrementFirstPos " << decrementFirstPos << " is not even" <<
            std::endl <<
            std::endl;

          return;
        }

//         int
//           indentation      = decrementFirstPos / 2,
//           indentationDelta = indentation - currentIndentationLevel;
//
//         switch (indentationDelta) {
//           case 0:
//             // we stay the same indentation level
//             --currentIndentationLevel;
//             std::cout <<
//             "==> NEW currentIndentationLevel: " << currentIndentationLevel <<
//               std::endl;
//             break;
//           default:
//             std::cout <<
//               "--> line number " << currentLineNumber <<
//               ": \"" << currentLine << "\"" <<
//               std::endl <<
//               "--> indentation delta " <<
//               indentationDelta <<
//               " wrong " <<
//               " on \"" <<
//               incrementString <<
//               "\"" <<
//               ", missing \"" <<
//               decrementString <<
//               '\"' <<
//               std::endl <<
//               std::endl <<
//               std::endl;
//
//             return;
//         } // switch
      }
    }

    if (std::cin.eof ()) break;
  } // while

#ifdef TRACE_TOOL_IS_ENABLED
  std::cout <<
    "Final currentIndentationLevel: " << currentIndentationLevel <<
    std::endl;
#endif
}


//_______________________________________________________________________________
int main ()
{
  checkGIndenterUsageNesting ();
}
