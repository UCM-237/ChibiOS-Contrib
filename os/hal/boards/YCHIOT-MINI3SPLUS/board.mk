# List of all the board related files.
BOARDSRC = ${CHIBIOS_CONTRIB}/os/hal/boards/YCHIOT-MINI3SPLUS/board.c

# Required include directories
BOARDINC = ${CHIBIOS_CONTRIB}/os/hal/boards/YCHIOT-MINI3SPLUS

# Shared variables
ALLCSRC += $(BOARDSRC)
ALLINC  += $(BOARDINC)
