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
static const char *ng0 = "/home/ise/ise_projects/RHS2000InterfaceXEM6010 release 180814_CustArch_v2_tb/OutputStateMachine.v";
static unsigned int ng1[] = {1U, 0U};
static unsigned int ng2[] = {0U, 0U};
static unsigned int ng3[] = {255U, 0U};
static unsigned int ng4[] = {2U, 0U};
static unsigned int ng5[] = {3U, 0U};
static unsigned int ng6[] = {4U, 0U};
static unsigned int ng7[] = {5U, 0U};
static unsigned int ng8[] = {6U, 0U};
static unsigned int ng9[] = {7U, 0U};
static unsigned int ng10[] = {8U, 0U};
static unsigned int ng11[] = {9U, 0U};
static unsigned int ng12[] = {10U, 0U};
static unsigned int ng13[] = {11U, 0U};
static unsigned int ng14[] = {12U, 0U};
static unsigned int ng15[] = {13U, 0U};
static unsigned int ng16[] = {14U, 0U};
static unsigned int ng17[] = {15U, 0U};
static unsigned int ng18[] = {160U, 0U};
static unsigned int ng19[] = {16U, 0U};



static void Always_80_0(char *t0)
{
    char t8[8];
    char t33[8];
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
    int t32;
    unsigned int t34;
    unsigned int t35;
    unsigned int t36;
    unsigned int t37;
    unsigned int t38;
    unsigned int t39;
    char *t40;

LAB0:    t1 = (t0 + 10408U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(80, ng0);
    t2 = (t0 + 11224);
    *((int *)t2) = 1;
    t3 = (t0 + 10440);
    *((char **)t3) = t2;
    *((char **)t1) = &&LAB4;

LAB1:    return;
LAB4:    xsi_set_current_line(81, ng0);

LAB5:    t4 = (t0 + 280);
    xsi_vlog_namedbase_setdisablestate(t4, &&LAB6);
    t5 = (t0 + 10216);
    xsi_vlog_namedbase_pushprocess(t4, t5);

LAB7:    xsi_set_current_line(82, ng0);
    t6 = (t0 + 3816U);
    t7 = *((char **)t6);
    t6 = ((char*)((ng1)));
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

LAB13:    xsi_set_current_line(92, ng0);

LAB16:    xsi_set_current_line(93, ng0);
    t2 = (t0 + 3976U);
    t3 = *((char **)t2);
    t2 = (t3 + 4);
    t11 = *((unsigned int *)t2);
    t12 = (~(t11));
    t13 = *((unsigned int *)t3);
    t14 = (t13 & t12);
    t15 = (t14 != 0);
    if (t15 > 0)
        goto LAB17;

LAB18:
LAB19:
LAB14:    t2 = (t0 + 280);
    xsi_vlog_namedbase_popprocess(t2);

LAB6:    t3 = (t0 + 10216);
    xsi_vlog_dispose_process_subprogram_invocation(t3);
    goto LAB2;

LAB10:    t23 = (t8 + 4);
    *((unsigned int *)t8) = 1;
    *((unsigned int *)t23) = 1;
    goto LAB11;

LAB12:    xsi_set_current_line(82, ng0);

LAB15:    xsi_set_current_line(83, ng0);
    t30 = ((char*)((ng2)));
    t31 = (t0 + 5016);
    xsi_vlogvar_wait_assign_value(t31, t30, 0, 0, 8, 0LL);
    xsi_set_current_line(84, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 5176);
    xsi_vlogvar_wait_assign_value(t3, t2, 0, 0, 8, 0LL);
    xsi_set_current_line(85, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 5336);
    xsi_vlogvar_wait_assign_value(t3, t2, 0, 0, 8, 0LL);
    xsi_set_current_line(86, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 5496);
    xsi_vlogvar_wait_assign_value(t3, t2, 0, 0, 8, 0LL);
    xsi_set_current_line(87, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 5656);
    xsi_vlogvar_wait_assign_value(t3, t2, 0, 0, 8, 0LL);
    xsi_set_current_line(88, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 5816);
    xsi_vlogvar_wait_assign_value(t3, t2, 0, 0, 8, 0LL);
    xsi_set_current_line(89, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 5976);
    xsi_vlogvar_wait_assign_value(t3, t2, 0, 0, 8, 0LL);
    xsi_set_current_line(90, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 4696);
    xsi_vlogvar_wait_assign_value(t3, t2, 0, 0, 5, 0LL);
    goto LAB14;

LAB17:    xsi_set_current_line(93, ng0);

LAB20:    xsi_set_current_line(94, ng0);
    t4 = ((char*)((ng2)));
    t5 = (t0 + 7416);
    xsi_vlogvar_assign_value(t5, t4, 0, 0, 9);
    xsi_set_current_line(95, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 7576);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 9);
    xsi_set_current_line(96, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 7736);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 9);
    xsi_set_current_line(97, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 7896);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 9);
    xsi_set_current_line(98, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 8056);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 9);
    xsi_set_current_line(99, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 8216);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 9);
    xsi_set_current_line(100, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 8376);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 9);
    xsi_set_current_line(101, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 8536);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 9);
    xsi_set_current_line(102, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 8696);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 9);
    xsi_set_current_line(103, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 8856);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 9);
    xsi_set_current_line(104, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 9016);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 9);
    xsi_set_current_line(105, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 9176);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 9);
    xsi_set_current_line(106, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 9336);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 9);
    xsi_set_current_line(107, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 9496);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 9);
    xsi_set_current_line(108, ng0);
    t2 = (t0 + 5016);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t0 + 6296);
    xsi_vlogvar_assign_value(t5, t4, 0, 0, 8);
    xsi_set_current_line(109, ng0);
    t2 = (t0 + 5176);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t0 + 6456);
    xsi_vlogvar_assign_value(t5, t4, 0, 0, 8);
    xsi_set_current_line(110, ng0);
    t2 = (t0 + 5656);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t0 + 7096);
    xsi_vlogvar_assign_value(t5, t4, 0, 0, 8);
    xsi_set_current_line(111, ng0);
    t2 = (t0 + 5976);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t0 + 6776);
    xsi_vlogvar_assign_value(t5, t4, 0, 0, 8);
    xsi_set_current_line(112, ng0);
    t2 = (t0 + 5816);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t0 + 6616);
    xsi_vlogvar_assign_value(t5, t4, 0, 0, 8);
    xsi_set_current_line(113, ng0);
    t2 = (t0 + 5496);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t0 + 7256);
    xsi_vlogvar_assign_value(t5, t4, 0, 0, 8);
    xsi_set_current_line(114, ng0);
    t2 = (t0 + 5336);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t0 + 6936);
    xsi_vlogvar_assign_value(t5, t4, 0, 0, 8);
    xsi_set_current_line(115, ng0);
    t2 = (t0 + 4696);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t0 + 6136);
    xsi_vlogvar_assign_value(t5, t4, 0, 0, 5);
    xsi_set_current_line(116, ng0);
    t2 = (t0 + 4696);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);

LAB21:    t5 = ((char*)((ng2)));
    t32 = xsi_vlog_unsigned_case_compare(t4, 5, t5, 5);
    if (t32 == 1)
        goto LAB22;

LAB23:    t2 = ((char*)((ng1)));
    t32 = xsi_vlog_unsigned_case_compare(t4, 5, t2, 5);
    if (t32 == 1)
        goto LAB24;

LAB25:    t2 = ((char*)((ng4)));
    t32 = xsi_vlog_unsigned_case_compare(t4, 5, t2, 5);
    if (t32 == 1)
        goto LAB26;

LAB27:    t2 = ((char*)((ng5)));
    t32 = xsi_vlog_unsigned_case_compare(t4, 5, t2, 5);
    if (t32 == 1)
        goto LAB28;

LAB29:    t2 = ((char*)((ng6)));
    t32 = xsi_vlog_unsigned_case_compare(t4, 5, t2, 5);
    if (t32 == 1)
        goto LAB30;

LAB31:    t2 = ((char*)((ng7)));
    t32 = xsi_vlog_unsigned_case_compare(t4, 5, t2, 5);
    if (t32 == 1)
        goto LAB32;

LAB33:    t2 = ((char*)((ng8)));
    t32 = xsi_vlog_unsigned_case_compare(t4, 5, t2, 5);
    if (t32 == 1)
        goto LAB34;

LAB35:    t2 = ((char*)((ng9)));
    t32 = xsi_vlog_unsigned_case_compare(t4, 5, t2, 5);
    if (t32 == 1)
        goto LAB36;

LAB37:    t2 = ((char*)((ng10)));
    t32 = xsi_vlog_unsigned_case_compare(t4, 5, t2, 5);
    if (t32 == 1)
        goto LAB38;

LAB39:    t2 = ((char*)((ng11)));
    t32 = xsi_vlog_unsigned_case_compare(t4, 5, t2, 5);
    if (t32 == 1)
        goto LAB40;

LAB41:    t2 = ((char*)((ng12)));
    t32 = xsi_vlog_unsigned_case_compare(t4, 5, t2, 5);
    if (t32 == 1)
        goto LAB42;

LAB43:    t2 = ((char*)((ng13)));
    t32 = xsi_vlog_unsigned_case_compare(t4, 5, t2, 5);
    if (t32 == 1)
        goto LAB44;

LAB45:    t2 = ((char*)((ng14)));
    t32 = xsi_vlog_unsigned_case_compare(t4, 5, t2, 5);
    if (t32 == 1)
        goto LAB46;

LAB47:    t2 = ((char*)((ng15)));
    t32 = xsi_vlog_unsigned_case_compare(t4, 5, t2, 5);
    if (t32 == 1)
        goto LAB48;

LAB49:    t2 = ((char*)((ng16)));
    t32 = xsi_vlog_unsigned_case_compare(t4, 5, t2, 5);
    if (t32 == 1)
        goto LAB50;

LAB51:    t2 = ((char*)((ng17)));
    t32 = xsi_vlog_unsigned_case_compare(t4, 5, t2, 5);
    if (t32 == 1)
        goto LAB52;

LAB53:
LAB55:
LAB54:    xsi_set_current_line(229, ng0);

LAB136:
LAB56:    xsi_set_current_line(233, ng0);
    t2 = (t0 + 4696);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);

LAB137:    t6 = ((char*)((ng2)));
    t32 = xsi_vlog_unsigned_case_compare(t5, 5, t6, 5);
    if (t32 == 1)
        goto LAB138;

LAB139:    t2 = ((char*)((ng1)));
    t32 = xsi_vlog_unsigned_case_compare(t5, 5, t2, 5);
    if (t32 == 1)
        goto LAB140;

LAB141:    t2 = ((char*)((ng4)));
    t32 = xsi_vlog_unsigned_case_compare(t5, 5, t2, 5);
    if (t32 == 1)
        goto LAB142;

LAB143:    t2 = ((char*)((ng5)));
    t32 = xsi_vlog_unsigned_case_compare(t5, 5, t2, 5);
    if (t32 == 1)
        goto LAB144;

LAB145:    t2 = ((char*)((ng6)));
    t32 = xsi_vlog_unsigned_case_compare(t5, 5, t2, 5);
    if (t32 == 1)
        goto LAB146;

LAB147:    t2 = ((char*)((ng7)));
    t32 = xsi_vlog_unsigned_case_compare(t5, 5, t2, 5);
    if (t32 == 1)
        goto LAB148;

LAB149:    t2 = ((char*)((ng8)));
    t32 = xsi_vlog_unsigned_case_compare(t5, 5, t2, 5);
    if (t32 == 1)
        goto LAB150;

LAB151:    t2 = ((char*)((ng9)));
    t32 = xsi_vlog_unsigned_case_compare(t5, 5, t2, 5);
    if (t32 == 1)
        goto LAB152;

LAB153:    t2 = ((char*)((ng10)));
    t32 = xsi_vlog_unsigned_case_compare(t5, 5, t2, 5);
    if (t32 == 1)
        goto LAB154;

LAB155:    t2 = ((char*)((ng11)));
    t32 = xsi_vlog_unsigned_case_compare(t5, 5, t2, 5);
    if (t32 == 1)
        goto LAB156;

LAB157:    t2 = ((char*)((ng12)));
    t32 = xsi_vlog_unsigned_case_compare(t5, 5, t2, 5);
    if (t32 == 1)
        goto LAB158;

LAB159:    t2 = ((char*)((ng13)));
    t32 = xsi_vlog_unsigned_case_compare(t5, 5, t2, 5);
    if (t32 == 1)
        goto LAB160;

LAB161:    t2 = ((char*)((ng14)));
    t32 = xsi_vlog_unsigned_case_compare(t5, 5, t2, 5);
    if (t32 == 1)
        goto LAB162;

LAB163:    t2 = ((char*)((ng15)));
    t32 = xsi_vlog_unsigned_case_compare(t5, 5, t2, 5);
    if (t32 == 1)
        goto LAB164;

LAB165:    t2 = ((char*)((ng16)));
    t32 = xsi_vlog_unsigned_case_compare(t5, 5, t2, 5);
    if (t32 == 1)
        goto LAB166;

LAB167:    t2 = ((char*)((ng17)));
    t32 = xsi_vlog_unsigned_case_compare(t5, 5, t2, 5);
    if (t32 == 1)
        goto LAB168;

LAB169:
LAB171:
LAB170:    xsi_set_current_line(324, ng0);

LAB264:    xsi_set_current_line(326, ng0);
    t2 = ((char*)((ng5)));
    t3 = (t0 + 6136);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 5);

LAB172:    xsi_set_current_line(329, ng0);
    t2 = (t0 + 6296);
    t3 = (t2 + 56U);
    t6 = *((char **)t3);
    t7 = (t0 + 5016);
    xsi_vlogvar_wait_assign_value(t7, t6, 0, 0, 8, 0LL);
    xsi_set_current_line(330, ng0);
    t2 = (t0 + 6456);
    t3 = (t2 + 56U);
    t6 = *((char **)t3);
    t7 = (t0 + 5176);
    xsi_vlogvar_wait_assign_value(t7, t6, 0, 0, 8, 0LL);
    xsi_set_current_line(331, ng0);
    t2 = (t0 + 7096);
    t3 = (t2 + 56U);
    t6 = *((char **)t3);
    t7 = (t0 + 5656);
    xsi_vlogvar_wait_assign_value(t7, t6, 0, 0, 8, 0LL);
    xsi_set_current_line(332, ng0);
    t2 = (t0 + 6776);
    t3 = (t2 + 56U);
    t6 = *((char **)t3);
    t7 = (t0 + 5976);
    xsi_vlogvar_wait_assign_value(t7, t6, 0, 0, 8, 0LL);
    xsi_set_current_line(333, ng0);
    t2 = (t0 + 6616);
    t3 = (t2 + 56U);
    t6 = *((char **)t3);
    t7 = (t0 + 5816);
    xsi_vlogvar_wait_assign_value(t7, t6, 0, 0, 8, 0LL);
    xsi_set_current_line(334, ng0);
    t2 = (t0 + 7256);
    t3 = (t2 + 56U);
    t6 = *((char **)t3);
    t7 = (t0 + 5496);
    xsi_vlogvar_wait_assign_value(t7, t6, 0, 0, 8, 0LL);
    xsi_set_current_line(335, ng0);
    t2 = (t0 + 6936);
    t3 = (t2 + 56U);
    t6 = *((char **)t3);
    t7 = (t0 + 5336);
    xsi_vlogvar_wait_assign_value(t7, t6, 0, 0, 8, 0LL);
    xsi_set_current_line(336, ng0);
    t2 = (t0 + 6136);
    t3 = (t2 + 56U);
    t6 = *((char **)t3);
    t7 = (t0 + 4696);
    xsi_vlogvar_wait_assign_value(t7, t6, 0, 0, 5, 0LL);
    goto LAB19;

LAB22:    xsi_set_current_line(118, ng0);

LAB57:    goto LAB56;

LAB24:    xsi_set_current_line(121, ng0);

LAB58:    xsi_set_current_line(122, ng0);
    t3 = ((char*)((ng2)));
    t5 = (t0 + 6296);
    xsi_vlogvar_assign_value(t5, t3, 0, 0, 8);
    xsi_set_current_line(123, ng0);
    t2 = (t0 + 5176);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    t6 = ((char*)((ng2)));
    xsi_vlogtype_concat(t8, 9, 9, 2U, t6, 1, t5, 8);
    t7 = (t0 + 8696);
    xsi_vlogvar_assign_value(t7, t8, 0, 0, 9);
    xsi_set_current_line(124, ng0);
    t2 = (t0 + 8696);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    t6 = ((char*)((ng1)));
    memset(t8, 0, 8);
    xsi_vlog_unsigned_add(t8, 9, t5, 9, t6, 9);
    t7 = (t0 + 7576);
    xsi_vlogvar_assign_value(t7, t8, 0, 0, 9);
    xsi_set_current_line(125, ng0);
    t2 = (t0 + 7576);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    memset(t8, 0, 8);
    t6 = (t8 + 4);
    t7 = (t5 + 4);
    t11 = *((unsigned int *)t5);
    t12 = (t11 >> 8);
    t13 = (t12 & 1);
    *((unsigned int *)t8) = t13;
    t14 = *((unsigned int *)t7);
    t15 = (t14 >> 8);
    t16 = (t15 & 1);
    *((unsigned int *)t6) = t16;
    t9 = ((char*)((ng2)));
    memset(t33, 0, 8);
    t10 = (t8 + 4);
    t23 = (t9 + 4);
    t17 = *((unsigned int *)t8);
    t18 = *((unsigned int *)t9);
    t19 = (t17 ^ t18);
    t20 = *((unsigned int *)t10);
    t21 = *((unsigned int *)t23);
    t22 = (t20 ^ t21);
    t25 = (t19 | t22);
    t26 = *((unsigned int *)t10);
    t27 = *((unsigned int *)t23);
    t28 = (t26 | t27);
    t29 = (~(t28));
    t34 = (t25 & t29);
    if (t34 != 0)
        goto LAB60;

LAB59:    if (t28 != 0)
        goto LAB61;

LAB62:    t30 = (t33 + 4);
    t35 = *((unsigned int *)t30);
    t36 = (~(t35));
    t37 = *((unsigned int *)t33);
    t38 = (t37 & t36);
    t39 = (t38 != 0);
    if (t39 > 0)
        goto LAB63;

LAB64:    xsi_set_current_line(128, ng0);

LAB67:    xsi_set_current_line(129, ng0);
    t2 = (t0 + 7576);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    memset(t8, 0, 8);
    t6 = (t8 + 4);
    t7 = (t5 + 4);
    t11 = *((unsigned int *)t5);
    t12 = (t11 >> 0);
    *((unsigned int *)t8) = t12;
    t13 = *((unsigned int *)t7);
    t14 = (t13 >> 0);
    *((unsigned int *)t6) = t14;
    t15 = *((unsigned int *)t8);
    *((unsigned int *)t8) = (t15 & 255U);
    t16 = *((unsigned int *)t6);
    *((unsigned int *)t6) = (t16 & 255U);
    t9 = (t0 + 6456);
    xsi_vlogvar_assign_value(t9, t8, 0, 0, 8);

LAB65:    goto LAB56;

LAB26:    xsi_set_current_line(133, ng0);

LAB68:    xsi_set_current_line(134, ng0);
    t3 = ((char*)((ng2)));
    t5 = (t0 + 6616);
    xsi_vlogvar_assign_value(t5, t3, 0, 0, 8);
    xsi_set_current_line(135, ng0);
    t2 = (t0 + 5016);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    t6 = ((char*)((ng2)));
    xsi_vlogtype_concat(t8, 9, 9, 2U, t6, 1, t5, 8);
    t7 = (t0 + 8856);
    xsi_vlogvar_assign_value(t7, t8, 0, 0, 9);
    xsi_set_current_line(136, ng0);
    t2 = (t0 + 8856);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    t6 = ((char*)((ng1)));
    memset(t8, 0, 8);
    xsi_vlog_unsigned_add(t8, 9, t5, 9, t6, 9);
    t7 = (t0 + 7736);
    xsi_vlogvar_assign_value(t7, t8, 0, 0, 9);
    xsi_set_current_line(137, ng0);
    t2 = (t0 + 7736);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    memset(t8, 0, 8);
    t6 = (t8 + 4);
    t7 = (t5 + 4);
    t11 = *((unsigned int *)t5);
    t12 = (t11 >> 8);
    t13 = (t12 & 1);
    *((unsigned int *)t8) = t13;
    t14 = *((unsigned int *)t7);
    t15 = (t14 >> 8);
    t16 = (t15 & 1);
    *((unsigned int *)t6) = t16;
    t9 = ((char*)((ng2)));
    memset(t33, 0, 8);
    t10 = (t8 + 4);
    t23 = (t9 + 4);
    t17 = *((unsigned int *)t8);
    t18 = *((unsigned int *)t9);
    t19 = (t17 ^ t18);
    t20 = *((unsigned int *)t10);
    t21 = *((unsigned int *)t23);
    t22 = (t20 ^ t21);
    t25 = (t19 | t22);
    t26 = *((unsigned int *)t10);
    t27 = *((unsigned int *)t23);
    t28 = (t26 | t27);
    t29 = (~(t28));
    t34 = (t25 & t29);
    if (t34 != 0)
        goto LAB70;

LAB69:    if (t28 != 0)
        goto LAB71;

LAB72:    t30 = (t33 + 4);
    t35 = *((unsigned int *)t30);
    t36 = (~(t35));
    t37 = *((unsigned int *)t33);
    t38 = (t37 & t36);
    t39 = (t38 != 0);
    if (t39 > 0)
        goto LAB73;

LAB74:    xsi_set_current_line(140, ng0);

LAB77:    xsi_set_current_line(141, ng0);
    t2 = (t0 + 7736);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    memset(t8, 0, 8);
    t6 = (t8 + 4);
    t7 = (t5 + 4);
    t11 = *((unsigned int *)t5);
    t12 = (t11 >> 0);
    *((unsigned int *)t8) = t12;
    t13 = *((unsigned int *)t7);
    t14 = (t13 >> 0);
    *((unsigned int *)t6) = t14;
    t15 = *((unsigned int *)t8);
    *((unsigned int *)t8) = (t15 & 255U);
    t16 = *((unsigned int *)t6);
    *((unsigned int *)t6) = (t16 & 255U);
    t9 = (t0 + 6296);
    xsi_vlogvar_assign_value(t9, t8, 0, 0, 8);

LAB75:    goto LAB56;

LAB28:    xsi_set_current_line(145, ng0);

LAB78:    xsi_set_current_line(146, ng0);
    t3 = (t0 + 5976);
    t5 = (t3 + 56U);
    t6 = *((char **)t5);
    t7 = ((char*)((ng2)));
    xsi_vlogtype_concat(t8, 9, 9, 2U, t7, 1, t6, 8);
    t9 = (t0 + 8536);
    xsi_vlogvar_assign_value(t9, t8, 0, 0, 9);
    xsi_set_current_line(147, ng0);
    t2 = (t0 + 8536);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    t6 = ((char*)((ng1)));
    memset(t8, 0, 8);
    xsi_vlog_unsigned_add(t8, 9, t5, 9, t6, 9);
    t7 = (t0 + 7416);
    xsi_vlogvar_assign_value(t7, t8, 0, 0, 9);
    xsi_set_current_line(148, ng0);
    t2 = (t0 + 7416);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    memset(t8, 0, 8);
    t6 = (t8 + 4);
    t7 = (t5 + 4);
    t11 = *((unsigned int *)t5);
    t12 = (t11 >> 8);
    t13 = (t12 & 1);
    *((unsigned int *)t8) = t13;
    t14 = *((unsigned int *)t7);
    t15 = (t14 >> 8);
    t16 = (t15 & 1);
    *((unsigned int *)t6) = t16;
    t9 = ((char*)((ng2)));
    memset(t33, 0, 8);
    t10 = (t8 + 4);
    t23 = (t9 + 4);
    t17 = *((unsigned int *)t8);
    t18 = *((unsigned int *)t9);
    t19 = (t17 ^ t18);
    t20 = *((unsigned int *)t10);
    t21 = *((unsigned int *)t23);
    t22 = (t20 ^ t21);
    t25 = (t19 | t22);
    t26 = *((unsigned int *)t10);
    t27 = *((unsigned int *)t23);
    t28 = (t26 | t27);
    t29 = (~(t28));
    t34 = (t25 & t29);
    if (t34 != 0)
        goto LAB80;

LAB79:    if (t28 != 0)
        goto LAB81;

LAB82:    t30 = (t33 + 4);
    t35 = *((unsigned int *)t30);
    t36 = (~(t35));
    t37 = *((unsigned int *)t33);
    t38 = (t37 & t36);
    t39 = (t38 != 0);
    if (t39 > 0)
        goto LAB83;

LAB84:    xsi_set_current_line(151, ng0);

LAB87:    xsi_set_current_line(152, ng0);
    t2 = (t0 + 7416);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    memset(t8, 0, 8);
    t6 = (t8 + 4);
    t7 = (t5 + 4);
    t11 = *((unsigned int *)t5);
    t12 = (t11 >> 0);
    *((unsigned int *)t8) = t12;
    t13 = *((unsigned int *)t7);
    t14 = (t13 >> 0);
    *((unsigned int *)t6) = t14;
    t15 = *((unsigned int *)t8);
    *((unsigned int *)t8) = (t15 & 255U);
    t16 = *((unsigned int *)t6);
    *((unsigned int *)t6) = (t16 & 255U);
    t9 = (t0 + 6776);
    xsi_vlogvar_assign_value(t9, t8, 0, 0, 8);

LAB85:    goto LAB56;

LAB30:    xsi_set_current_line(156, ng0);

LAB88:    xsi_set_current_line(157, ng0);
    t3 = ((char*)((ng2)));
    t5 = (t0 + 6456);
    xsi_vlogvar_assign_value(t5, t3, 0, 0, 8);
    goto LAB56;

LAB32:    xsi_set_current_line(160, ng0);

LAB89:    goto LAB56;

LAB34:    xsi_set_current_line(163, ng0);

LAB90:    xsi_set_current_line(164, ng0);
    t3 = ((char*)((ng2)));
    t5 = (t0 + 6936);
    xsi_vlogvar_assign_value(t5, t3, 0, 0, 8);
    xsi_set_current_line(165, ng0);
    t2 = (t0 + 5656);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    t6 = ((char*)((ng2)));
    xsi_vlogtype_concat(t8, 9, 9, 2U, t6, 1, t5, 8);
    t7 = (t0 + 9016);
    xsi_vlogvar_assign_value(t7, t8, 0, 0, 9);
    xsi_set_current_line(166, ng0);
    t2 = (t0 + 9016);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    t6 = ((char*)((ng1)));
    memset(t8, 0, 8);
    xsi_vlog_unsigned_add(t8, 9, t5, 9, t6, 9);
    t7 = (t0 + 7896);
    xsi_vlogvar_assign_value(t7, t8, 0, 0, 9);
    xsi_set_current_line(167, ng0);
    t2 = (t0 + 7896);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    memset(t8, 0, 8);
    t6 = (t8 + 4);
    t7 = (t5 + 4);
    t11 = *((unsigned int *)t5);
    t12 = (t11 >> 8);
    t13 = (t12 & 1);
    *((unsigned int *)t8) = t13;
    t14 = *((unsigned int *)t7);
    t15 = (t14 >> 8);
    t16 = (t15 & 1);
    *((unsigned int *)t6) = t16;
    t9 = ((char*)((ng2)));
    memset(t33, 0, 8);
    t10 = (t8 + 4);
    t23 = (t9 + 4);
    t17 = *((unsigned int *)t8);
    t18 = *((unsigned int *)t9);
    t19 = (t17 ^ t18);
    t20 = *((unsigned int *)t10);
    t21 = *((unsigned int *)t23);
    t22 = (t20 ^ t21);
    t25 = (t19 | t22);
    t26 = *((unsigned int *)t10);
    t27 = *((unsigned int *)t23);
    t28 = (t26 | t27);
    t29 = (~(t28));
    t34 = (t25 & t29);
    if (t34 != 0)
        goto LAB92;

LAB91:    if (t28 != 0)
        goto LAB93;

LAB94:    t30 = (t33 + 4);
    t35 = *((unsigned int *)t30);
    t36 = (~(t35));
    t37 = *((unsigned int *)t33);
    t38 = (t37 & t36);
    t39 = (t38 != 0);
    if (t39 > 0)
        goto LAB95;

LAB96:    xsi_set_current_line(170, ng0);

LAB99:    xsi_set_current_line(171, ng0);
    t2 = (t0 + 7896);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    memset(t8, 0, 8);
    t6 = (t8 + 4);
    t7 = (t5 + 4);
    t11 = *((unsigned int *)t5);
    t12 = (t11 >> 0);
    *((unsigned int *)t8) = t12;
    t13 = *((unsigned int *)t7);
    t14 = (t13 >> 0);
    *((unsigned int *)t6) = t14;
    t15 = *((unsigned int *)t8);
    *((unsigned int *)t8) = (t15 & 255U);
    t16 = *((unsigned int *)t6);
    *((unsigned int *)t6) = (t16 & 255U);
    t9 = (t0 + 7096);
    xsi_vlogvar_assign_value(t9, t8, 0, 0, 8);

LAB97:    goto LAB56;

LAB36:    xsi_set_current_line(175, ng0);

LAB100:    goto LAB56;

LAB38:    xsi_set_current_line(178, ng0);

LAB101:    xsi_set_current_line(179, ng0);
    t3 = ((char*)((ng2)));
    t5 = (t0 + 7096);
    xsi_vlogvar_assign_value(t5, t3, 0, 0, 8);
    xsi_set_current_line(180, ng0);
    t2 = (t0 + 5816);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    t6 = ((char*)((ng2)));
    xsi_vlogtype_concat(t8, 9, 9, 2U, t6, 1, t5, 8);
    t7 = (t0 + 9176);
    xsi_vlogvar_assign_value(t7, t8, 0, 0, 9);
    xsi_set_current_line(181, ng0);
    t2 = (t0 + 9176);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    t6 = ((char*)((ng1)));
    memset(t8, 0, 8);
    xsi_vlog_unsigned_add(t8, 9, t5, 9, t6, 9);
    t7 = (t0 + 8056);
    xsi_vlogvar_assign_value(t7, t8, 0, 0, 9);
    xsi_set_current_line(182, ng0);
    t2 = (t0 + 8056);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    memset(t8, 0, 8);
    t6 = (t8 + 4);
    t7 = (t5 + 4);
    t11 = *((unsigned int *)t5);
    t12 = (t11 >> 8);
    t13 = (t12 & 1);
    *((unsigned int *)t8) = t13;
    t14 = *((unsigned int *)t7);
    t15 = (t14 >> 8);
    t16 = (t15 & 1);
    *((unsigned int *)t6) = t16;
    t9 = ((char*)((ng2)));
    memset(t33, 0, 8);
    t10 = (t8 + 4);
    t23 = (t9 + 4);
    t17 = *((unsigned int *)t8);
    t18 = *((unsigned int *)t9);
    t19 = (t17 ^ t18);
    t20 = *((unsigned int *)t10);
    t21 = *((unsigned int *)t23);
    t22 = (t20 ^ t21);
    t25 = (t19 | t22);
    t26 = *((unsigned int *)t10);
    t27 = *((unsigned int *)t23);
    t28 = (t26 | t27);
    t29 = (~(t28));
    t34 = (t25 & t29);
    if (t34 != 0)
        goto LAB103;

LAB102:    if (t28 != 0)
        goto LAB104;

LAB105:    t30 = (t33 + 4);
    t35 = *((unsigned int *)t30);
    t36 = (~(t35));
    t37 = *((unsigned int *)t33);
    t38 = (t37 & t36);
    t39 = (t38 != 0);
    if (t39 > 0)
        goto LAB106;

LAB107:    xsi_set_current_line(185, ng0);

LAB110:    xsi_set_current_line(186, ng0);
    t2 = (t0 + 8056);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    memset(t8, 0, 8);
    t6 = (t8 + 4);
    t7 = (t5 + 4);
    t11 = *((unsigned int *)t5);
    t12 = (t11 >> 0);
    *((unsigned int *)t8) = t12;
    t13 = *((unsigned int *)t7);
    t14 = (t13 >> 0);
    *((unsigned int *)t6) = t14;
    t15 = *((unsigned int *)t8);
    *((unsigned int *)t8) = (t15 & 255U);
    t16 = *((unsigned int *)t6);
    *((unsigned int *)t6) = (t16 & 255U);
    t9 = (t0 + 6616);
    xsi_vlogvar_assign_value(t9, t8, 0, 0, 8);

LAB108:    goto LAB56;

LAB40:    xsi_set_current_line(190, ng0);

LAB111:    goto LAB56;

LAB42:    xsi_set_current_line(193, ng0);

LAB112:    goto LAB56;

LAB44:    xsi_set_current_line(196, ng0);

LAB113:    goto LAB56;

LAB46:    xsi_set_current_line(199, ng0);

LAB114:    goto LAB56;

LAB48:    xsi_set_current_line(202, ng0);

LAB115:    xsi_set_current_line(203, ng0);
    t3 = ((char*)((ng2)));
    t5 = (t0 + 6776);
    xsi_vlogvar_assign_value(t5, t3, 0, 0, 8);
    xsi_set_current_line(204, ng0);
    t2 = (t0 + 5496);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    t6 = ((char*)((ng2)));
    xsi_vlogtype_concat(t8, 9, 9, 2U, t6, 1, t5, 8);
    t7 = (t0 + 9336);
    xsi_vlogvar_assign_value(t7, t8, 0, 0, 9);
    xsi_set_current_line(205, ng0);
    t2 = (t0 + 9336);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    t6 = ((char*)((ng1)));
    memset(t8, 0, 8);
    xsi_vlog_unsigned_add(t8, 9, t5, 9, t6, 9);
    t7 = (t0 + 8216);
    xsi_vlogvar_assign_value(t7, t8, 0, 0, 9);
    xsi_set_current_line(206, ng0);
    t2 = (t0 + 8216);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    memset(t8, 0, 8);
    t6 = (t8 + 4);
    t7 = (t5 + 4);
    t11 = *((unsigned int *)t5);
    t12 = (t11 >> 8);
    t13 = (t12 & 1);
    *((unsigned int *)t8) = t13;
    t14 = *((unsigned int *)t7);
    t15 = (t14 >> 8);
    t16 = (t15 & 1);
    *((unsigned int *)t6) = t16;
    t9 = ((char*)((ng2)));
    memset(t33, 0, 8);
    t10 = (t8 + 4);
    t23 = (t9 + 4);
    t17 = *((unsigned int *)t8);
    t18 = *((unsigned int *)t9);
    t19 = (t17 ^ t18);
    t20 = *((unsigned int *)t10);
    t21 = *((unsigned int *)t23);
    t22 = (t20 ^ t21);
    t25 = (t19 | t22);
    t26 = *((unsigned int *)t10);
    t27 = *((unsigned int *)t23);
    t28 = (t26 | t27);
    t29 = (~(t28));
    t34 = (t25 & t29);
    if (t34 != 0)
        goto LAB117;

LAB116:    if (t28 != 0)
        goto LAB118;

LAB119:    t30 = (t33 + 4);
    t35 = *((unsigned int *)t30);
    t36 = (~(t35));
    t37 = *((unsigned int *)t33);
    t38 = (t37 & t36);
    t39 = (t38 != 0);
    if (t39 > 0)
        goto LAB120;

LAB121:    xsi_set_current_line(209, ng0);

LAB124:    xsi_set_current_line(210, ng0);
    t2 = (t0 + 8216);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    memset(t8, 0, 8);
    t6 = (t8 + 4);
    t7 = (t5 + 4);
    t11 = *((unsigned int *)t5);
    t12 = (t11 >> 0);
    *((unsigned int *)t8) = t12;
    t13 = *((unsigned int *)t7);
    t14 = (t13 >> 0);
    *((unsigned int *)t6) = t14;
    t15 = *((unsigned int *)t8);
    *((unsigned int *)t8) = (t15 & 255U);
    t16 = *((unsigned int *)t6);
    *((unsigned int *)t6) = (t16 & 255U);
    t9 = (t0 + 7256);
    xsi_vlogvar_assign_value(t9, t8, 0, 0, 8);

LAB122:    goto LAB56;

LAB50:    xsi_set_current_line(214, ng0);

LAB125:    xsi_set_current_line(215, ng0);
    t3 = ((char*)((ng2)));
    t5 = (t0 + 7256);
    xsi_vlogvar_assign_value(t5, t3, 0, 0, 8);
    xsi_set_current_line(216, ng0);
    t2 = (t0 + 5336);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    t6 = ((char*)((ng2)));
    xsi_vlogtype_concat(t8, 9, 9, 2U, t6, 1, t5, 8);
    t7 = (t0 + 9496);
    xsi_vlogvar_assign_value(t7, t8, 0, 0, 9);
    xsi_set_current_line(217, ng0);
    t2 = (t0 + 9496);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    t6 = ((char*)((ng1)));
    memset(t8, 0, 8);
    xsi_vlog_unsigned_add(t8, 9, t5, 9, t6, 9);
    t7 = (t0 + 8376);
    xsi_vlogvar_assign_value(t7, t8, 0, 0, 9);
    xsi_set_current_line(218, ng0);
    t2 = (t0 + 8376);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    memset(t8, 0, 8);
    t6 = (t8 + 4);
    t7 = (t5 + 4);
    t11 = *((unsigned int *)t5);
    t12 = (t11 >> 8);
    t13 = (t12 & 1);
    *((unsigned int *)t8) = t13;
    t14 = *((unsigned int *)t7);
    t15 = (t14 >> 8);
    t16 = (t15 & 1);
    *((unsigned int *)t6) = t16;
    t9 = ((char*)((ng2)));
    memset(t33, 0, 8);
    t10 = (t8 + 4);
    t23 = (t9 + 4);
    t17 = *((unsigned int *)t8);
    t18 = *((unsigned int *)t9);
    t19 = (t17 ^ t18);
    t20 = *((unsigned int *)t10);
    t21 = *((unsigned int *)t23);
    t22 = (t20 ^ t21);
    t25 = (t19 | t22);
    t26 = *((unsigned int *)t10);
    t27 = *((unsigned int *)t23);
    t28 = (t26 | t27);
    t29 = (~(t28));
    t34 = (t25 & t29);
    if (t34 != 0)
        goto LAB127;

LAB126:    if (t28 != 0)
        goto LAB128;

LAB129:    t30 = (t33 + 4);
    t35 = *((unsigned int *)t30);
    t36 = (~(t35));
    t37 = *((unsigned int *)t33);
    t38 = (t37 & t36);
    t39 = (t38 != 0);
    if (t39 > 0)
        goto LAB130;

LAB131:    xsi_set_current_line(221, ng0);

LAB134:    xsi_set_current_line(222, ng0);
    t2 = (t0 + 8376);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    memset(t8, 0, 8);
    t6 = (t8 + 4);
    t7 = (t5 + 4);
    t11 = *((unsigned int *)t5);
    t12 = (t11 >> 0);
    *((unsigned int *)t8) = t12;
    t13 = *((unsigned int *)t7);
    t14 = (t13 >> 0);
    *((unsigned int *)t6) = t14;
    t15 = *((unsigned int *)t8);
    *((unsigned int *)t8) = (t15 & 255U);
    t16 = *((unsigned int *)t6);
    *((unsigned int *)t6) = (t16 & 255U);
    t9 = (t0 + 6936);
    xsi_vlogvar_assign_value(t9, t8, 0, 0, 8);

LAB132:    goto LAB56;

LAB52:    xsi_set_current_line(226, ng0);

LAB135:    goto LAB56;

LAB60:    *((unsigned int *)t33) = 1;
    goto LAB62;

LAB61:    t24 = (t33 + 4);
    *((unsigned int *)t33) = 1;
    *((unsigned int *)t24) = 1;
    goto LAB62;

LAB63:    xsi_set_current_line(125, ng0);

LAB66:    xsi_set_current_line(126, ng0);
    t31 = ((char*)((ng3)));
    t40 = (t0 + 6456);
    xsi_vlogvar_assign_value(t40, t31, 0, 0, 8);
    goto LAB65;

LAB70:    *((unsigned int *)t33) = 1;
    goto LAB72;

LAB71:    t24 = (t33 + 4);
    *((unsigned int *)t33) = 1;
    *((unsigned int *)t24) = 1;
    goto LAB72;

LAB73:    xsi_set_current_line(137, ng0);

LAB76:    xsi_set_current_line(138, ng0);
    t31 = ((char*)((ng3)));
    t40 = (t0 + 6296);
    xsi_vlogvar_assign_value(t40, t31, 0, 0, 8);
    goto LAB75;

LAB80:    *((unsigned int *)t33) = 1;
    goto LAB82;

LAB81:    t24 = (t33 + 4);
    *((unsigned int *)t33) = 1;
    *((unsigned int *)t24) = 1;
    goto LAB82;

LAB83:    xsi_set_current_line(148, ng0);

LAB86:    xsi_set_current_line(149, ng0);
    t31 = ((char*)((ng3)));
    t40 = (t0 + 6776);
    xsi_vlogvar_assign_value(t40, t31, 0, 0, 8);
    goto LAB85;

LAB92:    *((unsigned int *)t33) = 1;
    goto LAB94;

LAB93:    t24 = (t33 + 4);
    *((unsigned int *)t33) = 1;
    *((unsigned int *)t24) = 1;
    goto LAB94;

LAB95:    xsi_set_current_line(167, ng0);

LAB98:    xsi_set_current_line(168, ng0);
    t31 = ((char*)((ng3)));
    t40 = (t0 + 7096);
    xsi_vlogvar_assign_value(t40, t31, 0, 0, 8);
    goto LAB97;

LAB103:    *((unsigned int *)t33) = 1;
    goto LAB105;

LAB104:    t24 = (t33 + 4);
    *((unsigned int *)t33) = 1;
    *((unsigned int *)t24) = 1;
    goto LAB105;

LAB106:    xsi_set_current_line(182, ng0);

LAB109:    xsi_set_current_line(183, ng0);
    t31 = ((char*)((ng3)));
    t40 = (t0 + 6616);
    xsi_vlogvar_assign_value(t40, t31, 0, 0, 8);
    goto LAB108;

LAB117:    *((unsigned int *)t33) = 1;
    goto LAB119;

LAB118:    t24 = (t33 + 4);
    *((unsigned int *)t33) = 1;
    *((unsigned int *)t24) = 1;
    goto LAB119;

LAB120:    xsi_set_current_line(206, ng0);

LAB123:    xsi_set_current_line(207, ng0);
    t31 = ((char*)((ng3)));
    t40 = (t0 + 7256);
    xsi_vlogvar_assign_value(t40, t31, 0, 0, 8);
    goto LAB122;

LAB127:    *((unsigned int *)t33) = 1;
    goto LAB129;

LAB128:    t24 = (t33 + 4);
    *((unsigned int *)t33) = 1;
    *((unsigned int *)t24) = 1;
    goto LAB129;

LAB130:    xsi_set_current_line(218, ng0);

LAB133:    xsi_set_current_line(219, ng0);
    t31 = ((char*)((ng3)));
    t40 = (t0 + 6936);
    xsi_vlogvar_assign_value(t40, t31, 0, 0, 8);
    goto LAB132;

LAB138:    xsi_set_current_line(235, ng0);

LAB173:    xsi_set_current_line(236, ng0);
    t7 = (t0 + 4136U);
    t9 = *((char **)t7);
    t7 = ((char*)((ng10)));
    memset(t8, 0, 8);
    t10 = (t9 + 4);
    t23 = (t7 + 4);
    t11 = *((unsigned int *)t9);
    t12 = *((unsigned int *)t7);
    t13 = (t11 ^ t12);
    t14 = *((unsigned int *)t10);
    t15 = *((unsigned int *)t23);
    t16 = (t14 ^ t15);
    t17 = (t13 | t16);
    t18 = *((unsigned int *)t10);
    t19 = *((unsigned int *)t23);
    t20 = (t18 | t19);
    t21 = (~(t20));
    t22 = (t17 & t21);
    if (t22 != 0)
        goto LAB177;

LAB174:    if (t20 != 0)
        goto LAB176;

LAB175:    *((unsigned int *)t8) = 1;

LAB177:    t30 = (t8 + 4);
    t25 = *((unsigned int *)t30);
    t26 = (~(t25));
    t27 = *((unsigned int *)t8);
    t28 = (t27 & t26);
    t29 = (t28 != 0);
    if (t29 > 0)
        goto LAB178;

LAB179:
LAB180:    goto LAB172;

LAB140:    xsi_set_current_line(241, ng0);

LAB182:    xsi_set_current_line(242, ng0);
    t3 = (t0 + 6456);
    t6 = (t3 + 56U);
    t7 = *((char **)t6);
    t9 = ((char*)((ng10)));
    memset(t8, 0, 8);
    t10 = (t7 + 4);
    t23 = (t9 + 4);
    t11 = *((unsigned int *)t7);
    t12 = *((unsigned int *)t9);
    t13 = (t11 ^ t12);
    t14 = *((unsigned int *)t10);
    t15 = *((unsigned int *)t23);
    t16 = (t14 ^ t15);
    t17 = (t13 | t16);
    t18 = *((unsigned int *)t10);
    t19 = *((unsigned int *)t23);
    t20 = (t18 | t19);
    t21 = (~(t20));
    t22 = (t17 & t21);
    if (t22 != 0)
        goto LAB186;

LAB183:    if (t20 != 0)
        goto LAB185;

LAB184:    *((unsigned int *)t8) = 1;

LAB186:    t30 = (t8 + 4);
    t25 = *((unsigned int *)t30);
    t26 = (~(t25));
    t27 = *((unsigned int *)t8);
    t28 = (t27 & t26);
    t29 = (t28 != 0);
    if (t29 > 0)
        goto LAB187;

LAB188:    xsi_set_current_line(245, ng0);

LAB191:    xsi_set_current_line(246, ng0);
    t2 = ((char*)((ng1)));
    t3 = (t0 + 6136);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 5);

LAB189:    goto LAB172;

LAB142:    xsi_set_current_line(250, ng0);

LAB192:    xsi_set_current_line(251, ng0);
    t3 = (t0 + 6296);
    t6 = (t3 + 56U);
    t7 = *((char **)t6);
    t9 = ((char*)((ng10)));
    memset(t8, 0, 8);
    t10 = (t7 + 4);
    t23 = (t9 + 4);
    t11 = *((unsigned int *)t7);
    t12 = *((unsigned int *)t9);
    t13 = (t11 ^ t12);
    t14 = *((unsigned int *)t10);
    t15 = *((unsigned int *)t23);
    t16 = (t14 ^ t15);
    t17 = (t13 | t16);
    t18 = *((unsigned int *)t10);
    t19 = *((unsigned int *)t23);
    t20 = (t18 | t19);
    t21 = (~(t20));
    t22 = (t17 & t21);
    if (t22 != 0)
        goto LAB196;

LAB193:    if (t20 != 0)
        goto LAB195;

LAB194:    *((unsigned int *)t8) = 1;

LAB196:    t30 = (t8 + 4);
    t25 = *((unsigned int *)t30);
    t26 = (~(t25));
    t27 = *((unsigned int *)t8);
    t28 = (t27 & t26);
    t29 = (t28 != 0);
    if (t29 > 0)
        goto LAB197;

LAB198:    xsi_set_current_line(254, ng0);

LAB201:    xsi_set_current_line(255, ng0);
    t2 = ((char*)((ng4)));
    t3 = (t0 + 6136);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 5);

LAB199:    goto LAB172;

LAB144:    xsi_set_current_line(259, ng0);

LAB202:    xsi_set_current_line(260, ng0);
    t3 = ((char*)((ng9)));
    t6 = (t0 + 6136);
    xsi_vlogvar_assign_value(t6, t3, 0, 0, 5);
    goto LAB172;

LAB146:    xsi_set_current_line(263, ng0);

LAB203:    xsi_set_current_line(264, ng0);
    t3 = ((char*)((ng7)));
    t6 = (t0 + 6136);
    xsi_vlogvar_assign_value(t6, t3, 0, 0, 5);
    goto LAB172;

LAB148:    xsi_set_current_line(267, ng0);

LAB204:    xsi_set_current_line(268, ng0);
    t3 = ((char*)((ng2)));
    t6 = (t0 + 6136);
    xsi_vlogvar_assign_value(t6, t3, 0, 0, 5);
    goto LAB172;

LAB150:    xsi_set_current_line(271, ng0);

LAB205:    xsi_set_current_line(272, ng0);
    t3 = (t0 + 7096);
    t6 = (t3 + 56U);
    t7 = *((char **)t6);
    t9 = ((char*)((ng10)));
    memset(t8, 0, 8);
    t10 = (t7 + 4);
    t23 = (t9 + 4);
    t11 = *((unsigned int *)t7);
    t12 = *((unsigned int *)t9);
    t13 = (t11 ^ t12);
    t14 = *((unsigned int *)t10);
    t15 = *((unsigned int *)t23);
    t16 = (t14 ^ t15);
    t17 = (t13 | t16);
    t18 = *((unsigned int *)t10);
    t19 = *((unsigned int *)t23);
    t20 = (t18 | t19);
    t21 = (~(t20));
    t22 = (t17 & t21);
    if (t22 != 0)
        goto LAB209;

LAB206:    if (t20 != 0)
        goto LAB208;

LAB207:    *((unsigned int *)t8) = 1;

LAB209:    t30 = (t8 + 4);
    t25 = *((unsigned int *)t30);
    t26 = (~(t25));
    t27 = *((unsigned int *)t8);
    t28 = (t27 & t26);
    t29 = (t28 != 0);
    if (t29 > 0)
        goto LAB210;

LAB211:
LAB212:    goto LAB172;

LAB152:    xsi_set_current_line(277, ng0);

LAB214:    xsi_set_current_line(278, ng0);
    t3 = (t0 + 6776);
    t6 = (t3 + 56U);
    t7 = *((char **)t6);
    t9 = ((char*)((ng18)));
    memset(t8, 0, 8);
    t10 = (t7 + 4);
    if (*((unsigned int *)t10) != 0)
        goto LAB216;

LAB215:    t23 = (t9 + 4);
    if (*((unsigned int *)t23) != 0)
        goto LAB216;

LAB219:    if (*((unsigned int *)t7) < *((unsigned int *)t9))
        goto LAB217;

LAB218:    t30 = (t8 + 4);
    t11 = *((unsigned int *)t30);
    t12 = (~(t11));
    t13 = *((unsigned int *)t8);
    t14 = (t13 & t12);
    t15 = (t14 != 0);
    if (t15 > 0)
        goto LAB220;

LAB221:    xsi_set_current_line(281, ng0);
    t2 = (t0 + 6776);
    t3 = (t2 + 56U);
    t6 = *((char **)t3);
    t7 = ((char*)((ng18)));
    memset(t8, 0, 8);
    t9 = (t6 + 4);
    t10 = (t7 + 4);
    t11 = *((unsigned int *)t6);
    t12 = *((unsigned int *)t7);
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
        goto LAB227;

LAB224:    if (t20 != 0)
        goto LAB226;

LAB225:    *((unsigned int *)t8) = 1;

LAB227:    t24 = (t8 + 4);
    t25 = *((unsigned int *)t24);
    t26 = (~(t25));
    t27 = *((unsigned int *)t8);
    t28 = (t27 & t26);
    t29 = (t28 != 0);
    if (t29 > 0)
        goto LAB228;

LAB229:
LAB230:
LAB222:    goto LAB172;

LAB154:    xsi_set_current_line(286, ng0);

LAB232:    xsi_set_current_line(287, ng0);
    t3 = (t0 + 6616);
    t6 = (t3 + 56U);
    t7 = *((char **)t6);
    t9 = ((char*)((ng10)));
    memset(t8, 0, 8);
    t10 = (t7 + 4);
    t23 = (t9 + 4);
    t11 = *((unsigned int *)t7);
    t12 = *((unsigned int *)t9);
    t13 = (t11 ^ t12);
    t14 = *((unsigned int *)t10);
    t15 = *((unsigned int *)t23);
    t16 = (t14 ^ t15);
    t17 = (t13 | t16);
    t18 = *((unsigned int *)t10);
    t19 = *((unsigned int *)t23);
    t20 = (t18 | t19);
    t21 = (~(t20));
    t22 = (t17 & t21);
    if (t22 != 0)
        goto LAB236;

LAB233:    if (t20 != 0)
        goto LAB235;

LAB234:    *((unsigned int *)t8) = 1;

LAB236:    t30 = (t8 + 4);
    t25 = *((unsigned int *)t30);
    t26 = (~(t25));
    t27 = *((unsigned int *)t8);
    t28 = (t27 & t26);
    t29 = (t28 != 0);
    if (t29 > 0)
        goto LAB237;

LAB238:
LAB239:    goto LAB172;

LAB156:    xsi_set_current_line(292, ng0);

LAB241:    xsi_set_current_line(293, ng0);
    t3 = ((char*)((ng12)));
    t6 = (t0 + 6136);
    xsi_vlogvar_assign_value(t6, t3, 0, 0, 5);
    goto LAB172;

LAB158:    xsi_set_current_line(296, ng0);

LAB242:    xsi_set_current_line(297, ng0);
    t3 = ((char*)((ng13)));
    t6 = (t0 + 6136);
    xsi_vlogvar_assign_value(t6, t3, 0, 0, 5);
    goto LAB172;

LAB160:    xsi_set_current_line(300, ng0);

LAB243:    xsi_set_current_line(301, ng0);
    t3 = ((char*)((ng14)));
    t6 = (t0 + 6136);
    xsi_vlogvar_assign_value(t6, t3, 0, 0, 5);
    goto LAB172;

LAB162:    xsi_set_current_line(304, ng0);

LAB244:    xsi_set_current_line(305, ng0);
    t3 = ((char*)((ng17)));
    t6 = (t0 + 6136);
    xsi_vlogvar_assign_value(t6, t3, 0, 0, 5);
    goto LAB172;

LAB164:    xsi_set_current_line(308, ng0);

LAB245:    xsi_set_current_line(309, ng0);
    t3 = (t0 + 7256);
    t6 = (t3 + 56U);
    t7 = *((char **)t6);
    t9 = ((char*)((ng10)));
    memset(t8, 0, 8);
    t10 = (t7 + 4);
    t23 = (t9 + 4);
    t11 = *((unsigned int *)t7);
    t12 = *((unsigned int *)t9);
    t13 = (t11 ^ t12);
    t14 = *((unsigned int *)t10);
    t15 = *((unsigned int *)t23);
    t16 = (t14 ^ t15);
    t17 = (t13 | t16);
    t18 = *((unsigned int *)t10);
    t19 = *((unsigned int *)t23);
    t20 = (t18 | t19);
    t21 = (~(t20));
    t22 = (t17 & t21);
    if (t22 != 0)
        goto LAB249;

LAB246:    if (t20 != 0)
        goto LAB248;

LAB247:    *((unsigned int *)t8) = 1;

LAB249:    t30 = (t8 + 4);
    t25 = *((unsigned int *)t30);
    t26 = (~(t25));
    t27 = *((unsigned int *)t8);
    t28 = (t27 & t26);
    t29 = (t28 != 0);
    if (t29 > 0)
        goto LAB250;

LAB251:
LAB252:    goto LAB172;

LAB166:    xsi_set_current_line(314, ng0);

LAB254:    xsi_set_current_line(315, ng0);
    t3 = (t0 + 6936);
    t6 = (t3 + 56U);
    t7 = *((char **)t6);
    t9 = ((char*)((ng10)));
    memset(t8, 0, 8);
    t10 = (t7 + 4);
    t23 = (t9 + 4);
    t11 = *((unsigned int *)t7);
    t12 = *((unsigned int *)t9);
    t13 = (t11 ^ t12);
    t14 = *((unsigned int *)t10);
    t15 = *((unsigned int *)t23);
    t16 = (t14 ^ t15);
    t17 = (t13 | t16);
    t18 = *((unsigned int *)t10);
    t19 = *((unsigned int *)t23);
    t20 = (t18 | t19);
    t21 = (~(t20));
    t22 = (t17 & t21);
    if (t22 != 0)
        goto LAB258;

LAB255:    if (t20 != 0)
        goto LAB257;

LAB256:    *((unsigned int *)t8) = 1;

LAB258:    t30 = (t8 + 4);
    t25 = *((unsigned int *)t30);
    t26 = (~(t25));
    t27 = *((unsigned int *)t8);
    t28 = (t27 & t26);
    t29 = (t28 != 0);
    if (t29 > 0)
        goto LAB259;

LAB260:
LAB261:    goto LAB172;

LAB168:    xsi_set_current_line(320, ng0);

LAB263:    xsi_set_current_line(321, ng0);
    t3 = ((char*)((ng19)));
    t6 = (t0 + 6136);
    xsi_vlogvar_assign_value(t6, t3, 0, 0, 5);
    goto LAB172;

LAB176:    t24 = (t8 + 4);
    *((unsigned int *)t8) = 1;
    *((unsigned int *)t24) = 1;
    goto LAB177;

LAB178:    xsi_set_current_line(236, ng0);

LAB181:    xsi_set_current_line(237, ng0);
    t31 = ((char*)((ng11)));
    t40 = (t0 + 6136);
    xsi_vlogvar_assign_value(t40, t31, 0, 0, 5);
    goto LAB180;

LAB185:    t24 = (t8 + 4);
    *((unsigned int *)t8) = 1;
    *((unsigned int *)t24) = 1;
    goto LAB186;

LAB187:    xsi_set_current_line(242, ng0);

LAB190:    xsi_set_current_line(243, ng0);
    t31 = ((char*)((ng6)));
    t40 = (t0 + 6136);
    xsi_vlogvar_assign_value(t40, t31, 0, 0, 5);
    goto LAB189;

LAB195:    t24 = (t8 + 4);
    *((unsigned int *)t8) = 1;
    *((unsigned int *)t24) = 1;
    goto LAB196;

LAB197:    xsi_set_current_line(251, ng0);

LAB200:    xsi_set_current_line(252, ng0);
    t31 = ((char*)((ng1)));
    t40 = (t0 + 6136);
    xsi_vlogvar_assign_value(t40, t31, 0, 0, 5);
    goto LAB199;

LAB208:    t24 = (t8 + 4);
    *((unsigned int *)t8) = 1;
    *((unsigned int *)t24) = 1;
    goto LAB209;

LAB210:    xsi_set_current_line(272, ng0);

LAB213:    xsi_set_current_line(273, ng0);
    t31 = ((char*)((ng10)));
    t40 = (t0 + 6136);
    xsi_vlogvar_assign_value(t40, t31, 0, 0, 5);
    goto LAB212;

LAB216:    t24 = (t8 + 4);
    *((unsigned int *)t8) = 1;
    *((unsigned int *)t24) = 1;
    goto LAB218;

LAB217:    *((unsigned int *)t8) = 1;
    goto LAB218;

LAB220:    xsi_set_current_line(278, ng0);

LAB223:    xsi_set_current_line(279, ng0);
    t31 = ((char*)((ng5)));
    t40 = (t0 + 6136);
    xsi_vlogvar_assign_value(t40, t31, 0, 0, 5);
    goto LAB222;

LAB226:    t23 = (t8 + 4);
    *((unsigned int *)t8) = 1;
    *((unsigned int *)t23) = 1;
    goto LAB227;

LAB228:    xsi_set_current_line(281, ng0);

LAB231:    xsi_set_current_line(282, ng0);
    t30 = ((char*)((ng15)));
    t31 = (t0 + 6136);
    xsi_vlogvar_assign_value(t31, t30, 0, 0, 5);
    goto LAB230;

LAB235:    t24 = (t8 + 4);
    *((unsigned int *)t8) = 1;
    *((unsigned int *)t24) = 1;
    goto LAB236;

LAB237:    xsi_set_current_line(287, ng0);

LAB240:    xsi_set_current_line(288, ng0);
    t31 = ((char*)((ng4)));
    t40 = (t0 + 6136);
    xsi_vlogvar_assign_value(t40, t31, 0, 0, 5);
    goto LAB239;

LAB248:    t24 = (t8 + 4);
    *((unsigned int *)t8) = 1;
    *((unsigned int *)t24) = 1;
    goto LAB249;

LAB250:    xsi_set_current_line(309, ng0);

LAB253:    xsi_set_current_line(310, ng0);
    t31 = ((char*)((ng16)));
    t40 = (t0 + 6136);
    xsi_vlogvar_assign_value(t40, t31, 0, 0, 5);
    goto LAB252;

LAB257:    t24 = (t8 + 4);
    *((unsigned int *)t8) = 1;
    *((unsigned int *)t24) = 1;
    goto LAB258;

LAB259:    xsi_set_current_line(315, ng0);

LAB262:    xsi_set_current_line(316, ng0);
    t31 = ((char*)((ng8)));
    t40 = (t0 + 6136);
    xsi_vlogvar_assign_value(t40, t31, 0, 0, 5);
    goto LAB261;

}

static void Always_341_1(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;
    int t8;
    char *t9;
    char *t10;

LAB0:    t1 = (t0 + 10656U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(341, ng0);
    t2 = (t0 + 11240);
    *((int *)t2) = 1;
    t3 = (t0 + 10688);
    *((char **)t3) = t2;
    *((char **)t1) = &&LAB4;

LAB1:    return;
LAB4:    xsi_set_current_line(341, ng0);

LAB5:    xsi_set_current_line(342, ng0);
    t4 = (t0 + 4696);
    t5 = (t4 + 56U);
    t6 = *((char **)t5);

LAB6:    t7 = ((char*)((ng2)));
    t8 = xsi_vlog_unsigned_case_compare(t6, 5, t7, 5);
    if (t8 == 1)
        goto LAB7;

LAB8:    t2 = ((char*)((ng1)));
    t8 = xsi_vlog_unsigned_case_compare(t6, 5, t2, 5);
    if (t8 == 1)
        goto LAB9;

LAB10:    t2 = ((char*)((ng4)));
    t8 = xsi_vlog_unsigned_case_compare(t6, 5, t2, 5);
    if (t8 == 1)
        goto LAB11;

LAB12:    t2 = ((char*)((ng5)));
    t8 = xsi_vlog_unsigned_case_compare(t6, 5, t2, 5);
    if (t8 == 1)
        goto LAB13;

LAB14:    t2 = ((char*)((ng6)));
    t8 = xsi_vlog_unsigned_case_compare(t6, 5, t2, 5);
    if (t8 == 1)
        goto LAB15;

LAB16:    t2 = ((char*)((ng7)));
    t8 = xsi_vlog_unsigned_case_compare(t6, 5, t2, 5);
    if (t8 == 1)
        goto LAB17;

LAB18:    t2 = ((char*)((ng8)));
    t8 = xsi_vlog_unsigned_case_compare(t6, 5, t2, 5);
    if (t8 == 1)
        goto LAB19;

LAB20:    t2 = ((char*)((ng9)));
    t8 = xsi_vlog_unsigned_case_compare(t6, 5, t2, 5);
    if (t8 == 1)
        goto LAB21;

LAB22:    t2 = ((char*)((ng10)));
    t8 = xsi_vlog_unsigned_case_compare(t6, 5, t2, 5);
    if (t8 == 1)
        goto LAB23;

LAB24:    t2 = ((char*)((ng11)));
    t8 = xsi_vlog_unsigned_case_compare(t6, 5, t2, 5);
    if (t8 == 1)
        goto LAB25;

LAB26:    t2 = ((char*)((ng12)));
    t8 = xsi_vlog_unsigned_case_compare(t6, 5, t2, 5);
    if (t8 == 1)
        goto LAB27;

LAB28:    t2 = ((char*)((ng13)));
    t8 = xsi_vlog_unsigned_case_compare(t6, 5, t2, 5);
    if (t8 == 1)
        goto LAB29;

LAB30:    t2 = ((char*)((ng14)));
    t8 = xsi_vlog_unsigned_case_compare(t6, 5, t2, 5);
    if (t8 == 1)
        goto LAB31;

LAB32:    t2 = ((char*)((ng15)));
    t8 = xsi_vlog_unsigned_case_compare(t6, 5, t2, 5);
    if (t8 == 1)
        goto LAB33;

LAB34:    t2 = ((char*)((ng16)));
    t8 = xsi_vlog_unsigned_case_compare(t6, 5, t2, 5);
    if (t8 == 1)
        goto LAB35;

LAB36:    t2 = ((char*)((ng17)));
    t8 = xsi_vlog_unsigned_case_compare(t6, 5, t2, 5);
    if (t8 == 1)
        goto LAB37;

LAB38:
LAB40:
LAB39:    xsi_set_current_line(408, ng0);

LAB58:    xsi_set_current_line(410, ng0);
    t2 = ((char*)((ng7)));
    t3 = (t0 + 4856);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 8);

LAB41:    goto LAB2;

LAB7:    xsi_set_current_line(344, ng0);

LAB42:    xsi_set_current_line(345, ng0);
    t9 = ((char*)((ng19)));
    t10 = (t0 + 4856);
    xsi_vlogvar_assign_value(t10, t9, 0, 0, 8);
    goto LAB41;

LAB9:    xsi_set_current_line(348, ng0);

LAB43:    xsi_set_current_line(349, ng0);
    t3 = ((char*)((ng15)));
    t4 = (t0 + 4856);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 8);
    goto LAB41;

LAB11:    xsi_set_current_line(352, ng0);

LAB44:    xsi_set_current_line(353, ng0);
    t3 = ((char*)((ng14)));
    t4 = (t0 + 4856);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 8);
    goto LAB41;

LAB13:    xsi_set_current_line(356, ng0);

LAB45:    xsi_set_current_line(357, ng0);
    t3 = ((char*)((ng8)));
    t4 = (t0 + 4856);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 8);
    goto LAB41;

LAB15:    xsi_set_current_line(360, ng0);

LAB46:    xsi_set_current_line(361, ng0);
    t3 = ((char*)((ng16)));
    t4 = (t0 + 4856);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 8);
    goto LAB41;

LAB17:    xsi_set_current_line(364, ng0);

LAB47:    xsi_set_current_line(365, ng0);
    t3 = ((char*)((ng17)));
    t4 = (t0 + 4856);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 8);
    goto LAB41;

LAB19:    xsi_set_current_line(368, ng0);

LAB48:    xsi_set_current_line(369, ng0);
    t3 = ((char*)((ng12)));
    t4 = (t0 + 4856);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 8);
    goto LAB41;

LAB21:    xsi_set_current_line(372, ng0);

LAB49:    xsi_set_current_line(373, ng0);
    t3 = ((char*)((ng9)));
    t4 = (t0 + 4856);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 8);
    goto LAB41;

LAB23:    xsi_set_current_line(376, ng0);

LAB50:    xsi_set_current_line(377, ng0);
    t3 = ((char*)((ng13)));
    t4 = (t0 + 4856);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 8);
    goto LAB41;

LAB25:    xsi_set_current_line(380, ng0);

LAB51:    xsi_set_current_line(381, ng0);
    t3 = ((char*)((ng2)));
    t4 = (t0 + 4856);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 8);
    goto LAB41;

LAB27:    xsi_set_current_line(384, ng0);

LAB52:    xsi_set_current_line(385, ng0);
    t3 = ((char*)((ng1)));
    t4 = (t0 + 4856);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 8);
    goto LAB41;

LAB29:    xsi_set_current_line(388, ng0);

LAB53:    xsi_set_current_line(389, ng0);
    t3 = ((char*)((ng4)));
    t4 = (t0 + 4856);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 8);
    goto LAB41;

LAB31:    xsi_set_current_line(392, ng0);

LAB54:    xsi_set_current_line(393, ng0);
    t3 = ((char*)((ng5)));
    t4 = (t0 + 4856);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 8);
    goto LAB41;

LAB33:    xsi_set_current_line(396, ng0);

LAB55:    xsi_set_current_line(397, ng0);
    t3 = ((char*)((ng10)));
    t4 = (t0 + 4856);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 8);
    goto LAB41;

LAB35:    xsi_set_current_line(400, ng0);

LAB56:    xsi_set_current_line(401, ng0);
    t3 = ((char*)((ng11)));
    t4 = (t0 + 4856);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 8);
    goto LAB41;

LAB37:    xsi_set_current_line(404, ng0);

LAB57:    xsi_set_current_line(405, ng0);
    t3 = ((char*)((ng6)));
    t4 = (t0 + 4856);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 8);
    goto LAB41;

}

static void Cont_417_2(char *t0)
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

LAB0:    t1 = (t0 + 10904U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(417, ng0);
    t2 = (t0 + 4856);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t0 + 11336);
    t6 = (t5 + 56U);
    t7 = *((char **)t6);
    t8 = (t7 + 56U);
    t9 = *((char **)t8);
    memset(t9, 0, 8);
    t10 = 255U;
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
    xsi_driver_vfirst_trans(t5, 0, 7);
    t18 = (t0 + 11256);
    *((int *)t18) = 1;

LAB1:    return;
}


extern void work_m_08728299871902389488_2814307527_init()
{
	static char *pe[] = {(void *)Always_80_0,(void *)Always_341_1,(void *)Cont_417_2};
	xsi_register_didat("work_m_08728299871902389488_2814307527", "isim/custom_architecture_tb_isim_beh.exe.sim/work/m_08728299871902389488_2814307527.didat");
	xsi_register_executes(pe);
}
