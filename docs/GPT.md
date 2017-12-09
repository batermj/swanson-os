GUID Partition Table
====================

The GUID Partition Table (otherwise known as GPT), is a partitioning specification.
It is formally defined in the [UEFI Standard](http://www.uefi.org/sites/default/files/resources/UEFI%202_5.pdf), section 5.
Here, the usage of GPT within the kernel is explained.

A GPT-formatted disk is exampled with the @ref gpt_accessor structure.
This structure is intended to abstract the method in which the GPT data
structures are accessed. It is meant for reading data structures, but not
modifying them. To modify a GPT data structure, the GPT mutator structure
may be used (see @ref gpt_mutator).
