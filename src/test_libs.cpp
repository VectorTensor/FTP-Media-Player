#include <iostream>

#include "lib/torrent_helper/torrent_helper.h"

int main() {
    std::cout<<"hello world"<<std::endl;
    lt::session session;
    start_torrent_download(session, "magnet:?xt=urn:btih:5cc0f0bdf3e22b1b257714b952d459389aa75f5c&dn=%5BToonsHub%5D%20Chainsmoker%20Cat%20S01E05%201080p%20NF%20WEB-DL%20AAC2.0%20H.264%20%28Yani%20Neko%2C%20Multi-Subs%2C%20Uncensored%29&tr=http%3A%2F%2Fnyaa.tracker.wf%3A7777%2Fannounce&tr=udp%3A%2F%2Fopen.stealth.si%3A80%2Fannounce&tr=udp%3A%2F%2Ftracker.opentrackr.org%3A1337%2Fannounce&tr=udp%3A%2F%2Fexodus.desync.com%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.torrent.eu.org%3A451%2Fannounce");
    return 0;

}
