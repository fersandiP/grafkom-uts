HEADERS = glm.h
SOURCES = glm.c main.c
DEPENDS = $(SOURCES:.c=.d)
OBJECTS = $(SOURCES:.c=.o)
PROGRAMS = main
OUT = out/

ifdef DEBUG
OPTFLAGS = -g
else
OPTFLAGS = -O3
endif

CC      = gcc
CFLAGS  = -Wno-everything $(OPTFLAGS)
LDFLAGS = -lGL -lglu -lglut

UNAME = $(shell uname)
ifeq ($(UNAME), Darwin)  # Mac OS X
# OpenGL and GLUT are frameworks, override LDFLAGS above
LDFLAGS = -framework OpenGL -framework GLUT
# Universal binary support
CFLAGS  += -arch i386
endif


all: clean makedir $(OUT)$(PROGRAMS)

$(OUT)$(PROGRAMS):
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

define PROGRAM_template
$(1): $(addsuffix .o,$(1)) out/glm.o
endef
$(foreach t,$(OUT)$(PROGRAMS),$(eval $(call PROGRAM_template,$(t))))

makedir: $(OUT)

$(OUT):
	mkdir -p $@

clean:
	$(RM) -r $(OUT)
run: 
	./out/main

.PHONY: makedir all clean run

$(OUT)%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

