import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path
from matplotlib.colors import ListedColormap

folder_path = Path("export/")
files = sorted(folder_path.glob("*.txt"))

plotColour = ListedColormap(['#161b22', '#39d353'])

for i, file in enumerate(files):
    dataset = np.loadtxt(file)

    plt.figure(facecolor="lightgray")
    plt.imshow(dataset, cmap=plotColour, interpolation="nearest", vmin=-1, vmax=1)
    plt.colorbar(ticks=[-1, 1])
    plt.axis("off")
    plt.title(f"L = 100   T = 2.269")
    plt.savefig(f"frames/frame_{i:04d}.png", dpi=300, bbox_inches="tight", pad_inches=0.05)
    plt.close()
