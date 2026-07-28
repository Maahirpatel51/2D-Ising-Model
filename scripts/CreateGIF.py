import imageio

images=[]
outfile = "results/animations/"

for i in range(0, 250, 1):
    images.append(imageio.imread(f"frames/frame_{i:04d}.png"))

imageio.mimsave(
    outfile + "metropolis_T2.269.gif",
    images,
    fps=15
)