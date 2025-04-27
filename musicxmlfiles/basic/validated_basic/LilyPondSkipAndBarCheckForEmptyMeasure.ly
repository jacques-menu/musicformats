\version "2.24.4"

\header {
  workCreditTypeTitle = "LilyPondSkipAndBarCheckForEmptyMeasure"
  encodingDate        = "2024-10-14"
  software            = "Sibelius 20240.6"
  software            = "Dolet 6.6 for Sibelius"
  title               = "LilyPondSkipAndBarCheckForEmptyMeasure"
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
  \stemUp g'2 ~  
  
  g'2 \bar "||"
  
  
  \key d \major
  a'2 ~  
  
  a'2
  \bar "|."
  
}

Part_POne_Staff_One_Voice_Two = \absolute {
  \language "nederlands"
