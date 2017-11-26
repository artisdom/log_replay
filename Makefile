all:
	@rm -f log_replay test_boost
	g++ -std=c++11 -pthread main.cpp -o log_replay -lboost_filesystem -lboost_system
	g++ -std=c++11 test_boost.cpp -o test_boost -lboost_filesystem -lboost_system
	g++ -std=c++11 test_boost3.cpp -o test_boost3 -lboost_filesystem -lboost_system
	g++ test_boost_ptime_duration.cpp -o test_boost_ptime_duration -lboost_date_time
install:
	cp -vf log_replay ~/bin/
