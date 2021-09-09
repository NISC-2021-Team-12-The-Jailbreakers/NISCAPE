# Shell PATH
SSD1306PATH = ./library/ssd1306

# RT Shell files.
SSD1306SRC = $(SSD1306PATH)/ssd1306.c

SSD1306INC = $(SSD1306PATH)

# Shared variables
ALLCSRC += $(SSD1306SRC)
ALLINC  += $(SSD1306INC)
