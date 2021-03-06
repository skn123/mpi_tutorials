/* 
MPI_Type_match_size

   Find an MPI datatype matching a specified size
int MPI_Type_match_size(
  int typeclass,
  int size,
  MPI_Datatype *datatype
);

Parameters

   typeclass
          [in] generic type specifier (integer)

   size
          [in] size, in bytes, of representation (integer)

   datatype
          [out] datatype with correct type, size (handle)

Remarks

   typeclass is one of MPI_TYPECLASS_REAL, MPI_TYPECLASS_INTEGER and
   MPI_TYPECLASS_COMPLEX, corresponding to the desired typeclass. The
   function returns an MPI datatype matching a local variable of type (
   typeclass, size ).

   This function returns a reference (handle) to one of the predefined
   named datatypes, not a duplicate. This type cannot be freed.
   MPI_TYPE_MATCH_SIZE can be used to obtain a size-specific type that
   matches a Fortran numeric intrinsic type by first calling MPI_SIZEOF in
   order to compute the variable size, and then calling
   MPI_TYPE_MATCH_SIZE to find a suitable datatype. In C and C++, one can
   use the C function sizeof(), instead of MPI_SIZEOF. In addition, for
   variables of default kind the variable's size can be computed by a call
   to MPI_TYPE_GET_EXTENT, if the typeclass is known. It is erroneous to
   specify a size not supported by the compiler.

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

   MPI_ERR_ARG
          Invalid argument. Some argument is invalid and is not identified
          by a specific error class (e.g., MPI_ERR_RANK).

*/
 
// Copyright 2009 Deino Software. All rights reserved.
// Source: http://mpi.deino.net/mpi_functions/index.htm

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * type match size is part of the extended Fortran support
 */
int main( int argc, char *argv[] )
{
    int errs = 0, err;
    int dsize;
    MPI_Datatype  newtype;
    MPI_Init( &argc, &argv );
    /* Check the most likely cases.  Note that it is an error to
       free the type returned by MPI_Type_match_size.  Also note
       that it is an error to request a size not supported by the compiler,
       so Type_match_size should generate an error in that case */
    
    MPI_Comm_set_errhandler( MPI_COMM_WORLD, MPI_ERRORS_RETURN );
    err = MPI_Type_match_size( MPI_TYPECLASS_REAL, sizeof(float), &newtype );
    if (err) {
        errs++;
    }
    else {
        err = MPI_Type_size( newtype, &dsize );
        if (err) {
            errs++;
        }
        else {
            if (dsize != sizeof(float)) {
                errs++;
                printf( "Unexpected size for float (%d != %d)\n",
			dsize, (int) sizeof(float) );
		fflush(stdout);
            }
        }
    }
    
    MPI_Finalize();
    return errs;
}

