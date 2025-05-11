\version "2.24.4"

\header {
  movementTitle       = "Simple Backup"
  encodingDate        = "2023-03-05"
  software            = "MuseScore 3.6.2"
  title               = "Simple Backup"
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
  \partial 2.
  
  \clef "treble"
  \key c \major
  \time 4/4
  \stemUp c'4 d' \once\omit Rest r  | % 0
  \barNumberCheck #1
}

Part_POne_Staff_One_Voice_Two = \absolute {
  \language "nederlands"
  \partial 2.
  
  \clef "treble"
  \key c \major
  \time 4/4
  s4 \stemDown a b  | % 1
  \barNumberCheck #1
}

\book {

  \score {
    <<
      
      
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Part_POne"
        }
        <<
          \context Voice = "Part_POne_Staff_One_Voice_One" <<
            \voiceOne % out of 2 regular voices
            \Part_POne_Staff_One_Voice_One
          >>
          \context Voice = "Part_POne_Staff_One_Voice_Two" <<
            \voiceTwo % out of 2 regular voices
            \Part_POne_Staff_One_Voice_Two
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
