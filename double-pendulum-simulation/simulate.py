import matplotlib.pyplot as plt
import numpy as np

l = 1


class State:
    def __init__(self, phi, phi_dot, psi, psi_dot):
        self.phi = phi
        self.phi_dot = phi_dot
        self.psi = psi
        self.psi_dot = psi_dot


data: list[State] = []
with open("double-pendelum-pts.txt", "r") as file:
    lines = file.readlines()
    tmp_data = [line.split() for line in lines]
    data = [State(float(d[0]), float(d[1]), float(d[2]), float(d[3])) for d in tmp_data]

x_pend_1 = [l * np.sin(state.phi) for state in data]
y_pend_1 = [-l * np.cos(state.phi) for state in data]
x_pend_2 = [l * np.sin(state.phi) + l * np.sin(state.psi) for state in data]
y_pend_2 = [-l * np.cos(state.phi) - l * np.cos(state.psi) for state in data]

plt.plot(x_pend_1, y_pend_1, "r", label="Point 1")
plt.plot(x_pend_2, y_pend_2, "g", label="Point 2")
plt.legend()
plt.show()
