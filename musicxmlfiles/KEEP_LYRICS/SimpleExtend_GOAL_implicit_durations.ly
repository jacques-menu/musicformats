\version "2.24.0"

% Generated by xml2ly vv0.9.70 (June 1, 2023)
% on Thursday 2023-06-01 @ 07:11:30 CEST
% from "SimpleExtend.xml"

% The conversion date was: Thursday 2023-06-01 @ 07:11:30 CEST

% The conversion command as supplied was:
%  xml2ly -auto-output-file-name -lilypond-generation-infos SimpleExtend.xml -dlpsr -lilylyrdurkind implicit
% or, with long option names:
%  xml2ly -auto-output-file-name -lilypond-generation-infos SimpleExtend.xml -display-lpsr -lilypond-lyrics-durations-style implicit
% or, with short option names:
%     SimpleExtend.xml  implicit

\header {
  movementTitle       = "Simple Extend"
  encodingDate        = "2023-06-01"
  software            = "MuseScore 4.0.2"
  title               = "Simple Extend"
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

  oddHeaderMarkup = ""
  evenHeaderMarkup = ""
  oddFooterMarkup = ""
  evenFooterMarkup = ""
}

\layout {
  \context {
    \Score
    autoBeaming = ##f % to display tuplets brackets
  }
  \context {
    \Voice
  }
}

Part_POne_Staff_One_Voice_One = \absolute {
  \language "nederlands"

  \clef "treble"
  \key c \major
  \numericTimeSignature \time 2/4
  c'8 r8 e'4  ~ | % 2
  \barNumberCheck #2
  e'8 g'4 g'8 | % 3
  \barNumberCheck #3
  c'4 r4
  \bar "|."
  | % 4
  \barNumberCheck #4
}

Part_POne_Staff_One_Voice_One_Stanza_One = \lyricmode {
  \set associatedVoice = #"Part_POne_Staff_One_Voice_One"
  \set ignoreMelismata = ##t
Bon ma -- \skip1 "tin!" Bon "jour!"
}

Part_POne_Staff_One_Voice_One_Stanza_Two = \lyricmode {
  \set associatedVoice = #"Part_POne_Staff_One_Voice_One"
  \set ignoreMelismata = ##t
Bon "soir!" __ \skip1 \skip1 %{ FOO 1 %} Bonne "nuit!"
}

\book {

  \score {
    <<

      \new Staff = "Part_POne_Staff_One"
      \with {
        instrumentName = "Part_POne"
      }
      <<
        \context Voice = "Part_POne_Staff_One_Voice_One" <<
          \Part_POne_Staff_One_Voice_One
        >>
        \new Lyrics
          \with {
          }
          \lyricsto "Part_POne_Staff_One_Voice_One" {\Part_POne_Staff_One_Voice_One_Stanza_One}
        \new Lyrics
          \with {
          }
          \lyricsto "Part_POne_Staff_One_Voice_One" {\Part_POne_Staff_One_Voice_One_Stanza_Two}
      >>

    >>

    \layout {
      \context {
        \Score
        autoBeaming = ##f % to display tuplets brackets
      }
      \context {
        \Voice
      }
    }

    \midi {
      \tempo 16= 360
    }
  }

}