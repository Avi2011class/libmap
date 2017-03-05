CCOMPILER = gcc-6
CPPCOMPLIER = g++-6
LINKER = g++-6
LINKER_OPTIONS = -lgtest -pthread
COMPILER_OPTIONS = -O3 -Wall -Wpedantic -Werror

BIN_PATH = bin/
SRC_PATH = src/
TST_PATH = unittests/
OBJ_PATH = object/

all: libmap.a

# binary test file building -> bin/gtest
gtest: $(OBJ_PATH)gtest.o libmap.a $(OBJ_PATH)tree_unittests.o $(OBJ_PATH)hash_unittests.o $(OBJ_PATH)hash2_unittests.o
	$(LINKER) $(OBJ_PATH)gtest.o $(OBJ_PATH)tree_unittests.o $(OBJ_PATH)hash_unittests.o $(OBJ_PATH)hash2_unittests.o libmap.a $(LINKER_OPTIONS) -o $(BIN_PATH)gtest 


$(OBJ_PATH)gtest.o: $(TST_PATH)gtest.cpp $(SRC_PATH)header.h
	$(CPPCOMPLIER) $(OPT) $(TST_PATH)gtest.cpp -c -o $(OBJ_PATH)gtest.o

$(OBJ_PATH)tree_unittests.o: $(TST_PATH)tree_unittests.c $(SRC_PATH)header.h
	$(CPPCOMPLIER) $(OPT) $(TST_PATH)tree_unittests.c -c -o $(OBJ_PATH)tree_unittests.o

$(OBJ_PATH)hash_unittests.o: $(TST_PATH)hash_unittests.c $(SRC_PATH)header.h
	$(CPPCOMPLIER) $(OPT) $(TST_PATH)hash_unittests.c -c -o $(OBJ_PATH)hash_unittests.o

$(OBJ_PATH)hash2_unittests.o: $(TST_PATH)hash2_unittests.c $(SRC_PATH)header.h
	$(CPPCOMPLIER) $(OPT) $(TST_PATH)hash2_unittests.c -c -o $(OBJ_PATH)hash2_unittests.o



$(OBJ_PATH)libcore.o: $(SRC_PATH)libcore.c $(SRC_PATH)header.h $(SRC_PATH)macro.h
	$(CCOMPILER) $(OPT) $(SRC_PATH)libcore.c -c -o $(OBJ_PATH)libcore.o
	
$(OBJ_PATH)avl_tree.o: $(SRC_PATH)avl_tree.c $(SRC_PATH)header.h $(SRC_PATH)macro.h
	$(CCOMPILER) $(OPT) $(SRC_PATH)avl_tree.c -c -o $(OBJ_PATH)avl_tree.o

$(OBJ_PATH)hash_table.o: $(SRC_PATH)hash_table.c $(SRC_PATH)header.h $(SRC_PATH)macro.h
	$(CCOMPILER) $(OPT) $(SRC_PATH)hash_table.c -c -o $(OBJ_PATH)hash_table.o

$(OBJ_PATH)hash-table-2.o: $(SRC_PATH)hash-table-2.c $(SRC_PATH)header.h $(SRC_PATH)macro.h
	$(CCOMPILER) $(OPT) $(SRC_PATH)hash-table-2.c -c -o $(OBJ_PATH)hash-table-2.o



libmap.a: $(OBJ_PATH)libcore.o $(OBJ_PATH)avl_tree.o $(OBJ_PATH)hash_table.o $(OBJ_PATH)hash-table-2.o
	ar -cr libmap.a $(OBJ_PATH)libcore.o $(OBJ_PATH)avl_tree.o $(OBJ_PATH)hash_table.o $(OBJ_PATH)hash-table-2.o


