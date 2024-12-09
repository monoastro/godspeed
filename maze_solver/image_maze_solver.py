import cv2
import numpy as np
from collections import deque

# Load the image
image = cv2.imread('maze.png', cv2.IMREAD_GRAYSCALE)

# Threshold the image to distinguish walls and paths
_, binary_maze = cv2.threshold(image, 128, 255, cv2.THRESH_BINARY)

# Convert to a binary matrix (0: path, 1: wall)
maze = (binary_maze == 0).astype(int)

# Define start and end points (row, col)
start = (8, 14)  # Example coordinates, adjust as needed
end = (5, 2)     # Example coordinates, adjust as needed

# BFS to find the path
def bfs(maze, start, end):
    rows, cols = maze.shape
    queue = deque([start])
    visited = set()
    parent = {start: None}
    
    while queue:
        current = queue.popleft()
        
        if current == end:
            break
        
        for dr, dc in [(-1, 0), (1, 0), (0, -1), (0, 1)]:  # Up, down, left, right
            r, c = current[0] + dr, current[1] + dc
            if 0 <= r < rows and 0 <= c < cols and (r, c) not in visited and maze[r, c] == 0:
                queue.append((r, c))
                visited.add((r, c))
                parent[(r, c)] = current
    
    # Backtrack to find the path
    path = []
    step = end
    while step is not None:
        path.append(step)
        step = parent.get(step)
    path.reverse()
    return path

# Get the path from the BFS function
path = bfs(maze, start, end)

# Visualize the path on the image
for r, c in path:
    image[r, c] = 128  # Mark path in a gray color

cv2.imshow('Solved Maze', image)
cv2.waitKey(0)
cv2.destroyAllWindows()

