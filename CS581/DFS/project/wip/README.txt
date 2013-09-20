DFS Vacuum Cleaner

Given an environment, this agent will search the environment using an online DFS, and clean it. 

The environment is a text file, starting with 2 numbers indicating the width and height, then symbols denoting the various objects;

^,<, > or V indicate the agent (facing up, left, right, or down)
H indicates the home position.
* indicates a dirty square.
. indicates a clean square.
# indicates a wall
| seperates squares.

The agent is awarded 20 points for cleaning a square, and penalized 1 point per action taken. If the agent finishes on its home square, it is rewarded 1000 points. 

Command line format:
(name).exe (name).txt