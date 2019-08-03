# 3DCarGame
First-Award Winning project in PCCOE Computer Department Mini Project Competition, Feb 2019

A simple car game demonstrating orthographic projections of a 3D world,
complete with proper keyboard input support and game loop logic.
There's no throttling (to limit framerate) used and will most probably use up one 
whole core of your machine, not exactly efficient...  But we had only 3 weeks 
to complete the project.

Didn't use github during development so, many intermediate and testing 
code files are scattered all over. 




## infBoost.cpp
Contains the final code, cheers!
First release (also the one presented in the competition): [MSKB.exe](https://github.com/makeway4pK/3DCarGame/releases/download/v1.0/MSKB.exe)

# Development Team:
### [@makeway4pK](http://github.com/makeway4pK)
- Project Lead
- 3D Rendering implementation
  
### [@omkar-7](http://github.com/omkar-7)
- Game logic
- Input control
  
### [@GroudonTheGreat](http://github.com/GroudonTheGreat)
- Data handling(Hi-Scores)
- Optimization & Testing
  
### [@17vjs](http://github.com/17vjs)
- Graphics & UI design
- Screenplay


 If You're planning on testing/contributing to this repo, make sure you get a copy of [Eigen](https://eigen.tuxfamily.org/dox/GettingStarted.html).
 This library helped us to achieve the performance speed we needed for 
a responsive game without multithreading
