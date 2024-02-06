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
This can be done by checking substrings in every row and column and looking for indices where a "reflection happens"

After which, the frequency of the reflection indices can be stored. If the frequency is equal to the total number of cols/rows, a true reflection is found.

## Part 2
Here we just need to slightly modify the method in part 1 by looking for reflection index frequencies which are exactly off by one. 

# Day 14
## Part 1
Simple calculation can be performed by iterating through each column.
Accumulate the number of Os until a # is encountered.
Then calculate the weight based on the position of the previous hex, and the number of Os.

## Part 2
A function could be written for "rotating" the board.

After running the function for a few iterations, again we see that the accumulated value is repeating in a cycle.

After which, it's just a modulo problem.

# Day 15
## Part 1
Very straightfoward, just implement a simple hashing algorithm and do simple sum.

## Part 2
More complexity but still rather straightfoward, just directly implement.

# Day 16
## Part 1
The challenge was in making custom structs to represent the Lights and Cells.

After these are represented, an iterative algorithm that processes active Lights in queue works.

Simply allow the algorithm to run to a halt then count the total number of Cells that contains a Light.

## Part 2
Basically, the same method from Part 1 could be recycled.

We just need to check every possible position for the starting light, and get the configuration with the maximum value.

# Day 17
## Part 1
## Part 2
This is an application of Dijkstra's algorithm with some minor addons.

Basically it is a minimum cost problem with known individual edge costs and a starting node.

However, it is not a straightforward application because there are some "movement constraints".

Therefore, a clever workaround can be done by "re-defining" each node to include not just the x-y position, but also the number of steps taken and last direction.

After which, it is just an implementation problem.

# Day 18
## Part 1
## Part 2
Both parts are again an application of Shoestring formula + Pick's theorem.

# Day 19
## Part 1
I went with a rather complicated representation of the problem.

"part" has a custom struct and "node" has a custom struct.

Thus there is a recursive function that takes a node and a part, and routes the part through the relevant nodes, until it reaches the final Accept or Reject node.

## Part 2
The Part 1 approach is still valid, except that now the evaluation function needs to take a range of values instead of one single value.

# Day 20 
## Part 1
Again, some work needs to be done to create custom structs for these components.

Once the structs are all defined, we can just call the function to process a "pulse" for the stipulated number of iterations.

## Part 2
This part again reduces to an LCM problem and some manual observation skills are required to find the cycle.

A little tricky with some assumptions having to be made.

As even after identifying the critical nodes, need to understand that multiple pulses may be sent from each node within one "iteration".

We'll just need to assume that each iteration where a node sends at least 1 "high pulse" can be marked, and then load for the common iteration between all critical nodes.

# Day 21
## Part 1
Part 1 here is pretty trivial as we just need to call a process function for the stipulated number of iterations, then do a count at the end.

## Part 2
This suddenly becomes very hard because the board can extend infinitely in all directions.

Since the sample size grows exponentially, it becomes impossible to brute force.

I found an ingenious geometric solution which exploits the fact that the number of boards at step n is derivable, and also that at certain step numbers, the number of reachable squares is calculable easily.

The rest is just implementing the solution.

# Day 22
## Part 1
First need to make a custom struct for a brick.

It is easier if each brick contains which other bricks it "supports" and is "supported by".

Then write a function that can process the falling of each brick in each coordinate.

Finally, just iterate through all available coordinates to update the supported By for each brick.

If a brick is supported by only one other brick, simply increment the count.

## Part 2
This is like an extension of Part 1, except we should update the "supports" property for each brick.

Using this property, can easily find how many bricks could each brick cause to fall.

# Day 23
## Part 1
The first part can be easily completed with a recursive brute force approach without optimization.

## Part 2
The problem space is a lot larger and brute force does not work.

Since it is a maximum path problem, it can't be solved with Dijkstra's algorithm.

However, using some path "pruning", the problem space can be reduced.

Since there are many "2-way streets", we can reduce these streets to just the start, the end, and the distance in between.

With this optimization, the brute force approach becomes viable.

# Day 24
## Part 1
Using cross product, deriving the answer is straightforward.

## Part 2
A system of linear equations can be generated through a clever manipulation of the cross product.
This can be trivially solved by multiplying the inverse matrix on both sides of the eqn.
The eigen library is super useful for the implementation.

# Day 25
## Part 1
Another ingenious solution was taken from Reddit.

Basically, heuristic can be used by abusing the fact that exactly 3 "cuts" separates the 2 groups.

Thus in the final desired state, the total number of neighbors when counted, should be exactly 3.

By starting off with 1 group and evicting nodes to the other group based on a heuristic, the solution can be derived iteratively.

It's worth noting that this doesn't "guarantee" a solution, depending on the initial nodes evicted.

In the case of the given input though, it works :)
