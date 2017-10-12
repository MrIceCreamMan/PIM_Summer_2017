#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool strcomp(string s1, string s2){
	if (s1.length() == s2.length()){
		for (int i = 0; i < s1.length(); i++){
			if (s1[i] != s2[i])
				return false;
		}
		return true;
	}
	else
		return false;
}

int main(){
	ifstream input_data;
	input_data.open("stats.txt");
	string temp_str;
	const int list_size = 34;
	int check[list_size];
	for (int i = 0; i < list_size; i++)
		check[i] = 0;
	string cmp_list[list_size] = {"timestamp6.sim_seconds","timestamp6.sim_ticks","timestamp6.host_inst_rate",
"timestamp6.host_op_rate","timestamp6.host_tick_rate","timestamp6.host_mem_usage","timestamp6.host_seconds",
"timestamp6.sim_insts","timestamp6.sim_ops",
"timestamp6.system.mem_ctrls11.num_reads::system.pim_sys.dma",
"timestamp6.system.mem_ctrls11.num_reads::total",
"timestamp6.system.mem_ctrls11.num_writes::system.pim_sys.dma",
"timestamp6.system.mem_ctrls11.num_writes::system.pim_sys.cpu.data",
"timestamp6.system.mem_ctrls11.bw_read::system.pim_sys.dma",
"timestamp6.system.mem_ctrls11.bw_read::system.cpu1.inst",
"timestamp6.system.mem_ctrls11.bw_read::system.cpu1.data",
"timestamp6.system.mem_ctrls11.bw_write::system.pim_sys.dma",
"timestamp6.system.mem_ctrls11.bw_write::system.pim_sys.cpu.data",
"timestamp6.system.mem_ctrls11.bw_write::total",
"timestamp6.system.mem_ctrls11.bw_total::system.pim_sys.dma",
"timestamp6.system.mem_ctrls11.bw_total::system.pim_sys.cpu.data",
"timestamp6.system.mem_ctrls08.bytes_read::system.pim_sys.dma",
"timestamp6.system.tol2bus.pkt_count_system.cpu0.icache.mem_side::system.l2.cpu_side",
"timestamp6.system.tol2bus.pkt_count_system.cpu0.dcache.mem_side::system.l2.cpu_side",
"timestamp6.system.tol2bus.pkt_count_system.cpu1.icache.mem_side::system.l2.cpu_side",
"timestamp6.system.tol2bus.pkt_count_system.cpu1.dcache.mem_side::system.l2.cpu_side",
"softwarestat.kernel.offloadsize","softwarestat.task.datasize",
"softwarestat.task.pagesize","softwarestat.task.overhead","softwarestat.pages.count","softwarestat.slices.count",
"softwarestat.slices.reduction","place_holder"};
	string results[list_size];
	int i = 0;
	while (input_data >> temp_str){
		if (strcomp(temp_str,cmp_list[i])){
			if (check[i] == 0){
				input_data >> results[i];
				check[i] = 1;
				if (i < (list_size-1))
					i++;
				else
					i = list_size-1;
			}
		}
	}
  input_data.close();
	/*
	results[16] = results[11];
	for (int i=11; i>1; i--)
		results[i] = results[i-1];
	results[1] = results[16];
	cmp_list[16] = cmp_list[11];
	for (int i=11; i>1; i--)
		cmp_list[i] = cmp_list[i-1];
	cmp_list[1] = cmp_list[16];
*/
	ofstream outd;
	outd.open("better_stat.txt");
	for (int i=0; i<(list_size-1); i++)
		outd << cmp_list[i] << endl;
	for (int j=0; j<(list_size-1); j++)
		outd << results[j] << "\t";
	outd << "\n\n\n\n\n";
	outd.close();
	cout << "\n\n________________________________________________________" << endl;
	cout << "Dear Master, your data are ready. Please have a look. :)\n\n" << endl;

	return 0;
}







