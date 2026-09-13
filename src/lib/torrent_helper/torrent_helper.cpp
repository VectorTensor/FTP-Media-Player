


#include <iostream>

#include "libtorrent/magnet_uri.hpp"
#include "libtorrent/session.hpp"

lt::torrent_handle start_torrent_download(lt::session& session, const std::string& url) {
    lt::add_torrent_params atp = lt::parse_magnet_uri(url);
    atp.save_path = ".";

    lt::torrent_handle h = session.add_torrent(atp);

    return h;
}
