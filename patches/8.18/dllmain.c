#define BASE_ADDRESS 0x140000000
#include "helpers.h"

HOOK_DYNAMIC (u8, __fastcall, qrVtable1, u64 a1) { return 1; }
HOOK_DYNAMIC (u8, __fastcall, qrReadFromCOM1, u64 a1) {
	*(u32 *)(a1 + 40) = 1;
	*(u32 *)(a1 + 16) = 1;
	return 1;
}

void
PreInit () {
	toml_table_t *config = openConfig (configPath ("plugins/patches.toml"));
	if (!config) return;
	void *handle = GetModuleHandle (0);
	WRITE_MEMORY (ASLR (0x1400239C0, handle), u8, 0xC3);                                                          // Stop error
	if (readConfigBool (config, "unlock_songs", true)) WRITE_MEMORY (ASLR (0x140314E8D, handle), u8, 0xB0, 0x01); // Unlock songs
	if (readConfigBool (config, "shared_audio", true)) WRITE_MEMORY (ASLR (0x140692E17, handle), u8, 0xEB);       // Shared audio
	// Remove song limit
	WRITE_MEMORY (ASLR (0x140313726, handle), u8, 0x28, 0x23);
	/* Unfinished network stuff
	WRITE_MEMORY (ASLR (0x1402F39E6, handle), u8, 0x28, 0x23);
	WRITE_MEMORY (ASLR (0x1402F3AB0, handle), u8, 0x28, 0x23);
	WRITE_MEMORY (ASLR (0x1402F3BE4, handle), u8, 0x28, 0x23);
	WRITE_MEMORY (ASLR (0x14030643B, handle), u8, 0x28, 0x23);
	WRITE_MEMORY (ASLR (0x140306507, handle), u8, 0x28, 0x23);
	WRITE_MEMORY (ASLR (0x1403065D3, handle), u8, 0x28, 0x23);
	WRITE_MEMORY (ASLR (0x1403066FB, handle), u8, 0x28, 0x23);
	WRITE_MEMORY (ASLR (0x1403067C7, handle), u8, 0x28, 0x23);
	WRITE_MEMORY (ASLR (0x140306893, handle), u8, 0x28, 0x23);
	WRITE_MEMORY (ASLR (0x14030698B, handle), u8, 0x28, 0x23);
	WRITE_MEMORY (ASLR (0x140313666, handle), u8, 0x28, 0x23);
	WRITE_MEMORY (ASLR (0x140313B04, handle), u8, 0x28, 0x23);
	WRITE_MEMORY (ASLR (0x140313C24, handle), u8, 0x28, 0x23);
	WRITE_MEMORY (ASLR (0x140313CF4, handle), u8, 0x28, 0x23);
	WRITE_MEMORY (ASLR (0x1403140C4, handle), u8, 0x28, 0x23);
	WRITE_MEMORY (ASLR (0x1403147AA, handle), u8, 0x28, 0x23);
	*/
	if (!readConfigBool (config, "vsync", false)) WRITE_MEMORY (ASLR (0x140517339, handle), u8, 0xBA, 0x00, 0x00, 0x00, 0x00, 0x90); // Disable VSync
	// Save settings cross session without F:/ and G:/ drive
	WRITE_MEMORY (ASLR (0x140B5C528, handle), u8, "./Setting1.bin");
	WRITE_MEMORY (ASLR (0x140B5C538, handle), u8, "./Setting2.bin");
	// Move F:/ files to current directory
	WRITE_MEMORY (ASLR (0x140B1B4B0, handle), u8, "./");
	WRITE_MEMORY (ASLR (0x14001C941, handle), u8, 0x02);
	// Patch TLS v1.0 to v1.2
	WRITE_MEMORY (ASLR (0x14044b1a9, handle), u8, 0x10);

	// Move various files to current directory
	void *amHandle = GetModuleHandle ("AMFrameWork.dll");
	WRITE_MEMORY (amHandle + 0x33EF7, u8, 0xEB);
	WRITE_MEMORY (amHandle + 0x3404A, u8, 0xEB);
	WRITE_MEMORY (amHandle + 0x34429, u8, 0xEB);
	WRITE_MEMORY (amHandle + 0x3457C, u8, 0xEB);
	WRITE_MEMORY (amHandle + 0x3497A, u8, 0xEB);
	WRITE_MEMORY (amHandle + 0x34ACD, u8, 0xEB);
	WRITE_MEMORY (amHandle + 0x148AF, u8, 0xEB);
	WRITE_MEMORY (amHandle + 0x14A1A, u8, 0xEB);

	WRITE_MEMORY (amHandle + 0x3CB90, u8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46);

	INSTALL_HOOK_DYNAMIC (qrVtable1, amHandle + 0x1BA00);
	INSTALL_HOOK_DYNAMIC (qrReadFromCOM1, amHandle + 0x1BC20);
}
