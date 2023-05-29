import csv
import tkinter as tk
from tkinter import colorchooser
from skimage import color
from skimage import io

def choose_color():
    # Open the color chooser dialog and get the selected color
    color_code = colorchooser.askcolor(title ="Choose color")
    return color_code

def rgb_to_cielab(rgb):
    # Normalize RGB values to 0-1
    rgb = [x/255 for x in rgb]
    # Reshape to a 3D array with single pixel, then convert to LAB
    lab = color.rgb2lab([[rgb]])
    return lab[0][0]

def write_to_csv(cielab, filename):
    # Open the file in write mode
    with open(filename, 'w', newline='') as file:
        writer = csv.writer(file)
        # Write the color
        writer.writerow(cielab)

def main():
    root = tk.Tk()
    root.withdraw()

    color_rgb, color_hex = choose_color()
    cielab = rgb_to_cielab(color_rgb[:3])
    write_to_csv(cielab, 'color_cielab.csv')

    root.destroy()

if __name__ == '__main__':
    main()

