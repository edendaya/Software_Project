import pandas as pd
import numpy as np
import copy
import sys
import math
import mykmeanssp


# import mykmeanssp

# START FUNCTIONS AREA OF CODE
# def read_file_to_array(file_path):
#     data = []
#     with open(file_path, 'r') as file:
#         for line in file:
#             vector = [float(num) for num in line.strip().split(',')]
#             data.append(vector)
#     return data

def kmeans_plusplus(vectors_np, K):
    np.random.seed(0)
    centers = []
    n = vectors_np.shape[0]  # num of vectors
    random_index = np.random.choice(n)
    centers.append(vectors_np[random_index])  # add first center

    # Add the rest of the centers acoording to Algorithm
    for _ in range(1, K):
        distances = np.array([min(np.linalg.norm(vector - center)
                             for center in centers) for vector in vectors_np])
        probabilities = distances / np.sum(distances)
        random_index = np.random.choice(n, p=probabilities)
        centers.append(vectors_np[random_index])

    return centers


# DEFINING INPUT VARIABLES AREA OF CODE
# initiating parameters
K = None
iter = None
file_path = None

# Check if the user provided 'iter', otherwise set the default value to 200
if len(sys.argv) >= 6:
    K = sys.argv[1]
    iter = sys.argv[2]
    epsilon = sys.argv[3]
    file_path_1 = str(sys.argv[4])
    file_path_2 = str(sys.argv[5])
else:
    iter = "300"
    K = sys.argv[1]
    epsilon = sys.argv[2]
    file_path_1 = str(sys.argv[3])
    file_path_2 = str(sys.argv[4])
centroids = []
final_centroids = []
vectors = []

# INNERJOIN AREA OF CODE
df1 = pd.read_csv(file_path_1, header=None)
df1.columns = ['index'] + list(df1.columns[1:])
df2 = pd.read_csv(file_path_2, header=None)
df2.columns = ['index'] + list(df2.columns[1:])

# Convert the 'index' column to integer type
df1['index'] = df1['index'].astype(int)
df2['index'] = df2['index'].astype(int)

# Perform an inner join on the first column
vectors_pre_sorted_pd = pd.merge(df1, df2, on='index', how='inner')

# Sort the data points by the 'key' column in ascending order
vectors_pd = vectors_pre_sorted_pd.sort_values(by='index', ascending=True)

# Convert the vectors from panda format to 2D array
vectors = vectors_pd.iloc[:, 1:].values

print(vectors)

# CHECKING FOR INPUT ERRORS
if 1 < int(K) < len(vectors):
    pass
else:
    print("Invalid number of clusters!")
    sys.exit(1)
if 1 < int(iter) < 1000:
    pass
else:
    print("Invalid maximum iteration!")
    sys.exit(1)
K = int(K)
iter = int(iter)

# Kmeans++ AREA OF CODE
vectors_np = np.array(vectors)
centroids = kmeans_plusplus(vectors_np, K)

#  Using C module mykmeanssp to get final centroids
# final_centroids = fit(centroids)

print("\n".join([",".join(map(lambda value: "{:.4f}".format(
    value), centroid[0])) for centroid in final_centroids]))
print("")
