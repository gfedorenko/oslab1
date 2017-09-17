
CC                    := gcc

RM                    := rm -rf

LDFLAGS            += -Llib

CFLAGS            += -Iinclude

SRCS                := $(wildcard src/*.c)

SRCS_TEST1						:= $(wildcard src/exec.c src/polling.c test/test1.c)

SRCS_TEST2						:= $(wildcard src/exec.c src/polling.c test/test2.c)

SRCS_TEST3  					:= $(wildcard src/exec.c src/polling.c test/test3.c)

SRCS_TEST4						:= $(wildcard src/exec.c src/polling.c test/test4.c)

SRCS_TEST5						:= $(wildcard src/exec.c src/polling.c test/test5.c)

SRCS_TEST6						:= $(wildcard src/exec.c src/polling.c test/test6.c)

OBJS                := $(SRCS:.c=.o)

OBJS_TEST1           := $(SRCS_TEST1:.c=.o)

OBJS_TEST2           := $(SRCS_TEST2:.c=.o)

OBJS_TEST3           := $(SRCS_TEST3:.c=.o)

OBJS_TEST4           := $(SRCS_TEST4:.c=.o)

OBJS_TEST5           := $(SRCS_TEST5:.c=.o)

OBJS_TEST6           := $(SRCS_TEST6:.c=.o)

NAME                := lab1

NAME_TEST1						:= test_lab1

NAME_TEST2						:= test_lab2

NAME_TEST3						:= test_lab3

NAME_TEST4						:= test_lab4

NAME_TEST5						:= test_lab5

NAME_TEST6						:= test_lab6

all:                $(NAME)

$(NAME):        $(OBJS)
											 $(CC) -o $(NAME) $(OBJS) $(LDFLAGS)

$(NAME_TEST1):      $(OBJS_TEST1)
											 $(CC) -o $(NAME_TEST1) $(OBJS_TEST1) $(LDFLAGS)

$(NAME_TEST2):      $(OBJS_TEST2)
											 	$(CC) -o $(NAME_TEST2) $(OBJS_TEST2) $(LDFLAGS)

$(NAME_TEST3):      $(OBJS_TEST3)
										   	$(CC) -o $(NAME_TEST3) $(OBJS_TEST3) $(LDFLAGS)

$(NAME_TEST4):      $(OBJS_TEST4)
												$(CC) -o $(NAME_TEST4) $(OBJS_TEST4) $(LDFLAGS)

$(NAME_TEST5):      $(OBJS_TEST5)
											 	$(CC) -o $(NAME_TEST5) $(OBJS_TEST5) $(LDFLAGS)

$(NAME_TEST6):      $(OBJS_TEST6)
											 	$(CC) -o $(NAME_TEST6) $(OBJS_TEST6) $(LDFLAGS)

clean:
											 $(RM) $(OBJS)

fclean:         clean
											 $(RM) $(NAME)

re_test:                 fclean

re:                 fclean all

test1:                re_test $(NAME_TEST1)
											 @(./$(NAME_TEST1) && echo "Test success" || echo "Test Fails")

test2:                re_test $(NAME_TEST2)
											 @(./$(NAME_TEST2) && echo "Test success" || echo "Test Fails")

test3:                re_test $(NAME_TEST3)
										   @(./$(NAME_TEST3) && echo "Test success" || echo "Test Fails")

test4:                re_test $(NAME_TEST4)
				 							 @(./$(NAME_TEST4) && echo "Test success" || echo "Test Fails")

test5:                re_test $(NAME_TEST5)
				 						   @(./$(NAME_TEST5) && echo "Test success" || echo "Test Fails")

test6:                re_test $(NAME_TEST6)
				 						   @(./$(NAME_TEST6) && echo "Test success" || echo "Test Fails")
