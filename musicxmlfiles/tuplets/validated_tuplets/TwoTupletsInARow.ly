\version "2.24.4"

\header {
  workCreditTypeTitle = "Two Tuplets In A Row"
  movementTitle       = "Two Tuplets In A Row"
  encodingDate        = "2025-02-10"
  composer            = "Lars P. Opfermann"
  software            = "Dorico 5.1.81.2225"
  right               = "©"
  title               = "Two Tuplets In A Row"
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
  
  %{ begin kMeasureKindOverFlowing, measure 1, % measureCurrentPositionInMeasure: pim 5/4 %}
  
  \clef "treble"
  \key c \major
  \numericTimeSignature \time 3/4
  
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 94 %}  r %{ sr333 %}  8 \stemUp g'8. \f [  %{ beam 1, line 142 %}
  _- a'16 ]  %{ beam 1, line 165 %}
  _- } \once \omit TupletBracket
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 179 %}  a'8 [  %{ beam 1, line 195 %}
  bis'!16 _. b'! _. cis''! _. d'' ]  %{ beam 1, line 301 %}
  _. } r4
  \bar "|."  %{ b333 visitStart (S_msrBarLine& elt) %}
  
  %{ end kMeasureKindOverFlowing, measure  %}
}

\book {

  \score {
    <<
    
      
      
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Flöte"
          shortInstrumentName = "Fl."
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
