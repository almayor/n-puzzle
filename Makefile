NAME = solver

SRC_NAME = Matrix.cpp aStar.cpp main.cpp Puzzle.cpp

################################################################################

PATHS = src/cpp
PATHO = obj/cpp
PATHI = src/cpp

################################################################################

CC = c++
RM = rm
MKDIR = /bin/mkdir

################################################################################

COMPILE = $(CC) -c
CFLAGS += -Werror -Wextra -Wall -Wno-sign-compare
CFLAGS += -std=c++17 -O2
CFLAGS += -MMD
CFLAGS += $(foreach path, $(PATHI), -I$(path))

LINK = $(CC)

################################################################################

ifeq ($(DEBUG), 1) 
	COMPILE += -g
endif

################################################################################

SRC = $(patsubst %.cpp, $(PATHS)/%.cpp, $(SRC_NAME))
OBJ = $(patsubst %.cpp, $(PATHO)/%.o, $(SRC_NAME))

################################################################################

$(NAME) : $(OBJ)
	$(LINK) $^ -o $@ $(LFLAGS)

$(PATHO)/%.o : $(PATHS)/%.cpp
	$(MKDIR) -p $(@D)
	$(COMPILE) $(CFLAGS) $< -o $@

################################################################################

DEP += $(patsubst %.cpp, $(PATHO)/%.d, $(SRC_NAME))

-include $(DEP)

################################################################################

.DEFAULT_GOAL = all

.PHONY : all clean fclean re docs test

all : $(NAME)

fclean : clean
	$(RM) -f $(NAME)

clean :
	$(RM) -rf $(PATHO)

re : fclean all

################################################################################
