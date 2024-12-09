# Maze Solver using Depth-First Search (DFS)

# A simple representation of a maze
# 0 represents an open space, 1 represents a wall
maze = [
    [0, 1, 0, 0, 0, 0],
    [0, 1, 0, 1, 1, 0],
    [0, 1, 0, 1, 0, 0],
    [0, 0, 0, 1, 0, 1],
    [0, 1, 0, 0, 0, 0],
    [0, 0, 0, 1, 1, 0]
]

# Function to solve the maze using DFS
def solve_maze(maze, start, end):
    # Stack to keep track of the path
    stack = [start]
    
    # Directions: right, down, left, up
    directions = [(0, 1), (1, 0), (0, -1), (-1, 0)]
    
    # Set to keep track of visited nodes
    visited = set()
    visited.add(start)
    
    while stack:
        current = stack[-1]
        x, y = current
        
        # If we have reached the end, return the path
        if current == end:
            return stack
        
        # Explore the neighbors
        for dx, dy in directions:
            nx, ny = x + dx, y + dy
            if 0 <= nx < len(maze) and 0 <= ny < len(maze[0]) and maze[nx][ny] == 0 and (nx, ny) not in visited:
                visited.add((nx, ny))
                stack.append((nx, ny))
                break
        else:
            stack.pop()
    
    # If no path was found, return None
    return None

# Start and End points
start = (0, 0)  # Top-left corner
end = (5, 5)    # Bottom-right corner

# Solve the maze
path = solve_maze(maze, start, end)

if path:
    print("Path found:", path)
else:
    print("No path found.")
