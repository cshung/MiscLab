#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv)
{

  /*
   * If I run these code directly in Windows, it crashes, for I believe Windows 
   * do not allow accessing the BIOS directly, that make sense
   * 
   * But I can compile a binary, then I can use the debugger to view what machine code
   * does these correspond to, and build to boot program!
   */
  /*
  __asm
  {
    mov ah, 0x0a
    mov al, 0x41
    mov bh, 0
    mov bl, 3
    int 0x10
    hlt
  }
  */
  int disk_length = 80 * 18 * 512 * 2;
  char* disk = (char*)calloc(disk_length, sizeof(char));

  const char program[] = {0xb4,0x0a,0xb0,0x41,0xb7,0x00,0xb3,0x03,0xcd,0x10,0xf4};
  const char boot_signature[] = {0x55, 0xAA};

  const int program_length = _countof(program);
  const int boot_signature_length = _countof(boot_signature);

  // Be careful with file mode
  FILE* imgFile = fopen("disk.img", "wb");

  memcpy(disk, program, program_length);
  memcpy(disk + 510, boot_signature, boot_signature_length);

  int written = 0;
  while (written < disk_length)
  {
    written += fwrite(disk + written, sizeof(char), disk_length - written, imgFile);
  }
  fclose(imgFile);

  return 0;
}
