\version "2.24.4"

% Generated from "MultipleStanzas.xml"
% by xml2ly v2026.2 (built on June 02, 2026)
% on Tuesday 2026-06-02 @ 08:20:33 CEST

% The conversion command as supplied was: 
%  xml2ly -lilypond-run-date -lilypond-generation-infos -output-file-name MultipleStanzas.ly MultipleStanzas.xml -lilypond-lyrics-durations-mode explicit
% or, with short option names:
%     MultipleStanzas.ly MultipleStanzas.xml explicit


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
  encodingDate        = "2016-09-05"
  software            = "MuseScore 2.0.3"
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
}

Part_POne_Staff_One_Voice_One_Stanza_One = \lyricmode {
  % ===> using explicit lyrics durations mode
  \set associatedVoice  = #"Part_POne_Staff_One_Voice_One#"
  \set ignoreMelismata = ##t
    "1. Sing"8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 153 ???%} out8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 201 ???%} all8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 249 ???%} your8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 294 ???%} prais4 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 341 ???%} es4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 72 ???%} | % 5316

}

Part_POne_Staff_One_Voice_One_Stanza_Two = \lyricmode {
  % ===> using explicit lyrics durations mode
  \set associatedVoice  = #"Part_POne_Staff_One_Voice_One#"
  \set ignoreMelismata = ##t
    "2. For"8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 157 ???%} the8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 205 ???%} Son8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 253 ???%} has8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 298 ???%} ris4 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 345 ???%} en4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 72 ???%} | % 5316

}

Part_POne_Staff_One_Voice_One_Stanza_Three = \lyricmode {
  % ===> using explicit lyrics durations mode
  \set associatedVoice  = #"Part_POne_Staff_One_Voice_One#"
  \set ignoreMelismata = ##t
    "3. No"8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 161 ???%} more8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 209 ???%} let8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 257 ???%} the8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 302 ???%} voice4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 349 ???%} of4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 72 ???%} | % 5316

}

Part_POne_Staff_One_Voice_One_Stanza_Four = \lyricmode {
  % ===> using explicit lyrics durations mode
  \set associatedVoice  = #"Part_POne_Staff_One_Voice_One#"
  \set ignoreMelismata = ##t
    "4. For"8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 165 ???%} your8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 213 ???%} guilt8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 261 ???%} is8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 306 ???%} nailed4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 353 ???%} un4 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 72 ???%} | % 5316

}

Part_POne_Staff_One_Voice_One_Stanza_Five = \lyricmode {
  % ===> using explicit lyrics durations mode
  \set associatedVoice  = #"Part_POne_Staff_One_Voice_One#"
  \set ignoreMelismata = ##t
    "5. Death"8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 169 ???%} and8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 217 ???%} all8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 265 ???%} his8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 310 ???%} min4 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 357 ???%} ions4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 72 ???%} | % 5316

}

Part_POne_Staff_One_Voice_One_Stanza_Six = \lyricmode {
  % ===> using explicit lyrics durations mode
  \set associatedVoice  = #"Part_POne_Staff_One_Voice_One#"
  \set ignoreMelismata = ##t
    "6. Je"8 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 173 ???%} sus8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 221 ???%} Christ8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 269 ???%} is8 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/8 whn, line 314 ???%} ris4 -- %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 361 ???%} "en!"4 %{ wholeNotesAsLilypondString(): numerator is not positive in -1/4 whn, line 72 ???%} | % 5316

}

Part_PTwo_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  
  \clef "bass"
  \key ees \major
  \numericTimeSignature \time 4/4
  \stemDown  < c g > 8 [  < bes, g >   < c g >   < c g >  ]  < d aes > 4  < ees bes >  
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
