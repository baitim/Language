OPTIONS = -D _DEBUG -ggdb3 -O0 -Wall -Wextra\
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

INCLUDE_FILES = 	$(wildcard *.h) $(wildcard Errors/*.h) $(wildcard Tree/*.h)
ERROR_FILES = 		$(wildcard Errors/*.c)
TREE_FILES = 		$(wildcard Tree/*.c)

FRONTEND_FILES =			$(wildcard Frontend/*.c)
FRONTEND_INCLUDE_FILES =	$(wildcard Frontend/*.h)

start : build run

build : $(ERROR_FILES) $(TREE_FILES) $(INCLUDE_FILES) $(FRONTEND_FILES) $(FRONTEND_INCLUDE_FILES)
	gcc $(ERROR_FILES) $(TREE_FILES) $(FRONTEND_FILES) -o frontend $(OPTIONS)

run :
	./frontend $(DATA_FILE)