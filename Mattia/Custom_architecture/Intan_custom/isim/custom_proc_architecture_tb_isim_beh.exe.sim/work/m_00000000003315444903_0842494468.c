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
static const char *ng0 = "C:/GitHub/intan_custom_arch/FPGA/New code/Working_project_from_simulink/RHS2000InterfaceXEM6010 release 180814_cust_arch/custom_proc_architecture_tb.v";
static const char *ng1 = "RHS_data_flow.txt";
static const char *ng2 = "RHS_data_valid.txt";
static const char *ng3 = "output_comp_data_out.txt";
static const char *ng4 = "w";
static int ng5[] = {0, 0};
static int ng6[] = {1, 0};
static unsigned int ng7[] = {1U, 0U};
static const char *ng8 = "Current value of FIFO_data_in is %d";
static const char *ng9 = "%b\n";



static void Initial_76_0(char *t0)
{
    char t3[8];
    char *t1;
    char *t2;
    char *t4;

LAB0:    t1 = (t0 + 2996U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(76, ng0);

LAB4:    xsi_set_current_line(78, ng0);
    t2 = (t0 + 2380);
    xsi_vlogfile_readmemb(ng1, 0, t2, 0, 0, 0, 0);
    xsi_set_current_line(79, ng0);
    t2 = (t0 + 2472);
    xsi_vlogfile_readmemb(ng2, 0, t2, 0, 0, 0, 0);
    xsi_set_current_line(80, ng0);
    *((int *)t3) = xsi_vlogfile_file_open_of_cname_ctype(ng3, ng4);
    t2 = (t3 + 4);
    *((int *)t2) = 0;
    t4 = (t0 + 2196);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 32);
    xsi_set_current_line(81, ng0);
    t2 = ((char*)((ng5)));
    t4 = (t0 + 2288);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 32);
    xsi_set_current_line(84, ng0);
    t2 = ((char*)((ng5)));
    t4 = (t0 + 1368);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 1);
    xsi_set_current_line(85, ng0);
    t2 = ((char*)((ng6)));
    t4 = (t0 + 1460);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 1);
    xsi_set_current_line(86, ng0);
    t2 = ((char*)((ng5)));
    t4 = (t0 + 1552);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 1);
    xsi_set_current_line(87, ng0);
    t2 = ((char*)((ng5)));
    t4 = (t0 + 1644);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 16);
    xsi_set_current_line(88, ng0);
    t2 = ((char*)((ng5)));
    t4 = (t0 + 1736);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 16);
    xsi_set_current_line(89, ng0);
    t2 = ((char*)((ng5)));
    t4 = (t0 + 1828);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 1);
    xsi_set_current_line(90, ng0);
    t2 = ((char*)((ng5)));
    t4 = (t0 + 1920);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 1);
    xsi_set_current_line(91, ng0);
    t2 = ((char*)((ng5)));
    t4 = (t0 + 2012);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 16);
    xsi_set_current_line(92, ng0);
    t2 = ((char*)((ng7)));
    t4 = (t0 + 2104);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 8);
    xsi_set_current_line(93, ng0);
    t2 = (t0 + 2896);
    xsi_process_wait(t2, 40000LL);
    *((char **)t1) = &&LAB5;

LAB1:    return;
LAB5:    xsi_set_current_line(94, ng0);
    t2 = ((char*)((ng5)));
    t4 = (t0 + 1460);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 1);
    xsi_set_current_line(95, ng0);
    t2 = ((char*)((ng7)));
    t4 = (t0 + 2104);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 8);
    goto LAB1;

}

static void Always_123_1(char *t0)
{
    char t3[8];
    char t17[8];
    char t43[8];
    char t68[8];
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
    char *t15;
    char *t16;
    char *t18;
    char *t19;
    unsigned int t20;
    unsigned int t21;
    unsigned int t22;
    unsigned int t23;
    unsigned int t24;
    unsigned int t25;
    unsigned int t26;
    unsigned int t27;
    unsigned int t28;
    unsigned int t29;
    unsigned int t30;
    unsigned int t31;
    char *t32;
    char *t33;
    unsigned int t34;
    unsigned int t35;
    unsigned int t36;
    unsigned int t37;
    unsigned int t38;
    char *t39;
    char *t40;
    char *t41;
    char *t42;
    char *t44;
    char *t45;
    unsigned int t46;
    unsigned int t47;
    unsigned int t48;
    unsigned int t49;
    unsigned int t50;
    unsigned int t51;
    unsigned int t52;
    unsigned int t53;
    unsigned int t54;
    unsigned int t55;
    unsigned int t56;
    unsigned int t57;
    char *t58;
    char *t59;
    unsigned int t60;
    unsigned int t61;
    unsigned int t62;
    unsigned int t63;
    unsigned int t64;
    char *t65;
    char *t66;
    char *t67;
    char *t69;
    char *t70;
    char *t71;
    char *t72;
    char *t73;
    char *t74;
    char *t75;
    char *t76;
    char *t77;
    char *t78;

LAB0:    t1 = (t0 + 3140U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(123, ng0);

LAB4:    xsi_set_current_line(124, ng0);
    t2 = (t0 + 3040);
    xsi_process_wait(t2, 20000LL);
    *((char **)t1) = &&LAB5;

LAB1:    return;
LAB5:    xsi_set_current_line(125, ng0);
    t4 = (t0 + 1368);
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

LAB9:    t14 = (t0 + 1368);
    xsi_vlogvar_assign_value(t14, t3, 0, 0, 1);
    xsi_set_current_line(126, ng0);
    t2 = (t0 + 2288);
    t4 = (t2 + 36U);
    t5 = *((char **)t4);
    t6 = (t0 + 264);
    t7 = *((char **)t6);
    memset(t3, 0, 8);
    xsi_vlog_signed_less(t3, 32, t5, 32, t7, 32);
    t6 = (t3 + 4);
    t8 = *((unsigned int *)t6);
    t9 = (~(t8));
    t10 = *((unsigned int *)t3);
    t11 = (t10 & t9);
    t12 = (t11 != 0);
    if (t12 > 0)
        goto LAB10;

LAB11:
LAB12:    goto LAB2;

LAB6:    *((unsigned int *)t3) = 1;
    goto LAB9;

LAB10:    xsi_set_current_line(126, ng0);

LAB13:    xsi_set_current_line(127, ng0);
    t13 = (t0 + 1460);
    t14 = (t13 + 36U);
    t15 = *((char **)t14);
    t16 = ((char*)((ng5)));
    memset(t17, 0, 8);
    t18 = (t15 + 4);
    t19 = (t16 + 4);
    t20 = *((unsigned int *)t15);
    t21 = *((unsigned int *)t16);
    t22 = (t20 ^ t21);
    t23 = *((unsigned int *)t18);
    t24 = *((unsigned int *)t19);
    t25 = (t23 ^ t24);
    t26 = (t22 | t25);
    t27 = *((unsigned int *)t18);
    t28 = *((unsigned int *)t19);
    t29 = (t27 | t28);
    t30 = (~(t29));
    t31 = (t26 & t30);
    if (t31 != 0)
        goto LAB17;

LAB14:    if (t29 != 0)
        goto LAB16;

LAB15:    *((unsigned int *)t17) = 1;

LAB17:    t33 = (t17 + 4);
    t34 = *((unsigned int *)t33);
    t35 = (~(t34));
    t36 = *((unsigned int *)t17);
    t37 = (t36 & t35);
    t38 = (t37 != 0);
    if (t38 > 0)
        goto LAB18;

LAB19:
LAB20:    goto LAB12;

LAB16:    t32 = (t17 + 4);
    *((unsigned int *)t17) = 1;
    *((unsigned int *)t32) = 1;
    goto LAB17;

LAB18:    xsi_set_current_line(127, ng0);

LAB21:    xsi_set_current_line(128, ng0);
    t39 = (t0 + 1368);
    t40 = (t39 + 36U);
    t41 = *((char **)t40);
    t42 = ((char*)((ng6)));
    memset(t43, 0, 8);
    t44 = (t41 + 4);
    t45 = (t42 + 4);
    t46 = *((unsigned int *)t41);
    t47 = *((unsigned int *)t42);
    t48 = (t46 ^ t47);
    t49 = *((unsigned int *)t44);
    t50 = *((unsigned int *)t45);
    t51 = (t49 ^ t50);
    t52 = (t48 | t51);
    t53 = *((unsigned int *)t44);
    t54 = *((unsigned int *)t45);
    t55 = (t53 | t54);
    t56 = (~(t55));
    t57 = (t52 & t56);
    if (t57 != 0)
        goto LAB25;

LAB22:    if (t55 != 0)
        goto LAB24;

LAB23:    *((unsigned int *)t43) = 1;

LAB25:    t59 = (t43 + 4);
    t60 = *((unsigned int *)t59);
    t61 = (~(t60));
    t62 = *((unsigned int *)t43);
    t63 = (t62 & t61);
    t64 = (t63 != 0);
    if (t64 > 0)
        goto LAB26;

LAB27:
LAB28:    goto LAB20;

LAB24:    t58 = (t43 + 4);
    *((unsigned int *)t43) = 1;
    *((unsigned int *)t58) = 1;
    goto LAB25;

LAB26:    xsi_set_current_line(128, ng0);

LAB29:    xsi_set_current_line(129, ng0);
    t65 = (t0 + 2472);
    t66 = (t65 + 36U);
    t67 = *((char **)t66);
    t69 = (t0 + 2472);
    t70 = (t69 + 44U);
    t71 = *((char **)t70);
    t72 = (t0 + 2472);
    t73 = (t72 + 40U);
    t74 = *((char **)t73);
    t75 = (t0 + 2288);
    t76 = (t75 + 36U);
    t77 = *((char **)t76);
    xsi_vlog_generic_get_array_select_value(t68, 1, t67, t71, t74, 2, 1, t77, 32, 1);
    t78 = (t0 + 1552);
    xsi_vlogvar_assign_value(t78, t68, 0, 0, 1);
    xsi_set_current_line(130, ng0);
    t2 = (t0 + 2380);
    t4 = (t2 + 36U);
    t5 = *((char **)t4);
    t6 = (t0 + 2380);
    t7 = (t6 + 44U);
    t13 = *((char **)t7);
    t14 = (t0 + 2380);
    t15 = (t14 + 40U);
    t16 = *((char **)t15);
    t18 = (t0 + 2288);
    t19 = (t18 + 36U);
    t32 = *((char **)t19);
    xsi_vlog_generic_get_array_select_value(t3, 16, t5, t13, t16, 2, 1, t32, 32, 1);
    t33 = (t0 + 1644);
    xsi_vlogvar_assign_value(t33, t3, 0, 0, 16);
    xsi_set_current_line(131, ng0);
    t2 = (t0 + 2288);
    t4 = (t2 + 36U);
    t5 = *((char **)t4);
    t6 = ((char*)((ng6)));
    memset(t3, 0, 8);
    xsi_vlog_signed_add(t3, 32, t5, 32, t6, 32);
    t7 = (t0 + 2288);
    xsi_vlogvar_assign_value(t7, t3, 0, 0, 32);
    xsi_set_current_line(132, ng0);
    t2 = (t0 + 680U);
    t4 = *((char **)t2);
    t2 = ((char*)((ng6)));
    memset(t3, 0, 8);
    t5 = (t4 + 4);
    t6 = (t2 + 4);
    t8 = *((unsigned int *)t4);
    t9 = *((unsigned int *)t2);
    t10 = (t8 ^ t9);
    t11 = *((unsigned int *)t5);
    t12 = *((unsigned int *)t6);
    t20 = (t11 ^ t12);
    t21 = (t10 | t20);
    t22 = *((unsigned int *)t5);
    t23 = *((unsigned int *)t6);
    t24 = (t22 | t23);
    t25 = (~(t24));
    t26 = (t21 & t25);
    if (t26 != 0)
        goto LAB33;

LAB30:    if (t24 != 0)
        goto LAB32;

LAB31:    *((unsigned int *)t3) = 1;

LAB33:    t13 = (t3 + 4);
    t27 = *((unsigned int *)t13);
    t28 = (~(t27));
    t29 = *((unsigned int *)t3);
    t30 = (t29 & t28);
    t31 = (t30 != 0);
    if (t31 > 0)
        goto LAB34;

LAB35:
LAB36:    goto LAB28;

LAB32:    t7 = (t3 + 4);
    *((unsigned int *)t3) = 1;
    *((unsigned int *)t7) = 1;
    goto LAB33;

LAB34:    xsi_set_current_line(132, ng0);

LAB37:    xsi_set_current_line(133, ng0);
    t14 = (t0 + 1644);
    t15 = (t14 + 36U);
    t16 = *((char **)t15);
    xsi_vlogfile_write(1, 0, 0, ng8, 2, t0, (char)118, t16, 16);
    xsi_set_current_line(134, ng0);
    t2 = (t0 + 2196);
    t4 = (t2 + 36U);
    t5 = *((char **)t4);
    t6 = (t0 + 772U);
    t7 = *((char **)t6);
    xsi_vlogfile_fwrite(*((unsigned int *)t5), 0, 0, 1, ng9, 2, t0, (char)118, t7, 16);
    goto LAB36;

}


extern void work_m_00000000003315444903_0842494468_init()
{
	static char *pe[] = {(void *)Initial_76_0,(void *)Always_123_1};
	xsi_register_didat("work_m_00000000003315444903_0842494468", "isim/custom_proc_architecture_tb_isim_beh.exe.sim/work/m_00000000003315444903_0842494468.didat");
	xsi_register_executes(pe);
}
