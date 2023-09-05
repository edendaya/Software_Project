import matplotlib.pyplot as plt
from sklearn.datasets import load_iris
from sklearn.cluster import KMeans

# Load the iris dataset
iris = load_iris()
X = iris.data

# Initialize empty lists to store the values of k and inertia
k_values = []
inertia_values = []

# Run k-means clustering for different values of k
for k in range(1, 11):
    kmeans = KMeans(n_clusters=k, init='k-means++', random_state=0)
    kmeans.fit(X)
    inertia = kmeans.inertia_

    # Append k and inertia values to the lists
    k_values.append(k)
    inertia_values.append(inertia)

# Plotting the inertia values
plt.plot(k_values, inertia_values, marker='o')
plt.xlabel('Number of clusters (k)')
plt.ylabel('Inertia')
plt.title('Elbow Method')
plt.savefig('elbow.png')  # Save the plot as elbow.png
plt.show()
