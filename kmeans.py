'''Assignment 1 - 
In this assignment we're implementing the k-means algorithm in python and in C.
Provided below is the python implementation.
'''
# sys.args returns a list with the python file name and the args used 
import sys

def main():
    try:
        input_file_path = check_input()
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
        iter = validation_iter()
    except(AssertionError):
        print("Invalid maximum iteration!")
        return 
    

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
This function checks if the input is valid   
'''

def check_input():
    n = len(sys.argv)
    assert n in (3, 4) # checks validity of input
    i = 1

    if n == 4: # iter included in input
        i += 1
    
    input_file_path = sys.argv[i + 1] # If iter value is not a part of input, this will be 2nd arg
    return input_file_path

'''
validation_x functions check whether the specific args are valid 
'''

def validation_k(N):  # need to change this so it will work 
    try:
        k = int(sys.argv[1])
        if not 1<k<N:
            assert 1 == 0
    except:
        assert 1 == 0
    else:
        return k


def validation_iter():  # need to change this so it will work 
    try:
        iter = int(sys.argv[2])
        if not 1<iter<1000:
            assert 2 == 1
    except:
        assert 2 == 1
    else:
        return iter 

if __name__ == "__main__":
    main() 