\version "2.24.4"

\header {
  movementTitle       = "Chords In Tuplets 2"
  encodingDate        = "2017-01-23"
  software            = "MuseScore 2.0.3.1"
  title               = "Chords In Tuplets 2"
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
  \stemDown  < c'' e'' g'' > 4  < b' d'' f'' >   < a' c'' e'' >   < c'' e'' g'' >   < b' d'' f'' >   < a' c'' e'' >  
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 3/2 {   < c'' e'' g'' > 4  < b' d'' f'' >   < a' c'' e'' >  }  < g' b' d'' >  \stemUp  < f' a' c'' >  \stemDown  < e' g' b' >   < g' b' d'' >  \stemUp  < f' a' c'' >  \stemDown  < e' g' b' >  
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 3/2 {   < g' b' d'' > 4 \stemUp  < f' a' c'' >  \stemDown  < e' g' b' >  }
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
