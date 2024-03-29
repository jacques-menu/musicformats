\version "2.25.0"

% Generated by xml2ly v0.9.70 (January 2, 2024)
% on Wednesday 2024-01-03 @ 17:41:10 CET
% from "Anacrusis.xml"

% The conversion date was: Wednesday 2024-01-03 @ 17:41:10 CET

% The conversion command as supplied was: 
%  xml2ly -auto-output-file-name -lilypond-generation-infos Anacrusis.xml -tpasses
% or, with long option names:
%  xml2ly -auto-output-file-name -lilypond-generation-infos Anacrusis.xml -trace-passes
% or, with short option names:
%     Anacrusis.xml

\header {
  workCreditTypeTitle = "Anacrusis"
  encodingDate        = "2016-11-22"
  software            = "MuseScore 2.0.3.1"
  title               = "Anacrusis"
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
  
  % oddHeaderMarkup = ""
  % evenHeaderMarkup = ""
  % oddFooterMarkup = ""
  % evenFooterMarkup = ""
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
  \partial 4
  
  \clef "treble"
  \key c \major
  \numericTimeSignature \time 3/4
  \stemUp e'8 [ f' ]  | % 1
  \barNumberCheck #1
  a'4 g' \stemDown c''  | % 2
  \barNumberCheck #2
  c''2 r4
  \bar "|."
   | % 3
  \barNumberCheck #3
}

\book {

  \score {
    <<
      
      \new Staff = "Part_POne_Staff_One"
      \with {
        instrumentName = "Voice"
      }
      <<
        \context Voice = "Part_POne_Staff_One_Voice_One" <<
          \Part_POne_Staff_One_Voice_One
        >>
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
