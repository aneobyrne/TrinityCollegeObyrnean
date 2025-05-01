# Practical 4

## Background

This practical has two parts (scheduler/filesystem),
both of which involve you writing a file that describes key observations of the behaviour of a given scenario.

## Submission

There are two files to be submitted **together**,
called `scheduler.txt` and `filesystem.txt`.
Their formats are described below.

 
### Part 1 - Scheduler Simulation (SJFP)

This part describes four (lightly) randomised processes P1..P4, with burst sizes in the ranges 2..8. Their arrival times are predetermined, ranging from 0 to 3 in order. The only guarantee you have is that the shortest job does not arrive first, and the longest job does not arrive last.

You are asked to determine the order in which parts of each process will execute given a Shortest-Job First Preemptive scheduler. An example of this is given in the Week-9 Slides, "43-Scheduler-Examples", on slides 2, 9, 10, and 11.


#### Objective

You create a text file called `scheduler.txt` which describes the outcome using a simple format, which is a series of lines of the form:

```
RUN <procno> Q <procnos> ;
```

Here, `<procno>` is the number of the currently running process,
while `<procnos>` is zero or more process numbers, seperated by whitespace, representing the queue (head-first).

So `RUN 3 Q 4 1 ;` states that P3 is running,
with two processes in the queue (P4,P1) where P4 is the next to run.

The first six slots from the example in slide 11 are described as follows:

```
RUN 1 Q ;
RUN 2 Q 1 ;
RUN 2 Q 1 3 ;
RUN 2 Q 4 1 3 ;
RUN 2 Q 4 1 3 ;
RUN 4 Q 1 3 ;
```
Use of whitespace between items is allowed.

### Submission

For Part 1 you submit file `scheduler.txt`.


 
### Part 2 - Filesystem Simulation 

This part describes a file-system setup with 3 processes,
a collection of read and write buffers,
and a file-system currently with only an empty root directory,
represented by a single inode with an empty block list.
Inodes, File-Descriptors and Data/Disk Blocks are provided, accessed by number.

Also provided is code that interleaves the actions of the three threads,
which involves opening some files, reading or writing them, and then closing them.
The following things are true about the threads and file actions:

- All file-operations run successfully.
- No two threads access the same file.
- All files are opened before reading or writing.
- No attempt is made to open a file for both reading and writing.
- Files are only closed once all reading and writing is done.
- We do not do Directory *creation* or changing directory.


#### Objective

You create a text file called `filesystem.txt` which describes the changes to all the key data-structures involved as the program runs. These are the data-blocks (`BLOCK`),the inodes (`INODE`), the file-descriptors (`FDESCR`), and read buffers (`BUFFER`). The first three of the above are provided in an array, indexed from zero. All are initially marked as "free", 
except for Inode 0 that represents the empty root directory, which is marked as "used".
The buffers have the same content-type as data-blocks but are accessed (here) via static program variables.

For each data-structure object there are two changes that can occur:

 - Allocation (`ALLOC`) : the free object with the lowest number is chosen, its status is changed to used and its contents are initialised.
 - Update (`UPDATE`) : covers two possibilities:
   - a used object has some of its contents changed, and is still considered used.
   - an object has been freed,  so the status of the object is changed to free (no other data in the object is altered in any way).

The contents of the data-structure types are:

- A Data Block (`BLOCK`) represents the data being written to disk. For simplicity we assume that a block either holds a single number(`NORMAL <number>`), or a directory entry that pairs a filename with an inode number (`DIR <filename> <inode-no>`).
- A Buffer (`BUFFER`) is like a Block but represents data stored in RAM, and not on disk. It is not allocated, but can the updated.
- The Inode (`INODE`) contains five values in the following order:
  - the filetype `<ftype>`, either `NORMAL` or `DIR`.
  - the current file size `<size>`, here the number of blocks.
  - the link count `<links>`.
  - the opened count `<opens>`.
  - the list `<blocks>` (possibly empty) of block *numbers*.
- A File Descriptor (`FDESCR`) contains four values as follows:
  - the number of the process that opened the file `<procid>`.
  - the inode number `<ind`> of the opened file.
  - the read/write mode `<rw>` in which the file is opened (`r` or `w`).
  - the current position `<cpos>` in the file, here measured in blocks.

The change commands (`ALLOC`,`UPDATE`) all take some extra initial parameters:

- the structure type (`BLOCK`,`INODE`,`FDESCR`)
- the structure number (`<blkn>`,`<inode>`,`<fd>`).
- the free/used indicator (`FREE`,`USED`).
  - If this has value `USED` then the contents are also listed
  - If this is `FREE` then no further details are provided.
- For a buffer, only `UPDATE` applies with the buffer name and value.

This results in the following possibilities:

```
ALLOC BLOCK <blkn> USED NORMAL <number>
ALLOC BLOCK <blkn> USED DIR <filename> <inode>
ALLOC INODE <inode> USED <ftype> <size> <links> <opens> <blocks>
ALLOC FDESCR <fd> USED <procid> <inode> <rw> <cpos>
UPDATE BLOCK <blkn> USED NORMAL <number>
UPDATE BLOCK <blkn> USED DIR <filename> <inode>
UPDATE INODE <inode> USED <ftype> <size> <links> <opens> <blocks>
UPDATE FDESCR <fd> USED <procid> <inode> <rw> <cpos>
UPDATE BLOCK <blkn> FREE
UPDATE INODE <inode> FREE
UPDATE FDESCR <fd> FREE
UPDATE BUFFER <bufvar> NORMAL <number>
UPDATE BUFFER <bufvar> DIR <filename> <inode>
```

Your task is to use the above notation to list out all the allocations, updates
and freeing that occurs as your program runs.

### Code Behaviour

The code is written using a simplified version of the most basic file operations found in Unix:

- `fd := open(fnm,rw)` opens the file names `fnm` for reading (`rw==r`) or writing (`rw==w`). The file-descriptor number is returned, here assigned to variable `fd`. If opening for writing and the file does not exist, then the file is created with empty contents.
- `close(fd)` closes the file designated by the file-descriptor indicated by `fd`. 
- `read(fd,buf)` reads the next block from the file  indicated by `fd` and writes it into the buffer `buf`.
- `write(fd,buf)` writes the contents of buffer `buf` to the next available block in the file indicated by `fd`.

### Submission

For Part 2 you submit file `filesystem.txt`.
