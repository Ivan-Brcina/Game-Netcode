Proof of concept and experimental structure for a networked opengl project.

Initial plan:

A small 3D game scene with two moving cubes on a flat platform independently controlled
by two different processes via loopback, serviced by an authoritative server.

Minimal goal:
Implement netcode that's playable in a real world internet setting with either an actual
real world test case, or with a simulation. Should provide relatively smooth experience with 
standard packet loss and latency percentages. 
