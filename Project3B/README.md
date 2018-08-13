# LFile System Consistency Analysis
## INTRODUCTION:
A file system is an example of a large aggregation of complex and highly inter-related data structures. Such systems are often at risk for corruption, most commonly resulting from incomplete execution of what should have been all-or-none updates. Unless we can so cleverly design our data structures and algorithms as to render such errors impossible, it is usually necessary to articulate consistency assertions and develop a utility to audit those systems for consistency, and (where possible) repair any detected anomalies. In part A of this project we wrote a program to examine on-disk data structures and summarize their contents. In this second part, we will analyze those summaries to detect errors and inconsistencies.

All previous projects have been implemented in C, which exposes the underlying system calls and is well suited for low level operations on well-defined data structures. This project involves trivial text processing (to read .csv input) and the assembly and processing of an internal state model. While this can certainly be done in C, you may find it easier to do in a higher level language (e.g., Python). You are free implement this project in any language that is supported on the departmental servers (where we will do the testing).



