open_system('CustArch_v20');
open_system('gm_CustArch_v20');
cs.HiliteType = 'user2';
cs.ForegroundColor = 'black';
cs.BackgroundColor = 'gray';
set_param(0, 'HiliteAncestorsData', cs);
hilite_system('gm_CustArch_v20/cust_architecture/process_and_retrieve/two_pole_IIR', 'user2');
annotate_port('gm_CustArch_v20/cust_architecture/process_and_retrieve/two_pole_IIR', 1, 1, 'Obstacle to clock-rate pipelining because the block is in a feedback loop');
hilite_system('CustArch_v20/cust_architecture/process_and_retrieve/two_pole_IIR', 'user2');
annotate_port('CustArch_v20/cust_architecture/process_and_retrieve/two_pole_IIR', 1, 1, 'Obstacle to clock-rate pipelining because the block is in a feedback loop');
cs.HiliteType = 'user2';
cs.ForegroundColor = 'black';
cs.BackgroundColor = 'gray';
set_param(0, 'HiliteAncestorsData', cs);
hilite_system('gm_CustArch_v20/cust_architecture/process_and_retrieve/two_pole_IIR/HP_Butter_2nd', 'user2');
annotate_port('gm_CustArch_v20/cust_architecture/process_and_retrieve/two_pole_IIR/HP_Butter_2nd', 1, 1, 'Obstacle to clock-rate pipelining because the block is in a feedback loop');
hilite_system('CustArch_v20/cust_architecture/process_and_retrieve/two_pole_IIR/HP_Butter_2nd', 'user2');
annotate_port('CustArch_v20/cust_architecture/process_and_retrieve/two_pole_IIR/HP_Butter_2nd', 1, 1, 'Obstacle to clock-rate pipelining because the block is in a feedback loop');
