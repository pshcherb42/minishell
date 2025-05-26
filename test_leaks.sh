#!/bin/bash

MINISHELL_PATH="./minishell"
VALGRIND_LOG="valgrind_run.log"
OVERALL_FAILURES=0

# Check if minishell executable exists
if [ ! -x "$MINISHELL_PATH" ]; then
    echo "Error: minishell executable not found or not executable at $MINISHELL_PATH"
    echo "Please build minishell first (e.g., using 'make')."
    exit 2
fi

# Check if Valgrind is installed
if ! command -v valgrind &> /dev/null; then
    echo "Error: Valgrind is not installed. Please install it first."
    exit 2
fi

# Test cases: Description and Command
# Using a HEREDOC for multiline commands for clarity
declare -a test_descriptions=(
    "Simple command"
    "Pipeline"
    "Output and Input Redirection"
    "Heredoc"
    "Command causing an error"
    "Complex command with pipe, redirection, and tee"
    "Multiple commands in sequence"
    "Export with no value and check env"
    "Unset a variable and check it's gone"
    "Exit with a specific status code"
)

read -r -d '' CMD_SIMPLE <<'EOF'
ls /tmp
EOF

read -r -d '' CMD_PIPELINE <<'EOF'
echo "test pipe" | wc -c
EOF

read -r -d '' CMD_REDIRECTION <<'EOF'
echo "test redirection" > test_output.txt
cat < test_output.txt
EOF

read -r -d '' CMD_HEREDOC <<'EOF'
cat << HDOC_EOF
heredoc test
line 2
HDOC_EOF
EOF

read -r -d '' CMD_ERROR <<'EOF'
cat non_existent_file
EOF

read -r -d '' CMD_COMPLEX <<'EOF'
echo "complex" | sed 's/complex/very complex/' | tee complex_output.txt | wc -c
cat complex_output.txt
EOF

read -r -d '' CMD_SEQUENCE <<'EOF'
pwd
export A_TEST_VAR=B_TEST_VAL
env | grep A_TEST_VAR
unset A_TEST_VAR
env | grep A_TEST_VAR
cd /
pwd
EOF

read -r -d '' CMD_EXPORT_NO_VALUE <<'EOF'
export NO_VALUE_VAR
env | grep NO_VALUE_VAR
EOF

read -r -d '' CMD_UNSET_CHECK <<'EOF'
export VAR_TO_UNSET=present
env | grep VAR_TO_UNSET
unset VAR_TO_UNSET
env | grep VAR_TO_UNSET
EOF

read -r -d '' CMD_EXIT_STATUS <<'EOF'
exit 55
EOF


declare -a test_commands=(
    "$CMD_SIMPLE"
    "$CMD_PIPELINE"
    "$CMD_REDIRECTION"
    "$CMD_HEREDOC"
    "$CMD_ERROR"
    "$CMD_COMPLEX"
    "$CMD_SEQUENCE"
    "$CMD_EXPORT_NO_VALUE"
    "$CMD_UNSET_CHECK"
    "$CMD_EXIT_STATUS"
)

# Function to run a test case with Valgrind
run_valgrind_test() {
    local description="$1"
    local command_to_run="$2"
    local test_failed=0

    echo "----------------------------------------------------"
    echo "Testing (Valgrind): $description"
    echo -e "Command(s):\n$command_to_run"

    # Ensure minishell exits after the command(s)
    # unless the command itself is 'exit'
    local input_commands="$command_to_run"
    if ! echo "$command_to_run" | grep -q "exit"; then
        input_commands="$command_to_run\nexit"
    fi

    # Run minishell under Valgrind
    # Stderr from Valgrind is where the leak summary goes.
    # We use --log-file for Valgrind's output.
    rm -f "$VALGRIND_LOG" # Clean up log from previous run
    printf "%b" "$input_commands" | valgrind \
        --leak-check=full \
        --show-leak-kinds=all \
        --track-fds=yes \
        --log-file="$VALGRIND_LOG" \
        "$MINISHELL_PATH" > /dev/null 2>&1 # Suppress minishell's stdout/stderr from console

    # Check for memory leaks
    if grep -q "definitely lost: [1-9][0-9]* bytes" "$VALGRIND_LOG" || \
       grep -q "indirectly lost: [1-9][0-9]* bytes" "$VALGRIND_LOG" || \
       grep -q "possibly lost: [1-9][0-9]* bytes" "$VALGRIND_LOG"; then
        echo "STATUS: FAIL (Memory Leaks Detected)"
        grep -E "definitely lost:|indirectly lost:|possibly lost:" "$VALGRIND_LOG" | grep -v " 0 bytes"
        test_failed=1
    fi

    # Check for open file descriptors
    # Valgrind reports "FILE DESCRIPTORS: X open (Y std) at exit."
    # We want to see if X > Y. Y is typically 3 (stdin, stdout, stderr).
    # Or, check for specific " open file descriptor [N]" messages for N > 2.
    local open_fds_summary
    open_fds_summary=$(grep "FILE DESCRIPTORS: " "$VALGRIND_LOG")
    if [ -n "$open_fds_summary" ]; then
        # Example: "FILE DESCRIPTORS: 4 open (3 std) at exit."
        local total_open
        total_open=$(echo "$open_fds_summary" | awk '{print $3}')
        local std_open
        std_open=$(echo "$open_fds_summary" | awk '{print $5}' | tr -d '()std')

        # Default to 0 if empty or not a number (basic check for robustness) - Turn 40
        if ! [[ "$total_open" =~ ^[0-9]+$ ]]; then
            # echo "DEBUG: total_open ('$total_open') is not a number, defaulting to 0."
            total_open=0
        fi
        if ! [[ "$std_open" =~ ^[0-9]+$ ]]; then
            # echo "DEBUG: std_open ('$std_open') is not a number, defaulting to 0."
            # Default to 3 for std fds if parsing somehow fails but summary line was found,
            # as Valgrind usually reports 3 standard FDs.
            std_open=3 
        fi

        if [ "$total_open" -gt "$std_open" ]; then # This is the critical comparison
            echo "STATUS: FAIL (Open File Descriptors Detected via Summary: $total_open open > $std_open std)"
            echo "$open_fds_summary"
            # Show specific open FDs if listed (Valgrind lists them above the summary)
            # Filter out standard FDs (0, 1, 2) from this detailed list as they are expected.
            grep " open file descriptor " "$VALGRIND_LOG" | grep -E -v "(inherited from parent|fd 0:|fd 1:|fd 2:)"
            test_failed=1
        fi
    fi
    
    # Fallback check for individual open FD messages if summary parsing is tricky or insufficient
    # This checks for FDs 3 or higher explicitly mentioned as open.
    # Only run this check if test_failed is still 0 to avoid redundant messages and preferably if no summary was found.
    if [ $test_failed -eq 0 ] && [ -z "$open_fds_summary" ]; then
        # Check for any line indicating an open FD other than 0, 1, 2
        # The grep below will have exit code 0 if any such line is found.
        if grep -q " open file descriptor " "$VALGRIND_LOG" | grep -E -v "(inherited from parent|fd 0:|fd 1:|fd 2:)"; then
            echo "STATUS: FAIL (Open File Descriptors Detected - individual FD check, no summary line found)"
            grep " open file descriptor " "$VALGRIND_LOG" | grep -E -v "(inherited from parent|fd 0:|fd 1:|fd 2:)"
            test_failed=1
        fi
    fi


    if [ $test_failed -eq 0 ]; then
        echo "STATUS: PASS"
    else
        OVERALL_FAILURES=$((OVERALL_FAILURES + 1))
        echo "Full Valgrind log available at: $VALGRIND_LOG"
        # Keep the log for the first failure, then overwrite for next tests to save space if needed,
        # or append test name to log file name. For now, it's overwritten.
        # To keep all logs, one might do: cp "$VALGRIND_LOG" "valgrind_fail_${description// /_}.log"
    fi
    echo "----------------------------------------------------"
}

# --- Run Test Cases ---
num_tests=${#test_commands[@]}
for i in $(seq 0 $((num_tests - 1))); do
    run_valgrind_test "${test_descriptions[$i]}" "${test_commands[$i]}"
done

# --- Cleanup ---
rm -f test_output.txt complex_output.txt
# rm -f "$VALGRIND_LOG" # Keep the last log if needed, or if a failure occurred.
if [ $OVERALL_FAILURES -eq 0 ]; then
    rm -f "$VALGRIND_LOG" # Clean up log if all tests passed
    echo "All Valgrind tests passed. No leaks detected."
else
    echo "$OVERALL_FAILURES Valgrind test(s) FAILED. Check '$VALGRIND_LOG' for the last failed test's details."
fi
echo "===================================================="


# --- Final Exit Status ---
if [ $OVERALL_FAILURES -eq 0 ]; then
    exit 0
else
    exit 1
fi
