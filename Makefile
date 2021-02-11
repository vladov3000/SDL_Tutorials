#PROGS specifies what programs to compile
PROGS = \
	01_hello_SDL \
	02_getting_an_image_on_the_screen \
	03_event_driven_programming \
	04_key_presses \
	05_optimized_surface_loading_and_soft_stretching \
	06_extension_libraries_and_loading_other_image_formats \
	07_texture_loading_and_rendering \
	11_clip_rendering_and_sprite_sheets

#OBJS specifies which files to compile as part of the project
OBJS = $(join $(PROGS),$(addprefix /,$(PROGS)))
all: $(OBJS)

#CC specifies which compiler we're using
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options we're using
# See https://fastcompression.blogspot.com/2019/01/compiler-warnings.html
# -Wall base warnings
# -Wextra extra warnings
# -Wcast-qual enforce only casting const ptr to const ptrs
# -Wcast-align type must be stored at an address suitable for its alignment restriction
# -Wstrict-aliasing=1 2 pointers of different types never reference the same address space
# -Wpointer-arith no pointer arithmetic on void* or function ptrs
# -Winit-self cannot init vairable using itself e.g. `int i = i+1` is invalid
# -Wshadow no name shadowing in nested scopes
# -Wswitch-enum all cases for a switch on an enum must be covered
# -Wstrict-prototypes all parameters to function must have type declared
# -Wmissing-prototypes any non-static function must have declaration
# -Wredundant-decls ensure prototype is only declared once
# -Wfloat-equal no equality operator when comparing floats
# -Wundef forbids evaluation of a macro symbol that’s not defined
# -Wformat=2 tracks printf() issues which can be abused to create security hazards
# -Wvla prevents usage of Variable Length Array
# -Wdeclaration-after-statement keep all declerations at the top of the file
# -Wc++-compat source can be compiled unmodified as both valid C and C++ code
# -Wconversion flag any silent type conversion which is not risk free
COMPILER_FLAGS = \
	-Wall \
	-Wextra \
	-Wcast-qual \
	-Wcast-align \
	-Wstrict-aliasing=1 \
	-Wpointer-arith \
	-Winit-self \
	-Wshadow \
	-Wswitch-enum \
	-Wstrict-prototypes \
	-Wmissing-prototypes \
	-Wredundant-decls \
	-Wfloat-equal \
	-Wformat=2 \
	-Wvla \
	-Wdeclaration-after-statement \
	-Wc++-compat \
	-Wconversion

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -F /Library/Frameworks -lSDL2 -lSDL2_image 

#OUTPUT specifies folder to put output binary
OUTPUT = ./out

#This is the target that compiles our executable
$(OBJS): %: %.c
	mkdir -p $(OUTPUT)
	$(CC) $< $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OUTPUT)/$(basename $(notdir $<))
