// SPDX-License-Identifier: GPL-3.0-or-later
//
// VB Lab Migration Assistant for Flipper Zero
// Copyright (C) 2022  cyanic
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include <furi.h>
#include <lib/nfc/nfc_device.h>

#define BANT_MAGIC (0x544E4142)

typedef struct {
    uint32_t magic;
    // Note: this should be big endian, but for convenience, we'll treat them as little endian
    uint16_t item_id;
    uint16_t item_no;
    uint8_t status;
    uint8_t dim_no;
    uint8_t operation;
    uint8_t reserved;
    uint8_t app_flag;
    uint8_t padding[3];
} __attribute__((packed)) BantBlock;

typedef enum {
    VbTagTypeUnknown,
    VbTagTypeVBDM,
    VbTagTypeVBV,
    VbTagTypeVBC,
    VbTagTypeVH,
    VbTagTypeMax
} VbTagType;

typedef struct {
    uint16_t item_id;
    uint16_t item_no;
    const char* name;
    const char* short_name;
    VbTagType type;
} VbTagProduct;

typedef enum {
    VbTagStatusReady = 1 << 0,
    VbTagStatusDimReady = 1 << 1,
} VbTagStatus;

typedef enum {
    VbTagOperationIdle,
    VbTagOperationReady,
    VbTagOperationTransferToApp,
    VbTagOperationCheckDim,
    VbTagOperationReturnFromApp,
    VbTagOperationSpotInit,
    VbTagOperationSpotCommit,
} VbTagOperation;

BantBlock* vb_tag_get_bant_block(NfcDeviceData* dev);
const VbTagProduct* vb_tag_find_product(const BantBlock* bant);
bool vb_tag_validate_product(NfcDeviceData* dev);
VbTagStatus vb_tag_get_status(const BantBlock* bant);
void vb_tag_set_status(BantBlock* bant, VbTagStatus status);
VbTagOperation vb_tag_get_operation(const BantBlock* bant);
void vb_tag_set_operation(BantBlock* bant, VbTagOperation operation);
const VbTagProduct* vb_tag_get_default_product(VbTagType type);
void vb_tag_set_item_id_no(BantBlock* bant, const VbTagProduct* product);
const char* vb_tag_get_tag_type_name(VbTagType type);