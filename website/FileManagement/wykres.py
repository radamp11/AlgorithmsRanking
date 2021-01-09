#!/usr/bin/python3

#from scipy.interpolate import make_interp_spline, BSpline
import matplotlib.style as mst
import matplotlib.pyplot as plt
import numpy as np
import szkielet

file1 = "RB.zip";
file2 = "DES.zip";

for x in [1,2]:
	if x == 1:
		mean_val = szkielet.calculateConvergenceGraph(file1, 21, 0)
	else:
		mean_val = szkielet.calculateConvergenceGraph(file2, 21, 0)

	#x_axis = [0.01, 0.02, 0.03, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1]
	x_axis = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13]
	y_axis = mean_val


	#usuwanie najw
	while y_axis[0] > 1500:
		x_axis.pop( max(x_axis) )
		y_axis.__delitem__(0)
	
	if x == 1:
		plt.scatter( x_axis, y_axis, c='darkred', marker='o' )
		plt.plot( x_axis, y_axis, c='black', label="error value" + file1 )
	else:
		plt.scatter( x_axis, y_axis, c='darkgreen', marker='o' )
		plt.plot( x_axis, y_axis, c='darkviolet', label="error value" + file2 )

mst.use('seaborn')
plt.title("jazda"+" Dimension:" + "10")
plt.xlabel("TestX")
plt.ylabel("TestY")
plt.ylim(bottom = -10)
plt.grid(True)
plt.legend()


#x_new = np.linspace( min(x_axis), max(x_axis), 500)
#a_BSpline = make_interp_spline(x_axis, y_axis)
#y_new = a_BSpline(x_new)
#plt.plot(x_new, y_new)
#plt.savefig('lib/lol.png',dpi = 512)
plt.show()
