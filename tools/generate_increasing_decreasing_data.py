from __future__ import division
import matplotlib.pyplot as plt
import numpy as np
import sys

data_directory = sys.argv[1]
plot_directory = sys.argv[2] 

num_instances  = 100
noise          = 0.3

h           = 10
vector_size = 100


def generateStraightLineCoefficients():
    a = abs(np.random.normal(0, 0.1, 1)[0])
    a = a * 64 * h / vector_size
    eps = np.random.normal(0, 1, 1)[0] * noise
    b = h - a * vector_size/2 * (1 + eps)
    return (a,b)

class Increase:

    def __init__(self):
        (self.a, self.b) = generateStraightLineCoefficients()

    def value(self, t):
        eps = np.random.normal(0, 1, 1)[0] * noise
        return (self.a*t + self.b) * (1 + eps)

class Decrease:

    def __init__(self):
        (self.a, self.b) = generateStraightLineCoefficients()

    def value(self, t):
        eps = np.random.normal(0, 1, 1)[0] * noise
        return -(self.a*t + self.b) * (1 + eps)

class Flat:

    def __init__(self):
        eps = np.random.normal(0, 1, 1)[0] * noise
        self.b = h * (1 + eps)

    def value(self, t):
        eps = np.random.normal(0, 1, 1)[0] * noise
        return self.b * (1 + eps)


def toCSV(array):
    return ",".join(map(lambda x: str(x), array))

def generate_data(data_generator, label, label_0_1, num_instances, 
        noise, color, f):
    x = range(vector_size)
    for j in range(num_instances):
        generator = data_generator()
        y = range(vector_size)
        for i in range(vector_size):
            y[i] = generator.value(x[i])
        plt.plot(x, y, color = color)
        filename = label + '-' + str(j) + '-' + str(noise)
        full_filename = plot_directory + filename + ".pdf"
        print >>f, filename + "," + str(label_0_1) + "," + toCSV(y)
        plt.savefig(full_filename)
        plt.clf()

f = open(data_directory + 'increase-decrease.txt', 'w')
print >>f, str(vector_size)
generate_data(Increase, "increase", 0, num_instances, noise, "green", f)
generate_data(Decrease, "decrease", 1, num_instances, noise, "red", f)
f.close()
