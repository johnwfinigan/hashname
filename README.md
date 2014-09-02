hashname
=========

Tool for generating (probably) unique filenames for data read from stdin.

purpose
=========

Given data on stdin, this program:

      * reads it in, and writes it to a file with a temporary name
      * calculates the SHA256 hash of the data
      * on EOF, renames the temporary-named file to a name derived from the SHA256 hash

The intended purpose is to write data to disk with unique names in a lockless concurrency safe way

build
=========

gcc -Os -Wall hashname.c -lssl -lcrypto -o hashname

run
=========

cat hashname.c | ./hashname

A new file will be generated, containing hashname.c's data and named based on the SHA256 of that data

bugs
=========

	* minimal testing (I am satisfied that it works for my purpose)
	* lack of configurability
	* hash collisions could cause data to be overwritten


