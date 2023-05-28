import csv
import numpy as np
import pygame

window_size = 560
canvas_size = 28
scaling_factor = window_size // canvas_size

pygame.init()

window = pygame.display.set_mode((window_size, window_size))
window.fill((0, 0, 0))

def display_image(image_data):
    surface = pygame.Surface((canvas_size, canvas_size))
    pygame.surfarray.blit_array(surface, np.stack((image_data, image_data, image_data), axis=-1))
    canvas_scaled = pygame.transform.scale(surface, (window_size, window_size))
    window.blit(canvas_scaled, (0, 0))

def load_image():
    with open("image.csv", 'r', newline='') as f:
        reader = csv.reader(f)
        for row in reader:
            image_data = np.array(row, dtype=np.uint8).reshape((canvas_size, canvas_size))
            display_image(image_data)

load_image()

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
    pygame.display.flip()

pygame.quit()

