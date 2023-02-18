# River Crossing Problem

There is an incoming stream of lions and rats. We have to ensure they cross the river over a boat. The boat can accommodate at most 4 living beings. We have to make a constraint such that there can't be 3 lions 1 rat and 3 rats 1 lion on the boat while crossing.

### Input
```
./river-crossing lrlrllrr
```

<!-- 
`rats.txt`:
First line:
number of rats incoming

Second Line Numbers in ascending order denoting time when a rat enters
1 2 5 7
4

`lions.txt`:

First line:
number of lions incoming

Second Line Numbers in ascending order denoting time when a lion enters
2 3 4 6
4
-->
### Output
Index of rats/lions rowed in boat.
```
Boat 0 Rowed: 
Lion: 0
Lion: 2
Rat: 1
Rat: 3
Boat 1 Rowed:
Lion: 4
Lion: 5
Rat: 6
Rat: 7
```
<!-- 
### Shared State
1. Rats:
    - RatArrive:
        1. The rat arrive waits for 1 more rat to arrive (rats arrive >= 2)
        1. Only two rats are allowed in to board the boat (using a pairing conditional). The other rats signals the rat waiting in stage 1. A rat can only be paired with its neighboring rats.
        1. 

    - LionArrive: Same as rat

|rat0|ra1|rat2|
|-|-|-|
|rat0 arrive  || |
|rat0 check rat-val == 0 else wait|||
|rat0 updated rat-val = 1|||
|rat0 wait for onboard signal |rat1 arrive |rat2 arrive|
|rat1 wait for onboard signal|||
| |rat1 passes onboards signal||
|-|-|-| -->
