#include "disasm.h"

#include <Zydis/Zydis.h>
#include <cstdio>

extern "C" void
disasm_print_bytes(FILE* fp, const uint8_t* code, size_t size, uint64_t runtime_address) {


    ZydisDecoder decoder;
    ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);

    ZydisFormatter formatter;
    ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);

    ZyanUSize offset = 0;
    ZydisDecodedInstruction instruction;
    ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT];

    while (offset < size) {
        ZyanStatus status = ZydisDecoderDecodeFull(
            &decoder,
            code + offset,
            size - offset,
            &instruction,
            operands
        );

        uint64_t address = runtime_address + offset;

        if (ZYAN_SUCCESS(status)) {
            char text[256];
            ZydisFormatterFormatInstruction(
                &formatter,
                &instruction,
                operands,
                instruction.operand_count_visible,
                text,
                sizeof(text),
                address,
                ZYAN_NULL
            );

            std::fprintf(fp, "%016llX  %s\n", (unsigned long long)address, text);
            offset += instruction.length;
        }
        else {
            std::fprintf(fp, "%016llX  (byte invalido: 0x%02X)\n",
                (unsigned long long)address, code[offset]);
            offset += 1;
        }
    }

}