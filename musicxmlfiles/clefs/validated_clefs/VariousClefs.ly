\version "2.24.4"

\header {
  workCreditTypeTitle = "Various Clefs"
  miscellaneousField  = "Various clefs: G, C, F, percussion,
          TAB and none; some are also possible with transposition and on other
          staff lines than their default (e.g. soprano/alto/tenor/baritone C
          clefs); Each measure shows a different clef (measure 17 has the \"none\"
          clef), only measure 18 has the same treble clef as measure
          1."
  title               = "Various Clefs"
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
  c'1
  \clef "alto"
  c'1  | % 3
  \barNumberCheck #3
  
  \clef "tenor"
  c'1  | % 4
  \barNumberCheck #4
  
  \clef "bass"
  c'1  | % 5
  \barNumberCheck #5
  
  \clef "percussion"
  c'1  | % 6
  \barNumberCheck #6
  
  \clef "treble_8"
  c'1  | % 7
  \barNumberCheck #7
  
  \clef "bass_8"
  c'1  | % 8
  \barNumberCheck #8
  
  \clef "varbaritone"
  c'1  | % 9
  \barNumberCheck #9
  
  \clef "french"
  c'1  | % 10
  \barNumberCheck #10
  
  \clef "baritone"
  c'1  | % 11
  \barNumberCheck #11
  
  \clef "mezzosoprano"
  c'1  | % 12
  \barNumberCheck #12
  
  \clef "soprano"
  c'1  | % 13
  \barNumberCheck #13
  
  \clef "percussion"
  c'1  | % 14
  \barNumberCheck #14
  
  \clef "treble^8"
  c'1  | % 15
  \barNumberCheck #15
  
  \clef "bass^8"
  c'1  | % 16
  \barNumberCheck #16
  
  \clef "tab"
  
  c'1  | % 17
  \barNumberCheck #17
  
  c'1  | % 18
  \barNumberCheck #18
  
  \clef "treble"
  c'1
  \bar "|."
   | % 19
  \barNumberCheck #19
}

\book {

  \score {
    <<
      
      
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "MusicXML Part"
        }
        <<
          \context Voice = "Part_POne_Staff_One_Voice_One" <<
            \Part_POne_Staff_One_Voice_One
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
