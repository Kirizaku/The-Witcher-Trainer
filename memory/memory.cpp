/*
* Memory Hacking Library C++
* by Kirizaku
* version 1.0.2
* https://github.com/kirizaku/memory

Licensed under the MIT License <http://opensource.org/licenses/MIT>.

Copyright (c) 2022 Rudeus Kirigaya <https://github.com/kirizaku>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "memory.h"

#if defined(__linux__)
void* mem::inject_syscall(mem_pid_t pid, int syscall_id, void* arg0, void* arg1, void* arg2, void* arg3, void* arg4, void* arg5)
{
	void* result = 0;
    ptrace(PTRACE_ATTACH, pid, 0, 0);
	
	int status;
    wait(&status);

	struct user_regs_struct regs, regs_backup;
    ptrace(PTRACE_GETREGS, pid, 0, &regs_backup); 

    void* inject_address;
	regs = regs_backup;
	#if defined(__x86_64__)
    regs.rax = (uintptr_t)syscall_id;
    regs.rdi = (uintptr_t)arg0;
    regs.rsi = (uintptr_t)arg1;
    regs.rdx = (uintptr_t)arg2;
    regs.r10 = (uintptr_t)arg3;
    regs.r8  = (uintptr_t)arg4;
    regs.r9  = (uintptr_t)arg5;

	inject_address = (void*)regs.rip;
	#else
	regs.eax = (uintptr_t)syscall_id;
    regs.ebx = (uintptr_t)arg0;
    regs.ecx = (uintptr_t)arg1;
    regs.edx = (uintptr_t)arg2;
    regs.esi = (uintptr_t)arg3;
    regs.edi = (uintptr_t)arg4;
    regs.ebp = (uintptr_t)arg5;

	inject_address = (void*)regs.eip;
	#endif

	const uint8_t syscall_buffer[] = { 
	#if defined(__x86_64__)
	0x0F,0x05,
	#else
	0xcd,0x80,
	#endif
	0x00,0x00,0x00,0x00,0x00,0x00
	}; //Fast System Call

	uintptr_t injection_syscall;
	std::memcpy(&injection_syscall, syscall_buffer, sizeof(injection_syscall));

	uintptr_t old_poketext = ptrace(PTRACE_PEEKTEXT, pid, (void*)inject_address, 0);
	ptrace(PTRACE_POKETEXT, pid, (void*)inject_address, (void*)injection_syscall);
    ptrace(PTRACE_SETREGS, pid, 0, &regs);
    ptrace(PTRACE_SINGLESTEP, pid, 0, 0); 

    waitpid(pid, &status, WSTOPPED);
	
    ptrace(PTRACE_GETREGS, pid, 0, &regs);

	#if defined(__x86_64__)
    result = (void*)regs.rax;
	#else
	result = (void*)regs.eax;
	#endif

	ptrace(PTRACE_POKETEXT, pid, (void*)inject_address, old_poketext);	
    ptrace(PTRACE_SETREGS, pid, 0, &regs_backup);
    ptrace(PTRACE_DETACH, pid, 0, 0);

	return result;
}
#endif

mem::mem_pid_t mem::get_pid(string_t process_name) {
	mem_pid_t pid = 0;
	#if defined(_WIN32)
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 process_entry;
		process_entry.dwSize = sizeof(process_entry);

		if (Process32First(hSnap, &process_entry)) {
			do {
				if (!mem_cmp(process_entry.szExeFile, process_name.c_str())) {
					pid = process_entry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &process_entry));
		}
	}
	CloseHandle(hSnap);
	#else
	struct dirent *dirent_object;
	DIR* dir = opendir("/proc");
	if (!dir) return pid;
	
    while ((dirent_object = readdir(dir)) != 0)
	{
		if (atoi(dirent_object->d_name) != 0)
		{
			std::string comm_file = std::string("/proc/") + dirent_object->d_name + "/comm";
			std::ifstream comm_ifs(comm_file.c_str());
			if(!comm_ifs.is_open()) return pid;

			std::ostringstream comm_ost;
			comm_ost << comm_ifs.rdbuf();
			if (std::strstr(comm_ost.str().c_str(), process_name.c_str()))
			{
				pid = atoi(dirent_object->d_name);
				comm_ifs.close();
				break;
			}
			comm_ifs.close();
		} 
	}
	closedir(dir);
	#endif
	return pid;
}

mem::string_t mem::get_process_name(mem_pid_t pid) {
	string_t process_name;
	#if defined(_WIN32)
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 process_entry;
		process_entry.dwSize = sizeof(process_entry);

		if (Process32First(hSnap, &process_entry)) {
			do {
				if (pid == process_entry.th32ProcessID) {
					process_name = string_t(process_entry.szExeFile);
					break;
				}
			} while (Process32Next(hSnap, &process_entry));
		}
	}
	CloseHandle(hSnap);
	#else
	std::stringstream comm_file;
	comm_file << "/proc/" << pid << "/comm";
	std::ifstream comm_ifst(comm_file.str());

	if(!comm_ifst.is_open()) return process_name;

	comm_file.str(std::string());
	comm_file << comm_ifst.rdbuf();

	std::string comm_string = comm_file.str();
	comm_string.erase(comm_string.find("\n"), 1);
	process_name = comm_string.c_str();
	comm_ifst.close();
	#endif
	return process_name;
}

uintptr_t mem::get_module(mem_pid_t pid, string_t module_name) {
	uintptr_t module_base = 0;
	#if defined(_WIN32)
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
	if (hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 module_entry;
		module_entry.dwSize = sizeof(module_entry);
		if (Module32First(hSnap, &module_entry)) {
			do {
				if (!mem_cmp(module_entry.szModule, module_name.c_str()) || !mem_cmp(module_entry.szExePath, module_name.c_str())) {
					module_base = (uintptr_t)module_entry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &module_entry));
		}
	}
	CloseHandle(hSnap);
	#else
	std::stringstream maps_file;
	maps_file << "/proc/" << pid << "/maps";
	std::ifstream maps_ifst(maps_file.str());

	if(!maps_ifst.is_open()) return module_base;

	maps_file.str(std::string());
	maps_file << maps_ifst.rdbuf();

	size_t module_base_path = maps_file.str().find(module_name);
	size_t module_base_start = maps_file.str().rfind('\n', module_base_path);
	if (module_base_start == maps_file.str().npos) { module_base_start = 0; }

	size_t module_base_end = maps_file.str().find('-', module_base_path);
	if(module_base_end == maps_file.str().npos) return module_base;
	
	module_base = std::stoul(maps_file.str().substr(module_base_start, module_base_end - module_base_start), nullptr, 16);
    maps_ifst.close();
	#endif
	return module_base;
}

uintptr_t mem::load_module(mem_pid_t pid, string_t path) {
	uintptr_t module_base = 0;
	#if defined(_WIN32)
	size_t path_size = mem_len(path.c_str()) + 1;
	void* path_address = allocate(pid, 0, path_size, PAGE_EXECUTE_READWRITE);
	if (path_address == 0) return module_base;
	write(pid, path_address, (LPCVOID*)path.c_str(), path_size);

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (hProcess && hProcess != INVALID_HANDLE_VALUE) {
		HANDLE hThread = (HANDLE)CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibrary, path_address, 0, 0);
		if (hThread) {
			WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);
		}
		CloseHandle(hProcess);
	}
	deallocate(pid, path_address, path_size);
	#else
	//WIP
	#endif
	module_base = get_module(pid, path);
	return module_base;
}

void* mem::allocate(mem_pid_t pid, void* src, size_t size, uintptr_t protection) {
	void* result = 0;
	#if defined(_WIN32)
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess || hProcess == INVALID_HANDLE_VALUE) return result;
	result = VirtualAllocEx(hProcess, 0, size, MEM_COMMIT | MEM_RESERVE, protection);
	if (!result) result = FALSE;
	CloseHandle(hProcess);
	#else
	int syscall_id = 0;
	#if defined(__x86_64__)
	syscall_id = __NR_mmap;
	#else
	syscall_id = __NR_mmap2;
	#endif

	result = inject_syscall(pid, syscall_id, 0, (void*)size, (void*)protection, (void*)(MAP_PRIVATE | MAP_ANON), 0, 0);
	if (result == ((void *)syscall_id) || result == MAP_FAILED) result = 0;
	#endif
	return result;
}

bool mem::deallocate(mem_pid_t pid, void* src, size_t size) {
	bool result = 0;
	#if defined(_WIN32)
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess || hProcess == INVALID_HANDLE_VALUE) return result;
	result = VirtualFreeEx(hProcess, src, 0, MEM_RELEASE) != 0 ? true : false;
	CloseHandle(hProcess);
	#else
	result = inject_syscall(pid, __NR_munmap, src, (void*)size, 0, 0, 0, 0) == 0 ? true : false;
	#endif
	return result;
}

bool mem::protect(mem_pid_t pid, void* src, size_t size, uintptr_t protection, uintptr_t *old_protection) {
	bool result = 0;
	#if defined(_WIN32)
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	DWORD old_protect = 0;
	if (!hProcess || hProcess == INVALID_HANDLE_VALUE) return result;
	result = VirtualProtectEx(hProcess, src, size, protection, &old_protect) != 0 ? true : false;
	if (old_protection) *old_protection = old_protect;
	CloseHandle(hProcess);
	#else
	if (old_protection) {
	std::stringstream maps_file;
	maps_file << "/proc/" << pid << "/maps";
	std::ifstream maps_ifst(maps_file.str());

	if(!maps_ifst.is_open()) return result;

	maps_file.str(std::string());
	maps_file << maps_ifst.rdbuf();

	std::stringstream strm;
	strm << src;
	std::string addr = strm.str();
	addr.erase(addr.find("0x"), 2);
	addr = addr + "-";

    size_t addr_protection_path = maps_file.str().find(addr);
	if (addr_protection_path == maps_file.str().npos) { *old_protection = 0; return result; }

	size_t addr_protection_start = maps_file.str().find(' ', addr_protection_path);
	if (addr_protection_start == maps_file.str().npos) { *old_protection = 0; return result; }

	size_t end = addr_protection_start + 4;

	intptr_t prot = 0;
	for(size_t i = addr_protection_start; i < end; i++)
	{
		char c = maps_file.str()[i];
		switch(c) {
			case 'r': 	prot |= PROT_READ;		break;
			case 'w':	prot |= PROT_WRITE;		break;
			case 'x':	prot |= PROT_EXEC; 		break;
		}
	}
	*old_protection = prot;
    maps_ifst.close();
	}
	result = inject_syscall(pid, __NR_mprotect, src, (void*)size, (void*)protection, 0, 0, 0) == 0 ? true : false;
	#endif
	return result;
}

bool mem::query(mem_pid_t pid, void* src, memory_information *mi)
{
	bool result = 0;
	#if defined(_WIN32)
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess || hProcess == INVALID_HANDLE_VALUE) return result;

	MEMORY_BASIC_INFORMATION mbi;
	result = (size_t)VirtualQueryEx(hProcess, (LPCVOID)src, &mbi, sizeof(mbi)) != 0 ? true : false;
	if (result) {
		mi->base_address		= mbi.BaseAddress;
		mi->allocation_base		= mbi.AllocationBase;
		mi->allocation_protect	= mbi.AllocationProtect;
		#if defined(_WIN64)
		mi->partition_id		= mbi.PartitionId;
		#endif
		mi->size				= mbi.RegionSize;
		mi->state				= mbi.State;
		mi->protect				= mbi.Protect;
		mi->type				= mbi.Type;
	}
	CloseHandle(hProcess);
	#else
	std::stringstream maps_file;
	maps_file << "/proc/" << pid << "/maps";
	std::ifstream maps_ifst(maps_file.str());

	if(!maps_ifst.is_open()) return result;

	maps_file.str(std::string());
	maps_file << maps_ifst.rdbuf();

	bool temp = false;
	uintptr_t base_address = 0;
	size_t base_address_start = 0;
	size_t base_address_end = 0;
	size_t base_address_end_temp = 0;

    while (true)
	{
		if (temp) {
			base_address_start = maps_file.str().find("\n", base_address_end_temp);
		 	if (base_address_start == maps_file.str().npos) return result;
		}

		base_address_end_temp = maps_file.str().find('-', base_address_start);
		if(base_address_end_temp == maps_file.str().npos) return result;
		
		uintptr_t base_address_temp = std::stoul(maps_file.str().substr(base_address_start, base_address_end_temp - base_address_start), nullptr, 16);

		if ((void*)base_address_temp < src) { 
			base_address = base_address_temp;
			base_address_end = base_address_end_temp;
		}
		else if (!base_address || (void*)base_address_temp == src) {
			base_address = base_address_temp;
			base_address_end = base_address_end_temp;
			mi->base_address = (void*)base_address;
			uintptr_t next_address_start = maps_file.str().find("\n", base_address_end_temp);
			uintptr_t next_address_end = maps_file.str().find('-', next_address_start);
			if(next_address_end == maps_file.str().npos) { mi->next_address = 0; break; }
			uintptr_t next_address = std::stoul(maps_file.str().substr(next_address_start, next_address_end - next_address_start), nullptr, 16);		
			mi->next_address = (void*)next_address;
			break;
		}
		else { 
			mi->base_address = (void*)base_address;
			mi->next_address = (void*)base_address_temp;
			break; 
		}
		temp = true;
	}
	size_t end_address_start = maps_file.str().find(' ', base_address_end);
	if(end_address_start == maps_file.str().npos) return result;
	uintptr_t end_address = std::stoul(maps_file.str().substr(base_address_end + 1, end_address_start - (base_address_end + 1)), nullptr, 16);

	uintptr_t address_size = end_address - base_address;

	intptr_t prot = 0, flags = 0;
	size_t end = end_address_start + 5;
	for(size_t i = end_address_start; i < end; i++)
	{
		char c = maps_file.str()[i];
		switch(c) {
			case 'r': 	prot |= PROT_READ; 	 	break;
			case 'w':	prot |= PROT_WRITE;	 	break;
			case 'x':	prot |= PROT_EXEC;	 	break;
			case 'p':	flags = MAP_PRIVATE; 	break;
			case 's':	flags = MAP_SHARED;		break;
		}
	}
	maps_ifst.close();

	mi->size = address_size;
	mi->protect = prot;
	mi->type = flags;
	
	result = true;
	#endif
	return result;
}

bool mem::read(mem_pid_t pid, void* src, void* dst, size_t size) {
	bool result = 0;
	#if defined(_WIN32)
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess || hProcess == INVALID_HANDLE_VALUE) return result;
	result = ReadProcessMemory(hProcess, (LPCVOID)src, dst, size, 0) != 0 ? true : false;
	CloseHandle(hProcess);
	#else
	struct iovec iosrc;
	struct iovec iodst;
	iodst.iov_base = dst;
	iodst.iov_len  = size;
	iosrc.iov_base = src;
	iosrc.iov_len  = size;
	result = (size_t)process_vm_readv(pid, &iodst, 1, &iosrc, 1, 0) == size ? true : false;
	#endif
	return result;
}

bool mem::write(mem_pid_t pid, void* src, void* dst, size_t size) {
	bool result = 0;
	#if defined(_WIN32)
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess || hProcess == INVALID_HANDLE_VALUE) return result;
	result = WriteProcessMemory(hProcess, src, (LPCVOID)dst, size, 0) != 0 ? true : false;
	CloseHandle(hProcess);
	#else
	struct iovec iosrc;
	struct iovec iodst;
	iosrc.iov_base = src;
	iosrc.iov_len = size;
	iodst.iov_base = dst;
	iodst.iov_len = size;
	result = (size_t)process_vm_writev(pid, &iodst, 1, &iosrc, 1, 0) == size ? true : false;
	#endif
	return result;
}

bool mem::nop_ex(mem_pid_t pid, void* src, size_t size) {
	bool result = 0;
	std::vector<uint8_t> nop;
	nop.resize(size);
	std::fill(nop.begin(), nop.end(), 0x90);
	result = write(pid, src, nop.data(), size);
	return result;
}

uintptr_t mem::read_pointer(mem_pid_t pid, uintptr_t src, std::vector<uintptr_t> offsets) {
	uintptr_t addr = src;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		read(pid, (void*)addr, &addr, sizeof(addr));
		addr += offsets[i];
	}
	return addr;
}

uintptr_t mem::aob_scan(mem_pid_t pid, const char* pattern, const char* mask, char* begin, char* end)
{
	uintptr_t result = 0;
	char* current_address = begin;

	while (current_address < end)
	{
		memory_information mi;

		if (!query(pid, current_address, &mi))
		{
			return result;
		}

		#if defined(_WIN32)
		if (mi.state == MEM_COMMIT && mi.protect != protection::NONE)
		#else
		current_address = (char*)mi.base_address;
		if (mi.protect != protection::NONE)
		#endif
		{
			uintptr_t old_protect = 0;
			char* buffer = new char[mi.size];

			if (protect(pid, mi.base_address, mi.size, protection::READ_WRITE_EXECUTE, &old_protect))
			{
				read(pid, mi.base_address, buffer, mi.size);
				protect(pid, mi.base_address, mi.size, old_protect, &old_protect);

				char* internal_address = 0;
				size_t pattern_length = std::strlen(mask);

				for (size_t i = 0; i < mi.size - pattern_length; i++)
				{
					bool found = true;
					for (size_t j = 0; j < pattern_length; j++)
					{
						if (mask[j] != '?' && pattern[j] != *(buffer + i + j))
						{
							found = false;
							break;
						}
					}
					if (found)
					{
						internal_address = buffer + i;
					}
				}

				if (internal_address != 0)
				{
					uintptr_t offset_from_buffer = internal_address - buffer;
					result = (uintptr_t)current_address + offset_from_buffer;
					delete[] buffer;
					break;
				}
			}
			delete[] buffer;
		}
		#if defined(_WIN32)
		current_address = current_address + mi.size;
		#else
		current_address = (char*)mi.next_address;
		if (!current_address) return result;
		#endif
	}
	return result;
}