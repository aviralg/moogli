import matplotlib.pyplot as plt


xv1 = 10

x = plt.vlines([1, 1.5, 2.0, 2.5, 3.0, 3.5], -0.25, 0.25,
               color='red')#, linewidth=20, linestyles='solid')
y = plt.vlines([1, 1.5, 2.0, 2.5, 3.0, 3.5], 0.25, 0.75,
               color='yellow')#, linewidth=20, linestyles='solid')
print x.get_axes()
print y.get_axes()
print x.get_axes().lines
plt.plot([1, 2, 3], [1, 2, 3], '-', label='line 1', linewidth=20)
plt.xlabel('Time [s]')
plt.ylabel('P [Pa]')
plt.grid()
plt.show()


# plt.plot([1, 2, 3], [1, 2, 3], '-', label='line 1', linewidth=20)
# plt.plot([1.25, 1.5, 2, 2.5, 3, 3.25],  '|', linewidth=1, label='line 2')
# plt.axvline([1.25, 1.5, 2, 2.5, 3, 3.25], [3, 3, 3, 3, 3, 3], '|', linewidth=100, label='line 2')
# plt.plot([1.25, 1.5, 2, 2.5, 3, 3.25], [4, 4, 4, 4, 4, 4], '|', linewidth=100, label='line 2')
# plt.show()
# [(2, 3), (2, 3), (2, 3), (2, 3), (2, 3), (2, 3)],
