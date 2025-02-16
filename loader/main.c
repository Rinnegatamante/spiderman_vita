/* main.c -- Ultimate Spider-Man: Total Mayhem .so loader
 *
 * Copyright (C) 2025 Alessio Tosto, Andy Nguyen
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.	See the LICENSE file for details.
 */

#include <vitasdk.h>
#include <kubridge.h>
#include <vitaGL.h>

#include <malloc.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include <math.h>

#include <sys/time.h>

#include "main.h"
#include "config.h"
#include "dialog.h"
#include "so_util.h"

//#define DEBUG

const char *sounds[] = {"music/m_title.ogg", "music/m_lose.ogg", "music/m_win.ogg", "music/m_score_screen.ogg", "music/m_downtown_calm.ogg", "music/m_downtown_mixed.ogg", "music/m_powerplant_calm.ogg", "music/m_powerplant_mixed.ogg", "music/m_bridge_calm.ogg", "music/m_bridge_mixed.ogg", "music/m_subway_calm.ogg", "music/m_subway_mixed.ogg", "music/m_stadium_calm.ogg", "music/m_stadium_mixed.ogg", "music/m_skyscraper_calm.ogg", "music/m_skyscraper_mixed.ogg", "music/m_laboratory_calm.ogg", "music/m_laboratory_mixed.ogg", "music/m_boss_rhino.ogg", "music/m_boss_electro.ogg", "music/m_boss_venom.ogg", "music/m_boss_green_goblin.ogg", "music/m_boss_sandman.ogg", "music/m_boss_octopus.ogg", "sfx/INTERFACE/sfx_interface_confirm.ogg", "sfx/INTERFACE/sfx_interface_back.ogg", "sfx/INTERFACE/sfx_interface_browse.ogg", "sfx/INTERFACE/sfx_interface_continue.ogg", "sfx/INTERFACE/sfx_interface_continue.ogg", "sfx/INTERFACE/sfx_point_spend.ogg", "sfx/INTERFACE/sfx_point_spend_no_points.ogg", "sfx/INTERFACE/sfx_upgrade.ogg", "sfx/INTERFACE/sfx_score_whoosh.ogg", "sfx/INTERFACE/sfx_counter.ogg", "sfx/INTERFACE/sfx_gold_rank.ogg", "sfx/INTERFACE/sfx_silver_rank.ogg", "sfx/INTERFACE/sfx_copper_rank.ogg", "sfx/INTERFACE/sfx_stone_rank.ogg", "sfx/Main_Character/Basic_Moves/sfx_land.ogg", "sfx/Main_Character/Basic_Moves/sfx_drop_floor.ogg", "sfx/Main_Character/Basic_Moves/sfx_die.ogg", "sfx/Main_Character/Basic_Moves/sfx_hurt_1.ogg", "sfx/Main_Character/Basic_Moves/sfx_hurt_2.ogg", "sfx/Main_Character/Basic_Moves/sfx_hurt_3.ogg", "sfx/Main_Character/Basic_Moves/sfx_jump_1.ogg", "sfx/Main_Character/Basic_Moves/sfx_jump_2.ogg", "sfx/Main_Character/Basic_Moves/sfx_jump_3.ogg", "sfx/Main_Character/Basic_Moves/sfx_jump_4.ogg", "sfx/Main_Character/Basic_Moves/sfx_jump_5.ogg", "sfx/Main_Character/Basic_Moves/sfx_jump_6.ogg", "sfx/Main_Character/Basic_Moves/sfx_kick_impact_1.ogg", "sfx/Main_Character/Basic_Moves/sfx_kick_impact_2.ogg", "sfx/Main_Character/Basic_Moves/sfx_kick_swoosh_1.ogg", "sfx/Main_Character/Basic_Moves/sfx_kick_swoosh_2.ogg", "sfx/Main_Character/Basic_Moves/sfx_pellet_impact_1.ogg", "sfx/Main_Character/Basic_Moves/sfx_pellet_shoot_1.ogg", "sfx/Main_Character/Basic_Moves/sfx_pellet_shoot_2.ogg", "sfx/Main_Character/Basic_Moves/sfx_pellet_shoot_3.ogg", "sfx/Main_Character/Basic_Moves/sfx_punch_impact_1.ogg", "sfx/Main_Character/Basic_Moves/sfx_punch_impact_2.ogg", "sfx/Main_Character/Basic_Moves/sfx_punch_swoosh_1.ogg", "sfx/Main_Character/Basic_Moves/sfx_punch_swoosh_2.ogg", "sfx/Main_Character/Basic_Moves/sfx_multikick_miss.ogg", "sfx/Main_Character/Basic_Moves/sfx_multikick_hit_1.ogg", "sfx/Main_Character/Basic_Moves/sfx_multikick_hit_2.ogg", "sfx/Main_Character/Basic_Moves/sfx_multikick_hit_3.ogg", "sfx/Main_Character/Basic_Moves/sfx_multikick_hit_4.ogg", "sfx/Main_Character/Basic_Moves/sfx_web_throw_1.ogg", "sfx/Main_Character/Basic_Moves/sfx_web_throw_2.ogg", "sfx/Main_Character/Basic_Moves/sfx_web_throw_3.ogg", "sfx/Main_Character/Basic_Moves/sfx_web_swing_start.ogg", "sfx/Main_Character/Basic_Moves/sfx_web_swing_end.ogg", "sfx/Main_Character/Basic_Moves/sfx_horizontal_web_slam_swing.ogg", "sfx/Main_Character/Basic_Moves/sfx_horizontal_web_slam_impact.ogg", "sfx/Main_Character/Basic_Moves/sfx_vertical_web_slam_swing.ogg", "sfx/Main_Character/Basic_Moves/sfx_vertical_web_slam.ogg", "sfx/Main_Character/Basic_Moves/sfx_wall_climb_start.ogg", "sfx/Main_Character/Basic_Moves/sfx_sliding.ogg", "sfx/Main_Character/Basic_Moves/sfx_aerial_suspend.ogg", "sfx/Main_Character/Basic_Moves/sfx_double_spin_kick.ogg", "sfx/Main_Character/Basic_Moves/sfx_enemy_pull.ogg", "sfx/Main_Character/Basic_Moves/sfx_double_flip_throw.ogg", "sfx/Main_Character/RED_SUIT_MOVES/sfx_super_web_ready.ogg", "sfx/Main_Character/RED_SUIT_MOVES/sfx_super_web_attack.ogg", "sfx/Main_Character/RED_SUIT_MOVES/sfx_super_web_final_attack.ogg", "sfx/Main_Character/RED_SUIT_MOVES/sfx_up_punch_attack.ogg", "sfx/Main_Character/RED_SUIT_MOVES/sfx_air_diagonal_kick.ogg", "sfx/Main_Character/Basic_Moves/sfx_air_circle.ogg", "sfx/Main_Character/Basic_Moves/sfx_torpedo_attack.ogg", "sfx/Main_Character/Basic_Moves/sfx_flip.ogg", "sfx/Main_Character/Basic_Moves/sfx_knock_wall.ogg", "sfx/Main_Character/Basic_Moves/sfx_critical_hit_1.ogg", "sfx/Main_Character/Basic_Moves/sfx_critical_hit_2.ogg", "sfx/Main_Character/Basic_Moves/sfx_critical_hit_3.ogg", "sfx/Main_Character/BLACK_SUIT_MOVES/sfx_tentacle_whip.ogg", "sfx/Main_Character/BLACK_SUIT_MOVES/sfx_tentacle_shoot.ogg", "sfx/Main_Character/BLACK_SUIT_MOVES/sfx_tentacle_spike.ogg", "sfx/Main_Character/BLACK_SUIT_MOVES/sfx_black_suit_special_swirl.ogg", "sfx/Objects/Collectibles/sfx_orbs_collect.ogg", "sfx/Objects/Collectibles/sfx_special_collect.ogg", "sfx/Objects/Destructibles/sfx_car_impact.ogg", "sfx/Objects/Destructibles/sfx_crate_impact.ogg", "sfx/Objects/Destructibles/sfx_flower_pot_impact.ogg", "sfx/Objects/Destructibles/sfx_hydrant_impact.ogg", "sfx/Objects/Destructibles/sfx_hydrant_leaking.ogg", "sfx/Objects/Destructibles/sfx_postal_box_impact.ogg", "sfx/Objects/Destructibles/sfx_shopping_cart_impact.ogg", "sfx/Objects/Destructibles/sfx_street_lamp_impact.ogg", "sfx/Objects/Destructibles/sfx_traffic_cone_impact.ogg", "sfx/Objects/Destructibles/sfx_trash_can_impact.ogg", "sfx/Objects/Destructibles/sfx_air_conditioner_impact.ogg", "sfx/Objects/Destructibles/sfx_antenna_impact.ogg", "sfx/Objects/Destructibles/sfx_barber_sign_impact.ogg", "sfx/Objects/Destructibles/sfx_barrier_impact.ogg", "sfx/Objects/Destructibles/sfx_battery_cell_impact.ogg", "sfx/Objects/Destructibles/sfx_book_stand_impact.ogg", "sfx/Objects/Destructibles/sfx_fire_extinguisher_impact.ogg", "sfx/Objects/Destructibles/sfx_gumball_machine_impact.ogg", "sfx/Objects/Destructibles/sfx_hotdog_stand_impact.ogg", "sfx/Objects/Destructibles/sfx_office_chair_impact.ogg", "sfx/Objects/Destructibles/sfx_bigoffice_table_impact.ogg", "sfx/Objects/Destructibles/sfx_office_table_impact.ogg", "sfx/Objects/Destructibles/sfx_bank_desk_impact.ogg", "sfx/Objects/Destructibles/sfx_newspaper_impact.ogg", "sfx/Objects/Destructibles/sfx_steam_pipe_impact.ogg", "sfx/Objects/Destructibles/sfx_bank_chair_impact.ogg", "sfx/Objects/Destructibles/sfx_vent_fan_box_impact.ogg", "sfx/Objects/Destructibles/sfx_bus_stop_impact.ogg", "sfx/Objects/Destructibles/sfx_vending_machine_impact.ogg", "sfx/Objects/Destructibles/sfx_bench_impact.ogg", "sfx/Objects/MISC/sfx_electric_door_loop.ogg", "sfx/Objects/MISC/sfx_obstacle_falling.ogg", "sfx/Objects/MISC/sfx_fountain_loop.ogg", "sfx/Objects/MISC/sfx_car_fire_loop.ogg", "sfx/Objects/MISC/sfx_bridge_falling.ogg", "sfx/Objects/MISC/sfx_fence_up.ogg", "sfx/Objects/MISC/sfx_fence_down.ogg", "sfx/Objects/MISC/sfx_glass_break.ogg", "sfx/Objects/MISC/sfx_drone_fly.ogg", "sfx/Objects/MISC/sfx_block_web_close.ogg", "sfx/Objects/MISC/sfx_drone_fly.ogg", "sfx/Objects/MISC/sfx_drone_fly.ogg", "sfx/Objects/MISC/sfx_drone_alarm_1.ogg", "sfx/Objects/MISC/sfx_drone_alarm_2.ogg", "sfx/Objects/MISC/sfx_laser_beam.ogg", "sfx/Objects/MISC/sfx_switch_off.ogg", "sfx/Objects/TRAPS/sfx_fire_trap.ogg", "sfx/Objects/TRAPS/sfx_fire_trap_hurt.ogg", "sfx/Objects/TRAPS/sfx_electric_alarm.ogg", "sfx/Objects/TRAPS/sfx_electric_trap.ogg", "sfx/Objects/TRAPS/sfx_electric_trap_hurt.ogg", "sfx/Objects/TRAPS/sfx_symbiote_action.ogg", "sfx/Objects/TRAPS/sfx_symbiote_move.ogg", "sfx/Objects/MISC/sfx_block_symbiote_close.ogg", "sfx/Objects/TRAPS/sfx_symbiote_trap.ogg", "sfx/Objects/TRAPS/sfx_symbiote_trap_hurt.ogg", "sfx/Objects/TRAPS/sfx_bridge_trap.ogg", "sfx/Objects/MISC/sfx_laser_beam.ogg", "sfx/Objects/TRAPS/sfx_subway_train_trumpet_1.ogg", "sfx/Objects/TRAPS/sfx_subway_train_trumpet_2.ogg", "sfx/Objects/TRAPS/sfx_subway_train_run.ogg", "sfx/Objects/TRAPS/sfx_subway_train_run.ogg", "sfx/NPC/HOSTAGE/sfx_woman_hostage_help.ogg", "sfx/NPC/HOSTAGE/sfx_woman_hostage_thank.ogg", "sfx/NPC/HOSTAGE/sfx_man_hostage_help.ogg", "sfx/NPC/HOSTAGE/sfx_man_hostage_thank.ogg", "sfx/NPC/Thugs/sfx_thug_voice_1.ogg", "sfx/NPC/Thugs/sfx_thug_voice_2.ogg", "sfx/NPC/Thugs/sfx_thug_voice_3.ogg", "sfx/NPC/Thugs/sfx_thug_voice_4.ogg", "sfx/NPC/Thugs/sfx_thug_voice_5.ogg", "sfx/NPC/Thugs/sfx_thug_voice_6.ogg", "sfx/NPC/Thugs/sfx_thug_voice_7.ogg", "sfx/NPC/Thugs/sfx_thug_voice_8.ogg", "sfx/NPC/Thugs/sfx_thug_voice_9.ogg", "sfx/NPC/Thugs/sfx_thug_voice_10.ogg", "sfx/NPC/Thugs/sfx_thug_voice_11.ogg", "sfx/NPC/Thugs/sfx_thug_voice_12.ogg", "sfx/NPC/Thugs/sfx_thug_swoosh.ogg", "sfx/NPC/Thugs/sfx_thug_gun_shoot.ogg", "sfx/NPC/Thugs/sfx_thug_molotov_shoot.ogg", "sfx/NPC/Thugs/sfx_thug_molotov_ground.ogg", "sfx/NPC/Thugs/sfx_thug_molotov_impact.ogg", "sfx/NPC/Thugs/sfx_thug_rocket_shoot.ogg", "sfx/NPC/Thugs/sfx_thug_rocket_impact.ogg", "sfx/NPC/Thugs/sfx_thug_hurt_1.ogg", "sfx/NPC/Thugs/sfx_thug_hurt_2.ogg", "sfx/NPC/Thugs/sfx_thug_hurt_3.ogg", "sfx/NPC/Thugs/sfx_thug_dies.ogg", "sfx/NPC/Thugs/sfx_thug_bat_hurt_1.ogg", "sfx/NPC/Thugs/sfx_thug_bat_hurt_2.ogg", "sfx/NPC/Thugs/sfx_thug_bat_hurt_3.ogg", "sfx/NPC/Thugs/sfx_thug_bat_dies.ogg", "sfx/NPC/Thugs/sfx_thug_molotov_hurt_1.ogg", "sfx/NPC/Thugs/sfx_thug_molotov_hurt_2.ogg", "sfx/NPC/Thugs/sfx_thug_molotov_hurt_3.ogg", "sfx/NPC/Thugs/sfx_thug_molotov_dies.ogg", "sfx/NPC/Thugs/sfx_thug_rocket_hurt_1.ogg", "sfx/NPC/Thugs/sfx_thug_rocket_hurt_2.ogg", "sfx/NPC/Thugs/sfx_thug_rocket_hurt_3.ogg", "sfx/NPC/Thugs/sfx_thug_rocket_dies.ogg", "sfx/NPC/Thugs/sfx_thug_gun_hurt_1.ogg", "sfx/NPC/Thugs/sfx_thug_gun_hurt_2.ogg", "sfx/NPC/Thugs/sfx_thug_gun_hurt_3.ogg", "sfx/NPC/Thugs/sfx_thug_gun_dies.ogg", "sfx/NPC/SLEDGER/sfx_sledger_swing.ogg", "sfx/NPC/SLEDGER/sfx_sledger_attack_1.ogg", "sfx/NPC/SLEDGER/sfx_sledger_attack_2.ogg", "sfx/NPC/SLEDGER/sfx_sledger_hurt_1.ogg", "sfx/NPC/SLEDGER/sfx_sledger_hurt_2.ogg", "sfx/NPC/SLEDGER/sfx_sledger_dies.ogg", "sfx/NPC/SYMBIOTE_ZOMBIES/sfx_zombie_voice_1.ogg", "sfx/NPC/SYMBIOTE_ZOMBIES/sfx_zombie_voice_2.ogg", "sfx/NPC/SYMBIOTE_ZOMBIES/sfx_zombie_voice_3.ogg", "sfx/NPC/SYMBIOTE_ZOMBIES/sfx_zombie_voice_4.ogg", "sfx/NPC/SYMBIOTE_ZOMBIES/sfx_zombie_voice_5.ogg", "sfx/NPC/SYMBIOTE_ZOMBIES/sfx_zombie_symbiote.ogg", "sfx/NPC/SYMBIOTE_ZOMBIES/sfx_zombie_bound.ogg", "sfx/NPC/SYMBIOTE_ZOMBIES/sfx_zombie_bound_fall.ogg", "sfx/NPC/SYMBIOTE_ZOMBIES/sfx_zombie_hurt_1.ogg", "sfx/NPC/SYMBIOTE_ZOMBIES/sfx_zombie_hurt_2.ogg", "sfx/NPC/SYMBIOTE_ZOMBIES/sfx_zombie_dies.ogg", "sfx/NPC/SYMBIOTE_CHARGERS/sfx_charger_voice_1.ogg", "sfx/NPC/SYMBIOTE_CHARGERS/sfx_charger_voice_2.ogg", "sfx/NPC/SYMBIOTE_CHARGERS/sfx_charger_voice_3.ogg", "sfx/NPC/SYMBIOTE_CHARGERS/sfx_charger_voice_4.ogg", "sfx/NPC/SYMBIOTE_CHARGERS/sfx_charger_voice_5.ogg", "sfx/NPC/SYMBIOTE_CHARGERS/sfx_charger_scream.ogg", "sfx/NPC/SYMBIOTE_CHARGERS/sfx_charger_smash.ogg", "sfx/NPC/SYMBIOTE_CHARGERS/sfx_chargers_whip.ogg", "sfx/NPC/SYMBIOTE_CHARGERS/sfx_charger_bound.ogg", "sfx/NPC/SYMBIOTE_CHARGERS/sfx_charger_bound_fall.ogg", "sfx/NPC/SYMBIOTE_CHARGERS/sfx_charger_hurt_1.ogg", "sfx/NPC/SYMBIOTE_CHARGERS/sfx_charger_hurt_2.ogg", "sfx/NPC/SYMBIOTE_CHARGERS/sfx_charger_dies.ogg", "sfx/NPC/GOBLIN/sfx_goblin_hurt_1.ogg", "sfx/NPC/GOBLIN/sfx_goblin_hurt_2.ogg", "sfx/NPC/GOBLIN/sfx_goblin_hurt_3.ogg", "sfx/NPC/GOBLIN/sfx_goblin_dies.ogg", "sfx/NPC/SECURITY_BOTS/sfx_bot_plasma_shoot.ogg", "sfx/NPC/SECURITY_BOTS/sfx_plasma_loop.ogg", "sfx/NPC/SECURITY_BOTS/sfx_bot_plasma_impact.ogg", "sfx/NPC/SECURITY_BOTS/sfx_bot_shield.ogg", "sfx/NPC/SECURITY_BOTS/sfx_bot_shield_break.ogg", "sfx/NPC/PHANTOM/sfx_phantom_double_slash_1.ogg", "sfx/NPC/PHANTOM/sfx_phantom_double_slash_2.ogg", "sfx/NPC/PHANTOM/sfx_phantom_slash.ogg", "sfx/NPC/PHANTOM/sfx_phantom_throw.ogg", "sfx/NPC/PHANTOM/sfx_phantom_throw_start.ogg", "sfx/NPC/PHANTOM/sfx_phantom_throw_loop.ogg", "sfx/NPC/PHANTOM/sfx_phantom_throw_end.ogg", "sfx/NPC/PHANTOM/sfx_phantom_hurt_1.ogg", "sfx/NPC/PHANTOM/sfx_phantom_hurt_2.ogg", "sfx/NPC/PHANTOM/sfx_phantom_hurt_3.ogg", "sfx/NPC/PHANTOM/sfx_phantom_die.ogg", "sfx/NPC/RHINO/sfx_rhino_strike_1.ogg", "sfx/NPC/RHINO/sfx_rhino_strike_2.ogg", "sfx/NPC/RHINO/sfx_rhino_tramp.ogg", "sfx/NPC/RHINO/sfx_rhino_bump.ogg", "sfx/NPC/RHINO/sfx_rhino_rotate_attack.ogg", "sfx/NPC/RHINO/sfx_rhino_car_throw.ogg", "sfx/NPC/RHINO/sfx_rhino_car_fall.ogg", "sfx/NPC/RHINO/sfx_rhino_grab.ogg", "sfx/NPC/RHINO/sfx_rhino_hurt_1.ogg", "sfx/NPC/RHINO/sfx_rhino_hurt_2.ogg", "sfx/NPC/RHINO/sfx_rhino_defense.ogg", "sfx/NPC/RHINO/sfx_rhino_dies.ogg", "sfx/NPC/RHINO/sfx_rhino_laugh.ogg", "sfx/NPC/ELECTRO/sfx_electro_whirlwind.ogg", "sfx/NPC/ELECTRO/sfx_electro_nova_charge.ogg", "sfx/NPC/ELECTRO/sfx_electro_nova_release.ogg", "sfx/NPC/ELECTRO/sfx_electro_lightning.ogg", "sfx/NPC/ELECTRO/sfx_electro_diving.ogg", "sfx/Objects/TRAPS/sfx_electric_trap.ogg", "sfx/NPC/ELECTRO/sfx_electro_charge.ogg", "sfx/NPC/ELECTRO/sfx_electro_hurt_1.ogg", "sfx/NPC/ELECTRO/sfx_electro_hurt_2.ogg", "sfx/NPC/ELECTRO/sfx_electro_hurt_3.ogg", "sfx/NPC/ELECTRO/sfx_electro_dies.ogg", "sfx/NPC/SANDMAN/sfx_sandman_Punch.ogg", "sfx/NPC/SANDMAN/sfx_sandman_strike.ogg", "sfx/NPC/SANDMAN/sfx_sandman_big_hands.ogg", "sfx/NPC/SANDMAN/sfx_sandman_get_in_ground.ogg", "sfx/NPC/SANDMAN/sfx_sandman_move_underground.ogg", "sfx/NPC/SANDMAN/sfx_sandman_get_out_ground.ogg", "sfx/NPC/SANDMAN/sfx_sandman_hurt_1.ogg", "sfx/NPC/SANDMAN/sfx_sandman_hurt_2.ogg", "sfx/NPC/SANDMAN/sfx_sandman_hurt_3.ogg", "sfx/NPC/SANDMAN/sfx_sandman_dies.ogg", "sfx/NPC/VENOM/sfx_venom_attack_1.ogg", "sfx/NPC/VENOM/sfx_venom_attack_2.ogg", "sfx/NPC/VENOM/sfx_venom_attack_3.ogg", "sfx/NPC/VENOM/sfx_venom_claw.ogg", "sfx/NPC/VENOM/sfx_venom_fall_1.ogg", "sfx/NPC/VENOM/sfx_venom_fall_2.ogg", "sfx/NPC/VENOM/sfx_venom_fall_3.ogg", "sfx/NPC/VENOM/sfx_venom_scream.ogg", "sfx/NPC/VENOM/sfx_venom_throw_land.ogg", "sfx/NPC/VENOM/sfx_venom_throw_symbiote.ogg", "sfx/NPC/VENOM/sfx_venom_whip.ogg", "sfx/NPC/VENOM/sfx_venom_pat_train.ogg", "sfx/NPC/VENOM/sfx_venom_hurt_1.ogg", "sfx/NPC/VENOM/sfx_venom_hurt_2.ogg", "sfx/NPC/VENOM/sfx_venom_hurt_3.ogg", "sfx/NPC/VENOM/sfx_venom_dies.ogg", "sfx/NPC/GREEN_GOBLIN/sfx_green_goblin_dash.ogg", "sfx/NPC/GREEN_GOBLIN/sfx_green_goblin_fireball_land.ogg", "sfx/NPC/GREEN_GOBLIN/sfx_green_goblin_jump.ogg", "sfx/NPC/GREEN_GOBLIN/sfx_green_goblin_land.ogg", "sfx/NPC/GREEN_GOBLIN/sfx_green_goblin_land.ogg", "sfx/NPC/GREEN_GOBLIN/sfx_green_goblin_roar.ogg", "sfx/NPC/GREEN_GOBLIN/sfx_green_goblin_throw.ogg", "sfx/NPC/GREEN_GOBLIN/sfx_green_goblin_roar_in_air.ogg", "sfx/NPC/GREEN_GOBLIN/sfx_green_goblin_spout_fireball.ogg", "sfx/Objects/TRAPS/sfx_fire_trap.ogg", "sfx/NPC/GREEN_GOBLIN/sfx_green_goblin_hurt_1.ogg", "sfx/NPC/GREEN_GOBLIN/sfx_green_goblin_hurt_2.ogg", "sfx/NPC/GREEN_GOBLIN/sfx_green_goblin_hurt_3.ogg", "sfx/NPC/GREEN_GOBLIN/sfx_green_goblin_die.ogg", "sfx/NPC/OCTOPUS/sfx_octopus_tentacle_attack.ogg", "sfx/NPC/OCTOPUS/sfx_octopus_electric_attack.ogg", "sfx/NPC/OCTOPUS/sfx_octopus_tentacle_out.ogg", "sfx/NPC/OCTOPUS/sfx_octopus_plasma_thin_start.ogg", "sfx/NPC/OCTOPUS/sfx_octopus_plasma_thin_loop.ogg", "sfx/NPC/OCTOPUS/sfx_octopus_plasma_thick.ogg", "sfx/NPC/OCTOPUS/sfx_octopus_jump.ogg", "sfx/NPC/OCTOPUS/sfx_octopus_land.ogg", "sfx/NPC/OCTOPUS/sfx_octopus_walk_01.ogg", "sfx/NPC/OCTOPUS/sfx_octopus_walk_02.ogg", "sfx/NPC/OCTOPUS/sfx_octopus_walk_03.ogg", "sfx/NPC/OCTOPUS/sfx_octopus_walk_04.ogg", "sfx/NPC/OCTOPUS/sfx_octopus_weak.ogg", "sfx/NPC/OCTOPUS/sfx_octopus_tentacle_hurt_01.ogg", "sfx/NPC/OCTOPUS/sfx_octopus_tentacle_hurt_02.ogg", "sfx/NPC/OCTOPUS/sfx_octopus_tentacle_hurt_03.ogg", "sfx/NPC/OCTOPUS/sfx_octopus_hurt_1.ogg", "sfx/NPC/OCTOPUS/sfx_octopus_hurt_2.ogg", "sfx/NPC/OCTOPUS/sfx_octopus_hurt_3.ogg", "sfx/NPC/OCTOPUS/sfx_octopus_dies.ogg", "sfx/CUTSCENES/sfx_door_open.ogg", "sfx/CUTSCENES/sfx_battery_cell_fall.ogg", "sfx/CUTSCENES/sfx_battery_cell_explosion.ogg", "sfx/CUTSCENES/sfx_electro_sparkle_1.ogg", "sfx/CUTSCENES/sfx_electro_sparkle_2.ogg", "sfx/CUTSCENES/sfx_electro_sparkle_3.ogg", "sfx/CUTSCENES/sfx_rhino_bust.ogg", "sfx/CUTSCENES/sfx_rhino_fs_1.ogg", "sfx/CUTSCENES/sfx_rhino_fs_2.ogg", "sfx/CUTSCENES/sfx_police_arriving.ogg", "sfx/CUTSCENES/sfx_truck_arrives.ogg", "sfx/CUTSCENES/sfx_truck_ends.ogg", "sfx/CUTSCENES/sfx_truck_hit.ogg", "sfx/CUTSCENES/sfx_subway_leaving.ogg", "sfx/CUTSCENES/sfx_venom_end_cutscene.ogg", "sfx/CUTSCENES/sfx_cutscene_lv1_earthquake.ogg", "sfx/CUTSCENES/sfx_cutscene_lv1_sandman_land.ogg", "sfx/CUTSCENES/sfx_cutscene_lv1_sandman_out.ogg", "sfx/CUTSCENES/sfx_cutscene_lv2_police_car.ogg", "sfx/CUTSCENES/sfx_cutscene_lv3_spidey_arrives.ogg", "sfx/CUTSCENES/sfx_cutscene_lv3_spidey_lands.ogg", "sfx/CUTSCENES/sfx_cutscene_lv7_spidey_arrives.ogg", "sfx/CUTSCENES/sfx_cutscene_lv7_zombie_scream.ogg", "sfx/CUTSCENES/sfx_cutscene_lv7_zombie_swing_1.ogg", "sfx/CUTSCENES/sfx_cutscene_lv7_zombie_swing_2.ogg", "sfx/CUTSCENES/sfx_cutscene_lv7_security_bot_action.ogg", "sfx/CUTSCENES/sfx_cutscene_lv8_stone_impact.ogg", "sfx/CUTSCENES/sfx_cutscene_lv8_spidey_arm_swing_1.ogg", "sfx/CUTSCENES/sfx_cutscene_lv8_spidey_arm_swing_2.ogg", "sfx/CUTSCENES/sfx_cutscene_lv8_spidey_arm_swing_3.ogg", "sfx/CUTSCENES/sfx_cutscene_lv8_spidey_arm_swing_4.ogg", "sfx/CUTSCENES/sfx_cutscene_lv10_beat_airplane_1.ogg", "sfx/CUTSCENES/sfx_cutscene_lv10_beat_airplane_2.ogg", "sfx/CUTSCENES/sfx_cutscene_lv10_beat_airplane_3.ogg", "sfx/CUTSCENES/sfx_cutscene_lv10_goblin_scream_1.ogg", "sfx/CUTSCENES/sfx_cutscene_lv10_goblin_scream_2.ogg", "sfx/CUTSCENES/sfx_cutscene_lv10_goblin_scream_3.ogg", "sfx/CUTSCENES/sfx_cutscene_lv11_human_cheers.ogg", "sfx/CUTSCENES/sfx_cutscene_lv12_green_goblin_gasp.ogg", "sfx/CUTSCENES/sfx_venom_kicked_face.ogg", "sfx/CUTSCENES/sfx_venom_hits_ground.ogg", "sfx/CUTSCENES/sfx_venom_jump_sea.ogg", "sfx/CUTSCENES/sfx_cutscene_lv8_venom_tentacles.ogg", "sfx/CUTSCENES/sfx_cutscene_lv8_venom_tentacles_exits.ogg", "sfx/CUTSCENES/sfx_cutscene_lv8_venom_tentacles_returns.ogg", "sfx/CUTSCENES/sfx_cutscene_lv11_camera_woosh.ogg", "sfx/CUTSCENES/sfx_cutscene_lv11_earthquake.ogg", "sfx/CUTSCENES/sfx_cutscene_lv11_goblin_down.ogg", "sfx/CUTSCENES/sfx_cutscene_lv11_goblin_jump.ogg", "sfx/CUTSCENES/sfx_cutscene_lv11_jump_out_rail.ogg", "sfx/CUTSCENES/sfx_cutscene_lv11_spidey_swing.ogg", "sfx/Production_Elements/sfx_spider_sense_in.ogg", "sfx/Production_Elements/sfx_spider_sense_out.ogg", "sfx/Production_Elements/sfx_qte_success.ogg", "sfx/Production_Elements/sfx_qte_fail.ogg", "sfx/Production_Elements/sfx_qte_click.ogg", "sfx/Production_Elements/sfx_qte_untie.ogg", "sfx/Production_Elements/sfx_spider_logo_in.ogg", "sfx/Production_Elements/sfx_spider_logo_out.ogg", "sfx/Production_Elements/sfx_camera_shot.ogg", "VFX/vfx_prologue_spidy_01.ogg", "VFX/vfx_prologue_spidy_02.ogg", "VFX/vfx_prologue_girl_01.ogg", "VFX/vfx_prologue_spidy_03.ogg", "VFX/vfx_prologue_cop_01.ogg", "VFX/vfx_lv1_01_spidy_01.ogg", "VFX/vfx_lv1_01_spidy_02.ogg", "VFX/vfx_lv1_01_spidy_03.ogg", "VFX/vfx_lv1_01_sandman_01.ogg", "VFX/vfx_lv1_01_spidy_04.ogg", "VFX/vfx_lv1_01_sandman_02.ogg", "VFX/vfx_lv1_02_spidy_01.ogg", "VFX/vfx_lv1_02_spidy_02.ogg", "VFX/vfx_lv1_02_spidy_03.ogg", "VFX/vfx_lv2_01_rhino_01.ogg", "VFX/vfx_lv2_01_spidy_01.ogg", "VFX/vfx_lv2_01_rhino_02.ogg", "VFX/vfx_lv2_01_rhino_03.ogg", "VFX/vfx_lv2_02_spidy_01.ogg", "VFX/vfx_lv2_02_COP1_01.ogg", "VFX/vfx_lv2_02_COP2_01.ogg", "VFX/vfx_lv2_02_COP1_02.ogg", "VFX/vfx_lv2_02_spidy_02.ogg", "VFX/vfx_lv2_03_spidy_01.ogg", "VFX/vfx_lv3_01_spidy_01.ogg", "VFX/vfx_lv3_01_electro_01.ogg", "VFX/vfx_lv3_01_spidy_02.ogg", "VFX/vfx_lv3_01_spidy_03.ogg", "VFX/vfx_lv4_01_electro_01.ogg", "VFX/vfx_lv4_01_spidy_01.ogg", "VFX/vfx_lv4_02_COP1_01.ogg", "VFX/vfx_lv4_02_spidy_01.ogg", "VFX/vfx_lv4_02_COP1_02.ogg", "VFX/vfx_lv4_02_spidy_02.ogg", "VFX/vfx_lv5_01_spidy_01.ogg", "VFX/vfx_lv5_01_spidy_02.ogg", "VFX/vfx_lv5_01_spidy_03.ogg", "VFX/vfx_lv6_01_octo_01.ogg", "VFX/vfx_lv6_01_spidy_01.ogg", "VFX/vfx_lv6_01_octo_02.ogg", "VFX/vfx_lv6_01_spidy_02.ogg", "VFX/vfx_lv6_01_octo_03.ogg", "VFX/vfx_lv6_02_spidy_01.ogg", "VFX/vfx_lv7_01_spidy_01.ogg", "VFX/vfx_lv7_01_spidy_02.ogg", "VFX/vfx_lv7_02_leader_01.ogg", "VFX/vfx_lv7_02_spidy_01.ogg", "VFX/vfx_lv7_02_leader_02.ogg", "VFX/vfx_lv7_02_spidy_02.ogg", "VFX/vfx_lv7_02_leader_03.ogg", "VFX/vfx_lv7_02_spidy_03.ogg", "VFX/vfx_lv7_03_spidy_01.ogg", "VFX/vfx_lv7_03_robot_01.ogg", "VFX/vfx_lv7_03_spidy_02.ogg", "VFX/vfx_lv7_03_spidy_03.ogg", "VFX/vfx_lv8_01_venom_01.ogg", "VFX/vfx_lv8_01_spidy_04.ogg", "VFX/vfx_lv8_01_venom_02.ogg", "VFX/vfx_lv8_01_spidy_05.ogg", "VFX/vfx_lv8_01_venom_03.ogg", "VFX/vfx_lv8_01_spidy_06.ogg", "VFX/vfx_lv8_01_venom_04.ogg", "VFX/vfx_lv8_02_spidy_07.ogg", "VFX/vfx_lv8_01_spidy_01.ogg", "VFX/vfx_lv8_01_spidy_02.ogg", "VFX/vfx_lv8_01_researcher_01.ogg", "VFX/vfx_lv8_01_spidy_03.ogg", "VFX/vfx_lv9_01_spidy_01.ogg", "VFX/vfx_lv9_01_venom_01.ogg", "VFX/vfx_lv9_02_spidy_01.ogg", "VFX/vfx_lv9_02_leader_01.ogg", "VFX/vfx_lv9_02_spidy_02.ogg", "VFX/vfx_lv9_02_leader_02.ogg", "VFX/vfx_lv9_02_spidy_03.ogg", "VFX/vfx_lv10_02_spidy_02.ogg", "VFX/vfx_lv11_01_spidy_01.ogg", "VFX/vfx_lv11_02_goblin_03.ogg", "VFX/vfx_lv11_02_goblin_01.ogg", "VFX/vfx_lv11_01_spidy_02.ogg", "VFX/vfx_lv11_02_goblin_02.ogg", "VFX/vfx_lv12_01_goblin_01.ogg", "VFX/vfx_lv12_01_spidy_01.ogg", "VFX/vfx_lv12_02_goblin_01.ogg", "VFX/vfx_lv12_02_spidy_01.ogg", "VFX/vfx_lv12_02_leader_01.ogg", "VFX/vfx_lv12_02_spidy_02.ogg", "VFX/vfx_lv12_02_leader_02.ogg", "VFX/vfx_lv12_02_spidy_03.ogg", "VFX/vfx_lv12_02_leader_03.ogg", "VFX/vfx_lv12_02_spidy_04.ogg", "VFX/vfx_end_girl_01.ogg", "VFX/vfx_end_spidy_01.ogg", "VFX/vfx_end_girl_02.ogg", "VFX/vfx_end_spidy_02.ogg"};

int is_xperia = 0;
int _newlib_heap_size_user = MEMORY_NEWLIB_MB * 1024 * 1024;

so_module spiderman_mod;

void *__wrap_memcpy(void *dest, const void *src, size_t n) {
	return sceClibMemcpy(dest, src, n);
}

void *__wrap_memmove(void *dest, const void *src, size_t n) {
	return sceClibMemmove(dest, src, n);
}

void *__wrap_memset(void *s, int c, size_t n) {
	return sceClibMemset(s, c, n);
}

int __android_log_write(int prio, const char *tag, const char *text) {
//#ifdef DEBUG
//	sceClibPrintf("%s: %s\n", tag, text);
//#endif
	return 0;
}


int __android_log_print(int prio, const char *tag, const char *fmt, ...) {
//#ifdef DEBUG
//	va_list list;
//	static char string[0x8000];

//	va_start(list, fmt);
//	vsprintf(string, fmt, list);
//	va_end(list);

//	sceClibPrintf("[LOG] %s: %s\n", tag, string);
//#endif
	return 0;
}

int ret0(void) {
	return 0;
}

int GetWidthOfScreen() {
	return SCREEN_W;
}

int GetHeightOfScreen() {
	return SCREEN_H;
}

uint32_t audio_player_play(char *path, uint8_t loop, float vol);
int audio_player_is_playing(void *m);
void audio_player_stop(void *m);
void audio_player_set_pause(void *m, int val);
void audio_player_init();
void audio_player_stop_all_sounds();
void audio_player_set_pause_all_sounds(int val);
void audio_player_instance(void *m, uint8_t loop, float vol);
void audio_player_set_volume(void *m, float vol);
void audio_player_change_bgm_volume(float vol);
void audio_player_change_sfx_volume(float vol);

#define AUDIO_SOURCES_NUM (2048)
float volumes[2];
void *audio_sources[AUDIO_SOURCES_NUM] = {};
char audio_path[256];

void nativePlaySound(int id, float vol, int loop) {
	//sceClibPrintf("nativeSound %s %f %d\n", sounds[id], vol, loop);
	char path[256];
	sprintf(path, "%s/%s", audio_path, sounds[id]);

	if (audio_sources[id] == 0xDEADBEEF) {
		audio_sources[id] = audio_player_play(path, loop, vol);
	} else {
		audio_player_instance(audio_sources[id], loop, vol);
	}
}

void nativeStopAllSounds() {
	//sceClibPrintf("nativeStopAllSounds\n");
	audio_player_stop_all_sounds();
	for (int i = 0; i < AUDIO_SOURCES_NUM; i++) {
		audio_sources[i] = 0xDEADBEEF;
	}
}

void nativePauseAllSounds() {
	//sceClibPrintf("nativePauseAllSounds\n");
	audio_player_set_pause_all_sounds(1);
}

void nativeResumeAllSounds() {
	//sceClibPrintf("nativeResumeAllSounds\n");
	audio_player_set_pause_all_sounds(0);
}

int nativeIsMediaPlaying(int id) {
	//sceClibPrintf("nativeIsMediaPlaying %s\n", sounds[id]);
	if (audio_sources[id] == 0xDEADBEEF)
		return 0;
	return audio_player_is_playing(audio_sources[id]);
}

void nativeKillSound(int id) {
	//sceClibPrintf("nativeKill/StopSound %s\n", sounds[id]);
	if (audio_sources[id] != 0xDEADBEEF) {
		audio_player_stop(audio_sources[id]);
		audio_sources[id] = 0xDEADBEEF;
	}
}

void nativePauseSound(int id) {
	//sceClibPrintf("nativePauseSound %s\n", sounds[id]);
	if (audio_sources[id] != 0xDEADBEEF) {
		audio_player_set_pause(audio_sources[id], 1);
	}
}

void nativeResumeSound(int id) {
	//sceClibPrintf("nativeResumeSound %s\n", sounds[id]);
	if (audio_sources[id] != 0xDEADBEEF) {
		audio_player_set_pause(audio_sources[id], 0);
	}
}

void nativeSetSoundVolume(int id, float vol) {
	//sceClibPrintf("nativeSetSoundVolume %s %f\n", sounds[id], vol);
	if (audio_sources[id] != 0xDEADBEEF) {
		audio_player_set_volume(audio_sources[id], vol);
	}
}

float nativeGetGroupVolume(int group_id) {
	//sceClibPrintf("nativeGetGroupVolume %d\n", group_id);
	return volumes[group_id - 1];
}

void nativeSetGroupVolume(int group_id, float vol) {
	//sceClibPrintf("nativeSetGroupVolume %d %f\n", group_id, vol);
	if (group_id == 1) {
		audio_player_change_bgm_volume(vol);
	} else {
		audio_player_change_sfx_volume(vol);
	}
	volumes[group_id - 1] = vol;
}

float EnlargeRateX(void *this, int unk) {
	return (float)SCREEN_W / 480.0f;
}

float EnlargeRateY(void *thid, int unk) {
	return (float)SCREEN_H / 320.0f;	
}

int ret1() {
	return 1;
}

int nativeOpenBrowser(char *url) {
	return 0;
}

int nativeExit() {
	sceKernelExitProcess(0);
}

void patch_game(void) {
	audio_player_init();
	for (int i = 0; i < AUDIO_SOURCES_NUM; i++) {
		audio_sources[i] = 0xDEADBEEF;
	}
	if (!is_xperia) {
		strcpy(audio_path, "ux0:data/spiderman/gameloft/games/GloftSMHP/sound");
	} else {
		strcpy(audio_path, "ux0:data/spiderman/gameloft/games/spiderman/sound");
	}

	hook_addr(so_symbol(&spiderman_mod, "_Z16GetWidthOfScreeni"), (uintptr_t)&GetWidthOfScreen);
	hook_addr(so_symbol(&spiderman_mod, "_Z17GetHeightOfScreeni"), (uintptr_t)&GetHeightOfScreen);
	
	hook_addr(so_symbol(&spiderman_mod, "nativeExit"), (uintptr_t)&nativeExit);
	hook_addr(so_symbol(&spiderman_mod, "nativePlaySound"), (uintptr_t)&nativePlaySound);
	hook_addr(so_symbol(&spiderman_mod, "nativePauseSound"), (uintptr_t)&nativePauseSound);
	hook_addr(so_symbol(&spiderman_mod, "nativeResumeSound"), (uintptr_t)&nativeResumeSound);
	hook_addr(so_symbol(&spiderman_mod, "nativeStopAllSounds"), (uintptr_t)&nativeStopAllSounds);
	hook_addr(so_symbol(&spiderman_mod, "nativeKillAllSounds"), (uintptr_t)&nativeStopAllSounds);
	hook_addr(so_symbol(&spiderman_mod, "nativePauseAllSounds"), (uintptr_t)&nativePauseAllSounds);
	hook_addr(so_symbol(&spiderman_mod, "nativeResumeAllSounds"), (uintptr_t)&nativeResumeAllSounds);
	hook_addr(so_symbol(&spiderman_mod, "nativeIsMediaPlaying"), (uintptr_t)&nativeIsMediaPlaying);
	hook_addr(so_symbol(&spiderman_mod, "nativeSetSoundVolume"), (uintptr_t)&nativeSetSoundVolume);
	hook_addr(so_symbol(&spiderman_mod, "nativeKillSound"), (uintptr_t)&nativeKillSound);
	hook_addr(so_symbol(&spiderman_mod, "nativeStopSound"), (uintptr_t)&nativeKillSound);
	hook_addr(so_symbol(&spiderman_mod, "nativeGetGroupVolume"), (uintptr_t)&nativeGetGroupVolume);
	hook_addr(so_symbol(&spiderman_mod, "nativeSetGroupVolume"), (uintptr_t)&nativeSetGroupVolume);
	hook_addr(so_symbol(&spiderman_mod, "nativeOpenBrowser"), (uintptr_t)&nativeOpenBrowser);
	
	if (is_xperia) {
		hook_addr(so_symbol(&spiderman_mod, "_ZN6UIInfo7GetScrHEv"), (uintptr_t)&GetHeightOfScreen);
		hook_addr(so_symbol(&spiderman_mod, "_ZN6UIInfo7GetScrWEv"), (uintptr_t)&GetWidthOfScreen);
		hook_addr(so_symbol(&spiderman_mod, "_ZN6UIInfo12EnlargeRateXEi"), (uintptr_t)&EnlargeRateX);
		hook_addr(so_symbol(&spiderman_mod, "_ZN6UIInfo12EnlargeRateYEi"), (uintptr_t)&EnlargeRateY);
		hook_addr(so_symbol(&spiderman_mod, "nativeMyReloadSound_Common"), (uintptr_t)&ret1);
		
		// Unlock framerate
		float v = 16.666;
		kuKernelCpuUnrestrictedMemcpy((void *)(spiderman_mod.text_base + 0x3CCAD0), &v, 4);
	}
}

FILE *fopen_hook(char *fname, char *mode) {
	char new_path[1024];
	if (strncmp(fname, "/sdcard", 7) == 0) {
		snprintf(new_path, sizeof(new_path), "%s%s", DATA_PATH, fname + 7);
		fname = new_path;
	}
	//sceClibPrintf("fopen %s\n", fname);
	return fopen(fname, mode);
}

int uname_fake(void *buf) {
	strcpy(buf + 195, "1.0");
	return 0;
}

extern void *__aeabi_atexit;
extern void *__aeabi_d2f;
extern void *__aeabi_d2iz;
extern void *__aeabi_d2uiz;
extern void *__aeabi_dadd;
extern void *__aeabi_dcmpeq;
extern void *__aeabi_dcmpge;
extern void *__aeabi_dcmpgt;
extern void *__aeabi_dcmple;
extern void *__aeabi_dcmplt;
extern void *__aeabi_ddiv;
extern void *__aeabi_dmul;
extern void *__aeabi_dsub;
extern void *__aeabi_f2d;
extern void *__aeabi_f2iz;
extern void *__aeabi_fadd;
extern void *__aeabi_fcmpeq;
extern void *__aeabi_fcmpge;
extern void *__aeabi_fcmpgt;
extern void *__aeabi_fcmple;
extern void *__aeabi_fcmplt;
extern void *__aeabi_fcmpun;
extern void *__aeabi_fdiv;
extern void *__aeabi_fmul;
extern void *__aeabi_fsub;
extern void *__aeabi_i2d;
extern void *__aeabi_i2f;
extern void *__aeabi_idiv;
extern void *__aeabi_idivmod;
extern void *__aeabi_ldivmod;
extern void *__aeabi_lmul;
extern void *__aeabi_ui2d;
extern void *__aeabi_ui2f;
extern void *__aeabi_uidiv;
extern void *__aeabi_uidivmod;
extern void *__cxa_guard_acquire;
extern void *__cxa_guard_release;
extern void *__dso_handle;
extern void *__stack_chk_fail;

static int __stack_chk_guard_fake = 0x42424242;

static FILE __sF_fake[0x100][3];

void glEnable_hook(GLenum v) {
	if (v != GL_LIGHTING)
		glEnable(v);
}

void glViewport_hook(GLint x, GLint y, GLsizei width, GLsizei height) {
	//sceClibPrintf("glViewport %d %d %d %d\n", x, y, width, height);
	glViewport(x + 1, y + 1, width - 1, height - 1);
}

static so_default_dynlib default_dynlib[] = {
	{ "__aeabi_atexit", (uintptr_t)&__aeabi_atexit },
	{ "__aeabi_d2f", (uintptr_t)&__aeabi_d2f },
	{ "__aeabi_d2iz", (uintptr_t)&__aeabi_d2iz },
	{ "__aeabi_d2uiz", (uintptr_t)&__aeabi_d2uiz },
	{ "__aeabi_dadd", (uintptr_t)&__aeabi_dadd },
	{ "__aeabi_dcmpeq", (uintptr_t)&__aeabi_dcmpeq },
	{ "__aeabi_dcmpge", (uintptr_t)&__aeabi_dcmpge },
	{ "__aeabi_dcmpgt", (uintptr_t)&__aeabi_dcmpgt },
	{ "__aeabi_dcmple", (uintptr_t)&__aeabi_dcmple },
	{ "__aeabi_dcmplt", (uintptr_t)&__aeabi_dcmplt },
	{ "__aeabi_ddiv", (uintptr_t)&__aeabi_ddiv },
	{ "__aeabi_dmul", (uintptr_t)&__aeabi_dmul },
	{ "__aeabi_dsub", (uintptr_t)&__aeabi_dsub },
	{ "__aeabi_f2d", (uintptr_t)&__aeabi_f2d },
	{ "__aeabi_f2iz", (uintptr_t)&__aeabi_f2iz },
	{ "__aeabi_fadd", (uintptr_t)&__aeabi_fadd },
	{ "__aeabi_fcmpeq", (uintptr_t)&__aeabi_fcmpeq },
	{ "__aeabi_fcmpge", (uintptr_t)&__aeabi_fcmpge },
	{ "__aeabi_fcmpgt", (uintptr_t)&__aeabi_fcmpgt },
	{ "__aeabi_fcmple", (uintptr_t)&__aeabi_fcmple },
	{ "__aeabi_fcmplt", (uintptr_t)&__aeabi_fcmplt },
	{ "__aeabi_fcmpun", (uintptr_t)&__aeabi_fcmpun },
	{ "__aeabi_fdiv", (uintptr_t)&__aeabi_fdiv },
	{ "__aeabi_fmul", (uintptr_t)&__aeabi_fmul },
	{ "__aeabi_fsub", (uintptr_t)&__aeabi_fsub },
	{ "__aeabi_i2d", (uintptr_t)&__aeabi_i2d },
	{ "__aeabi_i2f", (uintptr_t)&__aeabi_i2f },
	{ "__aeabi_idiv", (uintptr_t)&__aeabi_idiv },
	{ "__aeabi_idivmod", (uintptr_t)&__aeabi_idivmod },
	{ "__aeabi_ldivmod", (uintptr_t)&__aeabi_ldivmod },
	{ "__aeabi_lmul", (uintptr_t)&__aeabi_lmul },
	{ "__aeabi_ui2d", (uintptr_t)&__aeabi_ui2d },
	{ "__aeabi_ui2f", (uintptr_t)&__aeabi_ui2f },
	{ "__aeabi_uidiv", (uintptr_t)&__aeabi_uidiv },
	{ "__aeabi_uidivmod", (uintptr_t)&__aeabi_uidivmod },
	{ "__android_log_write", (uintptr_t)&__android_log_write },
	{ "__android_log_print", (uintptr_t)&__android_log_print },
	{ "__cxa_guard_acquire", (uintptr_t)&__cxa_guard_acquire },
	{ "__cxa_guard_release", (uintptr_t)&__cxa_guard_release },
	// { "__cxa_pure_virtual", (uintptr_t)&__cxa_pure_virtual },
	{ "__dso_handle", (uintptr_t)&__dso_handle },
	{ "__sF", (uintptr_t)&__sF_fake },
	{ "__stack_chk_fail", (uintptr_t)&__stack_chk_fail },
	{ "__stack_chk_guard", (uintptr_t)&__stack_chk_guard_fake },
	{ "abort", (uintptr_t)&abort },
	{ "acos", (uintptr_t)&acos },
	{ "acosf", (uintptr_t)&acosf },
	{ "asin", (uintptr_t)&asin },
	{ "asinf", (uintptr_t)&asinf },
	{ "atan", (uintptr_t)&atan },
	{ "atan2", (uintptr_t)&atan2 },
	{ "atan2f", (uintptr_t)&atan2f },
	{ "atanf", (uintptr_t)&atanf },
	{ "atoi", (uintptr_t)&atoi },
	{ "ceilf", (uintptr_t)&ceilf },
	// { "chdir", (uintptr_t)&chdir },
	// { "close", (uintptr_t)&close },
	// { "connect", (uintptr_t)&connect },
	{ "cos", (uintptr_t)&cos },
	{ "cosf", (uintptr_t)&cosf },
	{ "exit", (uintptr_t)&exit },
	{ "expf", (uintptr_t)&expf },
	{ "fclose", (uintptr_t)&fclose },
	{ "fgetc", (uintptr_t)&fgetc },
	{ "floor", (uintptr_t)&floor },
	{ "floorf", (uintptr_t)&floorf },
	{ "fmod", (uintptr_t)&fmod },
	{ "fmodf", (uintptr_t)&fmodf },
	{ "fopen", (uintptr_t)&fopen_hook },
	{ "fprintf", (uintptr_t)&fprintf },
	{ "fread", (uintptr_t)&fread },
	{ "free", (uintptr_t)&free },
	{ "fseek", (uintptr_t)&fseek },
	{ "ftell", (uintptr_t)&ftell },
	{ "fwrite", (uintptr_t)&fwrite },
	// { "gethostbyname", (uintptr_t)&gethostbyname },
	{ "gettimeofday", (uintptr_t)&gettimeofday },
	{ "glActiveTexture", (uintptr_t)&glActiveTexture },
	{ "glAlphaFunc", (uintptr_t)&glAlphaFunc },
	{ "glBindBuffer", (uintptr_t)&glBindBuffer },
	{ "glBindTexture", (uintptr_t)&glBindTexture },
	{ "glBlendFunc", (uintptr_t)&glBlendFunc },
	{ "glBufferData", (uintptr_t)&glBufferData },
	{ "glBufferSubData", (uintptr_t)&glBufferSubData },
	{ "glClear", (uintptr_t)&glClear },
	{ "glClearColor", (uintptr_t)&glClearColor },
	{ "glClearDepthf", (uintptr_t)&glClearDepthf },
	{ "glClientActiveTexture", (uintptr_t)&glClientActiveTexture },
	{ "glClipPlanef", (uintptr_t)&glClipPlanef },
	{ "glColor4f", (uintptr_t)&glColor4f },
	{ "glColor4ub", (uintptr_t)&glColor4ub },
	{ "glColorMask", (uintptr_t)&glColorMask },
	{ "glColorPointer", (uintptr_t)&glColorPointer },
	{ "glCompressedTexImage2D", (uintptr_t)&glCompressedTexImage2D },
	{ "glCopyTexImage2D", (uintptr_t)&glCopyTexImage2D },
	{ "glCullFace", (uintptr_t)&glCullFace },
	{ "glDeleteBuffers", (uintptr_t)&glDeleteBuffers },
	{ "glDeleteTextures", (uintptr_t)&glDeleteTextures },
	{ "glDepthFunc", (uintptr_t)&glDepthFunc },
	{ "glDepthMask", (uintptr_t)&glDepthMask },
	{ "glDepthRangef", (uintptr_t)&glDepthRangef },
	{ "glDisable", (uintptr_t)&glDisable },
	{ "glDisableClientState", (uintptr_t)&glDisableClientState },
	{ "glDrawArrays", (uintptr_t)&glDrawArrays },
	{ "glDrawElements", (uintptr_t)&glDrawElements },
	{ "glEnable", (uintptr_t)&glEnable_hook },
	{ "glEnableClientState", (uintptr_t)&glEnableClientState },
	{ "glFogf", (uintptr_t)&glFogf },
	{ "glFogfv", (uintptr_t)&glFogfv },
	{ "glFrontFace", (uintptr_t)&glFrontFace },
	{ "glFrustumf", (uintptr_t)&glFrustumf },
	{ "glGenBuffers", (uintptr_t)&glGenBuffers },
	{ "glGenTextures", (uintptr_t)&glGenTextures },
	{ "glGetBooleanv", (uintptr_t)&glGetBooleanv },
	{ "glGetError", (uintptr_t)&glGetError },
	{ "glGetFloatv", (uintptr_t)&glGetFloatv },
	{ "glGetIntegerv", (uintptr_t)&glGetIntegerv },
	{ "glGetPointerv", (uintptr_t)&glGetPointerv },
	{ "glGetString", (uintptr_t)&glGetString },
	{ "glGetTexEnviv", (uintptr_t)&glGetTexEnviv },
	{ "glHint", (uintptr_t)&glHint },
	{ "glIsEnabled", (uintptr_t)&glIsEnabled },
	{ "glLightModelfv", (uintptr_t)&glLightModelfv },
	{ "glLightf", (uintptr_t)&ret0 },
	{ "glLightfv", (uintptr_t)&glLightfv },
	{ "glLineWidth", (uintptr_t)&glLineWidth },
	{ "glLoadIdentity", (uintptr_t)&glLoadIdentity },
	{ "glLoadMatrixf", (uintptr_t)&glLoadMatrixf },
	{ "glMaterialf", (uintptr_t)&ret0 },
	{ "glMaterialfv", (uintptr_t)&glMaterialfv },
	{ "glMatrixMode", (uintptr_t)&glMatrixMode },
	{ "glMultMatrixf", (uintptr_t)&glMultMatrixf },
	{ "glNormal3f", (uintptr_t)&glNormal3f },
	{ "glNormalPointer", (uintptr_t)&glNormalPointer },
	{ "glOrthox", (uintptr_t)&glOrthox },
	{ "glPixelStorei", (uintptr_t)&glPixelStorei },
	{ "glPointParameterf", (uintptr_t)&ret0 },
	{ "glPointSize", (uintptr_t)&glPointSize },
	{ "glPolygonOffset", (uintptr_t)&glPolygonOffset },
	{ "glPopMatrix", (uintptr_t)&glPopMatrix },
	{ "glPushMatrix", (uintptr_t)&glPushMatrix },
	{ "glReadPixels", (uintptr_t)&glReadPixels },
	{ "glRotatef", (uintptr_t)&glRotatef },
	{ "glScalef", (uintptr_t)&glScalef },
	{ "glScissor", (uintptr_t)&glScissor },
	{ "glShadeModel", (uintptr_t)&ret0 },
	{ "glStencilFunc", (uintptr_t)&glStencilFunc },
	{ "glStencilMask", (uintptr_t)&glStencilMask },
	{ "glStencilOp", (uintptr_t)&glStencilOp },
	{ "glTexCoordPointer", (uintptr_t)&glTexCoordPointer },
	{ "glTexEnvf", (uintptr_t)&glTexEnvf },
	{ "glTexEnvfv", (uintptr_t)&glTexEnvfv },
	{ "glTexEnvi", (uintptr_t)&glTexEnvi },
	{ "glTexImage2D", (uintptr_t)&glTexImage2D },
	{ "glTexParameterf", (uintptr_t)&glTexParameterf },
	{ "glTexParameteri", (uintptr_t)&glTexParameteri },
	{ "glTexParameterx", (uintptr_t)&glTexParameterx },
	{ "glTexSubImage2D", (uintptr_t)&glTexSubImage2D },
	{ "glTranslatef", (uintptr_t)&glTranslatef },
	{ "glVertexPointer", (uintptr_t)&glVertexPointer },
	{ "glViewport", (uintptr_t)&glViewport_hook },
	{ "logf", (uintptr_t)&logf },
	{ "lrand48", (uintptr_t)&lrand48 },
	{ "malloc", (uintptr_t)&malloc },
	{ "memcmp", (uintptr_t)&memcmp },
	{ "memcpy", (uintptr_t)&memcpy },
	{ "memmove", (uintptr_t)&memmove },
	{ "memset", (uintptr_t)&memset },
	// { "nanosleep", (uintptr_t)&nanosleep },
	{ "pow", (uintptr_t)&pow },
	{ "powf", (uintptr_t)&powf },
	{ "printf", (uintptr_t)&printf },
	{ "puts", (uintptr_t)&puts },
	// { "recv", (uintptr_t)&recv },
	{ "remove", (uintptr_t)&remove },
	{ "rename", (uintptr_t)&rename },
	{ "sin", (uintptr_t)&sin },
	{ "sinf", (uintptr_t)&sinf },
	{ "snprintf", (uintptr_t)&snprintf },
	// { "socket", (uintptr_t)&socket },
	{ "sprintf", (uintptr_t)&sprintf },
	{ "sqrt", (uintptr_t)&sqrt },
	{ "sqrtf", (uintptr_t)&sqrtf },
	{ "srand48", (uintptr_t)&srand48 },
	{ "sscanf", (uintptr_t)&sscanf },
	{ "strcasecmp", (uintptr_t)&strcasecmp },
	{ "strcat", (uintptr_t)&strcat },
	{ "strchr", (uintptr_t)&strchr },
	{ "strcmp", (uintptr_t)&strcmp },
	{ "strcpy", (uintptr_t)&strcpy },
	{ "strdup", (uintptr_t)&strdup },
	{ "strlen", (uintptr_t)&strlen },
	{ "strncmp", (uintptr_t)&strncmp },
	{ "strpbrk", (uintptr_t)&strpbrk },
	{ "strstr", (uintptr_t)&strstr },
	{ "strtod", (uintptr_t)&strtod },
	{ "tan", (uintptr_t)&tan },
	{ "tanf", (uintptr_t)&tanf },
	{ "uname", (uintptr_t)&uname_fake },
	{ "vsprintf", (uintptr_t)&vsprintf },
	{ "wcscmp", (uintptr_t)&wcscmp },
	{ "wcscpy", (uintptr_t)&wcscpy },
	{ "wcslen", (uintptr_t)&wcslen },
	// { "write", (uintptr_t)&write },
};

static char fake_vm[0x1000];
static char fake_env[0x1000];

enum MethodIDs {
	UNKNOWN = 0,
} MethodIDs;

typedef struct {
	char *name;
	enum MethodIDs id;
} NameToMethodID;

static NameToMethodID name_to_method_ids[] = {
};

int GetStaticMethodID(void *env, void *class, const char *name, const char *sig) {
	//sceClibPrintf("%s\n", name);

	for (int i = 0; i < sizeof(name_to_method_ids) / sizeof(NameToMethodID); i++) {
		if (strcmp(name, name_to_method_ids[i].name) == 0)
			return name_to_method_ids[i].id;
	}

	return UNKNOWN;
}

int CallStaticIntMethod(void *env, void *obj, int methodID) {
	return 0;
}

float CallStaticFloatMethod(void *env, void *obj, int methodID) {
	return 0.0f;
}

void CallStaticVoidMethod(void *env, void *obj, int methodID) {
}

void *NewGlobalRef(void) {
	return (void *)0x42424242;
}

int check_kubridge(void) {
	int search_unk[2];
	return _vshKernelSearchModuleByName("kubridge", search_unk);
}

int file_exists(const char *path) {
	SceIoStat stat;
	return sceIoGetstat(path, &stat) >= 0;
}

typedef struct {
	float x;
	float y;
} touch_spot;

enum {
	INPUT_PUNCH,
	INPUT_JUMP,
	INPUT_PROJECTILE,
	INPUT_SUPER,
	INPUT_DANGER,
	INPUT_PAUSE,
	INPUT_MOVEMENT_BASE
};

enum {
	KEYCODE_CIRCLE = 4,
	KEYCODE_DPAD_UP = 19,
	KEYCODE_DPAD_DOWN = 20,
	KEYCODE_DPAD_LEFT = 21,
	KEYCODE_DPAD_RIGHT = 22,
	KEYCODE_CROSS = 23,
	KEYCODE_SQUARE = 99,
	KEYCODE_TRIANGLE = 100,
	KEYCODE_BUTTON_L1 = 102,
	KEYCODE_BUTTON_R1 = 103,
	KEYCODE_BUTTON_START = 108,
	KEYCODE_BUTTON_SELECT = 109,
};

enum {
	SCANCODE_UNK = 0,
	SCANCODE_LEFT = 103,
	SCANCODE_DOWN = 105,
	SCANCODE_UP = 106,
	SCANCODE_RIGHT = 108,
	SCANCODE_START = 139,
	SCANCODE_CROSS = 304,
	SCANCODE_CIRCLE = 305,
	SCANCODE_SQUARE = 307,
	SCANCODE_TRIANGLE = 308,
	SCANCODE_L1 = 310,
	SCANCODE_R1 = 311,
	SCANCODE_SELECT = 314
};

int GetEnv(void *vm, void **env, int r2) {
	*env = fake_env;
	return 0;
}

int AttachCurrentThread(void *vm, void **p_env, void *thr_args) {
	GetEnv(vm, p_env, 0);
	return 0;
}

void video_open(const char *path);
int video_draw();
void video_stop();

int main(int argc, char *argv[]) {
	sceSysmoduleLoadModule(SCE_SYSMODULE_AVPLAYER);
	SceCommonDialogConfigParam cmnDlgCfgParam;
	SceAppUtilInitParam appUtilParam;
	SceAppUtilBootParam appUtilBootParam;
	memset(&appUtilParam, 0, sizeof(SceAppUtilInitParam));
	memset(&appUtilBootParam, 0, sizeof(SceAppUtilBootParam));
	sceAppUtilInit(&appUtilParam, &appUtilBootParam);
	sceCommonDialogConfigParamInit(&cmnDlgCfgParam);
	sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_LANG, (int *)&cmnDlgCfgParam.language);
	sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_ENTER_BUTTON, (int *)&cmnDlgCfgParam.enterButtonAssign);
	sceCommonDialogSetConfigParam(&cmnDlgCfgParam);

	sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG_WIDE);

	scePowerSetArmClockFrequency(444);
	scePowerSetBusClockFrequency(222);
	scePowerSetGpuClockFrequency(222);
	scePowerSetGpuXbarClockFrequency(166);

	if (check_kubridge() < 0)
		fatal_error("Error kubridge.skprx is not installed.");

	if (!file_exists("ur0:/data/libshacccg.suprx") && !file_exists("ur0:/data/external/libshacccg.suprx"))
		fatal_error("Error libshacccg.suprx is not installed.");

	if (so_load(&spiderman_mod, SO_PATH, LOAD_ADDRESS) < 0)
		fatal_error("Error could not load %s.", SO_PATH);

	so_relocate(&spiderman_mod);
	so_resolve(&spiderman_mod, default_dynlib, sizeof(default_dynlib), 0);
	
	void (* appKeyPressed)(int id, int scancode) = (void *)so_symbol(&spiderman_mod, "appKeyPressed");
	void (* appKeyReleased)(int id, int scancode) = (void *)so_symbol(&spiderman_mod, "appKeyReleased");
	
	vglInitExtended(0, SCREEN_W, SCREEN_H, MEMORY_VITAGL_THRESHOLD_MB * 1024 * 1024, SCE_GXM_MULTISAMPLE_4X);
	
	uint16_t signature = *(uint16_t *)appKeyPressed;
	if (signature == 0x65b5) { // Xperia Build
		is_xperia = 1;
		sceClibPrintf("Xperia Build detected\n");
		video_open("ux0:data/spiderman/gameloft/games/spiderman/logo.mp4");
	} else {
		sceClibPrintf("Android Build detected\n");
		video_open("ux0:data/spiderman/gameloft/games/GloftSMHP/logo.mp4");
	}
	
	// Play logo video
	SceCtrlData pad;
	while (!video_draw()) {
		sceCtrlPeekBufferPositive(0, &pad, 1);
		if (pad.buttons)
			video_stop();
		vglSwapBuffers(GL_FALSE);
	}
	
	patch_game();
	so_flush_caches(&spiderman_mod);
	so_initialize(&spiderman_mod);

	memset(fake_vm, 'A', sizeof(fake_vm));
	*(uintptr_t *)(fake_vm + 0x00) = (uintptr_t)fake_vm; // just point to itself...
	*(uintptr_t *)(fake_vm + 0x10) = (uintptr_t)AttachCurrentThread;
	*(uintptr_t *)(fake_vm + 0x18) = (uintptr_t)GetEnv;

	int (* JNI_OnLoad)(void *vm, void *reserved) = (void *)so_symbol(&spiderman_mod, "JNI_OnLoad");
	if (JNI_OnLoad)
		JNI_OnLoad(fake_vm, NULL);
	

	memset(fake_env, 'A', sizeof(fake_env));
	*(uintptr_t *)(fake_env + 0x00) = (uintptr_t)fake_env; // just point to itself...
	*(uintptr_t *)(fake_env + 0x54) = (uintptr_t)NewGlobalRef;
	*(uintptr_t *)(fake_env + 0x1C4) = (uintptr_t)GetStaticMethodID;
	*(uintptr_t *)(fake_env + 0x204) = (uintptr_t)CallStaticIntMethod;
	*(uintptr_t *)(fake_env + 0x21C) = (uintptr_t)CallStaticFloatMethod;
	*(uintptr_t *)(fake_env + 0x234) = (uintptr_t)CallStaticVoidMethod;

	int (* Java_com_gameloft_android_GAND_GloftSMHP_ML_GameRenderer_nativeRendererGetJNIEnv)(void *env) = (void *)so_symbol(&spiderman_mod, "Java_com_gameloft_android_GAND_GloftSMHP_ML_GameRenderer_nativeRendererGetJNIEnv");
	int (* Java_com_gameloft_android_GAND_GloftSMHP_ML_GameRenderer_nativeResize)(void *env, void *obj, int width, int height) = (void *)so_symbol(&spiderman_mod, "Java_com_gameloft_android_GAND_GloftSMHP_ML_GameRenderer_nativeResize");
	int (* Java_com_gameloft_android_GAND_GloftSMHP_ML_GameRenderer_nativeRendererInit)(void *env, void *obj, int r2, int type) = (void *)so_symbol(&spiderman_mod, "Java_com_gameloft_android_GAND_GloftSMHP_ML_GameRenderer_nativeRendererInit");
	int (* Java_com_gameloft_android_GAND_GloftSMHP_ML_GameRenderer_nativeUpdateAndRender)() = (void *)so_symbol(&spiderman_mod, "Java_com_gameloft_android_GAND_GloftSMHP_ML_GameRenderer_nativeUpdateAndRender");

	int (* Java_com_gameloft_android_GAND_GloftSMHP_ML_GLMediaPlayer_nativeInit)(void *env, void *obj) = (void *)so_symbol(&spiderman_mod, "Java_com_gameloft_android_GAND_GloftSMHP_ML_GLMediaPlayer_nativeInit");

	int (*nativeOnTouch_Android)(void *env, void *obj, int action, int x, int y, int index) = (void *)so_symbol(&spiderman_mod, "Java_com_gameloft_android_GAND_GloftSMHP_ML_GameGLSurfaceView_nativeOnTouch");
	int (*nativeOnTouch_Xperia)(void *env, void *obj, int port, int action, int x, int y, int index) = nativeOnTouch_Android;
	
	Java_com_gameloft_android_GAND_GloftSMHP_ML_GameRenderer_nativeRendererGetJNIEnv(fake_env);
	Java_com_gameloft_android_GAND_GloftSMHP_ML_GameRenderer_nativeResize(fake_env, NULL, SCREEN_W, SCREEN_H);
	Java_com_gameloft_android_GAND_GloftSMHP_ML_GameRenderer_nativeRendererInit(fake_env, NULL, 0, 0);

	Java_com_gameloft_android_GAND_GloftSMHP_ML_GLMediaPlayer_nativeInit(fake_env, NULL);
	
	int lastX[2] = { -1, -1 };
	int lastY[2] = { -1, -1 };

	touch_spot fake_inputs[5];
	fake_inputs[INPUT_JUMP].x = 886.0f;
	fake_inputs[INPUT_JUMP].y = 354.0f;
	fake_inputs[INPUT_PUNCH].x = 769.0f;
	fake_inputs[INPUT_PUNCH].y = 354.0f;
	fake_inputs[INPUT_PROJECTILE].x = 730.0f;
	fake_inputs[INPUT_PROJECTILE].y = 460.0f;
	fake_inputs[INPUT_SUPER].x = 824.0f;
	fake_inputs[INPUT_SUPER].y = 460.0f;
	fake_inputs[INPUT_DANGER].x = 330.0f;
	fake_inputs[INPUT_DANGER].y = 496.0f;
	fake_inputs[INPUT_PAUSE].x = 30.0f;
	fake_inputs[INPUT_PAUSE].y = 30.0f;
	fake_inputs[INPUT_MOVEMENT_BASE].x = 132.0f;
	fake_inputs[INPUT_MOVEMENT_BASE].y = 436.0f;
	
	#define physicalToTouch(btn, id) \
		if ((pad.buttons & btn) == btn) { \
			nativeOnTouch_Android(fake_env, NULL, 1, fake_inputs[id].x, fake_inputs[id].y, 3); \
		} else if ((oldpad & btn) == btn) { \
			nativeOnTouch_Android(fake_env, NULL, 0, fake_inputs[id].x, fake_inputs[id].y, 3); \
		}
	
	#define physicalInput(btn, id, code) \
		if ((pad.buttons & btn) == btn) { \
			appKeyPressed(id, code); \
		} else if ((oldpad & btn) == btn) { \
			appKeyReleased(id, code); \
		}		
	
	uint32_t oldpad = 0;
	int joy_active = 0;
	while (1) {
		SceTouchData touch;
		sceTouchPeek(SCE_TOUCH_PORT_FRONT, &touch, 1);
		SceCtrlData pad;
		sceCtrlPeekBufferPositive(0, &pad, 1);
		
		if (is_xperia) { // Xperia build
			if (pad.lx > 162) {
				pad.buttons |= SCE_CTRL_RIGHT;
			} else if (pad.lx < 92) {
				pad.buttons |= SCE_CTRL_LEFT;
			}
			if (pad.ly > 162) {
				pad.buttons |= SCE_CTRL_DOWN;
			} else if (pad.ly < 92) {
				pad.buttons |= SCE_CTRL_UP;
			}
			
			physicalInput(SCE_CTRL_CROSS, KEYCODE_CROSS, SCANCODE_CROSS)
			physicalInput(SCE_CTRL_CIRCLE, KEYCODE_CIRCLE, SCANCODE_CIRCLE)
			physicalInput(SCE_CTRL_SQUARE, KEYCODE_SQUARE, SCANCODE_SQUARE)
			physicalInput(SCE_CTRL_TRIANGLE, KEYCODE_TRIANGLE, SCANCODE_TRIANGLE)
			physicalInput(SCE_CTRL_UP, KEYCODE_DPAD_UP, SCANCODE_UP)
			physicalInput(SCE_CTRL_LEFT, KEYCODE_DPAD_LEFT, SCANCODE_LEFT)
			physicalInput(SCE_CTRL_RIGHT, KEYCODE_DPAD_RIGHT, SCANCODE_RIGHT)
			physicalInput(SCE_CTRL_DOWN, KEYCODE_DPAD_DOWN, SCANCODE_DOWN)
			physicalInput(SCE_CTRL_LTRIGGER, KEYCODE_BUTTON_L1, SCANCODE_L1)
			physicalInput(SCE_CTRL_RTRIGGER, KEYCODE_BUTTON_R1, SCANCODE_R1)
			physicalInput(SCE_CTRL_SELECT, KEYCODE_BUTTON_SELECT, SCANCODE_SELECT)
			physicalInput(SCE_CTRL_START, KEYCODE_BUTTON_START, SCANCODE_START)
		} else { // Generic build
			physicalToTouch(SCE_CTRL_CROSS, INPUT_JUMP)
			physicalToTouch(SCE_CTRL_SQUARE, INPUT_PUNCH)
			physicalToTouch(SCE_CTRL_CIRCLE, INPUT_PROJECTILE)
			physicalToTouch(SCE_CTRL_TRIANGLE, INPUT_SUPER)
			physicalToTouch(SCE_CTRL_LTRIGGER, INPUT_DANGER)
			physicalToTouch(SCE_CTRL_START, INPUT_PAUSE)
		
		
			if (pad.lx > 162 || pad.lx < 92 || pad.ly > 162 || pad.ly < 92) {
				if (joy_active) {
					float delta_x = ((float)pad.lx - 127.0f) / 2.0f;
					float delta_y = ((float)pad.ly - 127.0f) / 2.0f;
					nativeOnTouch_Android(fake_env, NULL, 2, fake_inputs[INPUT_MOVEMENT_BASE].x + delta_x, fake_inputs[INPUT_MOVEMENT_BASE].y + delta_y, 2);
				} else {
					nativeOnTouch_Android(fake_env, NULL, 1, fake_inputs[INPUT_MOVEMENT_BASE].x, fake_inputs[INPUT_MOVEMENT_BASE].y, 2);
					joy_active = 1;
				}
			} else {
				joy_active = 0;
				nativeOnTouch_Android(fake_env, NULL, 0, fake_inputs[INPUT_MOVEMENT_BASE].x, fake_inputs[INPUT_MOVEMENT_BASE].y, 2);
			}
		}
		oldpad = pad.buttons;

		for (int i = 0; i < 2; i++) {
			if (i < touch.reportNum) {
				int x = (int)((float)touch.report[i].x * (float)SCREEN_W / 1920.0f);
				int y = (int)((float)touch.report[i].y * (float)SCREEN_H / 1088.0f);

				if (lastX[i] == -1 || lastY[i] == -1) {
					is_xperia ? nativeOnTouch_Xperia(fake_env, NULL, 0, 1, x, y, i) : nativeOnTouch_Android(fake_env, NULL, 1, x, y, i);
				} else if (lastX[i] != x || lastY[i] != y) {
					is_xperia ? nativeOnTouch_Xperia(fake_env, NULL, 0, 2, x, y, i) : nativeOnTouch_Android(fake_env, NULL, 2, x, y, i);
				}
				lastX[i] = x;
				lastY[i] = y;
			} else {
				if (lastX[i] != -1 || lastY[i] != -1) {
					is_xperia ? nativeOnTouch_Xperia(fake_env, NULL, 0, 0, lastX[i], lastY[i], i) : nativeOnTouch_Android(fake_env, NULL, 0, lastX[i], lastY[i], i);
				}
				lastX[i] = -1;
				lastY[i] = -1;
			}
		}
		
		Java_com_gameloft_android_GAND_GloftSMHP_ML_GameRenderer_nativeUpdateAndRender();
		vglSwapBuffers(GL_FALSE);
	}

	return 0;
}
