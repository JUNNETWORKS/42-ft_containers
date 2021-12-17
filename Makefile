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

all: $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -MMD -o $@

-include $(DEPENDENCIES)

$(NAME): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $^

clean:
	$(RM) $(OBJECTS) $(DEPENDENCIES)
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

# TEST

TEST_DIR  := test

test_all: test_vector

test_vector: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -o $@ $^ $(TEST_DIR)/vector_test.cpp
	./$@
	@$(RM) $@

.PHONY: all clean fclean re