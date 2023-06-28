# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/24 00:14:17 by abenamar          #+#    #+#              #
#    Updated: 2023/06/28 14:22:38 by abenamar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

PROJECT_DIR := $(CURDIR)/..

PROGRAM := $(PROJECT_DIR)/pipex

COMMAND := $(PROGRAM)

LEAK_STATUS := 240

VALGRIND := valgrind
VALGRIND += --leak-check=full
VALGRIND += --show-leak-kinds=all
VALGRIND += --track-origins=yes
VALGRIND += --quiet
VALGRIND += --errors-for-leak-kinds=all
VALGRIND += --error-exitcode=$(LEAK_STATUS)

RM := rm -f

.ONESHELL:

all: norm $(PROGRAM) valgrind-test valgrind-bonus community-tests

$(PROGRAM):
	@make -C $(PROJECT_DIR)

norm:
	@echo "\033[0;36m######################################## norminette ########################################\033[0m"
	@@cd $(PROJECT_DIR) && norminette $$(ls | grep "\.c\|\.h") && norminette $$(ls -d */ | grep -v 'tests/')

out:
	@chmod 200 cannotread
	@mkdir out

myecho:
	@cc myecho.c -o myecho

valgrind:
	@echo "\033[0;36m######################################### valgrind #########################################\033[0m"

test: $(PROGRAM) out myecho
	@echo "\033[0;36m########################################### test ###########################################\033[0m"
	@testfunc () { \
	eval "< $$2 $$3 | $$4 > out/expected$$1" 2> /dev/null;\
	$(COMMAND) $$2 "$$3" "$$4" out/actual$$1 2> /dev/null;\
	status=$$?;\
	result="\033[0;32m[OK]\033[0m";\
	if [ "$$(diff out/actual$$1 out/expected$$1)" != "" ] || [ $$status -lt $$5 ] || [ $$status -gt $$6 ] || [ $$status -eq $(LEAK_STATUS) ]; then\
		result="\033[0;31m[KO]\033[0m";\
	fi;\
	case="< $$2 $$3 | $$4 > out/expected$$1";\
	printf "%2d) %-80s " "$$1" "$$case";\
	echo $$result;\
	 };\
	testfunc 1 loremipsum "cat -e" "grep Et" 0 0;\
	testfunc 2 loremipsum "tail -n 16" "wc -l" 0 0;\
	testfunc 3 loremipsum "wc -w" "tee" 0 0;\
	testfunc 4 loremipsum "ls -ls" "grep my" 0 0;\
	testfunc 5 loremipsum "./myscript hello 1 world 2 !" "head -n 5" 0 0;\
	testfunc 6 loremipsum "pwd" "tr / \|" 0 0;\
	testfunc 7 /dev/null "cat" "wc -w" 0 0;\
	testfunc 8 loremipsum "hello" "uname" 0 0;\
	testfunc 9 /dev/null "df" "hello" 1 127;\
	testfunc 10 loremipsum "grep ipsum" "myecho.c" 1 127;\
	testfunc 11 /dev/null "myecho those are arguments ... bye bye" "grep e" 1 127;\
	testfunc 12 /dev/null "./myecho.c not sure about this one" "grep o" 1 127;\
	testfunc 13 /dev/null "./myecho this time it should work" "grep i" 0 0;\
	MYVAR="hello world!" testfunc 14 /dev/null "env" "grep MYVAR" 0 0;\
	PATH=$$PWD:$$PATH testfunc 15 /dev/null "myecho parent dir was added to PATH" "tee" 0 0;\
	testfunc 16 nofile "cat" "tee" 0 0;\
	testfunc 17 cannotread "cat" "tee" 0 0;\
	testfunc 18 loremipsum "tail -n 16" "grep -e '^Ut .*\.$$'" 0 0;\
	testfunc 19 loremipsum "head -n 16" "sed -e \"s/dolor/it's painful/g\"" 0 0;\
	testfunc 20 loremipsum "grep '.\+up.\+'" "sed \"s/\(up\)/what's '\1' \?/g\"" 0 0

valgrind-test: COMMAND := $(VALGRIND) $(PROGRAM)
valgrind-test: valgrind test

bonus: $(PROGRAM) out myecho
	@echo "\033[0;36m########################################### bonus ##########################################\033[0m"
	@testfunc () { \
	echo "\033[0;33mMultiple pipes :\033[0m";\
	eval "< $$2 $$3 | $$4 | $$5 | $$6 > out/expected$$1" 2> /dev/null;\
	$(COMMAND) $$2 "$$3" "$$4" "$$5" "$$6" out/actual$$1 2> /dev/null;\
	status=$$?;\
	result="\033[0;32m[OK]\033[0m";\
	if [ "$$(diff out/actual$$1 out/expected$$1)" != "" ] || [ $$status -lt $$7 ] || [ $$status -gt $$8 ] || [ $$status -eq $(LEAK_STATUS) ]; then\
		result="\033[0;31m[KO]\033[0m";\
	fi;\
	case="< $$2 $$3 | $$4 | $$5 | $$6 > out/expected$$1";\
	printf "%2d) %-80s " "$$1" "$$case";\
	echo $$result;\
	 };\
	testfunc 21 loremipsum "cat -e" "grep -e '^[M-Z].\+aut'" "tr o q" "tee" 0 0
	@testfunc () { \
	echo "\033[0;33m'<<' and '>>' :\033[0m";\
	./here_doc.sh "$(COMMAND)";\
	status=$$?;\
	result="\033[0;32m[OK]\033[0m";\
	if [ "$$(diff out/actual$$1 out/expected$$1)" != "" ] || [ $$status -lt $$2 ] || [ $$status -gt $$3 ] || [ $$status -eq $(LEAK_STATUS) ]; then\
		result="\033[0;31m[KO]\033[0m";\
	fi;\
	case="grep -e '^hello' -e bye << BYE | sed \"s/you/they/g\" >> out/expected$$1";\
	printf "%2d) %-80s " "$$1" "$$case";\
	echo $$result;\
	 };\
	testfunc 22 0 0

valgrind-bonus: COMMAND := $(VALGRIND) $(PROGRAM)
valgrind-bonus: valgrind bonus

community-tests:
	@if [ ! -d "pipex-tester" ] || [ -z "$$(ls -A pipex-tester)" ]; then \
		git submodule update --init pipex-tester; \
	fi
	@echo "\033[0;36m################################## vfurmane : pipex-tester #################################\033[0m"
	@./pipex-tester/run.sh

clean:
	@chmod 600 cannotread
	$(RM) -r out

fclean: clean
	$(RM) myecho
	$(RM) $(PROGRAM)

re: fclean all

.PHONY: re fclean clean community-tests valgrind-bonus bonus valgrind-test test norm all
