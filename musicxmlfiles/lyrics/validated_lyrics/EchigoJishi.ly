\version "2.24.4"

% Generated from "EchigoJishi.xml"
% by xml2ly v2026.2 (built on June 02, 2026)
% on Tuesday 2026-06-02 @ 06:29:46 CEST

% The conversion command as supplied was: 
%  xml2ly -lilypond-run-date -lilypond-generation-infos -output-file-name EchigoJishi.ly EchigoJishi.xml -lilypond-lyrics-durations-mode explicit
% or, with short option names:
%     EchigoJishi.ly EchigoJishi.xml explicit


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
  movementTitle       = "越後獅子 -- Echigo-Jishi"
  encodingDate        = "2007-06-19"
  arranger            = "Y. Nagai"
  arranger            = "K. Kobatake"
  software            = "Finale 2005 for Windows"
  software            = "Dolet 4.0 Beta 4 for Finale"
  right               = "Transcription donated to the public domain, 2005 by Tom Potter"
  title               = "越後獅子 -- Echigo-Jishi"
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
  \partial 2
  
  \clef "treble"
  \key c \major
  \numericTimeSignature \time 2/4
  \stemUp a'8 \f [
  \tempo \markup {
    "Allegro"\concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      92
    } % concat
  }
  a' b' a' ]
  \stemDown b'8 [
  a' b' c'' ]
  e''8 [
  c'' b' a' ]
  \stemUp f'4. r8 f'8 [
  a' b' a' ]
  f'4. e'8 \break | % -1
  
  f'8 [
  a' f' e' ]
  d'8 [
  d' d' e' ]
  f'8 \f [
  e' b d' ]
  e'4. r8 d'8 \mf d'4 e'8 f'8 f'4 e'8 \break | % -1
  
  a'8 [
  a' b' a' ]
  \stemDown b'4 c''8 [
  e'' ]
  fis''!8 fis''4 e''8 c''8 [
  c'' c'' e'' ]
  fis''!8 fis''4 e''8 c''8 [
  e''16 c'' ]
  \stemUp b'8 [
  a' ]
  \break | % -1
  
  \stemDown b'8 [
  c'' e'' a' ]
  \stemUp c''8 [
  b' a' f' ]
  e'4. f'8 a'8 [
  b' ]
  \stemDown c''4 e''4 c''8 [
  b' ]
  \stemUp a'4 a'8 [
  f' ]
  \break | % -1
  
  a'4. \stemDown b'8 \stemUp c''8 [
  b' a' f' ]
  e'4. e'8 \stemDown b'8 [
  c'' c'' b' ]
  c''8 [
  e'' c'' b' ]
  \stemUp a'8 [
  b' c'' a' ]
  \break | % -1
  
  f'8 [
  a'16 f' ]
  e'8 [
  d' ]
  e'8 [
  f' ]
  a'16 [
  a' a' a' ]
  \stemDown b'8 [
  b' ]
  c'' [
  c''16 c'' ]
  b'8 [
  c''16 b' ]
  \stemUp a'8 [
  b'16 a' ]
  f'8 [
  e' d' d' ]
  \break | % -1
  
  r8 e' [
  f' a' ]
  f'8 [
  e' ]
  b [
  b16 b ]
  b8 [
  d' ]
  e'4 r8 \stemDown c'' ( e''4 ) c''8 [
  b' a' b' ]
  \slurUp c''8 [
  ( a' ]
  ) \stemUp f'4 \break | % -1
  
  e'8 [
  d' e' a' ]
  f'4. e'8 f'8 [
  f'16 f' ]
  e'8 [
  d' ]
  d'8 [
  e' ]
  r \slurDown b'16. [
  ( a'32 ]
  f'4 ) \slurDown f' ( e'4 ) r
  \bar "|."
}

Part_POne_Staff_One_Voice_One_Stanza_One = \lyricmode {
  % ===> using explicit lyrics durations mode
  \set associatedVoice  = #"Part_POne_Staff_One_Voice_One#"
  \set ignoreMelismata = ##t
    2 | % 5316
    2 | % 5316
    2 | % 5316
    2 | % 5316
    2 | % 5316
    2 | % 5316
    \break
    2 | % 5316
    2 | % 5316
    2 | % 5316
    2 | % 5316
    8 "オノ"4 "ガ"8 | % 5316
    "ス"8 "ガタ"4 "ヲ"8 | % 5316
    \break
    "ハ"8 "ナ"8 "ト"8 "ミ"8 | % 5316
    "テ"4__  4 | % 5316
    "　"8 "ニ"4__  8 | % 5316
    "ワ"8__  8 "ニ"8__  8 | % 5316
    "　"8 "サイ"4__  8 | % 5316
    "タ"8__  8 "リ"8 8 | % 5316
    \break
    "　"8 "サ"8 "カ"8__  8 | % 5316
    "セ"8__  8 "タ"8__  8 | % 5316
    "リ"4. "ソ"8 | % 5316
    "コ"8 "ナ"8 "オ"4 | % 5316
    "ケ"4 "サ"8__  8 | % 5316
    "ニ"4 "イ"8 "ナ"8 | % 5316
    \break
    "コ"4. "ト"8 | % 5316
    "イ"8__  8 "ワ"8__  8 | % 5316
    "レ"4. 8 | % 5316
    8 "ネ"8 "マ"8 "リ"8 | % 5316
    "ネ"8 "マ"8 "ラ"8 "ズ"8 | % 5316
    "マ"8 "チ"8 "ア"8 "カ"8 | % 5316
    \break
    "ス"8 4 "ゴ"8 | % 5316
    "ザ"8 "レ"8 "ハ"16 "ナ"16 "シ"16 "マ"16 | % 5316
    "ショ"8 "　"8 "コン"8 "コ"16 "マ"16 | % 5316
    "ツ"8 "ノ"16__  16 "コ"8 "カ"16 16 | % 5316
    "ゲ"8 "デ"8 8 "マ"8 | % 5316
    \break
    "ツ"8 "ノ"8 "ハ"8 "ノ"8 | % 5316
    "ヨ"8 "ニ"8 "コン"8 "コ"16 "マ"16 | % 5316
    "ヤ"8 "カ"8 "ニ"4 | % 5316
    8 "ヒ"8 "イ"4 | % 5316
    "テ"8__  4 "ウ"8 | % 5316
    "ト"8__  4. | % 5316
    \break
    "ヤ"8__  4 "シ"8 | % 5316
    "シ"4.__  8 | % 5316
    "ノ"8__  4. | % 5316
    "　"8 4 "キョ"16.__  32 | % 5316
    4 "ク"4__  | % 5316
    2 | % 5316

}

\book {

  \score {
    <<
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Song"
        }
        <<
          \context Voice = "Part_POne_Staff_One_Voice_One" <<
            \Part_POne_Staff_One_Voice_One
          >>
          \new Lyrics
            \with {
            }
            \Part_POne_Staff_One_Voice_One_Stanza_One
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
