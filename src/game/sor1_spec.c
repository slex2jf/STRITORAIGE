/*
 * Streets of Rage 1 / Bare Knuckle
 * Minimal GameSpec para bring-up visual no GenesisRecomp.
 *
 * ROM: 512 KiB
 * CRC32: BFF227C6
 * Reset SSP: $00FFFF00
 * Entry:     $000208
 * HBlank:    $000200
 * VBlank:    $019D16
 */

#include "game_spec.h"
#include "genesis_runtime.h"

#include <stddef.h>
#include <stdint.h>

static void sor1_call_entry_point(void)
{
    recomp_call_addr(0x000208u);
}

static void sor1_call_vblank(void)
{
    recomp_call_addr(0x019D16u);
}

static void sor1_call_hblank(void)
{
    recomp_call_addr(0x000200u);
}

const GameSpec g_game_spec = {
    .display_name          = "Streets of Rage",
    .short_name            = "SOR1",

    .expected_rom_crc32    = 0xBFF227C6u,
    .expected_rom_size     = 0x80000u,

    /*
     * A descoberta estática ainda não cobre todo o jogo.
     * Permite que misses caiam no interpretador Tier-3.
     */
    .tier3_floor_default   = 1,

    .call_entry_point      = sor1_call_entry_point,
    .call_vblank           = sor1_call_vblank,
    .call_hblank           = sor1_call_hblank,

    .resume_main_loop_pc   = 0u,
    .save_resume_pc        = NULL,
    .dispatch_main_loop_pc = 0u,
    .call_periodic         = NULL,

    .on_post_reset         = NULL,
    .on_frame_pre          = NULL,
    .on_frame_post         = NULL,
    .on_hblank             = NULL,

    .handle_arg            = NULL,
    .arg_usage             = NULL,
    .dispatch_override     = NULL,

    .fill_frame_record     = NULL,
    .frame_record_version  = 0u,

    .commands              = NULL,
    .command_count         = 0,
};
