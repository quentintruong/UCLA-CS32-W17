#include "provided.h"
#include "MyMap.h"
#include <iostream>
#include <vector>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
    MyMap<string, vector<StreetSegment>> m_data;
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
    unsigned long numSeg = ml.getNumSegments();
    for (int a = 0; a < numSeg; a++){
        StreetSegment streetSeg;
        ml.getSegment(a, streetSeg);
     
        vector<StreetSegment> streetSegsStart;
        vector<StreetSegment>* streetSegsStartPtr = m_data.find(streetSeg.segment.start.latitudeText + streetSeg.segment.start.longitudeText);
        if (streetSegsStartPtr == nullptr){
            //std::cerr << "!=";
            streetSegsStart.push_back(streetSeg);
            m_data.associate(streetSeg.segment.start.latitudeText + streetSeg.segment.start.longitudeText, streetSegsStart);
        }
        if (streetSegsStartPtr != nullptr){
            //std::cerr << "==";
            streetSegsStartPtr->push_back(streetSeg);
        }
        
        vector<StreetSegment> streetSegsEnd;
        vector<StreetSegment>* streetSegsEndPtr = m_data.find(streetSeg.segment.end.latitudeText + streetSeg.segment.end.longitudeText);
        if (streetSegsEndPtr == nullptr){
            //std::cerr << "!=";
            streetSegsEnd.push_back(streetSeg);
            m_data.associate(streetSeg.segment.end.latitudeText + streetSeg.segment.end.longitudeText, streetSegsEnd);
        }
        if (streetSegsEndPtr != nullptr){
            //std::cerr << "==";
            streetSegsEndPtr->push_back(streetSeg);
        }
        
        for (int b = 0; b < streetSeg.attractions.size(); b++){
            
            vector<StreetSegment> streetSegsAtt;
            vector<StreetSegment>* streetSegsAttPtr = m_data.find(streetSeg.attractions[b].geocoordinates.latitudeText + streetSeg.attractions[b].geocoordinates.longitudeText);
            if (streetSegsAttPtr == nullptr){
                //std::cerr << "!=";
                streetSegsAtt.push_back(streetSeg);
                m_data.associate(streetSeg.attractions[b].geocoordinates.latitudeText + streetSeg.attractions[b].geocoordinates.longitudeText, streetSegsAtt);
            }
            if (streetSegsAttPtr != nullptr){
                //std::cerr << "==";
                streetSegsAttPtr->push_back(streetSeg);
            }
            
        }
        //std::cerr << a << std::endl;
    }
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
    return *m_data.find(gc.latitudeText + gc.longitudeText);
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}




/*
int main(){
    MapLoader ml;
    ml.load("mapdata.txt");
    
    SegmentMapper sm;
    sm.init(ml);
    
    vector<StreetSegment> a(sm.getSegments(GeoCoord("34.0552850", "-118.4132600")));
    std::cerr << a[0].streetName << std::endl;
    vector<StreetSegment> b(sm.getSegments(GeoCoord("34.0612088", "-118.4470689")));
    std::cerr << b[0].streetName << std::endl;
    vector<StreetSegment> c(sm.getSegments(GeoCoord("34.0619693", "-118.4479670")));
    std::cerr << c[0].streetName << " " << c[1].streetName << " " << c[2].streetName << " " << c[3].streetName << " " << c.size() << std::endl;
 

    vector<StreetSegment> bcm(sm.getSegments(GeoCoord("34.0474215", "-118.4901897")));
    std::cerr << bcm.size() << " " << bcm[0].streetName << std::endl;//1 26th Street
    std::cerr << bcm[0].segment.start.latitudeText << " " << bcm[0].segment.start.longitudeText << std::endl;//34.0476495 -118.4910837
    std::cerr << bcm[0].segment.end.latitudeText << " " << bcm[0].segment.end.longitudeText << std::endl;//34.0469402 -118.4902543
    
    vector<StreetSegment> bcmS(sm.getSegments(GeoCoord("34.0476495", "-118.4910837")));
    std::cerr << bcmS.size() << " " << bcmS[0].streetName << " " << bcmS[1].streetName << std::endl;//2 26th Street 26th Street
    std::cerr << bcmS[0].segment.start.latitudeText << " " << bcmS[0].segment.start.longitudeText << std::endl;//34.0480477 -118.4915421
    std::cerr << bcmS[0].segment.end.latitudeText << " " << bcmS[0].segment.end.longitudeText << std::endl;//34.0476495 -118.4910837
    
    vector<StreetSegment> bcmE(sm.getSegments(GeoCoord("34.0469402", "-118.4902543")));
    std::cerr << bcmE.size() << " " << bcmE[0].streetName << " " << bcmE[1].streetName << std::endl;;//2 26th Street 26th Street
    std::cerr << bcmE[1].segment.start.latitudeText << " " << bcmE[1].segment.start.longitudeText << std::endl;//34.0469402 -118.4902543
    std::cerr << bcmE[1].segment.end.latitudeText << " " << bcmE[1].segment.end.longitudeText << std::endl;//34.0466963 -118.4899691
    
}
*/



