import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation


# Читаем данные из файла
data = np.loadtxt("double-pendelum-pts.txt")
phi = data[:, 0]  # Первый угол
psi = data[:, 2]  # Второй угол

# Длина маятников
l = 1  # Можно поменять, если в C-коде другое значение

# Вычисляем координаты точек
x1 = l * np.sin(phi)
y1 = -l * np.cos(phi)
x2 = x1 + l * np.sin(psi)
y2 = y1 - l * np.cos(psi)

# Создаем анимацию
fig, ax = plt.subplots()
ax.set_xlim(-2 * l, 2 * l)
ax.set_ylim(-2 * l, 2 * l)
ax.set_aspect("equal")

(line,) = ax.plot([], [], "o-", lw=2)  # Линия маятника


def init():
    line.set_data([], [])
    return (line,)


def update(frame):
    line.set_data([0, x1[frame], x2[frame]], [0, y1[frame], y2[frame]])
    return (line,)


ani = animation.FuncAnimation(
    fig, update, frames=len(phi), init_func=init, blit=True, interval=10
)
plt.show()
