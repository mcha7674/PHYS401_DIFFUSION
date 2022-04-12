# ofileName = "../Plots/diffuse200.png"
# ifileName = "../Data/diffuse.csv"
# set datafile separator ','
# set terminal pngcairo enhanced font ",18" size 1440,800
# set output ofileName
# # Data point styles
# set style line 1 lc rgb "blue" pt 7 ps 0.3
# # Grid line styles
# set style line 2 lc rgb "black" lw 1 
# #set xrange [0:100]
# #set yrange [0:100]
# set grid xtics ytics ls 2          # enable the grid

# # figure specifications 
# unset key
# set xlabel "X" 
# set ylabel "Y" 
# set title "Record of Particle Positions after 100 seconds"
# plot ifileName using 1:2 ls 1

# ##### ENTROPY PLOT #######
# ofileName = "../Plots/entropy200.png"
# ifileName = "../Data/entropy100.csv"
# ifileName2 = "../Data/entropy200.csv"
# set datafile separator ','
# set terminal pngcairo enhanced font ",16" size 1600,1500
# set output ofileName
# # Data point styles
# set style line 1 lc rgb "blue" pt 7 ps 0.5
# # Grid line styles
# set style line 2 lc rgb "black" lw 1 dt 2
# # asymptote line
# set style line 3 lc rgb "red" lw 1.5
# #set xrange [800:]
# #set yrange [4.5:4.7]
# set grid xtics ytics ls 0.5         # enable the grid
# set key bottom right
# # figure specifications 
# set xlabel "Time (seconds)" 
# set ylabel "Entropy" 
# set title "Entropy Time Evolution per Particle"
# plot ifileName2 using 1:2 ls 1 title "", 4.6 ls 3 title "Asymptote at approximately 4.6"

# #### RELAXATION PLOT ###########

ofileName = "../Plots/relaxLinRegime200AND100.png"
ofileName2 = "../Plots/relaxLOGLinRegime200.png"
ofileName3 = "../Plots/relaxLOGLinRegime100.png"
ofileName4 = "../Plots/relaxLOGLinRegime200and100.png"
ofileName5 = "../Plots/relaxLOGRegime200.png"
ofileName6 = "../Plots/relaxLOGRegime100.png"

ifileName = "../Data/relax.csv"
ifileName2 = "../Data/relaxlog.csv"
ifileName3 = "../Data/relax200.csv"
ifileName4 = "../Data/relaxlog200.csv"
ifileName5 = "../Data/relaxlog200LIN.csv"
ifileName6 = "../Data/relaxlog100LIN.csv"
set datafile separator ','
set terminal pngcairo enhanced font ",18" size 1600,1500
# Data point styles
set style line 1 lc rgb "blue" pt 7 ps 0.5
set style line 4 lc rgb "orange" pt 7 ps 0.5 
# Grid line styles
set style line 2 lc rgb "black" lw 1 dt 2

set key autotitle columnhead
#set xrange [1000:]
#set yrange [4.54:4.61]
set grid xtics ytics ls 0.5         # enable the grid
#set key bottom right
# figure specifications 
set xlabel "Time (seconds)" 
set ylabel "ln(R(t))" 
set title "Relaxation of the Entropy to Equilibrium"
set key top right

# logarithmic plot 200
set output ofileName5
#set xrange [0:4000]
plot ifileName4 ls 4 title "ln(R(t)) for 200x200" with lines
# logarithmic plot 100
set output ofileName6
plot ifileName2 ls 1 title "ln(R(t)) for 100x100" with lines

# logarithmic plot 200 AND 100
set output ofileName4
plot ifileName2 ls 1 title "ln(R(t)) for 100x100" with lines, ifileName4 ls 4 title "ln(R(t)) for 200x200" with lines

#########################

# LINEAR REGIME FOR LOG M = 200
set output ofileName2
set style line 3 lc rgb "red" lw 1.5 # fitline line style
set key
# LINEAR REGRESSION FIT FOR LOGARITHMIC PLOT
f(x) = m*x + b
fit f(x) ifileName5 using 1:2 via m,b # stores value of m and b
# M = 200
# m = -0.000972965 . ,b = 0.795931   -> t = 1000:2500
set title "Relaxation of the Entropy to Equilibrium (M = 200)"
plot ifileName5 using 1:2 title "ln(R(t))" ls 1 with lines, f(x) title "y = -0.000972965x + 0.795931" ls 3 with lines

# LINEAR REGIME FOR LOG M = 100
set output ofileName6
f(x) = m*x + b
fit f(x) ifileName6 using 1:2 via m,b # stores value of m and b
M = 100
#m = -0.00426627 ,b = 0.859985   -> t = 100:600
set title "Relaxation of the Entropy to Equilibrium (M = 100)"

plot ifileName6 using 1:2 title "ln(R(t))" ls 1 with lines, f(x) title "y = -0.00426627x + 0.869985" ls 3 with lines