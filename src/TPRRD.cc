#if 0
#include "ptmp/api.h"

#include "ptmp/factory.h"
#include "json.hpp"

using json = nlohmann::json;
PTMP_AGENT(ptmp::tcs::TPRRDFiller, rrdfiller)

static 
void rrdfiller(zsock_t* pipe, void* vargs)
{
    auto config = json::parse((const char*) vargs);
    std::string name = "rrdfiller";
    if (config["name"].is_string()) {
        name = config["name"];
    }
    ptmp::internals::set_thread_name(name);

    zsock_t* isock = NULL;
    if (!config["input"].is_null()) {
        std::string cfg = config["input"].dump();
        isock = ptmp::internals::endpoint(cfg);
        if (isock) {
            zsys_info("rrdfiller: isock: %s", cfg.c_str());
        }
    }    

    // rrdcached server address string
    std::string rrdaddr = "localhost";
    if (config["rrd_daemon"].is_string()) {
        rrdaddr = config["rrd_daemon"];
    }
    rrd_client_t * rrd = rrd_client_new(rrdaddr.c_str());

    std::string filename = "ptmp.rrd";
    if (config["rrd_filename"].is_string()) {
        filename = config["rrd_daemon"];
    }


    zsock_signal(pipe, 0); // signal ready    
    
    zpoller_t* poller = zpoller_new(pipe, isock, NULL);
    bool got_quit = false;
    while (!zsys_interrupted) {

        void* which = zpoller_wait(poller, -1);
        if (which == pipe) {
            got_quit = true;
            goto cleanup;
        }

        // isock
        zmsg_t* msg = zmsg_recv(isock);
        if (!msg) {
            zsys_info("tprrd: interrupted in recv");
            break;
        }

        ptmp::data::TPSet tpset;
        ptmp::internals::recv(&msg, tpset); // throws

        rrd_client_update(rrd, filename.c_str(),
                          nvalues, values)

    }

  cleanup:

    rrd_client_disconnect(rrd);
    rrd_client_destroy(rrd); rrd=NULL:
    zpoller_destroy(&poller);
    zsock_destroy(&isock);

    if (got_quit) {
        return;
    }
    zsys_debug("czmqat: waiting for quit");
    zsock_wait(pipe);    
}

#endif

