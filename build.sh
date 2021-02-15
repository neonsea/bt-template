#!/bin/bash
set -e

PROGRAM_NAME="$(cat ./name.txt)"

EABI="arm-none-eabi"

LIB_BIP_PATH="$HOME/Projects/libbip"
LIB_BIP="$LIB_BIP_PATH/libbip.a"

GCC_OPT="-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=auto -fsingle-precision-constant -fno-math-errno -ffast-math -I${LIB_BIP_PATH} -c -Os -Wa,-R -Wall -fpie -pie -fpic -mlittle-endian  -ffunction-sections -fdata-sections"
# lookie here
LD_OPT="-L${LIB_BIP_PATH} -L/usr/lib/gcc/${EABI}/9.2.1 -EL -N -Os -pie --gc-sections --no-dynamic-linker"

AS="${EABI}-as"
LD="${EABI}-ld"
OBJCOPY="${EABI}-objcopy"
GCC="${EABI}-gcc"
NM="${EABI}-nm"

LABEL="$(cat ./label.txt)"

for file in *.c; do
    filename=$(basename -- "$file")
    name="${filename%.*}"
    $GCC $GCC_OPT -o "${name}.o" "${file}"
done

$LD -o "${PROGRAM_NAME}.elf" *.o $LD_OPT -lbip -lm -lc -lgcc

[[ -f "label.txt" ]] &&
    $OBJCOPY "${PROGRAM_NAME}.elf" --add-section .elf.label=label.txt

[[ -f "name.txt" ]] &&
    $OBJCOPY "${PROGRAM_NAME}.elf" --add-section .elf.name=name.txt

[[ -f "asset.res" ]] &&
    $OBJCOPY "${PROGRAM_NAME}.elf" --add-section .elf.resources=asset.res

[[ -f "settings.bin" ]] &&
    $OBJCOPY "${PROGRAM_NAME}.elf" --add-section .elf.settings=settings.bin

exit 0