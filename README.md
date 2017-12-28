# Various C++ Containers

This project serves as an excercise/assignment for a course in algorhitms and data structers at my university. Main goal was to design a Set class which would be implemented either by using BinaryTree or Linked list behind the scenes. In order for other operations to work one would also need Stack and Queue implementations as well, and I also wanted to test the performance with Vectors to see how it behaves.


Container implementation was inspired by STL, and I think it can be considered STL semi-compliant as begin() and end() container functions return iterators, so that could be used in shortand for loops. Only one kind of iterator is implemented though, and there are no specific constant iterator as that wasn't my main focus and would shift thinking more towards good OOP practices and less to algorhitms underneath, which was the initial goal of the project.


Benchmark class is here also implemented utilizing PerformanceCalculator class which I was given from professors and its purpose is testing different implementations of Set class. I am using functions from C++11 random header to generate random numbers and using seed constant to make sure that same numbers will be generated for different class implementations.


This code compiles sucessfully on Microsoft's native Visual Studio compiler. PerformanceCalculator class is dependant on windows' headers so 
benchmarking part of the code is windows dependant.