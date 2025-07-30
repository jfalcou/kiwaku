import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
import numpy as np

# 1. Your original list of Matplotlib "tab" colors
global_colors = [
  'tab:blue',
  'tab:orange',
  'tab:green',
  'tab:red',
  'tab:purple',
  'tab:brown',
  'tab:pink',
  'tab:gray',
  'tab:olive',
  'tab:cyan'
]

# 2. Generate the darker versions
# We will scale the RGB values of each color by a factor to make them darker.
# A factor of 0.6 makes them noticeably darker.
darkening_factor = 0.6
darker_colors = []

for color_name in global_colors:
    # Convert the named color to its RGB representation (a tuple of floats from 0 to 1)
    rgb = mcolors.to_rgb(color_name)
    
    # Create the darker RGB by scaling each component
    darker_rgb = (rgb[0] * darkening_factor, 
                  rgb[1] * darkening_factor, 
                  rgb[2] * darkening_factor)
    
    darker_colors.append(darker_rgb)

# 3. Create a plot to visualize and compare the colors
fig, ax = plt.subplots(figsize=(12, 7))

bar_width = 0.4
index = np.arange(len(global_colors))

# Plot bars for the original colors
ax.bar(index - bar_width/2, [1] * len(global_colors), bar_width, 
       label='Original Colors', color=global_colors)

# Plot bars for the new darker colors
ax.bar(index + bar_width/2, [1] * len(global_colors), bar_width, 
       label=f'Darker Colors (Factor: {darkening_factor})', color=darker_colors)

# --- Customize the plot for clarity ---
ax.set_title('Original vs. Darker Matplotlib Colors', fontsize=16, pad=20)
ax.set_xticks(index)
ax.set_xticklabels([name.split(':')[-1].capitalize() for name in global_colors], rotation=45, ha="right")
ax.set_yticks([]) # Hide the y-axis ticks
ax.spines['top'].set_visible(False)
ax.spines['right'].set_visible(False)
ax.spines['left'].set_visible(False)
ax.spines['bottom'].set_color('#DDDDDD')
ax.legend()

plt.tight_layout()
plt.show()
