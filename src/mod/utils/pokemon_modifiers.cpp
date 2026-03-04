#include "exlaunch.hpp"

#include "externals/Pml/PokePara/CoreParam.h"
#include "utils/utils.h"

const uint16_t BITSA_TOTEMFLAG_LOC = 6;
const uint16_t BITSA_TOTEMFLAG_SZ = 1;
const uint16_t BITSA_TOTEMFLAG_MASK = 64;

const uint16_t BITSA_ALPHAFLAG_LOC = 7;
const uint16_t BITSA_ALPHAFLAG_SZ = 1;
const uint16_t BITSA_ALPHAFLAG_MASK = 128;

const uint16_t BITSA_NOBLEFLAG_LOC = 8;
const uint16_t BITSA_NOBLEFLAG_SZ = 1;
const uint16_t BITSA_NOBLEFLAG_MASK = 256;

const uint16_t BITSA_TITANFLAG_LOC = 9;
const uint16_t BITSA_TITANFLAG_SZ = 1;
const uint16_t BITSA_TITANFLAG_MASK = 512;

const uint16_t BITSA_SHADOWFLAG_LOC = 10;
const uint16_t BITSA_SHADOWFLAG_SZ = 1;
const uint16_t BITSA_SHADOWFLAG_MASK = 1024;

const uint16_t BITSA_PURIFIEDSHADOWFLAG_LOC = 11;
const uint16_t BITSA_PURIFIEDSHADOWFLAG_SZ = 1;
const uint16_t BITSA_PURIFIEDSHADOWFLAG_MASK = 2048;

// Get the Totem modifier flag from a CoreParam.
bool IsTotem(Pml::PokePara::CoreParam::Object* param) {
    auto accessor = param->fields.m_accessor;
    auto addr = (accessor->fields.m_pCoreData == nullptr || accessor->fields.m_pCoreData->max_length == 0) ? nullptr : accessor->fields.m_pCoreData->m_Items;

    accessor->DecodeAndCheckIllegalWrite();
    auto blockA = accessor->GetCoreDataBlockA(addr, false);
    auto value = ((blockA->fields._bitsA & BITSA_TOTEMFLAG_MASK) >> BITSA_TOTEMFLAG_LOC) != 0;
    accessor->UpdateChecksumAndEncode();

    return value;
}

// Get the Alpha modifier flag from a CoreParam.
bool IsAlpha(Pml::PokePara::CoreParam::Object* param) {
    auto accessor = param->fields.m_accessor;
    auto addr = (accessor->fields.m_pCoreData == nullptr || accessor->fields.m_pCoreData->max_length == 0) ? nullptr : accessor->fields.m_pCoreData->m_Items;

    accessor->DecodeAndCheckIllegalWrite();
    auto blockA = accessor->GetCoreDataBlockA(addr, false);
    auto value = ((blockA->fields._bitsA & BITSA_ALPHAFLAG_MASK) >> BITSA_ALPHAFLAG_LOC) != 0;
    accessor->UpdateChecksumAndEncode();

    return value;
}

// Get the Noble modifier flag from a CoreParam.
bool IsNoble(Pml::PokePara::CoreParam::Object* param) {
    auto accessor = param->fields.m_accessor;
    auto addr = (accessor->fields.m_pCoreData == nullptr || accessor->fields.m_pCoreData->max_length == 0) ? nullptr : accessor->fields.m_pCoreData->m_Items;

    accessor->DecodeAndCheckIllegalWrite();
    auto blockA = accessor->GetCoreDataBlockA(addr, false);
    auto value = ((blockA->fields._bitsA & BITSA_NOBLEFLAG_MASK) >> BITSA_NOBLEFLAG_LOC) != 0;
    accessor->UpdateChecksumAndEncode();

    return value;
}

// Get the Titan modifier flag from a CoreParam.
bool IsTitan(Pml::PokePara::CoreParam::Object* param) {
    auto accessor = param->fields.m_accessor;
    auto addr = (accessor->fields.m_pCoreData == nullptr || accessor->fields.m_pCoreData->max_length == 0) ? nullptr : accessor->fields.m_pCoreData->m_Items;

    accessor->DecodeAndCheckIllegalWrite();
    auto blockA = accessor->GetCoreDataBlockA(addr, false);
    auto value = ((blockA->fields._bitsA & BITSA_TITANFLAG_MASK) >> BITSA_TITANFLAG_LOC) != 0;
    accessor->UpdateChecksumAndEncode();

    return value;
}

// Get the Shadow modifier flag from a CoreParam.
bool IsShadow(Pml::PokePara::CoreParam::Object* param) {
    auto accessor = param->fields.m_accessor;
    auto addr = (accessor->fields.m_pCoreData == nullptr || accessor->fields.m_pCoreData->max_length == 0) ? nullptr : accessor->fields.m_pCoreData->m_Items;

    accessor->DecodeAndCheckIllegalWrite();
    auto blockA = accessor->GetCoreDataBlockA(addr, false);
    auto value = ((blockA->fields._bitsA & BITSA_SHADOWFLAG_MASK) >> BITSA_SHADOWFLAG_LOC) != 0;
    accessor->UpdateChecksumAndEncode();

    return value;
}

// Get the Purified Shadow modifier flag from a CoreParam.
bool IsPurifiedShadow(Pml::PokePara::CoreParam::Object* param) {
    auto accessor = param->fields.m_accessor;
    auto addr = (accessor->fields.m_pCoreData == nullptr || accessor->fields.m_pCoreData->max_length == 0) ? nullptr : accessor->fields.m_pCoreData->m_Items;

    accessor->DecodeAndCheckIllegalWrite();
    auto blockA = accessor->GetCoreDataBlockA(addr, false);
    auto value = ((blockA->fields._bitsA & BITSA_PURIFIEDSHADOWFLAG_MASK) >> BITSA_PURIFIEDSHADOWFLAG_LOC) != 0;
    accessor->UpdateChecksumAndEncode();

    return value;
}


// Set the Totem modifier flag on a CoreParam.
void SetTotem(Pml::PokePara::CoreParam::Object* param, bool value) {
    auto accessor = param->fields.m_accessor;
    auto addr = (accessor->fields.m_pCoreData == nullptr || accessor->fields.m_pCoreData->max_length == 0) ? nullptr : accessor->fields.m_pCoreData->m_Items;

    accessor->DecodeAndCheckIllegalWrite();
    auto blockA = accessor->GetCoreDataBlockA(addr, true);
    blockA->fields._bitsA = (blockA->fields._bitsA & ~BITSA_TOTEMFLAG_MASK) | (value ? BITSA_TOTEMFLAG_MASK : 0);
    accessor->UpdateChecksumAndEncode();
}

// Set the Alpha modifier flag on a CoreParam.
void SetAlpha(Pml::PokePara::CoreParam::Object* param, bool value) {
    auto accessor = param->fields.m_accessor;
    auto addr = (accessor->fields.m_pCoreData == nullptr || accessor->fields.m_pCoreData->max_length == 0) ? nullptr : accessor->fields.m_pCoreData->m_Items;

    accessor->DecodeAndCheckIllegalWrite();
    auto blockA = accessor->GetCoreDataBlockA(addr, true);
    blockA->fields._bitsA = (blockA->fields._bitsA & ~BITSA_ALPHAFLAG_MASK) | (value ? BITSA_ALPHAFLAG_MASK : 0);
    accessor->UpdateChecksumAndEncode();
}

// Set the Noble modifier flag on a CoreParam.
void SetNoble(Pml::PokePara::CoreParam::Object* param, bool value) {
    auto accessor = param->fields.m_accessor;
    auto addr = (accessor->fields.m_pCoreData == nullptr || accessor->fields.m_pCoreData->max_length == 0) ? nullptr : accessor->fields.m_pCoreData->m_Items;

    accessor->DecodeAndCheckIllegalWrite();
    auto blockA = accessor->GetCoreDataBlockA(addr, true);
    blockA->fields._bitsA = (blockA->fields._bitsA & ~BITSA_NOBLEFLAG_MASK) | (value ? BITSA_NOBLEFLAG_MASK : 0);
    accessor->UpdateChecksumAndEncode();
}

// Set the Titan modifier flag on a CoreParam.
void SetTitan(Pml::PokePara::CoreParam::Object* param, bool value) {
    auto accessor = param->fields.m_accessor;
    auto addr = (accessor->fields.m_pCoreData == nullptr || accessor->fields.m_pCoreData->max_length == 0) ? nullptr : accessor->fields.m_pCoreData->m_Items;

    accessor->DecodeAndCheckIllegalWrite();
    auto blockA = accessor->GetCoreDataBlockA(addr, true);
    blockA->fields._bitsA = (blockA->fields._bitsA & ~BITSA_TITANFLAG_MASK) | (value ? BITSA_TITANFLAG_MASK : 0);
    accessor->UpdateChecksumAndEncode();
}

// Set the Shadow modifier flag on a CoreParam.
void SetShadow(Pml::PokePara::CoreParam::Object* param, bool value) {
    auto accessor = param->fields.m_accessor;
    auto addr = (accessor->fields.m_pCoreData == nullptr || accessor->fields.m_pCoreData->max_length == 0) ? nullptr : accessor->fields.m_pCoreData->m_Items;

    accessor->DecodeAndCheckIllegalWrite();
    auto blockA = accessor->GetCoreDataBlockA(addr, true);
    blockA->fields._bitsA = (blockA->fields._bitsA & ~BITSA_SHADOWFLAG_MASK) | (value ? BITSA_SHADOWFLAG_MASK : 0);
    accessor->UpdateChecksumAndEncode();
}

// Set the Purified Shadow modifier flag on a CoreParam.
void SetPurifiedShadow(Pml::PokePara::CoreParam::Object* param, bool value) {
    auto accessor = param->fields.m_accessor;
    auto addr = (accessor->fields.m_pCoreData == nullptr || accessor->fields.m_pCoreData->max_length == 0) ? nullptr : accessor->fields.m_pCoreData->m_Items;

    accessor->DecodeAndCheckIllegalWrite();
    auto blockA = accessor->GetCoreDataBlockA(addr, true);
    blockA->fields._bitsA = (blockA->fields._bitsA & ~BITSA_PURIFIEDSHADOWFLAG_MASK) | (value ? BITSA_PURIFIEDSHADOWFLAG_MASK : 0);
    accessor->UpdateChecksumAndEncode();
}