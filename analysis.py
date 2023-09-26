import sys
import copy
import math
import numpy as np
import symnmf
from sklearn.metrics import silhouette_score, pairwise_distances

#User Args
k = int(sys.argv[1])
file_name = sys.argv[2]

#define variables for KMenas
EPSILON = 0.0001
ITER = 300
centroids = []
vectors = []
counter = 0
oldcentroids = []

def read_data(file_name):
    with open(file_name, 'r') as file:
        lines = file.readlines()
    data = []
    for line in lines:
        data.append([float(val) for val in line.strip().split(',')])
    return np.array(data)

vectors = read_data(file_name)

#Functions for k-means area
def delta(vector1, vector2):
    output = 0
    for i in range(0, len(vector1)):
        output += (vector1[i]-vector2[i])**2
    return math.sqrt(output)


# checks if all the deltas are smaller then epsilon
def convergence(centroids, oldcentroids):
    if len(oldcentroids) == 0:
        return False
    for i in range(0, len(centroids)):
        if delta(centroids[i][0], oldcentroids[i][0]) >= EPSILON:
            return False
    return True

# puts the centroid in the dictionary and also puts the vector in the centroid list
def putvectorinmatchingcentroid(vector):
    closestcentroid = min(centroids, key=lambda x: delta(vector, x[0]))
    closestcentroid[1].append(vector)
    # cenroid_vector_dictionary[tuple(vector)]=closestcentroid

def update_centroid(centroid):
    updated_centroid = [0] * len(centroid[0])
    for i in range(len(centroid[0])):
        total = 0
        for vector in centroid[1]:
            total += vector[i]
        if total != 0:
            updated_centroid[i] = total / len(centroid[1])
    centroid[0] = updated_centroid

#End functions for k-means area

# K-means Algorithm
for i in range(0, k):
    centroids.append([vectors[i], []])
while not convergence(centroids, oldcentroids) and counter < ITER:
    for vector in vectors:
        putvectorinmatchingcentroid(vector)
    oldcentroids = copy.deepcopy(centroids)
    for centroid in centroids:
        update_centroid(centroid)
    counter += 1
    for centroid in centroids:
        centroid[1] = []

cluster_labels_kmeans = []
for vector in vectors:
    distances = [pairwise_distances(np.array(vector).reshape(1, -1), np.array(centroid[0]).reshape(1, -1))[0][0] for centroid in centroids]
    cluster_labels_kmeans.append(distances.index(min(distances)))

# Calculate silhouette score for K-means
silhouette_kmeans = silhouette_score(vectors, cluster_labels_kmeans)

# Call the symnmf function and capture the result
vectors = vectors.tolist()
H = symnmf.symnmf(k, vectors)

# Derive cluster assignments for each data point based on the maximum association score
cluster_labels_nmf = np.argmax(H, axis=1)

# Calculate silhouette score for nmf
silhouette_nmf = silhouette_score(vectors, cluster_labels_nmf)
# Print the scores
print("nmf: {:.4f}".format(silhouette_nmf))
print("kmeans: {:.4f}".format(silhouette_kmeans))

