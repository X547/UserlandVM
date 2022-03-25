/*
riscv_i.c - RISC-V I Decoder, Interpreter
Copyright (C) 2021  LekKit <github.com/LekKit>
                    Mr0maks <mr.maks0443@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#define RISCV_CPU_SOURCE

#include "bit_ops.h"
#include "riscv_cpu.h"
#include "riscv_mmu.h"

static void riscv_i_lui(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Place sign-extended upper imm into register rds (lower 12 bits are zero)
    regid_t rds = bit_cut(instruction, 7, 5);
    sxlen_t imm = sign_extend(instruction & 0xFFFFF000, 32);

    rvjit_li(rds, imm, 4);

    riscv_write_register(vm, rds, imm);
}

static void riscv_i_auipc(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Add sign-extended upper imm to PC, place the result to register rds
    regid_t rds = bit_cut(instruction, 7, 5);
    sxlen_t imm = sign_extend(instruction & 0xFFFFF000, 32);
    xlen_t pc = riscv_read_register(vm, REGISTER_PC);

    rvjit_auipc(rds, imm, 4);

    riscv_write_register(vm, rds, pc + imm);
}

static inline sxlen_t decode_jal_imm(const uint32_t instruction)
{
    // May be replaced by translation table
    uint32_t imm = (bit_cut(instruction, 31, 1) << 20) |
                   (bit_cut(instruction, 12, 8) << 12) |
                   (bit_cut(instruction, 20, 1) << 11) |
                   (bit_cut(instruction, 21, 10) << 1);
    return sign_extend(imm, 21);
}

static void riscv_i_jal(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Store PC+4 to rds, jump to PC+offset; remember further PC increment!
    // offset is signed imm * 2, left shift one more bit for *2
    regid_t rds = bit_cut(instruction, 7, 5);
    sxlen_t offset = decode_jal_imm(instruction);
    xlen_t pc = riscv_read_register(vm, REGISTER_PC);

    rvjit_jal(rds, offset, 4);

    riscv_write_register(vm, rds, pc + 4);
    riscv_write_register(vm, REGISTER_PC, pc + offset - 4);
}

static void riscv_i_srli_srai(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Perform right arithmetical/logical bitshift on rs1 by imm, store into rds
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    uint8_t shamt = bit_cut(instruction, 20, SHAMT_BITS);
    xlen_t src_reg = riscv_read_register(vm, rs1);

    uint8_t funct7 = bit_cut(instruction, 20 + SHAMT_BITS, 12 - SHAMT_BITS);
    
    if (funct7 == (0x400 >> SHAMT_BITS)) {
        rvjit_srai(rds, rs1, shamt, 4);
        riscv_write_register(vm, rds, ((sxlen_t)src_reg) >> shamt);
    } else if (likely(funct7 == 0x0)) {
        rvjit_srli(rds, rs1, shamt, 4);
        riscv_write_register(vm, rds, src_reg >> shamt);
    } else {
        riscv_illegal_insn(vm, instruction);
    }
}

static void riscv_i_add_sub(rvvm_hart_t *vm, const uint32_t instruction)
{
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    regid_t rs2 = bit_cut(instruction, 20, 5);
    xlen_t reg1 = riscv_read_register(vm, rs1);
    xlen_t reg2 = riscv_read_register(vm, rs2);

    uint8_t funct7 = bit_cut(instruction, 25, 7);

    if (funct7 == 0x20) {
        rvjit_sub(rds, rs1, rs2, 4);
        riscv_write_register(vm, rds, reg1 - reg2);
    } else if (likely(funct7 == 0x0)) {
        rvjit_add(rds, rs1, rs2, 4);
        riscv_write_register(vm, rds, reg1 + reg2);
    } else {
        riscv_illegal_insn(vm, instruction);
    }
}

static void riscv_i_srl_sra(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Perform right arithmetical/logical bitshift on rs1 by rs2, store into rds
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    regid_t rs2 = bit_cut(instruction, 20, 5);
    xlen_t reg1 = riscv_read_register(vm, rs1);
    xlen_t reg2 = riscv_read_register(vm, rs2);

    uint8_t funct7 = bit_cut(instruction, 25, 7);
    
    if (funct7 == 0x20) {
        rvjit_sra(rds, rs1, rs2, 4);
        riscv_write_register(vm, rds, ((sxlen_t)reg1) >> (reg2 & bit_mask(SHAMT_BITS)));
    } else if (likely(funct7 == 0x0)) {
        rvjit_srl(rds, rs1, rs2, 4);
        riscv_write_register(vm, rds, reg1 >> (reg2 & bit_mask(SHAMT_BITS)));
    } else {
        riscv_illegal_insn(vm, instruction);
    }
}

static void riscv_i_jalr(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Save PC+4 to rds, jump to rs1+offset (offset is signed)
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    sxlen_t offset = sign_extend(bit_cut(instruction, 20, 12), 12);
    xlen_t pc = riscv_read_register(vm, REGISTER_PC);
    xlen_t jmp_addr = riscv_read_register(vm, rs1);

    rvjit_jalr(rds, rs1, offset, 4);

    riscv_write_register(vm, rds, pc + 4);
    riscv_write_register(vm, REGISTER_PC, ((jmp_addr + offset)&(~(xlen_t)1)) - 4);
}

static inline sxlen_t decode_branch_imm(const uint32_t instruction)
{
    // May be replaced by translation table
    const uint32_t imm = (bit_cut(instruction, 31, 1) << 12) |
                         (bit_cut(instruction, 7, 1)  << 11) |
                         (bit_cut(instruction, 25, 6) << 5)  |
                         (bit_cut(instruction, 8, 4)  << 1);
    return sign_extend(imm, 13);
}

static void riscv_i_beq(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Conditional jump when rs1 == rs2
    regid_t rs1 = bit_cut(instruction, 15, 5);
    regid_t rs2 = bit_cut(instruction, 20, 5);
    sxlen_t offset = decode_branch_imm(instruction);

    if (riscv_read_register(vm, rs1) == riscv_read_register(vm, rs2)) {
        xlen_t pc = riscv_read_register(vm, REGISTER_PC);

        rvjit_beq(rs1, rs2, offset, 4, 4);

        riscv_write_register(vm, REGISTER_PC, pc + offset - 4);
    } else {
        rvjit_bne(rs1, rs2, 4, offset, 4);
    }
}

static void riscv_i_bne(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Conditional jump when rs1 != rs2
    regid_t rs1 = bit_cut(instruction, 15, 5);
    regid_t rs2 = bit_cut(instruction, 20, 5);
    sxlen_t offset = decode_branch_imm(instruction);

    if (riscv_read_register(vm, rs1) != riscv_read_register(vm, rs2)) {
        xlen_t pc = riscv_read_register(vm, REGISTER_PC);

        rvjit_bne(rs1, rs2, offset, 4, 4);

        riscv_write_register(vm, REGISTER_PC, pc + offset - 4);
    } else {
        rvjit_beq(rs1, rs2, 4, offset, 4);
    }
}

static void riscv_i_blt(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Conditional jump when rs1 < rs2 (signed)
    regid_t rs1 = bit_cut(instruction, 15, 5);
    regid_t rs2 = bit_cut(instruction, 20, 5);
    sxlen_t offset = decode_branch_imm(instruction);

    if (riscv_read_register_s(vm, rs1) < riscv_read_register_s(vm, rs2)) {
        xlen_t pc = riscv_read_register(vm, REGISTER_PC);

        rvjit_blt(rs1, rs2, offset, 4, 4);

        riscv_write_register(vm, REGISTER_PC, pc + offset - 4);
    } else {
        rvjit_bge(rs1, rs2, 4, offset, 4);
    }
}

static void riscv_i_bge(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Conditional jump when rs1 >= rs2 (signed)
    regid_t rs1 = bit_cut(instruction, 15, 5);
    regid_t rs2 = bit_cut(instruction, 20, 5);
    sxlen_t offset = decode_branch_imm(instruction);

    if (riscv_read_register_s(vm, rs1) >= riscv_read_register_s(vm, rs2)) {
        xlen_t pc = riscv_read_register(vm, REGISTER_PC);

        rvjit_bge(rs1, rs2, offset, 4, 4);

        riscv_write_register(vm, REGISTER_PC, pc + offset - 4);
    } else {
        rvjit_blt(rs1, rs2, 4, offset, 4);
    }
}

static void riscv_i_bltu(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Conditional jump when rs1 < rs2
    regid_t rs1 = bit_cut(instruction, 15, 5);
    regid_t rs2 = bit_cut(instruction, 20, 5);
    sxlen_t offset = decode_branch_imm(instruction);

    if (riscv_read_register(vm, rs1) < riscv_read_register(vm, rs2)) {
        xlen_t pc = riscv_read_register(vm, REGISTER_PC);

        rvjit_bltu(rs1, rs2, offset, 4, 4);

        riscv_write_register(vm, REGISTER_PC, pc + offset - 4);
    } else {
        rvjit_bgeu(rs1, rs2, 4, offset, 4);
    }
}

static void riscv_i_bgeu(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Conditional jump when rs1 >= rs2
    regid_t rs1 = bit_cut(instruction, 15, 5);
    regid_t rs2 = bit_cut(instruction, 20, 5);
    sxlen_t offset = decode_branch_imm(instruction);

    if (riscv_read_register(vm, rs1) >= riscv_read_register(vm, rs2)) {
        xlen_t pc = riscv_read_register(vm, REGISTER_PC);

        rvjit_bgeu(rs1, rs2, offset, 4, 4);

        riscv_write_register(vm, REGISTER_PC, pc + offset - 4);
    } else {
        rvjit_bltu(rs1, rs2, 4, offset, 4);
    }
}

static void riscv_i_lb(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Read 8-bit signed integer from address rs1+offset (offset is signed) to rds
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    sxlen_t offset = sign_extend(bit_cut(instruction, 20, 12), 12);

    xaddr_t addr = riscv_read_register(vm, rs1) + offset;
    
    rvjit_lb(rds, rs1, offset, 4);

    riscv_load_s8(vm, addr, rds);
}

static void riscv_i_lh(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Read 16-bit signed integer from address rs1+offset (offset is signed) to rds
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    sxlen_t offset = sign_extend(bit_cut(instruction, 20, 12), 12);

    xaddr_t addr = riscv_read_register(vm, rs1) + offset;
    
    rvjit_lh(rds, rs1, offset, 4);

    riscv_load_s16(vm, addr, rds);
}

static void riscv_i_lw(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Read 32-bit signed integer from address rs1+offset (offset is signed) to rds
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    sxlen_t offset = sign_extend(bit_cut(instruction, 20, 12), 12);

    xaddr_t addr = riscv_read_register(vm, rs1) + offset;
    
    rvjit_lw(rds, rs1, offset, 4);

    riscv_load_s32(vm, addr, rds);
}

static void riscv_i_lbu(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Read 8-bit unsigned integer from address rs1+offset (offset is signed) to rds
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    sxlen_t offset = sign_extend(bit_cut(instruction, 20, 12), 12);

    xaddr_t addr = riscv_read_register(vm, rs1) + offset;
    
    rvjit_lbu(rds, rs1, offset, 4);

    riscv_load_u8(vm, addr, rds);
}

static void riscv_i_lhu(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Read 16-bit unsigned integer from address rs1+offset (offset is signed) to rds
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    sxlen_t offset = sign_extend(bit_cut(instruction, 20, 12), 12);

    xaddr_t addr = riscv_read_register(vm, rs1) + offset;
    
    rvjit_lhu(rds, rs1, offset, 4);

    riscv_load_u16(vm, addr, rds);
}

static void riscv_i_sb(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Write 8-bit integer rs2 to address rs1+offset (offset is signed)
    regid_t rs1 = bit_cut(instruction, 15, 5);
    regid_t rs2 = bit_cut(instruction, 20, 5);
    sxlen_t offset = sign_extend(bit_cut(instruction, 7, 5) |
                               (bit_cut(instruction, 25, 7) << 5), 12);

    xaddr_t addr = riscv_read_register(vm, rs1) + offset;
    
    rvjit_sb(rs2, rs1, offset, 4);

    riscv_store_u8(vm, addr, rs2);
}

static void riscv_i_sh(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Write 16-bit integer rs2 to address rs1+offset (offset is signed)
    regid_t rs1 = bit_cut(instruction, 15, 5);
    regid_t rs2 = bit_cut(instruction, 20, 5);
    sxlen_t offset = sign_extend(bit_cut(instruction, 7, 5) |
                               (bit_cut(instruction, 25, 7) << 5), 12);

    xaddr_t addr = riscv_read_register(vm, rs1) + offset;
    
    rvjit_sh(rs2, rs1, offset, 4);

    riscv_store_u16(vm, addr, rs2);
}

static void riscv_i_sw(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Write 32-bit integer rs2 to address rs1+offset (offset is signed)
    regid_t rs1 = bit_cut(instruction, 15, 5);
    regid_t rs2 = bit_cut(instruction, 20, 5);
    sxlen_t offset = sign_extend(bit_cut(instruction, 7, 5) |
                               (bit_cut(instruction, 25, 7) << 5), 12);

    xaddr_t addr = riscv_read_register(vm, rs1) + offset;
    
    rvjit_sw(rs2, rs1, offset, 4);

    riscv_store_u32(vm, addr, rs2);
}

static void riscv_i_addi(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Add signed immediate to rs1, store to rds
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    sxlen_t imm = sign_extend(bit_cut(instruction, 20, 12), 12);
    xlen_t src_reg = riscv_read_register(vm, rs1);

    rvjit_addi(rds, rs1, imm, 4);

    riscv_write_register(vm, rds, src_reg + imm);
}

static void riscv_i_slti(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Set rds to 1 if rs1 < imm (signed)
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    sxlen_t imm = sign_extend(bit_cut(instruction, 20, 12), 12);
    sxlen_t src_reg = riscv_read_register_s(vm, rs1);

    rvjit_slti(rds, rs1, imm, 4);

    riscv_write_register(vm, rds, (src_reg < imm) ? 1 : 0);
}

static void riscv_i_sltiu(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Set rds to 1 if rs1 < imm
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    xlen_t imm = sign_extend(bit_cut(instruction, 20, 12), 12);
    xlen_t src_reg = riscv_read_register(vm, rs1);

    rvjit_sltiu(rds, rs1, imm, 4);

    riscv_write_register(vm, rds, (src_reg < imm) ? 1 : 0);
}

static void riscv_i_xori(rvvm_hart_t *vm, const uint32_t instruction)
{
    // XOR rs1 with sign-extended imm, store to rds
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    sxlen_t imm = sign_extend(bit_cut(instruction, 20, 12), 12);
    xlen_t src_reg = riscv_read_register(vm, rs1);

    rvjit_xori(rds, rs1, imm, 4);

    riscv_write_register(vm, rds, src_reg ^ imm);
}

static void riscv_i_ori(rvvm_hart_t *vm, const uint32_t instruction)
{
    // OR rs1 with sign-extended imm, store to rds
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    sxlen_t imm = sign_extend(bit_cut(instruction, 20, 12), 12);
    xlen_t src_reg = riscv_read_register(vm, rs1);

    rvjit_ori(rds, rs1, imm, 4);

    riscv_write_register(vm, rds, src_reg | imm);
}

static void riscv_i_andi(rvvm_hart_t *vm, const uint32_t instruction)
{
    // AND rs1 with sign-extended imm, store to rds
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    sxlen_t imm = sign_extend(bit_cut(instruction, 20, 12), 12);
    xlen_t src_reg = riscv_read_register(vm, rs1);

    rvjit_andi(rds, rs1, imm, 4);

    riscv_write_register(vm, rds, src_reg & imm);
}

static void riscv_i_slli(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Left-shift rs1 by immediate, store to rds
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    uint8_t shamt = bit_cut(instruction, 20, SHAMT_BITS);
    xlen_t src_reg = riscv_read_register(vm, rs1);

    rvjit_slli(rds, rs1, shamt, 4);

    riscv_write_register(vm, rds, src_reg << shamt);
}

static void riscv_i_sll(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Left-shift rs1 by rs2, store to rds
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    regid_t rs2 = bit_cut(instruction, 20, 5);
    xlen_t reg1 = riscv_read_register(vm, rs1);
    xlen_t reg2 = riscv_read_register(vm, rs2);

    rvjit_sll(rds, rs1, rs2, 4);

    riscv_write_register(vm, rds, reg1 << (reg2 & bit_mask(SHAMT_BITS)));
}

static void riscv_i_slt(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Set rds to 1 if rs1 < rs2 (signed)
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    regid_t rs2 = bit_cut(instruction, 20, 5);
    sxlen_t reg1 = riscv_read_register_s(vm, rs1);
    sxlen_t reg2 = riscv_read_register_s(vm, rs2);

    rvjit_slt(rds, rs1, rs2, 4);

    riscv_write_register(vm, rds, (reg1 < reg2) ? 1 : 0);
}

static void riscv_i_sltu(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Set rds to 1 if rs1 < rs2
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    regid_t rs2 = bit_cut(instruction, 20, 5);
    xlen_t reg1 = riscv_read_register(vm, rs1);
    xlen_t reg2 = riscv_read_register(vm, rs2);

    rvjit_sltu(rds, rs1, rs2, 4);

    riscv_write_register(vm, rds, (reg1 < reg2) ? 1 : 0);
}

static void riscv_i_xor(rvvm_hart_t *vm, const uint32_t instruction)
{
    // XOR rs1 with rs2, store to rds
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    regid_t rs2 = bit_cut(instruction, 20, 5);
    xlen_t reg1 = riscv_read_register(vm, rs1);
    xlen_t reg2 = riscv_read_register(vm, rs2);

    rvjit_xor(rds, rs1, rs2, 4);

    riscv_write_register(vm, rds, reg1 ^ reg2);
}

static void riscv_i_or(rvvm_hart_t *vm, const uint32_t instruction)
{
    // OR rs1 with rs2, store to rds
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    regid_t rs2 = bit_cut(instruction, 20, 5);
    xlen_t reg1 = riscv_read_register(vm, rs1);
    xlen_t reg2 = riscv_read_register(vm, rs2);

    rvjit_or(rds, rs1, rs2, 4);

    riscv_write_register(vm, rds, reg1 | reg2);
}

static void riscv_i_and(rvvm_hart_t *vm, const uint32_t instruction)
{
    // AND rs1 with rs2, store to rds
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    regid_t rs2 = bit_cut(instruction, 20, 5);
    xlen_t reg1 = riscv_read_register(vm, rs1);
    xlen_t reg2 = riscv_read_register(vm, rs2);

    rvjit_and(rds, rs1, rs2, 4);

    riscv_write_register(vm, rds, reg1 & reg2);
}

#ifdef RV64

static void riscv64i_slliw(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Left-shift rs1 by immediate, store to rds (32 bit + signext)
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    uint8_t shamt = bit_cut(instruction, 20, 5);
    uint32_t src_reg = vm->registers[rs1];

    rvjit_slliw(rds, rs1, shamt, 4);

    vm->registers[rds] = (int32_t)(src_reg << shamt);
}

static void riscv64i_srliw_sraiw(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Perform right arithmetical/logical bitshift on rs1 by imm, store into rds (32 bit + signext)
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    uint8_t shamt = bit_cut(instruction, 20, 5);
    uint32_t src_reg = riscv_read_register(vm, rs1);

    uint8_t funct7 = bit_cut(instruction, 25, 7);
    
    if (funct7 == 0x20) {
        rvjit_sraiw(rds, rs1, shamt, 4);
        vm->registers[rds] = (int32_t)(((int32_t)src_reg) >> shamt);
    } else if (likely(funct7 == 0x0)) {
        rvjit_srliw(rds, rs1, shamt, 4);
        vm->registers[rds] = (int32_t)(src_reg >> shamt);
    } else {
        riscv_illegal_insn(vm, instruction);
    }
}

static void riscv64i_addw_subw(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Add/sub rs2 to rs1, store to rds (32 bit + signext)
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    regid_t rs2 = bit_cut(instruction, 20, 5);
    uint32_t reg1 = riscv_read_register(vm, rs1);
    uint32_t reg2 = riscv_read_register(vm, rs2);

    uint8_t funct7 = bit_cut(instruction, 25, 7);

    if (funct7 == 0x20) {
        rvjit_subw(rds, rs1, rs2, 4);
        vm->registers[rds] = (int32_t)(reg1 - reg2);
    } else if (likely(funct7 == 0x0)) {
        rvjit_addw(rds, rs1, rs2, 4);
        vm->registers[rds] = (int32_t)(reg1 + reg2);
    } else {
        riscv_illegal_insn(vm, instruction);
    }
}

static void riscv64i_sllw(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Left-shift rs1 by rs2, store to rds (32 bit + signext)
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    regid_t rs2 = bit_cut(instruction, 20, 5);
    uint32_t reg1 = riscv_read_register(vm, rs1);
    uint32_t reg2 = riscv_read_register(vm, rs2);

    rvjit_sllw(rds, rs1, rs2, 4);

    vm->registers[rds] = (int32_t)(reg1 << (reg2 & bit_mask(5)));
}

static void riscv64i_srlw_sraw(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Perform right arithmetical/logical bitshift on rs1 by rs2, store into rds (32 bit + signext)
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    regid_t rs2 = bit_cut(instruction, 20, 5);
    uint32_t reg1 = riscv_read_register(vm, rs1);
    uint32_t reg2 = riscv_read_register(vm, rs2);

    uint8_t funct7 = bit_cut(instruction, 25, 7);
    
    if (instruction >> 25) {
        rvjit_sraw(rds, rs1, rs2, 4);
        riscv_write_register(vm, rds, ((int32_t)reg1) >> (reg2 & bit_mask(5)));
    } else if (likely(funct7 == 0x0)) {
        rvjit_srlw(rds, rs1, rs2, 4);
        riscv_write_register(vm, rds, (int32_t)(reg1 >> (reg2 & bit_mask(5))));
    } else {
        riscv_illegal_insn(vm, instruction);
    }
}

static void riscv64i_addiw(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Add signed immediate to rs1, store to rds (32 bit + signext)
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    uint32_t imm = sign_extend(bit_cut(instruction, 20, 12), 12);
    uint32_t src_reg = riscv_read_register(vm, rs1);

    rvjit_addiw(rds, rs1, imm, 4);

    vm->registers[rds] = (int32_t)(src_reg + imm);
}

static void riscv64i_lwu(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Read 32-bit unsigned integer from address rs1+offset (offset is signed) to rds
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    sxlen_t offset = sign_extend(bit_cut(instruction, 20, 12), 12);

    xaddr_t addr = riscv_read_register(vm, rs1) + offset;
    
    rvjit_lwu(rds, rs1, offset, 4);

    riscv_load_u32(vm, addr, rds);
}

static void riscv64i_ld(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Read 64-bit integer from address rs1+offset (offset is signed) to rds
    regid_t rds = bit_cut(instruction, 7, 5);
    regid_t rs1 = bit_cut(instruction, 15, 5);
    sxlen_t offset = sign_extend(bit_cut(instruction, 20, 12), 12);

    xaddr_t addr = riscv_read_register(vm, rs1) + offset;
    
    rvjit_ld(rds, rs1, offset, 4);

    riscv_load_u64(vm, addr, rds);
}

static void riscv64i_sd(rvvm_hart_t *vm, const uint32_t instruction)
{
    // Write 64-bit integer rs2 to address rs1+offset (offset is signed)
    regid_t rs1 = bit_cut(instruction, 15, 5);
    regid_t rs2 = bit_cut(instruction, 20, 5);
    sxlen_t offset = sign_extend(bit_cut(instruction, 7, 5) |
                                (bit_cut(instruction, 25, 7) << 5), 12);

    xaddr_t addr = riscv_read_register(vm, rs1) + offset;
    
    rvjit_sd(rs2, rs1, offset, 4);

    riscv_store_u64(vm, addr, rs2);
}

#endif

void riscv_i_init(rvvm_hart_t* vm)
{
    riscv_install_opcode_UJ(vm, RVI_LUI, riscv_i_lui);
    riscv_install_opcode_UJ(vm, RVI_AUIPC, riscv_i_auipc);
    riscv_install_opcode_UJ(vm, RVI_JAL, riscv_i_jal);

#ifndef RV64
    riscv_install_opcode_R(vm, RVI_SLLI, riscv_i_slli);
    riscv_install_opcode_R(vm, RVI_SRLI_SRAI, riscv_i_srli_srai);
#endif
    riscv_install_opcode_R(vm, RVI_ADD_SUB, riscv_i_add_sub);
    riscv_install_opcode_R(vm, RVI_SRL_SRA, riscv_i_srl_sra);
    riscv_install_opcode_R(vm, RVI_SLL, riscv_i_sll);
    riscv_install_opcode_R(vm, RVI_SLT, riscv_i_slt);
    riscv_install_opcode_R(vm, RVI_SLTU, riscv_i_sltu);
    riscv_install_opcode_R(vm, RVI_XOR, riscv_i_xor);
    riscv_install_opcode_R(vm, RVI_OR, riscv_i_or);
    riscv_install_opcode_R(vm, RVI_AND, riscv_i_and);

    riscv_install_opcode_ISB(vm, RVI_JALR, riscv_i_jalr);
    riscv_install_opcode_ISB(vm, RVI_BEQ, riscv_i_beq);
    riscv_install_opcode_ISB(vm, RVI_BNE, riscv_i_bne);
    riscv_install_opcode_ISB(vm, RVI_BLT, riscv_i_blt);
    riscv_install_opcode_ISB(vm, RVI_BGE, riscv_i_bge);
    riscv_install_opcode_ISB(vm, RVI_BLTU, riscv_i_bltu);
    riscv_install_opcode_ISB(vm, RVI_BGEU, riscv_i_bgeu);
    riscv_install_opcode_ISB(vm, RVI_LB, riscv_i_lb);
    riscv_install_opcode_ISB(vm, RVI_LH, riscv_i_lh);
    riscv_install_opcode_ISB(vm, RVI_LW, riscv_i_lw);
    riscv_install_opcode_ISB(vm, RVI_LBU, riscv_i_lbu);
    riscv_install_opcode_ISB(vm, RVI_LHU, riscv_i_lhu);
    riscv_install_opcode_ISB(vm, RVI_SB, riscv_i_sb);
    riscv_install_opcode_ISB(vm, RVI_SH, riscv_i_sh);
    riscv_install_opcode_ISB(vm, RVI_SW, riscv_i_sw);
    riscv_install_opcode_ISB(vm, RVI_ADDI, riscv_i_addi);
    riscv_install_opcode_ISB(vm, RVI_SLTI, riscv_i_slti);
    riscv_install_opcode_ISB(vm, RVI_SLTIU, riscv_i_sltiu);
    riscv_install_opcode_ISB(vm, RVI_XORI, riscv_i_xori);
    riscv_install_opcode_ISB(vm, RVI_ORI, riscv_i_ori);
    riscv_install_opcode_ISB(vm, RVI_ANDI, riscv_i_andi);
#ifdef RV64
    riscv_install_opcode_ISB(vm, RVI_SLLI, riscv_i_slli);
    riscv_install_opcode_ISB(vm, RVI_SRLI_SRAI, riscv_i_srli_srai);
    
    riscv_install_opcode_R(vm, RV64I_SLLIW, riscv64i_slliw);
    riscv_install_opcode_R(vm, RV64I_SRLIW_SRAIW, riscv64i_srliw_sraiw);
    riscv_install_opcode_R(vm, RV64I_ADDW_SUBW, riscv64i_addw_subw);
    riscv_install_opcode_R(vm, RV64I_SLLW, riscv64i_sllw);
    riscv_install_opcode_R(vm, RV64I_SRLW_SRAW, riscv64i_srlw_sraw);

    riscv_install_opcode_ISB(vm, RV64I_ADDIW, riscv64i_addiw);
    riscv_install_opcode_ISB(vm, RV64I_LWU, riscv64i_lwu);
    riscv_install_opcode_ISB(vm, RV64I_LD, riscv64i_ld);
    riscv_install_opcode_ISB(vm, RV64I_SD, riscv64i_sd);
#else
    // Remove RV64I-only instructions from decoder
    riscv_install_opcode_R(vm, RV64I_SLLIW, riscv_illegal_insn);
    riscv_install_opcode_R(vm, RV64I_SRLIW_SRAIW, riscv_illegal_insn);
    riscv_install_opcode_R(vm, RV64I_ADDW_SUBW, riscv_illegal_insn);
    riscv_install_opcode_R(vm, RV64I_SLLW, riscv_illegal_insn);
    riscv_install_opcode_R(vm, RV64I_SRLW_SRAW, riscv_illegal_insn);

    riscv_install_opcode_ISB(vm, RV64I_ADDIW, riscv_illegal_insn);
    riscv_install_opcode_ISB(vm, RV64I_LWU, riscv_illegal_insn);
    riscv_install_opcode_ISB(vm, RV64I_LD, riscv_illegal_insn);
    riscv_install_opcode_ISB(vm, RV64I_SD, riscv_illegal_insn);
#endif
}
