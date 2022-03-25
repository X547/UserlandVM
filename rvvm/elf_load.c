/*
elf_load.c - ELF loader
Copyright (C) 2021  cerg2010cerg2010 <github.com/cerg2010cerg2010>

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

// Currently broken
#if 0

#include "elf_load.h"
#include <stddef.h>

#if !defined _WIN32 && !defined __APPLE__

#include <stdio.h>
#include <string.h>

#include "bit_ops.h"
#include "riscv32.h"
#include "mem_ops.h"
#include "riscv32_csr.h"
#include "riscv32_mmu.h"

#include <elf.h>

#ifndef PN_XNUM
#define PN_XNUM 0xffff
#endif

bool riscv32_elf_load_by_path(rvvm_hart_t *vm, const char *path, bool use_mmu, ptrdiff_t offset)
{
	FILE *fp = fopen(path, "rb");
	if (!fp)
	{
		printf("Unable to open ELF file %s\n", path);
		return false;
	}

	bool status = true;
	Elf32_Ehdr ehdr;
	if (1 != fread(&ehdr, sizeof(ehdr), 1, fp))
	{
		printf("Unable to read ELF file header of %s\n", path);
		status = false;
		goto err_fclose;

	}

	size_t phdr_num = ehdr.e_phnum;
	if (phdr_num == PN_XNUM)
	{
		if (ehdr.e_shnum <= 0)
		{
			printf("Unable to load ELF section header of %s - section count is 0\n", path);
			status = false;
			goto err_fclose;
		}

		if (fseek(fp, ehdr.e_shoff, SEEK_SET))
		{
			printf("Unable to load ELF section header of %s - unable to seek to section offset\n", path);
			status = false;
			goto err_fclose;
		}

		Elf32_Shdr shdr;
		if (1 != fread(&shdr, sizeof(shdr), 1, fp))
		{
			printf("Unable to load ELF section header of %s\n", path);
			status = false;
			goto err_fclose;
		}

		phdr_num = shdr.sh_info;
	}

	size_t pgd = 0;
	size_t load_addr = offset;
	if (use_mmu)
	{
		/* set up page table */
		/* XXX: need to restore this on error exit? */

		/* put memory mappings at the end of physical memory to avoid overlap with program */
		pgd = vm->mem.begin + vm->mem.size - 4096;
		uint32_t satp = (pgd >> 12) | (1 << 31);
		riscv32_csr_op(vm, 0x180, &satp, CSR_SWAP);
	}

	for (size_t phdr_start = ehdr.e_phoff;
			phdr_start < ehdr.e_phoff + phdr_num * ehdr.e_phentsize;
			phdr_start += ehdr.e_phentsize)
	{
		if (fseek(fp, phdr_start, SEEK_SET))
		{
			printf("Unable to load ELF program header of %s - unable to seek to header offset 0x%zx\n", path, phdr_start);
			status = false;
			goto err_fclose;
		}

		Elf32_Phdr phdr;
		if (1 != fread(&phdr, sizeof(phdr), 1, fp))
		{
			printf("Unable to load ELF program header of %s - unable to read header offset 0x%zx\n", path, phdr_start);
			status = false;
			goto err_fclose;
		}

		if (phdr.p_type != PT_LOAD)
		{
			continue;
		}

		size_t dest_addr = load_addr + phdr.p_vaddr;
		if (dest_addr < vm->mem.begin || dest_addr + phdr.p_memsz >= vm->mem.begin + vm->mem.size)
		{
			printf("Unable to load ELF segment at offset 0x%zx of file %s - segment doesnt't fit in memory\n", phdr_start, path);
			printf("load addr: 0x%zx p_memsz: 0x%x end: 0x%zx\n", dest_addr, phdr.p_memsz, dest_addr + phdr.p_memsz);
			printf("mem begin: 0x%"PRIxXLEN" size: 0x%"PRIxXLEN" end: 0x%"PRIxXLEN"\n", vm->mem.begin, vm->mem.size, vm->mem.begin + vm->mem.size);
			status = false;
			goto err_fclose;
		}

		if (fseek(fp, phdr.p_offset, SEEK_SET))
		{
			printf("Unable to load ELF segment at offset 0x%zx of file %s - unable to seek to start of the segment\n", phdr_start, path);
			status = false;
			goto err_fclose;
		}

		vmptr_t temp_mem = malloc(phdr.p_filesz);
		if (temp_mem == NULL) {
			printf("Unable to allocate memory of size %d\n", phdr.p_filesz);
			status = false;
			goto err_fclose;
		}

		if (1 != fread(temp_mem, phdr.p_filesz, 1, fp))
		{
			printf("Unable to read ELF segment at offset 0x%zx of file %s\n", phdr_start, path);
			status = false;
			goto err_fclose;
		}

		riscv32_physmem_op(vm, dest_addr, temp_mem, phdr.p_filesz, MMU_WRITE);
		free(temp_mem);

		/* fill .bss */
		temp_mem = calloc(1, phdr.p_memsz - phdr.p_filesz);
		if (temp_mem == NULL) {
			printf("Unable to allocate memory of size %d\n", phdr.p_memsz - phdr.p_filesz);
			status = false;
			goto err_fclose;
		}

		riscv32_physmem_op(vm, dest_addr + phdr.p_filesz, temp_mem, phdr.p_memsz - phdr.p_filesz, MMU_WRITE);
		free(temp_mem);

		if (!use_mmu)
		{
			/* adjust entry point offset */
			if (ehdr.e_entry >= phdr.p_vaddr && ehdr.e_entry < phdr.p_vaddr + phdr.p_memsz)
			{
				ehdr.e_entry = ehdr.e_entry + load_addr;
			}

			continue;
		}

		/* fill page table */
		size_t segment_start_addr = dest_addr;

		size_t pte1_start = vm->root_page_table + GET_VPN1(phdr.p_vaddr) * 4;
		size_t pte1_count = GET_VPN1(phdr.p_memsz + bit_mask(22));//(page_count + gen_mask(10)) >> 10;
		size_t page_count = (phdr.p_memsz + bit_mask(12)) >> 12;

		size_t pte = MMU_VALID_PTE
			| MMU_PAGE_ACCESSED | MMU_PAGE_DIRTY /* optimize */
			| MMU_READ * !!(phdr.p_flags & PF_R)
			| MMU_WRITE * !!(phdr.p_flags & PF_W)
			| MMU_EXEC * !!(phdr.p_flags & PF_X);

		for (size_t i = 0; i < pte1_count; ++i)
		{
			size_t pte1_addr = pte1_start + i * 4;
			uint32_t pte1;
			riscv32_physmem_op(vm, pte1_addr, (vmptr_t)&pte1, sizeof(pte1), MMU_READ);
			pte1 = read_uint32_le(&pte1);

			size_t pte0_start;
			if (pte1 & MMU_VALID_PTE)
			{
				pte0_start = GET_PHYS_ADDR(pte1);
			}
			else
			{
				pgd -= 4096;
				pte0_start = pgd;
				assert((pte0_start & bit_mask(12)) == 0);

				uint32_t data = SET_PHYS_ADDR(MMU_VALID_PTE, pte0_start);
				riscv32_physmem_op(vm, pte1_addr, (vmptr_t)&data, sizeof(data), MMU_WRITE);
			}
			pte0_start += GET_VPN2(phdr.p_vaddr) * 4;

			size_t pte0_count = page_count > (1 << 10) ? (1 << 10) : page_count;

			for (size_t j = 0; j < pte0_count; ++j)
			{
				size_t pte0_addr = pte0_start + j * 4;

				uint32_t data = SET_PHYS_ADDR(pte, segment_start_addr + j * 4096);
				riscv32_physmem_op(vm, pte0_addr, (vmptr_t)&data, sizeof(data), MMU_WRITE);
			}

			page_count -= pte0_count;
			segment_start_addr += pte0_count * 4096;
		}

		assert(page_count == 0);
	}

	vm->registers[REGISTER_PC] = ehdr.e_entry;

#ifdef RV_DEBUG
	if (use_mmu)
	{
		riscv32_mmu_dump(vm);
	}
#endif

err_fclose:
	fclose(fp);
	return status;
}

#else

bool riscv32_elf_load_by_path(rvvm_hart_t *vm, const char *path, bool use_mmu, ptrdiff_t offset)
{
    UNUSED(vm);
    UNUSED(path);
    UNUSED(use_mmu);
    UNUSED(offset);
    return false;
}

#endif
#endif
