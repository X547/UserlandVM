#pragma once

#include <elf.h>
#include <AutoDeleter.h>
#include <AutoDeleterOS.h>
#include <AutoDeleterPosix.h>


class ElfImage {
private:
	typedef uint64 Address;
	typedef int64 PtrDiff;

	FileCloser fFile;
	Elf64_Ehdr fHeader;
	ArrayDeleter<Elf64_Phdr> fPhdrs;

	AreaDeleter fArea;
	void *fBase = 0;
	Address fSize = 0;
	PtrDiff fDelta = 0;

	void *fEntry = 0;
	Elf64_Dyn *fDynamic = 0;
	Elf64_Sym *fSymbols = 0;

	void *FromVirt(Address virtAdr) {return (void*)(virtAdr + fDelta);}
	Address ToVirt(void *adr) {return ((Address)adr - fDelta);}

	void LoadHeaders();
	void LoadSegments();
	void Relocate();
	void Register();
	
	void DoRelocate(Elf64_Rela *reloc, Address count);

public:
	static ElfImage *Load(const char *path);
	void *GetEntry() {return fEntry;}
};
