#include <boost/date_time.hpp>
#include <iostream>

namespace bt = boost::posix_time;

const std::locale formats[] = {
    std::locale(std::locale::classic(), new bt::time_input_facet("%Y-%m-%d %H:%M:%S")),
    std::locale(std::locale::classic(), new bt::time_input_facet("%Y/%m/%d %H:%M:%S")),
    std::locale(std::locale::classic(), new bt::time_input_facet("%d.%m.%Y %H:%M:%S")),
    std::locale(std::locale::classic(), new bt::time_input_facet("%Y-%m-%d")),
    std::locale(std::locale::classic(), new bt::time_input_facet("%b %d %H:%M:%S%F")),
};

const size_t formats_n = sizeof(formats) / sizeof(formats[0]);

std::time_t pt_to_time_t(const bt::ptime &pt)
{
  bt::ptime timet_start(boost::gregorian::date(1970, 1, 1));
  bt::time_duration diff = pt - timet_start;
  return diff.ticks() / bt::time_duration::rep_type::ticks_per_second;
}

void seconds_from_epoch(const std::string &s)
{
  bt::ptime pt;

  for (size_t i = 0; i < formats_n; ++i) {

    std::istringstream is(s);
    is.imbue(formats[i]);
    is >> pt;

    if (pt != bt::ptime())
      break;
  }

  std::cout << " ptime is " << pt << '\n';
  std::cout << " seconds from epoch are " << pt_to_time_t(pt) << '\n';
}

void get_ptime(const std::string &s, bt::ptime& pt)
{
    for (size_t i = 0; i < formats_n; ++i) {

        std::istringstream is(s);
        is.imbue(formats[i]);
        is >> pt;

        if (pt != bt::ptime())
            break;
    }

    std::cout << " ptime is " << pt << '\n';
    std::cout << " seconds from epoch are " << pt_to_time_t(pt) << '\n';
}

int main() {
    /*
  seconds_from_epoch("2004-03-21 12:45:33");
  seconds_from_epoch("2004/03/21 12:45:33");
  seconds_from_epoch("23.09.2004 04:12:21");
  seconds_from_epoch("2003-02-11");
  seconds_from_epoch("Nov 12 18:38:38");
  seconds_from_epoch("Nov 12 15:49:59.999");
  seconds_from_epoch("Nov 12 15:50:00.000");
  seconds_from_epoch("Nov 12 15:50:00.001");
    */
    bt::ptime pt1;
    bt::ptime pt2;
    bt::ptime pt3;
    bt::ptime pt4;

    get_ptime("Nov 12 18:38:38", pt1);
    get_ptime("Nov 12 15:49:59.999", pt2);
    get_ptime("Nov 12 15:50:00.000", pt3);
    get_ptime("Nov 12 15:50:00.001", pt4);

    boost::posix_time::time_duration td = pt2 - pt1;
    boost::posix_time::time_duration td2 = pt3 - pt2;
    boost::posix_time::time_duration td3 = pt4 - pt3;

    std::cout << td << std::endl;
    std::cout << td2 << std::endl;
    std::cout << td3 << std::endl;

    long microseconds = td.total_microseconds();
    long microseconds2 = td2.total_microseconds();
    long microseconds3 = td3.total_microseconds();

    std::cout << microseconds << std::endl;
    std::cout << microseconds2 << std::endl;
    std::cout << microseconds3 << std::endl;
}
