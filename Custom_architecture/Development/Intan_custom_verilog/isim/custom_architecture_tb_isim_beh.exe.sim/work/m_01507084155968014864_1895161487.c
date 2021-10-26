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
static const char *ng0 = "/home/ise/ise_projects/RHS2000InterfaceXEM6010 release 180814_CustArch_v2_tb/custom_architecture_tb.v";
static const char *ng1 = "fsm_out_chip10000000_nframe300.txt";
static const char *ng2 = "fsm_valid_chip10000000_nframe300.txt";
static const char *ng3 = "out_comp_data_out.txt";
static const char *ng4 = "w";
static int ng5[] = {0, 0};
static int ng6[] = {1, 0};
static unsigned int ng7[] = {1U, 0U};
static const char *ng8 = "Current value of FIFO_data_in is %d";
static const char *ng9 = "%b\n";



static void Initial_82_0(char *t0)
{
    char t3[8];
    char *t1;
    char *t2;
    char *t4;

LAB0:    t1 = (t0 + 5536U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(82, ng0);

LAB4:    xsi_set_current_line(85, ng0);
    t2 = (t0 + 4464);
    xsi_vlogfile_readmemb(ng1, 0, t2, 0, 0, 0, 0);
    xsi_set_current_line(86, ng0);
    t2 = (t0 + 4624);
    xsi_vlogfile_readmemb(ng2, 0, t2, 0, 0, 0, 0);
    xsi_set_current_line(87, ng0);
    *((int *)t3) = xsi_vlogfile_file_open_of_cname_ctype(ng3, ng4);
    t2 = (t3 + 4);
    *((int *)t2) = 0;
    t4 = (t0 + 4144);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 32);
    xsi_set_current_line(88, ng0);
    t2 = ((char*)((ng5)));
    t4 = (t0 + 4304);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 32);
    xsi_set_current_line(92, ng0);
    t2 = ((char*)((ng5)));
    t4 = (t0 + 2544);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 1);
    xsi_set_current_line(93, ng0);
    t2 = ((char*)((ng6)));
    t4 = (t0 + 2704);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 1);
    xsi_set_current_line(94, ng0);
    t2 = ((char*)((ng5)));
    t4 = (t0 + 3184);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 1);
    xsi_set_current_line(95, ng0);
    t2 = ((char*)((ng5)));
    t4 = (t0 + 3024);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 16);
    xsi_set_current_line(96, ng0);
    t2 = ((char*)((ng5)));
    t4 = (t0 + 3504);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 16);
    xsi_set_current_line(97, ng0);
    t2 = ((char*)((ng5)));
    t4 = (t0 + 3664);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 1);
    xsi_set_current_line(98, ng0);
    t2 = ((char*)((ng5)));
    t4 = (t0 + 3824);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 1);
    xsi_set_current_line(99, ng0);
    t2 = ((char*)((ng5)));
    t4 = (t0 + 3984);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 16);
    xsi_set_current_line(100, ng0);
    t2 = ((char*)((ng7)));
    t4 = (t0 + 3344);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 8);
    xsi_set_current_line(101, ng0);
    t2 = (t0 + 5344);
    xsi_process_wait(t2, 40000LL);
    *((char **)t1) = &&LAB5;

LAB1:    return;
LAB5:    xsi_set_current_line(102, ng0);
    t2 = ((char*)((ng5)));
    t4 = (t0 + 2704);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 1);
    xsi_set_current_line(103, ng0);
    t2 = ((char*)((ng7)));
    t4 = (t0 + 3344);
    xsi_vlogvar_assign_value(t4, t2, 0, 0, 8);
    goto LAB1;

}

static void Always_113_1(char *t0)
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

LAB0:    t1 = (t0 + 5784U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(113, ng0);

LAB4:    xsi_set_current_line(114, ng0);
    t2 = (t0 + 5592);
    xsi_process_wait(t2, 20000LL);
    *((char **)t1) = &&LAB5;

LAB1:    return;
LAB5:    xsi_set_current_line(115, ng0);
    t4 = (t0 + 2544);
    t5 = (t4 + 56U);
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

LAB9:    t14 = (t0 + 2544);
    xsi_vlogvar_assign_value(t14, t3, 0, 0, 1);
    xsi_set_current_line(116, ng0);
    t2 = (t0 + 4304);
    t4 = (t2 + 56U);
    t5 = *((char **)t4);
    t6 = (t0 + 472);
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

LAB10:    xsi_set_current_line(116, ng0);

LAB13:    xsi_set_current_line(117, ng0);
    t13 = (t0 + 2704);
    t14 = (t13 + 56U);
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

LAB18:    xsi_set_current_line(117, ng0);

LAB21:    xsi_set_current_line(118, ng0);
    t39 = (t0 + 2544);
    t40 = (t39 + 56U);
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

LAB26:    xsi_set_current_line(118, ng0);

LAB29:    xsi_set_current_line(119, ng0);
    t65 = (t0 + 4624);
    t66 = (t65 + 56U);
    t67 = *((char **)t66);
    t69 = (t0 + 4624);
    t70 = (t69 + 72U);
    t71 = *((char **)t70);
    t72 = (t0 + 4624);
    t73 = (t72 + 64U);
    t74 = *((char **)t73);
    t75 = (t0 + 4304);
    t76 = (t75 + 56U);
    t77 = *((char **)t76);
    xsi_vlog_generic_get_array_select_value(t68, 1, t67, t71, t74, 2, 1, t77, 32, 1);
    t78 = (t0 + 3184);
    xsi_vlogvar_assign_value(t78, t68, 0, 0, 1);
    xsi_set_current_line(120, ng0);
    t2 = (t0 + 4464);
    t4 = (t2 + 56U);
    t5 = *((char **)t4);
    t6 = (t0 + 4464);
    t7 = (t6 + 72U);
    t13 = *((char **)t7);
    t14 = (t0 + 4464);
    t15 = (t14 + 64U);
    t16 = *((char **)t15);
    t18 = (t0 + 4304);
    t19 = (t18 + 56U);
    t32 = *((char **)t19);
    xsi_vlog_generic_get_array_select_value(t3, 16, t5, t13, t16, 2, 1, t32, 32, 1);
    t33 = (t0 + 3024);
    xsi_vlogvar_assign_value(t33, t3, 0, 0, 16);
    xsi_set_current_line(121, ng0);
    t2 = (t0 + 4304);
    t4 = (t2 + 56U);
    t5 = *((char **)t4);
    t6 = ((char*)((ng6)));
    memset(t3, 0, 8);
    xsi_vlog_signed_add(t3, 32, t5, 32, t6, 32);
    t7 = (t0 + 4304);
    xsi_vlogvar_assign_value(t7, t3, 0, 0, 32);
    xsi_set_current_line(122, ng0);
    t2 = (t0 + 1504U);
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

LAB34:    xsi_set_current_line(122, ng0);

LAB37:    xsi_set_current_line(123, ng0);
    t14 = (t0 + 3024);
    t15 = (t14 + 56U);
    t16 = *((char **)t15);
    xsi_vlogfile_write(1, 0, 0, ng8, 2, t0, (char)118, t16, 16);
    xsi_set_current_line(124, ng0);
    t2 = (t0 + 4144);
    t4 = (t2 + 56U);
    t5 = *((char **)t4);
    t6 = (t0 + 1344U);
    t7 = *((char **)t6);
    xsi_vlogfile_fwrite(*((unsigned int *)t5), 0, 0, 1, ng9, 2, t0, (char)118, t7, 16);
    goto LAB36;

}


extern void work_m_01507084155968014864_1895161487_init()
{
	static char *pe[] = {(void *)Initial_82_0,(void *)Always_113_1};
	xsi_register_didat("work_m_01507084155968014864_1895161487", "isim/custom_architecture_tb_isim_beh.exe.sim/work/m_01507084155968014864_1895161487.didat");
	xsi_register_executes(pe);
}
