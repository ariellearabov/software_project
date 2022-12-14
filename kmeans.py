'''Assignment 1 - 
In this assignment we're implementing the k-means algorithm in python and in C.
Provided below is the python implementation.
'''
# sys.args returns a list with the python file name and the args used 
import sys

def main():
    try:
        input_file_path, iter_arg = check_input()
    except(AssertionError):
        print("An Error Has Occurred")
        return 
    
    mat = read_file(input_file_path)
    n = len(mat)

    try:
        k = validation_k(n)
    except(AssertionError):
        print("Invalid number of clusters!")
        return 

    try:
        iter = validation_iter(iter_arg)
    except(AssertionError):
        print("Invalid maximum iteration!")
        return 
    
    try:
        clusters = calc_k_means(k, iter, mat)
    except:
        print("An Error Has Occurred")
    

    return 0

'''
Input- the path to the input text file.
Output- a float matrix where each row is a vector from the input file. 
'''

def read_file(file_path):
    f = open(file_path, "r")
    line = f.readline() # reading the first line 
    mat = []
    while line.strip() != "": # until we reach the empty line
        x = [float(elem) for elem in line.split(",")] # wrap observation in a list. List meant to hold observation and cluster.
        mat.append(x)
        line = f.readline()
    f.close()
    return mat

'''
This function checks if the input is valid, if everything is OK returns the files' path  
'''

def check_input():
    n = len(sys.argv)
    assert n in (3, 4) # checks validity of input
    i = 1
    iter_arg = False

    if n == 4: # iter included in input
        i += 1
        iter_arg = True
    
    input_file_path = sys.argv[i + 1] # If iter value is not a part of input, this will be 2nd arg 
    return input_file_path, iter_arg

'''
validation_x functions check whether the specific args are valid 
'''

def validation_k(N):  
    try:
        k = int(sys.argv[1])  # checks if k arg is an int 
        if not 1<k<N:  # checks if k in valid range 
            assert 1 == 0
    except:
        assert 1 == 0
    else:
        return k


def validation_iter(iter_arg): 
    try:
        if iter_arg:  # if iter arg exists 
            iter = int(sys.argv[2])  # checks if k arg is an int 
            if not 1<iter<1000:  # checks if iter in valid range 
                assert 2 == 1
    except:
        assert 2 == 1
    if not iter_arg:
        iter = 200  # no arg given, gives defualt arg as stated 
    return iter 

'''
This function calculates the K cluster centroids produced by the K-means algorithm.
Firstly, the function initializes the centroids to be the first k datapoints.
Then the function performs the following-
- Add each observation to its closest cluster and removing it from its old one. 
  Finding the closet cluster is done using the "find_closest" function.
- Calculat each cluster's new centroid (as the average of the cluster's updated observations).
- Calculat each cluster's *deviation* between its old centroid and its new one. 
  The deviation is calculated using the "square_euclidean_distance" function. 
The function stops when iter iterations were preformed, or when the deviation of all the clusters 
is less than epsilon squared (i.e., the distance itself is less than epsilon).
'''
def calc_k_means(k, iter, mat, epsilon=0.001):
    centroids = [mat[i] for i in range(k)] # Initialize centroids as first k datapoints
    converged = False 
    while (iter > 0) and (not converged):
        for x in mat: # checks each dataset 
            i = 0 # just so threr wont be errors. 

    return 0

'''
This function finds the index of the closest centroid to dataset x.
Distance is measured with euclidean distance. 
Assumptions - for each centroid dimension(x) == dimension(centorid).
The function uses the euclidean_distance function. 
'''
def find_closest_cluster(x, clusters):
    minimal_index = 0
    min_dist = euclidean_distance()  # need to continue this func 

'''
'''
def euclidean_distance():  # need to wrtie this func 
    return 0
    
if __name__ == "__main__":
    main() 