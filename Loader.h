#pragma once

#include <elf.h>
#include <AutoDeleter.h>
#include <AutoDeleterOS.h>
#include <AutoDeleterPosix.h>

struct Elf32Class {
	static const uint8 identClass = ELFCLASS32;

	typedef uint32 Address;
	typedef int32 PtrDiff;
	typedef Elf32_Ehdr Ehdr;
	typedef Elf32_Phdr Phdr;
	typedef Elf32_Dyn Dyn;
	typedef Elf32_Sym Sym;
	typedef Elf32_Rel Rel;
	typedef Elf32_Rela Rela;
};

struct Elf64Class {
	static const uint8 identClass = ELFCLASS64;

	typedef uint64 Address;
	typedef int64 PtrDiff;
	typedef Elf64_Ehdr Ehdr;
	typedef Elf64_Phdr Phdr;
	typedef Elf64_Dyn Dyn;
	typedef Elf64_Sym Sym;
	typedef Elf64_Rel Rel;
	typedef Elf64_Rela Rela;
};

class ElfImage {
protected:
	FileCloser fFile;
	ArrayDeleter<char> fPath;

	virtual void DoLoad() = 0;

public:
	virtual ~ElfImage() {}
	static ElfImage *Load(const char *path);
	virtual const char *GetArchString() = 0;
	virtual void *GetImageBase() = 0;
	virtual void *GetEntry() = 0;
	virtual bool FindSymbol(const char *name, void **adr, size_t *size) = 0;
};

template <typename Class>
class ElfImageImpl: public ElfImage {
private:
	typedef typename Class::Address Address;
	typedef typename Class::PtrDiff PtrDiff;

	typename Class::Ehdr fHeader;
	ArrayDeleter<typename Class::Phdr> fPhdrs;

	AreaDeleter fArea;
	void *fBase{};
	Address fSize{};
	PtrDiff fDelta{};

	void *fEntry{};
	typename Class::Dyn *fDynamic{};
	typename Class::Sym *fSymbols{};
	uint32 *fHash{};
	const char *fStrings{};

	void *FromVirt(Address virtAdr) {return (void*)(virtAdr + fDelta);}
	Address ToVirt(void *adr) {return ((Address)(addr_t)adr - fDelta);}

	void LoadHeaders();
	void LoadSegments();
	void Relocate();
	void Register();

	template<typename Reloc>
	void DoRelocate(Reloc *reloc, Address relocSize);

protected:
	virtual void DoLoad() override;

public:
	virtual ~ElfImageImpl() {}
	const char *GetArchString() override;
	void *GetImageBase() override;
	void *GetEntry() override {return fEntry;}
	bool FindSymbol(const char *name, void **adr, size_t *size) override;
};
