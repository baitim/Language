OPTIONS = -lm -D _DEBUG -ggdb3 -O0 -Wall -Wextra\
-Waggressive-loop-optimizations -Wmissing-declarations -Wcast-align\
-Wcast-qual -Wchar-subscripts -Wconversion\
-Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness\
-Wformat=2 -Winline -Wlogical-op -Wopenmp-simd\
-Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion\
-Wstrict-overflow=2 -Wsuggest-attribute=noreturn\
-Wsuggest-final-methods -Wsuggest-final-types -Wswitch-default\
-Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused\
-Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing\
-Wno-varargs -Wstack-protector -fcheck-new\
-fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer\
-Wlarger-than=131072 -Wstack-usage=131072 -pie -fPIE -Werror=vla\
-Itests -Isrc\
-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

INCLUDE_FILES = 	$(wildcard *.h) $(wildcard */*.h)
ERROR_FILES = 		$(wildcard Errors/*.c)
TREE_FILES = 		$(wildcard Tree/*.c)
MATH_FILES = 		$(wildcard Math/*.c)
PROCESS_CMD_FILES = $(wildcard ProcessCmd/*.c)
NAME_TABLE_FILES = 	$(wildcard NameTable/*.c)

FRONTEND_FILES =			$(wildcard Frontend/*.c)
FRONTEND_INCLUDE_FILES =	$(wildcard Frontend/*.h)

ALL_FRONTEND_FILES = $(FRONTEND_FILES) $(ERROR_FILES) $(TREE_FILES) $(MATH_FILES) $(PROCESS_CMD_FILES) $(NAME_TABLE_FILES)
ALL_FRONTEND_INCLUDE_FILES = $(ALL_FRONTEND_FILES) $(INCLUDE_FILES)

FRONTEND_FLAGS = --name_data_file txt/program1

frontend_start : frontend_build frontend_run

frontend_build : $(ALL_FRONTEND_FILES) $(ALL_FRONTEND_INCLUDE_FILES)
	gcc $(ALL_FRONTEND_FILES) -o frontend $(OPTIONS)

frontend_run :
	./frontend $(FRONTEND_FLAGS) $(DATA_FILE)