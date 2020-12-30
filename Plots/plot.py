import matplotlib
matplotlib.use('Agg')

# importing the required module 
import matplotlib.pyplot as plt
  
# x axis values 
y_serial = [1.828125, 1.828125, 1.828125, 1.828125, 1.828125]
y_pthreads = [1.453125, 1.256250, 1.140625, 0.878125, 0.951875]
y_openmp = [9.265625, 6.453125, 5.640625, 4.906250, 3.968750]
y_mpi = [1.484375, 0.750000, 0.515625, 0.375000, 0.171875]
y_hybrid = [10.265625, 8.453125, 7.640625, 6.906250, 6.568750]

# corresponding y axis values 
x = [1, 2, 4, 8, 16] 
  
# plotting the points  
plt.plot(x, y_serial, label = "serial")
plt.plot(x, y_pthreads, label = "pthreads")
plt.plot(x, y_openmp, label = "openmp")
plt.plot(x, y_mpi, label = "mpi")
plt.plot(x, y_hybrid, label = "hybrid")
# naming the x axis 
plt.xlabel('threads') 
# naming the y axis 
plt.ylabel('time') 
  
# giving a title to my graph 
plt.title('Comparison') 
  
plt.legend()  
# function to show the plot 
plt.show()

plt.savefig('comparison.png')
