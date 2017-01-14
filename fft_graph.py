import matplotlib.pyplot as plt

f = open('debug_fft', 'r')

data = f.read()
parsed_data = data.split()

l = len(parsed_data)

new_array = [ val * 44100 / l for val in range(l) ]

plt.plot(new_array, parsed_data)
plt.show()