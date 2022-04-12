from matplotlib import projections, pyplot as plt
import numpy as np
import pandas as pd
from matplotlib.ticker import FormatStrFormatter
from matplotlib.ticker import AutoMinorLocator
from matplotlib.ticker import MultipleLocator
from mpl_toolkits import mplot3d
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from pylab import meshgrid, cm, imshow, contour, colorbar, show
import glob
import os
import imageio

# LAPTOP
dataPath1 = "C:\\Users\\mcha7\\Desktop\\COMP_PHYS\\HW7\\Data\\"
plotPath1 = "C:\\Users\\mcha7\\Desktop\\COMP_PHYS\\HW7\\Plots\\"
# DESKTOP
dataPath2 = "C:\\Users\\mcha7\\OneDrive\\Desktop\\Code\\COMP_PHYS\\PHYS-401-CompPhys\\HW7\\Data\\"
plotPath2 = "C:\\Users\\mcha7\\OneDrive\\Desktop\\Code\\COMP_PHYS\\PHYS-401-CompPhys\\HW7\\Plots\\"

data = pd.read_csv(dataPath2 + "diffuse.csv")
#data08 = pd.read_csv(dataPath2 + "string08.csv")

n = data["t"].unique()
numSnapShots = 20
interval = int((len(n)+1)/numSnapShots)
interval = 15
data1 = data[data["t"] % interval == 0]
data1.reset_index(inplace=True, drop=True)
n = data1["t"].unique()  # for every other n

imageList = []
for i, N in enumerate(n):
    print("t = ", N)
    data2 = data[data["t"] == N]
    x = data2["x"]
    y = data2["y"]

    fig, ax = plt.subplots(figsize=[12, 10])
    title = "Diffusion of 2000 particles at t = {0:.1f} seconds".format(N)
    ax.set_title(title, fontsize=18)
    ax.set_xlabel("X", fontsize=18)
    ax.set_ylabel("Y", fontsize=18)
    ax.scatter(x, y, c="black", s=10, marker='.')
    #ax.set_ylim([0, 100])
    #ax.set_xlim([0, 100])

    PNGplotPathLAPTOP = "C:\\Users\\mcha7\\Desktop\\COMP_PHYS\\HW7\\Plots\\Animations\\"
    PNGplotPathDESKTOP = "C:\\Users\\mcha7\\OneDrive\\Desktop\\Code\\COMP_PHYS\\PHYS-401-CompPhys\\HW7\\Plots\\Animations\\"
    imPath = PNGplotPathDESKTOP + f"diffuse{i+1}.png"
    # store path on file

    imageList.append(imPath)
    fig.savefig(imPath, dpi=144)

# ANIMATE
print("Length of the Image list = ", len(imageList))
images = []
fps = 20
duration = len(imageList)/fps
print(f"Animation will take {duration} seconds")
for path in imageList:
    images.append(imageio.imread(path))
imageio.mimsave(plotPath2+"diffusion.gif", images, fps=fps)
