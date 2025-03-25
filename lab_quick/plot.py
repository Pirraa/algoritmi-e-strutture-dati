import matplotlib.pyplot as plt

with open('report.txt') as f:
    lines = f.readlines()

x_data = list(map(int,lines[0].split()))
y_data = [list(map(float,line.split())) for line in lines[1:]]

algos=["INSERTION", "MERGE", "HYBRIDMERGE","QUICK"]

for i,time in enumerate(y_data):
    plt.plot(x_data,time,label=algos[i])

plt.xlabel('Input Size')
plt.ylabel('Time')
plt.title('Time Complexity')
plt.legend()
plt.show()
