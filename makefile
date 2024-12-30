# ----------------------------
# Makefile Options
# ----------------------------

NAME = STOPRES
ICON = src/gfx/icon.png
DESCRIPTION = "Resistor Learning game"
COMPRESSED = YES
ARCHIVED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -I"src/helpers/" -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
