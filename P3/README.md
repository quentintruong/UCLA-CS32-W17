Winter 2017 CS 32
Programming Assignment 3
Bugs!

Part 1 due 9:00 PM Thursday, February 23
Part 2 due 9:00 PM Thursday, March 2
The Project 3 specification document is complete.

3/2 6:45 pm: To run a sanity check to make sure you didn't overlook something fundamental that would cause you to lose almost all of the correctness points, unzip this zip file (testbugs.zip) on lnxsrv07.seas.ucla.edu or lnxsrv09.seas.ucla.edu. This will create a directory named testbugs. Copy your Actor.h, Actor.cpp, StudentWorld.h, and StudentWorld.cpp into that directory. Change directories into that directory and type the commands

    make
    ./testbugs
This sets up an field surrounded by '*' with a 'g' at x=10,y=5 and a '0' at x=53,y=58 and an ant program consisting only of a couple of moveForward instructions. It initializes a StudentWorld and calls move once, then cleanUp. The sanity check is passed if the grasshopper moves to an adjacent square. (The test does not check the anthill or any ant it might have produced.) If the test is passed, a SUCCESS message is written.
An alternative way to integrate your four files with the tester is to download the zip file (testbugs.zip), add your four files to it, then transfer it to the Linux server as the g++ with Linux writeup tells you.

2/27 1:15 am: Page 42 now lists the ant's (4c/viii) rotateClockwise and (4c/ix) rotateCounterClockwise commands. Page 43 clarifies in (4e) that an command that tries to change the simulation state but is unsuccessful still ends the ant's action for this tick.

2/22 2:30 am: If you've been having a hard time distinguishing the insects on the screen, put the eight .tga files in this zip file in the Assets folder, replacing the old sprites with the same names. If you'd also rather have a light background when you test your program, replace GameController.h and GameController.cpp with the versions in the zip file; you can then change the call at the end of main.cpp to add a final parameter: Game().run(argc, argv, gw, "Bugs", LIGHT);.

2/20 10:30 am: Where the spec used to say that StudentWorld::move should return GWSTATUS_ANT_WON in some circumstances, it now says the constant is GWSTATUS_PLAYER_WON, which is what it's always been called in GameConstants.h.

2/19 10:15 pm: Replace GameController.cpp with this updated version, which calls StudentWorld's cleanup function when the simulation is over (something the original version failed to do). The skeleton zip files have been updated.

2/19 4:45 pm: You can ignore this if you wish, but if you'd like to be able to get better error messages about missing or malformed field files, then replace Field.h with this updated version and use the revised version of how to use Field's loadFile function shown on page 26 of the spec.

2/19 10:30 am: Page 38 of the spec was updated to describe the depth parameter to the GraphObject constructor, and an updated GraphObject.h replaced the GraphObject.h in the skeleton zip files. If you had written a call to the old GraphObject.h before 2/19 10:30 am, replace GraphObject.h and change the call. You will presumably change a call like GraphObject(…, …, …, …, 0.25, depth) in a member initialization list to GraphObject(…, …, …, …, depth).

2/18 12:30 pm: In the Mac sample zip file, the Bugs executable (only) was replaced so that it no longer ignores command line arguments (looking for hardcoded test files of a particular name). Now it uses the files named as command line arguments.

To see what the competition program looks like in action in an sample field with sample ant programs:

Windows:
Open the Windows sample executable zip file and drag the folder named Bugs in it to a folder on your C: drive.
Open a command window and change directory into that folder:
    c:
    cd c:\wherever\you\put\it\Bugs
Run the command
    Bugs field.txt USCAnt.bug USCAnt.bug USCAnt.bug USCAnt.bug
to run the program with four colonies (of the same type of ant, the not-too-bright USCAnt.bug) in the field contained in field.txt.
Mac OS X: You'll need to have XQuartz and freeGLUT installed to run the program.
Install XQuartz:
Click the download link, open the downloaded .dmg file, double-click on XQuartz.pkg, and follow the installation instructions.
Log out and log back in again.
To verify the installation, open a Terminal window and run the command echo $DISPLAY. That should produce one line of output that ends with org.macosforge.xquartz:0.
Install freeGLUT. You have two options:
Option 1: Install the homebrew package manager. Then run the command brew install homebrew/x11/freeglut.
Option 2: Install the MacPorts package manager. Select the appropriate installation links and install the package:
MacPorts-2.4.0-10.12-Sierra.pkg
MacPorts-2.4.0-10.11-ElCapitan.pkg
MacPorts-2.4.0-10.10-Yosemite.pkg
MacPorts-2.4.0-10.9-Mavericks.pkg
MacPorts-2.4.0-10.8-MountainLion.pkg
To verify the installation, open a new Terminal window and run the command port version. That should produce the output Version: 2.4.0. In that new Terminal window, run the command sudo port install freeglut, which will prompt you for your password to authorize installation. A lot of output will be produced. To verify the installation, run the command port installed freeglut. 
When some people run sudo port install freeglut, the response is Error: Port freeglut not found and the installation fails. That's probably because you are behind the UCLA firewall. Bypassing the firewall (perhaps by creating a hotspot with your phone) may do the trick. You might also need to run sudo port selfupdate, but that step is probably not necessary. (Thanks to Albert Pan for this fix.)
Unzip the Mac sample executable zip file. In that zip file is a file named README.txt that tells you how to launch the program.
Linux:
Install OpenGL and freeGLUT (already installed on the SEASnet Linux servers lnxsrv07.seas.ucla.edu and lnxsrv09.seas.ucla.edu):
Ubuntu: sudo apt-get install freeglut3 freeglut3-dev
Fedora: sudo yum install freeglut freeglut-devel
Unzip the sample executable zip. Change into the Bugs directory. In that directory there is a file named README that tells you how to launch the program. You won't hear any sound effects. We haven't tested the Linux version beyond ensuring it builds and tries to connect to an X11 server, but it should work if you set your DISPLAY environment variable correctly, which we haven't had a chance to try.
You can also download the project skeletons with the code we've written, for Windows, Mac, or Linux, and make sure you can build and run them. All they'll do is end the program when you hit Enter, but at least you can verify that the graphics library works. [NOTE for Xcode users: You might get a message at startup relating to the Assets directory. If you want to fix the issue just for this project, select File / Project Settings... and for Derived Data Location, select Project-relative Location. If instead you want to fix it for every project you create with Xcode, select Xcode / Preferences... and for the Derived Data, select Relative.]

The only files you will modify are Actor.h, Actor.cpp, StudentWorld.h, and StudentWorld.cpp. Indeed, those will be the only code files you'll turn in, so in implementing your solution, you must not depend on any changes you make to code files other than those four.

Carey has provided some object-oriented programming tips.

At some point in Project 3, you may find the Stringstreams writeup helpful.