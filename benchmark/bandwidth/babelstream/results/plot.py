import numpy as np
import re
from matplotlib import pyplot as plt

with open ('babelstream.txt', 'r') as results:
    text = results.read()

# extract the sizes values
values = re.findall(r'Array size :: (\d+)*', text)
sizes = [float(value) if '.' in value else int(value) for value in values]

# extract the copy bandwidth
values = re.findall(r"\d+\.\d+", text)
values = [float(value) for value in values]

print(values[0])
# result = np.reshape(values,(35,5))

# SEQ = result[0:5,3]

# result = np.delete(result, (0,2,3), 1)

# T1 = result[0:5,:]
# T2 = result[5:10,:]
# T4 = result[10:15,:]
# T6 = result[15:20,:]
# T8 = result[20:25,:]
# T10 = result[25:30,:]
# T12 = result[30:35,:]

# plt.figure()
# plt.plot(T1[:,0],SEQ,'+-')
# plt.plot(T1[:,0],T1[:,1],'+-')
# plt.plot(T2[:,0],T2[:,1],'+-')
# plt.plot(T4[:,0],T4[:,1],'+-')
# plt.plot(T6[:,0],T6[:,1],'+-')
# plt.plot(T8[:,0],T8[:,1],'+-')
# plt.plot(T10[:,0],T10[:,1],'+-')
# plt.plot(T12[:,0],T12[:,1],'+-')

# plt.legend(["Sequentiel","1 Thread","2 Threads","4 Threads","6 Threads","8 Threads","10 Threads","12 Threads"])
# plt.xlabel("N")
# plt.ylabel("t (s)")
# plt.title("Multiplication")
# plt.show()
