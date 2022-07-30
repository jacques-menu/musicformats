//
//  musicxmlquery.cpp
//  GuidoKit
//
//  Created by Arshia Cont on 23/12/16.
//  Copyright © 2016 Antescofo SAS. All rights reserved.
//

#include "musicxmlquery.h"

#include "libmusicxml.h"
#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include <sstream>

// Transposition operations on MusicXML
#include "xml.h"
#include "xmlreader.h"
#include "xml_tree_browser.h"

namespace MusicXML2
{
    /// Constructors and Deconstructors
    musicxmlQuery::musicxmlQuery() {
        init();
    }
    
    musicxmlQuery::~musicxmlQuery() {
    }
    
    void musicxmlQuery::init() {

    }
    
    
    int musicxmlQuery::getStavesForFirstPart() {
        return stavesInPart.begin()->second;
    }
    
    std::vector<std::string> musicxmlQuery::getAllClefsOfFirstPart() {
        std::set<string> clefsSet = clefsInPart.begin()->second;
        std::vector<string> clefsVector(clefsSet.begin(), clefsSet.end());
        return clefsVector;
    }
    
    
    int musicxmlQuery::getTotalStaves() {
        int totalStaves = 0;
        for (auto&& e : stavesInPart) {
            totalStaves += e.second;
        }
        return totalStaves;
    }

    /// Instance methods:
    int  musicxmlQuery::getTransposeInstrumentChromatic () {
        // The chromatic element, representing the number of chromatic steps to add to the written pitch, is the one required element. The diatonic, octave-change, and double elements are optional elements.
        return fChromatic + (transposevisitor::fOctaveChange * 12);
    }
    
    std::string musicxmlQuery::getTransposeInstrumentName() {
        switch (fChromatic) {
            case -2:
                return "Bb";
                break;
                
            case -3:
                return "A";
                break;
                
            case -5:
                return "G";
                break;
                
            case 3:
                return "Eb";
                break;
                
            case 2:
                return "D";
                break;
                
            default:
                return "C";
                break;
        }
    }
    
    //________________________________________________________________________
    void musicxmlQuery::visitStart ( S_part& elt)
    {
        currentPart = elt->getAttributeValue("id");
        stavesInPart[currentPart] = 1;
    }
    
    //________________________________________________________________________
    void musicxmlQuery::visitStart ( S_staves& elt)
    {
        stavesInPart[currentPart] = int(*elt);
    }
    
    void musicxmlQuery::visitEnd ( S_clef& elt )
    {
        std::string key;
        if(clefvisitor::fSign == "G" && clefvisitor::fLine == 2 && clefvisitor::fOctaveChange == 0) {
            key = "g2";
        }else if(clefvisitor::fSign == "G" && clefvisitor::fLine == 2 && clefvisitor::fOctaveChange == -1) {
            key = "g-8";
        }else if(clefvisitor::fSign == "G" && clefvisitor::fLine == 2 && clefvisitor::fOctaveChange == 1) {
            key = "g+8";
        }else if(clefvisitor::fSign == "F" && clefvisitor::fLine == 4 && clefvisitor::fOctaveChange == 0) {
            key = "f4";
        }else if(clefvisitor::fSign == "F" && clefvisitor::fLine == 4 && clefvisitor::fOctaveChange == -1) {
            key = "f-8";
        }else if(clefvisitor::fSign == "F" && clefvisitor::fLine == 4 && clefvisitor::fOctaveChange == 1) {
            key = "f+8";
        }else if(clefvisitor::fSign == "C" && clefvisitor::fLine == 3 && clefvisitor::fOctaveChange == 0) {
            key = "alto";
        }else if(clefvisitor::fSign == "C" && clefvisitor::fLine == 4 && clefvisitor::fOctaveChange == 0) {
            key = "tenor";
        }else{
            key = "unknown";
        }
        clefsInPart[currentPart].insert(key);
    }

}
