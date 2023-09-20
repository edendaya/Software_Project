import sys
import numpy as np
import symnmfC 

# Set the random seed for consistent initialization
np.random.seed(0)

# Functions Area of Code

# Define a function to read the input file


def read_file_to_array(file_path):
    data = []
    with open(file_path, 'r') as file:
        for line in file:
            vector = [float(num) for num in line.strip().split(',')]
            data.append(vector)
    return data


# Define functions for various goals
def symnmf(k, vectors):
    # Calculate the graph Laplacian W
    n = len(vectors)
    m = len(vectors[0])
    A = sym(vectors,n, m)
    D = ddg(A, n)
    W = norm(A, n)
    # Calculate the average of all entries of W
    meanW = np.mean(W)

    # Initialize H as described in 1.4.1
    H_np = np.random.uniform(0, 2 * np.sqrt(meanW / k), (len(vectors), k))
    H = H_np.tolist()

    # Call the symnmf() method from the C extension module
    
    final_H=symnmfC.symnmff(H, W, len(W),k)

    
    # Output the final H matrix
    print("symnmf output")
    for row in final_H:
        formatted_row = [f"{element:.4f}" for element in row]
        print(",".join(formatted_row))
    


def sym(vectors,n, m):
    # Call the sym() method from the C extension module
    similarity_matrix = symnmfC.sym(vectors,n, m)
    print("sym output")
    for row in similarity_matrix:
        formatted_row = [f"{element:.4f}" for element in row]
        print(",".join(formatted_row))

    return similarity_matrix




def ddg(A, n):
    # Call the ddg() method from the C extension module

    diagonal_degree_matrix = symnmfC.ddg(A, n)
    print("ddg output")
    # Output the diagonal degree matrix
    for row in diagonal_degree_matrix:
        formatted_row = [f"{element:.4f}" for element in row]
        print(",".join(formatted_row))
    return diagonal_degree_matrix


def norm(A, n):
    # Call the norm() method from the C extension module
    normalized_similarity_matrix = symnmfC.norm(A, n)
    print("norm output")
    # Output the diagonal degree matrix
    for row in normalized_similarity_matrix:
        formatted_row = [f"{element:.4f}" for element in row]
        print(",".join(formatted_row))
    return normalized_similarity_matrix



# Arguments Area of Code
if len(sys.argv) != 4:
    print("An Error Has Occurred")
    sys.exit(1)
k = int(sys.argv[1])
goal = sys.argv[2]
file_name = sys.argv[3]

vectors = read_file_to_array(file_name)  # array of all our vectors
n = len(vectors)
m = len(vectors[0])

if goal == "symnmf":
    symnmf(k, vectors)
elif goal == "sym":
    sym(vectors,n,m)
elif goal == "ddg":
    A = sym(vectors,n,m)
    ddg(A,n)
elif goal == "norm":
    A = sym(vectors,n,m)
    norm(A,n)
else:
    print("An Error Has Occurred")
    
    sys.exit(1)
