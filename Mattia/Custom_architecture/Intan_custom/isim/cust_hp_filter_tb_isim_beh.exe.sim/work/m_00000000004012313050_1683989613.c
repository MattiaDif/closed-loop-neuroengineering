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

/* This file is designed for use with ISim build 0x8ef4fb42 */

#define XSI_HIDE_SYMBOL_SPEC true
#include "xsi.h"
#include <memory.h>
#ifdef __GNUC__
#include <stdlib.h>
#else
#include <malloc.h>
#define alloca _alloca
#endif
static const char *ng0 = "C:/GitHub/intan_custom_arch/FPGA/New code/Working_project_from_simulink/RHS2000InterfaceXEM6010 release 180814_cust_arch/cust_hp_filter_tb.v";
static int ng1[] = {0, 0};
static int ng2[] = {1, 0};
static int ng3[] = {3000, 0};
static const char *ng4 = "data_for_filter.txt";
static const char *ng5 = "filter_output.txt";
static const char *ng6 = "w";
static int ng7[] = {159, 0};
static unsigned int ng8[] = {1U, 0U};



static void Initial_65_0(char *t0)
{
    char t4[8];
    char *t1;
    char *t2;
    char *t3;

LAB0:    t1 = (t0 + 2536U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(65, ng0);

LAB4:    xsi_set_current_line(67, ng0);
    t2 = ((char*)((ng1)));
    t3 = (t0 + 1184);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 1);
    xsi_set_current_line(68, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 1276);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 1);
    xsi_set_current_line(69, ng0);
    t2 = ((char*)((ng1)));
    t3 = (t0 + 1368);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 16);
    xsi_set_current_line(70, ng0);
    t2 = ((char*)((ng1)));
    t3 = (t0 + 1460);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 8);
    xsi_set_current_line(71, ng0);
    t2 = ((char*)((ng1)));
    t3 = (t0 + 1552);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 1);
    xsi_set_current_line(72, ng0);
    t2 = ((char*)((ng1)));
    t3 = (t0 + 1644);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 1);
    xsi_set_current_line(73, ng0);
    t2 = ((char*)((ng3)));
    t3 = (t0 + 1736);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 16);
    xsi_set_current_line(74, ng0);
    t2 = ((char*)((ng1)));
    t3 = (t0 + 1828);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 8);
    xsi_set_current_line(76, ng0);
    t2 = (t0 + 2436);
    xsi_process_wait(t2, 500000LL);
    *((char **)t1) = &&LAB5;

LAB1:    return;
LAB5:    xsi_set_current_line(77, ng0);
    t2 = ((char*)((ng1)));
    t3 = (t0 + 1276);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 1);
    xsi_set_current_line(79, ng0);
    t2 = (t0 + 2012);
    xsi_vlogfile_readmemb(ng4, 0, t2, 0, 0, 0, 0);
    xsi_set_current_line(80, ng0);
    *((int *)t4) = xsi_vlogfile_file_open_of_cname_ctype(ng5, ng6);
    t2 = (t4 + 4);
    *((int *)t2) = 0;
    t3 = (t0 + 1920);
    xsi_vlogvar_assign_value(t3, t4, 0, 0, 32);
    goto LAB1;

}

static void Always_83_1(char *t0)
{
    char t3[8];
    char t32[8];
    char t58[8];
    char *t1;
    char *t2;
    char *t4;
    char *t5;
    char *t6;
    char *t7;
    unsigned int t8;
    unsigned int t9;
    unsigned int t10;
    unsigned int t11;
    unsigned int t12;
    char *t13;
    char *t14;
    unsigned int t15;
    unsigned int t16;
    unsigned int t17;
    unsigned int t18;
    unsigned int t19;
    unsigned int t20;
    unsigned int t21;
    char *t22;
    unsigned int t23;
    unsigned int t24;
    unsigned int t25;
    unsigned int t26;
    unsigned int t27;
    char *t28;
    char *t29;
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
    char *t56;
    char *t57;
    char *t59;
    char *t60;
    char *t61;
    char *t62;
    unsigned int t63;
    unsigned int t64;
    unsigned int t65;
    unsigned int t66;
    unsigned int t67;
    char *t68;
    char *t69;

LAB0:    t1 = (t0 + 2680U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(83, ng0);

LAB4:    xsi_set_current_line(84, ng0);
    t2 = (t0 + 2580);
    xsi_process_wait(t2, 20000LL);
    *((char **)t1) = &&LAB5;

LAB1:    return;
LAB5:    xsi_set_current_line(85, ng0);
    t4 = (t0 + 1184);
    t5 = (t4 + 36U);
    t6 = *((char **)t5);
    memset(t3, 0, 8);
    t7 = (t6 + 4);
    t8 = *((unsigned int *)t7);
    t9 = (~(t8));
    t10 = *((unsigned int *)t6);
    t11 = (t10 & t9);
    t12 = (t11 & 1U);
    if (t12 != 0)
        goto LAB9;

LAB7:    if (*((unsigned int *)t7) == 0)
        goto LAB6;

LAB8:    t13 = (t3 + 4);
    *((unsigned int *)t3) = 1;
    *((unsigned int *)t13) = 1;

LAB9:    t14 = (t0 + 1184);
    xsi_vlogvar_assign_value(t14, t3, 0, 0, 1);
    xsi_set_current_line(86, ng0);
    t2 = (t0 + 1276);
    t4 = (t2 + 36U);
    t5 = *((char **)t4);
    t6 = ((char*)((ng1)));
    memset(t3, 0, 8);
    t7 = (t5 + 4);
    t13 = (t6 + 4);
    t8 = *((unsigned int *)t5);
    t9 = *((unsigned int *)t6);
    t10 = (t8 ^ t9);
    t11 = *((unsigned int *)t7);
    t12 = *((unsigned int *)t13);
    t15 = (t11 ^ t12);
    t16 = (t10 | t15);
    t17 = *((unsigned int *)t7);
    t18 = *((unsigned int *)t13);
    t19 = (t17 | t18);
    t20 = (~(t19));
    t21 = (t16 & t20);
    if (t21 != 0)
        goto LAB13;

LAB10:    if (t19 != 0)
        goto LAB12;

LAB11:    *((unsigned int *)t3) = 1;

LAB13:    t22 = (t3 + 4);
    t23 = *((unsigned int *)t22);
    t24 = (~(t23));
    t25 = *((unsigned int *)t3);
    t26 = (t25 & t24);
    t27 = (t26 != 0);
    if (t27 > 0)
        goto LAB14;

LAB15:
LAB16:    goto LAB2;

LAB6:    *((unsigned int *)t3) = 1;
    goto LAB9;

LAB12:    t14 = (t3 + 4);
    *((unsigned int *)t3) = 1;
    *((unsigned int *)t14) = 1;
    goto LAB13;

LAB14:    xsi_set_current_line(86, ng0);

LAB17:    xsi_set_current_line(87, ng0);
    t28 = (t0 + 1184);
    t29 = (t28 + 36U);
    t30 = *((char **)t29);
    t31 = ((char*)((ng2)));
    memset(t32, 0, 8);
    t33 = (t30 + 4);
    t34 = (t31 + 4);
    t35 = *((unsigned int *)t30);
    t36 = *((unsigned int *)t31);
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
        goto LAB21;

LAB18:    if (t44 != 0)
        goto LAB20;

LAB19:    *((unsigned int *)t32) = 1;

LAB21:    t48 = (t32 + 4);
    t49 = *((unsigned int *)t48);
    t50 = (~(t49));
    t51 = *((unsigned int *)t32);
    t52 = (t51 & t50);
    t53 = (t52 != 0);
    if (t53 > 0)
        goto LAB22;

LAB23:
LAB24:    goto LAB16;

LAB20:    t47 = (t32 + 4);
    *((unsigned int *)t32) = 1;
    *((unsigned int *)t47) = 1;
    goto LAB21;

LAB22:    xsi_set_current_line(87, ng0);

LAB25:    xsi_set_current_line(88, ng0);
    t54 = (t0 + 1828);
    t55 = (t54 + 36U);
    t56 = *((char **)t55);
    t57 = ((char*)((ng7)));
    memset(t58, 0, 8);
    t59 = (t56 + 4);
    if (*((unsigned int *)t59) != 0)
        goto LAB27;

LAB26:    t60 = (t57 + 4);
    if (*((unsigned int *)t60) != 0)
        goto LAB27;

LAB30:    if (*((unsigned int *)t56) > *((unsigned int *)t57))
        goto LAB28;

LAB29:    t62 = (t58 + 4);
    t63 = *((unsigned int *)t62);
    t64 = (~(t63));
    t65 = *((unsigned int *)t58);
    t66 = (t65 & t64);
    t67 = (t66 != 0);
    if (t67 > 0)
        goto LAB31;

LAB32:
LAB33:    xsi_set_current_line(91, ng0);
    t2 = (t0 + 2012);
    t4 = (t2 + 36U);
    t5 = *((char **)t4);
    t6 = (t0 + 2012);
    t7 = (t6 + 44U);
    t13 = *((char **)t7);
    t14 = (t0 + 2012);
    t22 = (t14 + 40U);
    t28 = *((char **)t22);
    t29 = ((char*)((ng8)));
    t30 = (t0 + 1828);
    t31 = (t30 + 36U);
    t33 = *((char **)t31);
    xsi_vlogtype_concat(t32, 9, 9, 2U, t33, 8, t29, 1);
    xsi_vlog_generic_get_array_select_value(t3, 16, t5, t13, t28, 2, 1, t32, 9, 2);
    t34 = (t0 + 1368);
    xsi_vlogvar_assign_value(t34, t3, 0, 0, 16);
    xsi_set_current_line(92, ng0);
    t2 = (t0 + 1828);
    t4 = (t2 + 36U);
    t5 = *((char **)t4);
    t6 = (t0 + 1460);
    xsi_vlogvar_assign_value(t6, t5, 0, 0, 8);
    xsi_set_current_line(93, ng0);
    t2 = ((char*)((ng2)));
    t4 = (t0 + 1552);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 1);
    xsi_set_current_line(94, ng0);
    t2 = ((char*)((ng2)));
    t4 = (t0 + 1644);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 1);
    xsi_set_current_line(95, ng0);
    t2 = (t0 + 1828);
    t4 = (t2 + 36U);
    t5 = *((char **)t4);
    t6 = ((char*)((ng2)));
    memset(t3, 0, 8);
    xsi_vlog_unsigned_add(t3, 32, t5, 8, t6, 32);
    t7 = (t0 + 1828);
    xsi_vlogvar_assign_value(t7, t3, 0, 0, 8);
    goto LAB24;

LAB27:    t61 = (t58 + 4);
    *((unsigned int *)t58) = 1;
    *((unsigned int *)t61) = 1;
    goto LAB29;

LAB28:    *((unsigned int *)t58) = 1;
    goto LAB29;

LAB31:    xsi_set_current_line(88, ng0);

LAB34:    xsi_set_current_line(89, ng0);
    t68 = ((char*)((ng1)));
    t69 = (t0 + 1828);
    xsi_vlogvar_assign_value(t69, t68, 0, 0, 8);
    goto LAB33;

}


extern void work_m_00000000004012313050_1683989613_init()
{
	static char *pe[] = {(void *)Initial_65_0,(void *)Always_83_1};
	xsi_register_didat("work_m_00000000004012313050_1683989613", "isim/cust_hp_filter_tb_isim_beh.exe.sim/work/m_00000000004012313050_1683989613.didat");
	xsi_register_executes(pe);
}
