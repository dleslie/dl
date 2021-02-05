#ifndef DL_MACRO_H
#define DL_MACRO_H 1

#include "dl_core.h"
#include "dl_memory.h"

#if DL_USE_EXTENIONS && DL_IS_ATLEAST_C90 && (DL_IS_GNUC || DL_IS_CLANG)
#define dl_auto __auto_type
#define DL_SWAP(a, b) ({ \
  dl_auto _c = (a);      \
  a = b;                 \
  b = _c;                \
})
#else
#define DL_SWAP(a, b) dl_memory_swap(&(a), &(b), sizeof(a))
#endif

#if DL_IS_ATLEAST_C99

#define __DL_COUNT_ARGS(X100, X99, X98, X97, X96, X95, X94, X93, X92, X91, X90, X89, X88, X87, X86, X85, X84, X83, X82, X81, X80, X79, X78, X77, X76, X75, X74, X73, X72, X71, X70, X69, X68, X67, X66, X65, X64, X63, X62, X61, X60, X59, X58, X57, X56, X55, X54, X53, X52, X51, X50, X49, X48, X47, X46, X45, X44, X43, X42, X41, X40, X39, X38, X37, X36, X35, X34, X33, X32, X31, X30, X29, X28, X27, X26, X25, X24, X23, X22, X21, X20, X19, X18, X17, X16, X15, X14, X13, X12, X11, X10, X9, X8, X7, X6, X5, X4, X3, X2, X1, N, ...) N

#define DL_COUNT_ARGS(...) __DL_COUNT_ARGS(__VA_ARGS__, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define DL_COUNT_ARGS_STRING(...) __DL_COUNT_ARGS(__VA_ARGS__, "100", "99", "98", "97", "96", "95", "94", "93", "92", "91", "90", "89", "88", "87", "86", "85", "84", "83", "82", "81", "80", "79", "78", "77", "76", "75", "74", "73", "72", "71", "70", "69", "68", "67", "66", "65", "64", "63", "62", "61", "60", "59", "58", "57", "56", "55", "54", "53", "52", "51", "50", "49", "48", "47", "46", "45", "44", "43", "42", "41", "40", "39", "38", "37", "36", "35", "34", "33", "32", "31", "30", "29", "28", "27", "26", "25", "24", "23", "22", "21", "20", "19", "18", "17", "16", "15", "14", "13", "12", "11", "10", "9", "8", "7", "6", "5", "4", "3", "2", "1")

#define DL_EXPAND(X) X
#define DL_ARGLIST(...) (__VA_ARGS__)
#define DL_ARGCAR(X, ...) (X)
#define DL_ARGCDR(X, ...) (__VA_ARGS__)
#define DL_ARGMAP(MACRO, LST) _DL_ARGMAP_(DL_COUNT_ARGS LST, MACRO, LST)
#define _DL_ARGMAP_(N, M, LST) _DL_ARGMAP__(N, M, LST)
#define _DL_ARGMAP__(N, M, LST) _DL_ARGMAP_##N(M, LST)
#define _DL_ARGMAP_1(M, LST) M LST
#define _DL_ARGMAP_2(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST) \
  _DL_ARGMAP_1(M, DL_ARGCDR LST)
#define _DL_ARGMAP_3(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST) \
  _DL_ARGMAP_2(M, DL_ARGCDR LST)
#define _DL_ARGMAP_4(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST) \
  _DL_ARGMAP_3(M, DL_ARGCDR LST)
#define _DL_ARGMAP_5(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST) \
  _DL_ARGMAP_4(M, DL_ARGCDR LST)
#define _DL_ARGMAP_6(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST) \
  _DL_ARGMAP_5(M, DL_ARGCDR LST)
#define _DL_ARGMAP_7(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST) \
  _DL_ARGMAP_6(M, DL_ARGCDR LST)
#define _DL_ARGMAP_8(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST) \
  _DL_ARGMAP_7(M, DL_ARGCDR LST)
#define _DL_ARGMAP_9(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST) \
  _DL_ARGMAP_8(M, DL_ARGCDR LST)
#define _DL_ARGMAP_10(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_9(M, DL_ARGCDR LST)
#define _DL_ARGMAP_11(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_10(M, DL_ARGCDR LST)
#define _DL_ARGMAP_12(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_11(M, DL_ARGCDR LST)
#define _DL_ARGMAP_13(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_12(M, DL_ARGCDR LST)
#define _DL_ARGMAP_14(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_13(M, DL_ARGCDR LST)
#define _DL_ARGMAP_15(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_14(M, DL_ARGCDR LST)
#define _DL_ARGMAP_16(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_15(M, DL_ARGCDR LST)
#define _DL_ARGMAP_17(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_16(M, DL_ARGCDR LST)
#define _DL_ARGMAP_18(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_17(M, DL_ARGCDR LST)
#define _DL_ARGMAP_19(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_18(M, DL_ARGCDR LST)
#define _DL_ARGMAP_20(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_19(M, DL_ARGCDR LST)
#define _DL_ARGMAP_21(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_20(M, DL_ARGCDR LST)
#define _DL_ARGMAP_22(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_21(M, DL_ARGCDR LST)
#define _DL_ARGMAP_23(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_22(M, DL_ARGCDR LST)
#define _DL_ARGMAP_24(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_23(M, DL_ARGCDR LST)
#define _DL_ARGMAP_25(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_24(M, DL_ARGCDR LST)
#define _DL_ARGMAP_26(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_25(M, DL_ARGCDR LST)
#define _DL_ARGMAP_27(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_26(M, DL_ARGCDR LST)
#define _DL_ARGMAP_28(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_27(M, DL_ARGCDR LST)
#define _DL_ARGMAP_29(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_28(M, DL_ARGCDR LST)
#define _DL_ARGMAP_30(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_29(M, DL_ARGCDR LST)
#define _DL_ARGMAP_31(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_30(M, DL_ARGCDR LST)
#define _DL_ARGMAP_32(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_31(M, DL_ARGCDR LST)
#define _DL_ARGMAP_33(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_32(M, DL_ARGCDR LST)
#define _DL_ARGMAP_34(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_33(M, DL_ARGCDR LST)
#define _DL_ARGMAP_35(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_34(M, DL_ARGCDR LST)
#define _DL_ARGMAP_36(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_35(M, DL_ARGCDR LST)
#define _DL_ARGMAP_37(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_36(M, DL_ARGCDR LST)
#define _DL_ARGMAP_38(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_37(M, DL_ARGCDR LST)
#define _DL_ARGMAP_39(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_38(M, DL_ARGCDR LST)
#define _DL_ARGMAP_40(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_39(M, DL_ARGCDR LST)
#define _DL_ARGMAP_41(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_40(M, DL_ARGCDR LST)
#define _DL_ARGMAP_42(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_41(M, DL_ARGCDR LST)
#define _DL_ARGMAP_43(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_42(M, DL_ARGCDR LST)
#define _DL_ARGMAP_44(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_43(M, DL_ARGCDR LST)
#define _DL_ARGMAP_45(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_44(M, DL_ARGCDR LST)
#define _DL_ARGMAP_46(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_45(M, DL_ARGCDR LST)
#define _DL_ARGMAP_47(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_46(M, DL_ARGCDR LST)
#define _DL_ARGMAP_48(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_47(M, DL_ARGCDR LST)
#define _DL_ARGMAP_49(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_48(M, DL_ARGCDR LST)
#define _DL_ARGMAP_50(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_49(M, DL_ARGCDR LST)
#define _DL_ARGMAP_51(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_50(M, DL_ARGCDR LST)
#define _DL_ARGMAP_52(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_51(M, DL_ARGCDR LST)
#define _DL_ARGMAP_53(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_52(M, DL_ARGCDR LST)
#define _DL_ARGMAP_54(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_53(M, DL_ARGCDR LST)
#define _DL_ARGMAP_55(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_54(M, DL_ARGCDR LST)
#define _DL_ARGMAP_56(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_55(M, DL_ARGCDR LST)
#define _DL_ARGMAP_57(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_56(M, DL_ARGCDR LST)
#define _DL_ARGMAP_58(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_57(M, DL_ARGCDR LST)
#define _DL_ARGMAP_59(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_58(M, DL_ARGCDR LST)
#define _DL_ARGMAP_60(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_59(M, DL_ARGCDR LST)
#define _DL_ARGMAP_61(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_60(M, DL_ARGCDR LST)
#define _DL_ARGMAP_62(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_61(M, DL_ARGCDR LST)
#define _DL_ARGMAP_63(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_62(M, DL_ARGCDR LST)
#define _DL_ARGMAP_64(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_63(M, DL_ARGCDR LST)
#define _DL_ARGMAP_65(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_64(M, DL_ARGCDR LST)
#define _DL_ARGMAP_66(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_65(M, DL_ARGCDR LST)
#define _DL_ARGMAP_67(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_66(M, DL_ARGCDR LST)
#define _DL_ARGMAP_68(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_67(M, DL_ARGCDR LST)
#define _DL_ARGMAP_69(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_68(M, DL_ARGCDR LST)
#define _DL_ARGMAP_70(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_69(M, DL_ARGCDR LST)
#define _DL_ARGMAP_71(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_70(M, DL_ARGCDR LST)
#define _DL_ARGMAP_72(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_71(M, DL_ARGCDR LST)
#define _DL_ARGMAP_73(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_72(M, DL_ARGCDR LST)
#define _DL_ARGMAP_74(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_73(M, DL_ARGCDR LST)
#define _DL_ARGMAP_75(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_74(M, DL_ARGCDR LST)
#define _DL_ARGMAP_76(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_75(M, DL_ARGCDR LST)
#define _DL_ARGMAP_77(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_76(M, DL_ARGCDR LST)
#define _DL_ARGMAP_78(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_77(M, DL_ARGCDR LST)
#define _DL_ARGMAP_79(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_78(M, DL_ARGCDR LST)
#define _DL_ARGMAP_80(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_79(M, DL_ARGCDR LST)
#define _DL_ARGMAP_81(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_80(M, DL_ARGCDR LST)
#define _DL_ARGMAP_82(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_81(M, DL_ARGCDR LST)
#define _DL_ARGMAP_83(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_82(M, DL_ARGCDR LST)
#define _DL_ARGMAP_84(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_83(M, DL_ARGCDR LST)
#define _DL_ARGMAP_85(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_84(M, DL_ARGCDR LST)
#define _DL_ARGMAP_86(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_85(M, DL_ARGCDR LST)
#define _DL_ARGMAP_87(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_86(M, DL_ARGCDR LST)
#define _DL_ARGMAP_88(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_87(M, DL_ARGCDR LST)
#define _DL_ARGMAP_89(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_88(M, DL_ARGCDR LST)
#define _DL_ARGMAP_90(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_89(M, DL_ARGCDR LST)
#define _DL_ARGMAP_91(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_90(M, DL_ARGCDR LST)
#define _DL_ARGMAP_92(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_91(M, DL_ARGCDR LST)
#define _DL_ARGMAP_93(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_92(M, DL_ARGCDR LST)
#define _DL_ARGMAP_94(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_93(M, DL_ARGCDR LST)
#define _DL_ARGMAP_95(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_94(M, DL_ARGCDR LST)
#define _DL_ARGMAP_96(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_95(M, DL_ARGCDR LST)
#define _DL_ARGMAP_97(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_96(M, DL_ARGCDR LST)
#define _DL_ARGMAP_98(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_97(M, DL_ARGCDR LST)
#define _DL_ARGMAP_99(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)  \
  _DL_ARGMAP_98(M, DL_ARGCDR LST)
#define _DL_ARGMAP_100(M, LST) \
  DL_EXPAND(M DL_ARGCAR LST)   \
  _DL_ARGMAP_99(M, DL_ARGCDR LST)

#define DL_STRINGIFY(X) #X
#define DL_COMMASPLICE(X) X,
#define DL_STRINGIFY_COMMASPLICE(X) DL_COMMASPLICE(DL_STRINGIFY(X))

#define DL_DEFINE_ENUM(name, ...)                                          \
  enum name##_enum{DL_ARGMAP(DL_COMMASPLICE, (__VA_ARGS__)) name##_count}; \
  const char *name##_strings[] = {DL_ARGMAP(DL_STRINGIFY_COMMASPLICE, (__VA_ARGS__)) DL_COUNT_ARGS_STRING(__VA_ARGS__)};

#define DL_IS_BITFLAG(value) (0 == (value & (value - 1)))

#endif /* DL_IS_ATLEAST_C99 */

#endif
