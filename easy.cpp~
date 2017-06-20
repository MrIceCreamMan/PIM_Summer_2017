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
	int check[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	string cmp_list[17] = {"timestamp6.sim_seconds","timestamp6.sim_ticks","timestamp6.host_inst_rate",
"timestamp6.host_op_rate","timestamp6.host_tick_rate","timestamp6.host_mem_usage","timestamp6.host_seconds",
"timestamp6.sim_insts","timestamp6.sim_ops","softwarestat.kernel.offloadsize","softwarestat.task.datasize",
"softwarestat.task.pagesize","softwarestat.task.overhead","softwarestat.pages.count","softwarestat.slices.count",
"softwarestat.slices.reduction","place_holder"};
	string results[17];
	int i = 0;
	while (input_data >> temp_str){
		if (strcomp(temp_str,cmp_list[i])){
			if (check[i] == 0){
				input_data >> results[i];
				check[i] = 1;
				if (i < 16)
					i++;
				else
					i = 16;
			}
		}
	}
  input_data.close();
	results[16] = results[11];
	for (int i=11; i>1; i--)
		results[i] = results[i-1];
	results[1] = results[16];
	cmp_list[16] = cmp_list[11];
	for (int i=11; i>1; i--)
		cmp_list[i] = cmp_list[i-1];
	cmp_list[1] = cmp_list[16];

	ofstream outd;
	outd.open("better_stat.txt");
	for (int i=0; i<16; i++)
		outd << cmp_list[i] << endl;
	for (int j=0; j<16; j++)
		outd << results[j] << "\t";
	outd << "\n\n\n\n\n";
	outd.close();
	cout << "\n\n________________________________________________________" << endl;
	cout << "Dear Master, your data are ready. Please have a look. :)\n\n" << endl;

	return 0;
}







