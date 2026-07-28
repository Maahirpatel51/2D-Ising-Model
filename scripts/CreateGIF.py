import imageio

images=[]
outfile = "results/animations/"

for i in range(0, 200, 1):
    images.append(imageio.imread(f"frames/frame_{i:04d}.png"))

imageio.mimsave(
    outfile + "w_T2.269.gif",
    images,
    fps=10,
    loop=0
)