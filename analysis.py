import sys
import numpy as np
from sklearn.metrics import silhouette_score
import symnmf  # Import the symnmf module
import kmeans  # Import your kmeans module (replace with actual module name)

def read_data(file_name):
    with open(file_name, 'r') as file:
        lines = file.readlines()
    data = []
    for line in lines:
        data.append([float(val) for val in line.strip().split(',')])
    return np.array(data)

def main():
    if len(sys.argv) != 3:
        print("An Error Has Occurred")
        sys.exit(1)

    k = int(sys.argv[1])
    file_name = sys.argv[2]

    data = read_data(file_name)

    # Call the symnmf function and capture the result
    symnmf_result = symnmf.symnmf(k, data, True)

    # Call the kmeans function and capture the result
    kmeans_result = kmeans.kmeans(k, data, True)  # Replace with actual function name

    # Calculate silhouette scores
    symnmf_score = silhouette_score(data, np.argmax(symnmf_result, axis=1))
    kmeans_score = silhouette_score(data, np.argmax(kmeans_result, axis=1))

    print(f"nmf: {symnmf_score:.4f}")
    print(f"kmeans: {kmeans_score:.4f}")

if __name__ == "__main__":
    main()
