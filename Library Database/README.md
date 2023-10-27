Radulescu Matei

Hashmap implementation of a library database:

I use a function that read one word at a time in order to read each command, i also
use a function in order to read the name of each book as it can contain spaces.

In my main function i read commands while the given command is not EXIT, i than have
an if that checks for each command, i have added an invalid command case which helped
me for debugging.

I made four structs in order to help me save all the data i needed for the data base i had to
build. 

For the books i made a struct in which i save a hashtable which contains the definitions
in the book, a variable that shows wether the book is borrowed a not, it's total number of
purchases, it's total rating and the number of days that it has been borrowed for. I also
made a struct that has a books rating and it's name which i use for showing the top books.

For the users, i made a struct in which i save the users point, wether or not he borrowed a
book and the name of the book which he borrowed. I also made a struct that has the users
name and his points which is use for showing the top users.

For each command i made a function that does whatever is required to. I explained their
functionality in the comments that i put throughout the code

Overall i liked the homework and i think it was very well made. The only thing i struglled a bit
with is the resize function which i had some trouble implementing it and i finnaly made it work
after rewriting it a couple of times.

I hope you have a nice day.

