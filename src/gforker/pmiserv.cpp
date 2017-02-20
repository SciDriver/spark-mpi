
// Implemented from the gforker/mpiexec.cpp

#include "mpichconf.h"
#include <stdio.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <stdlib.h>
#include <csignal>

// src/pm/util
extern "C" { 
#include "pmutil.h"
#include "process.h"
#include "cmnargs.h"
#include "pmiserv.h"
#include "ioloop.h"
#include "labelout.h"
#include "env.h"
#include "simple_pmiutil.h"
}

typedef struct { PMISetup pmiinfo; IOLabelSetup labelinfo; } SetupInfo;

/* Forward declarations */
int mypreamble( void *, ProcessState* );
int mypostamble( void *, void *, ProcessState* );

// Set usePort to 1 if a host:port should be used 
// insted of inheriting an FD to a socketpair. 
static int usePort = 1;

#ifndef MAX_PORT_STRING
#define  MAX_PORT_STRING 1024
#endif

// The pmiserv replacement of MPIE_ForkProcess

int myForkProcesses( ProcessWorld *pWorld, char *envp[],
		     int (*preamble)(void*,ProcessState*), 
		     void *preambleData,
		     int (*postamble)(void*,void*,ProcessState*), 
		     void *postambleData
                    );

// A unique ID for each forked process, up to 2 billion.
// This is global to this file so that MPIE_SetupSingleton
// and MPIE_ForkProcess can both access it
static       int UniqId = 0; 


/* Note that envp is common but not standard */
int main( int argc, char *argv[], char *envp[] )
{
    int          rc;
    int          erc = 0;  /* Other (exceptional) return codes */
    int          reason, signaled = 0;
    SetupInfo    s;
    char         portString[MAX_PORT_STRING];

    /* MPIE_ProcessInit initializes the global pUniv */
    MPIE_ProcessInit();
    
    /* Set a default for the universe size */
    pUniv.size = 64;
    
    /* Tell the PMI untility routines that we are the server */
    PMIU_SetServer();

    /* Set defaults for any arguments that are options.  Also check the
       environment for special options, such as debugging.  Set 
       some defaults in pUniv */
    MPIE_CheckEnv( &pUniv, 0, 0 );
    IOLabelCheckEnv( );
    
    /* Handle the command line arguments.  Use the routine from util/cmnargs.c
       to fill in the universe */
    MPIE_Args( argc, argv, &pUniv, 0, 0 );

    /* Get the common port for creating PMI connections to the created
    processes */
    rc = PMIServSetupPort( &pUniv, portString, sizeof(portString) );
    if (rc) {
	printf( "Unable to setup port for listener\n" );
	exit(1);
    }

    s.pmiinfo.portName = portString;
    printf("PMI port: %s\n", portString);

    // PMIServInit(myspawn, &s);
    s.pmiinfo.pWorld = &pUniv.worlds[0];
    PMISetupNewGroup( pUniv.worlds[0].nProcess, 0 );
    
    /* PMISetupNewGroup will create a kvs space */
    MPIE_ForwardCommonSignals();
    MPIE_IgnoreSigPipe();
    
    myForkProcesses( &pUniv.worlds[0], envp,
		     mypreamble, &s,
		     mypostamble, 0 );
 
    reason = MPIE_IOLoop( pUniv.timeout );

    if (reason == IOLOOP_TIMEOUT) {
      // Exited due to timeout.  Generate an error message and
      // terminate the children */
	if (pUniv.timeout > 60) {
	    MPL_error_printf( "Timeout of %d minutes expired; job aborted\n",
			       pUniv.timeout / 60 );
	}
	else {
	    MPL_error_printf( "Timeout of %d seconds expired; job aborted\n",
			       pUniv.timeout );
	}
	erc = 1;
	MPIE_KillUniverse( &pUniv );
    }
    
    return( rc );
}

/* ------------------------------------------------------------------------- */
/* Redirect stdout and stderr to a handler */
// manages the creation of the PMI listener socket for 
//	   a connection back from the created process
int mypreamble( void *data, ProcessState *pState )
{
    SetupInfo *s = (SetupInfo *)data;

    IOLabelSetupFDs( &s->labelinfo );
    /* In the inherit-FD case, this routine creates the FD that will
       be used for PMI commands between the mpiexec process and the
       fork/exec'ed process */
    PMISetupSockets( usePort, &s->pmiinfo );

    /* Tell MPIE_ForkProcesses not to include the PMI environment if
       we are using a port, and use the PMI_PORT and ID instead */
    if (usePort) pState->initWithEnv = 0;
    
    /* Add clique information */
    {
	int i, size = pState->app->nProcess;
	char digits[10], ranks[1024];
	char key[256];
	
	/* Create the string of ranks.  These are ranks in comm_world */
	ranks[0] = 0;
	for (i=0; i<size; i++) {
	    MPL_snprintf( digits, sizeof(digits), "%d,", i );
	    MPIU_Strnapp( ranks, digits, sizeof(ranks) );
	}
	/* Remove the trailing comma */
	if (size > 0) 
	    ranks[strlen(ranks)-1] = 0;
	/* Add this to the predefined keys */
	MPL_snprintf( key, sizeof(key), "pmiPrivateLocalRanks_%d", 
		       pState->wRank );
	
	pmix_preput( key, ranks );
    }

    return 0;
}


/* Close one side of the pipe pair and register a handler for the I/O */
int mypostamble( void *predata, void *data, ProcessState *pState )
{
    SetupInfo *s = (SetupInfo *)predata;

    // IOLabelSetupFinishInServer( &s->labelinfo, pState );
    PMISetupFinishInServer( usePort, &s->pmiinfo, pState );

    return 0;
}

int myForkProcesses( ProcessWorld *pWorld, char *envp[],
			int (*preamble)(void*,ProcessState*), 
			void *preambleData,
			int (*postamble)(void*,void*,ProcessState*), 
			void *postambleData
			)
{
    pid_t         pid;
    ProcessApp   *app;
    ProcessState *pState=0;
    int          wRank = 0;    /* Rank in this comm world of the process */
    int          i, rc;
    int          nProcess = 0;

    app = pWorld->apps;

    while (app) {

	/* Allocate process state if necessary */
	if (!app->pState) {
	    pState = (ProcessState *)MPIU_Malloc( 
		app->nProcess * sizeof(ProcessState) );
	    if (!pState) {
		return -1;
	    }
	    app->pState = pState;
	}
	pState = app->pState;

	for (i=0; i < app->nProcess; i++) {

	    pState[i].app                   = app;
	    pState[i].wRank                 = wRank++;
	    pState[i].id                    = UniqId++;
	    pState[i].initWithEnv           = 1;  /* Default is to use env
						    to initialize connection */
	    pState[i].status                = PROCESS_UNINITIALIZED;
	    pState[i].exitStatus.exitReason = EXIT_NOTYET;
	    pState[i].pid                   = -1;

	    /* Execute any preamble */
	    if (preamble) {
		rc = (*preamble)( preambleData, &pState[i] );
	    }


	    nProcess++;
                
	    /* Add this to the live processes in the Universe */
	    pUniv.nLive++;

	    // pState[i].pid    = pid;
	    pState[i].status = PROCESS_ALIVE;

	    if (postamble) {
	      rc = (*postamble)( preambleData, postambleData, 
				 &pState[i] );
	      if (rc) {
		printf("mpiexec postamble failed\n" );
		// FIXME: kill children
		exit(1);
	      }	    
	    }
	    
	}
	
	app = app->nextApp;
    }
	
    return nProcess;
}
