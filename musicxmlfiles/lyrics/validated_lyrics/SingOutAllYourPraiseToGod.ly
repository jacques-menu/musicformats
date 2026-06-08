\version "2.24.4"

% Generated from "SingOutAllYourPraiseToGod.xml"
% by xml2ly v2026.2 (built on June 02, 2026)
% on Tuesday 2026-06-02 @ 08:20:38 CEST

% The conversion command as supplied was: 
%  xml2ly -lilypond-run-date -lilypond-generation-infos -output-file-name SingOutAllYourPraiseToGod.ly SingOutAllYourPraiseToGod.xml -lilypond-lyrics-durations-mode explicit
% or, with short option names:
%     SingOutAllYourPraiseToGod.ly SingOutAllYourPraiseToGod.xml explicit


% Scheme function(s): "date & time"
% A set of functions to obtain the LilyPond file creation or modification time.

#(define commandLine                  (object->string (command-line)))
#(define loc                          (+ (string-rindex commandLine #\space ) 2))
#(define commandLineLength            (- (string-length commandLine) 2))
#(define lilypondFileName             (substring commandLine loc commandLineLength))

#(define lilypondFileDirName          (dirname lilypondFileName))
#(define lilypondFileBaseName         (basename lilypondFileName))
#(define lilypondFileSuffixlessName   (basename lilypondFileBaseName ".ly"))

#(define pdfFileName                  (string-append lilypondFileSuffixlessName ".pdf"))
#(define pdfFileFullName              (string-append lilypondFileDirName file-name-separator-string pdfFileName))

#(define lilypondVersion              (object->string (lilypond-version)))
#(define currentDate                  (strftime "%d/%m/%Y" (localtime (current-time))))
#(define currentTime                  (strftime "%H:%M:%S" (localtime (current-time))))

#(define lilypondFileModificationTime (stat:mtime (stat lilypondFileName)))

#(define lilypondFileModificationTimeAsString (strftime "%A %d/%m/%Y, %H:%M:%S" (localtime lilypondFileModificationTime)))

#(use-modules (srfi srfi-19))
% https://www.gnu.org/software/guile/manual/html_node/SRFI_002d19-Date-to-string.html
%#(define pdfFileCreationTime (date->string (current-date) "~A, ~B ~e ~Y ~H:~M:~S"))
#(define pdfFileCreationTime (date->string (current-date) "~A ~d/~m/~Y, ~H:~M:~S"))


\header {
  movementTitle       = "Sing Out All Your Praise To God"
  encodingDate        = "2016-09-05"
  software            = "MuseScore 2.0.3"
  title               = "Sing Out All Your Praise To God"
}

\paper {
  % horizontal-shift = 0.0\mm
  % indent = 0.0\mm
  % short-indent = 0.0\mm
  
  % markup-system-spacing-padding = 0.0\mm
  % between-system-space = 0.0\mm
  % page-top-space = 0.0\mm
  
  % page-count = -1
  % system-count = -1
  
  oddHeaderMarkup = \markup {
    \fill-line {
      \unless \on-first-page {
        \fromproperty #'page:page-number-std::string
        ' '
        \fromproperty #'header:title
        ' '
        \fromproperty #'header:subtitle
      }
    }
  }

  evenHeaderMarkup = \markup {
    \fill-line {
      \unless \on-first-page {
        \fromproperty #'page:page-number-std::string
        ' '
        \fromproperty #'header:title
        ' '
        \fromproperty #'header:subtitle
      }
    }
  }

  oddFooterMarkup = \markup {
    \tiny
    \column {
      \fill-line {
        #(string-append
"Score generated from MusicXML data by xml2ly v2026.2 (built on June 02, 2026) and LilyPond " (lilypond-version))
      }
      \fill-line { \column { \italic { \concat { \lilypondFileName " was modified on " \lilypondFileModificationTimeAsString } } } }
      \fill-line { \column { \italic { \concat { \pdfFileName " was created on " \pdfFileCreationTime } } } }
     \fill-line { \column { \italic { \concat { "lilypondFileDirName: " \lilypondFileDirName } } } }
     \fill-line { \column { \italic { \concat { "pdfFileFullName: " \pdfFileFullName } } } }
%      \fill-line { \column { \italic { \concat { "lilypondFileBaseName: " \lilypondFileBaseName } } } }
%      \fill-line { \column { \italic { \concat { "lilypondFileSuffixlessName: " \lilypondFileSuffixlessName } } } }
%      \fill-line { \column { \italic { \concat { "pdfFileName: " \pdfFileName } } } }
    }
  }

  % evenFooterMarkup = ""
}

\layout {
  \context { \Score
    autoBeaming = ##f % to display tuplets brackets
  }
  \context { \Voice
  }
}

Part_POne_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  
  \clef "treble"
  \key ees \major
  \numericTimeSignature \time 4/4
  \stemUp  < ees' g' > 8 [  < d' bes' >   < ees' bes' >   < ees' g' >  ]  < f' aes' > 4  < ees' g' >  
  
   < d' f' > 4  < c' g' >   < c' ees' > 2
  
   < c' g' > 8 [  < d' bes' >   < ees' bes' >   < ees' g' >  ]  < f' aes' > 4  < ees' g' >  
  
  < d' f' > 1
  
  \break | % -1
  
   < c' c'' > 4  < d' d'' > 8 [  < ees' ees'' >  ]  < f' bes' > 4  < d' g' >  
  
   < ees' aes' > 4  < f' bes' > 8 [  < g' c'' >  ]  < c' f' > 2
  
   < aes f' > 8 [  < bes aes' >   < c' g' >   < d' f' >  ]  < ees' g' > 4  < d' f' >  
  ees'1
  \bar "|."
}

Part_POne_Staff_One_Voice_One_Stanza_One = \lyricmode {
  % ===> using explicit lyrics durations mode
  \set associatedVoice  = #"Part_POne_Staff_One_Voice_One#"
  \set ignoreMelismata = ##t
    "1. Sing"8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 168 ???%} out8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 216 ???%} all8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 264 ???%} your8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 309 ???%} prais4 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 356 ???%} es4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 87 ???%} | % 5316
    to4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 448 ???%} the4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 493 ???%} "Lord!"2 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/2 whn, line 393 ???%} | % 5316
    Sing8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 588 ???%} out8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 636 ???%} all8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 684 ???%} your8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 729 ???%} praise4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 776 ???%} to4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 531 ???%} | % 5316
    "God!"1 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/1 whn, line 813 ???%} | % 5316
    \break
    Lift4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 923 ???%} up8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 970 ???%} your8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1016 ???%} voic4 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1062 ???%} es4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 858 ???%} | % 5316
    in4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1157 ???%} joy8 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1204 ???%} ous8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1249 ???%} "psalms!"2 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/2 whn, line 1099 ???%} | % 5316
    Sing8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1345 ???%} out8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1392 ???%} all8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1438 ???%} your8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1484 ???%} praise4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1529 ???%} to4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1286 ???%} | % 5316
    "God!"1 | % 5316

}

Part_POne_Staff_One_Voice_One_Stanza_Two = \lyricmode {
  % ===> using explicit lyrics durations mode
  \set associatedVoice  = #"Part_POne_Staff_One_Voice_One#"
  \set ignoreMelismata = ##t
    "2. For"8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 172 ???%} the8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 220 ???%} Son8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 268 ???%} has8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 313 ???%} ris4 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 360 ???%} en4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 87 ???%} | % 5316
    on4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 452 ???%} His4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 497 ???%} day2 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/2 whn, line 393 ???%} | % 5316
    And8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 592 ???%} the8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 640 ???%} night8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 688 ???%} is8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 733 ???%} driv4 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 780 ???%} en4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 531 ???%} | % 5316
    "out!"1 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/1 whn, line 813 ???%} | % 5316
    \break
    Lift4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 927 ???%} your8 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 974 ???%} selves8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1020 ???%} from4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1066 ???%} that4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 858 ???%} | % 5316
    slew4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1161 ???%} of8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1208 ???%} des8 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1253 ???%} "pond!"2 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/2 whn, line 1099 ???%} | % 5316
    Sing8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1349 ???%} out8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1396 ???%} all8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1442 ???%} your8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1488 ???%} praise4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1533 ???%} to4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1286 ???%} | % 5316
    "God!"1 | % 5316

}

Part_POne_Staff_One_Voice_One_Stanza_Three = \lyricmode {
  % ===> using explicit lyrics durations mode
  \set associatedVoice  = #"Part_POne_Staff_One_Voice_One#"
  \set ignoreMelismata = ##t
    "3. No"8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 176 ???%} more8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 224 ???%} let8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 272 ???%} the8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 317 ???%} voice4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 364 ???%} of4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 87 ???%} | % 5316
    vic4 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 456 ???%} tor4 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 501 ???%} y2 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/2 whn, line 393 ???%} | % 5316
    Be8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 596 ???%} mut8 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 644 ???%} ed8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 692 ???%} with8 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 737 ???%} in4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 784 ???%} your4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 531 ???%} | % 5316
    "heart!"1 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/1 whn, line 813 ???%} | % 5316
    \break
    Lift4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 931 ???%} shout8 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 978 ???%} ing8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1024 ???%} prais4 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1070 ???%} es4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 858 ???%} | % 5316
    un4 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1165 ???%} to8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1212 ???%} the8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1257 ???%} "King!"2 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/2 whn, line 1099 ???%} | % 5316
    Sing8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1353 ???%} out8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1400 ???%} all8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1446 ???%} your8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1492 ???%} praise4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1537 ???%} to4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1286 ???%} | % 5316
    "God!"1 | % 5316

}

Part_POne_Staff_One_Voice_One_Stanza_Four = \lyricmode {
  % ===> using explicit lyrics durations mode
  \set associatedVoice  = #"Part_POne_Staff_One_Voice_One#"
  \set ignoreMelismata = ##t
    "4. For"8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 180 ???%} your8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 228 ???%} guilt8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 276 ???%} is8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 321 ???%} nailed4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 368 ???%} un4 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 87 ???%} | % 5316
    to4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 460 ???%} the4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 505 ???%} tree2 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/2 whn, line 393 ???%} | % 5316
    And8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 600 ???%} your8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 648 ???%} sin8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 696 ???%} un8 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 741 ???%} to4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 788 ???%} the4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 531 ???%} | % 5316
    "cross!"1 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/1 whn, line 813 ???%} | % 5316
    \break
    No4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 935 ???%} more8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 982 ???%} let8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1028 ???%} de4 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1074 ???%} mon4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 858 ???%} | % 5316
    chains4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1169 ???%} cum8 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1216 ???%} ber8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1261 ???%} "thee;"2 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/2 whn, line 1099 ???%} | % 5316
    Sing8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1357 ???%} out8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1404 ???%} all8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1450 ???%} your8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1496 ???%} praise4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1541 ???%} to4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1286 ???%} | % 5316
    "God!"1 | % 5316

}

Part_POne_Staff_One_Voice_One_Stanza_Five = \lyricmode {
  % ===> using explicit lyrics durations mode
  \set associatedVoice  = #"Part_POne_Staff_One_Voice_One#"
  \set ignoreMelismata = ##t
    "5. Death"8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 184 ???%} and8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 232 ???%} all8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 280 ???%} his8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 325 ???%} min4 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 372 ???%} ions4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 87 ???%} | % 5316
    turn4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 464 ???%} to4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 509 ???%} "flee!"2 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/2 whn, line 393 ???%} | % 5316
    Sing8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 604 ???%} out8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 652 ???%} all8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 700 ???%} your8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 745 ???%} praise4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 792 ???%} to4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 531 ???%} | % 5316
    "God!"1 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/1 whn, line 813 ???%} | % 5316
    \break
    Lift4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 939 ???%} up8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 986 ???%} your8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1032 ???%} voic4 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1078 ???%} es4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 858 ???%} | % 5316
    in4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1173 ???%} vic8 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1220 ???%} tor8 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1265 ???%} "y!"2 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/2 whn, line 1099 ???%} | % 5316
    Sing8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1361 ???%} out8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1408 ???%} all8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1454 ???%} your8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1500 ???%} praise4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1545 ???%} to4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1286 ???%} | % 5316
    "God!"1 | % 5316

}

Part_POne_Staff_One_Voice_One_Stanza_Six = \lyricmode {
  % ===> using explicit lyrics durations mode
  \set associatedVoice  = #"Part_POne_Staff_One_Voice_One#"
  \set ignoreMelismata = ##t
    "6. Je"8 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 188 ???%} sus8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 236 ???%} Christ8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 284 ???%} is8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 329 ???%} ris4 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 376 ???%} "en!"4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 87 ???%} | % 5316
    We4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 468 ???%} are4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 513 ???%} "free!"2 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/2 whn, line 393 ???%} | % 5316
    Sing8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 608 ???%} out8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 656 ???%} all8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 704 ???%} your8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 749 ???%} praise4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 796 ???%} to4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 531 ???%} | % 5316
    "God!"1 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/1 whn, line 813 ???%} | % 5316
    \break
    Guilt4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 943 ???%} is8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 990 ???%} for8 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1036 ???%} giv4 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1082 ???%} "en,"4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 858 ???%} | % 5316
    sin4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1177 ???%} is8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1224 ???%} washed8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1269 ???%} "clean!"2 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/2 whn, line 1099 ???%} | % 5316
    Sing8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1365 ???%} out8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1412 ???%} all8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1458 ???%} your8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 1504 ???%} praise4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1549 ???%} to4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 1286 ???%} | % 5316
    "God!"1 | % 5316

}

Part_POne_Staff_One_Voice_Two = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key ees \major
  \numericTimeSignature \time 4/4
  s1 s1 s1 s1 \break | % -1
  
  s1 s1 s1 ees'1
  \bar "|."
}

Part_PTwo_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  
  \clef "bass"
  \key ees \major
  \numericTimeSignature \time 4/4
  \stemDown  < c g > 8 [  < bes, g >   < c g >   < c g >  ]  < d aes > 4  < ees bes >  
  
   < d aes > 4  < ees g >   < ees aes > 2
  
   < ees g > 8 [  < f bes >   < g bes >   < g c' >  ]  < aes d' > 4  < g c' >  
  
  < f bes > 1
  
  \break | % -1
  
   < aes, f > 4  < aes, f > 8 [  < bes, g >  ]  < d bes > 4  < g bes >  
  
   < aes c' > 4  < d bes > 8 [  < ees c' >  ]  < f aes > 2
  
   < c f > 8 [  < d f >   < ees g >   < f aes >  ]  < g bes > 4  < f bes >  
  
  < g bes > 1
  \bar "|."
  
}

\book {

  \score {
    <<
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Violins 1"
        }
        <<
          \context Voice = "Part_POne_Staff_One_Voice_One" <<
            \voiceOne % out of 2 regular voices
            \Part_POne_Staff_One_Voice_One
          >>
          \new Lyrics
            \with {
            }
            \Part_POne_Staff_One_Voice_One_Stanza_One
          \new Lyrics
            \with {
            }
            \Part_POne_Staff_One_Voice_One_Stanza_Two
          \new Lyrics
            \with {
            }
            \Part_POne_Staff_One_Voice_One_Stanza_Three
          \new Lyrics
            \with {
            }
            \Part_POne_Staff_One_Voice_One_Stanza_Four
          \new Lyrics
            \with {
            }
            \Part_POne_Staff_One_Voice_One_Stanza_Five
          \new Lyrics
            \with {
            }
            \Part_POne_Staff_One_Voice_One_Stanza_Six
          \context Voice = "Part_POne_Staff_One_Voice_Two" <<
            \voiceTwo % out of 2 regular voices
            \Part_POne_Staff_One_Voice_Two
          >>
        >>
        \new Staff  = "Part_PTwo_Staff_One"
        \with {
          instrumentName = "Violoncellos"
        }
        <<
          \context Voice = "Part_PTwo_Staff_One_Voice_One" <<
            \Part_PTwo_Staff_One_Voice_One
          >>
        >>
      
      >>
    
    >>
    
    \layout {
      \context { \Score
        autoBeaming = ##f % to display tuplets brackets
      }
      \context { \Voice
      }
    }
    
    \midi {
      \tempo 16 = 360
    }
  }
  
}
