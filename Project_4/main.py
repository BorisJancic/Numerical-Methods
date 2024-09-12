import math as m
import matplotlib.pyplot as plt

def f(x): return m.sin(x)
def F(x): return 1 - m.cos(x)

def centered_approximation(f, x, h):
    return h * (-f(x-2*h) + 13*f(x-h) + 13*f(x) - f(x+h)) / 24

def backward_approximation(f, x, h):
    return h * (f(x-3*h) - 5*f(x-2*h) + 19*f(x-h) + 9*f(x)) / 24

def RMSE(f, F, integral_method, a, b, n):
    sum = 0
    sum_RMSE = 0
    h = (b-a)/n
    x = a
    for i in range(n):
        x += h
        sum += integral_method(f, x, h)
        sum_RMSE += (F(x) - sum)**2

    return m.sqrt(sum_RMSE/(n+1))

i = 2
max = 1000000
RMSE_values_centered = []
RMSE_values_backward = []
RMSE_h_values = []

while (i < max):
    RMSE_values_centered.append(RMSE(f, F, centered_approximation, 0, 1, i))
    RMSE_values_backward.append(RMSE(f, F, backward_approximation, 0, 1, i))
    RMSE_h_values.append(1/i)
    i *= 2
print(RMSE_h_values)
print(RMSE_values_centered)
print("*****")
print(RMSE_values_centered)
print(RMSE_values_backward)


plt.plot(RMSE_h_values, RMSE_values_centered, label='Centered Approximation')
plt.plot(RMSE_h_values, RMSE_values_backward, label='Backward Approximation')

plt.xlabel('Step Size h (log scale)')
plt.ylabel('RMSE (log scale)')
plt.title('Error of int(sin(x)) over [0, 1] with respect to stepsize h')
plt.legend(loc='upper center')

plt.xscale('log')
plt.yscale('log')

plt.show()

print(centered_approximation(f, 0, 1))