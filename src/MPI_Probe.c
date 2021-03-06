/* 
MPI_Probe

   Blocking test for a message
int MPI_Probe(
  int source,
  int tag,
  MPI_Comm comm,
  MPI_Status *status
);

Parameters

   source
          [in] source rank, or MPI_ANY_SOURCE (integer)

   tag
          [in] tag value or MPI_ANY_TAG (integer)

   comm
          [in] communicator (handle)

   status
          [out] status object (Status)

Remarks

   MPI_PROBE behaves like MPI_IPROBE except that it is a blocking call
   that returns only after a matching message has been found.

   The MPI implementation of MPI_PROBE and MPI_IPROBE needs to guarantee
   progress: if a call to MPI_PROBE has been issued by a process, and a
   send that matches the probe has been initiated by some process, then
   the call to MPI_PROBE will return, unless the message is received by
   another concurrent receive operation (that is executed by another
   thread at the probing process). Similarly, if a process busy waits with
   MPI_IPROBE and a matching message has been issued, then the call to
   MPI_IPROBE will eventually return flag = true unless the message is
   received by another concurrent receive operation.

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

   MPI_ERR_TAG
          Invalid tag argument. Tags must be non-negative; tags in a
          receive (MPI_Recv, MPI_Irecv, MPI_Sendrecv, etc.) may also be
          MPI_ANY_TAG. The largest tag value is available through the the
          attribute MPI_TAG_UB.

   MPI_ERR_RANK
          Invalid source or destination rank. Ranks must be between zero
          and the size of the communicator minus one; ranks in a receive
          (MPI_Recv, MPI_Irecv, MPI_Sendrecv, etc.) may also be
          MPI_ANY_SOURCE.

*/
 
// Copyright 2009 Deino Software. All rights reserved.
// Source: http://mpi.deino.net/mpi_functions/index.htm

#include "mpi.h"
#include <stdio.h>

#define MAX_BUF_SIZE_LG 22
#define NUM_MSGS_PER_BUF_SIZE 5
char buf[1 << MAX_BUF_SIZE_LG];

/*
* This program verifies that MPI_Probe() is operating properly in the face of
* unexpected messages arriving after MPI_Probe() has
* been called.
*/

int main(int argc, char **argv)
{
    int p_size;
    int p_rank;
    int msg_size_lg;
    int errs = 0;
    int mpi_errno;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &p_rank);

    for (msg_size_lg = 0; msg_size_lg <= MAX_BUF_SIZE_LG; msg_size_lg++)
    {
        const int msg_size = 1 << msg_size_lg;
        int msg_cnt;
        printf( "testing messages of size %d\n", msg_size);
	fflush(stdout);
        for (msg_cnt = 0; msg_cnt < NUM_MSGS_PER_BUF_SIZE; msg_cnt++)
        {
            MPI_Status status;
            const int tag = msg_size_lg * NUM_MSGS_PER_BUF_SIZE + msg_cnt;
            printf( "Message count %d\n", msg_cnt );
	    fflush(stdout);
            if (p_rank == 0)
            {
                int p;
                for (p = 1; p < p_size; p ++)
                {
                    /* Wait for synchronization message */
                    MPI_Recv(NULL, 0, MPI_BYTE, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
                    if (status.MPI_TAG != tag)
                    {
                        printf("ERROR: unexpected message tag from MPI_Recv(): lp=0, rp=%d, expected=%d, actual=%d, count=%d\n",
                                status.MPI_SOURCE, status.MPI_TAG, tag, msg_cnt);fflush(stdout);
                    }

                    /* Send unexpected message which hopefully MPI_Probe() is already waiting for at the remote process */
                    MPI_Send (buf, msg_size, MPI_BYTE, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD);
                }
            }
            else
            {
                int incoming_msg_size;

                /* Send synchronization message */
                MPI_Send(NULL, 0, MPI_BYTE, 0, tag, MPI_COMM_WORLD);
                /* Perform probe, hopefully before the master process can send its reply */
                MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                MPI_Get_count(&status, MPI_BYTE, &incoming_msg_size);
                if (status.MPI_SOURCE != 0)
                {
                    printf("ERROR: unexpected message source from MPI_Probe(): p=%d, expected=0, actual=%d, count=%d\n",
			   p_rank, status.MPI_SOURCE, msg_cnt);
		    fflush(stdout);
                }
                if (status.MPI_TAG != tag)
                {
                    printf("ERROR: unexpected message tag from MPI_Probe(): p=%d, expected=%d, actual=%d, count=%d\n",
			   p_rank, tag, status.MPI_TAG, msg_cnt);
		    fflush(stdout);
                }
                if (incoming_msg_size != msg_size)
                {
                    printf("ERROR: unexpected message size from MPI_Probe(): p=%d, expected=%d, actual=%d, count=%d\n",
			   p_rank, msg_size, incoming_msg_size, msg_cnt);
		    fflush(stdout);
                }

                /* Receive the probed message from the master process
*/
                MPI_Recv(buf, msg_size, MPI_BYTE, 0, tag, MPI_COMM_WORLD, &status);
                MPI_Get_count(&status, MPI_BYTE, &incoming_msg_size);
                if (status.MPI_SOURCE != 0)
                {
                    printf("ERROR: unexpected message source from MPI_Recv(): p=%d, expected=0, actual=%d, count=%d\n",
			   p_rank, status.MPI_SOURCE, msg_cnt);
		    fflush(stdout);
                }
                if (status.MPI_TAG != tag)
                {
                    printf("ERROR: unexpected message tag from MPI_Recv(): p=%d, expected=%d, actual=%d, count=%d\n",
			   p_rank, tag, status.MPI_TAG, msg_cnt);
		    fflush(stdout);
                }
                if (incoming_msg_size != msg_size)
                {
                    printf("ERROR: unexpected message size from MPI_Recv(): p=%d, expected=%d, actual=%d, count=%d\n",
			   p_rank, msg_size, incoming_msg_size, msg_cnt);
		    fflush(stdout);
                }
            }
        }
    }

    MPI_Finalize();
    return 0;
}
