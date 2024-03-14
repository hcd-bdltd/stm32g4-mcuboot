/**
 * @file flash_map_backend.c
 * @author B Duncan LTD (info@bduncanltd.com)
 * @brief Flash map backend for MCUboot
 * @version 0.1
 * @date 2024-03-14
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "flash_map_backend/flash_map_backend.h"

int flash_area_open(uint8_t id, const struct flash_area **area_outp)
{
	return -1;
}

void flash_area_close(const struct flash_area *fa)
{
	return -1;
}

int flash_area_read(const struct flash_area *fa, uint32_t off, void *dst, uint32_t len)
{
	return -1;
}

int flash_area_write(const struct flash_area *fa, uint32_t off, const void *src, uint32_t len)
{
	return -1;
}

int flash_area_erase(const struct flash_area *fa, uint32_t off, uint32_t len)
{
	return -1;
}

uint32_t flash_area_align(const struct flash_area *area)
{
	return -1;
}

uint8_t flash_area_erased_val(const struct flash_area *area)
{
	return -1;
}

int flash_area_get_sectors(int fa_id, uint32_t *count, struct flash_sector *sectors)
{
	return -1;
}

int flash_area_sector_from_off(uint32_t off, struct flash_sector *sector)
{
	return -1;
}

int flash_area_get_sector(const struct flash_area *area, uint32_t off, struct flash_sector *sector)
{
	return -1;
}

int flash_area_id_from_multi_image_slot(int image_index, int slot)
{
	return -1;
}

int flash_area_id_from_image_slot(int slot)
{
	return -1;
}

int flash_area_to_sectors(int idx, int *cnt, struct flash_area *fa)
{
	return -1;
}
