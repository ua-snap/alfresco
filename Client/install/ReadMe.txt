ReadMe.txt

This document assumes a general comfortability using a terminal to navigate
your computer and execute programs.


=-=-=-=-=-=-=-=-=
 How To Install
=-=-=-=-=-=-=-=-=
Place the 'fresco' folder into your home folder (e.g., /home/username/fresco).


=-=-=-=-=-=-=-=-=
 How To Run
=-=-=-=-=-=-=-=-=
1. With a text editor, open ~/fresco/bin/startclient and change the 
   IP address and PORT variables as needed to target the Fresco Server.


2. To start client(s), run:
  ~/fresco/bin/startclient
  
  If you are running multiple reps and your computer has multiple CPUs
  (or multiple CPU cores) you can start multiple clients by running the 
  script again.  You can also target the Fresco Server from multple 
  computers.


3. To stop client(s), run: 
  ~/fresco/bin/stopclient
  
  Stops all client processes on the given computer.


NOTE: You can add ~/<yourUserName>/fresco/bin to your PATH environment 
variable.  Doing so allows you to run the above from any directory.
