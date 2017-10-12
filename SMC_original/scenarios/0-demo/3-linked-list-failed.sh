#!/bin/bash

GEM5_STATISTICS=(
"sim_ticks.pim"
"sim_ticks.host"
"sim_ticks.ratio_host_to_pim"
"power_related_to_host"
"power_related_to_pim"
)

LINKED_LIST_KERNEL_NAME=linked_list
export KEY_LOWER_BOUND=1 # variable depending on available memory size
export KEY_UPPER_BOUND=512 # variable depending on available memory size
export MAX_LIST_SIZE=512 # variable depending on available memory size
export INITIAL_LIST_SIZE=100 # variable by user
export TOTAL_NUM_OPS=100 # variable by user
export READ_ONLY_PERCENTAGE=50 # variable by user

source UTILS/default_params.sh
create_scenario "$0/$*" "$LINKED_LIST_KERNEL_NAME-$(zpad $TOTAL_NUM_OPS 6)-$(zpad $READ_ONLY_PERCENTAGE 3)" "ARMv7 + HMC2011 + Linux (VExpress_EMM) + PIM(ARMv7)"

####################################
load_model memory/hmc_2011.sh
load_model system/gem5_fullsystem_arm7.sh
load_model system/gem5_pim.sh
#load_model gem5_perf_sim.sh				# Fast simulation without debugging

export OFFLOADED_KERNEL_NAME=$LINKED_LIST_KERNEL_NAME

#####################
#####################
load_model common_params_date2016.sh # TODO: remove? contains mostly unnecessary graph parameters for sgraph scenarios
#####################
#####################

source ./smc.sh -u $*	# Update these variables in the simulation environment
load_model gem5_automated_sim.sh homo		# Automated simulation

####################################

print_msg "Build and copy the required files to the extra image ..."

#*******
clonedir $PIM_SW_DIR/resident
cp $HOST_SW_DIR/app/linked_list/defs.hh .
run ./build.sh  7   # Build the main resident code
run ./build.sh 7 "${OFFLOADED_KERNEL_NAME}" # Build a specific kernel code (name without suffix)
returntopwd
#*******
clonedir $HOST_SW_DIR/driver
cp ../resident/definitions.h .
run ./build.sh
returntopwd
#*******
clonedir $HOST_SW_DIR/api
cp ../resident/definitions.h .
cp ../driver/defs.h .
run ./build.sh
returntopwd
#*******
clonedir $HOST_SW_DIR/app/linked_list
cp ../api/pim_api.a ./libpimapi.a
cp ../api/*.hh .
cp $HOST_SW_DIR/app/app_utils.hh .
run ./build.sh
returntopwd
#*******

cd $SCENARIO_CASE_DIR

echo -e "
echo; echo \">>>> Install the driver\";
./ins.sh
echo; echo \">>>> Run the application and offload the kernel ...\";
./main
" > ./do
chmod +x ./do

    copy_to_extra_image  driver/pim.ko driver/ins.sh ./do linked_list/main resident/${OFFLOADED_KERNEL_NAME}.hex 
returntopwd

####################################

source ./smc.sh $*
	
finalize_gem5_simulation
plot_bar_chart "sim_ticks.pim" 0 "(ps)" #--no-output
plot_bar_chart "sim_ticks.host" 0 "(ps)" #--no-output
print_msg "Done!"
