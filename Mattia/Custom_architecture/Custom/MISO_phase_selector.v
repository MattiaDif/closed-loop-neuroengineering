`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 		 Intan Technologies, LLC
// 
// Design Name: 	 RHS2000 Rhythm Interface
// Module Name:    MISO_phase_selector, MISO_DDR_phase_selector
// Project Name:   Opal Kelly FPGA/USB RHD2000 Interface
// Target Devices: 
// Tool versions: 
// Description:    Downsamples MISO by factor of 4, with selectable phase lag to
//                 compensate for headstage cable delay.
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////

module MISO_phase_selector(
	input wire [3:0] 		phase_select,	// MISO sampling phase lag to compensate for headstage cable delay
	input wire [133:0] 	MISO4x,			// 4x oversampled MISO input
	output reg [32:0] 	MISO				// 32-bit MISO output
	);

	always @(*) begin
		case (phase_select)
			0:       MISO <= {MISO4x[0],  MISO4x[4],  MISO4x[8],  MISO4x[12], MISO4x[16], MISO4x[20], MISO4x[24], MISO4x[28], MISO4x[32], MISO4x[36], MISO4x[40], MISO4x[44], MISO4x[48], MISO4x[52], MISO4x[56], MISO4x[60], MISO4x[64], MISO4x[68], MISO4x[72], MISO4x[76], MISO4x[80], MISO4x[84], MISO4x[88], MISO4x[92], MISO4x[96], MISO4x[100], MISO4x[104], MISO4x[108], MISO4x[112], MISO4x[116], MISO4x[120], MISO4x[124]};
			1:       MISO <= {MISO4x[1],  MISO4x[5],  MISO4x[9],  MISO4x[13], MISO4x[17], MISO4x[21], MISO4x[25], MISO4x[29], MISO4x[33], MISO4x[37], MISO4x[41], MISO4x[45], MISO4x[49], MISO4x[53], MISO4x[57], MISO4x[61], MISO4x[65], MISO4x[69], MISO4x[73], MISO4x[77], MISO4x[81], MISO4x[85], MISO4x[89], MISO4x[93], MISO4x[97], MISO4x[101], MISO4x[105], MISO4x[109], MISO4x[113], MISO4x[118], MISO4x[121], MISO4x[125]};
			2:       MISO <= {MISO4x[2],  MISO4x[6],  MISO4x[10], MISO4x[14], MISO4x[18], MISO4x[22], MISO4x[26], MISO4x[30], MISO4x[34], MISO4x[38], MISO4x[42], MISO4x[46], MISO4x[50], MISO4x[54], MISO4x[58], MISO4x[62], MISO4x[66], MISO4x[70], MISO4x[74], MISO4x[78], MISO4x[82], MISO4x[86], MISO4x[90], MISO4x[94], MISO4x[98], MISO4x[102], MISO4x[106], MISO4x[110], MISO4x[114], MISO4x[119], MISO4x[122], MISO4x[126]};
			3:       MISO <= {MISO4x[3],  MISO4x[7],  MISO4x[11], MISO4x[15], MISO4x[19], MISO4x[23], MISO4x[27], MISO4x[31], MISO4x[35], MISO4x[39], MISO4x[43], MISO4x[47], MISO4x[51], MISO4x[55], MISO4x[59], MISO4x[63], MISO4x[67], MISO4x[71], MISO4x[75], MISO4x[79], MISO4x[83], MISO4x[87], MISO4x[91], MISO4x[95], MISO4x[99], MISO4x[103], MISO4x[107], MISO4x[111], MISO4x[115], MISO4x[120], MISO4x[123], MISO4x[127]};
			4:       MISO <= {MISO4x[4],  MISO4x[8],  MISO4x[12], MISO4x[16], MISO4x[20], MISO4x[24], MISO4x[28], MISO4x[32], MISO4x[36], MISO4x[40], MISO4x[44], MISO4x[48], MISO4x[52], MISO4x[56], MISO4x[60], MISO4x[64], MISO4x[68], MISO4x[72], MISO4x[76], MISO4x[80], MISO4x[84], MISO4x[88], MISO4x[92], MISO4x[96], MISO4x[100], MISO4x[104], MISO4x[108], MISO4x[112], MISO4x[116], MISO4x[121], MISO4x[124], MISO4x[128]};
			5:       MISO <= {MISO4x[5],  MISO4x[9],  MISO4x[13], MISO4x[17], MISO4x[21], MISO4x[25], MISO4x[29], MISO4x[33], MISO4x[37], MISO4x[41], MISO4x[45], MISO4x[49], MISO4x[53], MISO4x[57], MISO4x[61], MISO4x[65], MISO4x[69], MISO4x[73], MISO4x[77], MISO4x[81], MISO4x[85], MISO4x[89], MISO4x[93], MISO4x[97], MISO4x[101], MISO4x[105], MISO4x[109], MISO4x[113], MISO4x[117], MISO4x[122], MISO4x[125], MISO4x[129]};
			6:       MISO <= {MISO4x[6],  MISO4x[10], MISO4x[14], MISO4x[18], MISO4x[22], MISO4x[26], MISO4x[30], MISO4x[34], MISO4x[38], MISO4x[42], MISO4x[46], MISO4x[50], MISO4x[54], MISO4x[58], MISO4x[62], MISO4x[66], MISO4x[70], MISO4x[74], MISO4x[78], MISO4x[82], MISO4x[86], MISO4x[90], MISO4x[94], MISO4x[98], MISO4x[102], MISO4x[106], MISO4x[110], MISO4x[114], MISO4x[118], MISO4x[123], MISO4x[126], MISO4x[130]};
			7:       MISO <= {MISO4x[7],  MISO4x[11], MISO4x[15], MISO4x[19], MISO4x[23], MISO4x[27], MISO4x[31], MISO4x[35], MISO4x[39], MISO4x[43], MISO4x[47], MISO4x[51], MISO4x[55], MISO4x[59], MISO4x[63], MISO4x[67], MISO4x[71], MISO4x[75], MISO4x[79], MISO4x[83], MISO4x[87], MISO4x[91], MISO4x[95], MISO4x[99], MISO4x[103], MISO4x[107], MISO4x[111], MISO4x[115], MISO4x[119], MISO4x[124], MISO4x[127], MISO4x[131]};
			8:       MISO <= {MISO4x[8],  MISO4x[12], MISO4x[16], MISO4x[20], MISO4x[24], MISO4x[28], MISO4x[32], MISO4x[36], MISO4x[40], MISO4x[44], MISO4x[48], MISO4x[52], MISO4x[56], MISO4x[60], MISO4x[64], MISO4x[68], MISO4x[72], MISO4x[76], MISO4x[80], MISO4x[84], MISO4x[88], MISO4x[92], MISO4x[96], MISO4x[100], MISO4x[104], MISO4x[108], MISO4x[112], MISO4x[116], MISO4x[120], MISO4x[125], MISO4x[128], MISO4x[132]};
			9:       MISO <= {MISO4x[9],  MISO4x[13], MISO4x[17], MISO4x[21], MISO4x[25], MISO4x[29], MISO4x[33], MISO4x[37], MISO4x[41], MISO4x[45], MISO4x[49], MISO4x[53], MISO4x[57], MISO4x[61], MISO4x[65], MISO4x[69], MISO4x[73], MISO4x[77], MISO4x[81], MISO4x[85], MISO4x[89], MISO4x[93], MISO4x[97], MISO4x[101], MISO4x[105], MISO4x[109], MISO4x[113], MISO4x[117], MISO4x[121], MISO4x[126], MISO4x[129], MISO4x[133]};
			default: MISO <= {MISO4x[9],  MISO4x[13], MISO4x[17], MISO4x[21], MISO4x[25], MISO4x[29], MISO4x[33], MISO4x[37], MISO4x[41], MISO4x[45], MISO4x[49], MISO4x[53], MISO4x[57], MISO4x[61], MISO4x[65], MISO4x[69], MISO4x[73], MISO4x[77], MISO4x[81], MISO4x[85], MISO4x[89], MISO4x[93], MISO4x[97], MISO4x[101], MISO4x[105], MISO4x[109], MISO4x[113], MISO4x[117], MISO4x[121], MISO4x[126], MISO4x[129], MISO4x[133]};
		endcase
	end
	
endmodule


module MISO_DDR_phase_selector(
	input wire [3:0] 		phase_select,	// MISO sampling phase lag to compensate for headstage cable delay
	input wire [73:0] 	MISO4x,			// 4x oversampled MISO input
	output reg [15:0] 	MISO				// 16-bit MISO output
	);
	
	always @(*) begin
		case (phase_select)
			0:       MISO <= {MISO4x[2],  MISO4x[6],  MISO4x[10], MISO4x[14], MISO4x[18], MISO4x[22], MISO4x[26], MISO4x[30], MISO4x[34], MISO4x[38], MISO4x[42], MISO4x[46], MISO4x[50], MISO4x[54], MISO4x[58], MISO4x[62]};
			1:       MISO <= {MISO4x[3],  MISO4x[7],  MISO4x[11], MISO4x[15], MISO4x[19], MISO4x[23], MISO4x[27], MISO4x[31], MISO4x[35], MISO4x[39], MISO4x[43], MISO4x[47], MISO4x[51], MISO4x[55], MISO4x[59], MISO4x[63]};
			2:       MISO <= {MISO4x[4],  MISO4x[8],  MISO4x[12], MISO4x[16], MISO4x[20], MISO4x[24], MISO4x[28], MISO4x[32], MISO4x[36], MISO4x[40], MISO4x[44], MISO4x[48], MISO4x[52], MISO4x[56], MISO4x[60], MISO4x[64]};
			3:       MISO <= {MISO4x[5],  MISO4x[9],  MISO4x[13], MISO4x[17], MISO4x[21], MISO4x[25], MISO4x[29], MISO4x[33], MISO4x[37], MISO4x[41], MISO4x[45], MISO4x[49], MISO4x[53], MISO4x[57], MISO4x[61], MISO4x[65]};
			4:       MISO <= {MISO4x[6],  MISO4x[10], MISO4x[14], MISO4x[18], MISO4x[22], MISO4x[26], MISO4x[30], MISO4x[34], MISO4x[38], MISO4x[42], MISO4x[46], MISO4x[50], MISO4x[54], MISO4x[58], MISO4x[62], MISO4x[66]};
			5:       MISO <= {MISO4x[7],  MISO4x[11], MISO4x[15], MISO4x[19], MISO4x[23], MISO4x[27], MISO4x[31], MISO4x[35], MISO4x[39], MISO4x[43], MISO4x[47], MISO4x[51], MISO4x[55], MISO4x[59], MISO4x[63], MISO4x[67]};
			6:       MISO <= {MISO4x[8],  MISO4x[12], MISO4x[16], MISO4x[20], MISO4x[24], MISO4x[28], MISO4x[32], MISO4x[36], MISO4x[40], MISO4x[44], MISO4x[48], MISO4x[52], MISO4x[56], MISO4x[60], MISO4x[64], MISO4x[68]};
			7:       MISO <= {MISO4x[9],  MISO4x[13], MISO4x[17], MISO4x[21], MISO4x[25], MISO4x[29], MISO4x[33], MISO4x[37], MISO4x[41], MISO4x[45], MISO4x[49], MISO4x[53], MISO4x[57], MISO4x[61], MISO4x[65], MISO4x[69]};
			8:       MISO <= {MISO4x[10], MISO4x[14], MISO4x[18], MISO4x[22], MISO4x[26], MISO4x[30], MISO4x[34], MISO4x[38], MISO4x[42], MISO4x[46], MISO4x[50], MISO4x[54], MISO4x[58], MISO4x[62], MISO4x[66], MISO4x[70]};
			9:       MISO <= {MISO4x[11], MISO4x[15], MISO4x[19], MISO4x[23], MISO4x[27], MISO4x[31], MISO4x[35], MISO4x[39], MISO4x[43], MISO4x[47], MISO4x[51], MISO4x[55], MISO4x[59], MISO4x[63], MISO4x[67], MISO4x[71]};
			10:      MISO <= {MISO4x[12], MISO4x[16], MISO4x[20], MISO4x[24], MISO4x[28], MISO4x[32], MISO4x[36], MISO4x[40], MISO4x[44], MISO4x[48], MISO4x[52], MISO4x[56], MISO4x[60], MISO4x[64], MISO4x[68], MISO4x[72]};
			11:      MISO <= {MISO4x[13], MISO4x[17], MISO4x[21], MISO4x[25], MISO4x[29], MISO4x[33], MISO4x[37], MISO4x[41], MISO4x[45], MISO4x[49], MISO4x[53], MISO4x[57], MISO4x[61], MISO4x[65], MISO4x[69], MISO4x[73]};
			default: MISO <= {MISO4x[13], MISO4x[17], MISO4x[21], MISO4x[25], MISO4x[29], MISO4x[33], MISO4x[37], MISO4x[41], MISO4x[45], MISO4x[49], MISO4x[53], MISO4x[57], MISO4x[61], MISO4x[65], MISO4x[69], MISO4x[73]};
		endcase
	end
	
endmodule
