\version "2.24.4"

\header {
  miscellaneousField  = "Chords and fretboards assigned to 
          the voices in a multi-voice, multi-staff part. There should be fret 
          diagrams above each of the two staves."
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

Part_POne_HARMONIES_Staff_HARMONIES_VoiceEleven = \chordmode {
  \language "nederlands"
  
  \key c \major
  \time 4/4
  s2 ees4:m7.9 c:5.3 d:7 c:m711
  \bar "|."
  
}

Part_POne_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key c \major
  \time 4/4
  d''4 c'' b' a'
  \bar "|."
}

Part_POne_Staff_Two_Voice_Three = \absolute {
  \language "nederlands"
  
  \clef "bass"
  \key c \major
  \time 4/4
  d4 e f g
  \bar "|."
}

\book {

  \score {
    <<
      
      
      
      <<
      
        \new PianoStaff
        \with {
          instrumentName = "Part_POne"
        }
        
        <<
        
          \context ChordNames = "Part_POne_HARMONIES_Staff_HARMONIES_VoiceEleven"
          \Part_POne_HARMONIES_Staff_HARMONIES_VoiceEleven
          
          \new Staff  = "Part_POne_Staff_One"
          \with {
          }
          <<
            \context Voice = "Part_POne_Staff_One_Voice_One" <<
              \Part_POne_Staff_One_Voice_One
            >>
          >>
          
          \new Staff  = "Part_POne_Staff_Two"
          \with {
          }
          <<
            \context Voice = "Part_POne_Staff_Two_Voice_Three" <<
              \Part_POne_Staff_Two_Voice_Three
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
