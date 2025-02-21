# Urban Traffic Simulation in Java  

## Project Description  
This project implements an object-oriented simulation of urban traffic, focusing on tram lines and passenger interactions with the transportation system. The program simulates daily passenger activity, considering schedules, vehicle and stop capacity limits, and passengers’ random travel decisions.  

## Main Simulation Assumptions  
- **Objects in the simulation**: passengers, trams, tram lines, and stops.  
- **Tram movement**:  
  - Each tram has an assigned line and fleet number.  
  - Trams follow predefined routes, stopping at various stations.  
  - After reaching the end of the route, they return via the same path.  
  - Services start at 6:00 AM and finish after 11:00 PM.  
- **Passengers**:  
  - Each passenger lives near a specific stop.  
  - They depart for travel at a random time between 6:00 AM and 12:00 PM.  
  - If a stop is overcrowded, they cancel their trip for the day.  
  - They choose any available tram line and travel to a random stop along the route.  
  - If the tram is full, they wait for the next one.  
- **Event queue**:  
  - The simulation operates based on a sorted list of events scheduled in chronological order.  
  - Each object in the simulation generates events added to the queue.  

## Program Output  
The program generates three types of output:  
1. **Loaded input parameters**.  
2. **Detailed event log of the simulation** (e.g., when passengers board and leave trams).  
3. **Statistics**:  
   - Total number of passenger trips.  
   - Average waiting time at stops.  
   - Daily summary of total trips and cumulative waiting time.  

## Implementation  
The program is written in **Java** following **object-oriented programming** principles. Key components include:  
- **Object models**: classes representing trams, passengers, stops, etc.  
- **Event queue**: a custom data structure for managing scheduled events.  
- **Randomization**: a `Losowanie` class for generating random values.  
- **Input/output handling**: reading input data and logging simulation results.  

## How to Run  
1. Copy the source code into a Java project.  
2. Use `java.util.Scanner` to read input data.  
3. Run the program to generate a detailed tram movement simulation.  
