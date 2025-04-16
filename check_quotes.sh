#!/bin/bash

MINISHELL=./minishell
TMP_OUTPUT=.minishell_test_output.txt

run_test() {
	echo "$1" | $MINISHELL > "$TMP_OUTPUT"
	RESULT=$(cat "$TMP_OUTPUT")
	echo "🔹 Comando: $1"
	echo "🔸 Resultado: $RESULT"
	echo "----------------------------"
}

echo "=== 🧪 Testing Quotes Behavior ==="
echo

# Preparar entorno
export USER=Polina
export TESTVAR="hello world"

# 1. Comillas dobles - expansión de variable
run_test 'echo "$USER"'
run_test 'echo "$TESTVAR"'
run_test 'echo "Hola $USER, $TESTVAR"'

# 2. Comillas simples - sin expansión
run_test "echo '\$USER'"
run_test "echo '\$TESTVAR'"
run_test "echo 'Hola \$USER, \$TESTVAR'"

# 3. Frases mixtas
run_test 'echo "User: $USER"'
run_test "echo 'User: $USER'"
run_test 'echo "$USER y texto libre"'
run_test "echo '\$USER y texto libre'"

# 4. Comillas vacías
run_test "echo ''"
run_test 'echo ""'

# 5. Comillas dentro de comillas
run_test 'echo "Texto con comillas simples: '\''hola'\''"'
run_test "echo 'Texto con \"dobles\" comillas'"

# 6. Texto con espacios dentro de comillas dobles
run_test 'echo "palabra con     muchos    espacios"'

# Cleanup
rm -f "$TMP_OUTPUT"

echo "✅ Finalizado"
