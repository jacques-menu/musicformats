\version "2.24.4"

\header {
  movementTitle       = "Excerpt from Clarinet Quintet, K. 581"
  encodingDate        = "2017-12-12"
  composer            = "Wolfgang Amadeus Mozart"
  software            = "Finale v25 for Mac"
  right               = "Copyright © 2010 MakeMusic, Inc."
  title               = "Excerpt from Clarinet Quintet, K. 581"
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
  
  \repeat volta 2 {
    \partial 4
    
    \clef "treble"
    \key c \major
    \numericTimeSignature \time 3/4
    \transposition a
    \stemDown c''8 \p [
    ( e'' ]
     | % 1
    \barNumberCheck #1
    g''8 [
    e'' ]
    c'''4 ) \slurUp g''8 [
    ( e'' ]
     | % 2
    \barNumberCheck #2
    d''8 [
    f'' ]
    a''4 ) \slurUp f''8 [
    ( d'' ]
     | % 3
    \barNumberCheck #3
    c''8 [
    b' e'' d'' g'' f'' ]
    )  | % 4
    \barNumberCheck #4
    \slurUp dis''!4 ( e'' ) \slurUp c''8 [
    ( e'' ]
     | % 5
    \barNumberCheck #5
    g''8 [
    e'' ]
    c'''4 ) \slurUp g''8 [
    ( e'' ]
     | % 6
    \barNumberCheck #6
    d''!8 [
    f'' ]
    a''4 ) r  | % 7
    \barNumberCheck #7
    R2.  | % 8
    \barNumberCheck #8
    
    \break | % 1333333 \myLineBreak
    
    r4 r
    \once\override TupletBracket.bracket-visibility = ##t
    \once\override TupletBracket.direction = 1 % UP
    \tuplet 3/2 {  \stemUp \slurDown d'8 [
    ( a f ]
    }
    a8 [
    ) d' _. f' _. a' _. d'' _. f'' ]
    _.  | % 9
    \barNumberCheck #10
    \stemDown \slurUp a''8 [
    ( g'' f'' e'' f'' d'' ]
    )  | % 10
    \barNumberCheck #11
    \slurUp c''2 ( e''8 [
    d'' ]
    )  | % 11
    \barNumberCheck #12
    c''4 r r4 R2.  | % 13
    \barNumberCheck #14
    R2.  | % 14
    \barNumberCheck #15
    R2.  | % 15
    \barNumberCheck #16
    r4 r \slurUp g'' (  | % 11
    \barNumberCheck #17
    r4 R2.  | % 13
    \barNumberCheck #18
    R2.  | % 14
    \barNumberCheck #19
    R2.  | % 15
    \barNumberCheck #20
    r4 r g'' (  | % 0
    \barNumberCheck #21
  }
  r4 R2.  | % 13
  \barNumberCheck #18
  R2.  | % 14
  \barNumberCheck #19
  R2.  | % 15
  \barNumberCheck #20
  r4 r g'' (  | % 0
  \barNumberCheck #21
}

Part_PTwo_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \repeat volta 2 {
    \partial 4
    
    \clef "treble"
    \key a \major
    \numericTimeSignature \time 3/4
    r4  | % 1
    \barNumberCheck #1
    r4 \p \stemUp a' a'  | % 2
    \barNumberCheck #2
    r4 a' a'  | % 3
    \barNumberCheck #3
    r4 gis' gis'  | % 4
    \barNumberCheck #4
    r4 a' a'  | % 5
    \barNumberCheck #5
    r4 a' a'  | % 6
    \barNumberCheck #6
    fis'4 r \stemDown cis''8 [
    ( ais'! ]
     | % 7
    \barNumberCheck #7
    b'8 [
    d'' ]
    fis''4 ) \slurUp cis''8 [
    ( ais'! ]
     | % 8
    \barNumberCheck #8
    \break | % 1333333 \myLineBreak
    
    b'8 [
    d'' ]
    fis''4 ) r  | % 9
    \barNumberCheck #9
    R2.  | % 10
    \barNumberCheck #10
    R2.  | % 11
    \barNumberCheck #11
    \stemUp \slurDown cis'8 [
    ( e' cis' e' d' e' ]
    )  | % 12
    \barNumberCheck #12
    cis'4 r \slurUp e'8 [
    ( gis' ]
    b'8 [
    gis' ]
    \stemDown e''4 ) \stemUp \slurUp e'8 [
    ( a' ]
     | % 14
    \barNumberCheck #14
    \stemDown cis''8 [
    a' ]
    e''4 ) \stemUp \slurUp e'8 [
    ( b' ]
     | % 15
    \barNumberCheck #15
    \stemDown d''8 [
    b' e'' d'' cis'' a' ]
    )  | % 16
    \barNumberCheck #16
    \stemUp \slurUp gis'8 [
    ( b' ]
    \stemDown e''4 ) \stemUp \slurDown e'8 [
    ( gis' ]
     | % 12
    \barNumberCheck #17
    \slurUp e'8 [
    ( gis' ]
    b'8 [
    gis' ]
    \stemDown e''4 ) \stemUp \slurUp e'8 [
    ( a' ]
     | % 14
    \barNumberCheck #18
    \stemDown cis''8 [
    a' ]
    e''4 ) \stemUp \slurUp e'8 [
    ( b' ]
     | % 15
    \barNumberCheck #19
    \stemDown d''8 [
    b' e'' d'' cis'' a' ]
    )  | % 16
    \barNumberCheck #20
    \stemUp \slurUp gis'8 [
    ( b' ]
    \stemDown e''4 ) \stemUp \slurDown e'8 [
    ( gis' ]
     | % 0
    \barNumberCheck #21
  }
  \slurUp e'8 [
  ( gis' ]
  b'8 [
  gis' ]
  \stemDown e''4 ) \stemUp \slurUp e'8 [
  ( a' ]
   | % 14
  \barNumberCheck #18
  \stemDown cis''8 [
  a' ]
  e''4 ) \stemUp \slurUp e'8 [
  ( b' ]
   | % 15
  \barNumberCheck #19
  \stemDown d''8 [
  b' e'' d'' cis'' a' ]
  )  | % 16
  \barNumberCheck #20
  \stemUp \slurUp gis'8 [
  ( b' ]
  \stemDown e''4 ) \stemUp \slurDown e'8 [
  ( gis' ]
   | % 0
  \barNumberCheck #21
}

Part_PThree_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \repeat volta 2 {
    \partial 4
    
    \clef "treble"
    \key a \major
    \numericTimeSignature \time 3/4
    r4  | % 1
    \barNumberCheck #1
    r4 \p \stemUp e' e'  | % 2
    \barNumberCheck #2
    r4 fis' fis'  | % 3
    \barNumberCheck #3
    r4 d' d'  | % 4
    \barNumberCheck #4
    r4 cis' cis'  | % 5
    \barNumberCheck #5
    r4 e' e'  | % 6
    \barNumberCheck #6
    d'4 r g'! (  | % 7
    \barNumberCheck #7
    fis'2 g'!4  | % 8
    \barNumberCheck #8
    \break | % 1333333 \myLineBreak
    
    fis'2 ) r4  | % 9
    \barNumberCheck #9
    R2.  | % 10
    \barNumberCheck #10
    R2.  | % 11
    \barNumberCheck #11
    \slurDown a2 ( gis!4 )  | % 12
    \barNumberCheck #12
    a4 r r4  < b gis' > 4 -\p -\p ^\markup { pizz. } ^\markup { pizz. }  < b gis' >  r  | % 14
    \barNumberCheck #14
     < a a' > 4  < a a' >  r  | % 15
    \barNumberCheck #15
     < gis' b' > 4  < gis' b' >  \stemDown  < a' cis'' >   | % 16
    \barNumberCheck #16
    \stemUp  < gis' b' > 4  < gis' b' >  r  | % 12
    \barNumberCheck #17
    r4  < b gis' > 4 -\p -\p ^\markup { pizz. } ^\markup { pizz. }  < b gis' >  r  | % 14
    \barNumberCheck #18
     < a a' > 4  < a a' >  r  | % 15
    \barNumberCheck #19
     < gis' b' > 4  < gis' b' >  \stemDown  < a' cis'' >   | % 16
    \barNumberCheck #20
    \stemUp  < gis' b' > 4  < gis' b' >  r  | % 0
    \barNumberCheck #21
  }
  r4  < b gis' > 4 -\p -\p ^\markup { pizz. } ^\markup { pizz. }  < b gis' >  r  | % 14
  \barNumberCheck #18
   < a a' > 4  < a a' >  r  | % 15
  \barNumberCheck #19
   < gis' b' > 4  < gis' b' >  \stemDown  < a' cis'' >   | % 16
  \barNumberCheck #20
  \stemUp  < gis' b' > 4  < gis' b' >  r  | % 0
  \barNumberCheck #21
}

Part_PFour_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \repeat volta 2 {
    \partial 4
    
    \clef "alto"
    \key a \major
    \numericTimeSignature \time 3/4
    r4  | % 1
    \barNumberCheck #1
    r4 \p \stemDown cis' cis'  | % 2
    \barNumberCheck #2
    r4 \stemUp b b  | % 3
    \barNumberCheck #3
    r4 b b  | % 4
    \barNumberCheck #4
    r4 a a  | % 5
    \barNumberCheck #5
    r4 \stemDown cis' cis'  | % 6
    \barNumberCheck #6
    \stemUp b4 r \stemDown \slurUp e' (  | % 7
    \barNumberCheck #7
    d'2 e'4  | % 8
    \barNumberCheck #8
    \break | % 1333333 \myLineBreak
    
    d'2 ) r4  | % 9
    \barNumberCheck #9
    R2.  | % 10
    \barNumberCheck #10
    R2.  | % 11
    \barNumberCheck #11
    \stemUp e2. ~   | % 12
    \barNumberCheck #12
    e4 r r4 \stemDown  < d' e' > 4 -\p -\p ^\markup { pizz. } ^\markup { pizz. }  < d' e' >  r  | % 14
    \barNumberCheck #14
     < cis' e' > 4  < cis' e' >  r  | % 15
    \barNumberCheck #15
    e'4 e' e'  | % 16
    \barNumberCheck #16
    e'4 e' r  | % 12
    \barNumberCheck #17
    r4  < d' e' > 4 -\p -\p ^\markup { pizz. } ^\markup { pizz. }  < d' e' >  r  | % 14
    \barNumberCheck #18
     < cis' e' > 4  < cis' e' >  r  | % 15
    \barNumberCheck #19
    e'4 e' e'  | % 16
    \barNumberCheck #20
    e'4 e' r  | % 0
    \barNumberCheck #21
  }
  r4  < d' e' > 4 -\p -\p ^\markup { pizz. } ^\markup { pizz. }  < d' e' >  r  | % 14
  \barNumberCheck #18
   < cis' e' > 4  < cis' e' >  r  | % 15
  \barNumberCheck #19
  e'4 e' e'  | % 16
  \barNumberCheck #20
  e'4 e' r  | % 0
  \barNumberCheck #21
}

Part_PFive_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \repeat volta 2 {
    \partial 4
    
    \clef "bass"
    \key a \major
    \numericTimeSignature \time 3/4
    r4  | % 1
    \barNumberCheck #1
    \stemDown a4 \p r r  | % 2
    \barNumberCheck #2
    d4 r r  | % 3
    \barNumberCheck #3
    e4 r r  | % 4
    \barNumberCheck #4
    fis4 r r  | % 5
    \barNumberCheck #5
    \stemUp cis4 r r  | % 6
    \barNumberCheck #6
    \stemDown d4 r r  | % 7
    \barNumberCheck #7
    R2.  | % 8
    \barNumberCheck #8
    \break | % 1333333 \myLineBreak
    
    R2.  | % 9
    \barNumberCheck #9
    R2.  | % 10
    \barNumberCheck #10
    R2.  | % 11
    \barNumberCheck #11
    \stemUp \slurDown e,4 ( _. e, _. e, ) _.  | % 12
    \barNumberCheck #12
    a,4 r r4 \stemDown e4 ^\markup { "pizz." } \p e r  | % 14
    \barNumberCheck #14
    e4 e r  | % 15
    \barNumberCheck #15
    e4 e e  | % 16
    \barNumberCheck #16
    e4 \stemUp e, r  | % 12
    \barNumberCheck #17
    r4 \stemDown e4 ^\markup { "pizz." } \p e r  | % 14
    \barNumberCheck #18
    e4 e r  | % 15
    \barNumberCheck #19
    e4 e e  | % 16
    \barNumberCheck #20
    e4 \stemUp e, r  | % 21
    \barNumberCheck #21
  }
  r4 \stemDown e4 ^\markup { "pizz." } \p e r  | % 14
  \barNumberCheck #18
  e4 e r  | % 15
  \barNumberCheck #19
  e4 e e  | % 16
  \barNumberCheck #20
  e4 \stemUp e, r  | % 21
  \barNumberCheck #21
}

\book {

  \score {
    <<
      
      
      
      <<
      
        
        \new StaffGroup
        
        
        <<
        
          \new Staff  = "Part_POne_Staff_One"
          \with {
            instrumentName = "clarinet in A"
          }
          <<
            \context Voice = "Part_POne_Staff_One_Voice_One" <<
              \Part_POne_Staff_One_Voice_One
            >>
          >>
          \new Staff  = "Part_PTwo_Staff_One"
          \with {
            instrumentName = "violino I"
          }
          <<
            \context Voice = "Part_PTwo_Staff_One_Voice_One" <<
              \Part_PTwo_Staff_One_Voice_One
            >>
          >>
          \new Staff  = "Part_PThree_Staff_One"
          \with {
            instrumentName = "violino II"
          }
          <<
            \context Voice = "Part_PThree_Staff_One_Voice_One" <<
              \Part_PThree_Staff_One_Voice_One
            >>
          >>
          \new Staff  = "Part_PFour_Staff_One"
          \with {
            instrumentName = "viola"
          }
          <<
            \context Voice = "Part_PFour_Staff_One_Voice_One" <<
              \Part_PFour_Staff_One_Voice_One
            >>
          >>
          \new Staff  = "Part_PFive_Staff_One"
          \with {
            instrumentName = "violoncello"
          }
          <<
            \context Voice = "Part_PFive_Staff_One_Voice_One" <<
              \Part_PFive_Staff_One_Voice_One
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
