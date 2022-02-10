CXX      := clang++
CXXFLAGS := -Wall -Wextra -Werror
CXXFLAGS += -std=c++98
SRC_DIR  := srcs
CXXFLAGS += -I$(SRC_DIR)
OBJ_DIR  := objs
NAME     := ft_containers_benchmark     

BM_DIR      := benchmark
BM_SRCS     := $(wildcard $(BM_DIR)/*.cpp)
BM_OBJ_DIR  := $(OBJ_DIR)/$(BM_DIR)
BM_OBJECTS  := $(BM_SRCS:%.cpp=$(OBJ_DIR)/%.o)
DEPENDENCIES \
         := $(BM_OBJECTS:.o=.d)

.PHONY: all
all: $(NAME)

$(BM_OBJ_DIR)/%.o: $(BM_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -MMD -o $@

-include $(DEPENDENCIES)

$(NAME): $(BM_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $^

.PHONY: clean
clean:
	$(RM) $(BM_OBJECTS) $(DEPENDENCIES)
	$(RM) -r $(OBJ_DIR)

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)
	$(RM) $(TESTER_NAME)

.PHONY: re
re: fclean all

.PHONY: debug
debug: $(OBJECTS)
	$(CXX) -g -O0 $^ -o debug
	gdb

############ Test ############

TESTER_NAME := ./tester

TEST_DIR := test
TEST_UTIL_DIR := $(TEST_DIR)/utils
TEST_UTIL_OBJ_DIR := $(OBJ_DIR)/$(TEST_UTIL_DIR)
TEST_UTIL_SRCS := $(wildcard $(TEST_UTIL_DIR)/*.cpp)
TEST_UTIL_OBJECTS  := $(TEST_UTIL_SRCS:%.cpp=$(OBJ_DIR)/%.o)
TEST_UTIL_DEPENDENCIES \
         := $(TEST_UTIL_OBJECTS:.o=.d)

-include $(TEST_UTIL_DEPENDENCIES)

$(TEST_UTIL_OBJ_DIR)/%.o: $(TEST_UTIL_DIR)/%.cpp
	@mkdir -p $(@D)
	clang++ -g -Wall -Wextra -Werror -DDEBUG --std=c++98 -I$(SRC_DIR) -I$(TEST_DIR) \
	-c $< -MMD -o $@

.PHONY: mytest
mytest: $(TEST_UTIL_OBJECTS)
	clang++ -Wall -Wextra -Werror -std=c++98 \
	-DDEBUG -g -fsanitize=address \
	-I$(SRC_DIR) -I$(TEST_DIR) -lpthread test/testlib/testlib_main.cpp $(TEST_UTIL_OBJECTS) -o $(TESTER_NAME)
	$(TESTER_NAME)

############ GooleTest ############

GTESTDIR    :=   ./google_test
GTEST       :=   $(GTESTDIR)/gtest $(GTESTDIR)/googletest-release-1.11.0

TEST_SRCS := $(TEST_DIR)/vector_test.cpp \
	$(TEST_DIR)/type_traits_test.cpp \
	$(TEST_DIR)/lexicographical_compare_test.cpp \
	$(TEST_DIR)/stack_test.cpp \
	$(TEST_DIR)/pair_test.cpp \
	$(TEST_DIR)/red_black_tree_test.cpp \
	$(TEST_DIR)/map_test.cpp \
	$(TEST_DIR)/set_test.cpp
TEST_OBJ_DIR := $(OBJ_DIR)/$(TEST_DIR)
TEST_OBJECTS  := $(TEST_SRCS:%.cpp=$(OBJ_DIR)/%.o)
TEST_DEPENDENCIES \
         := $(TEST_OBJECTS:%.o=%.d)

-include $(TEST_DEPENDENCIES)

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(@D)
	g++ -g -Wall -Wextra -Werror -DDEBUG --std=c++11 -I$(GTESTDIR) -I$(SRC_DIR) -I$(TEST_DIR) \
	-c $< -MMD -o $@

$(GTEST):
	mkdir -p $(GTESTDIR)
	curl -OL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz
	tar -xvzf release-1.11.0.tar.gz googletest-release-1.11.0
	rm -rf release-1.11.0.tar.gz
	python googletest-release-1.11.0/googletest/scripts/fuse_gtest_files.py $(GTESTDIR)
	mv googletest-release-1.11.0 $(GTESTDIR)

.PHONY: test
test: $(TEST_OBJECTS) $(TEST_UTIL_OBJECTS)
	# Google Test require C++11
	g++ -Wall -Wextra -Werror -std=c++11 $(GTESTDIR)/googletest-release-1.11.0/googletest/src/gtest_main.cc $(GTESTDIR)/gtest/gtest-all.cc \
	-DDEBUG -g -fsanitize=address \
	-I$(GTESTDIR) -I$(SRC_DIR) -lpthread $(TEST_OBJECTS) $(TEST_UTIL_OBJECTS) -o $(TESTER_NAME)
	$(TESTER_NAME)

.PHONY: coverage
coverage:
	# Google Test require C++11
	g++ -Wall -Wextra -Werror -std=c++11 $(GTESTDIR)/googletest-release-1.11.0/googletest/src/gtest_main.cc $(GTESTDIR)/gtest/gtest-all.cc \
	-DDEBUG -g -fsanitize=address \
	-ftest-coverage -fprofile-arcs -lgcov \
	-I$(GTESTDIR) -I$(SRC_DIR) -lpthread $(TEST_SRCS) $(TEST_UTIL_SRCS) -o tester
	./tester
	# coverage
	lcov -c -b . -d . -o cov_test.info --gcov-tool /usr/bin/gcov-8
	lcov -r cov_test.info "*/google_test/*" "*/c++/*" -o coverageFiltered.info --gcov-tool /usr/bin/gcov-8
	genhtml coverageFiltered.info -o cov_test_html
	rm *.gcda
	rm *.gcno
	rm coverageFiltered.info
	rm cov_test.info