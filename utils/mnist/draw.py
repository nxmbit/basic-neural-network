import pygame
import csv
import numpy as np

window_size = 560
canvas_size = 28
brush_size = 1  # Wielkość pędzla (rozmiar pędzla / 2)
scaling_factor = window_size // canvas_size

pygame.init()

window = pygame.display.set_mode((window_size, window_size + 50))
window.fill((0, 0, 0))

canvas = np.zeros((canvas_size, canvas_size), dtype=np.int16)

font = pygame.font.Font(None, 32)
instructions = font.render("Left Mouse - Draw, C - Clear, Enter - Save & Exit", True, (255, 255, 255))
instructions_rect = instructions.get_rect(center=(window_size/2, window_size + 25))

def draw_on_canvas(position):
    x = position[0] // scaling_factor
    y = position[1] // scaling_factor

    canvas[max(0, y-brush_size):min(canvas_size, y+brush_size),
           max(0, x-brush_size):min(canvas_size, x+brush_size)] += 10

    for i in range(-brush_size, brush_size + 1):
        for j in range(-brush_size, brush_size + 1):
            if 0 <= x+i < canvas_size and 0 <= y+j < canvas_size:
                canvas[y+j, x+i] = min(255, canvas[y+j, x+i] + max(0, 10 - abs(i) - abs(j)))

    np.clip(canvas, 0, 255, out=canvas)
    update_window()


def update_window():
    surface = pygame.Surface(canvas.shape)
    pygame.surfarray.blit_array(surface, np.stack((canvas.astype(np.uint8), canvas.astype(np.uint8), canvas.astype(np.uint8)), axis=-1))
    canvas_scaled = pygame.transform.scale(surface, (window_size, window_size))
    window.blit(canvas_scaled, (0, 0))
    window.blit(instructions, instructions_rect)

def clear_canvas():
    canvas.fill(0)

def save_image():
    with open("image.csv", 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(canvas.flatten())

window.blit(instructions, instructions_rect)

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.MOUSEMOTION:
            if pygame.mouse.get_pressed()[0]:
                if event.pos[1] < window_size:
                    draw_on_canvas((event.pos[1], event.pos[0]))
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_c:
                clear_canvas()
                update_window()
            elif event.key == pygame.K_RETURN:
                save_image()
                running = False
    pygame.display.flip()

pygame.quit()

