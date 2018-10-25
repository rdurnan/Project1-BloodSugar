Ryan Durnan Project1 Submission

This project creates a user interface that is meant to store blood sugar readings over two weeks.
I provide the user with commands to enter information, check daily summaries, check weekly 
summaries, and move on to the next day. 

When the user enters a value, I store it in an array of linked lists. The index of the array 
indicates which day the user is adding to. 

When the user checks their daily summary, I display the current sum of entries for the day, 
the current max for the day, the current min for the day, and the number of entries for the day. 

When the user checks their weekly summary, I display the current sum of entries for the week, the 
current max for the week, the current min for the week, the number of entries for the week, and 
the largest day-to-day delta of the number of entries for the week.

At the end of the weeks, I display all of the same information that I show for weekly summaries. 
The only difference is that I summarize the full two weeks instead of just one.

To create linked lists, I referenced the code files from Lab2. See LinkedList.cpp, LinkedList.h, 
Node.cpp, and Node.h

I also used a function that I found on Stack Overflow to help recognize if a string could be 
interpreted as a floating point number.
https://stackoverflow.com/questions/447206/c-isfloat-function
