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
    local output_and_stderr
    local exit_code
    local actual_stdout_raw
    local actual_stdout # Cleaned output

    output_and_stderr=$(printf "%b" "$input_commands" | { "$MINISHELL_PATH" 2>&1; echo "MINISHELL_EXIT_CODE:$?"; } )

    exit_code=$(echo "$output_and_stderr" | grep "MINISHELL_EXIT_CODE:" | sed 's/MINISHELL_EXIT_CODE://')
    actual_stdout_raw=$(echo "$output_and_stderr" | sed '$d') # Remove last line (exit code marker)

    # Clean the raw output:
    # 1. Remove "minishell$ exit" and "exit" lines from the end. (already done by sed '$d' for "MINISHELL_EXIT_CODE")
    #    The line before MINISHELL_EXIT_CODE is the "minishell$ exit" line.
    #    So, actual_stdout_raw contains:
    #    minishell$ <command>
    #    ... output ...
    #    minishell$ exit
    #
    #    Let's re-evaluate. output_and_stderr is everything.
    #    actual_stdout_raw is output_and_stderr MINUS the "MINISHELL_EXIT_CODE:..." line.
    #    So actual_stdout_raw's last line is "minishell$ exit" OR the actual output if it ends with "minishell$" (like echo -n)

    # Revised cleaning strategy for run_test (turn 33, using sed for suffix removal)
    local temp_output
    
    # 1. Start with raw output
    temp_output="$actual_stdout_raw"

    # 2. Remove the first line (e.g., "minishell$ pwd")
    temp_output=$(printf "%s\n" "$temp_output" | sed '1d')

    # 3. Remove the final "exit" line if it exists and is exactly "exit"
    #    (This is the "exit" that minishell prints after its prompt loop finishes)
    if [ "$(printf "%s" "$temp_output" | tail -n 1)" = "exit" ]; then
        temp_output=$(printf "%s" "$temp_output" | head -n -1)
    fi
    
    # 4. Try to remove "minishell$ exit" from the end of the string using sed.
    #    The `$` in sed regex means end of line. This should handle cases like "pwd"
    #    resulting in "/app\nminishell$ exit" and "echo -n hello" resulting in "hellominishell$ exit".
    local s_pattern_prompt_exit='s/minishell\$ exit$//'
    local sed_processed_output
    # Apply sed to temp_output. Use printf to ensure no issues with special chars in temp_output.
    sed_processed_output=$(printf "%s" "$temp_output" | sed "$s_pattern_prompt_exit")

    # 5. If sed didn't change anything (meaning "minishell$ exit" wasn't the suffix)
    #    AND it's an echo -n case AND the original temp_output (before this sed) ends with "minishell$",
    #    then try removing just "minishell$" as a fallback. This handles a hypothetical
    #    case where "echo -n" output is "VALUEminishell$" without the " exit" part.
    if [[ "$sed_processed_output" == "$temp_output" ]] && \
       [[ "$command_to_run" == "echo -n"* ]] && \
       [[ "$temp_output" == *minishell\$ ]]; then # Check original temp_output before sed
        local s_pattern_prompt_only='s/minishell\$$//'
        temp_output=$(printf "%s" "$temp_output" | sed "$s_pattern_prompt_only")
    else
        # Otherwise, use the result from the "minishell$ exit" removal attempt.
        temp_output="$sed_processed_output"
    fi
            
    # 6. Trim leading/trailing whitespace and newlines from the final result
    actual_stdout=$(echo "$temp_output" | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')

    actual_stderr="" # Placeholder, stderr checks use actual_stdout_raw

    # Normalize outputs before comparison
    normalized_actual_stdout=$(normalize_string "$actual_stdout")
    normalized_expected_stdout=$(normalize_string "$expected_stdout")

    echo "Expected STDOUT: '$expected_stdout'"
    echo "Actual STDOUT  : '$actual_stdout'"
    echo "Raw STDOUT     : '$actual_stdout_raw'" # For debugging
    echo "Expected STDERR: '$expected_stderr'" # This is for messages expected in raw output
    echo "Expected EXIT  : $expected_exit_code"
    echo "Actual EXIT    : $exit_code"

    local current_test_status=0

    if [ "$normalized_actual_stdout" != "$normalized_expected_stdout" ]; then
        echo "STDOUT MISMATCH!"
        echo "--- Expected STDOUT (original) ---"
        printf "%s\n" "$expected_stdout"
        echo "--- Actual STDOUT (original) ---"
        printf "%s\n" "$actual_stdout"
        # Revised logging for normalized strings (Turn 39)
        normalized_expected_for_log=$(normalize_string "$expected_stdout")
        echo "--- Expected STDOUT (normalized, cat -e) ---"
        printf "%s\n" "$normalized_expected_for_log" | cat -e
        normalized_actual_for_log=$(normalize_string "$actual_stdout")
        echo "--- Actual STDOUT (normalized, cat -e) ---"
        printf "%s\n" "$normalized_actual_for_log" | cat -e
        echo "------------------------------------------"
        current_test_status=1
    fi

    if [ -n "$expected_stderr" ]; then
        # Check raw output for expected stderr messages
        if ! echo "$actual_stdout_raw" | grep -qF "$expected_stderr"; then
            echo "STDERR MISMATCH! Expected error message '$expected_stderr' not found in raw output."
            current_test_status=1
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
    actual_stdout_raw=$(echo "$output_and_stderr" | sed '$d') # Remove last line (exit code marker)
    
    # Clean the raw output for sequences:
    # 1. Remove "minishell$ exit" line from the end.
    # 2. Remove echoed commands and prompts from the beginning of each command's output block.
    # 3. Concatenate the actual outputs.
    # This is complex because commands are newline separated.
    # A simpler approach for sequence: Remove all lines starting with "minishell$ " and the final "exit" line.
    # Then, filter out the input commands themselves if they are echoed.
    
    # Remove the "minishell$ exit" line
    cleaned_sequence_output="${actual_stdout_raw%minishell\$ exit}"
    # Remove trailing newline
    cleaned_sequence_output="${cleaned_sequence_output%$'\n'}"

    # Remove all "minishell$ " prompts and the "exit" line itself if it's there
    # Also remove echoed input lines. This is tricky because input lines might be valid output.
    # For now, let's try a more targeted removal of prompts and specific echoed commands.
    # This sed command attempts to:
    # 1. Delete lines that are just "minishell$ <some_command_from_input>"
    # 2. Delete the line that is just "minishell$ exit" (already handled mostly)
    # 3. Delete the line that is just "exit"
    # This is still not perfect for multi-line outputs from single commands within sequence.
    
    # New strategy for sequences:
    # Remove "minishell$ exit" and the final "MINISHELL_EXIT_CODE:" marker (already done for actual_stdout_raw).
    # Final refined cleaning strategy for run_test_sequence
    local temp_output
    
    # 1. Start with raw output
    temp_output="$actual_stdout_raw"

    # 2. Remove the last line if it is "minishell$ exit"
    if [ "$(printf "%s" "$temp_output" | tail -n 1)" = "minishell$ exit" ]; then
        temp_output=$(printf "%s" "$temp_output" | head -n -1)
    fi

    # 3. Filter out lines starting with "minishell$ " (prompts before commands in sequence)
    #    and lines starting with "> " (heredoc prompts).
    temp_output=$(printf "%s\n" "$temp_output" | sed -e '/^minishell\$ /d' -e '/^> /d')
    
    # 4. Remove the new last line if it is exactly "exit"
    #    This handles the case where minishell prints "exit" after all command outputs.
    if [ "$(printf "%s" "$temp_output" | tail -n 1)" = "exit" ]; then
        temp_output=$(printf "%s" "$temp_output" | head -n -1)
    fi
        
    # 5. Trim leading/trailing whitespace and newlines from the final result
    actual_stdout=$(echo "$temp_output" | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')


    echo "Expected FINAL STDOUT: '$expected_final_stdout'"
    echo "Actual FINAL STDOUT  : '$actual_stdout'"
    echo "Raw FINAL STDOUT     : '$actual_stdout_raw'" # For debugging
    echo "Expected STDERR      : '$expected_stderr'" # This is for messages expected in raw output
    echo "Expected FINAL EXIT  : $expected_final_exit_code"
    echo "Actual FINAL EXIT    : $exit_code"

    local current_test_status=0

    # Special handling for 'env' test: grep for PWD in raw output
    if [[ "$description" == "env" ]]; then
        local current_pwd_for_env_test
        current_pwd_for_env_test=$(pwd) # Get current directory for PWD check
        if ! echo "$actual_stdout_raw" | grep -q "PWD=$current_pwd_for_env_test"; then
            echo "ENV TEST: 'PWD=$current_pwd_for_env_test' not found in raw output."
            current_test_status=1
        fi
        # For 'env' test, we don't check expected_final_stdout further as the primary check is the grep.
        # expected_final_stdout should be empty for this specific test case.
    else
        # Normalize outputs before comparison for other sequence tests
        normalized_actual_stdout=$(normalize_string "$actual_stdout")
        normalized_expected_stdout=$(normalize_string "$expected_final_stdout")

        if [ "$normalized_actual_stdout" != "$normalized_expected_stdout" ]; then
            echo "FINAL STDOUT MISMATCH!"
            echo "--- Expected FINAL STDOUT (original) ---"
            printf "%s\n" "$expected_final_stdout"
            echo "--- Actual FINAL STDOUT (original) ---"
            printf "%s\n" "$actual_stdout"
            # Revised logging for normalized strings (Turn 39)
            normalized_expected_for_log_seq=$(normalize_string "$expected_final_stdout")
            echo "--- Expected FINAL STDOUT (normalized, cat -e) ---"
            printf "%s\n" "$normalized_expected_for_log_seq" | cat -e
            normalized_actual_for_log_seq=$(normalize_string "$actual_stdout")
            echo "--- Actual FINAL STDOUT (normalized, cat -e) ---"
            printf "%s\n" "$normalized_actual_for_log_seq" | cat -e
            echo "--------------------------------------------"
            current_test_status=1
        fi
    fi
    
    # Check STDERR (search in actual_stdout_raw, as stderr is mixed and actual_stdout is cleaned)
    if [ -n "$expected_stderr" ]; then
        if ! echo "$actual_stdout_raw" | grep -qF "$expected_stderr"; then
            echo "STDERR MISMATCH! Expected error message '$expected_stderr' not found in raw output."
            current_test_status=1
        fi
    # Optional: Heuristic check for unexpected "error" messages, uncomment if needed.
    # elif echo "$actual_stdout_raw" | grep -qi "error" | grep -vF "$expected_stderr" >/dev/null 2>&1; then
    #     echo "POTENTIAL UNEXPECTED STDERR content found in raw output (heuristic check)."
    #     current_test_status=1
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
# We'll check if `PWD` is in the output using grep. Expected STDOUT for this test is empty.
run_test_sequence "env" "env" "" "" 0

# 2. Simple commands with arguments
run_test "echo hello world" "echo hello world" "hello world" "" 0
run_test "echo -n hello" "echo -n hello" "hello" "" 0

# 3. Builtin cd
run_test_sequence "cd /tmp then pwd" "cd /tmp\npwd" "/tmp" "" 0

HOME_DIR=$(eval echo ~) # Get home directory more reliably
# Test 'cd' without arguments. Expect "minishell: cd: too many arguments\n/app" (if CWD is /app).
# Note: The error message from minishell IS part of its STDOUT.
CD_NO_ARGS_EXPECTED_STDOUT="minishell: cd: too many arguments\n$(pwd)"
run_test_sequence "cd to home (expecting error and original pwd)" \
    "cd\npwd" \
    "$CD_NO_ARGS_EXPECTED_STDOUT" "minishell: cd: too many arguments" 0

# For `cd non_existent_dir`, expect error message on STDOUT. Minishell exit code 0.
CD_NON_EXISTENT_EXPECTED_STDOUT="minishell: cd: non_existent_dir: No such file or directory"
run_test_sequence "cd non_existent_dir (expecting error message, exit 0)" \
    "cd non_existent_dir" \
    "$CD_NON_EXISTENT_EXPECTED_STDOUT" "minishell: cd: non_existent_dir: No such file or directory" 0

# 4. Builtin export
run_test_sequence "export TEST_VAR=hello then env | grep TEST_VAR" \
    "export TEST_VAR=hello\nenv | grep TEST_VAR" \
    "TEST_VAR=hello" "" 0

# Test export without value: export VAR_NO_VAL_TEST; echo "VAR_IS:$VAR_NO_VAL_TEST"
# Expected output: "VAR_IS:" (assuming unset/valueless vars expand to empty string by minishell's echo)
run_test_sequence "export VAR_NO_VAL_TEST then echo" \
    "export VAR_NO_VAL_TEST\necho \"VAR_IS:\$VAR_NO_VAL_TEST\"" \
    "VAR_IS:" "" 0

# 5. Builtin unset
# The previous 'unset' test was a custom block. Let's use run_test_sequence.
# `env | grep UNSET_TEST` should produce no output if unset is successful.
run_test_sequence "unset UNSET_TEST" \
    "export UNSET_TEST=123\nunset UNSET_TEST\nenv | grep UNSET_TEST" \
    "" "" 0 # Expected output is empty, minishell exits 0.

# 6. Builtin exit
# The `run_test` function's output stripping needs to be solid for these.
# `exit` command itself produces no stdout.
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
# The expected STDOUT includes the error message from cat, then the output from wc.
CAT_NONEXISTENT_WC_EXPECTED_STDOUT="cat: non_existent_file: No such file or directory\n0"
run_test_sequence "cat non_existent_file | wc -l" \
    "cat non_existent_file | wc -l" \
    "$CAT_NONEXISTENT_WC_EXPECTED_STDOUT" "non_existent_file: No such file or directory" 0

# 8. Input Redirection (<)
echo "hello world" > input.txt
run_test_sequence "wc -w < input.txt" "wc -w < input.txt" "2" "" 0
rm input.txt

# 9. Output Redirection (> and >>)
run_test_sequence "echo line1 > output.txt then cat output.txt" \
    "echo line1 > output.txt\ncat output.txt" \
    "line1" "" 0

run_test_sequence "echo line2 >> output.txt then cat output.txt" \
    "echo line1 > output.txt\necho line2 >> output.txt\ncat output.txt" \
    "line1\nline2" "" 0 # output.txt should have "line1" then "line2"
rm -f output.txt # Ensure cleanup even if test fails mid-way


# 10. Heredoc (<<)
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
