#ifndef ELF_TYPES_H
#define ELF_TYPES_H

#include "utils/traits.h"

#include <stdint.h>
#include <llvm/Support/raw_ostream.h>

namespace detail {
#define ELF_TYPE_WRAPPER(TYPE, IMPL)                                        \
  struct TYPE {                                                             \
    IMPL value;                                                             \
                                                                            \
    TYPE() : value(0) { }                                                   \
    TYPE(IMPL val) : value(val) { }                                         \
                                                                            \
    TYPE &operator=(TYPE const &with) { value = with.value; return *this; } \
    TYPE &operator=(IMPL val) { value = val; return *this; }                \
                                                                            \
    operator IMPL() const { return value; }                                 \
    operator IMPL &() { return value; }                                     \
  };

  ELF_TYPE_WRAPPER(ELFHalf      , uint16_t)
  ELF_TYPE_WRAPPER(ELFWord      , uint32_t)
  ELF_TYPE_WRAPPER(ELFSword     , int32_t)
  ELF_TYPE_WRAPPER(ELFXword     , uint64_t)
  ELF_TYPE_WRAPPER(ELFSxword    , int64_t)
  ELF_TYPE_WRAPPER(ELF32Address , uint32_t)
  ELF_TYPE_WRAPPER(ELF32Offset  , uint32_t)
  ELF_TYPE_WRAPPER(ELF64Address , uint64_t)
  ELF_TYPE_WRAPPER(ELF64Offset  , uint64_t)

#undef ELF_TYPE_WRAPPER

  extern llvm::raw_ostream &operator<<(llvm::raw_ostream &,
                                       ELF32Address const &);
  extern llvm::raw_ostream &operator<<(llvm::raw_ostream &,
                                       ELF32Offset const &);
  extern llvm::raw_ostream &operator<<(llvm::raw_ostream &,
                                       ELF64Address const &);
  extern llvm::raw_ostream &operator<<(llvm::raw_ostream &,
                                       ELF64Offset const &);
}

template <unsigned Bitwidth> class ELFHeader;
template <unsigned Bitwidth> class ELFProgramHeader;
template <unsigned Bitwidth> class ELFSectionHeader;
template <unsigned Bitwidth> class ELFSymbol;
template <unsigned Bitwidth> class ELFRel;
template <unsigned Bitwidth> class ELFRela;

// Note: Following TypeTraits specialization MUST be compliant to the
// System V Application Binary Interface, Chap 4, Data Representation.

TYPE_TRAITS_SPECIALIZE(detail::ELFHalf      , 2, 2)
TYPE_TRAITS_SPECIALIZE(detail::ELFWord      , 4, 4)
TYPE_TRAITS_SPECIALIZE(detail::ELFSword     , 4, 4)
TYPE_TRAITS_SPECIALIZE(detail::ELFXword     , 8, 8)
TYPE_TRAITS_SPECIALIZE(detail::ELFSxword    , 8, 8)
TYPE_TRAITS_SPECIALIZE(detail::ELF32Address , 4, 4)
TYPE_TRAITS_SPECIALIZE(detail::ELF32Offset  , 4, 4)
TYPE_TRAITS_SPECIALIZE(detail::ELF64Address , 8, 8)
TYPE_TRAITS_SPECIALIZE(detail::ELF64Offset  , 8, 8)

TYPE_TRAITS_SPECIALIZE(ELFHeader<32>        , 52, 4)
TYPE_TRAITS_SPECIALIZE(ELFHeader<64>        , 64, 8)

TYPE_TRAITS_SPECIALIZE(ELFProgramHeader<32> , 32, 4)
TYPE_TRAITS_SPECIALIZE(ELFProgramHeader<64> , 56, 8)

TYPE_TRAITS_SPECIALIZE(ELFSectionHeader<32> , 40, 4)
TYPE_TRAITS_SPECIALIZE(ELFSectionHeader<64> , 64, 8)

TYPE_TRAITS_SPECIALIZE(ELFSymbol<32>        , 16, 4)
TYPE_TRAITS_SPECIALIZE(ELFSymbol<64>        , 24, 8)

TYPE_TRAITS_SPECIALIZE(ELFRel<32>           , 8, 4)
TYPE_TRAITS_SPECIALIZE(ELFRel<64>           , 16, 8)

TYPE_TRAITS_SPECIALIZE(ELFRela<32>          , 12, 4)
TYPE_TRAITS_SPECIALIZE(ELFRela<64>          , 24, 8)


template <unsigned Bitwidth>
struct ELFTypes;

template <>
struct ELFTypes<32> {
  typedef detail::ELF32Address  address;
  typedef detail::ELF32Offset   offset;

  typedef unsigned char         byte;
  typedef detail::ELFHalf       half;
  typedef detail::ELFWord       word;
  typedef detail::ELFSword      sword;

  typedef detail::ELFSword      addend;
  typedef detail::ELFWord       symsize;

  // Note: Don't use these types.  They are not in the specification of
  // ELF 32.  However, we need these typedefs to define the type introduce
  // macro.
  typedef void xword;
  typedef void sxword;
};

template <>
struct ELFTypes<64> {
  typedef detail::ELF64Address  address;
  typedef detail::ELF64Offset   offset;

  typedef unsigned char         byte;
  typedef detail::ELFHalf       half;
  typedef detail::ELFWord       word;
  typedef detail::ELFSword      sword;
  typedef detail::ELFXword      xword;
  typedef detail::ELFSxword     sxword;

  typedef detail::ELFSxword     addend;
  typedef detail::ELFXword      symsize;
};

#define ELF_TYPE_INTRO_TO_TEMPLATE_SCOPE(BITWIDTH) \
  typedef typename ELFTypes<BITWIDTH>::address addr_t; \
  typedef typename ELFTypes<BITWIDTH>::offset  offset_t; \
  typedef typename ELFTypes<BITWIDTH>::byte    byte_t; \
  typedef typename ELFTypes<BITWIDTH>::half    half_t; \
  typedef typename ELFTypes<BITWIDTH>::word    word_t; \
  typedef typename ELFTypes<BITWIDTH>::sword   sword_t; \
  typedef typename ELFTypes<BITWIDTH>::xword   xword_t; \
  typedef typename ELFTypes<BITWIDTH>::sxword  sxword_t; \
  typedef typename ELFTypes<BITWIDTH>::addend  addend_t; \
  typedef typename ELFTypes<BITWIDTH>::symsize symsize_t;

#define ELF_TYPE_INTRO_TO_SCOPE(BITWIDTH) \
  typedef ELFTypes<BITWIDTH>::address addr_t; \
  typedef ELFTypes<BITWIDTH>::offset  offset_t; \
  typedef ELFTypes<BITWIDTH>::byte    byte_t; \
  typedef ELFTypes<BITWIDTH>::half    half_t; \
  typedef ELFTypes<BITWIDTH>::word    word_t; \
  typedef ELFTypes<BITWIDTH>::sword   sword_t; \
  typedef ELFTypes<BITWIDTH>::xword   xword_t; \
  typedef ELFTypes<BITWIDTH>::sxword  sxword_t; \
  typedef ELFTypes<BITWIDTH>::addend  addend_t; \
  typedef ELFTypes<BITWIDTH>::symsize symsize_t;


#endif // ELF_TYPES_H
