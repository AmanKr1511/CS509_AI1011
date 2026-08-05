PROJECT_NAME="matrix_project"

SRC_DIR="src"
BIN_DIR="bin"

mkdir -p "$BIN_DIR"
mkdir -p results

echo "======================================"
echo " Compiling Matrix Project..."
echo "======================================"

gcc \
    $SRC_DIR/main.c \
    $SRC_DIR/gemm.c \
    $SRC_DIR/gemm_block.c \
    $SRC_DIR/csr.c \
    $SRC_DIR/utils.c \
    -I$SRC_DIR \
    -Wall -Wextra \
    -O2 \
    -o $BIN_DIR/$PROJECT_NAME

if [ $? -ne 0 ]; then
    echo
    echo "Compilation Failed!"
    exit 1
fi

echo
echo "Compilation Successful."
echo

echo "======================================"
echo " Running Program"
echo "======================================"

./$BIN_DIR/$PROJECT_NAME