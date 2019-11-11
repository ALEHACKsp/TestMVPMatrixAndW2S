#pragma once
#include <vector>

uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);

uintptr_t GetBaseAddress(uintptr_t gameModule, uintptr_t address, std::vector<unsigned int> offsets) {
	return FindDMAAddy(gameModule + address, offsets);
}

uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets) {
	uintptr_t address = ptr;

	for(unsigned int i = 0; i < offsets.size(); ++i) {
		address = *(uintptr_t*)address;
		address += offsets[i];
	}

	return address;
}