#include<iostream>
#include <array>
#include <chrono>
#include <functional>
#include <string>
#include <bits/stdc++.h>
using namespace std;
string timeCalulator(function<void()> traversal_fn)
{
auto start = chrono::high_resolution_clock::now();
traversal_fn();
auto stop = chrono::high_resolution_clock::now();
auto result = stop - start;
auto duration = chrono::duration_cast<chrono::microseconds>(result);
return to_string(duration.count());
}
// Direction vectors
int dRow[] = { -1, 0, 1, 0 };
int dCol[] = { 0, 1, 0, -1 };
// Function to check if a cell
// is be visited or not
bool isValid(bool vis[][2],int row, int col)
{
 // If cell lies out of bounds
 if (row < 0 || col < 0 || col >= 2)
 return false;
 // If cell is already visited
 if (vis[row][col])
 return false;
 // Otherwise
 return true;
}
// Function to perform sequential BFS traversal
void s_BFS(int grid[][2], bool vis[][2],int row, int col)
{
 // Stores indices of the matrix cells
 queue<pair<int, int> > q;
 // Mark the starting cell as visited
 // and push it into the queue
 q.push({ row, col });
 vis[row][col] = true;
 // Iterate while the queue
 // is not empty
 while (!q.empty()) {
 pair<int, int> cell = q.front();
 int x = cell.first;
 int y = cell.second;
 q.pop();
 // Go to the adjacent cells
 for (int i = 0; i < 4; i++) {
 int adjx = x + dRow[i];
 int adjy = y + dCol[i];
 if (isValid(vis, adjx, adjy)) {
 q.push({ adjx, adjy });
 vis[adjx][adjy] = true;
 }
 }
 }
}
// Function to perform parallel BFS traversal
void p_BFS(int grid[][2], bool vis[][2],int row, int col)
{
 // Stores indices of the matrix cells
 queue<pair<int, int> > q;
 // Mark the starting cell as visited
 // and push it into the queue
 q.push({ row, col });
 vis[row][col] = true;
 // Iterate while the queue
 // is not empty
 #pragma omp parallel num_threads(2)
 while (!q.empty()) {
 pair<int, int> cell = q.front();
 int x = cell.first;
 int y = cell.second;
 //cout << grid[x][y] << " ";
 q.pop();
 // Go to the adjacent cells
 #pragma omp parallel for
 for (int i = 0; i < 4; i++) {
 int adjx = x + dRow[i];
 int adjy = y + dCol[i];
 if (isValid(vis, adjx, adjy)) {
 q.push({ adjx, adjy });
 vis[adjx][adjy] = true;
 }
 }
 }
}
//Function for generating a graph and calling the traversal function
void Graphs(int NOEdge, int NOVertex)
{
int i, j, edge[NOEdge][2], count;
i = 0;

 // Assign random values to the number of vertex and edges of the graph,Using rand().
while (i < NOEdge) {
edge[i][0] = rand() % NOVertex + 1;
edge[i][1] = rand() % NOVertex + 1;

 // Print the connections of each vertex, irrespective of the direction.
if (edge[i][0] == edge[i][1])
continue;
else {
for (j = 0; j < i; j++) {
if ((edge[i][0] == edge[j][0] && edge[i][1] == edge[j][1]) ||
(edge[i][0] == edge[j][1]
&& edge[i][1] == edge[j][0]))
 i--;
 }
 }
 i++;
 }

 // Declare the visited array
 bool vis[NOEdge][2];
 memset(vis, false, sizeof vis);

 //calling the function for sequential traversal
 string time1 = timeCalulator([&]{ s_BFS(edge, vis, NOEdge, 2); });
 cout << "\nSequential BFS Traveral " << time1 << " microseconds\n";

 //calling the function for parallel traversal
 string time2 = timeCalulator([&]{ p_BFS(edge, vis, NOEdge, 2); });
 cout << "\nParallel BFS Traversal " << time2 << " microseconds\n";

}

int main()
{
 int i, e, n;

 cout << "Enter the no of vertices" << endl;
 cin >> n;

 cout << "The graph has " << n << " vertices" << endl;

 e = rand() % ((n * (n - 1)) / 4);
 cout << "and has " << e << " edges." << endl;

 // Function call
 Graphs(e, n);

 return 0;
}
