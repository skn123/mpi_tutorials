/* 
MPI_Errhandler_set

   Sets the error handler for a communicator
int MPI_Errhandler_set(
  MPI_Comm comm,
  MPI_Errhandler errhandler
);

Parameters

   comm
          [in] communicator to set the error handler for (handle)

   errhandler
          [in] new MPI error handler for communicator (handle)

Remarks

   Associates the new error handler errorhandler with communicator comm at
   the calling process. Note that an error handler is always associated
   with the communicator.

Thread and Interrupt Safety

   This routine is thread-safe. This means that this routine may be safely
   used by multiple threads without the need for any user-provided thread
   locks. However, the routine is not interrupt safe. Typically, this is
   due to the use of memory allocation routines such as malloc or other
   non-MPICH runtime routines that are themselves not interrupt-safe.

Deprecated Function

   The MPI-2 standard deprecated a number of routines because MPI-2
   provides better versions. This routine is one of those that was
   deprecated. The routine may continue to be used, but new code should
   use the replacement routine. The replacement for this routine is
   MPI_Comm_set_errhandler.

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

   MPI_ERR_COMM
          Invalid communicator. A common error is to use a null
          communicator in a call (not even allowed in MPI_Comm_rank).

   MPI_ERR_ARG
          Invalid argument. Some argument is invalid and is not identified
          by a specific error class (e.g., MPI_ERR_RANK).

See Also

   MPI_Comm_set_errhandler, MPI_Errhandler_create,
   MPI_Comm_create_errhandler

*/
 
// Copyright 2009 Deino Software. All rights reserved.
// Source: http://mpi.deino.net/mpi_functions/index.htm


#include "mpi.h"
#include <stdio.h>
int main(int argc, char *argv[])
{
    int rank, nprocs;
    MPI_Init(&argc,&argv);
    MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
    MPI_Finalize();
    return 0;
}

