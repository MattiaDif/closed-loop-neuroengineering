/**********************************************************************/
/*   ____  ____                                                       */
/*  /   /\/   /                                                       */
/* /___/  \  /                                                        */
/* \   \   \/                                                       */
/*  \   \        Copyright (c) 2003-2009 Xilinx, Inc.                */
/*  /   /          All Right Reserved.                                 */
/* /---/   /\                                                         */
/* \   \  /  \                                                      */
/*  \___\/\___\                                                    */
/***********************************************************************/

/* This file is designed for use with ISim build 0xfbc00daa */

#define XSI_HIDE_SYMBOL_SPEC true
#include "xsi.h"
#include <memory.h>
#ifdef __GNUC__
#include <stdlib.h>
#else
#include <malloc.h>
#define alloca _alloca
#endif
static const char *ng0 = "/home/ise/ise_projects/RHS2000InterfaceXEM6010 release 180814_CustArch_v2_tb/DualPortRAM_generic.v";
static int ng1[] = {0, 0};
static int ng2[] = {2, 0};
static int ng3[] = {1, 0};
static unsigned int ng4[] = {1U, 0U};



static void Initial_49_0(char *t0)
{
    char t7[8];
    char t8[8];
    char t9[8];
    char t18[8];
    char t19[8];
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    char *t6;
    char *t10;
    unsigned int t11;
    unsigned int t12;
    unsigned int t13;
    unsigned int t14;
    unsigned int t15;
    char *t16;
    char *t17;
    char *t20;
    char *t21;
    char *t22;
    char *t23;
    char *t24;
    char *t25;
    char *t26;
    char *t27;
    char *t28;
    char *t29;
    unsigned int t30;
    int t31;
    char *t32;
    unsigned int t33;
    int t34;
    int t35;
    unsigned int t36;
    unsigned int t37;
    int t38;
    int t39;

LAB0:    xsi_set_current_line(49, ng0);

LAB2:    xsi_set_current_line(50, ng0);
    xsi_set_current_line(50, ng0);
    t1 = ((char*)((ng1)));
    t2 = (t0 + 3616);
    xsi_vlogvar_assign_value(t2, t1, 0, 0, 32);

LAB3:    t1 = (t0 + 3616);
    t2 = (t1 + 56U);
    t3 = *((char **)t2);
    t4 = ((char*)((ng2)));
    t5 = (t0 + 768);
    t6 = *((char **)t5);
    memset(t7, 0, 8);
    xsi_vlog_signed_power(t7, 32, t4, 32, t6, 32, 1);
    t5 = ((char*)((ng3)));
    memset(t8, 0, 8);
    xsi_vlog_signed_minus(t8, 32, t7, 32, t5, 32);
    memset(t9, 0, 8);
    xsi_vlog_signed_leq(t9, 32, t3, 32, t8, 32);
    t10 = (t9 + 4);
    t11 = *((unsigned int *)t10);
    t12 = (~(t11));
    t13 = *((unsigned int *)t9);
    t14 = (t13 & t12);
    t15 = (t14 != 0);
    if (t15 > 0)
        goto LAB4;

LAB5:    xsi_set_current_line(53, ng0);
    t1 = ((char*)((ng1)));
    t2 = (t0 + 3296);
    xsi_vlogvar_assign_value(t2, t1, 0, 0, 16);
    xsi_set_current_line(54, ng0);
    t1 = ((char*)((ng1)));
    t2 = (t0 + 3456);
    xsi_vlogvar_assign_value(t2, t1, 0, 0, 16);

LAB1:    return;
LAB4:    xsi_set_current_line(50, ng0);

LAB6:    xsi_set_current_line(51, ng0);
    t16 = ((char*)((ng1)));
    t17 = (t0 + 3136);
    t20 = (t0 + 3136);
    t21 = (t20 + 72U);
    t22 = *((char **)t21);
    t23 = (t0 + 3136);
    t24 = (t23 + 64U);
    t25 = *((char **)t24);
    t26 = (t0 + 3616);
    t27 = (t26 + 56U);
    t28 = *((char **)t27);
    xsi_vlog_generic_convert_array_indices(t18, t19, t22, t25, 2, 1, t28, 32, 1);
    t29 = (t18 + 4);
    t30 = *((unsigned int *)t29);
    t31 = (!(t30));
    t32 = (t19 + 4);
    t33 = *((unsigned int *)t32);
    t34 = (!(t33));
    t35 = (t31 && t34);
    if (t35 == 1)
        goto LAB7;

LAB8:    xsi_set_current_line(50, ng0);
    t1 = (t0 + 3616);
    t2 = (t1 + 56U);
    t3 = *((char **)t2);
    t4 = ((char*)((ng3)));
    memset(t7, 0, 8);
    xsi_vlog_signed_add(t7, 32, t3, 32, t4, 32);
    t5 = (t0 + 3616);
    xsi_vlogvar_assign_value(t5, t7, 0, 0, 32);
    goto LAB3;

LAB7:    t36 = *((unsigned int *)t18);
    t37 = *((unsigned int *)t19);
    t38 = (t36 - t37);
    t39 = (t38 + 1);
    xsi_vlogvar_assign_value(t17, t16, 0, *((unsigned int *)t19), t39);
    goto LAB8;

}

static void Always_58_1(char *t0)
{
    char t8[8];
    char t32[8];
    char t56[8];
    char t57[8];
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;
    char *t9;
    char *t10;
    unsigned int t11;
    unsigned int t12;
    unsigned int t13;
    unsigned int t14;
    unsigned int t15;
    unsigned int t16;
    unsigned int t17;
    unsigned int t18;
    unsigned int t19;
    unsigned int t20;
    unsigned int t21;
    unsigned int t22;
    char *t23;
    char *t24;
    unsigned int t25;
    unsigned int t26;
    unsigned int t27;
    unsigned int t28;
    unsigned int t29;
    char *t30;
    char *t31;
    char *t33;
    char *t34;
    unsigned int t35;
    unsigned int t36;
    unsigned int t37;
    unsigned int t38;
    unsigned int t39;
    unsigned int t40;
    unsigned int t41;
    unsigned int t42;
    unsigned int t43;
    unsigned int t44;
    unsigned int t45;
    unsigned int t46;
    char *t47;
    char *t48;
    unsigned int t49;
    unsigned int t50;
    unsigned int t51;
    unsigned int t52;
    unsigned int t53;
    char *t54;
    char *t55;
    char *t58;
    char *t59;
    char *t60;
    char *t61;
    char *t62;
    char *t63;
    char *t64;
    char *t65;
    unsigned int t66;
    int t67;
    char *t68;
    unsigned int t69;
    int t70;
    int t71;
    unsigned int t72;
    unsigned int t73;
    int t74;
    int t75;

LAB0:    t1 = (t0 + 4776U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(58, ng0);
    t2 = (t0 + 5592);
    *((int *)t2) = 1;
    t3 = (t0 + 4808);
    *((char **)t3) = t2;
    *((char **)t1) = &&LAB4;

LAB1:    return;
LAB4:    xsi_set_current_line(59, ng0);

LAB5:    t4 = (t0 + 280);
    xsi_vlog_namedbase_setdisablestate(t4, &&LAB6);
    t5 = (t0 + 4584);
    xsi_vlog_namedbase_pushprocess(t4, t5);

LAB7:    xsi_set_current_line(60, ng0);
    t6 = (t0 + 1776U);
    t7 = *((char **)t6);
    t6 = ((char*)((ng4)));
    memset(t8, 0, 8);
    t9 = (t7 + 4);
    t10 = (t6 + 4);
    t11 = *((unsigned int *)t7);
    t12 = *((unsigned int *)t6);
    t13 = (t11 ^ t12);
    t14 = *((unsigned int *)t9);
    t15 = *((unsigned int *)t10);
    t16 = (t14 ^ t15);
    t17 = (t13 | t16);
    t18 = *((unsigned int *)t9);
    t19 = *((unsigned int *)t10);
    t20 = (t18 | t19);
    t21 = (~(t20));
    t22 = (t17 & t21);
    if (t22 != 0)
        goto LAB11;

LAB8:    if (t20 != 0)
        goto LAB10;

LAB9:    *((unsigned int *)t8) = 1;

LAB11:    t24 = (t8 + 4);
    t25 = *((unsigned int *)t24);
    t26 = (~(t25));
    t27 = *((unsigned int *)t8);
    t28 = (t27 & t26);
    t29 = (t28 != 0);
    if (t29 > 0)
        goto LAB12;

LAB13:
LAB14:    t2 = (t0 + 280);
    xsi_vlog_namedbase_popprocess(t2);

LAB6:    t3 = (t0 + 4584);
    xsi_vlog_dispose_process_subprogram_invocation(t3);
    goto LAB2;

LAB10:    t23 = (t8 + 4);
    *((unsigned int *)t8) = 1;
    *((unsigned int *)t23) = 1;
    goto LAB11;

LAB12:    xsi_set_current_line(60, ng0);

LAB15:    xsi_set_current_line(61, ng0);
    t30 = (t0 + 2256U);
    t31 = *((char **)t30);
    t30 = ((char*)((ng4)));
    memset(t32, 0, 8);
    t33 = (t31 + 4);
    t34 = (t30 + 4);
    t35 = *((unsigned int *)t31);
    t36 = *((unsigned int *)t30);
    t37 = (t35 ^ t36);
    t38 = *((unsigned int *)t33);
    t39 = *((unsigned int *)t34);
    t40 = (t38 ^ t39);
    t41 = (t37 | t40);
    t42 = *((unsigned int *)t33);
    t43 = *((unsigned int *)t34);
    t44 = (t42 | t43);
    t45 = (~(t44));
    t46 = (t41 & t45);
    if (t46 != 0)
        goto LAB19;

LAB16:    if (t44 != 0)
        goto LAB18;

LAB17:    *((unsigned int *)t32) = 1;

LAB19:    t48 = (t32 + 4);
    t49 = *((unsigned int *)t48);
    t50 = (~(t49));
    t51 = *((unsigned int *)t32);
    t52 = (t51 & t50);
    t53 = (t52 != 0);
    if (t53 > 0)
        goto LAB20;

LAB21:    xsi_set_current_line(65, ng0);

LAB26:    xsi_set_current_line(66, ng0);
    t2 = (t0 + 3136);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t0 + 3136);
    t6 = (t5 + 72U);
    t7 = *((char **)t6);
    t9 = (t0 + 3136);
    t10 = (t9 + 64U);
    t23 = *((char **)t10);
    t24 = (t0 + 2096U);
    t30 = *((char **)t24);
    xsi_vlog_generic_get_array_select_value(t8, 16, t4, t7, t23, 2, 1, t30, 9, 2);
    t24 = (t0 + 3456);
    xsi_vlogvar_wait_assign_value(t24, t8, 0, 0, 16, 0LL);

LAB22:    xsi_set_current_line(68, ng0);
    t2 = (t0 + 3136);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t0 + 3136);
    t6 = (t5 + 72U);
    t7 = *((char **)t6);
    t9 = (t0 + 3136);
    t10 = (t9 + 64U);
    t23 = *((char **)t10);
    t24 = (t0 + 2416U);
    t30 = *((char **)t24);
    xsi_vlog_generic_get_array_select_value(t8, 16, t4, t7, t23, 2, 1, t30, 9, 2);
    t24 = (t0 + 3296);
    xsi_vlogvar_wait_assign_value(t24, t8, 0, 0, 16, 0LL);
    goto LAB14;

LAB18:    t47 = (t32 + 4);
    *((unsigned int *)t32) = 1;
    *((unsigned int *)t47) = 1;
    goto LAB19;

LAB20:    xsi_set_current_line(61, ng0);

LAB23:    xsi_set_current_line(62, ng0);
    t54 = (t0 + 1936U);
    t55 = *((char **)t54);
    t54 = (t0 + 3136);
    t58 = (t0 + 3136);
    t59 = (t58 + 72U);
    t60 = *((char **)t59);
    t61 = (t0 + 3136);
    t62 = (t61 + 64U);
    t63 = *((char **)t62);
    t64 = (t0 + 2096U);
    t65 = *((char **)t64);
    xsi_vlog_generic_convert_array_indices(t56, t57, t60, t63, 2, 1, t65, 9, 2);
    t64 = (t56 + 4);
    t66 = *((unsigned int *)t64);
    t67 = (!(t66));
    t68 = (t57 + 4);
    t69 = *((unsigned int *)t68);
    t70 = (!(t69));
    t71 = (t67 && t70);
    if (t71 == 1)
        goto LAB24;

LAB25:    xsi_set_current_line(63, ng0);
    t2 = (t0 + 1936U);
    t3 = *((char **)t2);
    t2 = (t0 + 3456);
    xsi_vlogvar_wait_assign_value(t2, t3, 0, 0, 16, 0LL);
    goto LAB22;

LAB24:    t72 = *((unsigned int *)t56);
    t73 = *((unsigned int *)t57);
    t74 = (t72 - t73);
    t75 = (t74 + 1);
    xsi_vlogvar_wait_assign_value(t54, t55, 0, *((unsigned int *)t57), t75, 0LL);
    goto LAB25;

}

static void Cont_72_2(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;
    char *t8;
    char *t9;
    unsigned int t10;
    unsigned int t11;
    char *t12;
    unsigned int t13;
    unsigned int t14;
    char *t15;
    unsigned int t16;
    unsigned int t17;
    char *t18;

LAB0:    t1 = (t0 + 5024U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(72, ng0);
    t2 = (t0 + 3296);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t0 + 5704);
    t6 = (t5 + 56U);
    t7 = *((char **)t6);
    t8 = (t7 + 56U);
    t9 = *((char **)t8);
    memset(t9, 0, 8);
    t10 = 65535U;
    t11 = t10;
    t12 = (t4 + 4);
    t13 = *((unsigned int *)t4);
    t10 = (t10 & t13);
    t14 = *((unsigned int *)t12);
    t11 = (t11 & t14);
    t15 = (t9 + 4);
    t16 = *((unsigned int *)t9);
    *((unsigned int *)t9) = (t16 | t10);
    t17 = *((unsigned int *)t15);
    *((unsigned int *)t15) = (t17 | t11);
    xsi_driver_vfirst_trans(t5, 0, 15);
    t18 = (t0 + 5608);
    *((int *)t18) = 1;

LAB1:    return;
}

static void Cont_73_3(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;
    char *t8;
    char *t9;
    unsigned int t10;
    unsigned int t11;
    char *t12;
    unsigned int t13;
    unsigned int t14;
    char *t15;
    unsigned int t16;
    unsigned int t17;
    char *t18;

LAB0:    t1 = (t0 + 5272U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(73, ng0);
    t2 = (t0 + 3456);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t0 + 5768);
    t6 = (t5 + 56U);
    t7 = *((char **)t6);
    t8 = (t7 + 56U);
    t9 = *((char **)t8);
    memset(t9, 0, 8);
    t10 = 65535U;
    t11 = t10;
    t12 = (t4 + 4);
    t13 = *((unsigned int *)t4);
    t10 = (t10 & t13);
    t14 = *((unsigned int *)t12);
    t11 = (t11 & t14);
    t15 = (t9 + 4);
    t16 = *((unsigned int *)t9);
    *((unsigned int *)t9) = (t16 | t10);
    t17 = *((unsigned int *)t15);
    *((unsigned int *)t15) = (t17 | t11);
    xsi_driver_vfirst_trans(t5, 0, 15);
    t18 = (t0 + 5624);
    *((int *)t18) = 1;

LAB1:    return;
}


extern void work_m_09793240656028029928_3973276404_init()
{
	static char *pe[] = {(void *)Initial_49_0,(void *)Always_58_1,(void *)Cont_72_2,(void *)Cont_73_3};
	xsi_register_didat("work_m_09793240656028029928_3973276404", "isim/custom_architecture_tb_isim_beh.exe.sim/work/m_09793240656028029928_3973276404.didat");
	xsi_register_executes(pe);
}
