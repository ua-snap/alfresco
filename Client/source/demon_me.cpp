//------------------------------------------------------------------------------
//| The Arctic Region Supercomputing Center (ARSC) maintains all copyright to  |
//| this program, and all of its associated files.                             |
//------------------------------------------------------------------------------
//
//  demon_me.c
//   
//  This causes the calling program to morph into a daemon.
//
//  2001Dec07: 1.0.0 Code works, tested for a long time (years)
//  2002Jan22: 1.0.2 daemon_me() now sets umask(077) explicitly, returns unsigned!
//  2002Jan23: 1.0.3 miscellaneous clean ups
//  2007Mar17: 2.0.0 Make it work stand alone for Tim Glaser (sah)
//  2007May21: 2.0.1 Now runs on *nix and windows.  Also provide exceptions. (tsg)

#include "Poco/Exception.h"

#ifndef _WIN32 // defines the unix way
// #include "cgi_lib.h"  the following were part of cgi_lib.h (sah)
#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h> 
#include <stdlib.h> 
#include <signal.h>
#define MAXFD                 64
#define NO_FAIL               000
#define FORK_FAIL             001
#define SETSID_FAIL           002
#define SIGHUP_IGNORE_FAIL    004
#define FORK2_FAIL            010
#define CHDIR_ROOT_FAIL       020
// end of cgi_lib.h insert

unsigned demon_me()
{
  /* this function makes the calling process a daemon */
  int         i;
  unsigned    retval  = NO_FAIL;
  pid_t       pid;
  
  /* Fork to isolate from terminal, step one */
  pid = fork();
 
  if(pid < 0)
  {
    /* Error, fork failed, set return val to indicate failure type */
    retval |= FORK_FAIL;
  }
  else if(pid != 0)
  {
    /* Parent exits leaving orphan child */
    exit(0);
  }
  /* Set this process to be process leader */
  if(setsid() < 0)
  {
    /* Failure to become process leader, note problem */
    retval |= SETSID_FAIL;
  }
  /* On next fork parent will try to take child down as terminal *
   * is finally relenquished                                     */
  if(signal(SIGHUP, SIG_IGN) == SIG_ERR)
  {
    /* This is NOT good */
    retval |= SIGHUP_IGNORE_FAIL;
  }
  else
  {
    /* Fork to isolate from terminal, step two */
    pid = fork();

    if(pid < 0)
    {
      /* Error, fork failed, set return val to indicate failure type */
      retval |= FORK2_FAIL;
    }
    else if(pid != 0)
    {
      /* Parent exits leaving child  */
      exit(0);
    }
  }
  /* Set working directory to root, standard daemon thing to do */
  //if(chdir("/") < 0)
  //{
  //  /* ooh, that's gonna hurt... */
  //  retval |= CHDIR_ROOT_FAIL;
  //}
  /* Set our umask to "safe" value */
  umask(077);
  /* Close all file descripters */
  for(i = 0; i < MAXFD; ++i)
  {
    close(i);
  }
 
  return retval;
}
#endif

void daemonize()
{
    #ifndef _WIN32 // unix way
        unsigned retval = demon_me();
        if (retval != 0) {
            throw Poco::Exception("Error while daemonizing.");
        }
    #else // windows way
        //TODO WIN32
        throw Poco::Exception("Error while daemonizing.  Not yet implemented for Windows.");
    #endif
}
