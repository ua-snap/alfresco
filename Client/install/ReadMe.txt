ReadMe.txt

=-=-=-=-=-=-=-=-=
 How To Install
=-=-=-=-=-=-=-=-=
Run the install script while at a command line terminal.
Run the following where "frescoInstallDir" is the directory 
path where this file is located.

  $ frescoInstallDir/install

The script places neccessary files in /home/<yourUserName>/fresco/
If this is an update, it is normal to see messages stating that files 
already exist.


=-=-=-=-=-=-=-=-=
 How To Run
=-=-=-=-=-=-=-=-=
1. Open /home/<yourUserName>/fresco/bin/startclient and change the 
   IP address to the public IP address where the Fresco Server will run.

2. Start client
  $ /home/<yourUserName>/fresco/bin/startclient

3. Stop client
  $ /home/<yourUserName>/fresco/bin/stopclient

NOTE: You can add /<yourUserName>/fresco/bin to your PATH environment 
variable.  Doing so allows you to run the above from any directory.
