#!/bin/bash

MINISHELL="./minishell"
BASH="/bin/bash"
TMP_OUT=".test_output"
TMP_EXPECTED=".test_expected"
CRASH_FLAG=0
INCOMPLETE_FLAG=0

print_test() {
	echo -e "\033[1;36m[TEST]\033[0m $1"
}

check_output() {
	# Strip 'minishell$' prompt from output before diffing
	sed '/^minishell\$ exit$/d; s/^minishell\$ //' "$TMP_OUT" > .tmp_filtered_out
	diff -u "$TMP_EXPECTED" .tmp_filtered_out
	if [ $? -ne 0 ]; then
		echo -e "\033[1;33m[FAIL]\033[0m Output mismatch"
		INCOMPLETE_FLAG=1
	else
		echo -e "\033[1;32m[PASS]\033[0m"
	fi
	rm -f .tmp_filtered_out
}

check_crash() {
	if [ $? -ne 0 ]; then
		echo -e "\033[1;31m[CRASH]\033[0m Shell crashed"
		CRASH_FLAG=1
	fi
}

run_test() {
		local input="$1"
	$MINISHELL > $TMP_OUT 2>&1 <<EOF
$input
exit
EOF

	$BASH > $TMP_EXPECTED 2>&1 <<EOF
$input
exit
EOF
input_escaped=$(printf "%s" "$input" | sed 's/[^^]/[&]/g; s/\^/\\^/g')
sed -i "/^$input_escaped$/d" "$TMP_OUT"
sed -i '/^exit$/d' "$TMP_OUT"
	check_output
	check_crash
}

run_test_exit_code() {
	local input="$1"
	local expected=$2

	echo "$input" | $MINISHELL > /dev/null
	code=$?
	if [ "$code" -eq "$expected" ]; then
		echo -e "\033[1;32m[PASS]\033[0m exit code = $code"
	else
		echo -e "\033[1;31m[FAIL]\033[0m exit code = $code, expected $expected"
		INCOMPLETE_FLAG=1
	fi
}

# 🔥 START TESTING

print_test "1. Basic command"
run_test "/bin/ls"

print_test "2. Arguments test"
run_test "/bin/echo hello world"

print_test "3. Echo with -n"
run_test "echo -n Hello"

print_test "4. Exit with code"
run_test "exit 42"
echo "42" > $TMP_EXPECTED; echo $? > $TMP_OUT; check_output

print_test "5. Return value test"
run_test "ls filethatdoesnotexist ; echo \$?"

print_test "6. Simple quotes"
run_test "echo 'hello | world \$USER'"

print_test "7. Double quotes"
run_test "echo \"hello | world \$USER\""

print_test "8. Empty command"
run_test ""

print_test "9. Space-only input"
run_test "    "

print_test "10. env built-in"
run_test "env"

print_test "11. export + env"
run_test "export MYVAR=queen && env | grep MYVAR"

print_test "12. unset + env"
run_test "export MYVAR=gone && unset MYVAR && env | grep MYVAR"

print_test "13. cd and pwd"
run_test "cd .. && pwd"

print_test "14. relative path"
run_test "./minishell"

print_test "15. PATH unset"
run_test "unset PATH && ls"

print_test "16. Redirection"
run_test "echo hi > test_redir && cat test_redir"

print_test "17. Append redirection"
run_test "echo one > test_redir && echo two >> test_redir && cat test_redir"

print_test "18. Heredoc"
echo -e "cat << EOF\ntest line\nEOF" | $MINISHELL > $TMP_OUT
echo -e "test line" > $TMP_EXPECTED
check_output

print_test "19. Pipes"
run_test "echo one | cat | wc -l"

print_test "20. Broken command"
run_test "sdghsjdfgs"

print_test "21. Long command"
longcmd=$(printf "echo arg%.0s " {1..100})
run_test "$longcmd"

print_test "22. $? expression"
run_test "false; echo \$?; true; echo \$?; expr \$? + \$?"

# 🔔 SIGNALS: Ctrl-C, Ctrl-D, etc. are hard to test automatically, do manually if needed

# 🚩 RESULTS

if [ $CRASH_FLAG -eq 1 ]; then
	echo -e "\n\033[1;31m[RESULT] Crashes were detected. Mark with crash flag.\033[0m"
else
	echo -e "\n\033[1;32m[RESULT] No crashes detected.\033[0m"
fi

if [ $INCOMPLETE_FLAG -eq 1 ]; then
	echo -e "\033[1;33m[RESULT] Some features are incomplete or broken.\033[0m"
else
	echo -e "\033[1;32m[RESULT] All tested features working!\033[0m"
fi

# 🧼 Cleanup
rm -f $TMP_OUT $TMP_EXPECTED test_redir
