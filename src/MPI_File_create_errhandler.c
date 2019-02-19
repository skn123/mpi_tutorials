/* 
MPI_File_create_errhandler

   Create a file error handler
int MPI_File_create_errhandler(
  MPI_File_errhandler_fn *function,
  MPI_Errhandler *errhandler
);

Parameters

   function
          [in] user defined error handling procedure (function)

   errhandler
          [out] MPI error handler (handle)

Remarks

   The user routine should be, in C, a function of type
   MPI_File_errhandler_fn, which is defined as

   typedef void MPI_File_errhandler_fn(MPI_File *, int *, ...);

   The first argument is the file in use, the second is the error code to
   be returned.

   In Fortran, the user routine should be of the form:

   SUBROUTINE FILE_ERRHANDLER_FN(FILE, ERROR_CODE, ... )
   INTEGER FILE, ERROR_CODE

   In C++, the user routine should be of the form:

   typedef void MPI::File::Errhandler_fn(MPI::File &, int *, ... );


Thread and Interrupt Safety

   This routine is thread-safe. This means that this routine may be safely
   used by multiple threads without the need for any user-provided thread
   locks. However, the routine is not interrupt safe. Typically, this is
   due to the use of memory allocation routines such as malloc or other
   non-MPICH runtime routines that are themselves not interrupt-safe.

Notes for Fortran

   All MPI routines in Fortran (except for MPI_WTIME and MPI_WTICK) have
   an additional argument ierr at the end of the argument list. ierr is an
   integer and has the same meaning as the return value of the routine in
   C. In Fortran, MPI routines are subroutines, and are invoked with the
   call statement.

   All MPI objects (e.g., MPI_Datatype, MPI_Comm) are of type INTEGER in
   Fortran.

Errors

   All MPI routines (except MPI_Wtime and MPI_Wtick) return an error
   value; C routines as the value of the function and Fortran routines in
   the last argument. Before the value is returned, the current MPI error
   handler is called. By default, this error handler aborts the MPI job.
   The error handler may be changed with MPI_Comm_set_errhandler (for
   communicators), MPI_File_set_errhandler (for files), and
   MPI_Win_set_errhandler (for RMA windows). The MPI-1 routine
   MPI_Errhandler_set may be used but its use is deprecated. The
   predefined error handler MPI_ERRORS_RETURN may be used to cause error
   values to be returned. Note that MPI does not guarentee that an MPI
   program can continue past an error; however, MPI implementations will
   attempt to continue whenever possible.

   MPI_SUCCESS
          No error; MPI routine completed successfully.

*/
 
// Copyright 2009 Deino Software. All rights reserved.
// Source: http://mpi.deino.net/mpi_functions/index.htm

#include "mpi.h"
#include <stdio.h>
#include <string.h>

static int ncalls = 0;

void efn( MPI_File *fh, int *code, ... );
void efn( MPI_File *fh, int *code, ... )
{
    ncalls ++;
    *code = MPI_SUCCESS;
}

int main( int argc, char *argv[] )
{
    MPI_File fh;
    MPI_Errhandler eh;
    char filename[10];
    int errs = 0, toterrs, rank, rc;
    MPI_Init( &argc, &argv );
    /* Test that the default error handler is errors return for files */
    strncpy( filename, "t1", sizeof(filename) );
    rc = MPI_File_open( MPI_COMM_WORLD, filename, MPI_MODE_RDWR, MPI_INFO_NULL, &fh );
    if (!rc) {
        errs++;
        printf( "Did not get error from open for writing without CREATE\n" );
	fflush(stdout);
    }
    /* Test that we can change the default error handler by changing
       the error handler on MPI_FILE_NULL. */
    MPI_File_create_errhandler( efn, &eh );
    MPI_File_set_errhandler( MPI_FILE_NULL, eh );
    rc = MPI_File_open(MPI_COMM_WORLD, filename, MPI_MODE_RDWR, MPI_INFO_NULL, &fh );
    if (rc) {
        printf( "Returned error from open (should have called error handler instead)\n");
	fflush(stdout);
    }
    if (ncalls != 1) {
        errs++;
        printf( "Did not invoke error handler when opening a non-existent"
		"file for writing and reading (without MODE_CREATE)\n");
	fflush(stdout);
    }
    
    MPI_Finalize();
    return errs;
}
