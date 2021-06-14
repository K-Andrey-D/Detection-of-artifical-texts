import numpy as np
from sklearn_extra.cluster import KMedoids
from sklearn.svm import OneClassSVM

def MyFunc(t_end):
    print (t_end)
    kmedoids = KMedoids (n_clusters = 2, random_state = 0).fit(t_end)
    result = kmedoids.labels_
    res = np.array([2,3,1])
    print(result)
    return result
 
def SVM(X):
    clf = OneClassSVM(kernel = 'rbf', gamma=0.01, nu=0.4).fit(X)
    print(clf.predict(X))
    
    