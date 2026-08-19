g++ AVLTree.cpp -o AVLTree.exe
AVLTree.exe < testcase_avl.txt > output_avl.txt

g++ IntervalScheduler.cpp -o IntervalScheduler.exe
IntervalScheduler.exe < testcase_basic_interval.txt > output_interval_basic.txt
IntervalScheduler.exe < testcase_edge_interval.txt > output_interval_edge.txt
IntervalScheduler.exe < testcase_large_interval.txt > output_interval_large.txt