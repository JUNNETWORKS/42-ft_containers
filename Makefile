
$(info MAKECMDGOALS: $(strip $(MAKECMDGOALS)))
ifneq ($(filter $(strip $(MAKECMDGOALS)),googletest coverage),)
# GoogleTest, coverage の時は g++ を使う
CXX      := g++
else
CXX      := clang++
endif
$(info compiler: $(CXX))

ifneq ($(filter $(strip $(MAKECMDGOALS)),googletest coverage test),)
CXXFLAGS += -DDEBUG -g -fsanitize=address
endif

ifneq ($(filter $(strip $(MAKECMDGOALS)),googletest coverage),)
CXXFLAGS += -std=c++11

ifeq ($(shell uname -s),Linux)
CXXFLAGS += -ftest-coverage -fprofile-arcs -lgcov
endif

else
# GoogleTest などはC++11を用いるため、-Wall などのフラグはつけない(deprecated errorなどが出る)
CXXFLAGS += -std=c++98
CXXFLAGS := -Wall -Wextra -Werror
endif

INCLUDES_DIR  := includes
CXXFLAGS += -I$(INCLUDES_DIR)
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
	$(CXX) -g -Wall -Wextra -Werror -DDEBUG --std=c++98 -I$(INCLUDES_DIR) -I$(TEST_DIR) \
	-c $< -MMD -o $@

.PHONY: test
test: $(TEST_UTIL_OBJECTS)
	$(CXX) $(CXXFLAGS) -I$(INCLUDES_DIR) -I$(TEST_DIR) test/testlib/testlib_main.cpp $(TEST_UTIL_OBJECTS) -o $(TESTER_NAME)
	$(TESTER_NAME)

############ GooleTest ############

GTEST_DIR   :=   ./google_test
GTEST       :=   $(GTEST_DIR)/gtest $(GTEST_DIR)/googletest-release-1.11.0
GTEST_MAIN  := $(GTEST_DIR)/googletest-release-1.11.0/googletest/src/gtest_main.cc
GTEST_ALL   := $(GTEST_DIR)/gtest/gtest-all.cc

TEST_SRCS := $(TEST_DIR)/vector_test.cpp \
	$(TEST_DIR)/type_traits_test.cpp \
	$(TEST_DIR)/lexicographical_compare_test.cpp \
	$(TEST_DIR)/equal_test.cpp \
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
	$(CXX) $(CXXFLAGS) -I$(GTEST_DIR) -I$(INCLUDES_DIR) -I$(TEST_DIR) \
	-c $< -MMD -o $@

$(GTEST):
	mkdir -p $(GTEST_DIR)
	curl -OL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz
	tar -xvzf release-1.11.0.tar.gz googletest-release-1.11.0
	rm -rf release-1.11.0.tar.gz
	python googletest-release-1.11.0/googletest/scripts/fuse_gtest_files.py $(GTEST_DIR)
	mv googletest-release-1.11.0 $(GTEST_DIR)

.PHONY: googletest
googletest: $(GTEST) $(TEST_OBJECTS) $(TEST_UTIL_OBJECTS)
	# Google Test require C++11
	$(CXX) $(CXXFLAGS) $(GTEST_MAIN) $(GTEST_ALL) \
	-I$(GTEST_DIR) -I$(INCLUDES_DIR) -lpthread $(TEST_OBJECTS) $(TEST_UTIL_OBJECTS) -o $(TESTER_NAME)
	$(TESTER_NAME)

.PHONY: coverage
coverage: $(GTEST)
	# Google Test require C++11
	g++ $(CXXFLAGS) $(GTEST_MAIN) $(GTEST_ALL) \
	-I$(GTEST_DIR) -I$(INCLUDES_DIR) -lpthread $(TEST_SRCS) $(TEST_UTIL_SRCS) -o $(TESTER_NAME)
	$(TESTER_NAME)
	# coverage
	lcov -c -b . -d . -o cov_test.info --gcov-tool /usr/bin/gcov-8
	lcov -r cov_test.info "*/google_test/*" "*/c++/*" -o coverageFiltered.info --gcov-tool /usr/bin/gcov-8
	genhtml coverageFiltered.info -o cov_test_html
	rm *.gcda
	rm *.gcno
	rm coverageFiltered.info
	rm cov_test.info