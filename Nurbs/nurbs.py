import numpy as np
import matplotlib.pyplot as plt


def basis_function(t, i, p, U):
    if p == 0:
        if U[i] <= t and t < U[i + 1]:
            return 1
        else:
            return 0
    else:  # can be optimized
        B1 = basis_function(t, i, p - 1, U)
        B2 = basis_function(t, i + 1, p - 1, U)
        if B1 == 0:
            N1 = 0
        else:
            N1 = B1 * (t - U[i]) / (U[i + p] - U[i])
        if B2 == 0:
            N2 = 0
        else:
            N2 = B2 * (U[i + p + 1] - t) / (U[i + p + 1] - U[i + 1])
        # l1 = (t - U[i]) / (U[i+p] - U[i])
        # l2 = (U[i+p+1] - t) / (U[i+p+1] - U[i+p])

        # return basis_function(t, i, p-1, U) * l1 + \
        #    basis_function(t, i+1, p-1, U)* l2
        return N1 + N2


def rational_basis_function(t, i, p, n, w, U):
    sum = 0
    for j in range(n + 1):
        sum += w[j] * basis_function(t, j, p, U)
    return basis_function(t, i, p, U) * w[i] / sum


def nurbs(t, p, n, w, U, controlPoints):
    vertices = np.zeros([1, 2])
    for i in range(n + 1):
        vertices += rational_basis_function(t, i, p, n, w, U) * controlPoints[i]
    return vertices

if __name__ == "__main__":
    # use nurbs to draw a perfect circle
    n, p = 6, 2  # n : #ContolPoints-1, p : spline order(?)
    # control points
    P = np.array(
        [
            [0, 0],
            [1, 0],
            [1 / 2, np.sqrt(3) / 2],
            [0, np.sqrt(3)],
            [-1 / 2, np.sqrt(3) / 2],
            [-1, 0],
            [0, 0],
        ]
    )
    # Knot vector
    U = np.array(
        [0, 0, 0, 1 / 3, 1 / 3, 2 / 3, 2 / 3, 1, 1, 1]
    )  # in dimention m(=n+p+1)+1
    # weight (if all w are 1, equals to B-Spline)
    w = np.array([1, 0.5, 1, 0.5, 1, 0.5, 1])  # np.random.random([n+1,1])

    # # print(vertices)

    # --------------Plot-------------------
    tt = np.linspace(0, 1, 1000)

    # plt.plot(vv[:,0], vv[:,1])
    y = np.zeros([1000, 2])
    for i in range(1000):
        y[i] = nurbs(tt[i], p, n, w, U, P)
    plt.scatter(P[:, 0], P[:, 1])
    plt.plot(y[:, 0], y[:, 1], c="r")
    plt.plot(P[:, 0], P[:, 1])
    plt.show()
