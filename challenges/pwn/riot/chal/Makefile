APPLICATION = ctf
BOARD ?= microbit
RIOTBASE ?= $(CURDIR)/../../RIOT

USEMODULE += base64
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += xtimer

FEATURES_REQUIRED += periph_flashpage

CFLAGS += -DFLAG=\"ENO{fake_flag}\"

DEVELHELP ?= 1

include $(RIOTBASE)/Makefile.include
