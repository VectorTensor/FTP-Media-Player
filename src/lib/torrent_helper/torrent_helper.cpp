


#include <iostream>

#include "libtorrent/magnet_uri.hpp"
#include "libtorrent/session.hpp"

void start_torrent_download(lt::session& session, const std::string& url) {

    std::cout<< "url: " <<url << std::endl;

    lt::add_torrent_params atp = lt::parse_magnet_uri(url);

}
