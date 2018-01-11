ramfs
=====

The layout of ramfs is extremely simple.
All integers are 32-bit and little endian.
The beginning of the file system starts with the root directory.

Here's the layout of a file:

| Offset | Name              | Type   | Size (bytes)           |
|--------|-------------------|--------|------------------------|
| 0x0000 | Name Size         | uint32 | 4                      |
| 0x0004 | File Size         | uint32 | 4                      |
| 0x---- | Name              | char   | (Name Size)            |
| 0x---- | File Data         |        | (File Size)            |

Here's the layout of a directory:

| Offset | Name              | Type   | Size (bytes)           |
|--------|-------------------|--------|------------------------|
| 0x0000 | Name Size         | uint32 | 4                      |
| 0x0004 | File Count        | uint32 | 4                      |
| 0x0008 | Directory Count   | uint32 | 4                      |
| 0x000c | Name              | char   | (Name Size)            |
| 0x---- | File Entries      |        | (File Entry Size)      |
| 0x---- | Directory Entries |        | (Directory Entry Size) |

