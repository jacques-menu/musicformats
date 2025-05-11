\version "2.24.4"

\header {
  movementTitle       = "Test 1"
  encodingDate        = "2025-02-28"
  software            = "Sibelius 20240.6"
  software            = "Dolet 6.6 for Sibelius"
  title               = "Test 1"
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
  \key f \major
  \numericTimeSignature \time 4/4
  \stemUp a'4 -- \tempo \markup {
    \concat {
      ( \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      109)
    
    } % concat
  }
  f' -- g'8 [
  -- g'16 ( a' ]
  bes' [
  a' g'8 ]
  ) \stemDown c''8. [
  c''16 ]
  \stemUp  < f' a' d'' > 8 [ a' ]
  \stemDown c''4 ~  \stemUp c''8. [
  f'16 ]
  
  \bar "|."
   | % 1
  \barNumberCheck #3
}

Part_POne_Staff_Two_Voice_Five = \absolute {
  \language "nederlands"
  
  \clef "bass"
  \key f \major
  \numericTimeSignature \time 4/4
  \tempo \markup {
    \concat {
      ( \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      109)
    
    } % concat
  }
  \stemUp g,4 -- \stemDown  < f bes >  -- \stemUp c, -- \stemDown  < e bes >  -- \stemUp f,4 -- \stemDown  < f a >  -- \stemUp d, -- \stemDown  < f a >  --
  \bar "|."
   | % 3
  \barNumberCheck #3
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
