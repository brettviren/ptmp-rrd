/*
 rrdtool create test.rrd -s 1 DS:link01:COUNTER:1:U:U RRA:AVERAGE:0.5:1s:10d RRA:AVERAGE:0.5:1m:90d RRA:AVERAGE:0.5:1h:18M RRA:AVERAGE:0.5:1h:10y 

$ ./build/test/check-rrdtool update test.rrd 1564598462:25
 */
#include <rrd.h>

#include <ctime>
#include <cstdlib>

int main(int argc, char* argv[])
{
    return rrd_update(argc-1, &argv[1]);
}
