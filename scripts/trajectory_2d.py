import matplotlib.pyplot as plt
import numpy as np
from math import ceil, sqrt
from copy import deepcopy

from __init__ import ureg


class Trajectory2D:
    def __init__(self, points=None, curvatures=None, velocities=None, file=None):
        self.points = []
        self.curvatures = []
        self.velocities = []
        if file is not None:
            with open(file) as f:
                for line in f:
                    parts = list(map(float, line.split(",")))
                    if len(parts) < 2:
                        raise ValueError("Each point should have at least 2 data points")
                    parts.extend([0.0] * (4 - len(parts)))
                    self.points.append(np.array(parts[:2]))
                    self.curvatures.append(parts[2])
                    self.velocities.append(parts[3])
        else:
            self.points = [np.array(point) for point in points]
            if curvatures is not None:
                self.curvatures = curvatures
            else:
                self.curvatures = [0.0] * len(self.points)
            if velocities is not None:
                self.velocities = velocities
            else:
                self.velocities = [0.0] * len(self.points)

    def save_to_file(self, file=None):
        with open(file, mode="w") as f:
            for i, (x, y) in enumerate(self.points):
                f.write("{},{},{},{}\n".format(x, y, self.curvatures[i] if self.curvatures else 0,
                                               self.velocities[i] if self.velocities else 0))

    def interpolate(self, resolution):
        temp = []
        for start, end in zip(self.points[:-1], self.points[1:]):
            diff = end - start
            steps = ceil(np.linalg.norm(diff) / resolution)
            step = diff / steps

            for i in range(steps):
                temp.append(start + step * i)
        if len(self.points):
            temp.append(self.points[-1])
        self.points = temp
        self.curvatures.extend([0.0] * (len(self.points) - len(self.curvatures)))
        self.velocities.extend([0.0] * (len(self.points) - len(self.velocities)))

    def smoothen(self, weight, iterations):
        smooth_weight = 1 - weight
        for _ in range(iterations):
            for i in range(1, len(self.points) - 1):
                for j in range(2):
                    aux = self.points[i][j]
                    data_fac = weight * (self.points[i][j] - aux)
                    smooth_fac = smooth_weight * (self.points[i - 1][j] + self.points[i + 1][j] - 2 * aux)
                    self.points[i][j] += data_fac + smooth_fac

    def set_curvatures(self):
        self.curvatures[0] = self.curvatures[-1] = 0.0
        for i in range(1, len(self.points) - 1):
            a = np.linalg.norm(self.points[i] - self.points[i - 1])
            b = np.linalg.norm(self.points[i + 1] - self.points[i])
            c = np.linalg.norm(self.points[i - 1] - self.points[i + 1])

            abc = a * b * c
            s = (a + b + c) / 2
            area = s * (s - a) * (s - b) * (s - c)
            area = sqrt(area) if area >= 0.0 else 0.0
            self.curvatures[i] = 4 * area / abc if abc else 0.0

    def set_velocities(self, max_vel, max_decel, k_turn):
        self.velocities[-1] = 0.0
        for i in range(len(self.points) - 2, -1, -1):
            target_vel = k_turn / self.curvatures[i] if self.curvatures[i] else max_vel
            dist = np.linalg.norm(self.points[i] - self.points[i + 1])
            decel_limited = sqrt(self.velocities[i + 1] ** 2 + 2 * max_decel * dist)
            self.velocities[i] = min(target_vel, decel_limited, max_vel)

    def visualise(self, colour=None, scale=1):
        x = [point[0] / scale for point in self.points]
        y = [point[1] / scale for point in self.points]
        if colour == "curvatures":
            plt.scatter(x, y, s=10, c=self.curvatures)
            plt.colorbar()
        elif colour == "velocities":
            plt.scatter(x, y, s=10, c=self.velocities)
            plt.colorbar()
        else:
            plt.scatter(x, y, s=10)
        plt.show()


feet = (1 * ureg.foot).to(ureg.metre).magnitude
traj = Trajectory2D([(0, 0), (0, 4 * feet), (2 * feet, 4 * feet), (4 * feet, 2 * feet)])
traj.interpolate(0.01)
traj.smoothen(0.8, 1000)
traj.set_curvatures()
traj.set_velocities(0.75, 0.4, 3)
traj.visualise("velocities", feet)

traj2 = Trajectory2D(file="test.csv")
traj2.visualise("velocities", feet)
