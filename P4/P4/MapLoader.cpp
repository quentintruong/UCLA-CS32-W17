#include "provided.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
    vector<StreetSegment> m_data;
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile)
{
    std::ifstream ifs(mapFile);
    
    if (!ifs)
        return false;
    
    string currString;
    while (getline(ifs, currString)){
        StreetSegment seg;
        seg.streetName = currString;
        getline(ifs, currString);
        
        string ss = currString.substr(0, currString.find(","));
        currString = currString.substr(currString.find(",")+1);
        if (currString[0] == ' ')
            currString = currString.substr(1);
        
        string se = currString.substr(0, currString.find(" "));
        currString = currString.substr(currString.find(" ")+1);
        if (currString[0] == ' ')
            currString = currString.substr(1);
        
        string es = currString.substr(0, currString.find(","));
        currString = currString.substr(currString.find(",")+1);
        if (currString[0] == ' ')
            currString = currString.substr(1);
        
        string ee = currString;
        
        seg.segment = GeoSegment(GeoCoord(ss, se),
                                 GeoCoord(es, ee));
        
        string numAttractions;
        getline(ifs, numAttractions);
        
        
        vector<Attraction> attractions;
        for (int a = 0; a < stoi(numAttractions); a++){
            getline(ifs, currString);
            Attraction currAttraction;
            currAttraction.name = currString.substr(0, currString.find("|"));
            currString = currString.substr(currString.find("|") + 1, string::npos);
            
            string lat = currString.substr(0, currString.find(","));
            string lon = currString.substr(currString.find(",") + 1, string::npos);
            if (lon[0] == ' ')
                lon = lon.substr(1);
            
            currAttraction.geocoordinates = GeoCoord(lat, lon);
            attractions.push_back(currAttraction);
        }
        seg.attractions = attractions;
        
        m_data.push_back(seg);
    }
    
    return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return m_data.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
    if (segNum >= m_data.size())
        return false;
        seg = m_data[segNum];
	return true;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}

/*
int main(){
    MapLoader a;
    a.load("mapdata.txt");
    std::cerr << a.getNumSegments() << std::endl;
    StreetSegment fuck;
    std::cerr << a.getSegment(255,fuck) << std::endl;
    std::cerr << fuck.streetName << std::endl << fuck.segment.start.latitudeText << " " << fuck.segment.start.longitudeText << std::endl << fuck.segment.end.latitudeText << " " << fuck.segment.end.longitudeText << std::endl;
    std::cerr << fuck.attractions[0].name << std::endl << fuck.attractions[0].geocoordinates.latitudeText << " " << fuck.attractions[0].geocoordinates.longitudeText << std::endl;
    
    std::cerr << a.getSegment(19641,fuck) << std::endl;
}
*/













