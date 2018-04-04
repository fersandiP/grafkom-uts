HEADERS = glm.h
SOURCES = glm.c main.c
DEPENDS = $(SOURCES:.c=.d)
OBJECTS = $(SOURCES:.c=.o)
PROGRAMS = main

ifdef DEBUG
OPTFLAGS = -g
else
OPTFLAGS = -O3
endif

CC      = gcc
CFLAGS  = -Wall $(OPTFLAGS)
LDFLAGS = -lGL -lglu -lglut

UNAME = $(shell uname)
ifeq ($(UNAME), Darwin)  # Mac OS X
# OpenGL and GLUT are frameworks, override LDFLAGS above
LDFLAGS = -framework OpenGL -framework GLUT
# Universal binary support
CFLAGS  += -arch i386
endif


all: $(PROGRAMS)

$(PROGRAMS):
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

define PROGRAM_template
$(1): $(addsuffix .o,$(1)) glm.o
endef
$(foreach t,$(PROGRAMS),$(eval $(call PROGRAM_template,$(t))))

clean:
	$(RM) $(OBJECTS) $(DEPENDS)
	$(RM) $(PROGRAMS:=$(EXE))

.PHONY: all clean

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.d: %.c
	$(CC) -MM $< -o $@

