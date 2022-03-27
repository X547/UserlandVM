#include "Loader.h"
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <OS.h>
#include <private/system/arch/riscv64/arch_elf.h>
#include <private/system/syscalls.h>


static void Assert(bool cond)
{
	if (!cond) abort();
}

static void FileRead(FILE *f, void *data, size_t size)
{
	Assert(fread(data, size, 1, f) == 1);
}

void ElfImage::LoadHeaders()
{
	FileRead(fFile.Get(), &fHeader, sizeof(fHeader));
	fPhdrs.SetTo(new Elf64_Phdr[fHeader.e_phnum]);
	for (uint32 i = 0; i < fHeader.e_phnum; i++) {
		fseek(fFile.Get(), fHeader.e_phoff + (off_t)i*fHeader.e_phentsize, SEEK_SET);
		FileRead(fFile.Get(), &fPhdrs[i], sizeof(Elf64_Phdr));
	}
}

void ElfImage::LoadSegments()
{
	Address minAdr = (Address)(-1), maxAdr = 0;
	for (uint32 i = 0; i < fHeader.e_phnum; i++) {
		Elf64_Phdr &phdr = fPhdrs[i];
		if (phdr.p_type == PT_LOAD) {
			minAdr = std::min<Address>(minAdr, phdr.p_vaddr);
			maxAdr = std::max<Address>(maxAdr, phdr.p_vaddr + phdr.p_memsz - 1);
		}
	}
	fSize = maxAdr - minAdr + 1;
	fArea.SetTo(create_area("image", &fBase, B_ANY_ADDRESS, fSize, B_NO_LOCK, B_READ_AREA | B_WRITE_AREA));
	fDelta = (Address)fBase - minAdr;
	printf("delta: %#" B_PRIx64 "\n", fDelta);
	fEntry = FromVirt(fHeader.e_entry);
	for (uint32 i = 0; i < fHeader.e_phnum; i++) {
		Elf64_Phdr &phdr = fPhdrs[i];
		switch (phdr.p_type) {
			case PT_LOAD: {
				fseek(fFile.Get(), phdr.p_offset, SEEK_SET);
				FileRead(fFile.Get(), FromVirt(phdr.p_vaddr), phdr.p_filesz);
				break;
			}
			case PT_DYNAMIC: {
				fDynamic = (Elf64_Dyn*)FromVirt(phdr.p_vaddr);
				break;
			}
		}
	}
}

void ElfImage::Relocate()
{
	Address relocAdr = 0, relocSize = 0;
	Address pltRelocAdr = 0, pltRelocSize = 0;
	if (fDynamic == NULL) return;
	for (Elf64_Dyn *dyn = fDynamic; dyn->d_tag != DT_NULL; dyn++) {
		switch (dyn->d_tag) {
			case DT_RELA: relocAdr = dyn->d_un.d_ptr; break;
			case DT_RELASZ: relocSize = dyn->d_un.d_ptr; break;
			case DT_JMPREL: pltRelocAdr = dyn->d_un.d_ptr; break;
			case DT_PLTRELSZ: pltRelocSize = dyn->d_un.d_ptr; break;
			case DT_SYMTAB: fSymbols = (Elf64_Sym*)FromVirt(dyn->d_un.d_ptr);
		}
	}
	if (relocSize > 0) DoRelocate((Elf64_Rela*)FromVirt(relocAdr), relocSize/sizeof(Elf64_Rela));
	if (pltRelocSize > 0) DoRelocate((Elf64_Rela*)FromVirt(pltRelocAdr), pltRelocSize/sizeof(Elf64_Rela));
}

void ElfImage::DoRelocate(Elf64_Rela *reloc, Address count)
{
	for (; count > 0; reloc++, count--) {
		uint32 kind = (uint32)(reloc->r_info);
		uint32 sym = (uint32)(reloc->r_info >> 32);
		Address *dst = (Address*)FromVirt(reloc->r_offset);
		Address src = reloc->r_addend;
		if (sym != 0) {
			src += (Address)FromVirt(fSymbols[sym].st_value);
		}
		switch (kind) {
			case R_RISCV_NONE:
				continue;
			case R_RISCV_64:
			case R_RISCV_JUMP_SLOT:
				*dst = src;
				break;
			case R_RISCV_RELATIVE:
				*dst = (Address)FromVirt(*dst + src);
				break;
			default:
				abort();
		}
	}
}

void ElfImage::Register()
{
	extended_image_info info {
		.basic_info = {
			.type = B_LIBRARY_IMAGE,
			//.name = "runtime_loader",
			.text = fBase,
			.text_size = fSize,
		},
		.text_delta = fDelta,
		.symbol_table = fSymbols
	};
	strcpy(info.basic_info.name, "runtime_loader");
	_kern_register_image(&info, sizeof(info));
}


ElfImage *ElfImage::Load(const char *path)
{
	ObjectDeleter<ElfImage> image(new ElfImage());
	image->fFile.SetTo(fopen(path, "rb"));
	image->LoadHeaders();
	image->LoadSegments();
	image->Relocate();
	//image->Register();
	return image.Detach();
}
