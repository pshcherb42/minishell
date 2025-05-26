# Minishell Project

This project is a custom implementation of a basic Unix shell.

## Building the Project

To compile `minishell`, ensure you have `make`, `gcc`, and `libreadline-dev` installed.

**Prerequisites:**
*   **make & gcc:** Usually available by default on Linux systems, or installable via `build-essential`:
    ```bash
    sudo apt-get update
    sudo apt-get install build-essential
    ```
*   **libreadline-dev:** For command line editing features.
    ```bash
    sudo apt-get install libreadline-dev
    ```

**Compilation:**
Once prerequisites are met, run the following command from the project's root directory:
```bash
make
```
This will compile all source files and create the `minishell` executable in the root directory.

## Running Tests

The project includes a test suite to verify basic functionality and check for resource leaks using Valgrind.

**Prerequisites for Testing:**
*   **Valgrind:** Required for leak detection tests.
    ```bash
    sudo apt-get install valgrind
    ```

**Executing Tests:**
To run all tests, use the following command from the project's root directory:
```bash
make test
```

This command performs the following actions:
1.  Builds the `minishell` executable if it hasn't been built already.
2.  Ensures the test scripts (`test_basic_features.sh` and `test_leaks.sh`) are executable.
3.  Executes `test_basic_features.sh`: This script runs a series of tests for common shell commands, builtins (like `cd`, `export`, `unset`, `exit`), pipes, and redirections.
4.  Executes `test_leaks.sh`: This script runs various commands and scenarios within `minishell` under Valgrind to detect memory leaks and open file descriptors.

## Interpreting Test Results

-   **Basic Feature Tests (`test_basic_features.sh`):**
    *   For each test case, the script will print `Testing: [Test Description]`, followed by the command being run, expected/actual output, and finally `PASS` or `FAIL`.
    *   At the end, a summary message `All tests passed!` or `Some tests FAILED!` will be displayed.
    *   If a test fails, the output will indicate the mismatch (e.g., STDOUT MISMATCH, EXIT CODE MISMATCH).

-   **Leak Detection Tests (`test_leaks.sh`):**
    *   For each scenario tested with Valgrind, the script will print `Testing (Valgrind): [Test Description]`.
    *   It will then report `STATUS: PASS` if no leaks or open file descriptor issues are detected by Valgrind for that scenario.
    *   If Valgrind detects issues, it will report `STATUS: FAIL (Memory Leaks Detected)` or `STATUS: FAIL (Open File Descriptors Detected)`. Relevant snippets from Valgrind's output (e.g., "definitely lost bytes", "FILE DESCRIPTORS: X open") will be shown.
    *   If any test fails, the `valgrind_run.log` file in the project root will contain the full Valgrind output for the *last failing test scenario*, which can be inspected for more details.
    *   At the end, a summary message `All Valgrind tests passed. No leaks detected.` or `X Valgrind test(s) FAILED.` will be displayed.

## Cleaning Up

To clean the build artifacts:

-   To remove all compiled object files (`.o` files) and the `minishell` executable, but keep the `libft.a` library:
    ```bash
    make clean
    ```

-   To remove all object files, the `minishell` executable, and also clean any files created by the `libft` library (including `libft.a`):
    ```bash
    make fclean
    ```
