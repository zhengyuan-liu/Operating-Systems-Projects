# File System Interpretation
## INTRODUCTION:
We are all familiar with the characteristics of the files and directories in which we store all of our data. As with many other persistent objects, their functionality, generality, performance and robustness all derive from the underlying data structures used to implement them. In this project we will design and implement a program to read the on-disk representation of a file system, analyze it, and summarize its contents. In the next project, we will write a program to analyze this summary for evidence of corruption.

This project can be broken into two major steps:
* Understand the on-disk data format of the EXT2 file system. We will mount a provided image file on your own Linux system and explore it with familiar file navigation commands and debugfs(8).
* Write a program to analyze the file system in that image file and output a summary to standard out (describing the super block, groups, free-lists, inodes, indirect blocks, and directories).
The second part may involve much more code than we have written in previous projects, but the first part is (by far) the more difficult. Once the underlying data structures are understood, the actual code is likely to be fairly simple.

