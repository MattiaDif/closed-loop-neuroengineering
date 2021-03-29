`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   11:14:38 04/23/2019
// Design Name:   CUST_ARCH
// Module Name:   C:/Users/BuccelliLab/Desktop/RHS2000InterfaceXEM6010 release 180814/custom_proc_architecture_tb.v
// Project Name:  RHS2000InterfaceXEM6010
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: CUST_ARCH
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////


module custom_proc_architecture_tb;

	// Inputs
	reg clk;
	reg reset;
	reg FIFO_write_to;
	reg [15:0] FIFO_data_in;
	reg [15:0] pipe_in_data;
	reg pipe_in_en;
	reg pipe_out_read;
	reg [15:0] cfg;
	reg [7:0] data_stream_en;

	// Outputs
	wire comp_data_out_valid;
	wire [15:0] comp_data_out;
	wire [7:0] trig_stim;
	wire computing;
	wire [15:0] pipe_out_data;
	wire [15:0] status;
	
	// need to redefine this parameter otherwise it would be zero
	defparam uut.HEADER_MAGIC_NUMBER = 64'h8d542c8a49712f0b;  // Fixed 64-bit "magic number" that begins each data frame
                                                        // to aid in synchronization.
									  
  
	integer f_comp_data_out, count;
	parameter WIDTH = 57520;
	reg [15:0] RHS_data_stored [0:WIDTH-1]; //WIDTH 16-bits words is the length of RHS_data_flow.txt
	reg RHS_data_valid [0:WIDTH-1]; //WIDTH 16-bits words is the length of RHS_data_flow.txt

	// Instantiate the Unit Under Test (UUT)
	CUST_ARCH uut (
		.clk(clk), 
		.reset(reset), 
		.FIFO_write_to(FIFO_write_to), 
		.FIFO_data_in(FIFO_data_in), 
		.comp_data_out_valid(comp_data_out_valid), 
		.comp_data_out(comp_data_out), 
		.trig_stim(trig_stim), 
		.computing(computing), 
		.pipe_in_data(pipe_in_data), 
		.pipe_in_en(pipe_in_en), 
		.pipe_out_data(pipe_out_data), 
		.pipe_out_read(pipe_out_read), 
		.cfg(cfg), 
		.data_stream_en(data_stream_en), 
		.status(status)
	);

	initial begin
	
		$readmemb("RHS_data_flow.txt", RHS_data_stored); // read data from txt in binary format
		$readmemb("RHS_data_valid.txt", RHS_data_valid); // read data from txt in binary format
		f_comp_data_out = $fopen("output_comp_data_out.txt","w");
		count=0;
		
		// Initialize Inputs
		clk = 0;
		reset = 1;
		FIFO_write_to = 0;
		FIFO_data_in = 0;
		pipe_in_data = 0;
		pipe_in_en = 0;
		pipe_out_read = 0;
		cfg = 0;
		data_stream_en = 8'b00000001; // play with this to check chan_en_rollmap.. still not clear to me!!
		#40;
		reset=0;
		data_stream_en = 8'b00000001; // play with this to check chan_en_rollmap.. still not clear to me!!

		/*// Wait 100 ns for global reset to finish
		#100;
		reset=0;
		FIFO_write_to = 1;
		// Add stimulus here
		#4;
		pipe_in_en=1;
		pipe_in_data=16'h0200; //512 meaning, next command (i.e. next value of pipe_in_data retrieved next positive clock edge) will be the new filter coefficient.
		#4;
		pipe_in_en=1;
		pipe_in_data=150; // is the new filter coefficient
		#4;
		pipe_in_en=1;
		pipe_out_read=1; // this is crucial to mimick the reading by USB so that it can pipe out next values
		pipe_in_data=16'h0200; 
		#4;
		pipe_in_en=1;
		pipe_in_data=16'h0100;
		#40;
		pipe_in_en=0;
		#2;
		pipe_in_en=1;
		pipe_in_data=16'h0101;	*/
	end
	
	
	always begin
		#20 
		clk =  ! clk;
		if (count<WIDTH) begin
			if (reset==0) begin
				if (clk==1) begin
					FIFO_write_to = RHS_data_valid[count];
					FIFO_data_in=RHS_data_stored[count];
					count=count+1;
					if (comp_data_out_valid==1) begin
					$display ("Current value of FIFO_data_in is %d", FIFO_data_in);
					$fwrite(f_comp_data_out,"%b\n",   comp_data_out); // write to comp_data_out output_file
					end
				end /*else begin
					FIFO_write_to = 0;
					FIFO_data_in = 0;
				end*/
			end
		end
	end
		
	/*always @(posedge clk) begin
		pipe_in_en=1;
		pipe_in_data=16'h0100; // this is to get the minor and major version numbers.. that works!
		
		// then you can change the filter coefficient writing to the PipeIn 0x88 0x0200 (the instruction) followed by a 16bits value (the new filter coefficient). 
//		pipe_in_data=16'h0200;
		
	end*/
	
endmodule

