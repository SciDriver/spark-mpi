#include "mpichconf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "process.h"
#include "cmnargs.h"

/* method extracted from src/pm/remshell/mpiexec.c
   required by cmnargs.c and env.c
*/

void mpiexec_usage( const char *msg )
{
    if (msg) {
	printf( "%s", msg );
	if (msg[strlen(msg)-1] != '\n') {
	    printf( "\n" );
	}
    }
    printf( "Usage: mpiexec %s\n", MPIE_ArgDescription() );
    exit( -1 );
}
