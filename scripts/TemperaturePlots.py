import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl

# Plot formatting
mpl.rcParams.update({
    "font.family": "serif",
    "mathtext.fontset": "cm",
    "font.size": 11,
    "axes.labelsize": 11,
    "axes.titlesize": 11,
    "legend.fontsize": 9,
    "xtick.labelsize": 9,
    "ytick.labelsize": 9,
    "figure.dpi": 300,
    "axes.linewidth": 0.8,
    "lines.linewidth": 1.5,
})
colors = ['#000000', '#E69F00', '#56B4E9', '#009E73']
labels = ['Metropolis', 'Wolff', r'Critical Temperature $T_c$']

outfile = "results/"

datafile1 = "results/metropolis_T.txt"
datafile2 = "results/wolff_T.txt"
data1 = np.loadtxt(datafile1)
data2 = np.loadtxt(datafile2)

T1 = data1.T[0]
M1 = np.abs(data1.T[1])
T2 = data2.T[0]
M2 = np.abs(data2.T[1])

fig, ax = plt.subplots(figsize=(6, 4))
ax.scatter(T1, M1, color=colors[1], s=20, label=labels[0])
ax.scatter(T2, M2, color=colors[3], s=20, label=labels[1], marker="s")
ax.axvline(x=2.269, linestyle='--', color="gray", label=labels[2])
ax.set_xlabel('Temperature [$T$]')
ax.set_ylabel(r'Magnetization Per Spin $\left[\frac{|M|}{L^2}\right]$')
#ax.set_ylabel(r'Specific Heat Capacity [$C_v(T)$]')
ax.grid(True, which='major', linestyle='--', linewidth=0.5, alpha=0.5)
ax.legend(frameon=False, loc='best')
ax.minorticks_on()
plt.tight_layout(pad=0.5)
plt.savefig(f"{outfile}MvsT.png")