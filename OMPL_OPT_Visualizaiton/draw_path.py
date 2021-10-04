import numpy as np
import matplotlib.pyplot as plt


def read_coordinates(filename):
    coordinates = []
    f = open(filename,"r")
    lines = f.readlines()
    for line in lines:
        line = line.split()
        fl_array = [float(i) for i in line]
        coordinates.append(tuple(fl_array))
    coordinates.pop(-1)
    coordinates = np.vstack(coordinates)
    x = coordinates[:,0]
    y = coordinates[:,1]
    return list(x),list(y)


x,y = read_coordinates("out.txt")
figure,axis = plt.subplots()
plt.plot(x,y)
circle1 = plt.Circle((0.5, 0.5), 0.25, color='r')
axis.set_aspect(1)
plt.gca().add_patch(circle1)
plt.show()