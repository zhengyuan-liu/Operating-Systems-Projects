# Project 2B Lock Granularity and Performance
## INTRODUCTION:
In the previous project the mutex and spin-lock proved to be bottlenecks, preventing parallel access to the list. In this project, you will do additional performance instrumentation to confirm this, and extend your previous solution to deal with this problem. This project can be broken up into a few major steps:

* Do performance instrumentation and measurement to confirm the cause of the problem.
* Implement a new option to divide a list into sublists and support synchronization on sublists, thus allowing parallel access to the (original) list.
* Do new performance measurements to confirm that the problem has been solved.
