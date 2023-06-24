# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/24 00:14:17 by abenamar          #+#    #+#              #
#    Updated: 2023/06/24 23:43:32 by abenamar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

PROJECT_DIR := ..

PROGRAM := $(PROJECT_DIR)/pipex

COMMAND := $(PROGRAM)

VALGRIND := valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes

all: norm $(PROGRAM) vmandatory

$(PROGRAM):
	@make -C $(PROJECT_DIR)

norm:
	@echo "\033[0;36m######################################## norminette ########################################\033[0m"
	@norminette ..

outfiles:
	@mkdir outfiles

myecho:
	@cc myecho.c -o myecho

valgrind:
	@echo "\033[0;36m######################################### valgrind #########################################\033[0m"

vmandatory: COMMAND := $(VALGRIND) $(PROGRAM)
vmandatory: valgrind mandatory

mandatory: $(PROGRAM) outfiles myecho
	@echo "\033[0;36m######################################### mandatory ########################################\033[0m"
	@testfunc () { \
	actual="$(COMMAND) $$2 \"$$3\" \"$$4\" outfiles/actual$$1";\
	expected="< $$2 $$3 | $$4 > outfiles/expected$$1";\
	echo "$$1) $$expected";\
	eval $$actual;\
	eval $$expected;\
	if [ "$$(diff outfiles/actual$$1 outfiles/expected$$1)" != "" ]; then\
		echo "\033[0;31mKO\033[0m";\
	else\
		echo "\033[0;32mOK\033[0m";\
	fi };\
	testfunc 1 infiles/loremipsum "cat -e" "grep Et";\
	testfunc 2 infiles/loremipsum "grep sit" "wc -l";\
	testfunc 3 infiles/loremipsum "wc -w" "tee";\
	testfunc 4 infiles/loremipsum "ls -ls" "grep my";\
	testfunc 5 infiles/loremipsum "./myscript hello 1 world 2 !" "head -n 8"
