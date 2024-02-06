# Day 1 
## Part 1
Simply get all the numbers from each string and add them

## Part 2
A number could be represented by either a single numeric char or a string

For latter case, get the first char of each numeric string, eg. nine->'n'

Then if first char match, try to find corresponding numeric string in the input

# Day 2
## Part 1
This is just checking if the "games" in each input passes the imposed constraints

## Part 2
This is just keeping a "running max" for each color over the input

# Day 3
## Part 1
This is just iterating through a 2D board and checking each cell's value

## Part 2
The same as Part 1 but with more constraints. Need to check all neighbors of specific cells.

# Day 4
## Part 1
Just parsing and checking each token in the input

## Part 2
Need a hashmap to keep track of the number of copies of each scratchcard.
Then we can just iterate through the scratchcards and update the copies

# Day 5
## Part 1
This is just consecutive value-mappings

## Part 2
The provided seeds are actually ranges of values instead of individual values.
So apply the mapping to each range of value, then determine the lowest value out of all ranges.

# Day 6
## Part 1
The first answer can just be found by a brute force counting approach

## Part 2
We can just solve (get roots) for the equation for acceleration and displacement to get the answer

# Day 7
## Part 1
Just need to calculate mapped value of each input according to the description

## Part 2
We need to account for the Joker card before calculating the rank of each input

# Day 8
## Part 1
Part 1 is really simple, just keep processing the instructions.

## Part 2
This part is tricky because it takes way too long to bruteforce.
However, each set hits the "end" in a regular cycle.
Thus, we just need to find the iteration number where the cycles coincide, which reduces to an LCM problem.

# Day 9
## Part 1
## Part 2

# Day 10
## Part 1
## Part 2

# Day 11
## Part 1
## Part 2

# Day 12
## Part 1
## Part 2


