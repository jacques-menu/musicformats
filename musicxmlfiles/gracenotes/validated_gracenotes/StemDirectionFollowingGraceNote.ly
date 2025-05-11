\version "2.24.4"

\header {
  workCreditTypeTitle = "Stem Direction Following Grace Note"
  movementTitle       = "test"
  encodingDate        = "2020-07-13"
  software            = "soundnotation"
  software            = "Dolet 6.6"
  title               = "Stem Direction Following Grace Note"
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
  \key d \major
  \numericTimeSignature \time 4/4
  \stemDown a'8 [
  \tempo \markup {
    \concat {
      ( \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      105)
    
    } % concat
  }
  d'' d'' d'' ]
d'' r \acciaccatura { \stemUp d''8 } \stemDown d'''8 \stemUp ( ) -. r }

Part_POne_Staff_Two_Voice_Five = \absolute {
  \language "nederlands"
  
  \clef "bass"
  \key d \major
  \numericTimeSignature \time 4/4
  \tempo \markup {
    \concat {
      ( \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      105)
    
    } % concat
  }
\stemUp d,4 r r r }

\book {

  \score {
    <<
      
      
      
      <<
      
        \new PianoStaff
        \with {
          instrumentName = "Part_POne"
        }
        
        <<
        
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
            \context Voice = "Part_POne_Staff_Two_Voice_Five" <<
              \Part_POne_Staff_Two_Voice_Five
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
