# Call-Center-Synchronization-Threading
Final Project for CS3650, Computer Systems, taken at UMass Amherst

Threads and Synchronization: UMass Amherst New Chancellor Inauguration

This project simulates the simultaneous proposal of questions by audience members during the inauguration of a UMass chancellor. Audience members make online calls to propose questions during an inauguration period, which is set by the user. Online calls are managed by separate threads, and semaphores are employed to manage connection access. Each caller is differentiated by an ID and there may be more callers than available operators.

Description:

This program implements threading to handle the inauguration period's length and for online call management. The user inputs a number as a command line argument to manage the duration of the inauguration. This thread is created and runs first through a timer function that accepts the command line argument and runs for that amount of time. 

The program then creates 50 call threads, each individually created in a for loop and passed through the "onlinecall" function. 

The "onlinecall" function assigns each thread a unique ID number. This is a critical region, and is protected with binary semaphore that sequentially increments thread IDs. There are 10 available call lines at any given time, and the connected variable is used to manage the number of threads, or audience members, that are connected. A binary semaphore is again used to manage the incrementation of the connected variable, as this is a critical region since only 10 call lines can be accessed.

If a thread is able to make a connection, another critical region is encountered and combatted with a semaphore. 5 operators are able to take questions simultaneously, so a semaphore allows for 5 threads to continue and propose their questions to the operators. 

A binary semaphore is then used to decrement the connected variable. This is done sequentially to protect the critical region. When connected decreases, another thread is able to make a connection and will then await their opportunity to propose a question. This continues until the timer thread terminates, which causes all threads except the main thread to terminate.

The semaphores are then freed and the program ends.

To Run:

To run this program, run the make command.
Then, run "./umass_Inauguration inauguration_time", where inauguration time is manually inputted as an integer by the user.

Summary:

This project displays the importance of threading and synchronization in a real-world simulation where users simultaneously interact with a system.
