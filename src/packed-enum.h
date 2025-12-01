#ifndef __PACKED_ENUM_H
#define __PACKED_ENUM_H

/**
 * PACKED_ENUM macro
 * ----------------
 * Creates 1-byte enums in a cross-compiler compatible way
 * 
 * Usage:
 *   PACKED_ENUM(enum_name) { ... };
 *
 * Purpose: 
 *   Ensures enums (e.g., block_property_e, bhawana_effect_e in block.h)
 *   are 1 byte to optimize memory in structs like block_t
 */

// GCC, Clang, Intel, IBM XL, SunCC, Green Hills
#if defined(__GNUC__) || defined(__clang__) || defined(__INTEL_COMPILER) || defined(__INTEL_CLANG_COMPILER) || defined(__IBMC__) || defined(__xlC__) || defined(__SUNPRO_C) || defined(__ghs__)
   #define PACKED_ENUM(enum_name) enum __attribute__((packed)) enum_name

// MSVC, Pelles C (Requires C11 or higher)
#elif (defined(_MSC_VER) || defined(__POCC__)) && defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L) 
   #define PACKED_ENUM(enum_name) enum enum_name : uint8_t

// ARM Compiler (armcc, not armclang)
#elif defined(__ARMCC_VERSION) && !defined(__clang__)
   #define PACKED_ENUM(enum_name) __packed enum enum_name

// IAR
#elif defined(__IAR_SYSTEMS_ICC__)
   #define PACKED_ENUM(enum_name) enum __packed enum_name

// Fallback: Standard enum, may be 4 bytes depending on compiler
#else
   #warning "Warning: PACKED_ENUM(...) will not produce 1-byte enums. Check documentation for enum packing options."
   #define PACKED_ENUM(enum_name) enum enum_name

#endif

#endif /* __PACKED_ENUM_H */