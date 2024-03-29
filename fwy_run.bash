#iostat -x 1 -k -p > iostat_wcc.log&
#./bin/wcc_vc_batch_exec -i ssd/workspace_4/ -cc 1 -buffer_size 3 -cores 12 > wcc.txt 2>&1
#killall iostat
iostat -x 1 -k -p > iostat_pr.log&
./bin/pr_vc_exec -i ssd/workspace_10/ -cc 1 -buffer_size 1 -cores 12 > pr.txt 2>&1
killall iostat