\version "2.24.4"

\header {
  movementTitle       = "OctaveShiftInChord"
  encodingDate        = "2018-10-19"
  software            = "Sibelius 18.5"
  software            = "Dolet 6.6 for Sibelius"
  title               = "OctaveShiftInChord"
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

Part_PTwo_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key a \minor
  \numericTimeSignature \time 4/4
  
  \arpeggioNormal \ottava #1 < b'' ~  e''' ~  b''' ~  > 1 -\arpeggio \stemDown  < b'' e''' b''' > 2  < f''' c'''' >   | % 3
  \barNumberCheck #3
  
  \arpeggioNormal < b'' ~  e''' ~  b''' ~  > 1 -\arpeggio  | % 4
  \barNumberCheck #4
   < b'' e''' b''' > 2
  \arpeggioNormal  \ottava #0 < f'' bes''! f''' >  -\arpeggio  | % 1
  \barNumberCheck #5
}

Part_PTwo_Staff_Two_Voice_Five = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key a \minor
  \numericTimeSignature \time 4/4
  a'1 ~  a'1  | % 3
  \barNumberCheck #3
  a'1 ~   | % 4
  \barNumberCheck #4
  a'1  | % 5
  \barNumberCheck #5
}

\book {

  \score {
    <<
    
      
      
      
      <<
      
        \new PianoStaff
        \with {
          instrumentName = "Part_PTwo"
          shortInstrumentName = "Pno."
        }
        
        <<
        
          \new Staff  = "Part_PTwo_Staff_One"
          \with {
          }
          <<
            \context Voice = "Part_PTwo_Staff_One_Voice_One" <<
              \Part_PTwo_Staff_One_Voice_One
            >>
          >>
          
          \new Staff  = "Part_PTwo_Staff_Two"
          \with {
          }
          <<
            \context Voice = "Part_PTwo_Staff_Two_Voice_Five" <<
              \Part_PTwo_Staff_Two_Voice_Five
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
