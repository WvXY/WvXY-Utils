"""_summary_
This is a python component for Grasshopper to generate an Apollonian Gasket.
Need to copy and paste the code into a python component in Grasshopper. 
Cannot execute it directly.

@params
n:      int, number of circles in each level
levels: int, number of levels(outside circles)
"""

from ghpythonlib.componentbase import executingcomponent as component
import Grasshopper as gh
import GhPython
import System
import Rhino
import rhinoscriptsyntax as rs
import Rhino.Geometry as rg
import math, cmath

# functions for rhino geometry
def newPoint(x, y):
    return rg.Point3d(x, y, 0)


def newCircle(r, center=None):
    if not center:
        center = newPoint(0, r)
    return rg.Circle(center, r)


def manyCircles(centers, rs):
    cs = []
    for r, c in zip(rs, centers):
        cs.append(newCircle(r, center=c))
    return cs


# design
class Tesslelation(component):
    def __init__(self):
        self.cs = []
        self.rs = []
        self.ks = []

    def apolloniusCircle(self, ic1, ic2, ic3, inside=-1):
        """ic1, ic2, ic3 are indexes for circles, return the apollonius circle"""
        k1, k2, k3 = self.ks[ic1] * inside, self.ks[ic2], self.ks[ic3]
        c1, c2, c3 = self.cs[ic1], self.cs[ic2], self.cs[ic3]
        k4 = k1 + k2 + k3 + 2 * cmath.sqrt(k1 * k2 + k2 * k3 + k1 * k3)

        kc1, kc2, kc3 = c1 * k1, c2 * k2, c3 * k3
        kc4 = kc1 + kc2 + kc3 + 2 * cmath.sqrt(kc1 * kc2 + kc2 * kc3 + kc1 * kc3)
        c4 = kc4 / k4
        self.cs.append(c4)
        self.ks.append(k4)
        self.rs.append(1 / k4)

    def RunScript(self, n, levels):
        # initialize
        R = 128
        ratio = 0.8
        radii_init = [R, R * ratio, R * (1 - ratio)]  # 0,1,2
        center_init = [[0, 0], [0, R * ratio - R], [0, R - R * (1 - ratio)]]

        # centers, radii, bend
        self.cs = [complex(*c) for c in center_init]
        self.rs = [complex(r) for r in radii_init]
        self.ks = [1 / r for r in self.rs]

        for l in range(int(levels)):
            R *= ratio
            self.rs.append(complex(R * ratio))
            self.ks.append(1 / self.rs[-1])
            self.rs.append(complex(R * (1 - ratio)))
            self.ks.append(1 / self.rs[-1])
            self.cs.append(complex([0, R * ratio - R]))
            self.cs.append([0, R - R * (1 - ratio)])

            for i in range(int(n)):
                self.apolloniusCircle(l, 1, 2 + i)

        # output
        centers = [newPoint(c.real, c.imag) for c in self.cs]
        radii = [r.real for r in self.rs]
        circles = manyCircles(centers, radii)
        return circles, centers, radii
