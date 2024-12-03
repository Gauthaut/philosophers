CC = gcc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

PHILO_DIR = philo
PHILO_BONUS_DIR = philo_bonus

PHILO_SRC_DIR = $(PHILO_DIR)/src
PHILO_INC_DIR = $(PHILO_DIR)/includes

PHILO_BONUS_SRC_DIR = $(PHILO_BONUS_DIR)/src
PHILO_BONUS_INC_DIR = $(PHILO_BONUS_DIR)/includes

PHILO_SRCS = $(PHILO_SRC_DIR)/actions.c $(PHILO_SRC_DIR)/exit.c \
             $(PHILO_SRC_DIR)/parsing.c $(PHILO_SRC_DIR)/philo.c \
             $(PHILO_SRC_DIR)/utils.c
PHILO_OBJS = $(PHILO_SRCS:.c=.o)

PHILO_BONUS_SRCS = $(PHILO_BONUS_SRC_DIR)/action_bonus.c \
                   $(PHILO_BONUS_SRC_DIR)/monitor_bonus.c \
                   $(PHILO_BONUS_SRC_DIR)/parsing_bonus.c \
                   $(PHILO_BONUS_SRC_DIR)/philo_bonus.c \
                   $(PHILO_BONUS_SRC_DIR)/utils.c
PHILO_BONUS_OBJS = $(PHILO_BONUS_SRCS:.c=.o)

PHILO_NAME = philosopher
PHILO_BONUS_NAME = philosopher_bonus

all: $(PHILO_NAME)

$(PHILO_NAME): $(PHILO_OBJS)
	$(CC) $(CFLAGS) -I$(PHILO_INC_DIR) -o $@ $^

bonus: $(PHILO_BONUS_NAME)

$(PHILO_BONUS_NAME): $(PHILO_BONUS_OBJS)
	$(CC) $(CFLAGS) -I$(PHILO_BONUS_INC_DIR) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -I$(PHILO_INC_DIR) -I$(PHILO_BONUS_INC_DIR) -c $< -o $@

clean:
	$(RM) $(PHILO_OBJS) $(PHILO_BONUS_OBJS)

fclean: clean
	$(RM) $(PHILO_NAME) $(PHILO_BONUS_NAME)

re: fclean all

.PHONY: all bonus clean fclean re
