/* 
MPI_Bsend_init

   Builds a handle for a buffered send
int MPI_Bsend_init(
  void *buf,
  int count,
  MPI_Datatype datatype,
  int dest,
  int tag,
  MPI_Comm comm,
  MPI_Request *request
);

Parameters

   buf
          [in] initial address of send buffer (choice)

   count
          [in] number of elements sent (integer)

   datatype
          [in] type of each element (handle)

   dest
          [in] rank of destination (integer)

   tag
          [in] message tag (integer)

   comm
          [in] communicator (handle)

   request
          [out] communication request (handle)

Remarks

   Creates a persistent communication request for a standard mode send
   operation, and binds to it all the arguments of a send operation.

   Often a communication with the same argument list is repeatedly
   executed within the inner loop of a parallel computation. In such a
   situation, it may be possible to optimize the communication by binding
   the list of communication arguments to a persistent communication
   request once and, then, repeatedly using the request to initiate and
   complete messages. The persistent request thus created can be thought
   of as a communication port or a "half-channel." It does not provide the
   full functionality of a conventional channel, since there is no binding
   of the send port to the receive port. This construct allows reduction
   of the overhead for communication between the process and communication
   controller, but not of the overhead for communication between one
   communication controller and another. It is not necessary that messages
   sent with a persistent request be received by a receive operation using
   a persistent request, or vice versa.

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

   MPI_ERR_COMM
          Invalid communicator. A common error is to use a null
          communicator in a call (not even allowed in MPI_Comm_rank).

   MPI_ERR_COUNT
          Invalid count argument. Count arguments must be non-negative; a
          count of zero is often valid.

   MPI_ERR_TYPE
          Invalid datatype argument. May be an uncommitted MPI_Datatype
          (see MPI_Type_commit).

   MPI_ERR_RANK
          Invalid source or destination rank. Ranks must be between zero
          and the size of the communicator minus one; ranks in a receive
          (MPI_Recv, MPI_Irecv, MPI_Sendrecv, etc.) may also be
          MPI_ANY_SOURCE.

   MPI_ERR_TAG
          Invalid tag argument. Tags must be non-negative; tags in a
          receive (MPI_Recv, MPI_Irecv, MPI_Sendrecv, etc.) may also be
          MPI_ANY_TAG. The largest tag value is available through the the
          attribute MPI_TAG_UB.

See Also

   MPI_Buffer_attach

*/
 
// Copyright 2009 Deino Software. All rights reserved.
// Source: http://mpi.deino.net/mpi_functions/index.htm

#include <stdio.h>
#include "mpi.h"
#define BUFSIZE 2000
int main( int argc, char *argv[] )
{
    MPI_Status status;
    MPI_Request request;
    int a[10], b[10];
    int buf[BUFSIZE], *bptr, bl, i, j, rank, size;
    int errs = 0;
    MPI_Init( 0, 0 );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    MPI_Buffer_attach( buf, BUFSIZE );
    for (j=0; j<10; j++) {
        MPI_Bsend_init( a, 10, MPI_INT, 0, 27+j, MPI_COMM_WORLD,
&request );
        for (i=0; i<10; i++) {
            a[i] = (rank + 10 * j) * size + i;
        }
        MPI_Start( &request );
        MPI_Wait( &request, &status );
        MPI_Request_free( &request );
    }
    if (rank == 0) {
        for (i=0; i<size; i++) {
            for (j=0; j<10; j++) {
                int k;
                status.MPI_TAG = -10;
                status.MPI_SOURCE = -20;
                MPI_Recv( b, 10, MPI_INT, i, 27+j, MPI_COMM_WORLD,
&status );
                if (status.MPI_TAG != 27+j) {
                    errs++;
                    printf( "Wrong tag = %d\n", status.MPI_TAG
);fflush(stdout);
                }
                if (status.MPI_SOURCE != i) {
                    errs++;
                    printf( "Wrong source = %d\n", status.MPI_SOURCE
);fflush(stdout);
                }
                for (k=0; k<10; k++) {
                    if (b[k] != (i + 10 * j) * size + k) {
                        errs++;
                        printf( "received b[%d] = %d from %d tag %d\n",
                                k, b[k], i, 27+j );fflush(stdout);
                    }
                }
            }
        }
    }
    MPI_Buffer_detach( &bptr, &bl );
    MPI_Finalize();
    return 0;
}

