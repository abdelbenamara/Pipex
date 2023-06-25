# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/24 00:14:17 by abenamar          #+#    #+#              #
#    Updated: 2023/06/25 19:20:38 by abenamar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

PROJECT_DIR := $(CURDIR)/..

PROGRAM := $(PROJECT_DIR)/pipex

COMMAND := $(PROGRAM)

VALGRIND := valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes

VFURMANE_OPTS := -l

RM := rm -f

all: norm $(PROGRAM) test bonus community-tests

$(PROGRAM):
	@make -C $(PROJECT_DIR)

norm:
	@echo "\033[0;36m######################################## norminette ########################################\033[0m"
	@@cd $(PROJECT_DIR) && norminette $$(ls | grep "\.c\|\.h") && norminette $$(ls -d */ | grep -v 'tests/')

outfiles:
	@mkdir outfiles

myecho:
	@cc myecho.c -o myecho

valgrind:
	@echo "\033[0;36m######################################### valgrind #########################################\033[0m"

test: $(PROGRAM) outfiles myecho
	@echo "\033[0;36m########################################### test ###########################################\033[0m"
	@testfunc () { \
	$(COMMAND) $$2 "$$3" "$$4" outfiles/actual$$1;\
	actual=$$?;\
	< $$2 $$3 | $$4 > outfiles/expected$$1;\
	expected=$$?;\
	result="\033[0;32m[OK]\033[0m";\
	if [ "$$(diff outfiles/actual$$1 outfiles/expected$$1)" != "" ] || [ $$actual -ne $$expected ]; then\
		result="\033[0;31m[KO]\033[0m";\
	fi;\
	case="< $$2 $$3 | $$4 > outfiles/expected$$1";\
	printf "%d) %-84s " "$$1" "$$case";\
	echo $$result;\
	 };\
	testfunc 1 infiles/loremipsum "cat -e" "grep Et";\
	testfunc 2 infiles/loremipsum "grep sit" "wc -l";\
	testfunc 3 infiles/loremipsum "wc -w" "tee";\
	testfunc 4 infiles/loremipsum "ls -ls" "grep my";\
	testfunc 5 infiles/loremipsum "./myscript hello 1 world 2 !" "head -n 8"

valgrind-test: COMMAND := $(VALGRIND) $(PROGRAM)
valgrind-test: valgrind test

bonus: $(PROGRAM) outfiles myecho
	@echo "\033[0;36m########################################### bonus ##########################################\033[0m"

valgrind-bonus: COMMAND := $(VALGRIND) $(PROGRAM)
valgrind-bonus: valgrind bonus

community-tests:
	@if [ ! -d "pipex-tester" ] || [ -z "$$(ls -A pipex-tester)" ]; then \
		git submodule update --init pipex-tester; \
	fi
	@echo "\033[0;36m################################## vfurmane : pipex-tester #################################\033[0m"
	@./pipex-tester/run.sh $(VFURMANE_OPTS) 

clean:
	$(RM) -r outfiles

fclean: clean
	$(RM) myecho
	$(RM) $(PROGRAM)

re: fclean all

.PHONY: re fclean clean community-tests valgrind-bonus bonus valgrind-test test norm all
