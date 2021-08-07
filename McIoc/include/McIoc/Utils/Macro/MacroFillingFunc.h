/*
 * MIT License
 *
 * Copyright (c) 2021 mrcao20
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once

#define MC_FILLING_FUNC_1(FUNC, args) FUNC(args)
#define MC_FILLING_FUNC_2(FUNC, args, ...)                                        \
    FUNC(args) MC_FILLING_FUNC_1(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_3(FUNC, args, ...)                                        \
    FUNC(args) MC_FILLING_FUNC_2(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_4(FUNC, args, ...)                                        \
    FUNC(args) MC_FILLING_FUNC_3(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_5(FUNC, args, ...)                                        \
    FUNC(args) MC_FILLING_FUNC_4(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_6(FUNC, args, ...)                                        \
    FUNC(args) MC_FILLING_FUNC_5(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_7(FUNC, args, ...)                                        \
    FUNC(args) MC_FILLING_FUNC_6(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_8(FUNC, args, ...)                                        \
    FUNC(args) MC_FILLING_FUNC_7(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_9(FUNC, args, ...)                                        \
    FUNC(args) MC_FILLING_FUNC_8(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_10(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_9(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_11(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_10(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_12(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_11(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_13(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_12(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_14(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_13(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_15(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_14(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_16(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_15(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_17(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_16(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_18(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_17(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_19(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_18(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_20(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_19(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_21(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_20(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_22(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_21(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_23(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_22(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_24(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_23(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_25(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_24(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_26(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_25(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_27(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_26(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_28(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_27(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_29(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_28(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_30(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_29(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_31(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_30(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_32(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_31(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_33(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_32(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_34(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_33(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_35(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_34(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_36(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_35(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_37(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_36(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_38(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_37(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_39(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_38(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_40(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_39(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_41(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_40(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_42(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_41(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_43(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_42(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_44(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_43(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_45(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_44(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_46(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_45(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_47(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_46(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_48(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_47(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_49(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_48(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_50(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_49(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_51(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_50(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_52(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_51(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_53(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_52(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_54(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_53(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_55(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_54(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_56(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_55(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_57(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_56(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_58(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_57(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_59(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_58(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_60(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_59(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_61(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_60(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_62(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_61(FUNC, __VA_ARGS__)
#define MC_FILLING_FUNC_63(FUNC, args, ...)                                       \
    FUNC(args) MC_FILLING_FUNC_62(FUNC, __VA_ARGS__)
