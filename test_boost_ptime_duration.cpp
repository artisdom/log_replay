#include <boost/date_time/gregorian/gregorian.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"

int main(int argc, char** argv) {

    std::string date_1 = "2014-08-15 10:12:10";
    std::string date_2 = "2014-08-15 16:40:02";

    boost::posix_time::ptime t1(boost::posix_time::time_from_string(date_1));
    boost::posix_time::ptime t2(boost::posix_time::time_from_string(date_2));

    std::cout << "t1: " << t1 << std::endl;
    std::cout << "t2: " << t2 << std::endl;

    boost::posix_time::time_duration td = t2 - t1;

    std::cout << boost::posix_time::to_simple_string(td) << std::endl;
}
