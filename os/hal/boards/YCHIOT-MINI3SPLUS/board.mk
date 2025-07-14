# List of all the board related files.
BOARDSRC = ${CHIBIOS}/os/hal/boards/YCHIOT-MINI3SPLUS/board.c

# Required include directories
BOARDINC = ${CHIBIOS}/os/hal/boards/YCHIOT-MINI3SPLUS

# Shared variables
ALLCSRC += $(BOARDSRC)
ALLINC  += $(BOARDINC)
