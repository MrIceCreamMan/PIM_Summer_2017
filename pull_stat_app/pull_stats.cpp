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

int main(int argc, char const *argv[]) {
	ifstream input_data;
        // cout << " 1 " << argv[1] << " 2 " << argv[2] << " 3 " << argv[3] << endl; 
        if (argc > 1)
                input_data.open(argv[1]);
        else
	        input_data.open("stats.txt");
	string temp_str;
	const int list_size = 20;
	int check[list_size];
	for (int i = 0; i < list_size; i++)
		check[i] = 0;
	string cmp_list[list_size] = {
"timestamp6.sim_seconds",
"timestamp6.sim_ticks",
"timestamp6.host_inst_rate",
"timestamp6.host_op_rate",
"timestamp6.host_tick_rate",
"timestamp6.host_mem_usage",
"timestamp6.host_seconds",
"timestamp6.sim_insts",
"timestamp6.sim_ops",
"timestamp6.system.pim_sys.dtlb.tlb_access",
"timestamp6.system.l2.overall_misses::total",
"timestamp6.system.l2.ReadExReq_miss_latency::total",
"softwarestat.kernel.offloadsize",
"softwarestat.task.datasize",
"softwarestat.task.pagesize",
"softwarestat.task.overhead",
"softwarestat.pages.count",
"softwarestat.slices.count",
"softwarestat.slices.reduction",
"place_holder"};
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
// cmp_list[1] = cmp_list[16];
	ofstream outd;
        if (argc > 1)
	        outd.open(argv[2]);
        else
                outd.open("shorter.txt");

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







