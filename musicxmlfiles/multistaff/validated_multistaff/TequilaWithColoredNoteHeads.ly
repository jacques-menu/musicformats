\version "2.24.4"

% Generated from "TequilaWithColoredNoteHeads.xml"
% on Wednesday 2025-05-28 @ 09:25:54 CEST
% by xml2ly v0.9.74 (built May 26, 2025 @ 16:51)

% The conversion command as supplied was: 
%  xml2ly -lilypond-run-date -lilypond-generation-infos -output-file-name TequilaWithColoredNoteHeads.ly TequilaWithColoredNoteHeads.xml
% or, with short option names:
%     TequilaWithColoredNoteHeads.ly TequilaWithColoredNoteHeads.xml


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
  workCreditTypeTitle = "Tequila With Colored Note Heads"
  encodingDate        = "2018-10-22"
  composer            = ""
  arranger            = ""
  artist              = ""
  software            = "soundnotation"
  software            = "Dolet 6.6"
  right               = ""
  title               = "Tequila With Colored Note Heads"
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
"Score generated from MusicXML data by xml2ly v0.9.74 (built May 26, 2025 @ 16:51) and LilyPond " (lilypond-version))
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
  \key c \major
  \time 4/4
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  \stemUp g'4 \p ( \tempo \markup {
    \concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      173
    } % concat
  }
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  \stemDown c'' \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a'' \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g'' ~  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g''1 ) \fermata  | % 3
  \barNumberCheck #3
  R1  | % 4
  \barNumberCheck #4
  
  \break | % 1333333 \myLineBreak
  
  
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 3/2 {  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X51 #X85 #XFF))
  f''2 \mf \fermata \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X00 #XC6 #X32))
  d'' \fermata \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X24 #XBB #XFF))
  e'' \fermata }
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c''1 ~  -\markup { "A Tempo" }  | % 5
  \barNumberCheck #6
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c''4 r r2 \bar "||"
   | % 6
  \barNumberCheck #7
  \break | % 1333333 \myLineBreak
  
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  \stemUp g'4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  \stemDown c'' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a'' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g'' ~   | % 7
  \barNumberCheck #8
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g''1  | % 8
  \barNumberCheck #9
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  \stemUp g'4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XC2 #X81 #X32))
  \stemDown b' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a'' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g'' ~   | % 9
  \barNumberCheck #10
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g''1  | % 10
  \barNumberCheck #11
  \break | % 1333333 \myLineBreak
  
  r4 \< \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XC2 #X81 #X32))
  b'' --  | % 11
  \barNumberCheck #12
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c'''4 \! \> -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c''' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XC2 #X81 #X32))
  b'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a'' --  | % 12
  \barNumberCheck #13
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X24 #XBB #XFF))
  e''1 \!  | % 13
  \barNumberCheck #14
  \break | % 1333333 \myLineBreak
  
  R1  | % 14
  \barNumberCheck #15
  r4 \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a'' \< -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XC2 #X81 #X32))
  b'' --  | % 15
  \barNumberCheck #16
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c'''4 \! \> -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c''' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XC2 #X81 #X32))
  b'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a'' --  | % 16
  \barNumberCheck #17
  \pageBreak | % 14444444
  
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X24 #XBB #XFF))
  e''1 \!  | % 17
  \barNumberCheck #18
  r4 \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X24 #XBB #XFF))
  e'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X00 #XC6 #X32))
  d'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c'' --  | % 18
  \barNumberCheck #19
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g''1  | % 19
  \barNumberCheck #20
  R1 \bar "||"
   | % 20
  \barNumberCheck #21
  \break | % 1333333 \myLineBreak
  
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  \stemUp g'4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  \stemDown c'' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a'' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g'' ~   | % 21
  \barNumberCheck #22
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g''1  | % 22
  \barNumberCheck #23
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  \stemUp g'4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XC2 #X81 #X32))
  \stemDown b' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a'' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g'' ~   | % 23
  \barNumberCheck #24
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g''1  | % 24
  \barNumberCheck #25
  \break | % 1333333 \myLineBreak
  
  r4 \< \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XC2 #X81 #X32))
  b'' --  | % 25
  \barNumberCheck #26
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c'''4 \! \> -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c''' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XC2 #X81 #X32))
  b'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a'' --  | % 26
  \barNumberCheck #27
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X24 #XBB #XFF))
  e''1 \!  | % 27
  \barNumberCheck #28
  \break | % 1333333 \myLineBreak
  
  R1  | % 28
  \barNumberCheck #29
  r4 \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a'' \< -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XC2 #X81 #X32))
  b'' --  | % 29
  \barNumberCheck #30
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c'''4 \! \> -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c''' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XC2 #X81 #X32))
  b'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a'' --  | % 30
  \barNumberCheck #31
  \break | % 1333333 \myLineBreak
  
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X24 #XBB #XFF))
  e''1 \!  | % 31
  \barNumberCheck #32
  r4 \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X24 #XBB #XFF))
  e'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X00 #XC6 #X32))
  d'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c'' --  | % 32
  \barNumberCheck #33
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g''1 ~   | % 33
  \barNumberCheck #34
  \break | % 1333333 \myLineBreak
  
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g''1 \fermata  | % 34
  \barNumberCheck #35
  r2 \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X51 #X85 #XFF))
  f'' \f \fermata  | % 35
  \barNumberCheck #36
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X00 #XC6 #X32))
  d''2 \fermata \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X24 #XBB #XFF))
  e'' \fermata  | % 36
  \barNumberCheck #37
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c''1 \fermata
  \bar "|."
   | % 1
  \barNumberCheck #38
}

Part_PTwo_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key c \major
  \time 4/4
  R1 r4 \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  \stemUp g' ( \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  \stemDown c'' \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a''  | % 3
  \barNumberCheck #3
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g''1 ) \fermata  | % 4
  \barNumberCheck #4
  
  \break | % 1333333 \myLineBreak
  
  
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 3/2 {  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  \stemUp a'2 \fermata \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X51 #X85 #XFF))
  f' \fermata \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g' \fermata }
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X24 #XBB #XFF))
  e'4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  \stemDown c'' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  \stemUp g' -.  | % 5
  \barNumberCheck #6
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X24 #XBB #XFF))
  e'4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  \stemDown c'' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  \stemUp g' -. \bar "||"
   | % 6
  \barNumberCheck #7
  \break | % 1333333 \myLineBreak
  
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X24 #XBB #XFF))
  e'4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  \stemDown c'' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  \stemUp g' -.  | % 7
  \barNumberCheck #8
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  \stemDown c''4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  \stemUp g' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X24 #XBB #XFF))
  e' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g' -.  | % 8
  \barNumberCheck #9
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X24 #XBB #XFF))
  e'1  | % 9
  \barNumberCheck #10
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X24 #XBB #XFF))
  e'4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XC2 #X81 #X32))
  \stemDown b' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  \stemUp g' -.  | % 10
  \barNumberCheck #11
  \break | % 1333333 \myLineBreak
  
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X51 #X85 #XFF))
  f'4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X51 #X85 #XFF))
  \stemDown f'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X51 #X85 #XFF))
  f'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g'' --  | % 11
  \barNumberCheck #12
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a''4 -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X51 #X85 #XFF))
  f'' --  | % 12
  \barNumberCheck #13
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c''4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XC2 #X81 #X32))
  b' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c'' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  \stemUp a' -.  | % 13
  \barNumberCheck #14
  \break | % 1333333 \myLineBreak
  
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X24 #XBB #XFF))
  e'4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XC2 #X81 #X32))
  \stemDown b' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c'' -.  | % 14
  \barNumberCheck #15
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X51 #X85 #XFF))
  f''4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X51 #X85 #XFF))
  f'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X51 #X85 #XFF))
  f'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g'' --  | % 15
  \barNumberCheck #16
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a''4 -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X51 #X85 #XFF))
  f'' --  | % 16
  \barNumberCheck #17
  \pageBreak | % 14444444
  
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c''4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XC2 #X81 #X32))
  b' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  \stemUp a' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g' -.  | % 17
  \barNumberCheck #18
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X1A #X28 #XCD))
  fis'!2 \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X1A #X28 #XCD))
  fis'4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X1A #X28 #XCD))
  fis' -.  | % 18
  \barNumberCheck #19
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g'4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XC2 #X81 #X32))
  \stemDown b' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c'' -.  | % 19
  \barNumberCheck #20
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X00 #XC6 #X32))
  d''4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XC2 #X81 #X32))
  b' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  \stemUp a' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g' -. \bar "||"
   | % 20
  \barNumberCheck #21
  \break | % 1333333 \myLineBreak
  
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X24 #XBB #XFF))
  e'4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  \stemDown c'' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  \stemUp g' -.  | % 21
  \barNumberCheck #22
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  \stemDown c''4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  \stemUp g' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X24 #XBB #XFF))
  e' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g' -.  | % 22
  \barNumberCheck #23
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X24 #XBB #XFF))
  e'1  | % 23
  \barNumberCheck #24
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X24 #XBB #XFF))
  e'4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XC2 #X81 #X32))
  \stemDown b' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  \stemUp g' -.  | % 24
  \barNumberCheck #25
  \break | % 1333333 \myLineBreak
  
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X51 #X85 #XFF))
  f'4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X51 #X85 #XFF))
  \stemDown f'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X51 #X85 #XFF))
  f'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g'' --  | % 25
  \barNumberCheck #26
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a''4 -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X51 #X85 #XFF))
  f'' --  | % 26
  \barNumberCheck #27
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c''4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XC2 #X81 #X32))
  b' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c'' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  \stemUp a' -.  | % 27
  \barNumberCheck #28
  \break | % 1333333 \myLineBreak
  
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X24 #XBB #XFF))
  e'4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XC2 #X81 #X32))
  \stemDown b' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c'' -.  | % 28
  \barNumberCheck #29
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X51 #X85 #XFF))
  f''4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X51 #X85 #XFF))
  f'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X51 #X85 #XFF))
  f'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g'' --  | % 29
  \barNumberCheck #30
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a''4 -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g'' -- \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X51 #X85 #XFF))
  f'' --  | % 30
  \barNumberCheck #31
  \break | % 1333333 \myLineBreak
  
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c''4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XC2 #X81 #X32))
  b' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  \stemUp a' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g' -.  | % 31
  \barNumberCheck #32
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X1A #X28 #XCD))
  fis'!2 \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X1A #X28 #XCD))
  fis'4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X1A #X28 #XCD))
  fis' -.  | % 32
  \barNumberCheck #33
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g'4 -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  a' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XC2 #X81 #X32))
  \stemDown b' -. \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XFF #XC9 #X22))
  c'' -.  | % 33
  \barNumberCheck #34
  \break | % 1333333 \myLineBreak
  
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XC2 #X81 #X32))
  b'1 \fermata  | % 34
  \barNumberCheck #35
  r2 \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XE2 #X25 #X4E))
  \stemUp a' \fermata  | % 35
  \barNumberCheck #36
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X51 #X85 #XFF))
  f'2 \fermata \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#XAC #X01 #XE3))
  g' \fermata  | % 36
  \barNumberCheck #37
  \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X24 #XBB #XFF))
  e'1 \fermata
  \bar "|."
   | % 38
  \barNumberCheck #38
}

\book {

  \score {
    <<
      
      <<
      
        \new StaffGroup
        
        <<
        
          \new Staff  = "Part_POne_Staff_One"
          \with {
            instrumentName = "Instrument 1"
            shortInstrumentName = "Sblfl."
          }
          <<
            \context Voice = "Part_POne_Staff_One_Voice_One" <<
              \Part_POne_Staff_One_Voice_One
            >>
          >>
          \new Staff  = "Part_PTwo_Staff_One"
          \with {
            instrumentName = "Instrument 2"
            shortInstrumentName = "Sblfl."
          }
          <<
            \context Voice = "Part_PTwo_Staff_One_Voice_One" <<
              \Part_PTwo_Staff_One_Voice_One
            >>
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
