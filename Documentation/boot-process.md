Boot Process
============

When the operating system is started, the kernel data structure is constructed from the host.
For the kernel to be properly constructed, it must contain a memory map and at least one disk for the file system.
The memory map is contained by the @ref memmap structure. It must contain at least one section of free memory that
is large enough for the system to run properly. The disk is contained by the @ref fdisk structure. The disk must contain
a GPT formatted partitioning scheme. The kernel then searches for a 'Swanson' partition that contains the primary file system.
The partition is identified by it's globally unique identifier, @ref GPT_GUID_SWANSON.

Once the kernel is properly constructed, it will attempt to open an operating system. The operating system is started by a file
called '/sbin/init'. If this file does not exist, the kernel will print a debug message and exit (otherwise known as a 'panic').
If the file does exist, it is loaded into memory and, in turn, loads the operating system.
