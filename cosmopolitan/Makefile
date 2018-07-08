# Debugging
DEBUG=1

CC := clang++
SRCDIR := src
BUILDDIR := build
RESOURCEDIR := resources

TARGET := bin/cosmo
TARGET_RESOURCES := bin/resources

RESOURCES_DIR := $(realpath $(TARGET_RESOURCES))
DEFINES+=-D RESOURCES_DIR=\"$(RESOURCES_DIR)/\"

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -std=c++11 -g $(DEFINES) # -Wall
LIB := -lm -lstdc++ -std=c++11 -lpthread -lSDL2 -lSDL2_image

$(TARGET): $(OBJECTS) $(TARGET_RESOURCES)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $(OBJECTS) -o $(TARGET) $(LIB) 

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) -c -o $@ $<"; $(CC) $(CFLAGS) -c -o $@ $<


$(TARGET_RESOURCES): FORCE
	@echo " Copying resources..."
	@mkdir -p $(TARGET_RESOURCES)
	@cp -f $(RESOURCEDIR)/* $(TARGET_RESOURCES) 2> /dev/null || :


clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean

FORCE:
.PHONY: FORCE
