#include "provided.h"
#include "MyMap.h"
#include <string>
#include <cassert>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
    string lowerString(string in) const;
    MyMap<string, GeoCoord> m_data;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    unsigned long numSeg = ml.getNumSegments();
    for (int a = 0; a < numSeg; a++){
        StreetSegment streetSeg;
        ml.getSegment(a, streetSeg);
        
        //m_data.associate(lowerString(streetSeg.streetName), streetSeg.segment.start);//if streetnames are attractions, include this line
        
        for (int b = 0; b < streetSeg.attractions.size(); b++)
            m_data.associate(lowerString(streetSeg.attractions[b].name), streetSeg.attractions[b].geocoordinates);
    }
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    const GeoCoord* temp = m_data.find(lowerString(attraction));
    
    if (temp == nullptr)
        return false;

    gc = *temp;
    return true;
}

string AttractionMapperImpl::lowerString(string in) const{
    string out;
    for (int a = 0; a < in.size(); a++)
        out += tolower(in[a]);
    return out;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}






void example(const MapLoader& ml)
{
    AttractionMapper am;
    am.init(ml); // let our object build its internal data structures
    // by iterating thru all segments from the MapLoader object
    GeoCoord fillMe;
    string attraction = "The Coffee Bean & Tea Leaf";
    bool found = am.getGeoCoord(attraction, fillMe);
    if ( ! found)
    {
        cout << "No geolocation found for " << attraction << endl;
        return;
    }
    cout << "The location of " << attraction << " is " << fillMe.latitudeText << ", " << fillMe.longitudeText << endl;
}

/*
int main(){
    MapLoader ml;
    ml.load("mapdata.txt");
    
    example(ml);
    
    AttractionMapper am;
    am.init(ml);
    GeoCoord gc;
    assert(am.getGeoCoord("Robertson Playground", gc) == true);
    std::cerr << gc.latitudeText << " " << gc.longitudeText << std::endl;
    assert(am.getGeoCoord("fox plaza", gc) == true);
    std::cerr << gc.latitudeText << " " << gc.longitudeText << std::endl;
    assert(am.getGeoCoord("Fox Plaza", gc) == true);
    std::cerr << gc.latitudeText << " " << gc.longitudeText << std::endl;
    assert(am.getGeoCoord("O'Hara's", gc) == true);
    std::cerr << gc.latitudeText << " " << gc.longitudeText << std::endl;
    assert(am.getGeoCoord("Twentieth Century Fox Film Corporatio", gc) == false);
    std::cerr << gc.latitudeText << " " << gc.longitudeText << std::endl;
    
    assert(am.getGeoCoord("Brentwood Country Mart", gc) == true);
    std::cerr << gc.latitudeText << " " << gc.longitudeText << std::endl;
    
    
    //26th Street
    
}
*/








