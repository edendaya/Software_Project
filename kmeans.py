import copy
import sys
import math

# START FUNCTIONS AREA OF CODE


def is_natural_number(value):
    return value.isdigit() and int(value) > 0


def read_file_to_array(file_path):
    data = []
    with open(file_path, 'r') as file:
        for line in file:
            vector = [float(num) for num in line.strip().split(',')]
            data.append(vector)
    return data


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
        if delta(centroids[i][0], oldcentroids[i][0]) >= epsilon:
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

    # END FUNCTIONS AREA OF CODE


# DEFINING INPUT VARIABLES AREA OF CODE
# initiating parameters
K = None
iter = None
file_path = None

# Check if the user provided 'iter', otherwise set the default value to 200
if len(sys.argv) >= 4:
    K = sys.argv[1]
    iter = sys.argv[2]
    file_path = str(sys.argv[3])
else:
    iter = "200"
    K = sys.argv[1]
    file_path = str(sys.argv[2])
centroids = []
vectors = []
counter = 0
# cenroid_vector_dictionary={}
oldcentroids = []
epsilon = 0.001

vectors = read_file_to_array(file_path)  # array of all our vectors

# CHECKING FOR INPUT ERRORS
if is_natural_number(K) and int(K) > 1 and int(K) < len(vectors):
    pass
else:
    print("Invalid number of clusters!")
    sys.exit(1)
if is_natural_number(iter) and int(iter) > 1 and int(iter) < 1000:
    pass
else:
    print("Invalid maximum iteration!")
    sys.exit(1)
K = int(K)
iter = int(iter)

# ALGORITHM STARTS HERE
for i in range(0, K):
    centroids.append([vectors[i], []])
while not convergence(centroids, oldcentroids) and counter < iter:
    for vector in vectors:
        putvectorinmatchingcentroid(vector)
    oldcentroids = copy.deepcopy(centroids)
    for centroid in centroids:
        update_centroid(centroid)
    counter += 1
    for centroid in centroids:
        centroid[1] = []

print("\n".join([",".join(map(lambda value: "{:.4f}".format(
    value), centroid[0])) for centroid in centroids]))
print("")
