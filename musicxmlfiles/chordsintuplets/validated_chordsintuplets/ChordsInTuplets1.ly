\version "2.24.4"

\header {
  movementTitle       = "Chords In Tuplets 1"
  encodingDate        = "2024-01-19"
  software            = "MuseScore 4.1.1"
  title               = "Chords In Tuplets 1"
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
  \stemDown  < c'' e'' > 4  < a' c'' >   < e' g' >   < c'' e'' >   < a' c'' >   < e' g' >  
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 3/2 {   < c'' e'' > 4  < a' c'' >   < e' g' >  } r2
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
