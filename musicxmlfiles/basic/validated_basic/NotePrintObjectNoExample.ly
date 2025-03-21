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

_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key f \major
  \numericTimeSignature \time 4/4
  \stemDown f''4 f'' g''8 [  %{ beam 1, line 143 %}
  g'' g'' f'' ]  %{ beam 1, line 179 %}
}

_Staff_Two_Voice_Five = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key f \major
  \numericTimeSignature \time 4/4
  \stemDown d8 [  %{ beam 1, line 194 %}
   < a d' f' >  d  < a d' f' >  ] g [  %{ beam 1, line 324 %}
 < bes d' >  g  < bes d' >  ] }

_Staff_Two_Voice_Six = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key f \major
  \numericTimeSignature \time 4/4
\once\hide NoteHead \once\hide Stem \stemDown d4 e g a }

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
        
          \new Staff  = "_Staff_One"
          \with {
          }
          <<
            \context Voice = "_Staff_One_Voice_One" <<
              \_Staff_One_Voice_One
            >>
          >>
          
          \new Staff  = "_Staff_Two"
          \with {
          }
          <<
            \context Voice = "_Staff_Two_Voice_Five" <<
              \voiceOne % out of 2 regular voices
              \_Staff_Two_Voice_Five
            >>
            \context Voice = "_Staff_Two_Voice_Six" <<
              \voiceTwo % out of 2 regular voices
              \_Staff_Two_Voice_Six
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
