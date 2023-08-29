\version "2.25.5"
% automatically converted by musicxml2ly from BackupInBetweenTupletMembers_musicxml2ly.xml
\pointAndClickOff

\header {
    title =  "Tuplets In Multi-Voice Staff"
    }

\layout {
    \context { \Score
        autoBeaming = ##f
        }
    }
PartPOneVoiceOne =  \relative a {
    \clef "bass" \time 3/4 \key f \major \partial 4 \times 2/3 {
        \stemUp a8 [ \stemUp c8 \stemUp f8 ] }
    }

PartPOneVoiceTwo =  \relative f {
    \clef "bass" \time 3/4 \key f \major \partial 4 \stemDown f2 }


% The score definition
\score {
    <<
        
        \new Staff
        <<
            \set Staff.instrumentName = "Klavier"
            \set Staff.shortInstrumentName = "Pno."
            
            \context Staff = "1" << 
                \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                \context Voice = "PartPOneVoiceOne" {  \voiceOne \PartPOneVoiceOne }
                \context Voice = "PartPOneVoiceTwo" {  \voiceTwo \PartPOneVoiceTwo }
                >>
            >>
        
        >>
    \layout {}
    % To create MIDI output, uncomment the following line:
    %  \midi {\tempo 4 = 100 }
    }

