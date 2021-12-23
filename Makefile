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

# Google Test

GTESTDIR    :=   ./google_test
GTEST       :=   $(GTESTDIR)/gtest $(GTESTDIR)/googletest-release-1.11.0

TEST_DIR := ./test
SRCS_TEST := $(TEST_DIR)/vector_test.cpp  \
	$(TEST_DIR)/type_traits_test.cpp \
	$(TEST_DIR)/lexicographical_compare_test.cpp \
	$(TEST_DIR)/stack_test.cpp

$(GTEST):
	mkdir -p $(GTESTDIR)
	curl -OL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz
	tar -xvzf release-1.11.0.tar.gz googletest-release-1.11.0
	rm -rf release-1.11.0.tar.gz
	python googletest-release-1.11.0/googletest/scripts/fuse_gtest_files.py $(GTESTDIR)
	mv googletest-release-1.11.0 $(GTESTDIR)

.PHONY: test
test: $(GTEST) 
	# Google Test require C++11
	clang++ -std=c++11 $(GTESTDIR)/googletest-release-1.11.0/googletest/src/gtest_main.cc $(GTESTDIR)/gtest/gtest-all.cc \
	-DDEBUG -g -fsanitize=integer -fsanitize=address \
	-I$(GTESTDIR) -I$(SRC_DIR) -lpthread $(SRCS_TEST) -o tester
	./tester
	rm tester