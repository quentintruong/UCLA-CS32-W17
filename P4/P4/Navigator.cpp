#include "provided.h"
#include "MyMap.h"
#include <string>
#include <vector>
#include <queue>
#include <cassert>
using namespace std;

struct gcNode{
    gcNode(GeoCoord gc, double g, double f) : m_gc(gc), m_g(g), m_f(f){}
    GeoCoord m_gc;
    double m_g;
    double m_f;
};

struct gcPath{
    gcPath(GeoCoord gcCurr, GeoCoord gcPrev, StreetSegment ss) : m_gcCurr(gcCurr), m_gcPrev(gcPrev), m_ss(ss){}
    GeoCoord m_gcCurr;//Curr came from prev
    GeoCoord m_gcPrev;
    StreetSegment m_ss;//the segment Curr is from
};

string dirToString(double dir){
    if (dir <= 22.5)
        return "east";
    else if (dir <= 67.5)
        return "northeast";
    else if (dir <= 112.5)
        return "north";
    else if (dir <= 157.5)
        return "northwest";
    else if (dir <= 202.5)
        return "west";
    else if (dir <= 247.5)
        return "southwest";
    else if (dir <= 292.5)
        return "south";
    else if (dir <= 337.5)
        return "southeast";
    return "east";
}

string rlToString(double dir){
    if (dir < 180)
        return "left";
    return "right";
}

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
    string latlong(GeoCoord curr) const;
    int find(GeoCoord gc, vector<gcNode> vec) const;
    int findMinPos(vector<gcNode> vec) const;
    
    AttractionMapper am;
    SegmentMapper sm;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    MapLoader ml;
    bool valid = ml.load(mapFile);
    
    am.init(ml);
    sm.init(ml);
    return valid;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    GeoCoord startGC, endGC;
    bool s = am.getGeoCoord(start, startGC);
    bool e = am.getGeoCoord(end, endGC);
    
    vector<StreetSegment> sV(sm.getSegments(startGC));
    vector<StreetSegment> eV(sm.getSegments(endGC));
    for (int a = 0; a < sV.size(); a++){
        for (int b = 0; b < eV.size(); b++){
            if (sV[a].segment.start.latitudeText == eV[b].segment.start.latitudeText &&
                sV[a].segment.start.longitudeText == eV[b].segment.start.longitudeText &&
                sV[a].segment.end.latitudeText == eV[b].segment.end.latitudeText &&
                sV[a].segment.end.longitudeText == eV[b].segment.end.longitudeText){
                vector<NavSegment> myDir;
                myDir.push_back(NavSegment(dirToString(angleOfLine(GeoSegment(startGC, endGC))),
                                           sV[a].streetName,
                                           distanceEarthMiles(startGC, endGC),
                                           GeoSegment(startGC, endGC)));
                /*for (int a = 0; a < myDir.size(); a++){
                    if (myDir[a].m_command == 0){
                        std::cerr << "m_command: " << myDir[a].m_command << std::endl << "m_geoSegment.start: " << myDir[a].m_geoSegment.start.latitudeText << " " << myDir[a].m_geoSegment.start.longitudeText << std::endl << "m_geoSegment.end: " << myDir[a].m_geoSegment.end.latitudeText << " " << myDir[a].m_geoSegment.end.longitudeText << std::endl << "m_direction: " << myDir[a].m_direction << std::endl << "m_distance: " << myDir[a].m_distance << std::endl << "m_streetName: " << myDir[a].m_streetName << std::endl << std::endl;
                    }
                    else{
                        std::cerr << "m_command: " << myDir[a].m_command << std::endl << "m_direction: " << myDir[a].m_direction << std::endl << "m_streetName: " << myDir[a].m_streetName << std::endl << std::endl;
                    }
                }*/
                directions = myDir;
                return NAV_SUCCESS;
            }
            
        }
    }
    
    if (!s)
        return NAV_BAD_SOURCE;
    if (!e)
        return NAV_BAD_DESTINATION;
    
    MyMap<string, bool> closedMap;
    vector<gcNode> openVector;
    MyMap<string, gcPath> cameFrom;
    
    openVector.push_back(gcNode(startGC, 0, 0+distanceEarthMiles(startGC, endGC)));
    
    while (!openVector.empty()){
        int minPos = findMinPos(openVector);
        gcNode curr = openVector[minPos];
        openVector.erase(openVector.begin() + minPos);
        closedMap.associate(latlong(curr.m_gc), true);
        
        if (latlong(curr.m_gc) == latlong(endGC)){
            vector<GeoCoord> myGC;
            vector<StreetSegment> mySS;
            vector<NavSegment> myDir;
            
            am.getGeoCoord(end, endGC);
            myGC.push_back(endGC);
            mySS.push_back(sm.getSegments(endGC)[0]);
            gcPath* gcP = cameFrom.find(latlong(curr.m_gc));
            
            if (gcP != nullptr)
                myGC.push_back(gcP->m_gcCurr);
            
            while (gcP != nullptr){
                
                myGC.push_back(gcP->m_gcCurr);
                myGC.push_back(gcP->m_gcPrev);
                mySS.push_back(gcP->m_ss);
                
                gcP = cameFrom.find(latlong(gcP->m_gcPrev));
            }
            
            for (int a = myGC.size()-1; a >= 2; a--){
                
                if (a == myGC.size()-1 || mySS[a/2].streetName == mySS[(a+2)/2].streetName){//PROCEED
                    myDir.push_back(NavSegment(dirToString(angleOfLine(GeoSegment(myGC[a], myGC[a-1]))),
                                               mySS[a/2].streetName,
                                               distanceEarthMiles(myGC[a], myGC[a-1]),
                                               GeoSegment(myGC[a], myGC[a-1])));
                }
                else{//TURN
                    myDir.push_back(NavSegment(rlToString(angleBetween2Lines(mySS[a/2].segment, mySS[(a-2)/2].segment)), mySS[(a-2)/2].streetName));
                    
                    myDir.push_back(NavSegment(dirToString(angleOfLine(GeoSegment(myGC[a], myGC[a-1]))),
                                               mySS[a/2].streetName,
                                               distanceEarthMiles(myGC[a], myGC[a-1]),
                                               GeoSegment(myGC[a], myGC[a-1])));
                }
                //std::cerr << myGC[a].latitudeText << " " << myGC[a].longitudeText << " " << mySS[a/2].streetName << std::endl;
                //std::cerr << myGC[a-1].latitudeText << " " << myGC[a-1].longitudeText << " " << mySS[(a-2)/2].streetName << std::endl;
                a--;
            }
            /*
            for (int a = 0; a < myDir.size(); a++){
                if (myDir[a].m_command == 0){
                    std::cerr << "m_command: " << myDir[a].m_command << std::endl << "m_geoSegment.start: " << myDir[a].m_geoSegment.start.latitudeText << " " << myDir[a].m_geoSegment.start.longitudeText << std::endl << "m_geoSegment.end: " << myDir[a].m_geoSegment.end.latitudeText << " " << myDir[a].m_geoSegment.end.longitudeText << std::endl << "m_direction: " << myDir[a].m_direction << std::endl << "m_distance: " << myDir[a].m_distance << std::endl << "m_streetName: " << myDir[a].m_streetName << std::endl << std::endl;
                }
                else{
                    std::cerr << "m_command: " << myDir[a].m_command << std::endl << "m_direction: " << myDir[a].m_direction << std::endl << "m_streetName: " << myDir[a].m_streetName << std::endl << std::endl;
                }
            }*/
            directions = myDir;
            return NAV_SUCCESS;
        }
        
        vector<StreetSegment> neighborsSS(sm.getSegments(curr.m_gc));
        vector<GeoCoord> neighborsGC;
        for (int numNeigh = 0; numNeigh < neighborsSS.size(); numNeigh++){
            neighborsGC.push_back(neighborsSS[numNeigh].segment.start);
            neighborsGC.push_back(neighborsSS[numNeigh].segment.end);
            
            //if on the ending street, add end gc
            vector<StreetSegment> currVec(sm.getSegments(curr.m_gc));
            vector<StreetSegment> endVec(sm.getSegments(endGC));
            for (int a = 0; a < currVec.size(); a++)
                for (int b = 0; b < endVec.size(); b++)
                    if (currVec[a].segment.start.latitudeText == endVec[b].segment.start.latitudeText &&
                        currVec[a].segment.start.longitudeText == endVec[b].segment.start.longitudeText &&
                        currVec[a].segment.end.latitudeText == endVec[b].segment.end.latitudeText &&
                        currVec[a].segment.end.longitudeText == endVec[b].segment.end.longitudeText)
                        neighborsGC.push_back(endGC);
        }
        
        for (int numNeigh = 0; numNeigh < neighborsGC.size(); numNeigh++){
            if (closedMap.find(latlong(neighborsGC[numNeigh])) != nullptr){
                continue;
            }
            
            double tentative_gScore = curr.m_g + distanceEarthMiles(curr.m_gc, neighborsGC[numNeigh]);
            
            if (find(neighborsGC[numNeigh], openVector) == -1){
                
                gcNode neighborNode(neighborsGC[numNeigh],
                                    tentative_gScore,
                                    tentative_gScore + distanceEarthMiles(neighborsGC[numNeigh], endGC));
                openVector.push_back(neighborNode);
            }
            else if (tentative_gScore >= openVector[find(neighborsGC[numNeigh], openVector)].m_g){
                continue;
            }
            cameFrom.associate(latlong(neighborsGC[numNeigh]),
                               gcPath(neighborsGC[numNeigh], curr.m_gc, neighborsSS[(int) numNeigh / 2]));
            openVector[find(neighborsGC[numNeigh], openVector)].m_g = tentative_gScore;
            openVector[find(neighborsGC[numNeigh], openVector)].m_f = tentative_gScore + distanceEarthMiles(neighborsGC[numNeigh], endGC);
        }
    }
    return NAV_NO_ROUTE;
}

string NavigatorImpl::latlong(GeoCoord curr) const{
    return curr.latitudeText + curr.longitudeText;
}

int NavigatorImpl::find(GeoCoord gc, vector<gcNode> vec) const{
    for (int a = 0; a < vec.size(); a++){
        if (latlong(vec[a].m_gc) == latlong(gc))
            return a;
    }
    return -1;
}

int NavigatorImpl::findMinPos(vector<gcNode> vec) const{
    if (vec.empty())
        return -1;
    int pos = 0;
    double min = vec[pos].m_f;
    for (int a = 1; a < vec.size(); a++){
        if (min > vec[a].m_f){
            pos = a;
            min = vec[a].m_f;
        }
    }
    return pos;
}



                
//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
// This is the BruinNav main routine.  If the executable built from this file
// and the other .cpp files you write is named BruinNav (or BruinNav.exe on
// Windows), then you can run it with
//  ./BruinNav theMapDataFileName "Start Attraction" "End Attraction"
// to get the turn-by-turn instructions a user wants to see, or
//  ./BruinNav theMapDataFileName "Start Attraction" "End Attraction" -raw
// to see the sequence of NavSegments produced by Navigator::navigate()
// (Under Windows, say "BruinNav" instead of "./BruinNav")
// For example, with the mapdata.txt file we supplied you,
//  ./BruinNav mapdata.txt "Harvard-Westlake Middle School" "GreyStone Mansion"
// should produce something like
//   Routing...
//   You are starting at: Harvard-Westlake Middle School
//   Proceed 0.47 miles southeast on Brooklawn Drive
//   Turn right onto Angelo Drive
//   Proceed 0.43 miles east on Angelo Drive
//   Turn right onto Benedict Canyon Drive
//   Proceed 0.13 miles southeast on Benedict Canyon Drive
//   Turn left onto Hartford Way
//   Proceed 0.13 miles east on Hartford Way
//   Turn left onto Lexington Road
//   Proceed 0.63 miles east on Lexington Road
//   Turn right onto Alpine Drive
//   Proceed 0.07 miles southeast on Alpine Drive
//   Turn left onto West Sunset Boulevard
//   Proceed 0.20 miles northeast on West Sunset Boulevard
//   Turn left onto Mountain Drive
//   Proceed 0.15 miles northeast on Mountain Drive
//   Turn left onto Schuyler Road
//   Proceed 0.19 miles north on Schuyler Road
//   Turn right onto Stonewood Drive
//   Proceed 0.15 miles northeast on Stonewood Drive
//   You have reached your destination: GreyStone Mansion
//   Total travel distance: 2.5 miles
// and
//  ./BruinNav mapdata.txt "Harvard-Westlake Middle School" "GreyStone Mansion" -raw
// might produce 133 lines starting
//   Start: Harvard-Westlake Middle School
//   End:   GreyStone Mansion
//   34.0904161,-118.4344198 34.0905309,-118.4343340 northeast 0.0093 Brooklawn Drive
//   34.0905309,-118.4343340 34.0904815,-118.4341398 east 0.0116 Brooklawn Drive
//   34.0904815,-118.4341398 34.0903824,-118.4339467 southeast 0.0130 Brooklawn Drive
//   34.0903824,-118.4339467 34.0902310,-118.4337702 southeast 0.0145 Brooklawn Drive
//   34.0902310,-118.4337702 34.0900681,-118.4335630 southeast 0.0163 Brooklawn Drive
//   34.0900681,-118.4335630 34.0899633,-118.4334635 southeast 0.0092 Brooklawn Drive
//   34.0899633,-118.4334635 34.0897917,-118.4333739 southeast 0.0129 Brooklawn Drive
//   34.0897917,-118.4333739 34.0894654,-118.4333087 south 0.0229 Brooklawn Drive
// and ending
//   34.0904163,-118.4036377 34.0905573,-118.4036590 north 0.0098 Schuyler Road
//   34.0905573,-118.4036590 34.0908428,-118.4038080 northwest 0.0215 Schuyler Road
//   turn right Stonewood Drive
//   34.0908428,-118.4038080 34.0908832,-118.4036471 east 0.0096 Stonewood Drive
//   34.0908832,-118.4036471 34.0908732,-118.4034846 east 0.0093 Stonewood Drive
//   34.0908732,-118.4034846 34.0908807,-118.4033732 east 0.0064 Stonewood Drive
//   34.0908807,-118.4033732 34.0909505,-118.4031144 east 0.0156 Stonewood Drive
//   34.0909505,-118.4031144 34.0909630,-118.4030512 east 0.0037 Stonewood Drive
//   34.0909630,-118.4030512 34.0909256,-118.4029338 east 0.0072 Stonewood Drive
//   34.0909256,-118.4029338 34.0919749,-118.4018226 northeast 0.0964 Stonewood Drive
//
// If you build the program as is, you'll notice the turn-by-turn instructions
// say IN_SOME_DIRECTION instead of east or southwest or some actual direction.
// That's because of the template appearing a few lines below; read the comment
// before it.
/*
#include "provided.h"
//#include "support.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
using namespace std;

// START OF WHAT YOU CAN REMOVE ONCE YOU'VE IMPLEMENTED string directionOfLine(const GeoSegment& gs)
// If you want the turn-by-turn directions to give a real direction like
// east or southwest instead of IN_SOME_DIRECTION, you'll need to
// implement the ordinary function
//    string directionOfLine(const GeoSegment& gs)
// to return a string like "east" or "southwest" based on the angle of the
// GeoSegment gs according to the table at the bottom of page 20 of the spec.
// When you do that, you can delete this comment and the template function
// below that appears here solely to allow this main.cpp to build.
// Why didn't we just write the real function for you?  Because it's also
// a function you'd find useful in producing the NavSegments in the navigate()
// method.  Since it's useful in more than one .cpp file, its declaration
// should go in support.h and its implementation in support.cpp.


template<typename T>
string directionOfLine(const T&)
{
    return "IN_SOME_DIRECTION";
}
// END OF WHAT YOU CAN REMOVE ONCE YOU'VE IMPLEMENTED string directionOfLine(const GeoSegment& gs)

void printDirectionsRaw(string start, string end, vector<NavSegment>& navSegments);
void printDirections(string start, string end, vector<NavSegment>& navSegments);

int main(int argc, char *argv[])
{
    bool raw = false;
    if (argc == 5  &&  strcmp(argv[4], "-raw") == 0)
    {
        raw = true;
        argc--;
    }
    if (argc != 4)
    {
        cout << "Usage: BruinNav mapdata.txt \"start attraction\" \"end attraction name\"" << endl
        << "or" << endl
        << "Usage: BruinNav mapdata.txt \"start attraction\" \"end attraction name\" -raw" << endl;
        return 1;
    }
    
    Navigator nav;
    
    if ( ! nav.loadMapData(argv[1]))
    {
        cout << "Map data file was not found or has bad format: " << argv[1] << endl;
        return 1;
    }
    
    if ( ! raw)
        cout << "Routing..." << flush;
    
    string start = argv[2];
    string end = argv[3];
    vector<NavSegment> navSegments;
    
    NavResult result = nav.navigate(start, end, navSegments);
    if ( ! raw)
        cout << endl;
    
    switch (result)
    {
        case NAV_NO_ROUTE:
            cout << "No route found between " << start << " and " << end << endl;
            break;
        case NAV_BAD_SOURCE:
            cout << "Start attraction not found: " << start << endl;
            break;
        case NAV_BAD_DESTINATION:
            cout << "End attraction not found: " << end << endl;
            break;
        case NAV_SUCCESS:
            if (raw)
                printDirectionsRaw(start, end, navSegments);
            else
                printDirections(start, end, navSegments);
            break;
    }
}

void printDirectionsRaw(string start, string end, vector<NavSegment>& navSegments)
{
    cout << "Start: " << start << endl;
    cout << "End:   " << end << endl;
    cout.setf(ios::fixed);
    cout.precision(4);
    for (auto ns : navSegments)
    {
        switch (ns.m_command)
        {
            case NavSegment::PROCEED:
                cout << ns.m_geoSegment.start.latitudeText << ","
                << ns.m_geoSegment.start.longitudeText << " "
                << ns.m_geoSegment.end.latitudeText << ","
                << ns.m_geoSegment.end.longitudeText << " "
                << ns.m_direction << " "
                << ns.m_distance << " "
                << ns.m_streetName << endl;
                break;
            case NavSegment::TURN:
                cout << "turn " << ns.m_direction << " " << ns.m_streetName << endl;
                break;
        }
    }
}

void printDirections(string start, string end, vector<NavSegment>& navSegments)
{
    cout.setf(ios::fixed);
    cout.precision(2);
    
    cout << "You are starting at: " << start << endl;
    
    double totalDistance = 0;
    string thisStreet;
    GeoSegment effectiveSegment;
    double distSinceLastTurn = 0;
    
    for (auto ns : navSegments)
    {
        switch (ns.m_command)
        {
            case NavSegment::PROCEED:
                if (thisStreet.empty())
                {
                    thisStreet = ns.m_streetName;
                    effectiveSegment.start = ns.m_geoSegment.start;
                }
                effectiveSegment.end = ns.m_geoSegment.end;
                distSinceLastTurn += ns.m_distance;
                totalDistance += ns.m_distance;
                break;
            case NavSegment::TURN:
                if (distSinceLastTurn > 0)
                {
                    cout << "Proceed " << distSinceLastTurn << " miles "
                    << directionOfLine(effectiveSegment) << " on " << thisStreet << endl;
                    thisStreet.clear();
                    distSinceLastTurn = 0;
                }
                cout << "Turn " << ns.m_direction << " onto " << ns.m_streetName << endl;
                break;
        }
    }
    
    if (distSinceLastTurn > 0)
        cout << "Proceed " << distSinceLastTurn << " miles "
        << directionOfLine(effectiveSegment) << " on " << thisStreet << endl;
    cout << "You have reached your destination: " << end << endl;
    cout.precision(1);
    cout << "Total travel distance: " << totalDistance << " miles" << endl;
}
 */
/*
int main(){
    Navigator nav;
    assert(nav.loadMapData("mapdata.txt") == true);
    
    vector<NavSegment> dir;
    //nav.navigate("Brentwood Country Mart", "26th Street", dir);//26th Street
    //34.0474215 -118.4901897
    //34.0441570 -118.4869998
    //"Veterans Hospital", "Transit Operations and Rental Services"
    //"Fox Theater", "Transit Operations and Rental Services",
    nav.navigate("Fox Theater", "Transit Operations and Rental Services", dir);
    //nav.navigate("1061 Broxton Avenue", "Headlines!", dir);
    //nav.navigate("1031 Broxton Avenue", "1061 Broxton Avenue", dir);
    
}*/

/*
#include "provided.h"
#include "MyMap.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <cassert>
using namespace std;

int main()
{
    cout << "About to test MyMap" << endl;
    {
        MyMap<int, string> mm;
        mm.associate(20, "Ethel");
        mm.associate(10, "Fred");
        const string* p = mm.find(10);
        assert(p != nullptr  &&  *p == "Fred");
        assert(mm.find(30) == nullptr);
        assert(mm.size() == 2);
    }
    cout << "MyMap PASSED" << endl;
    
    cout << "About to test MapLoader" << endl;
    {
        MapLoader ml;
        assert(ml.load("testmap.txt"));
        size_t numSegments = ml.getNumSegments();
        assert(numSegments == 7);
        bool foundAttraction = false;
        for (size_t i = 0; i < numSegments; i++)
        {
            StreetSegment seg;
            assert(ml.getSegment(i, seg));
            if (seg.streetName == "Picadilly")
            {
                assert(seg.attractions.size() == 1);
                assert(seg.attractions[0].name == "Eros Statue");
                foundAttraction = true;
                break;
            }
        }
        assert(foundAttraction);
    }
    cout << "MapLoader PASSED" << endl;
    
    cout << "About to test AttractionMapper" << endl;
    {
        MapLoader ml;
        assert(ml.load("testmap.txt"));
        AttractionMapper am;
        am.init(ml);
        GeoCoord gc;
        assert(am.getGeoCoord("Hamleys Toy Store", gc));
        assert(gc.latitudeText == "51.512812");
        assert(gc.longitudeText == "-0.140114");
    }
    cout << "AttractionMapper PASSED" << endl;
    
    cout << "About to test SegmentMapper" << endl;
    {
        MapLoader ml;
        assert(ml.load("testmap.txt"));
        SegmentMapper sm;
        sm.init(ml);
        GeoCoord gc("51.510087", "-0.134563");
        vector<StreetSegment> vss = sm.getSegments(gc);
        assert(vss.size() == 4);
        string names[4];
        for (size_t i = 0; i < 4; i++)
            names[i] = vss[i].streetName;
        sort(names, names+4);
        const string expected[4] = {
            "Coventry Street", "Picadilly", "Regent Street", "Shaftesbury Avenue"
        };
        assert(equal(names, names+4, expected));
    }
    cout << "SegmentMapper PASSED" << endl;
    
    cout << "About to test Navigator" << endl;
    {
        Navigator nav;
        assert(nav.loadMapData("testmap.txt"));
        vector<NavSegment> directions;
        assert(nav.navigate("Eros Statue", "Hamleys Toy Store", directions) == NAV_SUCCESS);
        assert(directions.size() == 6);
        struct ExpectedItem
        {
            NavSegment::NavCommand command;
            string direction;
            double distance;
            string streetName;
        };
        const ExpectedItem expected[6] = {
            { NavSegment::PROCEED, "northwest", 0.0138, "Picadilly" },
            { NavSegment::TURN, "left", 0, "" },
            { NavSegment::PROCEED, "west", 0.0119, "Regent Street" },
            { NavSegment::PROCEED, "west", 0.0845, "Regent Street" },
            { NavSegment::PROCEED, "west", 0.0696, "Regent Street" },
            { NavSegment::PROCEED, "northwest", 0.1871, "Regent Street" },
        };
        for (size_t i = 0; i < 6; i++)
        {
            const NavSegment& ns = directions[i];
            const ExpectedItem& exp = expected[i];
            assert(ns.m_command == exp.command);
            assert(ns.m_direction == exp.direction);
            if (ns.m_command == NavSegment::PROCEED)
            {
                assert(abs(ns.m_distance - exp.distance) < 0.00051);
                assert(ns.m_streetName == exp.streetName);
            }
        }
    }
    cout << "Navigator PASSED" << endl;
}
*/




/*
 
 51.509894 -0.134482 Picadilly
 51.510087 -0.134563 Regent Street
 51.510087 -0.134563 Regent Street
 51.510127 -0.134831 Regent Street
 51.510127 -0.134831 Regent Street
 51.509919 -0.136767 Regent Street
 51.509919 -0.136767 Regent Street
 51.510377 -0.138209 Regent Street
 51.510377 -0.138209 Regent Street
 51.513719 -0.141174 Regent Street
 51.513719 -0.141174 Regent Street
 51.512812 -0.140114 Regent Street
 
 
 */


/*
 Routing...
 //   You are starting at: Harvard-Westlake Middle School
 //   Proceed 0.47 miles southeast on Brooklawn Drive
 //   Turn right onto Angelo Drive
 //   Proceed 0.43 miles east on Angelo Drive
 //   Turn right onto Benedict Canyon Drive
 //   Proceed 0.13 miles southeast on Benedict Canyon Drive
 //   Turn left onto Hartford Way
 //   Proceed 0.13 miles east on Hartford Way
 //   Turn left onto Lexington Road
 //   Proceed 0.63 miles east on Lexington Road
 //   Turn right onto Alpine Drive
 //   Proceed 0.07 miles southeast on Alpine Drive
 //   Turn left onto West Sunset Boulevard
 //   Proceed 0.20 miles northeast on West Sunset Boulevard
 //   Turn left onto Mountain Drive
 //   Proceed 0.15 miles northeast on Mountain Drive
 //   Turn left onto Schuyler Road
 //   Proceed 0.19 miles north on Schuyler Road
 //   Turn right onto Stonewood Drive
 //   Proceed 0.15 miles northeast on Stonewood Drive
 //   You have reached your destination: GreyStone Mansion
 //   Total travel distance: 2.5 miles
 */

