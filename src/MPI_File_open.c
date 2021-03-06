/* 
MPI_File_open

   Opens a file
int MPI_File_open(
  MPI_Comm comm,
  char *filename,
  int amode,
  MPI_Info info,
  MPI_File *mpi_fh
);

int MPI_File_open(
  MPI_Comm comm,
  wchar_t *filename,
  int amode,
  MPI_Info info,
  MPI_File *mpi_fh
);

Parameters

   comm
          [in] communicator (handle)

   filename
          [in] name of file to open (string)

   amode
          [in] file access mode (integer)

   info
          [in] info object (handle)

   mpi_fh
          [out] file handle (handle)

Remarks

   MPI_FILE_OPEN opens the file identified by the file name filename on
   all processes in the comm communicator group. MPI_FILE_OPEN is a
   collective routine: all processes must provide the same value for
   amode, and all processes must provide filenames that reference the same
   file. (Values for info may vary.) comm must be an intracommunicator; it
   is erroneous to pass an intercommunicator to MPI_FILE_OPEN. Errors in
   MPI_FILE_OPEN are raised using the default file error handler. A
   process can open a file independently of other processes by using the
   MPI_COMM_SELF communicator. The file handle returned, fh, can be
   subsequently used to access the file until the file is closed using
   MPI_FILE_CLOSE. Before calling MPI_FINALIZE, the user is required to
   close (via MPI_FILE_CLOSE) all files that were opened with
   MPI_FILE_OPEN. Note that the communicator comm is unaffected by
   MPI_FILE_OPEN and continues to be usable in all MPI routines (e.g.,
   MPI_SEND). Furthermore, the use of comm will not interfere with I/O
   behavior.

   The format for specifying the file name in the filename argument is
   implementation dependent and must be documented by the implementation.

   Advice to users.

   On some implementations of MPI, the file namespace may not be identical
   from all processes of all applications. For example, "c:\tmp\foo" may
   denote different files on different processes, or a single file may
   have many names, dependent on process location. The user is responsible
   for ensuring that a single file is referenced by the filename argument,
   as it may be impossible for an implementation to detect this type of
   namespace error. ( End of advice to users.)
   Initially, all processes view the file as a linear byte stream, and
   each process views data in its own native representation (no data
   representation conversion is performed). (POSIX files are linear byte
   streams in the native representation.) The file view can be changed via
   the MPI_FILE_SET_VIEW routine.

   The following access modes are supported (specified in amode, a bit
   vector OR of the following integer constants):
     * MPI_MODE_RDONLY --- read only,
     * MPI_MODE_RDWR --- reading and writing,
     * MPI_MODE_WRONLY --- write only,
     * MPI_MODE_CREATE --- create the file if it does not exist,
     * MPI_MODE_EXCL --- error if creating file that already exists,
     * MPI_MODE_DELETE_ON_CLOSE --- delete file on close,
     * MPI_MODE_UNIQUE_OPEN --- file will not be concurrently opened
       elsewhere,
     * MPI_MODE_SEQUENTIAL --- file will only be accessed sequentially,
     * MPI_MODE_APPEND --- set initial position of all file pointers to
       end of file.

   Advice to users.

   C/C++ users can use bit vector OR (|) to combine these constants;
   Fortran 90 users can use the bit vector IOR intrinsic. Fortran 77 users
   can use (nonportably) bit vector IOR on systems that support it.
   Alternatively, Fortran users can portably use integer addition to OR
   the constants (each constant should appear at most once in the
   addition.). ( End of advice to users.)
   The modes MPI_MODE_RDONLY, MPI_MODE_RDWR, MPI_MODE_WRONLY,
   MPI_MODE_CREATE, and MPI_MODE_EXCL have identical semantics to their
   POSIX counterparts. Exactly one of MPI_MODE_RDONLY, MPI_MODE_RDWR, or
   MPI_MODE_WRONLY, must be specified. It is erroneous to specify
   MPI_MODE_CREATE or MPI_MODE_EXCL in conjunction with MPI_MODE_RDONLY;
   it is erroneous to specify MPI_MODE_SEQUENTIAL together with
   MPI_MODE_RDWR.

   The MPI_MODE_DELETE_ON_CLOSE mode causes the file to be deleted
   (equivalent to performing an MPI_FILE_DELETE) when the file is closed.

   The MPI_MODE_UNIQUE_OPEN mode allows an implementation to optimize
   access by eliminating the overhead of file locking. It is erroneous to
   open a file in this mode unless the file will not be concurrently
   opened elsewhere.

   Advice to users.

   For MPI_MODE_UNIQUE_OPEN, not opened elsewhere includes both inside and
   outside the MPI environment. In particular, one needs to be aware of
   potential external events which may open files (e.g., automated backup
   facilities). When MPI_MODE_UNIQUE_OPEN is specified, the user is
   responsible for ensuring that no such external events take place. ( End
   of advice to users.)
   The MPI_MODE_SEQUENTIAL mode allows an implementation to optimize
   access to some sequential devices (tapes and network streams). It is
   erroneous to attempt nonsequential access to a file that has been
   opened in this mode.

   Specifying MPI_MODE_APPEND only guarantees that all shared and
   individual file pointers are positioned at the initial end of file when
   MPI_FILE_OPEN returns. Subsequent positioning of file pointers is
   application dependent. In particular, the implementation does not
   ensure that all writes are appended.

   Errors related to the access mode are raised in the class
   MPI_ERR_AMODE.

   The info argument is used to provide information regarding file access
   patterns and file system specifics. The constant MPI_INFO_NULL can be
   used when no info needs to be specified.
   Advice to users.

   Some file attributes are inherently implementation dependent (e.g.,
   file permissions). These attributes must be set using either the info
   argument or facilities outside the scope of MPI. ( End of advice to
   users.)
   Files are opened by default using nonatomic mode file consistency
   semantics. The more stringent atomic mode consistency semantics,
   required for atomicity of conflicting accesses, can be set using
   MPI_FILE_SET_ATOMICITY.


*/
 
// Copyright 2009 Deino Software. All rights reserved.
// Source: http://mpi.deino.net/mpi_functions/index.htm

/*
vector OR of the following integer constants):
  * MPI_MODE_RDONLY --- read only,
  * MPI_MODE_RDWR --- reading and writing,
  * MPI_MODE_WRONLY --- write only,
  * MPI_MODE_CREATE --- create the file if it does not exist,
  * MPI_MODE_EXCL --- error if creating file that already exists,
  * MPI_MODE_DELETE_ON_CLOSE --- delete file on close,
  * MPI_MODE_UNIQUE_OPEN --- file will not be concurrently opened
    elsewhere,
  * MPI_MODE_SEQUENTIAL --- file will only be accessed sequentially,
  * MPI_MODE_APPEND --- set initial position of all file pointers to
    end of file.

Advice to users.

C/C++ users can use bit vector OR (|) to combine these constants;
Fortran 90 users can use the bit vector IOR intrinsic. Fortran 77 users
can use (nonportably) bit vector IOR on systems that support it.
Alternatively, Fortran users can portably use integer addition to OR
the constants (each constant should appear at most once in the
addition.). ( End of advice to users.)
The modes MPI_MODE_RDONLY, MPI_MODE_RDWR, MPI_MODE_WRONLY,
MPI_MODE_CREATE, and MPI_MODE_EXCL have identical semantics to their
POSIX counterparts. Exactly one of MPI_MODE_RDONLY, MPI_MODE_RDWR, or
MPI_MODE_WRONLY, must be specified. It is erroneous to specify
MPI_MODE_CREATE or MPI_MODE_EXCL in conjunction with MPI_MODE_RDONLY;
it is erroneous to specify MPI_MODE_SEQUENTIAL together with
MPI_MODE_RDWR.

The MPI_MODE_DELETE_ON_CLOSE mode causes the file to be deleted
(equivalent to performing an MPI_FILE_DELETE) when the file is closed.

The MPI_MODE_UNIQUE_OPEN mode allows an implementation to optimize
access by eliminating the overhead of file locking. It is erroneous to
open a file in this mode unless the file will not be concurrently
opened elsewhere.

Advice to users.

For MPI_MODE_UNIQUE_OPEN, not opened elsewhere includes both inside and
outside the MPI environment. In particular, one needs to be aware of
potential external events which may open files (e.g., automated backup
facilities). When MPI_MODE_UNIQUE_OPEN is specified, the user is
responsible for ensuring that no such external events take place. ( End
of advice to users.)
The MPI_MODE_SEQUENTIAL mode allows an implementation to optimize
access to some sequential devices (tapes and network streams). It is
erroneous to attempt nonsequential access to a file that has been
opened in this mode.

Specifying MPI_MODE_APPEND only guarantees that all shared and
individual file pointers are positioned at the initial end of file when
MPI_FILE_OPEN returns. Subsequent positioning of file pointers is
application dependent. In particular, the implementation does not
ensure that all writes are appended.

Errors related to the access mode are raised in the class
MPI_ERR_AMODE.

The info argument is used to provide information regarding file access
patterns and file system specifics. The constant MPI_INFO_NULL can be
used when no info needs to be specified.
Advice to users.

Some file attributes are inherently implementation dependent (e.g.,
file permissions). These attributes must be set using either the info
argument or facilities outside the scope of MPI. ( End of advice to
users.)
Files are opened by default using nonatomic mode file consistency
semantics. The more stringent atomic mode consistency semantics,
required for atomicity of conflicting accesses, can be set using
MPI_FILE_SET_ATOMICITY.


mple Code
*/

#include "mpi.h"
#include <stdio.h>

int main( int argc, char *argv[] )
{
    MPI_Fint handleA, handleB;
    int rc;
    int errs = 0;
    int rank;
    MPI_File cFile;

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    rc = MPI_File_open( MPI_COMM_WORLD, "temp",
			MPI_MODE_RDWR | MPI_MODE_DELETE_ON_CLOSE |
			MPI_MODE_CREATE, MPI_INFO_NULL, &cFile );
    if (rc) {
        printf( "Unable to open file \"temp\"\n" );
	fflush(stdout);
    }
    else {
        MPI_File_close( &cFile );
    }
    
    MPI_Finalize();
    return 0;
}

