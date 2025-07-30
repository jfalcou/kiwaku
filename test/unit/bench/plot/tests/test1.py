import matplotlib.pyplot as plt
import numpy as np

# --- 1. Sample Data ---
# Imagine you have two sets of benchmark results (e.g., before and after an optimization)
labels = ['Hand-written', 'Kiwaku CPU', 'Kiwaku SIMD', 'std::par', 'SYCL GPU']
times_before = [28, 35, 15, 14, 5]  # Slower times
times_after = [27, 25, 8, 14, 3]   # Faster times

# X-axis positions for the bars
x = np.arange(len(labels))

# --- 2. Create the Plot ---
# Use a slightly larger figure size for better readability
fig, ax = plt.subplots(figsize=(10, 7))

# --- 3. Plot the Bars ---
# First, plot the "before" data. We'll make these bars slightly wider and
# give them a lighter color to act as a background.
ax.bar(x, times_before, width=0.8, color='lightgray', label='Before Optimization')

# Now, plot the "after" data directly on top.
# These bars are slightly narrower and have a more vibrant color.
# The alpha value adds some transparency.
ax.bar(x, times_after, width=0.8, color='dodgerblue', alpha=0.9, label='After Optimization')

# --- 4. Customize the Plot for Clarity ---
ax.set_ylabel('Execution Time (milliseconds)', fontsize=12)
ax.set_title('Benchmark Performance: Before vs. After Optimization', fontsize=16, pad=20)

# Set the x-axis labels
ax.set_xticks(x)
ax.set_xticklabels(labels, rotation=15, ha="right")

# Add a horizontal grid for easier value reading and remove unnecessary spines
ax.yaxis.grid(True, linestyle='--', which='major', color='grey', alpha=0.7)
ax.spines['top'].set_visible(False)
ax.spines['right'].set_visible(False)
ax.spines['left'].set_visible(False)
ax.spines['bottom'].set_color('#DDDDDD')

# Add a legend to explain the colors
ax.legend()

# Ensure everything fits without overlapping
plt.tight_layout()

# --- 5. Show the Plot ---
plt.show()
