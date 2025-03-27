\version "2.24.4"

\header {
  movementTitle       = "Unpitched Tuplets"
  encodingDate        = "2017-05-20"
  software            = "MuseScore 2.0.3.1"
  title               = "Unpitched Tuplets"
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

Part_PFour_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  %{ begin kMeasureKindOverFlowing, measure 1, % measureCurrentPositionInMeasure: pim 3/2 %}
  
  \clef "percussion"
  \key c \major
  
  \stopStaff
  \override Staff.StaffSymbol.line-count = 1
  \startStaff
  \stemDown e4 r \once \omit TupletBracket
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 118 %}  e8 :16 [  %{ beam 1, line 132 %}
  e e ]  %{ beam 1, line 170 %}
  } \once \omit TupletBracket
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 175 %}  e8 :16 [  %{ beam 1, line 189 %}
  e e ]  %{ beam 1, line 227 %}
  }
  \bar "|."  %{ b333 visitStart (S_msrBarLine& elt) %}
  
  %{ end kMeasureKindOverFlowing, measure  %}
}

\book {

  \score {
    <<
    
      
      
      
      <<
      
        \new DrumStaff  = "Part_PFour_Staff_One"
        \with {
          instrumentName = "Snare Drum 4"
          shortInstrumentName = "Sn. Dr."
        }
        <<
          \context DrumVoice = "Part_PFour_Staff_One_Voice_One" <<
            \Part_PFour_Staff_One_Voice_One
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
