## Fall 2021 ECE199 
This little project is for myself to test out some of the basic features of ompl and fcl in c++ enviroment.   
### Installation
To run the code, please first install OMPL and FCL for c++ and matplotlib for python.
### Running
Maze_Planning_NO_Reg does not re-register collision object after each iteration of search,
while Maze_Planning will do it. The no reg version roughly doubles the amount of point tested with RRT*, generaly generats better path. To change the starting and ending points, please change **start(space)**
and **goal(space)**.

**Note**: If you are compiling the code yourself, note that it might take 30sec to 1min to compile. 
  
The two c++ output files were compiled on Ubuntu 20.04, after running it, an out.txt file will begenerated for visualization: 
```bash
./Maze_Planning_NO_Reg
```
or 
```bash
./Maze_Planning
```
To visualize the path, run:
```bash
python draw_maze.py
```
