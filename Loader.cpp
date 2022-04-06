#include "Loader.h"
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <type_traits>
#include <OS.h>
#include <private/system/arch/riscv64/arch_elf.h>
#include <private/system/syscalls.h>


area_id	vm32_create_area(const char *name, void **address, uint32 addressSpec, size_t size, uint32 lock, uint32 protection);


static void Assert(bool cond)
{
	if (!cond) abort();
}

static void FileRead(FILE *f, void *data, size_t size)
{
	Assert(fread(data, size, 1, f) == 1);
}

template<typename Class>
void ElfImageImpl<Class>::LoadHeaders()
{
	fseek(fFile.Get(), 0, SEEK_SET);
	FileRead(fFile.Get(), &fHeader, sizeof(fHeader));
	fPhdrs.SetTo(new typename Class::Phdr[fHeader.e_phnum]);
	for (uint32 i = 0; i < fHeader.e_phnum; i++) {
		fseek(fFile.Get(), fHeader.e_phoff + (off_t)i*fHeader.e_phentsize, SEEK_SET);
		FileRead(fFile.Get(), &fPhdrs[i], sizeof(typename Class::Phdr));
	}
}

template<typename Class>
void ElfImageImpl<Class>::LoadSegments()
{
	Address minAdr = (Address)(-1), maxAdr = 0;
	for (uint32 i = 0; i < fHeader.e_phnum; i++) {
		typename Class::Phdr &phdr = fPhdrs[i];
		if (phdr.p_type == PT_LOAD) {
			minAdr = std::min<Address>(minAdr, phdr.p_vaddr);
			maxAdr = std::max<Address>(maxAdr, phdr.p_vaddr + phdr.p_memsz - 1);
		}
	}
	fSize = maxAdr - minAdr + 1;
	if constexpr(std::is_same<Class, Elf32Class>()) {
		fArea.SetTo(vm32_create_area("image", &fBase, B_ANY_ADDRESS, fSize, B_NO_LOCK, B_READ_AREA | B_WRITE_AREA));
	} else {
		fArea.SetTo(create_area("image", &fBase, B_ANY_ADDRESS, fSize, B_NO_LOCK, B_READ_AREA | B_WRITE_AREA));
	}
	fDelta = (Address)(addr_t)fBase - minAdr;
	//printf("delta: %#" B_PRIx64 "\n", fDelta);
	fEntry = FromVirt(fHeader.e_entry);
	for (uint32 i = 0; i < fHeader.e_phnum; i++) {
		typename Class::Phdr &phdr = fPhdrs[i];
		switch (phdr.p_type) {
			case PT_LOAD: {
				fseek(fFile.Get(), phdr.p_offset, SEEK_SET);
				FileRead(fFile.Get(), FromVirt(phdr.p_vaddr), phdr.p_filesz);
				break;
			}
			case PT_DYNAMIC: {
				fDynamic = (typename Class::Dyn*)FromVirt(phdr.p_vaddr);
				break;
			}
		}
	}
}

template<typename Class>
void ElfImageImpl<Class>::Relocate()
{
	typename Class::Rel *relocAdr = NULL; Address relocSize = 0;
	typename Class::Rela *relocAAdr = NULL; Address relocASize = 0;
	Address pltRelocAdr = 0, pltRelocSize = 0;
	if (fDynamic == NULL) return;
	for (typename Class::Dyn *dyn = fDynamic; dyn->d_tag != DT_NULL; dyn++) {
		switch (dyn->d_tag) {
			case DT_REL: relocAdr = (typename Class::Rel*)FromVirt(dyn->d_un.d_ptr); break;
			case DT_RELSZ: relocSize = dyn->d_un.d_ptr; break;
			case DT_RELA: relocAAdr = (typename Class::Rela*)FromVirt(dyn->d_un.d_ptr); break;
			case DT_RELASZ: relocASize = dyn->d_un.d_ptr; break;
			case DT_JMPREL: pltRelocAdr = dyn->d_un.d_ptr; break;
			case DT_PLTRELSZ: pltRelocSize = dyn->d_un.d_ptr; break;
			case DT_SYMTAB: fSymbols = (typename Class::Sym*)FromVirt(dyn->d_un.d_ptr);
		}
	}
	if (relocAdr != NULL) DoRelocate<typename Class::Rel>(relocAdr, relocSize);
	if (relocAAdr != NULL) DoRelocate<typename Class::Rela>(relocAAdr, relocASize);
	if (pltRelocSize > 0) DoRelocate<typename Class::Rela>((typename Class::Rela*)FromVirt(pltRelocAdr), pltRelocSize);
}

template<typename Class>
template<typename Reloc>
void ElfImageImpl<Class>::DoRelocate(Reloc *reloc, Address relocSize)
{
	Address count = relocSize/sizeof(Reloc);
	for (; count > 0; reloc++, count--) {
		Address *dst = (Address*)FromVirt(reloc->r_offset);
		Address src;
		if constexpr (std::is_same<Reloc, typename Class::Rel>()) {
			src = *dst;
		} else if constexpr (std::is_same<Reloc, typename Class::Rela>()) {
			src = reloc->r_addend;
		} else {
			abort();
		}
		if (reloc->SymbolIndex() != 0) {
			src += (Address)(addr_t)FromVirt(fSymbols[reloc->SymbolIndex()].st_value);
		}

		switch (fHeader.e_machine) {
			case EM_386:
			case EM_486:
				switch (reloc->Type()) {
					case R_386_NONE:
						break;
					case R_386_RELATIVE:
						*dst = (Address)(addr_t)FromVirt(src);
						break;
					default:
						abort();
				}
				break;
			case EM_68K:
				abort();
				break;
			case EM_PPC:
				abort();
				break;
			case EM_ARM:
				abort();
				break;
			case EM_ARM64:
				abort();
				break;
			case EM_X86_64:
				abort();
				break;
			case EM_RISCV:
				switch (reloc->Type()) {
					case R_RISCV_NONE:
						break;
					case R_RISCV_64:
					case R_RISCV_JUMP_SLOT:
						*dst = src;
						break;
					case R_RISCV_RELATIVE:
						*dst = (Address)(addr_t)FromVirt(src);
						break;
					default:
						abort();
				}
				break;
			default:
				abort();
		}
	}
}

template<typename Class>
void ElfImageImpl<Class>::Register()
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

template<typename Class>
void ElfImageImpl<Class>::DoLoad()
{
	LoadHeaders();
	LoadSegments();
	Relocate();
	//Register();
}

template<typename Class>
const char *ElfImageImpl<Class>::GetArchString()
{
	switch (fHeader.e_machine) {
		case EM_386:
		case EM_486: return "x86";
		case EM_68K: return "m68k";
		case EM_PPC: return "ppc";
		case EM_ARM: return "arm";
		case EM_ARM64: return "arm64";
		case EM_X86_64: return "x86_64";
		case EM_RISCV: return "riscv";
	}
	return NULL;
}

template class ElfImageImpl<Elf32Class>;
template class ElfImageImpl<Elf64Class>;


ElfImage *ElfImage::Load(const char *path)
{
	ObjectDeleter<ElfImage> image;
	FileCloser file(fopen(path, "rb"));
	if (!file.IsSet()) abort();
	uint8 ident[EI_NIDENT];
	FileRead(file.Get(), ident, sizeof(ident));
	if (!(
		(ident[EI_MAG0] == ELFMAG0 &&
		ident[EI_MAG1] == ELFMAG1 &&
		ident[EI_MAG2] == ELFMAG2 &&
		ident[EI_MAG3] == ELFMAG3))
	) abort();
	switch (ident[EI_CLASS]) {
		case ELFCLASS32:
			image.SetTo(new ElfImageImpl<Elf32Class>());
			break;
		case ELFCLASS64:
			image.SetTo(new ElfImageImpl<Elf64Class>());
			break;
		default:
			abort();
	}

	image->fPath.SetTo(new char[strlen(path) + 1]);
	strcpy(image->fPath.Get(), path);
	image->fFile.SetTo(file.Detach());
	image->DoLoad();
	return image.Detach();
}
