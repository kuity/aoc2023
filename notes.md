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
Just generate arrays based on the difference between adjacent elements in the original array.
When the generated array is all 0s, just add all the final elements in each array.

## Part 2
This is almost the same as part 1 except in the final calculate, we should subtract from the first element of the generated arrays.

# Day 10
## Part 1
This is straightforward as the input is guaranteed to loop and the start tile is provided.
Thus, can just keep traversing the loop until the start is revisited.

## Part 2
This part is really tricky.
It seems like there is no simple solution so I checked on Reddit and found the shoestring formula.

Then it's just a case of applying the formula to calculate the area of the loop.

After which, Pick's theorem may be used to get the number of interior points in the loop, which is the answer.

# Day 11
## Part 1
In the first part, we can easily implement what was described in the problem setting.

We can simply identify all the "empty" rows and columns and expand them.

## Part 2
The expansion cannot be done for obvious reasons.

However, similar idea as Part 1 may still be used by identifying all empty rows and columns.

And then, simple math can be done by multiplying the number of empty rows and cols with the large constant and add it to the distance.

# Day 12
## Part 1
## Part 2
The rules for this are pretty complicated.

But it can be framed into a DP problem.
In each function call, we basically need to have 2 parameters, remaining numbers and remaining requirements.
The tricky part is getting the terminating conditions right.

After memoizing the recursive function calls, the efficiency is good enough to solve both parts with a reasonable runtime.

# Day 13
## Part 1
## Part 2

# Day 14
## Part 1
## Part 2

# Day 15
## Part 1
## Part 2

# Day 16
## Part 1
## Part 2

# Day 17
## Part 1
## Part 2

# Day 18
## Part 1
## Part 2

# Day 19
## Part 1
## Part 2

# Day 20 
## Part 1
## Part 2

# Day 21
## Part 1
## Part 2

# Day 22
## Part 1
## Part 2

# Day 23
## Part 1
## Part 2

# Day 24
## Part 1
## Part 2

# Day 25
## Part 1
