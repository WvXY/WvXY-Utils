from time import time

import matplotlib
import matplotlib.animation as animation
import matplotlib.pyplot as plt
import numpy as np

matplotlib.use('QtAgg')

fig, ax = plt.subplots()


def draw_connections(p: np.ndarray, adj: np.ndarray):
    """
    draw connections using matplotlib
    :param p: points [n,2]
    :param adj: weighted adjacency matrix [n,n]
    :return: None
    """
    if p.shape[0] != adj.shape[0]:
        raise ValueError("Dimension mismatch")

    n = p.shape[0]
    for i in range(n):
        for j in range(i + 1, n):
            if adj[i, j] != 0:
                plt.plot([p[i, 0], p[j, 0]], [p[i, 1], p[j, 1]],
                         "k", lw=adj[i, j], alpha=0.1)  # lines
    plt.scatter(p[:, 0], p[:, 1], c="k", s=40, marker=".")  # center


def draw_rectangles(center: np.ndarray, wh: np.ndarray):
    """
    draw rectangles using matplotlib
    :param center: center of the rectangles
    :param wh: width and height (x & y)
    :return: None
    """
    if center.shape != wh.shape:
        raise ValueError("Dimension mismatch")

    for i in range(center.shape[0]):
        color = np.random.rand(3)
        w, h = wh[i]
        cx, cy = center[i]
        x = np.array([cx - w, cx + w, cx + w, cx - w])
        y = np.array([cy - h, cy - h, cy + h, cy + h])
        plt.fill(x, y,
                 fc='w', hatch="//", color=color, alpha=0.5, lw=2)
        plt.text(cx, cy, f"{i}", ha="left", va="bottom",
                 color=color / 3, fontsize=12, style='italic', )


def init_plot_settings():
    np.random.seed(0)
    ax.clear()
    # center_plot = np.average(c, axis=0)
    # ax.set_xlim(center_plot[0] - 5, center_plot[0] + 5)
    # ax.set_ylim(center_plot[1] - 5, center_plot[1] + 5)
    # ax.grid(True)
    # ax.axes.get_xaxis().set_visible(False)
    # ax.axes.get_yaxis().set_visible(False)
    ax.axis('off')
    ax.set_aspect('equal')

    # show fps
    global t
    plt.title(f"FPS: {1 / (time() - t):.2f}")
    t = time()


def main_loop(f):
    init_plot_settings()
    # add some function below

    # example
    c = np.random.randint(0, 10, [5, 2])
    wh = np.random.randint(4, 8, [5, 2])
    adj = np.random.random([5, 5])
    draw_rectangles(c, wh)
    draw_connections(c, adj)


def show():
    global t
    t = time()
    ani = animation.FuncAnimation(fig, main_loop,
                                  repeat=False, frames=3000, interval=0)
    plt.show()


if __name__ == "__main__":
    show()
