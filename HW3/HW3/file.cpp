//
//  file.cpp
//  HW3
//
//  Created by Quentin Truong on 2/10/17.
//  Copyright © 2017 QT. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

//File Class
class File{
public:
    File(string fileName){
        m_name = fileName;
    }
    virtual ~File(){};
    string name() const{
        return m_name;
    }
    virtual void open() const = 0;
    virtual void redisplay() const = 0;
private:
    string m_name;
};

//TextMsg Class
class TextMsg : public File {
public:
    TextMsg(string fileName) : File(fileName){
    }
    virtual ~TextMsg(){
        cout << "Destroying " << name() << ", a text message" << endl;
    }
    virtual void open() const{
        cout << "open text message";
    }
    virtual void redisplay() const{
        cout << "refresh the screen";
    }
private:
};

//Video Class
class Video : public File{
public:
    Video(string fileName, int runTime) : File(fileName){
        m_runTime = runTime;
    }
    virtual ~Video(){
        cout << "Destroying " << name() << ", a video" << endl;
    }
    virtual void open() const{
        cout << "play " << m_runTime << " second video";
    }
    virtual void redisplay() const{
        cout << "replay video";
    }
private:
    int m_runTime;
};

//Picture Class
class Picture : public File{
public:
    Picture(string fileName) : File(fileName){
    }
    virtual ~Picture(){
        cout << "Destroying the picture " << name() << endl;
    }
    virtual void open() const{
        cout << "show picture";
    }
    virtual void redisplay() const{
        cout << "refresh the screen";
    }
private:
};


void openAndRedisplay(const File* f)
{
    cout << f->name() << ": ";
    f->open();
    cout << endl << "Redisplay: ";
    f->redisplay();
    cout << endl;
}
/*
int main()
{
    File* files[4];
    files[0] = new TextMsg("fromFred.txt");
    // Videos have a name and running time
    files[1] = new Video("goblin.mpg", 3780);
    files[2] = new Picture("kitten.jpg");
    files[3] = new Picture("baby.jpg");
    
    for (int k = 0; k < 4; k++)
        openAndRedisplay(files[k]);
    
    // Clean up the files before exiting
    cout << "Cleaning up." << endl;
    for (int k = 0; k < 4; k++)
        delete files[k];
}*/
/*
 fromFred.txt: open text message
 Redisplay: refresh the screen
 goblin.mpg: play 3780 second video
 Redisplay: replay video
 kitten.jpg: show picture
 Redisplay: refresh the screen
 baby.jpg: show picture
 Redisplay: refresh the screen
 Cleaning up.
 Destroying fromFred.txt, a text message
 Destroying goblin.mpg, a video
 Destroying the picture kitten.jpg
 Destroying the picture baby.jpg
*/


