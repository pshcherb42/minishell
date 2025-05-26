#!/bin/bash

# Path to the minishell executable
MINISHELL_PATH="./minishell"

# Check if minishell executable exists
if [ ! -x "$MINISHELL_PATH" ]; then
    echo "Error: minishell executable not found or not executable at $MINISHELL_PATH"
    exit 2
fi

# Global pass/fail status
OVERALL_STATUS=0 # 0 for pass, 1 for fail

# Function to run a test case
# Usage: run_test "Test Description" "command_to_run" "expected_stdout" "expected_stderr" expected_exit_code
# For commands that produce no stdout/stderr, use ""
# For grep commands where no output is success, expected_exit_code for grep might be 1
run_test() {
    local description="$1"
    local command_to_run="$2"
    local expected_stdout="$3"
    local expected_stderr="$4"
    local expected_exit_code="$5"
    # Optional: command to run after minishell, e.g. for grep
    local post_command_stdout_check="$6"
    local post_command_exit_code_check="$7"


    echo "----------------------------------------------------"
    echo "Testing: $description"
    echo "Command: $command_to_run"

    # Prepare the input for minishell
    # We always add 'exit' to ensure minishell terminates after the command.
    # For sequences like 'cd /tmp; pwd', the command_to_run will contain newlines.
    local input_commands="$command_to_run"
    # Add an exit command to ensure minishell terminates if not already part of the command
    if ! echo "$command_to_run" | grep -q "exit"; then
        input_commands="$command_to_run\nexit"
    fi

    # Execute minishell
    # Using process substitution to capture stdout, stderr, and exit code
    # Stderr is redirected to stdout for capture, then separated.
    local output
    local stderr_output
    local exit_code

    # Perform the command
    # We need to handle the exit code of minishell itself, not just the commands run within it.
    # Shellcheck SC2034: output appears unused. (Used indirectly via eval)
    # shellcheck disable=SC2034
    output_and_stderr=$(printf "%b" "$input_commands" | { "$MINISHELL_PATH" 2>&1; echo "MINISHELL_EXIT_CODE:$?"; } )

    exit_code=$(echo "$output_and_stderr" | grep "MINISHELL_EXIT_CODE:" | sed 's/MINISHELL_EXIT_CODE://')
    actual_stdout=$(echo "$output_and_stderr" | sed '$d') # Remove last line (exit code marker)

    # For now, stderr is mixed with stdout. This needs refinement if specific stderr checks are crucial beyond just presence.
    # A more sophisticated approach might involve temporary files for stderr.
    # For this script, we'll assume critical errors will cause non-zero exit codes or specific stderr messages.
    actual_stderr="" # Placeholder, as true stderr capture is tricky here without temp files

    echo "Expected STDOUT: '$expected_stdout'"
    echo "Actual STDOUT  : '$actual_stdout'"
    echo "Expected STDERR: '$expected_stderr'"
    # echo "Actual STDERR  : '$actual_stderr'" # Not reliably captured separately yet
    echo "Expected EXIT  : $expected_exit_code"
    echo "Actual EXIT    : $exit_code"

    local current_test_status=0 # 0 for pass

    # Check STDOUT
    if [ "$actual_stdout" != "$expected_stdout" ]; then
        echo "STDOUT MISMATCH!"
        current_test_status=1
    fi

    # Check STDERR (basic check: if expected_stderr is not empty, check if actual_stdout contains it)
    # This is a simplification because stderr is merged with stdout in the current capture method.
    if [ -n "$expected_stderr" ]; then
        if ! echo "$actual_stdout" | grep -qF "$expected_stderr"; then
            echo "STDERR MISMATCH! Expected error message not found."
            current_test_status=1
        else
             # If stderr is expected, we might want to clear actual_stdout for other checks,
             # or ensure expected_stdout accounts for this. For now, we assume specific stderr tests
             # will primarily look for the error message.
             # Example: actual_stdout=$(echo "$actual_stdout" | grep -vF "$expected_stderr")
             : # Placeholder for more complex stderr handling
        fi
    fi
    
    # Check EXIT code
    if [ "$exit_code" -ne "$expected_exit_code" ]; then
        echo "EXIT CODE MISMATCH!"
        current_test_status=1
    fi

    if [ $current_test_status -eq 0 ]; then
        echo "PASS"
    else
        echo "FAIL"
        OVERALL_STATUS=1
    fi
    echo "----------------------------------------------------"
    return $current_test_status
}

# Function to run a test case where the command is a sequence,
# and the final exit code is what matters (e.g. for export then env | grep)
# This is a variation of run_test, tailored for sequences where intermediate commands should succeed.
run_test_sequence() {
    local description="$1"
    local commands_to_run="$2" # Newline separated commands for minishell
    local expected_final_stdout="$3"
    local expected_stderr="$4" # Expected stderr for the whole sequence (if any command fails)
    local expected_final_exit_code="$5" # Expected exit code of minishell after all commands

    echo "----------------------------------------------------"
    echo "Testing Sequence: $description"
    echo -e "Commands:\n$commands_to_run"

    local input_commands="$commands_to_run\nexit" # Ensure minishell exits

    local output_and_stderr
    local exit_code
    local actual_stdout

    output_and_stderr=$(printf "%b" "$input_commands" | { "$MINISHELL_PATH" 2>&1; echo "MINISHELL_EXIT_CODE:$?"; } )
    
    exit_code=$(echo "$output_and_stderr" | grep "MINISHELL_EXIT_CODE:" | sed 's/MINISHELL_EXIT_CODE://')
    actual_stdout=$(echo "$output_and_stderr" | sed '$d') # Remove last line (exit code marker)
    
    # Strip minishell prompt if present (e.g. "minishell$ ")
    actual_stdout=$(echo "$actual_stdout" | sed 's/^minishell\$ //g' | sed '/^exit$/d' )


    echo "Expected FINAL STDOUT: '$expected_final_stdout'"
    echo "Actual FINAL STDOUT  : '$actual_stdout'"
    echo "Expected STDERR      : '$expected_stderr'"
    echo "Expected FINAL EXIT  : $expected_final_exit_code"
    echo "Actual FINAL EXIT    : $exit_code"

    local current_test_status=0

    # Check STDOUT
    # Using grep for partial matches, as command sequences might have intermediate output
    if ! echo "$actual_stdout" | grep -qF "$expected_final_stdout"; then
        echo "FINAL STDOUT MISMATCH! Expected content not found."
        current_test_status=1
    fi
    
    # Check STDERR
    if [ -n "$expected_stderr" ]; then
        if ! echo "$actual_stdout" | grep -qF "$expected_stderr"; then # stderr is mixed with stdout
            echo "STDERR MISMATCH! Expected error message not found."
            current_test_status=1
        fi
    elif echo "$actual_stdout" | grep -i "error"; then # Basic check for unexpected errors
        # This is a heuristic. Some commands might legitimately output "error".
        # Add more specific checks if needed.
        # echo "UNEXPECTED STDERR content found in output."
        # current_test_status=1
        : # Ignored for now
    fi

    # Check EXIT code
    if [ "$exit_code" -ne "$expected_final_exit_code" ]; then
        echo "FINAL EXIT CODE MISMATCH!"
        current_test_status=1
    fi

    if [ $current_test_status -eq 0 ]; then
        echo "PASS"
    else
        echo "FAIL"
        OVERALL_STATUS=1
    fi
    echo "----------------------------------------------------"
    return $current_test_status
}


# --- Test Cases ---

# 1. Simple commands without arguments
CURRENT_DIR=$(pwd)
run_test "pwd" "pwd" "$CURRENT_DIR" "" 0

# env test: output is variable, so we check for a common variable like PWD and exit code
# A more robust check would be to set a variable and check for it.
# For now, just check if it runs and produces some output without error.
# This test is tricky because `env` output is extensive and environment-dependent.
# We'll check if `PWD` is in the output.
run_test_sequence "env" "env" "PWD=$CURRENT_DIR" "" 0 # Checks if PWD is in env output

# 2. Simple commands with arguments
run_test "echo hello world" "echo hello world" "hello world" "" 0
run_test "echo -n hello" "echo -n hello" "hello" "" 0

# 3. Builtin cd
run_test_sequence "cd /tmp then pwd" "cd /tmp\npwd" "/tmp" "" 0
HOME_DIR=$(eval echo ~$USER) # Get home directory for current user
run_test_sequence "cd to home then pwd" "cd\npwd" "$HOME_DIR" "" 0
# For `cd non_existent_dir`, minishell might print an error.
# The exact error message needs to be known or tested for. Assuming "No such file or directory"
# Exit code for `cd non_existent_dir` in bash is 1. Minishell might differ.
# For now, let's assume it prints an error containing "non_existent_dir" and "No such file or directory" and exits non-zero.
# The prompt "minishell$" might be part of the output.
# The actual command `cd non_existent_dir` might have exit status 0 in minishell if it's only the `cd` builtin that fails,
# but the overall minishell execution (due to `exit` later) should be 0 unless `exit` is called with a value.
# This needs clarification on how minishell handles builtin errors and exit codes.
# Let's assume the error is printed and minishell continues, then exits 0.
# The problem description says "exit non-zero" for `cd non_existent_dir`. This means the `cd` command itself,
# or the shell if `set -e` were active (not typical for interactive shells).
# We'll assume the `cd` failure causes minishell to exit non-zero if that's the *last* command.
# If we do `cd non_existent_dir\nexit 0`, the exit code will be 0.
# If we do `cd non_existent_dir\nexit`, and `cd` sets a global error status that `exit` uses, then it would be non-zero.
# Let's test for a specific error message on stderr (which is currently stdout).
run_test_sequence "cd non_existent_dir" "cd non_existent_dir" "" "non_existent_dir: No such file or directory" 1 # Assuming exit 1 and specific error

# 4. Builtin export
run_test_sequence "export TEST_VAR=hello then env | grep TEST_VAR" \
    "export TEST_VAR=hello\nenv | grep TEST_VAR" \
    "TEST_VAR=hello" "" 0

# For VAR_NO_VALUE, different shells behave differently. Some print "VAR_NO_VALUE=", some "declare -x VAR_NO_VALUE".
# We'll grep for "VAR_NO_VALUE" and check exit code 0.
run_test_sequence "export VAR_NO_VALUE then env | grep VAR_NO_VALUE" \
    "export VAR_NO_VALUE\nenv | grep VAR_NO_VALUE" \
    "VAR_NO_VALUE" "" 0 # Grep should find it.

# 5. Builtin unset
# `grep` should exit 1 if it doesn't find the variable. Minishell should exit 0.
# This requires careful handling of exit codes. The `run_test_sequence` is for minishell's exit code.
# We need to check grep's output (empty) and its exit code (1) separately.
echo "----------------------------------------------------"
echo "Testing: unset UNSET_TEST"
echo "Command: export UNSET_TEST=123; unset UNSET_TEST; env | grep UNSET_TEST"
COMMAND_SEQUENCE="export UNSET_TEST=123\nunset UNSET_TEST\nenv | grep UNSET_TEST\nexit"
OUTPUT=$(printf "%b" "$COMMAND_SEQUENCE" | "$MINISHELL_PATH")
GREP_OUTPUT=$(echo "$OUTPUT" | sed 's/^minishell\$ //g' | sed '/^exit$/d') # Remove prompt and exit line
MINISHELL_EXIT_CODE=$? # This is bash's exit code for the pipe, not minishell's internal one. This is tricky.

# Re-evaluating how to get minishell's exit code in this complex pipe.
# The previous method for run_test is better. Let's adapt.

echo "Running: export UNSET_TEST=123; unset UNSET_TEST; env | grep UNSET_TEST"
output_and_stderr=$(printf "export UNSET_TEST=123\nunset UNSET_TEST\nenv | grep UNSET_TEST\nexit\n" | { "$MINISHELL_PATH" 2>&1; echo "MINISHELL_EXIT_CODE:$?"; } )
minishell_actual_exit_code=$(echo "$output_and_stderr" | grep "MINISHELL_EXIT_CODE:" | sed 's/MINISHELL_EXIT_CODE://')
actual_stdout_from_minishell=$(echo "$output_and_stderr" | sed '$d' | sed 's/^minishell\$ //g' | sed '/^exit$/d')

echo "Expected STDOUT (from grep via minishell): '' (empty)"
echo "Actual STDOUT (from grep via minishell)  : '$actual_stdout_from_minishell'"
echo "Expected MINISHELL EXIT  : 0" # Minishell itself should exit 0
echo "Actual MINISHELL EXIT    : $minishell_actual_exit_code"

UNSET_TEST_STATUS=0
if [ -n "$actual_stdout_from_minishell" ]; then # Should be empty if unset worked
    echo "UNSET STDOUT MISMATCH! Expected empty, got '$actual_stdout_from_minishell'"
    UNSET_TEST_STATUS=1
fi
if [ "$minishell_actual_exit_code" -ne 0 ]; then
    echo "UNSET MINISHELL EXIT CODE MISMATCH!"
    UNSET_TEST_STATUS=1
fi

if [ $UNSET_TEST_STATUS -eq 0 ]; then
    echo "PASS"
else
    echo "FAIL"
    OVERALL_STATUS=1
fi
echo "----------------------------------------------------"


# 6. Builtin exit
run_test "exit" "exit" "" "" 0
run_test "exit 42" "exit 42" "" "" 42


# 7. Pipes
# `echo hello` has 6 chars ("h,e,l,l,o,\n"). `wc -c` counts bytes.
# Behavior of `echo` regarding newline might vary. Standard echo adds newline.
# Let's assume minishell's echo is like standard echo.
# The prompt "minishell$ " might be part of the output if not handled.
# The `run_test` function's stdout processing needs to be robust to prompts.
# `actual_stdout=$(echo "$output_and_stderr" | sed '$d' | sed 's/^minishell\$ //g')`
# The `run_test_sequence` has better prompt stripping. Let's use that.

# For `echo hello | wc -c`, the output of `wc -c` should be "       6" (padded) or "6" depending on `wc`.
# Let's target "6" and trim whitespace for comparison.
# `printf "echo hello\nexit\n" | ./minishell | wc -c` is different from `printf "echo hello | wc -c\nexit\n" | ./minishell`
# We are testing minishell's pipe handling.
HELLO_WC_C_EXPECTED=$(echo -n "hello" | wc -c) # Output of "hello" (no newline)
HELLO_NL_WC_C_EXPECTED=$(echo "hello" | wc -c) # Output of "hello\n"

run_test_sequence "echo hello | wc -c" "echo hello | wc -c" "$HELLO_NL_WC_C_EXPECTED" "" 0

# ls | grep main : Assuming 'main.c' or 'main/' directory exists from previous ls.
# Let's create a dummy main file for consistency.
touch main_test_file_for_grep
run_test_sequence "ls | grep main_test_file_for_grep" "ls | grep main_test_file_for_grep" "main_test_file_for_grep" "" 0
rm main_test_file_for_grep

# cat non_existent_file | wc -l
# `cat non_existent_file` should write to stderr "cat: non_existent_file: No such file or directory" and exit 1.
# `wc -l` should then receive empty stdin, output "0".
# The overall pipe exit code is typically the exit code of the last command (wc -l -> 0).
# Minishell's behavior for stderr in pipes needs to be observed.
# Expect "cat: non_existent_file: No such file or directory" on stderr (mixed with stdout here)
# and "0" as the final stdout from wc -l.
run_test_sequence "cat non_existent_file | wc -l" \
    "cat non_existent_file | wc -l" \
    "0" "non_existent_file: No such file or directory" 0 # wc -l is 0, cat error to stderr

# 8. Input Redirection (<)
echo "hello world" > input.txt
run_test_sequence "wc -w < input.txt" "wc -w < input.txt" "2" "" 0
rm input.txt

# 9. Output Redirection (> and >>)
run_test_sequence "echo line1 > output.txt then cat output.txt" \
    "echo line1 > output.txt\ncat output.txt" \
    "line1" "" 0

run_test_sequence "echo line2 >> output.txt then cat output.txt" \
    "echo line2 >> output.txt\ncat output.txt" \
    "line1\nline2" "" 0 # Assuming output.txt still has "line1"
rm output.txt


# 10. Heredoc (<<)
# The sed commands in run_test_sequence to remove "minishell$ " and "exit" lines are important here.
run_test_sequence "cat << EOF (heredoc)" \
    "cat << EOF\nhello\nworld\nEOF" \
    "hello\nworld" "" 0

run_test_sequence "cat << END | wc -l (heredoc with pipe)" \
    "cat << END | wc -l\nline1\nline2\nEND" \
    "2" "" 0


# --- Final Status ---
echo "===================================================="
if [ $OVERALL_STATUS -eq 0 ]; then
    echo "All tests passed!"
    exit 0
else
    echo "Some tests FAILED!"
    exit 1
fi
