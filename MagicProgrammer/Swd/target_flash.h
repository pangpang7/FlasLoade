/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef TARGET_FLASH_H
#define TARGET_FLASH_H

#include "target_struct.h"
#include "swd_host.h"
#include <stdint.h>

#define FLASH_SECTOR_SIZE           (1024)  /* 1024 is assuming that this value is in number of uint32_t's */

#define TARGET_AUTO_INCREMENT_PAGE_SIZE    (0x1000)

static uint8_t target_flash_init(uint32_t clk);
static uint8_t target_flash_uninit(void);
static uint8_t target_flash_erase_chip(void);
static uint8_t target_flash_erase_sector(uint32_t adr);
static uint8_t target_flash_program_page(uint32_t adr, uint8_t * buf, uint32_t size);

static const uint32_t SI6600_FLM[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0x20004603, 0x46014770, 0x47702000, 0x491c2003, 0x481c6208, 0x60082100, 0x47702000, 0x20004601, 
    0x22026900, 0x22004310, 0x20006110, 0x69006141, 0x43102240, 0x61102200, 0x4813e002, 0x60104a13, 
    0x68c02000, 0x0fc007c0, 0xd1f62800, 0x22026900, 0x22004390, 0x20006110, 0x46034770, 0x47702001, 
    0x4603b510, 0x4c062001, 0xe0046220, 0x60186810, 0x1d121d1b, 0x29001f09, 0x2000d1f8, 0x0000bd10, 
    0x40020000, 0x66000001, 0x0000aaaa, 0x40003000, 0x00000000, 0x00000000, 
};

static const TARGET_FLASH flash = {
    0x20000021, // Init
    0x20000027, // UnInit
    0x2000002D, // EraseChip
    0x2000003D, // EraseSector
    0x20000081, // ProgramPage

    // static_base : base address is address of Execution Region PrgData in map file
    //               to access global/static data
    // stack_pointer : Initial stack pointer
    {0x20000001, 0x20000300, 0x20001000}, // {breakpoint, static_base, stack_pointer}

    0x20000500, // program_buffer
    0x20000000, // algo_start
    sizeof(SI6600_FLM), // algo_size

/*
    0x10000000   ----------------------  algo_start
                 | FLASH ALGORITHM    |
    0x10000001   |                    |  breakpoint
    0x1000003d   |                    |  Init
    0x1000008b   |                    |  UnInit
    0x1000008f   |                    |  EraseChip
    0x10000133   |                    |  EraseSector
    0x100001ab   |                    |  ProgramPage
                 |                    |
                 ----------------------  algo_start + algo_size
                 |      unused        |
    0x10000300   ----------------------  static_base
                 | GLOBAL/STATIC DATA |
                 |                    |
    0x10000500   ----------------------  program_buffer
                 | DATA TO BE WRITTEN |
                 |                    |
                 ----------------------  program_buffer + ram_to_flash_bytes_to_be_written
                 |      unused        |
                 ----------------------
                 | SPACE FOR STACK    |
    0x10001000   ----------------------  stack_pointer
                 |      unused        |
                 ----------------------
                 | 32 BYTES FOR IAP   |
    0x10??????   ----------------------  CHIP MEMORY LIMIT
*/
  
    SI6600_FLM, // image

    1024          // ram_to_flash_bytes_to_be_written
};

static uint8_t target_flash_init(uint32_t clk) {
    // Download flash programming algorithm to target and initialise.
    if (!swd_write_memory(flash.algo_start, (uint8_t *)flash.image, flash.algo_size)) {
        return 0;
    }

    if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.init, 0, 0 /* clk value is not used */, 0, 0)) {
        return 0;
    }

    return 1;
}

static uint8_t target_flash_erase_sector(unsigned int sector) {
    if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.erase_sector, sector, 0, 0, 0)) {
        return 0;
    }

    return 1;
}

static uint8_t target_flash_erase_chip(void) {
    if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.erase_chip, 0, 0, 0, 0)) {
        return 0;
    }

    return 1;
}

static uint8_t target_flash_program_page(uint32_t addr, uint8_t * buf, uint32_t size)
{
    uint32_t bytes_written = 0;

    // Program a page in target flash.
    if (!swd_write_memory(flash.program_buffer, buf, size)) {
        return 0;
    }

    while(bytes_written < size) {
        if (!swd_flash_syscall_exec(&flash.sys_call_param,
                                    flash.program_page,
                                    addr,
                                    flash.ram_to_flash_bytes_to_be_written,
                                    flash.program_buffer + bytes_written, 0)) {
            return 0;
        }

        bytes_written += flash.ram_to_flash_bytes_to_be_written;
        addr += flash.ram_to_flash_bytes_to_be_written;
    }

    return 1;
}


#endif
