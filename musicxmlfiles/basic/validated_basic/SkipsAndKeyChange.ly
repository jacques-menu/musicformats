\version "2.24.4"

\header {
  workCreditTypeTitle = "SkipsAndKeyChange"
  movementTitle       = "Partie 1"
  encodingDate        = "2024-12-09"
  software            = "Dorico 5.1.51.2153"
  title               = "SkipsAndKeyChange"
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
  \key g \major
  \stemUp g'4 b'
  
  c''2
  
  b'4 g'
  
  a'4 \key f \major
  bes' ~  
  
  bes'4 a'
  
  g'2
  
  f'2
  
  r2
  \bar "|."
  
}

Part_POne_Staff_One_Voice_Two = \absolute {
  \language "nederlands"
