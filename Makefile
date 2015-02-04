
# The programs to build.
TARGETS = server deliver

# The source files.
SRCS = server.c deliver.c

# Compile flags.
CFLAGS = -g -Wall
LDFLAGS = -g -Wall 

# Dependencies file
DEPEND_FILE = depend.mk
CC=    gcc
YYFLAGS= -v  -d
INCLUDE = -I.

# ---------------------------------------------------------
# Targets to build the storage client and server.
# ---------------------------------------------------------

# Default targets.
build: $(TARGETS)

# Build the server.
server: server.o
	$(CC) $(LDFLAGS) $^ -o $@

# Build the client.
deliver: deliver.o
	$(CC) $(LDFLAGS) $^ -o $@


# Compile a .c source file to a .o object file.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Delete generated files.
clean:
	-rm -rf $(TARGETS) *.o tags $(DEPEND_FILE)

# Create dependencies file.
depend:
	$(CC) $(CFLAGS) -M $(SRCS) > $(DEPEND_FILE)

-include $(DEPEND_FILE)


# ---------------------------------------------------------
# Targets to help work with the code.
# ---------------------------------------------------------

# Create tags file to help with code navigation in some editors.
tags: *.c *.h
	ctags -R .

# Create tags file including system header files.
fulltags: *.c *.h
	ctags -R . `$(MAKE) -s print_include_dirs`

# Print the include directories for current system.
print_include_dirs:
	@gcc -x c -Wp,-v -E - < /dev/null 2>&1 |grep '^ '

.PHONY: build depend clean tags fulltags print_include_dirs



