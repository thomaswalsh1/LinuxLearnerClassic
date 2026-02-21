# =========================
# Project configuration
# =========================

# Compiler (native)
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g \
	-I./src \
	-I./src/helpers \
	-I./src/engine \
	-I./src/ui \
	-I./src/validators \
	-I./src/config

# Linker flags
LDFLAGS = -lncurses -lform -lreadline

# Source files (recursive)
SRCS := $(shell find src -name '*.c')

# Object files (native build only)
OBJS := $(SRCS:.c=.o)

# Target executable
TARGET = linuxlearner

# Version
VERSION = 0.1.1

# Project name (current directory)
PROJECT_NAME = $(shell basename $(CURDIR))

# Release naming
RELEASE_NAME = linuxlearner-v$(VERSION)
BIN_RELEASE_X86 = $(RELEASE_NAME)-linux-x86_64

# =========================
# Default build
# =========================

all: $(TARGET)

# Link native build
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile C files into objects
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


# =========================
# Cleanup
# =========================

clean:
	rm -f $(OBJS) $(TARGET)

# =========================
# Source release
# =========================

release-src: clean
	@echo "Creating source distribution..."
	cd .. && tar --exclude='.git' \
		--exclude='*.o' \
		--exclude='.vscode' \
		--exclude='*.swp' \
		--exclude='.DS_Store' \
		--exclude='*.tar.gz' \
		--exclude='labs_backup' \
		--exclude='resize.log' \
		--exclude='*.log' \
		--exclude='src/log' \
		--exclude='src/config/comparatorlogs.txt' \
		--exclude='labs/output.txt' \
		--transform='s,^$(PROJECT_NAME),$(RELEASE_NAME),' \
		-czf $(RELEASE_NAME)-source.tar.gz $(PROJECT_NAME)/
	@echo "Source tarball created: ../$(RELEASE_NAME)-source.tar.gz"

# =========================
# x86_64 binary release
# =========================

release-bin86: clean all
	@echo "Creating x86_64 binary release..."
	mkdir -p dist/$(BIN_RELEASE_X86)

	cp $(TARGET) dist/$(BIN_RELEASE_X86)/
	cp -r labs dist/$(BIN_RELEASE_X86)/
	cp README.md dist/$(BIN_RELEASE_X86)/
	cp LICENSE dist/$(BIN_RELEASE_X86)/ 2>/dev/null || true

	cd dist && tar -czf ../$(BIN_RELEASE_X86).tar.gz $(BIN_RELEASE_X86)

	@echo "Binary release created: $(BIN_RELEASE_X86).tar.gz"

# =========================
# Phony targets
# =========================

.PHONY: all clean arm release-src release-bin86 release-binarm
