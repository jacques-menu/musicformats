\version "2.24.4"

\header {
  movementTitle       = "NotePrintObjectNoExample"
  encodingDate        = "2019-02-04"
  software            = "soundnotation"
  software            = "Dolet 6.6"
  right               = ""
  title               = "NotePrintObjectNoExample"
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
  \stemDown f''4 f'' g''8 [
  g'' g'' f'' ]
  
}

Part_POne_Staff_Two_Voice_Five = \absolute {
  \language "nederlands"
  
  
  \clef "treble"
  \key f \major
  \stemDown d8 [
   < a d' f' >  d  < a d' f' >  ] g [
   < bes d' >  g  < bes d' >  ]
}

Part_POne_Staff_Two_Voice_Six = \absolute {
  \language "nederlands"
  
  
  \clef "treble"
  \key f \major
  \once\hide NoteHead \once\hide Stem \stemDown d4 e g a
}

\book {

  \score {
    <<
    
      
      
      
      <<
      
        \new PianoStaff
        \with {
          instrumentName = "Part_POne"
          shortInstrumentName = "Pno."
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
              \voiceOne % out of 2 regular voices
              \Part_POne_Staff_Two_Voice_Five
            >>
            \context Voice = "Part_POne_Staff_Two_Voice_Six" <<
              \voiceTwo % out of 2 regular voices
              \Part_POne_Staff_Two_Voice_Six
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
