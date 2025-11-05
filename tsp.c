#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <string.h>

typedef struct
{
  float x;
  float y;
} node;

time_t start;

int tmp[1010];

int computeDist(node node1, node node2)
{
  float dist = sqrt(pow(node1.x - node2.x, 2) + pow(node1.y - node2.y, 2));
  return round(dist);
}

// Compute the solution value (tour length)
int computeSolutionValue(int *path, int **distMatrix, int n)
{
  int value = 0;
  for (int i = 0; i < n - 1; i++)
  {
    value += distMatrix[path[i]][path[i + 1]];
  }
  value += distMatrix[path[n - 1]][path[0]];
  return value;
}

// Swap two elements in the path
void swap(int *path, int i, int j)
{
  int temp = path[i];
  path[i] = path[j];
  path[j] = temp;
}

int **computeDistanceMatrix(node *nodes, int n)
{
  int **distMatrix = (int **)malloc(n * sizeof(int *));
  if (!distMatrix)
  {
    return NULL;
  }

  for (int i = 0; i < n; i++)
  {
    for (int j = i; j < n; j++)
    {
      if (i == 0)
      {
        distMatrix[j] = (int *)malloc(n * sizeof(int));
      }

      if (i == j)
      {
        distMatrix[i][j] = distMatrix[j][i] = 0;
      }
      else
      {
        distMatrix[i][j] = distMatrix[j][i] = computeDist(nodes[i], nodes[j]);
      }
    }
  }
  return distMatrix;
}

// Perform 2-opt move (simplified)
void twoOpt(int *path, int **distMatrix, int n)
{
  int improved = 1;
  while (improved)
  {
    improved = 0;
    for (int i = 1; i < n - 1; i++)
    {
      for (int j = i + 1; j < n; j++)
      {
        int distBefore = distMatrix[path[i - 1]][path[i]] + distMatrix[path[j]][path[(j + 1) % n]];
        int distAfter = distMatrix[path[i - 1]][path[j]] + distMatrix[path[i]][path[(j + 1) % n]];
        if (distAfter < distBefore)
        {
          // Perform the 2-opt swap
          for (int k = 0; k < (j - i + 1) / 2; k++)
          {
            int temp = path[i + k];
            path[i + k] = path[j - k];
            path[j - k] = temp;
          }
          improved = 1;
        }
      }
    }
  }
}

void threeOpt(int *path, int **distMatrix, int n)
{
  int improved = 1;
  int maxIter = 200000000;

  while (improved && maxIter > 0)
  {
    improved = 0;

    // Iterate over all possible segments of the path
    for (int i = 0; i < n - 2; i++)
    {
      for (int j = i + 1; j < n - 1; j++)
      {
        for (int k = j + 1; k < n; k++)
        {
          // Calculate the original distance (three edges)
          int distBefore = distMatrix[path[i]][path[i + 1]] +
                           distMatrix[path[j]][path[j + 1]] +
                           distMatrix[path[k]][path[(k + 1) % n]];

          // Define possible reconnections
          int distOptions[7];
          int bestOption = 0;

          // Option 1: Keep the current connections
          distOptions[0] = distBefore;

          // Option 2: Reverse the segment (i+1 to j)
          // distOptions[1] = distMatrix[path[i]][path[j]] +
          //                  distMatrix[path[i + 1]][path[k + 1]] +
          //                  distMatrix[path[k]][path[j + 1] +
          // distOptions[1] = distMatrix[path[i]][path[j]] +
          //                  distMatrix[path[i + 1]][path[j + 1]] +
          //                  distMatrix[path[k]][path[k + 1]];

          // // // Option 3: Reverse the segment (j+1 to k)
          // distOptions[2] = distMatrix[path[i]][path[i + 1]] +
          //                  distMatrix[path[j]][path[k]] +
          //                  distMatrix[path[j + 1]][path[k + 1]];

          // // // Option 4: Reverse both segments (i+1 to j) and (j+1 to k)
          // distOptions[3] = distMatrix[path[i]][path[j]] +
          //                  distMatrix[path[k]][path[i + 1]] +
          //                  distMatrix[path[j + 1]][path[k + 1]];

          // // Option 5: Alternate reconnection
          distOptions[4] = distMatrix[path[i]][path[j + 1]] +
                           distMatrix[path[j]][path[k + 1]] +
                           distMatrix[path[i + 1]][path[k]];

          // // Option 6: Another reconnection pattern
          distOptions[5] = distMatrix[path[i]][path[j + 1]] +
                           distMatrix[path[k + 1]][path[i + 1]] +
                           distMatrix[path[j]][path[k]];

          // // Option 7: Reverse entire segment from i+1 to k
          distOptions[6] = distMatrix[path[i]][path[k]] +
                           distMatrix[path[k + 1]][path[j]] +
                           distMatrix[path[i + 1]][path[j + 1]];

          // int debugging = ((rand() % 2000) == 0);

          // Find the best reconnection
          // for (int opt = 1; opt <= 6; opt++)
          for (int opt = 4; opt <= 6; opt++)
          {
            // if (opt > 4)
            // continue;
            if (distOptions[opt] < distOptions[bestOption])
            {
              // if (debugging)
              // {
              //   fprintf(stderr, "Upgrading from %d to %d\n", distOptions[bestOption], distOptions[opt]);
              //   fprintf(stderr, "Previous value: %d\n", computeSolutionValue(path, distMatrix, n));
              // }
              bestOption = opt;
            }
          }

          // If an improvement is found, apply it
          if (bestOption > 0)
          {
            switch (bestOption)
            {
            case 1:
              // Reverse (i+1 to j)
              for (int a = 0, b = j; a < b - i; a++, b--)
              {
                int temp = path[i + 1 + a];
                path[i + 1 + a] = path[b];
                path[b] = temp;
              }
              break;
            case 2:
              // Reverse (j+1 to k)
              for (int a = 0, b = k; a < b - j; a++, b--)
              {
                int temp = path[j + 1 + a];
                path[j + 1 + a] = path[b];
                path[b] = temp;
              }
              break;
            case 3:
              // Reverse both (i+1 to j) and (j+1 to k)
              for (int a = 0, b = j; a < b - i; a++, b--)
              {
                int temp = path[i + 1 + a];
                path[i + 1 + a] = path[b];
                path[b] = temp;
              }
              for (int a = 0, b = k; a < b - j; a++, b--)
              {
                int temp = path[j + 1 + a];
                path[j + 1 + a] = path[b];
                path[b] = temp;
              }
              break;
            case 4:
              //   // Alternate reconnection (custom swap pattern)
              // for (int a = 0; a < j - i; a++)
              // {
              //   int temp = path[i + 1 + a];
              //   path[i + 1 + a] = path[j + 1 + a];
              //   path[j + 1 + a] = temp;
              // }
              // Copying path to tmp
              for (int a = 0; a < n; a++)
              {
                tmp[a] = path[a];
              }
              // Copying tmp[j+1] to tmp[k] to path[i+1] to path[i+k-j]
              for (int a = 0; a < k - j; a++)
              {
                path[i + 1 + a] = tmp[j + 1 + a];
              }
              // Copying tmp[i+1] to tmp[j] to path[i+k-j+1] to path[k]
              for (int a = 0; a < j - i; a++)
              {
                path[i + k - j + 1 + a] = tmp[i + 1 + a];
              }
              break;
            case 5:
              // Reverse (i+1 to j)
              for (int a = 0, b = j; a < b - i; a++, b--)
              {
                int temp = path[i + 1 + a];
                path[i + 1 + a] = path[b];
                path[b] = temp;
              }
              for (int a = 0; a < n; a++)
              {
                tmp[a] = path[a];
              }
              // Copying tmp[j+1] to tmp[k] to path[i+1] to path[i+k-j]
              for (int a = 0; a < k - j; a++)
              {
                path[i + 1 + a] = tmp[j + 1 + a];
              }
              // Copying tmp[i+1] to tmp[j] to path[i+k-j+1] to path[k]
              for (int a = 0; a < j - i; a++)
              {
                path[i + k - j + 1 + a] = tmp[i + 1 + a];
              }
              break;
            case 6:
              //   // Reverse entire segment from i+1 to k
              //   for (int a = 0, b = k; a < b - i; a++, b--)
              //   {
              //     int temp = path[i + 1 + a];
              //     path[i + 1 + a] = path[b];
              //     path[b] = temp;
              //   }
              // Reverse (j+1 to k)
              for (int a = 0, b = k; a < b - j; a++, b--)
              {
                int temp = path[j + 1 + a];
                path[j + 1 + a] = path[b];
                path[b] = temp;
              }
              for (int a = 0; a < n; a++)
              {
                tmp[a] = path[a];
              }
              // Copying tmp[j+1] to tmp[k] to path[i+1] to path[i+k-j]
              for (int a = 0; a < k - j; a++)
              {
                path[i + 1 + a] = tmp[j + 1 + a];
              }
              // Copying tmp[i+1] to tmp[j] to path[i+k-j+1] to path[k]
              for (int a = 0; a < j - i; a++)
              {
                path[i + k - j + 1 + a] = tmp[i + 1 + a];
              }
              break;
            }

            // if (debugging)
            // {
            //   fprintf(stderr, "New value: %d\n", computeSolutionValue(path, distMatrix, n));
            // }
            improved = 1;
            break; // Exit inner loops to reevaluate the new path
          }
          maxIter--;
        }

        maxIter--;
        // if (improved)
        //   break;
        if (maxIter <= 0)
        {
          break;
        }
      }
      maxIter--;
      if ((clock() - start) / (double)CLOCKS_PER_SEC > 1.95)
      {
        return;
      }
      // if (improved)
      //   break;
      if (maxIter <= 0)
      {
        break;
      }
    }
    maxIter--;
  }
}

// Randomized Nearest Neighbor heuristic
int **randomizedNearestNeighbor(node *nodes, int n, int iterations, int **distMatrix)
{
  int **paths = (int **)malloc(iterations * sizeof(int *));
  if (!paths)
  {
    return NULL;
  }

  for (int iter = 0; iter < iterations; iter++)
  {
    int *visited = (int *)calloc(n, sizeof(int));
    int *path = (int *)malloc((n + 1) * sizeof(int));
    if (!visited || !path)
    {
      free(visited);
      free(path);
      paths[iter] = NULL;
      continue;
    }

    // Start from a random node
    int start = rand() % n;
    int current = start;
    path[0] = current;
    visited[current] = 1;

    // Construct path
    for (int i = 1; i < n; i++)
    {
      int nearest = -1, minDist = INT_MAX;
      for (int j = 0; j < n; j++)
      {
        if (!visited[j] && (distMatrix[current][j] < minDist))
        {
          minDist = distMatrix[current][j];
          nearest = j;
        }
      }
      current = nearest;
      path[i] = current;
      visited[current] = 1;
    }

    path[n] = path[0]; // Return to start
    paths[iter] = path;
    free(visited);
  }

  return paths;
}

// Solve the TSP with a combination of heuristics
int *computeTSP(node *nodes, int n, int **distMatrix)
{
  int iterations = 110;
  int **paths = randomizedNearestNeighbor(nodes, n, iterations, distMatrix);
  if (!paths)
  {
    for (int i = 0; i < n; i++)
    {
      free(distMatrix[i]);
    }
    free(distMatrix);
    return NULL;
  }
  int *bestPath = NULL;
  int minVal = INT_MAX;
  for (int i = 0; i < iterations; i++)
  {
    if (!paths[i])
    {
      continue;
    }
    twoOpt(paths[i], distMatrix, n);
    int val = computeSolutionValue(paths[i], distMatrix, n);
    if (val < minVal)
    {
      minVal = val;
      // fprintf(stderr, "Improvement in iteration %d: from %d to %d\n", i, minVal, val);
      if (bestPath)
      {
        free(bestPath);
      }
      bestPath = paths[i];
    }
    else
    {
      free(paths[i]);
    }
  }
  //fprintf(stderr, "\n");
  free(paths);
  int val = computeSolutionValue(bestPath, distMatrix, n);
  //fprintf(stderr, "%d\t", val);
  threeOpt(bestPath, distMatrix, n);
  val = computeSolutionValue(bestPath, distMatrix, n);
  //fprintf(stderr, ".%d.\t", val);
  return bestPath;
}

int main(int argc, char **argv)
{
  start = clock();
  int n; // Number of nodes

  if (scanf("%d", &n) != 1 || n < 1 || n > 1000)
  {
    return 1;
  }

  node *nodes = (node *)malloc(n * sizeof(node));
  if (nodes == NULL)
  {
    return 1;
  }

  int nodeCount = 0;
  for (int i = 0; i < n; i++)
  {
    if (scanf("%f %f", &nodes[i].x, &nodes[i].y) != 2)
    {
      free(nodes);
      return 1;
    }
    nodeCount++;
  }

  if (nodeCount != n)
  {
    free(nodes);
    return 1;
  }

  int **distMatrix = computeDistanceMatrix(nodes, n);
  if (!distMatrix)
  {
    return 1;
  }

  int *solution = computeTSP(nodes, n, distMatrix);
  if (!solution)
  {
    free(nodes);
    return 1;
  }

  // Print the result
  for (int i = 0; i < n; i++)
  {
    printf("%d\n", solution[i]);
  }

  printf("Value: %d\n", computeSolutionValue(solution, distMatrix, n));

  // Free allocated memory
  free(nodes);
  free(solution);
  for (int i = 0; i < n; i++)
  {
    free(distMatrix[i]);
  }
  free(distMatrix);

  return 0;
}
