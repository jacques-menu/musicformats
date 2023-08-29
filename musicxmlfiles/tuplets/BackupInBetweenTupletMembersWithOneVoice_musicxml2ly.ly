\version "2.25.5"
% automatically converted by musicxml2ly from BackupInBetweenTupletMembersWithOneVoice_musicxml2ly.xml
\pointAndClickOff

\header {
    title =  "Backup Between Tuplet Members With One Voice"
    subtitle =  "Backup Between Tuplet Members With One Voice"
    }

\layout {
    \context { \Score
        autoBeaming = ##f
        }
    }
PartPOneVoiceOne =  \relative a {
    \clef "bass" \time 3/4 \key f \major \partial 4 \times 2/3 {
        \stemDown <a f>8 [ \stemUp c8 \stemUp f8 ] }
    }


% The score definition
\score {
    <<
        
        \new Staff
        <<
            \set Staff.instrumentName = "Klavier"
            \set Staff.shortInstrumentName = "Pno."
            
            \context Staff << 
                \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                \context Voice = "PartPOneVoiceOne" {  \PartPOneVoiceOne }
                >>
            >>
        
        >>
    \layout {}
    % To create MIDI output, uncomment the following line:
    %  \midi {\tempo 4 = 100 }
    }

