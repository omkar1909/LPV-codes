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
auto duration = chrono::duration_cast<chrono::nanoseconds>(result);
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
// Function to perform sequential DFS traversal
void s_DFS(int graph[][2],bool vis[][2],int row, int col)
{
 // Initialize a stack of pairs and
 // push the starting cell into it
 stack<pair<int, int> > st;
 st.push({ row, col });
 // Iterate until the
 // stack is not empty
 #pragma omp parallel num_threads(2)
 while (!st.empty()) {
 // Pop the top pair
 pair<int, int> curr = st.top();
 st.pop();
 int row = curr.first;
 int col = curr.second;
 // Check if the current popped
 // cell is a valid cell or not
 if (!isValid(vis, row, col))
 continue;
 // Mark the current
 // cell as visited
 vis[row][col] = true;
 // Print the element at
 // the current top cell
 //cout << grid[row][col] << " ";
 // Push all the adjacent cells
 #pragma omp parallel for
 for (int i = 0; i < 4; i++) {
 int adjx = row + dRow[i];
 int adjy = col + dCol[i];
 st.push({ adjx, adjy });
 }
 }
}
// Function to perform parallel DFS traversal
void p_DFS(int graph[][2],bool vis[][2],int row, int col)
{
 // Initialize a stack of pairs and
 // push the starting cell into it
 stack<pair<int, int> > st;
 st.push({ row, col });
 // Iterate until the
 // stack is not empty
 while (!st.empty()) {
 // Pop the top pair
 pair<int, int> curr = st.top();
 st.pop();
 int row = curr.first;
 int col = curr.second;
 // Check if the current popped
 // cell is a valid cell or not
 if (!isValid(vis, row, col))
 continue;
 // Mark the current
 // cell as visited
 vis[row][col] = true;
 // Print the element at
 // the current top cell
 //cout << grid[row][col] << " ";
 // Push all the adjacent cells
 for (int i = 0; i < 4; i++) {
 int adjx = row + dRow[i];
 int adjy = col + dCol[i];
 st.push({ adjx, adjy });
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
 string time1 = timeCalulator([&]{ s_DFS(edge, vis, NOEdge, 2); });
 cout << "\nSequential BFS Traveral " << time1 << " nanoseconds\n";

 //calling the function for parallel traversal
 string time2 = timeCalulator([&]{ p_DFS(edge, vis, NOEdge, 2); });
 cout << "\nParallel BFS Traversal " << time2 << " nanoseconds\n";

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
