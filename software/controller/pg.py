import pygame
import serial

use_serial = True

pygame.init()
pygame.display.init()
screen = pygame.display.set_mode((300, 300))

background_colour = (0,0,0)
screen.fill(background_colour)
pygame.display.flip()

if use_serial:
    ser = serial.Serial('/dev/tty.usbmodem2398521', 115200)

def send_command(keydown, kid):
    buff = ""
    if keydown:
        buff += "1"
    else:
        buff += "0"
    buff += chr(kid)
    buff += "\n"
    if use_serial:
        ser.write(buff)
    else:
        print buff

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            if use_serial:
                ser.close()
            pygame.quit(); #sys.exit() if sys is imported
        if event.type == pygame.KEYDOWN:
            background_colour = (10,0,0)
            screen.fill(background_colour)
            pygame.display.flip()
            if event.key == pygame.K_ESCAPE:
                print "Closing!"
                print
                if use_serial:
                    ser.close()
                quit()
            send_command(True, event.key)
        elif event.type == pygame.KEYUP:
            background_colour = (0,0,0)
            screen.fill(background_colour)
            pygame.display.flip()
            send_command(False, event.key)

if use_serial:
    ser.close()