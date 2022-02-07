CXX      := clang++
CXXFLAGS := -Wall -Wextra -Werror
CXXFLAGS += -std=c++98 -g -fsanitize=address
OBJ_DIR  := objs
NAME     := ft_containers

SRC_DIR  := srcs
SRCS     := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS  := $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
DEPENDENCIES \
         := $(OBJECTS:.o=.d)

.PHONY: all
all: $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -MMD -o $@

-include $(DEPENDENCIES)

$(NAME): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $^

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(DEPENDENCIES)
	$(RM) -r $(OBJ_DIR)

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)

.PHONY: re
re: fclean all

.PHONY: debug
debug: $(OBJECTS)
	$(CXX) -g -O0 $^ -o debug
	gdb

# Google Test

GTESTDIR    :=   ./google_test
GTEST       :=   $(GTESTDIR)/gtest $(GTESTDIR)/googletest-release-1.11.0

TEST_DIR := ./test
SRCS_TEST := $(TEST_DIR)/vector_test.cpp  \
	$(TEST_DIR)/type_traits_test.cpp \
	$(TEST_DIR)/lexicographical_compare_test.cpp \
	$(TEST_DIR)/stack_test.cpp \
	$(TEST_DIR)/pair_test.cpp \
	$(TEST_DIR)/red_black_tree_test.cpp \
	$(TEST_DIR)/map_test.cpp \
	$(TEST_DIR)/set_test.cpp
SRCS_TEST_UTILS := $(wildcard $(TEST_DIR)/utils/*.cpp)
OBJECTS_TEST_UTILS  := $(SRCS_TEST_UTILS:%.cpp=$(OBJ_DIR)/%.o)
OBJECTS_TEST  := $(SRCS_TEST:%.cpp=$(OBJ_DIR)/%.o)
DEPENDENCIES_TEST \
         := $(OBJECTS_TEST:.o=.d)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	g++ -g -Wall -Wextra -Werror -DDEBUG --std=c++11 -I$(GTESTDIR) -I$(SRC_DIR) -I$(TEST_DIR) \
	-c $< -MMD -o $@

-include $(DEPENDENCIES_TEST)

$(GTEST):
	mkdir -p $(GTESTDIR)
	curl -OL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz
	tar -xvzf release-1.11.0.tar.gz googletest-release-1.11.0
	rm -rf release-1.11.0.tar.gz
	python googletest-release-1.11.0/googletest/scripts/fuse_gtest_files.py $(GTESTDIR)
	mv googletest-release-1.11.0 $(GTESTDIR)

.PHONY: test
test: $(OBJECTS_TEST) $(OBJECTS_TEST_UTILS)
	# Google Test require C++11
	g++ -Wall -Wextra -Werror -std=c++11 $(GTESTDIR)/googletest-release-1.11.0/googletest/src/gtest_main.cc $(GTESTDIR)/gtest/gtest-all.cc \
	-DDEBUG -g -fsanitize=address \
	-I$(GTESTDIR) -I$(SRC_DIR) -lpthread $(OBJECTS_TEST) $(OBJECTS_TEST_UTILS) -o tester
	./tester

coverage:
	# Google Test require C++11
	g++ -Wall -Wextra -Werror -std=c++11 $(GTESTDIR)/googletest-release-1.11.0/googletest/src/gtest_main.cc $(GTESTDIR)/gtest/gtest-all.cc \
	-DDEBUG -g -fsanitize=address \
	-ftest-coverage -fprofile-arcs -lgcov \
	-I$(GTESTDIR) -I$(SRC_DIR) -lpthread $(SRCS_TEST) $(SRCS_TEST_UTILS) -o tester
	./tester
	# coverage
	lcov -c -b . -d . -o cov_test.info --gcov-tool /usr/bin/gcov-8
	lcov -r cov_test.info "*/google_test/*" "*/c++/*" -o coverageFiltered.info --gcov-tool /usr/bin/gcov-8
	genhtml coverageFiltered.info -o cov_test_html
	rm *.gcda
	rm *.gcno
	rm coverageFiltered.info
	rm cov_test.info